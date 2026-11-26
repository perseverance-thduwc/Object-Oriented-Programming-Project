// File: Game.cpp
#include "Game.h"
#include "HeroesAndBoss.h"
#include "BattleGrade.h"
#include <iostream>

using namespace std;

// ---------------------------------------------------------------------------
Game::Game()
    : mPhase(PHASE_CHOOSE_HERO), mActive(-1), mTurn(1), mCleared(0),
      mRewardType(0), mOfferCount(0)
{
    for (int i = 0; i < TEAM_SIZE; i++) mHeroes[i] = 0;
    for (int i = 0; i < 3; i++) mOffers[i] = 0;
    newGame();
}

Game::~Game() {
    deleteHeroes();
    mArena.clearEnemies();
}

void Game::deleteHeroes() {
    for (int i = 0; i < TEAM_SIZE; i++) {
        delete mHeroes[i];               // virtual destructor
        mHeroes[i] = 0;
    }
}

void Game::newGame() {
    deleteHeroes();
    mArena.clearEnemies();
    mArena.setFloor(1);

    mHeroes[0] = new Warrior("Warrior", 140, 25, 18, 4);
    mHeroes[1] = new Mage("Mage", 100, 60, 6, 1, 20);
    mHeroes[2] = new Archer("Archer", 110, 35, 14, 2);
    for (int i = 0; i < TEAM_SIZE; i++) {
        mHeroes[i]->addItem(new Potion("Health Potion", 10, 40, 10));
        mHeroes[i]->setX(i).setY(0);      // chained calls
    }
    mPhase = PHASE_CHOOSE_HERO;
    mActive = -1;
    mTurn = 1;
    mCleared = 0;
    mOfferCount = 0;
}

// ---------------------------------------------------------------------------
// Truy van
// ---------------------------------------------------------------------------
int Game::getPhase() const { return mPhase; }
int Game::getActive() const { return mActive; }
int Game::getTurn() const { return mTurn; }
int Game::getCleared() const { return mCleared; }
Character* Game::getHero(int index) const { return mHeroes[index]; }
const DungeonArena& Game::getArena() const { return mArena; }
int Game::getRewardType() const { return mRewardType; }
int Game::offerCount() const { return mOfferCount; }
Character* Game::activeHero() const { return mHeroes[mActive]; }

int Game::aliveHeroes() const {
    int n = 0;
    for (int i = 0; i < TEAM_SIZE; i++) {
        if (mHeroes[i]->isAlive()) n++;
    }
    return n;
}

bool Game::isVictory() const {
    return mCleared >= DungeonArena::MAX_FLOOR;
}

bool Game::canLearnSkill() const {
    if (mActive < 0) return false;
    Character* hero = activeHero();
    if (hero->skillCount() >= Character::MAX_SKILLS) return false;
    for (int k = 0; k < DungeonArena::SKILL_KINDS; k++) {
        if (!hero->hasSkill(mArena.skillName(k))) return true;
    }
    return false;
}

std::string Game::describeOffer(int kind) const {
    if (mRewardType == REWARD_SKILL) {
        Skill* s = mArena.createSkill(kind);
        std::string text = s->getDescription();
        delete s;
        return text;
    }
    Item* it = mArena.createItem(kind);
    std::string text = it->getDescription();
    delete it;
    return text;
}

std::string Game::offerDescription(int index) const {
    return describeOffer(mOffers[index]);
}

void Game::getResult(int& raw, int& curved, char& letter) const {
    double score = mCleared * 16.0 + aliveHeroes() * 6.0;
    CurvedActivity grade;                  // redefining setScore
    grade.setPercentage(1.10);
    grade.setScore(score);
    raw = (int)grade.getRawScore();
    curved = (int)grade.getScore();
    letter = grade.getLetterGrade();
}

