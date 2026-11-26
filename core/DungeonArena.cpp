// File: DungeonArena.cpp
#include "DungeonArena.h"
#include <iostream>
#include "HeroesAndBoss.h"

const int DungeonArena::MAX_FLOOR = 5;
const int DungeonArena::ITEM_KINDS = 6;
const int DungeonArena::SKILL_KINDS = 6;

DungeonArena::DungeonArena() : mFloor(1), mEnemies(0), mEnemyCount(0) {}
DungeonArena::~DungeonArena() { clearEnemies(); }

int DungeonArena::getFloor() const { return mFloor; }
void DungeonArena::setFloor(int f) { mFloor = f; }
bool DungeonArena::isLastFloor() const { return mFloor >= MAX_FLOOR; }
Character* DungeonArena::enemyAt(int index) const { return mEnemies[index]; }
int DungeonArena::enemyCount() const { return mEnemyCount; }

int DungeonArena::livingEnemyCount() const {
    int n = 0;
    for (int i = 0; i < mEnemyCount; i++) {
        if (mEnemies[i]->isAlive()) n++;
    }
    return n;
}

void DungeonArena::clearEnemies() {
    for (int i = 0; i < mEnemyCount; i++) {
        delete mEnemies[i];          // virtual destructor
    }
    delete[] mEnemies;
    mEnemies = 0;
    mEnemyCount = 0;
}

// kind: 0 Warrior, 1 Mage, 2 Archer, 3 Spitter, 4 Brute
Character* DungeonArena::createMinion(int kind, int number) const {
    int f = (mFloor > 4) ? 3 : mFloor;      // man 5: quai nho yeu (boss moi la nhan vat chinh)
    std::string n = intToString(number);
    switch (kind) {
    case 0:  return new Warrior("Germ Warrior " + n, 20 + (5 * f) / 2, 15, 3 + f, 1);
    case 1:  return new Mage("Germ Mage " + n, 16 + (5 * f) / 2, 30, 2, 0, 3 + f);
    case 2:  return new Archer("Germ Archer " + n, 18 + (5 * f) / 2, 20, 2 + f, 0);
    case 3:  return new Spitter("Acid Spitter " + n, 18 + (5 * f) / 2, 20, 1 + f, 0);
    default: return new Brute("Shell Brute " + n, 26 + 3 * f, 10, 3 + f, 1 + f / 3);
    }
}

// Man 1-4: mFloor quai nho. Man 5: 4 quai nho + 1 trum (o cuoi mang).
// Loai quai lay ngau nhien trong "ho" mo rong dan theo man.
void DungeonArena::spawnEnemies(Dice& d) {
    clearEnemies();

    int minions = mFloor;
    mEnemyCount = mFloor;
    if (isLastFloor()) {
        minions = 4;
        mEnemyCount = 5;
    }
    mEnemies = new Character*[mEnemyCount];
    for (int i = 0; i < mEnemyCount; i++) mEnemies[i] = 0;

    // Ho quai theo man
    int pool[5];
    int poolSize = 0;
    if (mFloor == 1) {
        pool[poolSize++] = 0; pool[poolSize++] = 2;
    } else if (mFloor == 2) {
        pool[poolSize++] = 0; pool[poolSize++] = 2; pool[poolSize++] = 3;
    } else if (mFloor == 3) {
        pool[poolSize++] = 0; pool[poolSize++] = 1;
        pool[poolSize++] = 2; pool[poolSize++] = 3;
    } else {      // man 4-5: du 5 loai (co Brute)
        for (int k = 0; k < 5; k++) pool[poolSize++] = k;
    }

    for (int i = 0; i < minions; i++) {
        mEnemies[i] = createMinion(pool[d.roll() % poolSize], i + 1);
    }

    if (isLastFloor()) {
        mEnemies[4] = new AmoebaBoss("AMOEBA BOSS", 50, 40, 4, 2);
        mEnemies[4]->scaleLevel(mFloor);
    }

    std::cout << ">> Floor " << mFloor << ": " << mEnemyCount
              << (mEnemyCount == 1 ? " enemy appears" : " enemies appear");
    if (isLastFloor()) std::cout << " (BOSS!)";
    std::cout << "." << std::endl;
}

Item* DungeonArena::createItem(int kind) const {
    int f = mFloor;
    switch (kind) {
    case 0:  return new Potion("Health Potion", 10 + 5 * f, 30 + 12 * f, 0);
    case 1:  return new Potion("Mana Potion", 10 + 5 * f, 0, 30 + 5 * f);
    case 2:  return new Potion("Elixir", 20 + 8 * f, 25 + 8 * f, 20);
    case 3:  return new Potion("Great Potion", 30 + 10 * f, 60 + 15 * f, 10);
    case 4:  return new Weapon("Sharp Blade", 20 + 10 * f, 2 + 2 * f);
    default: return new Armor("Iron Shield", 20 + 10 * f, 1 + (f + 1) / 2);
    }
}

Skill* DungeonArena::createSkill(int kind) const {
    switch (kind) {
    case 0:  return new Skill("Fire Burst", 10, 16);
    case 1:  return new Skill("Venom Dart", 8, 5, Skill::EFFECT_POISON, 3);
    case 2:  return new Skill("Thunder Stun", 12, 8, Skill::EFFECT_STUN, 2);
    case 3:  return new Skill("Second Wind", 10, 8, Skill::EFFECT_REGEN, 3);
    case 4:  return new Skill("Life Drain", 10, 8, Skill::EFFECT_DRAIN);
    default: return new Skill("Mighty Blow", 16, 28);
    }
}

std::string DungeonArena::skillName(int kind) const {
    Skill* tmp = createSkill(kind);
    std::string name = tmp->getSkillName();
    delete tmp;
    return name;
}
