// File: StatusAndSkill.h
// Hieu ung trang thai (StatusEffect - da hinh) va Ky nang (Skill).
#ifndef STATUSANDSKILLHEADER
#define STATUSANDSKILLHEADER

#include <string>
#include "Dice.h"

class Character; // forward declaration

// ===== StatusEffect: lop co so TRUU TUONG =====
class StatusEffect {
protected:
    std::string mName;
    int mDuration;
public:
    StatusEffect(const std::string& name, int duration);
    virtual ~StatusEffect();

    const std::string& getName() const;
    int getDuration() const;
    bool isExpired() const;
    void decrementTurn();

    // Goi dau luot. Tra ve true neu nhan vat duoc hanh dong,
    // false neu bi Choang (mat luot).
    virtual bool onTurnStart(Character& target) = 0;
    virtual std::string describe() const = 0;
};

// Doc: tru mau moi dau luot
class PoisonEffect : public StatusEffect {
private:
    int mDamagePerTurn;
public:
    PoisonEffect(const std::string& name, int duration, int damagePerTurn);
    virtual ~PoisonEffect();
    virtual bool onTurnStart(Character& target);
    virtual std::string describe() const;
};

// Choang: mat luot hanh dong
class StunEffect : public StatusEffect {
public:
    StunEffect(const std::string& name, int duration);
    virtual ~StunEffect();
    virtual bool onTurnStart(Character& target);
    virtual std::string describe() const;
};

// Hoi mau theo luot
class RegenEffect : public StatusEffect {
private:
    int mHealPerTurn;
public:
    RegenEffect(const std::string& name, int duration, int healPerTurn);
    virtual ~RegenEffect();
    virtual bool onTurnStart(Character& target);
    virtual std::string describe() const;
};

// ===== Skill =====
class Skill {
public:
    static const int EFFECT_NONE = 0;
    static const int EFFECT_POISON = 1;  // gan len MUC TIEU
    static const int EFFECT_STUN = 2;    // gan len MUC TIEU
    static const int EFFECT_REGEN = 3;   // gan len NGUOI THI
    static const int EFFECT_DRAIN = 4;   // nguoi thi hoi mot nua sat thuong gay ra

private:
    std::string mSkillName;
    int mMpCost;
    int mPower;
    int mEffectType;
    int mEffectDuration;

public:
    Skill(const std::string& skillName, int mpCost, int power,
          int effectType = EFFECT_NONE, int effectDuration = 3);

    const std::string& getSkillName() const;
    int getMpCost() const;
    int getPower() const;
    int getEffectType() const;
    int getEffectDuration() const;
    std::string getDescription() const;   // vd: "Fire Burst (MP 10, power 16)"

    // Tao hieu ung moi (new) theo effectType; tra ve 0 neu EFFECT_NONE
    StatusEffect* createEffect() const;

    // 1) Tru MP (throw GameException neu thieu MP)
    // 2) Gay sat thuong = power + xuc xac len target
    // 3) Gan hieu ung: Poison/Stun -> target, Regen -> caster
    // Tra ve sat thuong thuc te
    int cast(Character& caster, Character& target, Dice& d) const;
};

#endif
