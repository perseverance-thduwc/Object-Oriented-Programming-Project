// File: GameEngine.h
// TANG CAU NOI (bridge) giua loi C++ va giao dien web.
//  - Boc lop Game cua core/ (toan bo luat choi nam trong Game).
//  - Nhan 1 lenh dang chu, goi Game, tra ve 1 dong JSON mo ta trang thai.
//  - Moi chu core in ra cout duoc bat lai lam "log" de web hien thi.
#ifndef GAMEENGINEHEADER
#define GAMEENGINEHEADER

#include <string>
#include "Game.h"

class GameEngine {
private:
    Game mGame;

    GameEngine(const GameEngine& other);
    GameEngine& operator=(const GameEngine& other);

    void dispatch(const std::string& command, int a, int b);
    std::string stateJson(const std::string& log) const;

public:
    GameEngine();

    // Lenh: NEW, STATE, HERO i, ATTACK e, SKILL s e, ITEM k, DEFEND,
    //       REWARD t (1 item, 2 skill), BACK, PICK i
    std::string handle(const std::string& line);
};

#endif
