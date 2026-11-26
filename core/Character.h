// File: Character.h
// Lop co so TRUU TUONG cua moi thuc the chien dau.
//  - Ham thuan ao: performAction, speak
//  - virtual destructor: giai phong tui do / hieu ung / ky nang
//  - Composition : mInventory[], mActiveEffects[], mSkills[]
//  - Aggregation : mEquippedWeapon (tro vao Weapon nam trong mInventory)
//  - static      : totalCharacters (dem so thuc the dang song)
//  - Chained calls: hero->setX(3).setY(7)
//  - Chi mang (critical) / truot (miss) va Defend (phong thu)
#ifndef CHARACTERHEADER
#define CHARACTERHEADER

#include <iostream>
#include <string>
#include "Dice.h"
#include "Item.h"
#include "StatusAndSkill.h"

class Character {
public:
    static int totalCharacters;
    static const int MAX_ITEMS = 6;
    static const int MAX_SKILLS = 8;
    static const int MAX_EFFECTS = 6;
    static int getTotalCharacters() { return totalCharacters; }

private:
    // Cam sao chep (so huu con tro dong): khai bao private, khong cai dat
    Character(const Character& other);
    Character& operator=(const Character& other);

protected:
    std::string mName;
    int mMaxHealth;
    int mHealth;
    int mMaxMana;
    int mMana;
    int mAttack;
    int mArmor;
    int mLevel;
    int mX;
    int mY;
    int mCritChance;     // % chi mang (sat thuong x2)
    int mMissChance;     // % danh truot
    bool mDefending;     // dang phong thu: nhan 1/2 sat thuong den luot sau

    // Composition
    Item* mInventory[MAX_ITEMS];
    int mItemCount;
    Skill* mSkills[MAX_SKILLS];
    int mSkillCount;
    StatusEffect* mActiveEffects[MAX_EFFECTS];
    int mEffectCount;

    // Aggregation: KHONG delete rieng (da nam trong mInventory)
    Weapon* mEquippedWeapon;
    Armor* mEquippedArmor;

public:
    Character(const std::string& name,
              int maxHealth = 100, int maxMana = 30,
              int attack = 10, int armor = 0);
    virtual ~Character();

    // ===== Ham thuan ao =====
    virtual int performAction(Character& target, Dice& d) = 0;
    virtual void speak() const = 0;

    // Level Scaling (mac dinh; lop con co the ghi de)
    virtual void scaleLevel(int level);

    // ===== Getter =====
    const std::string& getName() const;
    int getHealth() const;
    int getMaxHealth() const;
    int getMana() const;
    int getMaxMana() const;
    int getAttack() const;
    int getArmor() const;
    int getLevel() const;
    int getX() const;
    int getY() const;
    bool isAlive() const;

    // ===== Setter noi chieu (chained calls, con tro this) =====
    Character& setX(int x);
    Character& setY(int y);
    Character& setLevel(int level);
    Character& setCritChance(int percent);
    Character& setMissChance(int percent);
    int getCritChance() const;
    int getMissChance() const;

    // ===== Mau & MP =====
    void changeHealth(int delta);
    void changeMana(int delta);
    int takeDamage(int rawDamage);   // tru giap (+ phong thu), thiet toi thieu 1

    // Danh 1 don: co the TRUOT (neu canMiss) hoac CHI MANG (x2). In ket qua.
    // Tra ve sat thuong thuc te (0 neu truot).
    int hitTarget(Character& target, int rawDamage,
                  const std::string& action, bool canMiss);

    // Phong thu: nhan 1/2 sat thuong den dau luot sau, hoi 6 MP
    void defend();
    void endDefend();
    bool isDefending() const;
    void heal(int amount);           // nap chong 1: hoi so HP can thi
    void heal(double ratioOfMax);    // nap chong 2: hoi ty le (0,1] HP toi da

    // ===== Tui do =====
    bool addItem(Item* item);        // false neu tui day
    int itemCount() const;
    Item* itemAt(int index) const;
    void useItem(int index);         // Potion: dung xong xoa; Weapon: trang bi
    void equipWeapon(Weapon* w);
    void equipArmor(Armor* a);
    int getEquippedIndex() const;    // vu khi dang cam, -1 neu khong
    int getEquippedArmorIndex() const;
    int getArmorBonus() const;
    int getWeaponBonus() const;
    int getEffectiveAttack() const;  // attack + bonus vu khi
    void printInventory() const;

    // ===== Hieu ung trang thai =====
    void addStatusEffect(StatusEffect* effect);
    // Goi dau luot: ap dung moi hieu ung, giam thoi luong, xoa hieu ung het han.
    // Tra ve false neu bi Choang (mat luot).
    bool processTurnEffects();
    int activeEffectCount() const;
    StatusEffect* effectAt(int index) const;   // de hien thi (describe)

    // ===== Ky nang =====
    bool addSkill(Skill* skill);
    int skillCount() const;
    Skill* skillAt(int index) const;
    bool hasSkill(const std::string& name) const;
    int castSkill(int index, Character& target, Dice& d);

    // ===== In trang thai (iomanip) =====
    void printStatus() const;

};

#endif
