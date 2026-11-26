// File: DungeonArena.h
// Dau truong 5 man: man n co n quai nho; man 5 co 4 quai nho + 1 trum.
// Quai tao bang mang con tro dong Character**. Co kho phan thuong (item + skill).
#ifndef DUNGEONARENAHEADER
#define DUNGEONARENAHEADER

#include "Character.h"
#include "Dice.h"

class DungeonArena {
public:
    static const int MAX_FLOOR;        // 5 man
    static const int ITEM_KINDS;       // so loai item co the roi
    static const int SKILL_KINDS;      // so loai skill co the hoc

private:
    int mFloor;
    Character** mEnemies;              // mang con tro dong
    int mEnemyCount;

    // Cam sao chep
    DungeonArena(const DungeonArena& other);
    DungeonArena& operator=(const DungeonArena& other);

    Character* createMinion(int kind, int number) const;

public:
    DungeonArena();
    ~DungeonArena();

    int getFloor() const;
    void setFloor(int f);
    bool isLastFloor() const;

    void spawnEnemies(Dice& d);        // cap phat quai cho man hien tai
    void clearEnemies();               // giai phong quai

    Character* enemyAt(int index) const;
    int enemyCount() const;
    int livingEnemyCount() const;

    // Kho phan thuong (do khoe dan theo man hien tai)
    //  Item : 0 Health Potion, 1 Mana Potion, 2 Elixir, 3 Great Potion,
    //         4 Sharp Blade (Weapon), 5 Iron Shield (Armor)
    //  Skill: 0 Fire Burst, 1 Venom Dart, 2 Thunder Stun, 3 Second Wind,
    //         4 Life Drain, 5 Mighty Blow
    Item* createItem(int kind) const;
    Skill* createSkill(int kind) const;
    std::string skillName(int kind) const;
};

#endif
