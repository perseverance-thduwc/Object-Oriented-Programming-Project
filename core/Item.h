// File: Item.h
// He thong vat pham: Item (truu tuong) -> Weapon, Armor, Potion.
// Character so huu Item* (Composition), tro vao Weapon* (Aggregation).
#ifndef ITEMHEADER
#define ITEMHEADER

#include <string>

class Character; // forward declaration

class Item {
protected:
    std::string mName;
    int mValue;
public:
    Item(const std::string& name, int value);
    virtual ~Item();   // virtual destructor: xoa an toan qua Item*

    const std::string& getName() const;
    int getValue() const;

    // Ham thuan ao: moi loai vat pham tu quyet dinh tac dung (Dynamic Binding)
    virtual void applyItem(Character& user) = 0;
    virtual std::string getItemType() const = 0;
    virtual std::string getDescription() const = 0;
};

class Weapon : public Item {
private:
    int mBonusAtk;
public:
    Weapon(const std::string& name, int value, int bonusAtk);
    virtual ~Weapon();

    int getBonusAtk() const;

    virtual void applyItem(Character& user);   // trang bi chinh no
    virtual std::string getItemType() const;
    virtual std::string getDescription() const;
};

class Potion : public Item {
private:
    int mHealAmount;
    int mManaAmount;
public:
    // Tham so mac dinh: manaAmount = 0
    Potion(const std::string& name, int value, int healAmount,
           int manaAmount = 0);
    virtual ~Potion();

    int getHealAmount() const;
    int getManaAmount() const;

    virtual void applyItem(Character& user);   // hoi HP/MP cho user
    virtual std::string getItemType() const;
    virtual std::string getDescription() const;
};

// Giap: trang bi thi cong them ARMOR (Aggregation giong Weapon)
class Armor : public Item {
private:
    int mBonusArmor;
public:
    Armor(const std::string& name, int value, int bonusArmor);
    virtual ~Armor();

    int getBonusArmor() const;

    virtual void applyItem(Character& user);   // mac giap
    virtual std::string getItemType() const;
    virtual std::string getDescription() const;
};

#endif
