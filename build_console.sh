#!/bin/bash
# Bien dich + chay BAN CONSOLE (chi can core/ + console/)
cd "$(dirname "$0")"
g++ -Wall -I core -o ArenaRPG console/main.cpp core/*.cpp && ./ArenaRPG
