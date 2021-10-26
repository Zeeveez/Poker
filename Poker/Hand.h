#ifndef HAND_H
#define HAND_H

#include "Card.h"
#include "Deck.h"

#include <map>
#include <functional>
#include <ostream>

namespace Poker {
    class Hand {
    private:
        std::vector<Card> cards;
        std::pair<bool, Card::Rank> StraightFlush() const;                                    // Rank of high card - A for royal flush
        std::pair<bool, std::pair<Card::Rank, Card::Rank>> FourOfAKind() const;               // Rank of four of a kind, plus rank of kicker
        std::pair<bool, std::pair<Card::Rank, Card::Rank>> FullHouse() const;                 // Rank of triple, plus rank of pair
        std::pair<bool, std::vector<Card::Rank>> Flush() const;                               // Ranks of cards
        std::pair<bool, Card::Rank> Straight() const;                                         // Rank of high card
        std::pair<bool, std::pair<Card::Rank, std::vector<Card::Rank>>> ThreeOfAKind() const; // Rank of triple, plus rank of kickers
        std::pair<bool, std::pair<std::vector<Card::Rank>, Card::Rank>> TwoPair() const;      // Ranks of pairs, plus rank of kicker
        std::pair<bool, std::pair<Card::Rank, std::vector<Card::Rank>>> Pair() const;         // Rank of pair, plus ranks of kickers
        std::vector<Card::Rank> HighCard() const;                                             // Ranks of cards

    public:
        enum class Type
        {
            HIGH_CARD = 0,
            PAIR = 1,
            TWO_PAIR = 2,
            THREE_OF_A_KIND = 3,
            STRAIGHT = 4,
            FLUSH = 5,
            FULL_HOUSE = 6,
            FOUR_OF_A_KIND = 7,
            STRAIGHT_FLUSH = 8
        };
        static std::map<Hand::Type, std::function<std::string(std::vector<Card::Rank>)>> TYPE_NAMES;
        Hand();
        void Draw(Deck& deck);
        void Draw(Deck& deck, int n);
        void Discard(Deck& deck);
        std::pair<Type, std::vector<Card::Rank>> Score() const;
        Hand operator+(const Hand& other);
        friend bool operator<(const Hand& lhs, const Hand& rhs);
        friend bool operator>(const Hand& lhs, const Hand& rhs);
        friend bool operator<=(const Hand& lhs, const Hand& rhs);
        friend bool operator>=(const Hand& lhs, const Hand& rhs);
        friend bool operator==(const Hand& lhs, const Hand& rhs);
        friend bool operator!=(const Hand& lhs, const Hand& rhs);
        friend std::ostream& operator<<(std::ostream& os, const Hand& hand);
    };
}

#endif