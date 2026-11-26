'use strict';
// game.js - UI only. It contains NO game rules: it sends commands to the C++ core
// (through server.py) and redraws the JSON state it gets back.

const ui = { started: false, pending: null, menu: null, log: [], busy: false, error: '' };
let S = null;       // current state (JSON from C++)

// ---------------------------------------------------------------- communication
async function api(cmd) {
  const r = await fetch('/api/cmd', {
    method: 'POST', headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify({ cmd })
  });
  if (!r.ok) throw new Error('HTTP ' + r.status);
  return r.json();
}

async function act(cmd) {
  if (ui.busy) return;
  ui.busy = true;
  try {
    const prev = S;
    const d = await api(cmd);
    if (cmd === 'NEW') ui.log = [];
    S = d;
    addLog(d.log);
    const errs = d.log.split('\n').filter(l => l.indexOf('[Error]') >= 0);
    ui.error = errs.length ? errs[errs.length - 1].replace('[Error]', '').trim() : '';
    ui.started = true; ui.pending = null; ui.menu = null;
    render();
    playFx(prev, S);
  } catch (e) {
    ui.error = 'Lost connection to the C++ server (' + e.message + '). Please restart server.py.';
    render();
  }
  ui.busy = false;
}

function addLog(text) {
  (text || '').split('\n').filter(l => l.trim()).forEach(l => ui.log.push(l.trim()));
  if (ui.log.length > 120) ui.log = ui.log.slice(-120);
}

