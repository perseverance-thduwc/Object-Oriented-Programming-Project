// File: Game.h
// Lop dieu phoi TOAN BO luat choi (state machine). Console va Web deu dung lop nay.
//
// Luat:
//  - Chon 1 trong 3 hero de choi. Hero do choi lien tuc qua cac man cho den khi guc.
//  - Hero guc -> chon 1 trong cac hero con lai de vao tran TIEP (quai giu nguyen mau).
//  - Ca 3 hero guc -> thua. Qua man 5 (co trum) -> thang.
//  - Moi luot: Attack / Skill / Item / Defend.
//  - Xong moi man: chon thuong ITEM hoac SKILL, roi chon 1 trong 3 phan thuong.
#ifndef GAMEHEADER
#define GAMEHEADER

#include <string>
#include "Dice.h"
#include "Character.h"
#include "DungeonArena.h"

class Game {
public:
    static const int TEAM_SIZE = 3;

    static const int PHASE_CHOOSE_HERO = 0;   // chon hero (dau game hoac khi hero guc)
    static const int PHASE_BATTLE = 1;        // cho lenh cua hero
    static const int PHASE_REWARD_TYPE = 2;   // chon thuong Item hay Skill
    static const int PHASE_REWARD_PICK = 3;   // chon 1 trong 3 phan thuong
    static const int PHASE_OVER = 4;          // ket thuc

    static const int REWARD_ITEM = 1;
    static const int REWARD_SKILL = 2;

private:
    Character* mHeroes[TEAM_SIZE];
    DungeonArena mArena;
    Dice mD6;
    int mPhase;
    int mActive;         // hero dang choi (-1 neu chua chon)
    int mTurn;
    int mCleared;        // so man da vuot
    int mRewardType;
    int mOffers[3];
    int mOfferCount;

    Game(const Game& other);
    Game& operator=(const Game& other);

    void deleteHeroes();
    Character* activeHero() const;
    void requirePhase(int phase, const std::string& what) const;
    void requireEnemy(int enemy) const;

    void startFloor();
    void beginHeroTurn();
    void endHeroAction();
    void enemyPhase();
    void floorWon();
    void heroDown();
    void makeOffers();
    std::string describeOffer(int kind) const;

public:
    Game();
    ~Game();

    void newGame();

    // ----- Truy van trang thai -----
    int getPhase() const;
    int getActive() const;
    int getTurn() const;
    int getCleared() const;
    int aliveHeroes() const;
    Character* getHero(int index) const;
    const DungeonArena& getArena() const;
    int getRewardType() const;
    int offerCount() const;
    std::string offerDescription(int index) const;
    bool canLearnSkill() const;
    bool isVictory() const;
    // Diem: goc, sau duong cong (CurvedActivity), xep hang
    void getResult(int& raw, int& curved, char& letter) const;

    // ----- Lenh (nem GameException neu khong hop le) -----
    void selectHero(int index);
    void attack(int enemy);
    void useSkill(int skill, int enemy);
    void useItem(int index);
    void defend();
    void chooseRewardType(int type);
    void backToRewardType();
    void pickOffer(int index);
};

#endif
