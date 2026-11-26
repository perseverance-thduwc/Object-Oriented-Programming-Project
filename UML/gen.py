# -*- coding: utf-8 -*-
import html, subprocess

def esc(s): return html.escape(s, quote=False)

def fmt(line):
    """line: (text, flags) flags: s=static(underline) a=abstract(italic)"""
    t, fl = line
    t = esc(t)
    if 's' in fl: t = "<U>%s</U>" % t
    if 'a' in fl: t = "<I>%s</I>" % t
    return t

def node(name, stereo, attrs, meths, abstract=False, fill="#FFFFFF"):
    head = "<B>%s</B>" % esc(name)
    if abstract: head = "<I>%s</I>" % head
    top = ""
    if stereo: top = "&laquo;%s&raquo;<BR/>" % esc(stereo)
    if abstract: top += "{abstract}<BR/>"
    a = "".join(fmt(l) + '<BR ALIGN="LEFT"/>' for l in attrs) or " "
    m = "".join(fmt(l) + '<BR ALIGN="LEFT"/>' for l in meths) or " "
    return ('%s [label=<<TABLE BORDER="1" CELLBORDER="0" CELLSPACING="0" CELLPADDING="4" BGCOLOR="%s">'
            '<TR><TD ALIGN="CENTER">%s%s</TD></TR><HR/>'
            '<TR><TD ALIGN="LEFT" BALIGN="LEFT">%s</TD></TR><HR/>'
            '<TR><TD ALIGN="LEFT" BALIGN="LEFT">%s</TD></TR></TABLE>>];\n') % (
            name.replace("::", "_"), fill, top, head, a, m)

N = []
def add(name, attrs, meths, stereo=None, abstract=False, fill="#FFFFFF"):
    N.append(node(name, stereo, attrs, meths, abstract, fill))

# ---------------- classes ----------------
N.append('std_exception [label=<<TABLE BORDER="1" CELLBORDER="0" CELLSPACING="0" CELLPADDING="4"><TR><TD><B>std::exception</B></TD></TR></TABLE>>];\n')
add("GameException",
    [("- message: string","")],
    [("+ GameException(msg: string)",""), ("+ ~GameException()  {throw()}",""),
     ("+ what(): const char*  {redefined}","")], fill="#FDEDEC")

add("Dice",
    [("+ defaultSides: const int = 6","s"), ("- iMyNumSides: int",""), ("- iMyResult: int","")],
    [("+ Dice()",""), ("+ Dice(numSides: int)",""), ("+ roll(): int",""),
     ("+ getResult(): int",""), ("+ getNumSides(): int","")], fill="#FEF9E7")

add("Item", [("# mName: string",""), ("# mValue: int","")],
    [("+ Item(name: string, value: int)",""), ("+ ~Item()  {virtual}",""),
     ("+ getName(): string",""), ("+ getValue(): int",""),
     ("+ applyItem(user: Character&): void  = 0","a"),
     ("+ getItemType(): string  = 0","a"),
     ("+ getDescription(): string  = 0","a")], abstract=True, fill="#EBF5FB")
add("Weapon", [("- mBonusAtk: int","")],
    [("+ Weapon(name: string, value: int, bonusAtk: int)",""),
     ("+ getBonusAtk(): int",""), ("+ applyItem(user: Character&): void",""),
     ("+ getItemType(): string",""), ("+ getDescription(): string","")], fill="#EBF5FB")
add("Armor", [("- mBonusArmor: int","")],
    [("+ Armor(name: string, value: int, bonusArmor: int)",""),
     ("+ getBonusArmor(): int",""), ("+ applyItem(user: Character&): void",""),
     ("+ getItemType(): string",""), ("+ getDescription(): string","")], fill="#EBF5FB")
add("Potion", [("- mHealAmount: int",""), ("- mManaAmount: int","")],
    [("+ Potion(name: string, value: int, healAmount: int,",""),
     ("        manaAmount: int = 0)",""),
     ("+ getHealAmount(): int",""), ("+ getManaAmount(): int",""),
     ("+ applyItem(user: Character&): void",""),
     ("+ getItemType(): string",""), ("+ getDescription(): string","")], fill="#EBF5FB")

add("StatusEffect", [("# mName: string",""), ("# mDuration: int","")],
    [("+ StatusEffect(name: string, duration: int)",""), ("+ ~StatusEffect()  {virtual}",""),
     ("+ getName(): string",""), ("+ getDuration(): int",""),
     ("+ isExpired(): bool",""), ("+ decrementTurn(): void",""),
     ("+ onTurnStart(target: Character&): bool  = 0","a"),
     ("+ describe(): string  = 0","a")], abstract=True, fill="#F4ECF7")