// ---------------------------------------------------------------- helpers
const esc = s => String(s).replace(/[&<>"]/g, c => ({ '&': '&amp;', '<': '&lt;', '>': '&gt;', '"': '&quot;' }[c]));
const pct = (v, m) => Math.max(0, Math.min(100, m > 0 ? Math.round(v * 100 / m) : 0));
const livingEnemies = () => S.enemies.map((e, i) => e.alive ? i : -1).filter(i => i >= 0);

function badges(f) {
  let h = f.effects.map(t => {
    const k = t.indexOf('POISON') === 0 ? ['psn', 'PSN'] : t.indexOf('STUN') === 0 ? ['stn', 'STN'] : ['rgn', 'RGN'];
    return `<span class="tag ${k[0]}" title="${esc(t)}">${k[1]}</span>`;
  }).join('');
  if (f.defending) h += '<span class="tag def" title="Defending: damage halved">DEF</span>';
  return h;
}

function bars(f) {
  return `<div class="bar hp ${pct(f.hp, f.maxHp) < 30 ? 'low' : ''}"><i style="width:${pct(f.hp, f.maxHp)}%"></i><b>HP ${f.hp}/${f.maxHp}</b></div>
          <div class="bar mp"><i style="width:${pct(f.mp, f.maxMp)}%"></i><b>MP ${f.mp}</b></div>`;
}

const statLine = f => `ATK ${f.atk} · ARMOR ${f.armor} · CRIT ${f.crit}% · MISS ${f.miss}%`;

function fighter(id, f, sprite, extra = '') {
  const clickable = extra.indexOf('target') >= 0;
  return `<div class="fighter ${f.alive ? '' : 'dead'} ${f.defending ? 'defending' : ''} ${extra}" data-id="${id}" ${clickable ? `data-act="target:${id.slice(1)}"` : ''}>
    <div class="tags">${badges(f)}</div>
    <div class="sprite">${sprite}</div>
    <div class="name">${esc(f.name)}${f.alive ? '' : ' ✝'}</div>
    ${bars(f)}
    <div class="stats">ARMOR ${f.armor} · ATK ${f.atk}</div>
  </div>`;
}

// ---------------------------------------------------------------- screens
function titleScreen() {
  return `<div class="title">
    <h1>ARENA RPG</h1>
    <p class="sub">Object-Oriented Programming Project — PHY10609 / PET10001</p>
    <div class="cast">
      ${PIX.hero(0, 150)}${PIX.hero(1, 150)}${PIX.hero(2, 150)}${PIX.img('Boss', 230)}
    </div>
    <button class="pbtn big" data-act="start">▶ PRESS START</button>
    <p class="foot">The whole game runs in <b>C++ OOP</b> — this page only draws it and sends commands.</p>
  </div>`;
}

function topbar() {
  let dots = '';
  for (let f = 1; f <= S.maxFloor; f++) {
    dots += `<span class="dot ${f <= S.cleared ? 'done' : ''} ${f === S.floor && S.phase !== 'over' ? 'now' : ''}">${f === S.maxFloor ? 'B' : f}</span>`;
  }
  const turn = S.phase === 'battle' ? ` · TURN ${S.turn}` : '';
  const alive = S.heroes.filter(h => h.alive).length;
  return `<header class="top pix"><div class="brand">ARENA RPG</div>
    <div class="floors">${dots}</div>
    <div class="info">FLOOR ${Math.min(S.floor, S.maxFloor)}/${S.maxFloor}${turn} · HEROES ${alive}/3</div></header>`;
}

function chooseHeroScreen() {
  const first = S.active < 0;
  const cards = S.heroes.map((h, i) => `
    <div class="card hero pix ${h.alive ? 'pick' : 'disabled'}" data-id="h${i}" data-act="hero:${i}">
      <div class="sprite">${PIX.hero(i, 150)}</div>
      <h3>${esc(h.name)}${h.alive ? '' : ' ✝'}</h3>
      ${bars(h)}
      <div class="stats">${statLine(h)}</div>
      <ul class="skills">${h.skills.map((s, k) => `<li><b>${esc(s.name)}</b> <small>MP ${s.mp} · ${k === 0 ? 'common' : 'unique'}</small></li>`).join('')}</ul>
      <div class="muted">${h.items.length} item${h.items.length === 1 ? '' : 's'}</div>
    </div>`).join('');
  return `<main class="center">
    <h2>${first ? 'Choose your hero' : 'Your hero has fallen!'}</h2>
    <p class="muted">${first
      ? 'You fight with ONE hero until it dies. Then you choose another. If all 3 fall, it is game over.'
      : `Choose the next hero to continue the fight on floor ${S.floor}.`}</p>
    <div class="cards">${cards}</div>${first ? '' : logPanel(6)}</main>`;
}

function battleScreen() {
  const hero = S.heroes[S.active];
  const targeting = !!ui.pending;
  const boss = S.floor === S.maxFloor;
  const enemies = S.enemies.map((e, i) => {
    const h = e.boss ? 230 : boss ? 92 : 120;
    return fighter('e' + i, e, PIX.enemy(e, h), (e.boss ? 'boss ' : boss ? 'small ' : '') + (targeting && e.alive ? 'target' : ''));
  }).join('');

  const party = S.heroes.map((h, i) => `<div class="mini ${i === S.active ? 'on' : ''} ${h.alive ? '' : 'dead'}">
      ${PIX.hero(i, 46)}<div><b>${esc(h.name)}</b><br><small>HP ${h.hp}/${h.maxHp}</small></div></div>`).join('');

  let actions;
  if (targeting) {
    actions = `<div class="alert">TARGET: ${esc(ui.pending.label)} — click an enemy</div>
               <button class="pbtn" data-act="cancel">Cancel</button>`;
  } else if (ui.menu === 'skills') {
    actions = hero.skills.map((s, k) => `<button class="pbtn skill ${hero.mp < s.mp ? 'disabled' : ''}" data-act="skill:${k}">
        ${esc(s.name)}<small>MP ${s.mp} · ${k === 0 ? 'common' : 'unique'}</small></button>`).join('')
      + '<button class="pbtn ghost" data-act="cancel">← Back</button>';
  } else if (ui.menu === 'items') {
    actions = (hero.items.length ? hero.items.map((it, k) => `<button class="pbtn item" data-act="item:${k}">
        ${PIX.icon(it.desc, 34)}<span>${esc(it.desc)}${k === hero.weapon || k === hero.armorItem ? ' <b>(equipped)</b>' : ''}</span></button>`).join('')
      : '<div class="muted">Inventory is empty.</div>') + '<button class="pbtn ghost" data-act="cancel">← Back</button>';
  } else {
    actions = `<button class="pbtn" data-act="attack">ATTACK</button>
      <button class="pbtn" data-act="skillmenu">SKILLS</button>
      <button class="pbtn" data-act="itemmenu">ITEMS</button>
      <button class="pbtn def" data-act="defend">DEFEND</button>`;
  }

  return `<main class="battle">
    <section class="stage pix" style="background-image:url(${PIX.scene(S.floor)})">
      <div class="heroSide">${fighter('h' + S.active, hero, PIX.hero(S.active, 175), 'active')}</div>
      <div class="vs">VS</div>
      <div class="enemySide ${boss ? 'bossfloor' : ''}">${enemies}</div>
    </section>
    <section class="panel">
      <div class="pix bench"><h4>Party</h4>${party}</div>
      <div class="pix actions"><h4>${esc(hero.name)}'s turn</h4>
        <div class="stats">${statLine(hero)}</div>
        <div class="abuttons">${actions}</div></div>
      ${logPanel(10)}
    </section></main>`;
}

function rewardTypeScreen() {
  const hero = S.heroes[S.active];
  return `<main class="center"><h2>FLOOR ${S.floor} CLEARED!</h2>
    <p class="muted">${esc(hero.name)} catches their breath. Choose your reward:</p>
    <div class="cards two">
      <div class="card pix pick" data-act="reward:1"><div class="icon">${PIX.img('iconPotionHp', 96)}${PIX.img('iconWeapon', 96)}${PIX.img('iconArmor', 96)}</div>
        <h3>ITEMS</h3><p>Potions, weapons and armor. Pick 1 of 3.</p></div>
      <div class="card pix ${S.canLearn ? 'pick' : 'disabled'}" data-act="reward:2"><div class="icon">${PIX.img('iconSkill', 96)}</div>
        <h3>SKILLS</h3><p>${S.canLearn ? 'Learn a new skill. Pick 1 of 3.' : 'No new skills available.'}</p></div>
    </div>${logPanel(5)}</main>`;
}

function rewardPickScreen() {
  const isSkill = S.rewardType === 2;
  const cards = S.offers.map((d, i) => {
    const m = d.match(/^(?:\[\w+\] )?(.+?) \((.*)\)$/);
    const name = m ? m[1] : d, info = m ? m[2] : '';
    const kind = isSkill ? 'SKILL' : (d.match(/^\[(\w+)\]/) || [0, 'ITEM'])[1].toUpperCase();
    return `<div class="card pix pick" data-act="pick:${i}"><div class="icon">${PIX.icon(isSkill ? name : d, 110)}</div>
      <small class="muted">${kind}</small><h3>${esc(name)}</h3><p>${esc(info)}</p></div>`;
  }).join('');
  return `<main class="center"><h2>Pick one ${isSkill ? 'skill' : 'item'}</h2>
    <div class="cards">${cards}</div>
    <button class="pbtn ghost" data-act="back">← Back</button>${logPanel(3)}</main>`;
}

function overScreen() {
  const r = S.result;
  return `<main class="center over ${r.win ? 'win' : 'lose'}">
    <h1>${r.win ? 'VICTORY!' : 'GAME OVER'}</h1>
    <div class="letter">${esc(r.letter)}</div>
    <table class="result pix">
      <tr><td>Floors cleared</td><td>${S.cleared}/${S.maxFloor}</td></tr>
      <tr><td>Heroes alive</td><td>${r.alive}/3</td></tr>
      <tr><td>Raw score</td><td>${r.raw}</td></tr>
      <tr><td>Curved score (+10%)</td><td>${r.score}</td></tr>
    </table>
    <p class="muted">Score is computed by the C++ class <code>CurvedActivity</code> (redefining <code>setScore</code>).</p>
    <button class="pbtn big" data-act="again">↻ PLAY AGAIN</button>${logPanel(6)}</main>`;
}

function logPanel(n) {
  const lines = ui.log.slice(-n).map(l => {
    let c = '';
    if (/CRITICAL/.test(l)) c = 'crit'; else if (/MISS/.test(l)) c = 'miss';
    else if (/POISON/.test(l)) c = 'poison'; else if (/STUN/.test(l)) c = 'stun';
    else if (/REGEN|DRAIN|recovers/.test(l)) c = 'regen'; else if (/\[Error\]/.test(l)) c = 'err';
    else if (/cleared|BOSS|Floor \d|fallen/.test(l)) c = 'big'; else if (/\[SKILL\]|\[LEARN\]|\[REWARD\]/.test(l)) c = 'skill';
    return `<div class="${c}">${esc(l)}</div>`;
  }).join('');
  return `<div class="log pix"><h4>Battle log</h4><div class="lines">${lines}</div></div>`;
}

// ---------------------------------------------------------------- rendering
function render() {
  const app = document.getElementById('app');
  if (!ui.started) { app.innerHTML = titleScreen(); return; }
  const screens = { choose_hero: chooseHeroScreen, battle: battleScreen, reward_type: rewardTypeScreen,
                    reward_pick: rewardPickScreen, over: overScreen };
  app.innerHTML = topbar() + screens[S.phase]() +
    `<footer>Entities alive — <code>Character::totalCharacters</code> = <b>${S.entities}</b></footer>` +
    (ui.error ? `<div class="toast">${esc(ui.error)}</div>` : '');
  const lines = app.querySelector('.log .lines');
  if (lines) lines.scrollTop = lines.scrollHeight;
}

// ---------------------------------------------------------------- floating numbers
// Reads the C++ combat log of the last command and pops up DMG / CRIT / MISS / heal numbers.
function playFx(prev, cur) {
  if (!cur || cur.phase !== 'battle') return;     // only animate inside a fight
  const ids = {};                                    // entity name -> element id
  cur.heroes.forEach((h, i) => { ids[h.name] = 'h' + i; });
  cur.enemies.forEach((e, i) => { ids[e.name] = 'e' + i; });
  const names = Object.keys(ids).sort((a, b) => b.length - a.length);
  const who = text => names.find(n => text.endsWith(n));      // target = last name in the text
  const sums = {};
  const events = [];
  const add = (name, text, cls, delta) => {
    if (!name || !ids[name]) return;
    events.push({ id: ids[name], text, cls });
    sums[name] = (sums[name] || 0) + delta;
  };

  cur.log.split('\n').forEach(line => {
    let m;
    if ((m = line.match(/-> (.+?): MISS!/))) add(who(m[1]), 'MISS', 'miss', 0);
    else if ((m = line.match(/-> (.+?): loses (\d+) HP( \(CRITICAL!\))?/))) {
      const crit = !!m[3];
      add(who(m[1]), (crit ? 'CRIT -' : '-') + m[2], crit ? 'crit' : 'dmg', -parseInt(m[2], 10));
    }
    else if ((m = line.match(/\[POISON\] (.+?) is poisoned and loses (\d+) HP/))) add(m[1], '-' + m[2], 'psn', -parseInt(m[2], 10));
    else if ((m = line.match(/\[(?:REGEN|DRAIN)\] (.+?) recovers (\d+) HP/))) add(m[1], '+' + m[2], 'heal', parseInt(m[2], 10));
    else if ((m = line.match(/\[ITEM\] (.+?) uses .*\(\+(\d+) HP, \+(\d+) MP\)/))) {
      if (+m[2] > 0) add(m[1], '+' + m[2] + ' HP', 'heal', +m[2]);
      if (+m[3] > 0) add(m[1], '+' + m[3] + ' MP', 'mp', 0);
    }
    else if ((m = line.match(/\[DEFEND\] (.+?) braces/))) add(m[1], 'DEFEND', 'def', 0);
  });

  // leftover HP change not explained by the log (e.g. resting between floors)
  if (prev) cur.heroes.forEach((h, i) => {
    const d = (h.hp - prev.heroes[i].hp) - (sums[h.name] || 0);
    if (d !== 0) events.push({ id: 'h' + i, text: (d > 0 ? '+' : '') + d, cls: d > 0 ? 'heal' : 'dmg' });
  });

  events.forEach((ev, k) => setTimeout(() => floatAt(ev), k * 420));
}

function floatAt(ev) {
  const el = document.querySelector('[data-id="' + ev.id + '"]');
  if (!el) return;
  const s = document.createElement('div');
  s.className = 'float ' + ev.cls;
  s.textContent = ev.text;
  s.style.marginLeft = (Math.round(Math.random() * 40) - 20) + 'px';
  el.appendChild(s);
  if (ev.cls === 'dmg' || ev.cls === 'crit' || ev.cls === 'psn') {
    el.classList.add('hit'); setTimeout(() => el.classList.remove('hit'), 380);
  }
  setTimeout(() => s.remove(), 1500);
}

// ---------------------------------------------------------------- events
document.addEventListener('click', ev => {
  const el = ev.target.closest('[data-act]');
  if (!el || ui.busy || el.classList.contains('disabled')) return;
  const [a, v] = el.dataset.act.split(':');
  const n = parseInt(v, 10);

  const pickTarget = (label, build) => {
    const alive = livingEnemies();
    if (alive.length === 1) { act(build(alive[0])); return; }
    ui.pending = { label, build }; ui.menu = null; render();
  };

  switch (a) {
    case 'start': case 'again': act('NEW'); break;
    case 'hero': act('HERO ' + n); break;
    case 'attack': pickTarget('Attack', j => 'ATTACK ' + j); break;
    case 'skillmenu': ui.menu = 'skills'; render(); break;
    case 'itemmenu': ui.menu = 'items'; render(); break;
    case 'skill': pickTarget(S.heroes[S.active].skills[n].name, j => 'SKILL ' + n + ' ' + j); break;
    case 'item': act('ITEM ' + n); break;
    case 'defend': act('DEFEND'); break;
    case 'target': if (ui.pending) act(ui.pending.build(n)); break;
    case 'cancel': ui.pending = null; ui.menu = null; render(); break;
    case 'reward': act('REWARD ' + n); break;
    case 'pick': act('PICK ' + n); break;
    case 'back': act('BACK'); break;
  }
});

// Preload the pixel sprites, then show the title screen
document.getElementById('app').innerHTML = '<div class="loading">LOADING...</div>';
PIX.preload().then(render);
