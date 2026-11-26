// File: HeroesAndBoss.cpp
#include "HeroesAndBoss.h"
#include <cassert>
#include <iostream>

// ================= Warrior =================
Warrior::Warrior(const std::string& name, int maxHealth, int maxMana,
                 int attack, int armor)
    : Character(name, maxHealth, maxMana, attack, armor)
{
    assert(armor > 0);
    setMissChance(5);
    addSkill(new Skill("Shield Bash", 8, 6, Skill::EFFECT_STUN, 2));
}
Warrior::~Warrior() {}

int Warrior::performAction(Character& target, Dice& d) {
    return hitTarget(target, getEffectiveAttack() + d.roll(), "slashes", true);
}
void Warrior::speak() const {
    std::cout << mName << ": \"Steel never fears!\"" << std::endl;
}

// ================= Mage =================
const int Mage::SPELL_MP_COST = 10;

Mage::Mage(const std::string& name, int maxHealth, int maxMana,
           int attack, int armor, int spellPower)
    : Character(name, maxHealth, maxMana, attack, armor),
      mSpellPower(spellPower)
{
    assert(spellPower > 0);
    assert(maxMana >= SPELL_MP_COST);
    addSkill(new Skill("Venom Bolt", 12, 8, Skill::EFFECT_POISON, 3));
}
Mage::~Mage() {}

// Het MP -> throw GameException (noi goi tu xu ly)
int Mage::performAction(Character& target, Dice& d) {
    if (mMana < SPELL_MP_COST) {
        throw GameException(mName + " is out of MP to cast a spell.");
    }
    changeMana(-SPELL_MP_COST);
    return hitTarget(target, mSpellPower + getWeaponBonus() + d.roll(),
                     "blasts", true);
}
void Mage::speak() const {
    std::cout << mName << ": \"Elements, awaken!\"" << std::endl;
}

// ================= Archer =================
Archer::Archer(const std::string& name, int maxHealth, int maxMana,
               int attack, int armor)
    : Character(name, maxHealth, maxMana, attack, armor)
{
    setCritChance(25);
    addSkill(new Skill("Deep Breath", 10, 10, Skill::EFFECT_REGEN, 3));
}
Archer::~Archer() {}

// Ban tu xa: hai lan xuc xac
int Archer::performAction(Character& target, Dice& d) {
    return hitTarget(target, getEffectiveAttack() + d.roll() + d.roll(),
                     "shoots", true);
}
void Archer::speak() const {
    std::cout << mName << ": \"One arrow, one target!\"" << std::endl;
}

// ================= Spitter =================
Spitter::Spitter(const std::string& name, int maxHealth, int maxMana,
                 int attack, int armor)
    : Character(name, maxHealth, maxMana, attack, armor)
{
    setCritChance(5);
}
Spitter::~Spitter() {}

int Spitter::performAction(Character& target, Dice& d) {
    int real = hitTarget(target, getEffectiveAttack() + d.roll(),
                         "spits acid at", true);
    if (real > 0 && target.activeEffectCount() == 0) {   // khong chong doc
        std::cout << "     [EFFECT] " << target.getName()
                  << " is covered in acid!" << std::endl;
        target.addStatusEffect(new PoisonEffect("Acid", 2, 2 + mLevel));
    }
    return real;
}
void Spitter::speak() const {
    std::cout << mName << ": \"Hsss... dissolve!\"" << std::endl;
}

// ================= Brute =================
Brute::Brute(const std::string& name, int maxHealth, int maxMana,
             int attack, int armor)
    : Character(name, maxHealth, maxMana, attack, armor)
{
    setMissChance(25);
    setCritChance(15);
}
Brute::~Brute() {}

int Brute::performAction(Character& target, Dice& d) {
    int real = hitTarget(target, getEffectiveAttack() + d.roll() + d.roll(),
                         "smashes", true);
    if (real > 0 && d.roll() == 1 && target.activeEffectCount() == 0) {
        std::cout << "     [EFFECT] " << target.getName()
                  << " is dazed!" << std::endl;
        target.addStatusEffect(new StunEffect("Dazed", 1));
    }
    return real;
}
void Brute::speak() const {
    std::cout << mName << ": \"GRRROOOAR!\"" << std::endl;
}

// ================= AmoebaBoss =================
const int AmoebaBoss::HIF_SOUND_ID = 42;

AmoebaBoss::AmoebaBoss(const std::string& name, int maxHealth, int maxMana,
                       int attack, int armor, int xPivot, int yPivot)
    : Character(name, maxHealth, maxMana, attack, armor),
      mXPivot(xPivot), mYPivot(yPivot), mSpinPower(2), mSpinAngle(0),
      mActions(0)
{
    setX(mXPivot).setY(mYPivot);   // chained calls
    setMissChance(5);
    setCritChance(15);
    addSkill(new Skill("Toxic Storm", 15, 8, Skill::EFFECT_POISON, 3));
}
AmoebaBoss::~AmoebaBoss() {}

// Xoay quanh tam: goc = xuc xac * 45 do
void AmoebaBoss::rotateAroundPivot(Dice& d) {
    mSpinAngle = d.roll() * 45;
    std::cout << "  [SOUND hif" << HIF_SOUND_ID << ".hif] " << mName
              << " spins around pivot (" << mXPivot << "," << mYPivot
              << ") at " << mSpinAngle << " degrees!" << std::endl;
}

// Chu ky 3 luot: Toxic Storm -> Pseudopod Slam -> xoay dap
int AmoebaBoss::performAction(Character& target, Dice& d) {
    rotateAroundPivot(d);
    mActions += 1;
    int step = mActions % 3;

    if (step == 1 && mMana >= mSkills[1]->getMpCost()
        && target.activeEffectCount() == 0) {
        return castSkill(1, target, d);
    }
    if (step == 2) {
        return hitTarget(target, getEffectiveAttack() * 3 / 2 + mSpinPower + d.roll(),
                         "crushes with a pseudopod", true);
    }
    return hitTarget(target, getEffectiveAttack() + mSpinPower + d.roll(),
                     "slams", true);
}
void AmoebaBoss::speak() const {
    std::cout << mName << ": \"I am a teenage amoeba, nothing can stop me!\""
              << std::endl;
}

// Level Scaling dac thu cua trum
void AmoebaBoss::scaleLevel(int level) {
    assert(level >= mLevel);
    int steps = level - mLevel;
    mMaxHealth += 20 * steps;
    mMaxMana += 10 * steps;
    mAttack += 2 * steps;
    mSpinPower += 1 * steps;
    mHealth = mMaxHealth;
    mMana = mMaxMana;
    mLevel = level;
}
