@echo off
cd /d "%~dp0"
g++ -Wall -I core -o ArenaRPG.exe console\main.cpp core\Dice.cpp core\Item.cpp core\StatusAndSkill.cpp core\Character.cpp core\HeroesAndBoss.cpp core\DungeonArena.cpp
if exist ArenaRPG.exe ArenaRPG.exe
pause