// ---------------------------------------------------------------------------
// Kiem tra lenh
// ---------------------------------------------------------------------------
void Game::requirePhase(int phase, const std::string& what) const {
    if (mPhase != phase) {
        throw GameException("You cannot " + what + " right now.");
    }
}

void Game::requireEnemy(int enemy) const {
    if (enemy < 0 || enemy >= mArena.enemyCount()
        || !mArena.enemyAt(enemy)->isAlive()) {
        throw GameException("Invalid target.");
    }
}

// ---------------------------------------------------------------------------
// Luong tran dau
// ---------------------------------------------------------------------------
void Game::selectHero(int index) {
    requirePhase(PHASE_CHOOSE_HERO, "choose a hero");
    if (index < 0 || index >= TEAM_SIZE || !mHeroes[index]->isAlive()) {
        throw GameException("Invalid hero choice.");
    }
    mActive = index;
    if (mArena.livingEnemyCount() > 0) {          // thay hero giua tran
        cout << "  " << mHeroes[index]->getName() << " steps into the battle!"
             << endl;
        mPhase = PHASE_BATTLE;
        beginHeroTurn();
    } else {
        startFloor();
    }
}

void Game::startFloor() {
    mArena.spawnEnemies(mD6);
    mArena.enemyAt(mArena.enemyCount() - 1)->speak();
    mTurn = 1;
    mPhase = PHASE_BATTLE;
    beginHeroTurn();
}

// Dau luot hero: het phong thu, ap dung hieu ung; neu bi choang thi ke dich danh luon
void Game::beginHeroTurn() {
    Character* hero = activeHero();
    while (true) {
        if (mArena.livingEnemyCount() == 0) { floorWon(); return; }
        if (!hero->isAlive()) { heroDown(); return; }

        hero->endDefend();
        bool canAct = hero->processTurnEffects();
        if (!hero->isAlive()) { heroDown(); return; }   // chet vi doc
        if (canAct) return;                             // cho lenh nguoi choi

        enemyPhase();                                   // hero mat luot
        mTurn++;
    }
}

void Game::endHeroAction() {
    if (mArena.livingEnemyCount() == 0) { floorWon(); return; }
    enemyPhase();
    mTurn++;
    beginHeroTurn();
}

void Game::enemyPhase() {
    Character* hero = activeHero();
    cout << "--- Enemy turn ---" << endl;
    for (int i = 0; i < mArena.enemyCount(); i++) {
        Character* e = mArena.enemyAt(i);
        if (!e->isAlive() || !hero->isAlive()) continue;
        if (!e->processTurnEffects() || !e->isAlive()) continue;
        try {
            e->performAction(*hero, mD6);                // Dynamic Binding
        } catch (GameException& ex) {                    // quai phap su het MP
            cout << "  " << ex.what() << endl;
            e->hitTarget(*hero, e->getEffectiveAttack(), "punches", true);
        }
    }
}

void Game::floorWon() {
    mCleared++;
    cout << "*** Floor " << mArena.getFloor() << " cleared! ***" << endl;
    if (mCleared >= DungeonArena::MAX_FLOOR) {
        mArena.clearEnemies();
        mPhase = PHASE_OVER;
        return;
    }
    Character* hero = activeHero();                      // nghi ngoi nhe
    hero->heal(0.5);
    hero->changeMana(hero->getMaxMana() / 3);
    cout << "  " << hero->getName() << " catches their breath." << endl;
    mPhase = PHASE_REWARD_TYPE;
}

void Game::heroDown() {
    cout << "  " << activeHero()->getName() << " has fallen!" << endl;
    if (aliveHeroes() == 0) {
        mArena.clearEnemies();
        mPhase = PHASE_OVER;
    } else {
        mPhase = PHASE_CHOOSE_HERO;                      // chon hero ke tiep
    }
}

