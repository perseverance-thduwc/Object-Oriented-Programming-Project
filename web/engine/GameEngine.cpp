// File: GameEngine.cpp
#include "GameEngine.h"
#include <iostream>
#include <sstream>

using namespace std;

// ---------------------------------------------------------------------------
// Tien ich JSON
// ---------------------------------------------------------------------------
static string jsonStr(const string& s) {
    string out = "\"";
    for (size_t i = 0; i < s.size(); i++) {
        char c = s[i];
        if (c == '"') out += "\\\"";
        else if (c == '\\') out += "\\\\";
        else if (c == '\n') out += "\\n";
        else if (c == '\r') { }
        else if (c == '\t') out += " ";
        else out += c;
    }
    return out + "\"";
}

static string num(int v) { return intToString(v); }
static string boolStr(bool b) { return b ? "true" : "false"; }

static string charJson(const Character* c) {
    string j = "{";
    j += "\"name\":" + jsonStr(c->getName());
    j += ",\"hp\":" + num(c->getHealth());
    j += ",\"maxHp\":" + num(c->getMaxHealth());
    j += ",\"mp\":" + num(c->getMana());
    j += ",\"maxMp\":" + num(c->getMaxMana());
    j += ",\"armor\":" + num(c->getArmor());
    j += ",\"atk\":" + num(c->getEffectiveAttack());
    j += ",\"crit\":" + num(c->getCritChance());
    j += ",\"miss\":" + num(c->getMissChance());
    j += ",\"alive\":" + boolStr(c->isAlive());
    j += ",\"defending\":" + boolStr(c->isDefending());
    j += ",\"weapon\":" + num(c->getEquippedIndex());
    j += ",\"armorItem\":" + num(c->getEquippedArmorIndex());

    j += ",\"skills\":[";
    for (int i = 0; i < c->skillCount(); i++) {
        if (i > 0) j += ",";
        j += "{\"name\":" + jsonStr(c->skillAt(i)->getSkillName())
           + ",\"mp\":" + num(c->skillAt(i)->getMpCost())
           + ",\"desc\":" + jsonStr(c->skillAt(i)->getDescription()) + "}";
    }
    j += "],\"items\":[";
    for (int i = 0; i < c->itemCount(); i++) {
        if (i > 0) j += ",";
        j += "{\"name\":" + jsonStr(c->itemAt(i)->getName())
           + ",\"type\":" + jsonStr(c->itemAt(i)->getItemType())
           + ",\"desc\":" + jsonStr(c->itemAt(i)->getDescription()) + "}";
    }
    j += "],\"effects\":[";
    for (int i = 0; i < c->activeEffectCount(); i++) {
        if (i > 0) j += ",";
        j += jsonStr(c->effectAt(i)->describe());
    }
    j += "]}";
    return j;
}

GameEngine::GameEngine() {}

// ---------------------------------------------------------------------------
// Chuyen lenh -> lop Game (ma luat choi nam o core/)
// ---------------------------------------------------------------------------
void GameEngine::dispatch(const string& cmd, int a, int b) {
    if (cmd == "NEW") mGame.newGame();
    else if (cmd == "STATE") { }
    else if (cmd == "HERO") mGame.selectHero(a);
    else if (cmd == "ATTACK") mGame.attack(a);
    else if (cmd == "SKILL") mGame.useSkill(a, b);
    else if (cmd == "ITEM") mGame.useItem(a);
    else if (cmd == "DEFEND") mGame.defend();
    else if (cmd == "REWARD") mGame.chooseRewardType(a);
    else if (cmd == "BACK") mGame.backToRewardType();
    else if (cmd == "PICK") mGame.pickOffer(a);
    else throw GameException("Unknown command: " + cmd);
}

string GameEngine::handle(const string& line) {
    istringstream in(line);
    string cmd;
    int a = -1, b = -1;
    in >> cmd >> a >> b;

    ostringstream captured;
    streambuf* old = cout.rdbuf(captured.rdbuf());
    try {
        dispatch(cmd, a, b);
    } catch (GameException& ex) {
        cout << "[Error] " << ex.what() << endl;
    }
    cout.rdbuf(old);
    return stateJson(captured.str());
}

// ---------------------------------------------------------------------------
// JSON trang thai
// ---------------------------------------------------------------------------
string GameEngine::stateJson(const string& log) const {
    const char* names[5] = { "choose_hero", "battle", "reward_type",
                             "reward_pick", "over" };
    const DungeonArena& arena = mGame.getArena();
    string j = "{";
    j += "\"phase\":" + jsonStr(names[mGame.getPhase()]);
    j += ",\"floor\":" + num(arena.getFloor());
    j += ",\"maxFloor\":" + num(DungeonArena::MAX_FLOOR);
    j += ",\"turn\":" + num(mGame.getTurn());
    j += ",\"active\":" + num(mGame.getActive());
    j += ",\"cleared\":" + num(mGame.getCleared());
    j += ",\"entities\":" + num(Character::getTotalCharacters());
    j += ",\"canLearn\":" + boolStr(mGame.canLearnSkill());

    j += ",\"heroes\":[";
    for (int i = 0; i < Game::TEAM_SIZE; i++) {
        if (i > 0) j += ",";
        j += charJson(mGame.getHero(i));
    }
    j += "],\"enemies\":[";
    for (int i = 0; i < arena.enemyCount(); i++) {
        if (i > 0) j += ",";
        string e = charJson(arena.enemyAt(i));
        bool boss = arena.isLastFloor() && i == arena.enemyCount() - 1;
        e.insert(e.size() - 1, string(",\"boss\":") + boolStr(boss));
        j += e;
    }
    j += "]";

    if (mGame.getPhase() == Game::PHASE_REWARD_PICK) {
        j += ",\"rewardType\":" + num(mGame.getRewardType());
        j += ",\"offers\":[";
        for (int i = 0; i < mGame.offerCount(); i++) {
            if (i > 0) j += ",";
            j += jsonStr(mGame.offerDescription(i));
        }
        j += "]";
    }
    if (mGame.getPhase() == Game::PHASE_OVER) {
        int raw, curved;
        char letter;
        mGame.getResult(raw, curved, letter);
        j += ",\"result\":{\"win\":" + boolStr(mGame.isVictory());
        j += ",\"alive\":" + num(mGame.aliveHeroes());
        j += ",\"raw\":" + num(raw);
        j += ",\"score\":" + num(curved);
        j += ",\"letter\":" + jsonStr(string(1, letter)) + "}";
    }
    j += ",\"log\":" + jsonStr(log) + "}";
    return j;
}