add("PoisonEffect", [("- mDamagePerTurn: int","")],
    [("+ PoisonEffect(name, duration, damagePerTurn)",""),
     ("+ onTurnStart(target: Character&): bool",""), ("+ describe(): string","")], fill="#F4ECF7")
add("StunEffect", [],
    [("+ StunEffect(name, duration)",""),
     ("+ onTurnStart(target: Character&): bool",""), ("+ describe(): string","")], fill="#F4ECF7")
add("RegenEffect", [("- mHealPerTurn: int","")],
    [("+ RegenEffect(name, duration, healPerTurn)",""),
     ("+ onTurnStart(target: Character&): bool",""), ("+ describe(): string","")], fill="#F4ECF7")

add("Skill",
    [("+ EFFECT_NONE = 0, EFFECT_POISON = 1, EFFECT_STUN = 2: const int","s"),
     ("+ EFFECT_REGEN = 3, EFFECT_DRAIN = 4: const int","s"),
     ("- mSkillName: string",""), ("- mMpCost: int",""), ("- mPower: int",""),
     ("- mEffectType: int",""), ("- mEffectDuration: int","")],
    [("+ Skill(skillName: string, mpCost: int, power: int,",""),
     ("      effectType: int = EFFECT_NONE, effectDuration: int = 3)",""),
     ("+ getSkillName() / getMpCost() / getPower()",""),
     ("+ getEffectType() / getEffectDuration()",""),
     ("+ getDescription(): string",""),
     ("+ createEffect(): StatusEffect*",""),
     ("+ cast(caster: Character&, target: Character&, d: Dice&): int","")], fill="#F4ECF7")

add("Character",
    [("+ totalCharacters: int","s"),
     ("+ MAX_ITEMS = 6, MAX_SKILLS = 8, MAX_EFFECTS = 6: const int","s"),
     ("# mName: string",""),
     ("# mMaxHealth, mHealth, mMaxMana, mMana: int",""),
     ("# mAttack, mArmor, mLevel, mX, mY: int",""),
     ("# mCritChance, mMissChance: int   // %",""),
     ("# mDefending: bool",""),
     ("# mInventory: Item*[MAX_ITEMS]",""), ("# mItemCount: int",""),
     ("# mSkills: Skill*[MAX_SKILLS]",""), ("# mSkillCount: int",""),
     ("# mActiveEffects: StatusEffect*[MAX_EFFECTS]",""), ("# mEffectCount: int",""),
     ("# mEquippedWeapon: Weapon*",""), ("# mEquippedArmor: Armor*","")],
    [("+ Character(name: string, maxHealth: int = 100, maxMana: int = 30,",""),
     ("      attack: int = 10, armor: int = 0)   // auto-adds skill Power Strike",""),
     ("+ ~Character()  {virtual}",""),
     ("+ getTotalCharacters(): int","s"),
     ("+ performAction(target: Character&, d: Dice&): int  = 0","a"),
     ("+ speak(): void  = 0","a"),
     ("+ scaleLevel(level: int): void  {virtual}",""),
     ("+ getName() / getHealth() / getMaxHealth() / getMana() / getMaxMana()",""),
     ("+ getAttack() / getArmor() / getLevel() / getX() / getY() / isAlive()",""),
     ("+ setX(x) / setY(y) / setLevel(l) / setCritChance(p) / setMissChance(p)",""),
     ("      : Character&   // chained calls",""),
     ("+ changeHealth(delta: int) / changeMana(delta: int): void",""),
     ("+ takeDamage(rawDamage: int): int",""),
     ("+ hitTarget(target: Character&, raw: int, action: string, canMiss: bool): int",""),
     ("+ defend() / endDefend(): void  ;  isDefending(): bool",""),
     ("+ heal(amount: int): void", ""), ("+ heal(ratioOfMax: double): void",""),
     ("+ addItem(item: Item*): bool", ""), ("+ itemAt(index: int): Item*",""),
     ("+ useItem(index: int): void",""),
     ("+ equipWeapon(w: Weapon*) / equipArmor(a: Armor*): void",""),
     ("+ getEquippedIndex() / getEquippedArmorIndex(): int",""),
     ("+ getWeaponBonus() / getArmorBonus() / getEffectiveAttack(): int",""),
     ("+ addStatusEffect(effect: StatusEffect*): void",""),
     ("+ processTurnEffects(): bool", ""),
     ("+ addSkill(skill: Skill*): bool", ""), ("+ hasSkill(name: string): bool",""),
     ("+ skillAt(index: int): Skill*",""),
     ("+ castSkill(index: int, target: Character&, d: Dice&): int",""),
     ("+ printStatus() / printInventory(): void",""),
     ("- Character(other: const Character&)  {cam sao chep}","")],
    abstract=True, fill="#E8F8F5")
