// Poker.cpp : Defines the entry point for the application.
//

#include "Game.h"
#include "Deck.h"
#include "Hand.h"
#include <iostream>


int main()
{
    std::cout << R"(################################################################################)" << "\n";
    std::cout << R"(#     /\                                                              _   _    #)" << "\n";
    std::cout << R"(#    <  >                        ___                                 / \_/ \   #)" << "\n";
    std::cout << R"(#  /\_||_/\                     / __\                               |       |  #)" << "\n";
    std::cout << R"(# <  _  _  >               _/\  \ \___                               \     /   #)" << "\n";
    std::cout << R"(#  \/ || \/               / _ \  \___ \                               \   /    #)" << "\n";
    std::cout << R"(#    /__\                |  \| \__ _/ /                                \_/     #)" << "\n";
    std::cout << R"(#        __            /\ \    __/ \_/                                         #)" << "\n";
    std::cout << R"(#     __/  \  ___  ___/ /_ \__/  __   __  _____  _    ____                     #)" << "\n";
    std::cout << R"(#  __/    _/ / _ \ \_   __|     |  |_|  ||  _  || |  |  _ \                    #)" << "\n";
    std::cout << R"(#  \      \ \ |/ |   / /        |   _   || |_| || |_ | |_| |                   #)" << "\n";
    std::cout << R"(#   \__/\  \ \  /_/\ \/         |__| |__||_____||___||____/                    #)" << "\n";
    std::cout << R"(#        \  \ \____/                   _   ______   __    __           __      #)" << "\n";
    std::cout << R"(#     /\  \_/                         | | |  ____| |  \  /  |         /  \     #)" << "\n";
    std::cout << R"(#    /  \                            /_/  | |__    |   \/   |        /    \    #)" << "\n";
    std::cout << R"(#   /    \                                |  __|   | |\  /| |       /      \   #)" << "\n";
    std::cout << R"(#   \    /                                | |____  | | \/ | |      |  _  _  |  #)" << "\n";
    std::cout << R"(#    \  /                                 |______| |_|    |_|      \_/ || \_/  #)" << "\n";
    std::cout << R"(#     \/                                                              /__\     #)" << "\n";
    std::cout << R"(################################################################################)" << "\n";
    //Poker::Game game(4);
    //game.Start();
    Poker::Deck deck;
    Poker::Hand hand;
    int c = 0;
    for (int i = 0; i < 1000000; i++) {
        hand.Draw(deck, 7);
        if (hand.Score().first == Poker::Hand::Type::STRAIGHT) {
            c++;
        }
        hand.Discard(deck);
    }
    std::cout << 1.0*c/100000;

}
