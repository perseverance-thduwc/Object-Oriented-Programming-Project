'use strict';
// art.js - All character artwork drawn as inline SVG (no external images).

const ART = {
  shadow: '<ellipse cx="60" cy="144" rx="34" ry="6" fill="rgba(0,0,0,.35)"/>',

  // ---------- HERO ----------
  Warrior() {
    return `<svg viewBox="0 0 120 150" xmlns="http://www.w3.org/2000/svg">${this.shadow}
      <rect x="42" y="104" width="15" height="36" rx="5" fill="#33405f"/>
      <rect x="63" y="104" width="15" height="36" rx="5" fill="#33405f"/>
      <rect x="38" y="58" width="44" height="52" rx="12" fill="#5d7fb8" stroke="#27385c" stroke-width="3"/>
      <rect x="38" y="92" width="44" height="8" fill="#c9962e"/>
      <circle cx="60" cy="42" r="19" fill="#f2c9a0" stroke="#7a5a3a" stroke-width="2"/>
      <path d="M39 42 Q39 12 60 12 Q81 12 81 42 L74 42 Q74 22 60 22 Q46 22 46 42 Z" fill="#aab6cc" stroke="#4b5568" stroke-width="2"/>
      <rect x="57" y="6" width="6" height="14" rx="3" fill="#d9534f"/>
      <circle cx="53" cy="44" r="2.6" fill="#222"/><circle cx="67" cy="44" r="2.6" fill="#222"/>
      <path d="M53 52 Q60 56 67 52" stroke="#7a3b2e" stroke-width="2" fill="none"/>
      <g transform="translate(8 66)"><path d="M0 0 H30 V26 Q30 46 15 52 Q0 46 0 26 Z" fill="#b8352f" stroke="#5e1815" stroke-width="3"/>
      <path d="M15 8 V44 M4 22 H26" stroke="#f5c451" stroke-width="4"/></g>
      <g transform="rotate(25 92 80)"><rect x="89" y="22" width="7" height="62" rx="2" fill="#dfe6ee" stroke="#6b7685" stroke-width="2"/>
      <rect x="82" y="82" width="21" height="6" rx="2" fill="#c9962e"/><rect x="90" y="88" width="5" height="14" rx="2" fill="#6b3f1d"/></g>
    </svg>`;
  },

  Mage() {
    return `<svg viewBox="0 0 120 150" xmlns="http://www.w3.org/2000/svg">${this.shadow}
      <path d="M60 52 L92 140 H28 Z" fill="#6b4ab8" stroke="#33206b" stroke-width="3"/>
      <path d="M60 70 V140" stroke="#8c6be0" stroke-width="3"/>
      <rect x="40" y="96" width="40" height="7" fill="#f5c451"/>
      <circle cx="60" cy="46" r="17" fill="#f2c9a0" stroke="#7a5a3a" stroke-width="2"/>
      <circle cx="54" cy="47" r="2.4" fill="#222"/><circle cx="66" cy="47" r="2.4" fill="#222"/>
      <path d="M52 58 Q60 70 68 58 Q60 62 52 58 Z" fill="#e8e8f0" stroke="#aaa" stroke-width="1.5"/>
      <path d="M34 40 H86 L74 36 Q70 4 62 -2 Q56 4 46 36 Z" fill="#7d5bd0" stroke="#33206b" stroke-width="3" transform="translate(0 6)"/>
      <ellipse cx="60" cy="44" rx="29" ry="5" fill="#5238a0" stroke="#33206b" stroke-width="2"/>
      <circle cx="62" cy="14" r="3" fill="#f5c451"/>
      <rect x="92" y="40" width="5" height="100" rx="2" fill="#8a5a2b"/>
      <circle cx="94.5" cy="34" r="10" fill="#7be0ff" stroke="#2b7a99" stroke-width="2"/>
      <circle cx="94.5" cy="34" r="16" fill="rgba(123,224,255,.25)"/>
    </svg>`;
  },

  Archer() {
    return `<svg viewBox="0 0 120 150" xmlns="http://www.w3.org/2000/svg">${this.shadow}
      <rect x="44" y="104" width="13" height="36" rx="5" fill="#4b3a26"/>
      <rect x="63" y="104" width="13" height="36" rx="5" fill="#4b3a26"/>
      <rect x="40" y="58" width="40" height="50" rx="10" fill="#4f9a5a" stroke="#244d2b" stroke-width="3"/>
      <rect x="40" y="90" width="40" height="7" fill="#8a5a2b"/>
      <circle cx="60" cy="42" r="18" fill="#f2c9a0" stroke="#7a5a3a" stroke-width="2"/>
      <path d="M38 44 Q38 12 60 12 Q82 12 82 44 Q70 30 60 30 Q50 30 38 44 Z" fill="#3c7d46" stroke="#244d2b" stroke-width="2"/>
      <path d="M82 20 L96 8 L88 26 Z" fill="#d9534f"/>
      <circle cx="53" cy="44" r="2.5" fill="#222"/><circle cx="67" cy="44" r="2.5" fill="#222"/>
      <path d="M53 52 Q60 56 67 52" stroke="#7a3b2e" stroke-width="2" fill="none"/>
      <path d="M96 30 Q120 80 96 130" stroke="#8a5a2b" stroke-width="5" fill="none" stroke-linecap="round"/>
      <path d="M96 30 L96 130" stroke="#eee" stroke-width="1.5"/>
      <rect x="72" y="78" width="30" height="3" fill="#ddd"/><path d="M72 79.5 L66 76 V83 Z" fill="#aaa"/>
      <rect x="30" y="60" width="9" height="34" rx="3" fill="#6b3f1d"/>
    </svg>`;
  },

  // ---------- QUÁI NHỎ (vi khuẩn amoeba) ----------
  blob: 'M60 14 C82 6 108 28 104 58 C101 88 84 112 58 110 C30 108 8 90 14 60 C17 36 38 22 60 14 Z',
  eyes() {
    return `<circle cx="46" cy="58" r="9" fill="#fff"/><circle cx="74" cy="58" r="9" fill="#fff"/>
      <circle cx="48" cy="60" r="4.5" fill="#222"/><circle cx="72" cy="60" r="4.5" fill="#222"/>
      <path d="M38 48 L54 54 M82 48 L66 54" stroke="#5b1030" stroke-width="3"/>
      <path d="M48 80 Q60 90 72 80" stroke="#5b1030" stroke-width="3" fill="none"/>`;
  },
  MinionWarrior() {
    return `<svg viewBox="0 0 120 130" xmlns="http://www.w3.org/2000/svg">
      <ellipse cx="60" cy="122" rx="38" ry="6" fill="rgba(0,0,0,.35)"/>
      <path d="${this.blob}" fill="#ff6fa5" stroke="#a02a5e" stroke-width="3"/>
      <circle cx="30" cy="40" r="6" fill="#ff8fb9"/><circle cx="92" cy="86" r="7" fill="#ff8fb9"/>
      <path d="M36 22 L40 6 L48 20 M72 20 L80 6 L84 24" fill="#c9c9d6" stroke="#6a6a7a" stroke-width="2"/>
      ${this.eyes()}
    </svg>`;
  },
  MinionMage() {
    return `<svg viewBox="0 0 120 130" xmlns="http://www.w3.org/2000/svg">
      <ellipse cx="60" cy="122" rx="38" ry="6" fill="rgba(0,0,0,.35)"/>
      <path d="${this.blob}" fill="#b58cff" stroke="#4d2a9a" stroke-width="3"/>
      <circle cx="28" cy="70" r="6" fill="#c9aaff"/><circle cx="94" cy="40" r="7" fill="#c9aaff"/>
      <path d="M36 28 H84 L66 26 L60 -2 L54 26 Z" fill="#6a3fd0" stroke="#33206b" stroke-width="2.5" transform="translate(0 4)"/>
      <circle cx="60" cy="8" r="3" fill="#f5c451"/>
      ${this.eyes()}
      <circle cx="104" cy="96" r="7" fill="#7be0ff"/>
    </svg>`;
  },

  MinionArcher() {
    return `<svg viewBox="0 0 120 130" xmlns="http://www.w3.org/2000/svg">
      <ellipse cx="60" cy="122" rx="38" ry="6" fill="rgba(0,0,0,.35)"/>
      <path d="${this.blob}" fill="#7bdc6b" stroke="#2c7a2c" stroke-width="3"/>
      <circle cx="30" cy="40" r="6" fill="#a5f08f"/><circle cx="92" cy="86" r="7" fill="#a5f08f"/>
      <path d="M22 34 Q60 14 98 34 L98 44 Q60 26 22 44 Z" fill="#d9433a" stroke="#7a1b16" stroke-width="2"/>
      <path d="M104 30 Q130 66 104 106" stroke="#8a5a2b" stroke-width="6" fill="none" stroke-linecap="round"/>
      <path d="M104 30 L104 106" stroke="#eee" stroke-width="2"/>
      ${this.eyes()}
    </svg>`;
  },
  MinionSpitter() {
    return `<svg viewBox="0 0 120 130" xmlns="http://www.w3.org/2000/svg">
      <ellipse cx="60" cy="122" rx="38" ry="6" fill="rgba(0,0,0,.35)"/>
      <path d="${this.blob}" fill="#d4f542" stroke="#6b7a10" stroke-width="3"/>
      <circle cx="30" cy="70" r="6" fill="#e8ff80"/><circle cx="92" cy="40" r="7" fill="#e8ff80"/>
      <circle cx="46" cy="56" r="9" fill="#fff"/><circle cx="74" cy="56" r="9" fill="#fff"/>
      <circle cx="48" cy="58" r="4.5" fill="#222"/><circle cx="72" cy="58" r="4.5" fill="#222"/>
      <path d="M40 44 L54 50 M80 44 L66 50" stroke="#3b4a00" stroke-width="3"/>
      <ellipse cx="60" cy="86" rx="14" ry="10" fill="#3b4a00"/>
      <path d="M52 94 Q52 112 58 112 Q64 112 62 94 Z" fill="#33ff66"/>
      <circle cx="60" cy="118" r="5" fill="#33ff66"/>
    </svg>`;
  },
  MinionBrute() {
    return `<svg viewBox="0 0 120 130" xmlns="http://www.w3.org/2000/svg">
      <ellipse cx="60" cy="122" rx="44" ry="6" fill="rgba(0,0,0,.35)"/>
      <path d="M60 10 C92 2 116 30 112 62 C110 94 90 116 58 114 C26 112 4 92 10 60 C14 32 34 16 60 10 Z" fill="#9b7653" stroke="#4a3320" stroke-width="3.5"/>
      <path d="M22 44 Q60 26 98 44 L98 56 Q60 40 22 56 Z" fill="#6b7a99" stroke="#2f3a52" stroke-width="3"/>
      <path d="M24 92 Q60 108 96 92 L96 102 Q60 118 24 102 Z" fill="#6b7a99" stroke="#2f3a52" stroke-width="3"/>
      <path d="M30 18 L36 -2 L46 16 M74 16 L84 -2 L90 18" fill="#e5ecf4" stroke="#6b7685" stroke-width="2.5"/>
      <circle cx="44" cy="66" r="8" fill="#fff"/><circle cx="76" cy="66" r="8" fill="#fff"/>
      <circle cx="46" cy="68" r="4" fill="#b00020"/><circle cx="74" cy="68" r="4" fill="#b00020"/>
      <path d="M32 56 L54 62 M88 56 L66 62" stroke="#2a1a0e" stroke-width="4"/>
      <path d="M44 86 Q60 96 76 86" stroke="#2a1a0e" stroke-width="4" fill="none"/>
    </svg>`;
  },

  // ---------- BOSS ----------
  Boss() {
    return `<svg viewBox="0 0 200 200" xmlns="http://www.w3.org/2000/svg">
      <ellipse cx="100" cy="190" rx="62" ry="8" fill="rgba(0,0,0,.4)"/>
      <g class="spin"><circle cx="100" cy="100" r="92" fill="none" stroke="#ffd166" stroke-width="3" stroke-dasharray="10 12" opacity=".8"/>
      <circle cx="100" cy="8" r="6" fill="#ffd166"/></g>
      <path d="M100 26 C128 12 168 38 170 78 C186 92 184 124 164 136 C166 164 136 180 108 176 C86 190 48 178 40 150 C16 140 14 102 34 90 C28 54 62 22 100 26 Z"
            fill="#e0357a" stroke="#6b0f3a" stroke-width="4"/>
      <path d="M34 90 C14 80 6 60 18 48 C26 56 30 70 40 74" fill="#e0357a" stroke="#6b0f3a" stroke-width="3"/>
      <path d="M164 136 C184 144 192 164 178 172 C172 160 166 152 158 150" fill="#e0357a" stroke="#6b0f3a" stroke-width="3"/>
      <circle cx="62" cy="120" r="9" fill="#ff7aa8"/><circle cx="140" cy="146" r="11" fill="#ff7aa8"/><circle cx="150" cy="66" r="8" fill="#ff7aa8"/>
      <path d="M66 42 L74 14 L88 34 L100 8 L112 34 L126 14 L134 42 Z" fill="#ffd166" stroke="#a8741a" stroke-width="3"/>
      <circle cx="100" cy="10" r="4" fill="#ff4d4d"/>
      <circle cx="76" cy="88" r="17" fill="#fff"/><circle cx="124" cy="88" r="17" fill="#fff"/>
      <circle cx="80" cy="92" r="8" fill="#b00020"/><circle cx="120" cy="92" r="8" fill="#b00020"/>
      <circle cx="82" cy="94" r="3.5" fill="#000"/><circle cx="118" cy="94" r="3.5" fill="#000"/>
      <path d="M56 70 L92 82 M144 70 L108 82" stroke="#33000f" stroke-width="6" stroke-linecap="round"/>
      <path d="M70 126 Q100 152 130 126 Q100 134 70 126 Z" fill="#33000f"/>
      <path d="M80 128 L84 138 L90 130 M110 130 L116 138 L120 128" fill="#fff"/>
    </svg>`;
  },

  // ---------- ICON ----------
  PotionIcon(c1 = '#e0524a', c2 = '#ff8a80', cap = '#8a5a2b') {
    return `<svg viewBox="0 0 80 100" xmlns="http://www.w3.org/2000/svg">
      <rect x="30" y="6" width="20" height="14" rx="3" fill="${cap}"/>
      <path d="M32 20 H48 V36 Q70 50 68 74 Q66 94 40 94 Q14 94 12 74 Q10 50 32 36 Z" fill="${c1}" stroke="#222" stroke-width="3"/>
      <path d="M18 66 Q40 54 62 66 Q64 90 40 90 Q16 90 18 66 Z" fill="${c2}"/>
      <circle cx="32" cy="72" r="4" fill="#fff" opacity=".7"/></svg>`;
  },
  ArmorIcon() {
    return `<svg viewBox="0 0 80 100" xmlns="http://www.w3.org/2000/svg">
      <path d="M10 14 H70 V52 Q70 82 40 96 Q10 82 10 52 Z" fill="#6b8fd1" stroke="#1d2c52" stroke-width="4"/>
      <path d="M40 14 V96" stroke="#1d2c52" stroke-width="3"/><path d="M10 40 H70" stroke="#1d2c52" stroke-width="3"/>
      <path d="M40 24 L46 36 L58 36 L48 44 L52 56 L40 48 L28 56 L32 44 L22 36 L34 36 Z" fill="#f5c451"/></svg>`;
  },
  SkillIcon() {
    return `<svg viewBox="0 0 80 100" xmlns="http://www.w3.org/2000/svg">
      <rect x="14" y="10" width="52" height="80" rx="6" fill="#e8d5a3" stroke="#6b4a1d" stroke-width="4"/>
      <path d="M40 22 L47 40 L66 40 L51 52 L57 72 L40 60 L23 72 L29 52 L14 40 L33 40 Z" fill="#8e5bd6" stroke="#33206b" stroke-width="3" transform="scale(.8) translate(10 6)"/></svg>`;
  },
  WeaponIcon() {
    return `<svg viewBox="0 0 80 100" xmlns="http://www.w3.org/2000/svg">
      <rect x="36" y="4" width="8" height="64" rx="2" fill="#e5ecf4" stroke="#6b7685" stroke-width="2.5"/>
      <path d="M36 4 L40 -2 L44 4 Z" fill="#e5ecf4"/>
      <rect x="22" y="66" width="36" height="9" rx="3" fill="#c9962e" stroke="#7a5a12" stroke-width="2"/>
      <rect x="36" y="75" width="8" height="20" rx="2" fill="#6b3f1d"/>
      <circle cx="40" cy="97" r="4" fill="#c9962e"/></svg>`;
  },

  hero(index) { return this[['Warrior', 'Mage', 'Archer'][index]](); },
  enemy(e) {
    return this[ART.enemyKind(e.name)]();
  },
  // Ten quai (tu loi C++) -> ten ham ve
  enemyKind(name) {
    if (name.indexOf('AMOEBA') === 0) return 'Boss';
    if (name.indexOf('Germ Mage') === 0) return 'MinionMage';
    if (name.indexOf('Germ Archer') === 0) return 'MinionArcher';
    if (name.indexOf('Acid Spitter') === 0) return 'MinionSpitter';
    if (name.indexOf('Shell Brute') === 0) return 'MinionBrute';
    return 'MinionWarrior';
  }
};
