// File: Character.cpp
#include "Character.h"
#include <cassert>
#include <iomanip>
#include <iostream>

int Character::totalCharacters = 0;

Character::Character(const std::string& name, int maxHealth, int maxMana,
                     int attack, int armor)
    : mName(name), mMaxHealth(maxHealth), mHealth(maxHealth),
      mMaxMana(maxMana), mMana(maxMana), mAttack(attack), mArmor(armor),
      mLevel(1), mX(0), mY(0), mCritChance(10), mMissChance(10),
      mDefending(false),
      mItemCount(0), mSkillCount(0), mEffectCount(0),
      mEquippedWeapon(0), mEquippedArmor(0)
{
    assert(maxHealth > 0);
    assert(maxMana >= 0);
    assert(attack >= 0);
    assert(armor >= 0);
    for (int i = 0; i < MAX_ITEMS; i++) mInventory[i] = 0;
    for (int i = 0; i < MAX_SKILLS; i++) mSkills[i] = 0;
    for (int i = 0; i < MAX_EFFECTS; i++) mActiveEffects[i] = 0;
    addSkill(new Skill("Power Strike", 5, 10));   // chieu CHUNG cho moi nhan vat
    totalCharacters += 1;
}

// Virtual destructor: giai phong toan bo bo nho dong.
// mEquippedWeapon KHONG delete (aggregation) -> tranh double free.
Character::~Character() {
    for (int i = 0; i < mItemCount; i++) delete mInventory[i];
    for (int i = 0; i < mEffectCount; i++) delete mActiveEffects[i];
    for (int i = 0; i < mSkillCount; i++) delete mSkills[i];
    totalCharacters -= 1;
}

// ===== Getter =====
const std::string& Character::getName() const { return mName; }
int Character::getHealth() const { return mHealth; }
int Character::getMaxHealth() const { return mMaxHealth; }
int Character::getMana() const { return mMana; }
int Character::getMaxMana() const { return mMaxMana; }
int Character::getAttack() const { return mAttack; }
int Character::getArmor() const { return mArmor + getArmorBonus(); }
int Character::getLevel() const { return mLevel; }
int Character::getX() const { return mX; }
int Character::getY() const { return mY; }
bool Character::isAlive() const { return mHealth > 0; }

// ===== Setter noi chieu =====
Character& Character::setX(int x) { this->mX = x; return *this; }
Character& Character::setY(int y) { this->mY = y; return *this; }
Character& Character::setLevel(int level) { this->mLevel = level; return *this; }
Character& Character::setCritChance(int percent) { mCritChance = percent; return *this; }
Character& Character::setMissChance(int percent) { mMissChance = percent; return *this; }
int Character::getCritChance() const { return mCritChance; }
int Character::getMissChance() const { return mMissChance; }

// ===== Mau & MP =====
void Character::changeHealth(int delta) {
    mHealth += delta;
    if (mHealth < 0) mHealth = 0;
    if (mHealth > mMaxHealth) mHealth = mMaxHealth;
}

void Character::changeMana(int delta) {
    mMana += delta;
    if (mMana < 0) mMana = 0;
    if (mMana > mMaxMana) mMana = mMaxMana;
}

// real = raw - armor (toi thieu 1); dang phong thu thi giam mot nua
int Character::takeDamage(int rawDamage) {
    if (rawDamage <= 0) return 0;
    int real = rawDamage - getArmor();
    if (real < 1) real = 1;
    if (mDefending) real = (real + 1) / 2;
    mHealth -= real;
    if (mHealth < 0) mHealth = 0;
    return real;
}

// Danh 1 don: truot (neu canMiss) hoac chi mang (x2 sat thuong truoc giap).
// Dung doi tuong an danh Dice(100) de tung ti le %.
int Character::hitTarget(Character& target, int rawDamage,
                         const std::string& action, bool canMiss) {
    if (canMiss && Dice(100).roll() <= mMissChance) {
        std::cout << "  -> " << mName << " " << action << " " << target.getName()
                  << ": MISS!" << std::endl;
        return 0;
    }
    bool critical = (Dice(100).roll() <= mCritChance);
    if (critical) rawDamage = rawDamage * 2;
    int real = target.takeDamage(rawDamage);
    std::cout << "  -> " << mName << " " << action << " " << target.getName()
              << ": loses " << real << " HP" << (critical ? " (CRITICAL!)" : "")
              << "." << std::endl;
    return real;
}

void Character::defend() {
    mDefending = true;
    changeMana(6);
    std::cout << "  -> [DEFEND] " << mName
              << " braces for impact (damage halved, +6 MP)." << std::endl;
}
void Character::endDefend() { mDefending = false; }
bool Character::isDefending() const { return mDefending; }

void Character::heal(int amount) {
    if (amount < 0) throw GameException("Logic error: heal amount cannot be negative!");
    if (!isAlive()) return;   // nguoi da guc khong the hoi mau
    mHealth += amount;
    if (mHealth > mMaxHealth) mHealth = mMaxHealth;
}

void Character::heal(double ratioOfMax) {
    assert(ratioOfMax > 0.0 && ratioOfMax <= 1.0);
    heal((int)(mMaxHealth * ratioOfMax)); // dung lai nap chong 1
}

// ===== Tui do =====
bool Character::addItem(Item* item) {
    if (item == 0 || mItemCount >= MAX_ITEMS) return false;
    mInventory[mItemCount] = item;
    mItemCount += 1;
    return true;
}

int Character::itemCount() const { return mItemCount; }

Item* Character::itemAt(int index) const {
    assert(index >= 0 && index < mItemCount);
    return mInventory[index];
}

