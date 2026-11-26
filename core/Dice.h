// File: Dice.h
// GameException (ngoai le rieng cua game) va lop Dice (xuc xac).
#ifndef DICEHEADER
#define DICEHEADER

#include <exception>
#include <string>

// GameException ke thua std::exception, ghi de what() (Tuan 8)
class GameException : public std::exception {
private:
    std::string message;
public:
    GameException(std::string msg) : message(msg) {}
    virtual ~GameException() throw() {}
    virtual const char* what() const throw() {
        return message.c_str();
    }
};

// Doi so nguyen -> chuoi (de ghep vao thong bao loi)
std::string intToString(int value);

// Dice: xuc xac n mat (Tuan 5)
//  - static const int defaultSides = 6
//  - Precondition: numSides > 1 (assert)
//  - Co the dung doi tuong an danh: Dice(8).roll()
class Dice {
public:
    static const int defaultSides = 6;

    Dice();
    Dice(int numSides);

    int roll();
    int getResult() const;
    int getNumSides() const;

private:
    int iMyNumSides;
    int iMyResult;
};

#endif
