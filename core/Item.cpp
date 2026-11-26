// File: Item.cpp
#include "Item.h"
#include "Character.h"
#include <cassert>

// ===== Item =====
Item::Item(const std::string& name, int value)
    : mName(name), mValue(value) {
    assert(value >= 0); // Precondition: gia tri khong am
}
Item::~Item() {}
const std::string& Item::getName() const { return mName; }
int Item::getValue() const { return mValue; }

// ===== Weapon =====
Weapon::Weapon(const std::string& name, int value, int bonusAtk)
    : Item(name, value), mBonusAtk(bonusAtk) {
    assert(bonusAtk >= 0);
}
Weapon::~Weapon() {}
int Weapon::getBonusAtk() const { return mBonusAtk; }

void Weapon::applyItem(Character& user) {
    user.equipWeapon(this);
}
std::string Weapon::getItemType() const { return "Weapon"; }
std::string Weapon::getDescription() const {
    return "[Weapon] " + mName + " (+" + intToString(mBonusAtk)
           + " ATK, value " + intToString(mValue) + ")";
}

// ===== Potion =====
Potion::Potion(const std::string& name, int value, int healAmount,
               int manaAmount)
    : Item(name, value), mHealAmount(healAmount), mManaAmount(manaAmount) {
    assert(healAmount >= 0);
    assert(manaAmount >= 0);
}
Potion::~Potion() {}
int Potion::getHealAmount() const { return mHealAmount; }
int Potion::getManaAmount() const { return mManaAmount; }

void Potion::applyItem(Character& user) {
    user.heal(mHealAmount);          // nap chong heal(int)
    user.changeMana(mManaAmount);
    std::cout << "  -> [ITEM] " << user.getName() << " uses " << mName
              << " (+" << mHealAmount << " HP, +" << mManaAmount << " MP)."
              << std::endl;
}
std::string Potion::getItemType() const { return "Potion"; }
std::string Potion::getDescription() const {
    return "[Potion] " + mName + " (+" + intToString(mHealAmount) + " HP, +"
           + intToString(mManaAmount) + " MP, value "
           + intToString(mValue) + ")";
}

// ===== Armor =====
Armor::Armor(const std::string& name, int value, int bonusArmor)
    : Item(name, value), mBonusArmor(bonusArmor) {
    assert(bonusArmor >= 0);
}
Armor::~Armor() {}
int Armor::getBonusArmor() const { return mBonusArmor; }

void Armor::applyItem(Character& user) {
    user.equipArmor(this);
}
std::string Armor::getItemType() const { return "Armor"; }
std::string Armor::getDescription() const {
    return "[Armor] " + mName + " (+" + intToString(mBonusArmor)
           + " ARMOR, value " + intToString(mValue) + ")";
}
