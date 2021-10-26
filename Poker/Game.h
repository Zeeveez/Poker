#ifndef GAME_H
#define GAME_H

#include "Deck.h"
#include "Hand.h"

namespace Poker {
    class Game {
    private:
        static const int STARTING_CASH = 500;

        Deck deck;
        std::size_t buttonPos;
        int pot;
        int minimumBet;

        std::vector<Hand> hands;
        Hand communityCards;
        std::vector<int> cash;
        std::vector<int> bets;
        std::vector<bool> inRound;

    public:
        Game(int players);
        void Start();

    private:
        void PlayRound();
        void BettingRound();
        void PlayerDeal();
        void TableDeal(int n);
        void DrawTable();
        char GetInput(std::string prompt, std::vector<char> validInputs);
    };
}

#endif