// File: HeroesAndBoss.h
// Cac lop dan xuat cua Character (quan he is-a).
//  Hero : Warrior, Mage, Archer      Quai: Spitter, Brute, AmoebaBoss
// Moi lop co skill RIENG (them sau skill chung "Power Strike" cua Character).
#ifndef HEROESANDBOSSHEADER
#define HEROESANDBOSSHEADER

#include "Character.h"

// Chien binh: giap cao, it truot. Skill rieng: Shield Bash (stun)
class Warrior : public Character {
public:
    Warrior(const std::string& name, int maxHealth, int maxMana,
            int attack, int armor);
    virtual ~Warrior();
    virtual int performAction(Character& target, Dice& d);
    virtual void speak() const;
};

// Phap su: don thuong ton MP. Skill rieng: Venom Bolt (poison)
class Mage : public Character {
private:
    int mSpellPower;
    static const int SPELL_MP_COST;
public:
    Mage(const std::string& name, int maxHealth, int maxMana,
         int attack, int armor, int spellPower);
    virtual ~Mage();
    virtual int performAction(Character& target, Dice& d);
    virtual void speak() const;
};

// Cung thu: ti le chi mang cao (25%). Skill rieng: Deep Breath (regen)
class Archer : public Character {
public:
    Archer(const std::string& name, int maxHealth, int maxMana,
           int attack, int armor);
    virtual ~Archer();
    virtual int performAction(Character& target, Dice& d);
    virtual void speak() const;
};

// Quai phun axit: trung don thi bi nhiem doc
class Spitter : public Character {
public:
    Spitter(const std::string& name, int maxHealth, int maxMana,
            int attack, int armor);
    virtual ~Spitter();
    virtual int performAction(Character& target, Dice& d);
    virtual void speak() const;
};

// Quai vo cung: trau, giap cao, hay truot, nhung don trung co the lam choang
class Brute : public Character {
public:
    Brute(const std::string& name, int maxHealth, int maxMana,
          int attack, int armor);
    virtual ~Brute();
    virtual int performAction(Character& target, Dice& d);
    virtual void speak() const;
};

// Trum vi khuan: xoay quanh tam (xPivot, yPivot); chu ky 3 luot:
// Toxic Storm (skill rieng) -> Pseudopod Slam (don nang) -> xoay dap
class AmoebaBoss : public Character {
private:
    int mXPivot;
    int mYPivot;
    int mSpinPower;
    int mSpinAngle;
    int mActions;                    // dem so luot da hanh dong
    static const int HIF_SOUND_ID;   // id am thanh *.hif (mo phong)
public:
    AmoebaBoss(const std::string& name, int maxHealth, int maxMana,
               int attack, int armor, int xPivot = 0, int yPivot = 0);
    virtual ~AmoebaBoss();
    void rotateAroundPivot(Dice& d);
    virtual int performAction(Character& target, Dice& d);
    virtual void speak() const;
    virtual void scaleLevel(int level);   // Level Scaling dac thu
};

#endif
