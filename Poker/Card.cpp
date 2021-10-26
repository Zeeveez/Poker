#include "Card.h"

namespace Poker {
    // ----------------------------   Public   ----------------------------
    Card::Card(Rank rank, Suit suit) : rank(rank), suit(suit) {}

    std::map<Card::Rank, std::string> Card::RANK_NAMES = {
            { Rank::TWO, "Two" },
            { Rank::THREE, "Three" },
            { Rank::FOUR, "Four" },
            { Rank::FIVE, "Five" },
            { Rank::SIX, "Six" },
            { Rank::SEVEN, "Seven" },
            { Rank::EIGHT, "Eight" },
            { Rank::NINE, "Nine" },
            { Rank::TEN, "Ten" },
            { Rank::JACK, "Jack" },
            { Rank::QUEEN, "Queen" },
            { Rank::KING, "King" },
            { Rank::ACE, "Ace" },
    };

    std::map<Card::Suit, std::string> Card::SUIT_NAMES = {
            { Suit::CLUBS, "Clubs" },
            { Suit::DIAMONDS, "Diamonds" },
            { Suit::HEARTS, "Hearts" },
            { Suit::SPADES, "Spades" },
    };

    // ----------------------------  Private  ----------------------------

    // ---------------------------- Operators ----------------------------
    std::ostream& operator<<(std::ostream& os, const Card& card)
    {
        os << Card::RANK_NAMES.find(card.rank)->second << " of " << Card::SUIT_NAMES.find(card.suit)->second;
        return os;
    }
};