add("Warrior", [],
    [("+ Warrior(name, maxHealth, maxMana, attack, armor)",""),
     ("+ performAction(target: Character&, d: Dice&): int",""),
     ("+ speak(): void",""), ("// unique skill: Shield Bash (stun)","")], fill="#E8F8F5")
add("Mage",
    [("- mSpellPower: int",""), ("- SPELL_MP_COST: const int = 10","s")],
    [("+ Mage(name, maxHealth, maxMana, attack, armor, spellPower)",""),
     ("+ performAction(target: Character&, d: Dice&): int",""),
     ("+ speak(): void",""), ("// unique skill: Venom Bolt (poison)","")], fill="#E8F8F5")
add("Archer", [],
    [("+ Archer(name, maxHealth, maxMana, attack, armor)  // crit 25%",""),
     ("+ performAction(target: Character&, d: Dice&): int",""),
     ("+ speak(): void",""), ("// unique skill: Deep Breath (regen)","")], fill="#E8F8F5")
add("Spitter", [],
    [("+ Spitter(name, maxHealth, maxMana, attack, armor)",""),
     ("+ performAction(target: Character&, d: Dice&): int   // poisons",""),
     ("+ speak(): void","")], fill="#FDEDEC")
add("Brute", [],
    [("+ Brute(name, maxHealth, maxMana, attack, armor)    // miss 25%",""),
     ("+ performAction(target: Character&, d: Dice&): int   // may stun",""),
     ("+ speak(): void","")], fill="#FDEDEC")
add("AmoebaBoss",
    [("- mXPivot, mYPivot: int",""), ("- mSpinPower, mSpinAngle, mActions: int",""),
     ("- HIF_SOUND_ID: const int = 42","s")],
    [("+ AmoebaBoss(name, maxHealth, maxMana, attack, armor,",""),
     ("      xPivot: int = 0, yPivot: int = 0)",""),
     ("+ rotateAroundPivot(d: Dice&): void",""),
     ("+ performAction(target: Character&, d: Dice&): int   // 3-turn cycle",""),
     ("+ speak(): void",""),
     ("+ scaleLevel(level: int): void  {redefined}",""),
     ("// unique skill: Toxic Storm (poison)","")], fill="#FDEDEC")

add("DungeonArena",
    [("+ MAX_FLOOR = 5, ITEM_KINDS = 6, SKILL_KINDS = 6: const int","s"),
     ("- mFloor: int",""), ("- mEnemies: Character**",""), ("- mEnemyCount: int","")],
    [("+ DungeonArena()",""), ("+ ~DungeonArena()",""),
     ("+ getFloor() / setFloor(f) / isLastFloor()",""),
     ("+ spawnEnemies(d: Dice&): void",""), ("+ clearEnemies(): void",""),
     ("+ enemyAt(index: int): Character*",""),
     ("+ enemyCount() / livingEnemyCount(): int",""),
     ("+ createItem(kind: int): Item*",""),
     ("+ createSkill(kind: int): Skill*",""),
     ("+ skillName(kind: int): string",""),
     ("- createMinion(kind: int, number: int): Character*","")], fill="#FEF5E7")

add("Game",
    [("+ TEAM_SIZE = 3: const int","s"),
     ("+ PHASE_CHOOSE_HERO / BATTLE / REWARD_TYPE / REWARD_PICK / OVER: const int","s"),
     ("- mHeroes: Character*[3]",""), ("- mArena: DungeonArena",""), ("- mD6: Dice",""),
     ("- mPhase, mActive, mTurn, mCleared: int",""),
     ("- mRewardType, mOffers[3], mOfferCount: int","")],
    [("+ Game() / ~Game() / newGame()",""),
     ("+ getPhase() / getActive() / getTurn() / getCleared(): int",""),
     ("+ getHero(i): Character* / getArena(): DungeonArena&",""),
     ("+ aliveHeroes(): int / isVictory(): bool / canLearnSkill(): bool",""),
     ("+ offerCount(): int / offerDescription(i): string",""),
     ("+ getResult(raw&, curved&, letter&): void",""),
     ("+ selectHero(i) / attack(e) / useSkill(s, e) / useItem(k) / defend()",""),
     ("+ chooseRewardType(t) / backToRewardType() / pickOffer(i)",""),
     ("- beginHeroTurn() / endHeroAction() / enemyPhase()",""),
     ("- floorWon() / heroDown() / makeOffers()","")],
    fill="#FEF5E7")

add("GradedActivity",
    [("# letter: char",""), ("# score: double","")],
    [("# determineGrade(): void",""), ("+ GradedActivity()",""),
     ("+ setScore(s: double): void",""), ("+ getScore(): double",""),
     ("+ getLetterGrade(): char","")], fill="#FDFEFE")
