#ifndef CARD_H
#define CARD_H

#include <ostream>
#include <string>
#include <map>

namespace Poker {
    class Card {
    public:
        enum class Rank
        {
            TWO = 2,
            THREE = 3,
            FOUR = 4,
            FIVE = 5,
            SIX = 6,
            SEVEN = 7,
            EIGHT = 8,
            NINE = 9,
            TEN = 10,
            JACK = 11,
            QUEEN = 12,
            KING = 13,
            ACE = 14
        };
        enum class Suit
        {
            CLUBS = 0,
            DIAMONDS = 1,
            HEARTS = 2,
            SPADES = 3
        };

        static std::map<Rank, std::string> RANK_NAMES;
        static std::map<Suit, std::string> SUIT_NAMES;

        Rank rank;
        Suit suit;

        Card(Rank rank, Suit suit);

        friend std::ostream& operator<< (std::ostream& os, const Card& card);
    };
}
#endif