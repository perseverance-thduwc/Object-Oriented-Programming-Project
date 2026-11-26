'use strict';
// pixel.js - Biến hình vẽ SVG thành PIXEL ART:
//   1) vẽ SVG vào canvas rất nhỏ (độ phân giải thấp)
//   2) cắt alpha (không còn viền mờ) và giảm số màu (posterize)
//   3) thêm viền đen 1 pixel quanh nhân vật
// Ảnh nhỏ được phóng to bằng CSS `image-rendering: pixelated` nên thấy rõ từng ô pixel.

const PIX = {
  cache: {},

  // size: [rộng, cao] của ảnh pixel (chưa tính viền 1px mỗi phía)
  SPEC: {
    hero0: ['Warrior', 38, 48], hero1: ['Mage', 38, 48], hero2: ['Archer', 38, 48],
    MinionWarrior: ['MinionWarrior', 36, 40], MinionMage: ['MinionMage', 36, 40],
    MinionArcher: ['MinionArcher', 36, 40], MinionSpitter: ['MinionSpitter', 36, 40],
    MinionBrute: ['MinionBrute', 40, 44], Boss: ['Boss', 62, 62],
    iconPotionHp: ['PotionIcon', 22, 28], iconPotionMp: ['PotionIcon', 22, 28, '#3b82f6', '#93c5fd', '#8a5a2b'],
    iconElixir: ['PotionIcon', 22, 28, '#a855f7', '#e9d5ff', '#f5c451'],
    iconGreat: ['PotionIcon', 22, 28, '#f43f5e', '#fecdd3', '#f5c451'],
    iconWeapon: ['WeaponIcon', 22, 28], iconArmor: ['ArmorIcon', 22, 28], iconSkill: ['SkillIcon', 22, 28]
  },

  svgToPixel(svg, w, h) {
    return new Promise(resolve => {
      const img = new Image();
      svg = svg.replace('<svg ', `<svg width="${w}" height="${h}" `);
      img.onload = () => {
        const W = w + 2, H = h + 2;                       // chừa 1px viền mỗi phía
        const c = document.createElement('canvas'); c.width = W; c.height = H;
        const ctx = c.getContext('2d');
        ctx.imageSmoothingEnabled = true;
        ctx.drawImage(img, 1, 1, w, h);
        const data = ctx.getImageData(0, 0, W, H), px = data.data;
        for (let i = 0; i < px.length; i += 4) {          // cắt alpha + giảm màu
          if (px[i + 3] < 110) { px[i + 3] = 0; continue; }
          px[i + 3] = 255;
          for (let k = 0; k < 3; k++) px[i + k] = Math.min(255, Math.round(px[i + k] / 51) * 51);
        }
        const out = new Uint8ClampedArray(px);            // viền đen quanh hình
        const A = (x, y) => (x < 0 || y < 0 || x >= W || y >= H) ? 0 : px[(y * W + x) * 4 + 3];
        for (let y = 0; y < H; y++) for (let x = 0; x < W; x++) {
          if (A(x, y) === 0 && (A(x - 1, y) || A(x + 1, y) || A(x, y - 1) || A(x, y + 1))) {
            const i = (y * W + x) * 4; out[i] = 12; out[i + 1] = 8; out[i + 2] = 28; out[i + 3] = 255;
          }
        }
        ctx.putImageData(new ImageData(out, W, H), 0, 0);
        resolve(c.toDataURL());
      };
      img.onerror = () => resolve('');
      img.src = 'data:image/svg+xml;charset=utf-8,' + encodeURIComponent(svg);
    });
  },

  // Vẽ trước toàn bộ sprite (async) rồi mới hiển thị game
  async preload() {
    const jobs = Object.keys(this.SPEC).map(async key => {
      const [fn, w, h, a, b, c] = this.SPEC[key];
      this.cache[key] = await this.svgToPixel(ART[fn](a, b, c), w, h);
    });
    await Promise.all(jobs);
  },

  get(key) { return this.cache[key] || ''; },

  // <img> pixel với chiều cao hiển thị cho trước
  img(key, heightPx, cls = '') {
    return `<img class="px ${cls}" src="${this.get(key)}" style="height:${heightPx}px" alt="" draggable="false">`;
  },

  hero(i, h, cls) { return this.img('hero' + i, h, cls); },
  enemy(e, h, cls) { return this.img(ART.enemyKind(e.name), h, cls); },

  // icon theo mô tả vật phẩm / kỹ năng
  icon(desc, h = 60) {
    let k = 'iconSkill';
    if (desc.indexOf('[Weapon]') === 0) k = 'iconWeapon';
    else if (desc.indexOf('[Armor]') === 0) k = 'iconArmor';
    else if (desc.indexOf('[Potion]') === 0) {
      k = desc.indexOf('Mana') >= 0 ? 'iconPotionMp' : desc.indexOf('Elixir') >= 0 ? 'iconElixir'
        : desc.indexOf('Great') >= 0 ? 'iconGreat' : 'iconPotionHp';
    }
    return this.img(k, h);
  },

  // ---------- nền pixel theo từng màn ----------
  THEMES: [
    { sky: ['#5fb3ff', '#7cc4ff', '#9fd6ff', '#c4e6ff'], hill: ['#3f9e4a', '#2f7d3a'], ground: ['#4caf50', '#43a047', '#6d4c41'] },
    { sky: ['#ff9966', '#ffb27a', '#ffc891', '#ffdfb0'], hill: ['#6a8f3a', '#557a2b'], ground: ['#7cb342', '#6fa339', '#6d4c41'] },
    { sky: ['#1b1740', '#2a2060', '#3a2c7a', '#4a3a92'], hill: ['#2a2450', '#1f1a40'], ground: ['#5b4a8a', '#4f3f7a', '#2f2650'] },
    { sky: ['#3a0d0d', '#5a1414', '#7a1c1c', '#a02a1a'], hill: ['#3a1410', '#2a0e0b'], ground: ['#8a3b22', '#7a3320', '#3a1a10'] },
    { sky: ['#1a0524', '#2e0a3c', '#4a1058', '#6b1a72'], hill: ['#2a0a30', '#1c0622'], ground: ['#6b2a5a', '#5a2250', '#2a1030'] }
  ],

  scene(floor) {
    const key = 'scene' + floor;
    if (this.cache[key]) return this.cache[key];
    const W = 192, H = 62, t = this.THEMES[Math.min(floor, 5) - 1];
    const c = document.createElement('canvas'); c.width = W; c.height = H;
    const g = c.getContext('2d');
    let seed = floor * 7919;
    const rnd = () => (seed = (seed * 16807) % 2147483647) / 2147483647;
    const horizon = 36;
    for (let y = 0; y < horizon; y++) {                   // bầu trời (dải màu bậc thang)
      g.fillStyle = t.sky[Math.min(3, Math.floor(y / (horizon / 4)))];
      g.fillRect(0, y, W, 1);
    }
    for (let i = 0; i < 40; i++) {                        // sao / mây pixel
      g.fillStyle = floor >= 3 ? '#fff8' : '#ffffffcc';
      const x = Math.floor(rnd() * W), y = Math.floor(rnd() * 22);
      g.fillRect(x, y, floor >= 3 ? 1 : 3 + Math.floor(rnd() * 4), 1);
    }
    for (let layer = 0; layer < 2; layer++) {             // đồi núi 2 lớp
      g.fillStyle = t.hill[layer];
      const phase = rnd() * 6;
      for (let x = 0; x < W; x++) {
        const hgt = 7 + layer * 4 + Math.round(5 * Math.sin(x / (12 - layer * 3) + phase) + 3 * Math.sin(x / 5 + phase * 2));
        g.fillRect(x, horizon - hgt + layer * 3, 1, hgt + 2);
      }
    }
    for (let y = horizon; y < H; y++) {                   // nền đất caro
      for (let x = 0; x < W; x += 6) {
        g.fillStyle = t.ground[((x / 6 | 0) + y) % 2 === 0 ? 0 : 1];
        g.fillRect(x, y, 6, 1);
      }
    }
    g.fillStyle = t.ground[2]; g.fillRect(0, horizon, W, 1);
    return (this.cache[key] = c.toDataURL());
  }
};
