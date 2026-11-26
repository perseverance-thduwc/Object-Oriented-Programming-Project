// File: main.cpp
// Arena Text-based RPG (console) - PHY10609 / PET10001
// Giao dien dong lenh: moi luat choi nam trong lop Game (core/Game.*).
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include "Game.h"
using namespace std;

// ----------------------------------------------------------------------------
// Doc so nguyen trong [lo, hi], nhap sai thi hoi lai
// ----------------------------------------------------------------------------
int readInt(int lo, int hi) {
    int value;
    while (true) {
        cout << "> ";
        if (cin >> value && value >= lo && value <= hi) return value;
        if (cin.eof()) throw GameException("Input ended.");
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "  Enter a number from " << lo << " to " << hi << "." << endl;
    }
}

// ----------------------------------------------------------------------------
// Hien thi
// ----------------------------------------------------------------------------
void printBattlefield(const Game& game) {
    const DungeonArena& arena = game.getArena();
    cout << endl << "=============== FLOOR " << arena.getFloor() << " | TURN "
         << game.getTurn() << " ===============" << endl;
    cout << " HERO:" << endl;
    game.getHero(game.getActive())->printStatus();
    cout << " ENEMIES:" << endl;
    for (int i = 0; i < arena.enemyCount(); i++) {
        arena.enemyAt(i)->printStatus();
    }
}

// Chon muc tieu trong so quai con song, tra ve chi so (hoac -1 neu huy)
int chooseTarget(const Game& game) {
    const DungeonArena& arena = game.getArena();
    int alive = 0, last = -1;
    for (int i = 0; i < arena.enemyCount(); i++) {
        if (arena.enemyAt(i)->isAlive()) { alive++; last = i; }
    }
    if (alive == 1) return last;             // chi con 1 quai -> tu chon

    cout << "  Choose a target (0 = cancel):" << endl;
    for (int i = 0; i < arena.enemyCount(); i++) {
        if (arena.enemyAt(i)->isAlive()) {
            cout << "   " << (i + 1) << ". " << arena.enemyAt(i)->getName()
                 << " HP[" << arena.enemyAt(i)->getHealth() << "]" << endl;
        }
    }
    while (true) {
        int pick = readInt(0, arena.enemyCount());
        if (pick == 0) return -1;
        if (arena.enemyAt(pick - 1)->isAlive()) return pick - 1;
        cout << "  That enemy is dead." << endl;
    }
}

// ----------------------------------------------------------------------------
// Tung pha
// ----------------------------------------------------------------------------
void phaseChooseHero(Game& game) {
    cout << endl << (game.getActive() < 0 ? "Choose your hero:"
                                           : "Your hero has fallen! Choose the next hero:")
         << endl;
    for (int i = 0; i < Game::TEAM_SIZE; i++) {
        if (game.getHero(i)->isAlive()) {
            cout << " " << (i + 1) << ". ";
            game.getHero(i)->printStatus();
        }
    }
    while (true) {
        int pick = readInt(1, Game::TEAM_SIZE) - 1;
        try {
            game.selectHero(pick);
            return;
        } catch (GameException& ex) {
            cout << "  [Error] " << ex.what() << endl;
        }
    }
}

void phaseBattle(Game& game) {
    printBattlefield(game);
    Character* me = game.getHero(game.getActive());
    cout << endl << "--- " << me->getName() << "'s turn ---" << endl;
    cout << " 1. Attack   2. Skills   3. Items   4. Defend" << endl;
    int choice = readInt(1, 4);

    try {
        if (choice == 1) {
            int t = chooseTarget(game);
            if (t >= 0) game.attack(t);
        } else if (choice == 2) {
            cout << "  Choose a skill (0 = cancel):" << endl;
            for (int s = 0; s < me->skillCount(); s++) {
                cout << "   " << (s + 1) << ". "
                     << me->skillAt(s)->getDescription() << endl;
            }
            int s = readInt(0, me->skillCount());
            if (s == 0) return;
            int t = chooseTarget(game);
            if (t >= 0) game.useSkill(s - 1, t);
        } else if (choice == 3) {
            me->printInventory();
            if (me->itemCount() == 0) return;
            cout << "  Choose an item (0 = cancel):" << endl;
            int k = readInt(0, me->itemCount());
            if (k > 0) game.useItem(k - 1);
        } else {
            game.defend();
        }
    } catch (GameException& ex) {
        cout << "  [Error] " << ex.what() << endl;
    }
}

void phaseRewardType(Game& game) {
    cout << endl << "*** Choose your reward ***" << endl;
    cout << " 1. Items   2. Skills" << endl;
    int kind = readInt(1, 2);
    try {
        game.chooseRewardType(kind);
    } catch (GameException& ex) {
        cout << "  [Error] " << ex.what() << endl;
    }
}

void phaseRewardPick(Game& game) {
    cout << endl << "Pick one (0 = go back):" << endl;
    for (int i = 0; i < game.offerCount(); i++) {
        cout << " " << (i + 1) << ". " << game.offerDescription(i) << endl;
    }
    int pick = readInt(0, game.offerCount());
    try {
        if (pick == 0) game.backToRewardType();
        else game.pickOffer(pick - 1);
    } catch (GameException& ex) {
        cout << "  [Error] " << ex.what() << endl;
    }
}

void showResult(const Game& game) {
    int raw, curved;
    char letter;
    game.getResult(raw, curved, letter);
    cout << endl << "================= RESULT =================" << endl;
    cout << (game.isVictory() ? "VICTORY!" : "DEFEAT!") << endl;
    cout << "Floors cleared : " << game.getCleared() << "/" << DungeonArena::MAX_FLOOR << endl;
    cout << "Heroes alive   : " << game.aliveHeroes() << "/" << Game::TEAM_SIZE << endl;
    cout << "Raw score      : " << raw << endl;
    cout << "Curved score   : " << curved << " (+10%)" << endl;
    cout << "Grade          : " << letter << endl;
}

// ----------------------------------------------------------------------------
int main() {
    srand((unsigned int)time(0));
    Game game;

    try {
        while (true) {
            cout << endl << "====== ARENA TEXT-BASED RPG ======" << endl;
            cout << " 1. Play   0. Quit" << endl;
            if (readInt(0, 1) == 0) break;

            game.newGame();
            while (game.getPhase() != Game::PHASE_OVER) {
                switch (game.getPhase()) {
                case Game::PHASE_CHOOSE_HERO: phaseChooseHero(game); break;
                case Game::PHASE_BATTLE:      phaseBattle(game); break;
                case Game::PHASE_REWARD_TYPE: phaseRewardType(game); break;
                case Game::PHASE_REWARD_PICK: phaseRewardPick(game); break;
                }
            }
            showResult(game);
        }
    }
    catch (GameException& ex) {
        cout << "[Game error] " << ex.what() << endl;
    }
    catch (...) {
        cout << "[Unknown error]" << endl;
    }
    return 0;
}
