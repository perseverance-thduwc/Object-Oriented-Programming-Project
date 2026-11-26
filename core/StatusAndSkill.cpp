// File: StatusAndSkill.cpp
#include "StatusAndSkill.h"
#include "Character.h"
#include <cassert>
#include <iostream>

// ===== StatusEffect =====
StatusEffect::StatusEffect(const std::string& name, int duration)
    : mName(name), mDuration(duration) {
    assert(duration >= 1); // Precondition
}
StatusEffect::~StatusEffect() {}
const std::string& StatusEffect::getName() const { return mName; }
int StatusEffect::getDuration() const { return mDuration; }
bool StatusEffect::isExpired() const { return mDuration <= 0; }
void StatusEffect::decrementTurn() { if (mDuration > 0) mDuration -= 1; }

// ===== PoisonEffect =====
PoisonEffect::PoisonEffect(const std::string& name, int duration,
                           int damagePerTurn)
    : StatusEffect(name, duration), mDamagePerTurn(damagePerTurn) {
    assert(damagePerTurn > 0);
}
PoisonEffect::~PoisonEffect() {}
bool PoisonEffect::onTurnStart(Character& target) {
    target.changeHealth(-mDamagePerTurn);
    std::cout << "  [POISON] " << target.getName() << " is poisoned and loses "
              << mDamagePerTurn << " HP." << std::endl;
    return true;
}
std::string PoisonEffect::describe() const {
    return "POISON '" + mName + "': -" + intToString(mDamagePerTurn)
           + " HP/turn, " + intToString(mDuration) + " turns left";
}

// ===== StunEffect =====
StunEffect::StunEffect(const std::string& name, int duration)
    : StatusEffect(name, duration) {}
StunEffect::~StunEffect() {}
bool StunEffect::onTurnStart(Character& target) {
    std::cout << "  [STUN] " << target.getName()
              << " is stunned and loses the turn!" << std::endl;
    return false;
}
std::string StunEffect::describe() const {
    return "STUN '" + mName + "': skips turn, "
           + intToString(mDuration) + " turns left";
}

// ===== RegenEffect =====
RegenEffect::RegenEffect(const std::string& name, int duration,
                         int healPerTurn)
    : StatusEffect(name, duration), mHealPerTurn(healPerTurn) {
    assert(healPerTurn > 0);
}
RegenEffect::~RegenEffect() {}
bool RegenEffect::onTurnStart(Character& target) {
    target.heal(mHealPerTurn);
    std::cout << "  [REGEN] " << target.getName() << " recovers "
              << mHealPerTurn << " HP." << std::endl;
    return true;
}
std::string RegenEffect::describe() const {
    return "REGEN '" + mName + "': +" + intToString(mHealPerTurn)
           + " HP/turn, " + intToString(mDuration) + " turns left";
}

// ===== Skill =====
Skill::Skill(const std::string& skillName, int mpCost, int power,
             int effectType, int effectDuration)
    : mSkillName(skillName), mMpCost(mpCost), mPower(power),
      mEffectType(effectType), mEffectDuration(effectDuration) {
    assert(mpCost >= 0);
    assert(power > 0);
    assert(effectType >= EFFECT_NONE && effectType <= EFFECT_DRAIN);
    assert(effectDuration >= 1);
}
const std::string& Skill::getSkillName() const { return mSkillName; }
int Skill::getMpCost() const { return mMpCost; }
int Skill::getPower() const { return mPower; }
int Skill::getEffectType() const { return mEffectType; }
int Skill::getEffectDuration() const { return mEffectDuration; }

std::string Skill::getDescription() const {
    std::string s = mSkillName + " (MP " + intToString(mMpCost) + ", power "
                    + intToString(mPower);
    switch (mEffectType) {
    case EFFECT_POISON: s += ", poison"; break;
    case EFFECT_STUN:   s += ", stun"; break;
    case EFFECT_REGEN:  s += ", regen"; break;
    case EFFECT_DRAIN:  s += ", life drain"; break;
    default: break;
    }
    return s + ")";
}

StatusEffect* Skill::createEffect() const {
    int perTurn = mPower / 2;
    if (perTurn < 1) perTurn = 1;
    switch (mEffectType) {
    case EFFECT_POISON:
        return new PoisonEffect(mSkillName + " (Poison)", mEffectDuration, perTurn);
    case EFFECT_STUN:
        return new StunEffect(mSkillName + " (Stun)", mEffectDuration);
    case EFFECT_REGEN:
        return new RegenEffect(mSkillName + " (Regen)", mEffectDuration,
                               perTurn);
    default:
        return 0;
    }
}

int Skill::cast(Character& caster, Character& target, Dice& d) const {
    if (caster.getMana() < mMpCost) {
        throw GameException(caster.getName() + " does not have enough MP for '"
                            + mSkillName + "' (needs " + intToString(mMpCost)
                            + ", has " + intToString(caster.getMana()) + ").");
    }
    caster.changeMana(-mMpCost);

    // Ky nang khong bao gio truot, nhung co the chi mang
    std::cout << "  -> [SKILL] " << caster.getName() << " uses '" << mSkillName
              << "' (-" << mMpCost << " MP)." << std::endl;
    int real = caster.hitTarget(target, mPower + d.roll(), "strikes", false);

    if (mEffectType == EFFECT_DRAIN) {
        int gain = real / 2;
        caster.heal(gain);
        std::cout << "     [DRAIN] " << caster.getName() << " recovers "
                  << gain << " HP." << std::endl;
    }

    StatusEffect* effect = createEffect();
    if (effect != 0) {
        // Regen la hieu ung co loi -> gan len nguoi thi; con lai len muc tieu
        Character& owner = (mEffectType == EFFECT_REGEN) ? caster : target;
        std::cout << "     [EFFECT] " << owner.getName() << ": "
                  << effect->describe() << std::endl;
        owner.addStatusEffect(effect);
    }
    return real;
}