void Character::useItem(int index) {
    if (index < 0 || index >= mItemCount) {
        throw GameException(mName + " chose an invalid inventory slot: "
                            + intToString(index));
    }
    Item* chosen = mInventory[index];
    chosen->applyItem(*this);            // Dynamic Binding

    if (chosen->getItemType() == "Potion") {   // Potion dung xong thi bien mat
        delete chosen;
        for (int i = index; i < mItemCount - 1; i++) {
            mInventory[i] = mInventory[i + 1];
        }
        mInventory[mItemCount - 1] = 0;
        mItemCount -= 1;
    }
}

void Character::equipWeapon(Weapon* w) {
    if (w == 0) return;
    mEquippedWeapon = w;
    std::cout << "  -> [EQUIP] " << mName << " equips " << w->getName()
              << " (+" << w->getBonusAtk() << " ATK)." << std::endl;
}

int Character::getEquippedIndex() const {
    for (int i = 0; i < mItemCount; i++) {
        if (mInventory[i] == mEquippedWeapon && mEquippedWeapon != 0) return i;
    }
    return -1;
}

void Character::equipArmor(Armor* a) {
    if (a == 0) return;
    mEquippedArmor = a;
    std::cout << "  -> [EQUIP] " << mName << " wears " << a->getName()
              << " (+" << a->getBonusArmor() << " ARMOR)." << std::endl;
}

int Character::getEquippedArmorIndex() const {
    for (int i = 0; i < mItemCount; i++) {
        if (mInventory[i] == mEquippedArmor && mEquippedArmor != 0) return i;
    }
    return -1;
}

int Character::getArmorBonus() const {
    return (mEquippedArmor != 0) ? mEquippedArmor->getBonusArmor() : 0;
}

int Character::getWeaponBonus() const {
    return (mEquippedWeapon != 0) ? mEquippedWeapon->getBonusAtk() : 0;
}

int Character::getEffectiveAttack() const {
    return mAttack + getWeaponBonus();
}

void Character::printInventory() const {
    std::cout << "  " << mName << "'s inventory (" << mItemCount << "/"
              << MAX_ITEMS << "):" << std::endl;
    if (mItemCount == 0) {
        std::cout << "    (empty)" << std::endl;
        return;
    }
    for (int i = 0; i < mItemCount; i++) {
        std::cout << ((i == getEquippedIndex() || i == getEquippedArmorIndex()) ? "  * " : "    ")
                  << "[" << i << "] " << mInventory[i]->getDescription()
                  << std::endl;
    }
}

// ===== Hieu ung trang thai =====
void Character::addStatusEffect(StatusEffect* effect) {
    assert(effect != 0);
    if (mEffectCount >= MAX_EFFECTS) {   // day -> bo hieu ung cu nhat
        delete mActiveEffects[0];
        for (int i = 0; i < mEffectCount - 1; i++) {
            mActiveEffects[i] = mActiveEffects[i + 1];
        }
        mEffectCount -= 1;
    }
    mActiveEffects[mEffectCount] = effect;
    mEffectCount += 1;
}

bool Character::processTurnEffects() {
    bool canAct = true;
    int i = 0;
    while (i < mEffectCount) {
        StatusEffect* e = mActiveEffects[i];
        if (!e->onTurnStart(*this)) canAct = false;  // Dynamic Binding
        e->decrementTurn();
        if (e->isExpired()) {
            std::cout << "  [END] Effect '" << e->getName() << "' on "
                      << mName << " has expired." << std::endl;
            delete e;
            for (int k = i; k < mEffectCount - 1; k++) {
                mActiveEffects[k] = mActiveEffects[k + 1];
            }
            mActiveEffects[mEffectCount - 1] = 0;
            mEffectCount -= 1;
        } else {
            i++;
        }
    }
    return canAct;
}

int Character::activeEffectCount() const { return mEffectCount; }

StatusEffect* Character::effectAt(int index) const {
    assert(index >= 0 && index < mEffectCount);
    return mActiveEffects[index];
}

// ===== Ky nang =====
bool Character::addSkill(Skill* skill) {
    if (skill == 0 || mSkillCount >= MAX_SKILLS) return false;
    mSkills[mSkillCount] = skill;
    mSkillCount += 1;
    return true;
}

int Character::skillCount() const { return mSkillCount; }

Skill* Character::skillAt(int index) const {
    assert(index >= 0 && index < mSkillCount);
    return mSkills[index];
}

bool Character::hasSkill(const std::string& name) const {
    for (int i = 0; i < mSkillCount; i++) {
        if (mSkills[i]->getSkillName() == name) return true;
    }
    return false;
}

int Character::castSkill(int index, Character& target, Dice& d) {
    if (index < 0 || index >= mSkillCount) {
        throw GameException(mName + " chose an invalid skill: "
                            + intToString(index));
    }
    return mSkills[index]->cast(*this, target, d);   // co the throw (het MP)
}

// ===== Level Scaling mac dinh =====
void Character::scaleLevel(int level) {
    assert(level >= mLevel);
    int steps = level - mLevel;
    mLevel = level;
    mMaxHealth += 15 * steps;
    mMaxMana += 5 * steps;
    mAttack += 3 * steps;
    mHealth = mMaxHealth;
    mMana = mMaxMana;
}

// ===== In trang thai (iomanip) =====
void Character::printStatus() const {
    std::cout << std::left << "  " << std::setw(20) << mName
              << "HP[" << mHealth << "] MP[" << mMana << "] ARMOR["
              << getArmor() << "]" << (isAlive() ? "" : "  (DEAD)") << std::endl;
    for (int i = 0; i < mEffectCount; i++) {
        std::cout << "      - " << mActiveEffects[i]->describe() << std::endl;
    }
}