// ---------------------------------------------------------------------------
// Lenh trong tran
// ---------------------------------------------------------------------------
void Game::attack(int enemy) {
    requirePhase(PHASE_BATTLE, "attack");
    requireEnemy(enemy);
    Character* hero = activeHero();
    Character* target = mArena.enemyAt(enemy);
    try {
        hero->performAction(*target, mD6);
    } catch (GameException& ex) {            // het MP -> danh tay
        cout << "  [Error] " << ex.what() << endl;
        hero->hitTarget(*target, hero->getEffectiveAttack(), "punches", true);
    }
    endHeroAction();
}

void Game::useSkill(int skill, int enemy) {
    requirePhase(PHASE_BATTLE, "use a skill");
    requireEnemy(enemy);
    activeHero()->castSkill(skill, *mArena.enemyAt(enemy), mD6);  // throw neu thieu MP
    endHeroAction();
}

void Game::useItem(int index) {
    requirePhase(PHASE_BATTLE, "use an item");
    Character* hero = activeHero();
    if (index < 0 || index >= hero->itemCount()) {
        throw GameException("Invalid inventory slot.");
    }
    bool isPotion = (hero->itemAt(index)->getItemType() == "Potion");
    hero->useItem(index);
    if (isPotion) endHeroAction();           // Potion ton luot; trang bi thi khong
}

void Game::defend() {
    requirePhase(PHASE_BATTLE, "defend");
    activeHero()->defend();
    endHeroAction();
}

// ---------------------------------------------------------------------------
// Phan thuong sau man
// ---------------------------------------------------------------------------
void Game::makeOffers() {
    Character* hero = activeHero();
    int kinds[8];
    int n = 0;
    if (mRewardType == REWARD_ITEM) {
        for (int k = 0; k < DungeonArena::ITEM_KINDS; k++) kinds[n++] = k;
    } else {
        for (int k = 0; k < DungeonArena::SKILL_KINDS; k++) {
            if (!hero->hasSkill(mArena.skillName(k))) kinds[n++] = k;
        }
    }
    mOfferCount = 0;
    while (mOfferCount < 3 && n > 0) {       // chon toi da 3 loai khac nhau
        int idx = (n == 1) ? 0 : Dice(n).roll() - 1;    // Dice an danh
        mOffers[mOfferCount++] = kinds[idx];
        kinds[idx] = kinds[n - 1];
        n--;
    }
}

void Game::chooseRewardType(int type) {
    requirePhase(PHASE_REWARD_TYPE, "choose a reward");
    if (type != REWARD_ITEM && type != REWARD_SKILL) {
        throw GameException("Invalid reward type.");
    }
    if (type == REWARD_SKILL && !canLearnSkill()) {
        throw GameException("There are no new skills to learn.");
    }
    mRewardType = type;
    makeOffers();
    mPhase = PHASE_REWARD_PICK;
}

void Game::backToRewardType() {
    requirePhase(PHASE_REWARD_PICK, "go back");
    mPhase = PHASE_REWARD_TYPE;
}

void Game::pickOffer(int index) {
    requirePhase(PHASE_REWARD_PICK, "pick a reward");
    if (index < 0 || index >= mOfferCount) {
        throw GameException("Invalid reward choice.");
    }
    Character* hero = activeHero();
    int kind = mOffers[index];

    if (mRewardType == REWARD_ITEM) {
        if (hero->itemCount() >= Character::MAX_ITEMS) {
            throw GameException("Inventory is full! Use an item first or learn a skill.");
        }
        Item* it = mArena.createItem(kind);
        hero->addItem(it);
        cout << "  [REWARD] " << hero->getName() << " receives "
             << it->getDescription() << std::endl;
        if (it->getItemType() != "Potion") {
            hero->useItem(hero->itemCount() - 1);        // tu dong trang bi
        }
    } else {
        Skill* s = mArena.createSkill(kind);
        hero->addSkill(s);
        cout << "  [LEARN] " << hero->getName() << " learns "
             << s->getDescription() << std::endl;
    }
    mArena.setFloor(mArena.getFloor() + 1);
    startFloor();
}
