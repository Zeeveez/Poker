﻿# CMakeList.txt : CMake project for Poker, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set (CMAKE_CXX_STANDARD 23)

# Add source to this project's executable.
add_executable (Poker "Poker.cpp"  "Hand.cpp" "Hand.h" "Deck.cpp" "Deck.h" "Card.cpp" "Card.h" "Game.cpp" "Game.h" "getch.h")

# TODO: Add tests and install targets if needed.
