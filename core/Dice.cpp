// File: Dice.cpp
#include "Dice.h"
#include <cstdlib>
#include <cassert>

std::string intToString(int value) {
    if (value == 0) return "0";
    bool negative = (value < 0);
    if (negative) value = -value;
    std::string s = "";
    while (value > 0) {
        s = char('0' + value % 10) + s;
        value = value / 10;
    }
    if (negative) s = "-" + s;
    return s;
}

Dice::Dice() : iMyNumSides(defaultSides), iMyResult(1) {}

/* pre:  numSides > 1
   post: getResult() == 1, getNumSides() == numSides */
Dice::Dice(int numSides) {
    assert(numSides > 1 && "Violation of precondition: Dice(int)");
    iMyNumSides = numSides;
    iMyResult = 1;
    assert(getResult() == 1 && getNumSides() == numSides);
}

/* pre:  iMyNumSides > 1
   post: 1 <= getResult() <= getNumSides() */
int Dice::roll() {
    assert(iMyNumSides > 1);
    iMyResult = (std::rand() % iMyNumSides) + 1;
    assert(iMyResult >= 1 && iMyResult <= iMyNumSides);
    return iMyResult;
}

int Dice::getResult() const { return iMyResult; }
int Dice::getNumSides() const { return iMyNumSides; }