add("CurvedActivity",
    [("# rawScore: double",""), ("# percentage: double","")],
    [("+ CurvedActivity()",""), ("+ setPercentage(c: double): void",""),
     ("+ setScore(s: double): void  {redefined}",""), ("+ getRawScore(): double","")], fill="#FDFEFE")

add("GameEngine",
    [("- mGame: Game","")],
    [("+ GameEngine()",""),
     ("+ handle(line: string): string  // command -> JSON",""),
     ("- dispatch(cmd: string, a: int, b: int): void",""),
     ("- stateJson(log: string): string","")],
    stereo="web bridge", fill="#FDEDEC")

# ---------------- edges ----------------
E = []
def inh(c, p):   E.append('%s -> %s [arrowhead=empty, color="#1B2631", penwidth=1.3];\n' % (c, p))
def comp(a, b, lbl, hl, tl="1"):
    E.append('%s -> %s [dir=both, arrowtail=diamond, arrowhead=none, headlabel="%s", taillabel="%s", label="%s", fontsize=10, labeldistance=2.2, color="#1B2631"];\n' % (a, b, hl, tl, lbl))
def agg(a, b, lbl, hl, tl="1"):
    E.append('%s -> %s [dir=both, arrowtail=odiamond, arrowhead=none, headlabel="%s", taillabel="%s", label="%s", fontsize=10, labeldistance=2.2, color="#1B2631"];\n' % (a, b, hl, tl, lbl))
def dep(a, b, lbl="", extra=""):
    E.append('%s -> %s [style=dashed, arrowhead=open, label="%s", fontsize=9, color="#555555" %s];\n' % (a, b, lbl, extra))
U = "&laquo;use&raquo;"; C = "&laquo;create&raquo;"; T = "&laquo;throw&raquo;"

inh("GameException", "std_exception")
inh("Weapon", "Item"); inh("Armor", "Item"); inh("Potion", "Item")
inh("PoisonEffect", "StatusEffect"); inh("StunEffect", "StatusEffect"); inh("RegenEffect", "StatusEffect")
for c in ("Warrior", "Mage", "Archer", "Spitter", "Brute", "AmoebaBoss"): inh(c, "Character")
inh("CurvedActivity", "GradedActivity")

comp("Character", "Item", "mInventory", "0..6")
comp("Character", "Skill", "mSkills", "0..8")
comp("Character", "StatusEffect", "mActiveEffects", "0..6")
agg("Character", "Weapon", "mEquippedWeapon", "0..1")
agg("Character", "Armor", "mEquippedArmor", "0..1")
comp("DungeonArena", "Character", "mEnemies", "0..*")
comp("Game", "Character", "mHeroes", "3")
comp("Game", "DungeonArena", "mArena", "1")
comp("Game", "Dice", "mD6", "1")
comp("GameEngine", "Game", "mGame", "1")
dep("Game", "CurvedActivity", U)
dep("Game", "Item", C); dep("Game", "Skill", C)

dep("Skill", "StatusEffect", C)
dep("Skill", "Dice", U)
dep("Item", "Character", U)
dep("Character", "Dice", U)
dep("Character", "GameException", T)
dep("Skill", "GameException", T)
dep("Game", "GameException", T)
dep("DungeonArena", "Item", C); dep("DungeonArena", "Skill", C)
dep("DungeonArena", "AmoebaBoss", C)
dep("DungeonArena", "Warrior", C); dep("DungeonArena", "Mage", C)
dep("DungeonArena", "Spitter", C); dep("DungeonArena", "Brute", C)
dep("Spitter", "PoisonEffect", C); dep("Brute", "StunEffect", C)
dep("DungeonArena", "Dice", U)

dot = ('digraph UML {\n rankdir=BT; splines=true; nodesep=0.55; ranksep=1.0; compound=true;\n'
       ' node [shape=plain, fontname="DejaVu Sans", fontsize=10];\n'
       ' edge [fontname="DejaVu Sans"];\n'
       ' graph [fontname="DejaVu Sans", labelloc=t, fontsize=18, '
       'label=<<B>UML Class Diagram &#8211; Arena Text-based RPG (PHY10609 / PET10001)</B><BR/>'
       '<FONT POINT-SIZE="11">Theo ma nguon thuc te. Gach chan = static &#183; In nghieng = abstract / thuan ao &#183; '
       '&#9671; aggregation &#183; &#9670; composition &#183; &#9655; ke thua (is-a) &#183; net dut = phu thuoc</FONT>>];\n')
dot += "".join(N) + "".join(E) + "}\n"
open("arena_uml.dot", "w").write(dot)
