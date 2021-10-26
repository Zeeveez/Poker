#ifndef DECK_H
#define DECK_H

#include <vector>

#include "Card.h"

namespace Poker {
    class Deck {
    private:
        std::vector<Card> cards;
        std::vector<Card> discardPile;

    public:
        Deck();
        void Shuffle();
        Card Draw();
        std::vector<Card> Draw(int n);
        void Discard(Card card);
        void Discard(std::vector<Card> discards);
    };
}

#endif