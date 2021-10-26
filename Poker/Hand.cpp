#include "Hand.h"

#include <stdint.h>

#include <algorithm>
#include <cmath>
#include <format>

namespace Poker {
    // ----------------------------   Public   ----------------------------
    Hand::Hand() : cards({}) {}

    std::map<Hand::Type, std::function<std::string(std::vector<Card::Rank>)>> Hand::TYPE_NAMES = {
        { Type::HIGH_CARD, [](std::vector<Card::Rank> ranks) {
            return std::format("{} High", Card::RANK_NAMES[ranks[0]]);
        } },
        { Type::PAIR, [](std::vector<Card::Rank> ranks) {
            return std::format("Pair of {}s", Card::RANK_NAMES[ranks[0]]);
        } },
        { Type::TWO_PAIR, [](std::vector<Card::Rank> ranks) {
            return std::format("Two Pair, {}s and {}s", Card::RANK_NAMES[ranks[0]], Card::RANK_NAMES[ranks[1]]);
        } },
        { Type::THREE_OF_A_KIND, [](std::vector<Card::Rank> ranks) {
            return std::format("Three {}s", Card::RANK_NAMES[ranks[0]]);
        } },
        { Type::STRAIGHT, [](std::vector<Card::Rank> ranks) {
            return std::format("{} High Straight", Card::RANK_NAMES[ranks[0]]);
        } },
        { Type::FLUSH, [](std::vector<Card::Rank> ranks) {
            return "Flush";
        } },
        { Type::FULL_HOUSE, [](std::vector<Card::Rank> ranks) {
            return std::format("Full House, {}s full of {}s", Card::RANK_NAMES[ranks[0]], Card::RANK_NAMES[ranks[1]]);
        } },
        { Type::FOUR_OF_A_KIND, [](std::vector<Card::Rank> ranks) {
            return std::format("Four {}s", Card::RANK_NAMES[ranks[0]]);
        } },
        { Type::STRAIGHT_FLUSH, [](std::vector<Card::Rank> ranks) {
            return std::format("{} High Straight Flush", Card::RANK_NAMES[ranks[0]]);
        } }
    };

    void Hand::Draw(Deck& deck) {
        cards.push_back(deck.Draw());
    }

    void Hand::Draw(Deck& deck, int n) {
        for (int i = 0; i < n; i++) {
            Draw(deck);
        }
    }

    void Hand::Discard(Deck& deck) {
        deck.Discard(cards);
        cards = {};
    }

    std::pair<Hand::Type, std::vector<Card::Rank>>  Hand::Score() const {

        auto straightFlushResults = StraightFlush();
        if (straightFlushResults.first) {
            return { Hand::Type::STRAIGHT_FLUSH, { straightFlushResults.second } };
        }

        auto fourOfAKindResults = FourOfAKind();
        if (fourOfAKindResults.first) {
            return { Hand::Type::FOUR_OF_A_KIND, { fourOfAKindResults.second.first, fourOfAKindResults.second.second } };
        }

        auto fullHouseResults = FullHouse();
        if (fullHouseResults.first) {
            return { Hand::Type::FULL_HOUSE, { fullHouseResults.second.first, fullHouseResults.second.second } };
        }

        auto flushResults = Flush();
        if (flushResults.first) {
            return { Hand::Type::FLUSH, flushResults.second };
        }

        auto straightResults = Straight();
        if (straightResults.first) {
            return { Hand::Type::STRAIGHT, { straightResults.second } };
        }

        auto threeOfAKindResults = ThreeOfAKind();
        if (threeOfAKindResults.first) {
            return { Hand::Type::THREE_OF_A_KIND, { threeOfAKindResults.second.first, threeOfAKindResults.second.second[0], threeOfAKindResults.second.second[1] } };
        }

        auto twoPairResults = TwoPair();
        if (twoPairResults.first) {
            return { Hand::Type::TWO_PAIR, { twoPairResults.second.first[0], twoPairResults.second.first[1], twoPairResults.second.second } };
        }

        auto pairResults = Pair();
        if (pairResults.first) {
            return { Hand::Type::PAIR, { pairResults.second.first, pairResults.second.second[0], pairResults.second.second[1], pairResults.second.second[2] } };
        }

        return { Hand::Type::HIGH_CARD, HighCard() };
    }

    // ----------------------------  Private  ----------------------------
    std::pair<bool, Card::Rank> Hand::StraightFlush() const {
        // T
        static const std::uint64_t STRAIGHT_FLUSH = 0b00000000000000000000000000000000000000000000000000011111;
        static const std::uint64_t STRAIGHT_FLUSH_LOW_ACE = 0b0000000000000000000000000000000000000000000000000001000000001111;

        uint64_t hand = 0;
        for (auto& card : cards) {
            hand |= (uint64_t)1 << (static_cast<int>(card.suit) * (1 + static_cast<int>(Card::Rank::ACE) - static_cast<int>(Card::Rank::TWO)) + (static_cast<int>(card.rank) - static_cast<int>(Card::Rank::TWO)));
        }

        for (uint64_t rankOffset = 0; rankOffset < 9; rankOffset++) {
            for (uint64_t suit = static_cast<uint64_t>(Card::Suit::CLUBS); suit <= static_cast<uint64_t>(Card::Suit::SPADES); suit++) {
                if ((hand & (STRAIGHT_FLUSH << 13 * suit + 8 - rankOffset)) == (STRAIGHT_FLUSH << 13 * suit + 8 - rankOffset)) {
                    return { true, static_cast<Card::Rank>(static_cast<int>(Card::Rank::SIX) + 8 - rankOffset) };
                }
            }
        }
        for (uint64_t suit = static_cast<uint64_t>(Card::Suit::CLUBS); suit <= static_cast<uint64_t>(Card::Suit::SPADES); suit++) {
            if ((hand & (STRAIGHT_FLUSH_LOW_ACE << 13 * suit)) == (STRAIGHT_FLUSH_LOW_ACE << 13 * suit)) {
                return { true, Card::Rank::FIVE };
            }
        }
        return { false, Card::Rank::ACE };
    }

    std::pair<bool, std::pair<Card::Rank, Card::Rank>> Hand::FourOfAKind() const {
        std::vector<int> counts = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }; // _,_,2,3,4,5,6,7,8,9,X,J,Q,K,A
        for (auto& card : cards) { counts[static_cast<std::size_t>(card.rank)]++; }

        for (std::size_t i = counts.size() - 1; i > 0; i--) {
            if (counts[i] >= 4) {
                for (std::size_t j = counts.size() - 1; j > 0; j--) {
                    if (counts[j] >= 1 && i != j) {
                        return { true, { static_cast<Card::Rank>(i), static_cast<Card::Rank>(j) } };
                    }
                }
            }
        }
        return { false, {} };
    }

    std::pair<bool, std::pair<Card::Rank, Card::Rank>> Hand::FullHouse() const {
        std::vector<int> counts = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }; // _,_,2,3,4,5,6,7,8,9,X,J,Q,K,A
        for (auto& card : cards) { counts[static_cast<std::size_t>(card.rank)]++; }

        for (std::size_t i = counts.size() - 1; i > 0; i--) {
            if (counts[i] >= 3) {
                for (std::size_t j = counts.size() - 1; j > 0; j--) {
                    if (counts[j] >= 2 && i != j) {
                        return { true, { static_cast<Card::Rank>(i), static_cast<Card::Rank>(j) } };
                    }
                }
            }
        }
        return { false, { } };
    }

    std::pair<bool, std::vector<Card::Rank>> Hand::Flush() const {
        std::vector<int> counts = { 0,0,0,0 }; // C,D,H,S
        for (auto& card : cards) { counts[static_cast<std::size_t>(card.suit)]++; }

        for (std::size_t i = static_cast<std::size_t>(Card::Suit::CLUBS); i <= static_cast<std::size_t>(Card::Suit::SPADES); i++) {
            if (counts[i] >= 5) {
                std::vector<Card::Rank> ranks = {};
                for (auto& card : cards) {
                    if (static_cast<std::size_t>(card.suit) == i) {
                        ranks.push_back(card.rank);
                    }
                }
                std::sort(std::begin(ranks), std::end(ranks), [](Card::Rank& a, Card::Rank& b) {
                    return static_cast<int>(a) > static_cast<int>(b);
                    });
                return { true, { ranks[0], ranks[1], ranks[2], ranks[3], ranks[4] } };
            }
        }
        return { false, { } };
    }

    std::pair<bool, Card::Rank> Hand::Straight() const {
        std::vector<int> counts = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }; // _,_,2,3,4,5,6,7,8,9,X,J,Q,K,A
        for (auto& card : cards) { counts[static_cast<std::size_t>(card.rank)]++; }
        counts[1] = counts[counts.size() - 1]; // _,A,2,3,4,5,6,7,8,9,X,J,Q,K,A

        for (std::size_t i = counts.size() - 1; i >= 4; i--) {
            bool straight = true;
            for (std::size_t j = 0; j < 5; j++) {
                if (!counts[i - j]) {
                    straight = false;
                    break;
                }
            }
            if (straight) {
                return { true, static_cast<Card::Rank>(i) };
            }
        }
        return { false, Card::Rank::ACE };
    }

    std::pair<bool, std::pair<Card::Rank, std::vector<Card::Rank>>> Hand::ThreeOfAKind() const {
        std::vector<int> counts = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }; // _,_,2,3,4,5,6,7,8,9,X,J,Q,K,A
        for (auto& card : cards) { counts[static_cast<std::size_t>(card.rank)]++; }

        for (std::size_t i = counts.size() - 1; i > 0; i--) {
            if (counts[i] >= 3) {
                std::vector<Card::Rank> kickers = {};
                for (std::size_t j = counts.size() - 1; j > 0; j--) {
                    if (counts[j] >= 1 && i != j) {
                        kickers.push_back(static_cast<Card::Rank>(j));
                    }
                    if (kickers.size() >= 2) {
                        return { true, { static_cast<Card::Rank>(i), kickers } };
                    }
                }
            }
        }
        return { false, {} };
    }

    std::pair<bool, std::pair<std::vector<Card::Rank>, Card::Rank>> Hand::TwoPair() const {
        std::vector<int> counts = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }; // _,_,2,3,4,5,6,7,8,9,X,J,Q,K,A
        for (auto& card : cards) { counts[static_cast<std::size_t>(card.rank)]++; }

        for (std::size_t i = counts.size() - 1; i > 0; i--) {
            if (counts[i] >= 2) { // First pair
                for (std::size_t j = counts.size() - 1; j > 0; j--) {
                    if (counts[j] >= 2 && i != j) { // Second pair
                        for (std::size_t k = counts.size() - 1; k > 0; k--) {
                            if (counts[j] >= 1 && i != j && i != k && j != k) { // Kicker
                                return { true, { { static_cast<Card::Rank>(i), static_cast<Card::Rank>(j) }, static_cast<Card::Rank>(k) } };
                            }
                        }
                    }
                }
            }
        }
        return { false, {} };
    }

    std::pair<bool, std::pair<Card::Rank, std::vector<Card::Rank>>> Hand::Pair() const {
        std::vector<int> counts = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }; // _,_,2,3,4,5,6,7,8,9,X,J,Q,K,A
        for (auto& card : cards) { counts[static_cast<std::size_t>(card.rank)]++; }

        for (std::size_t i = counts.size() - 1; i > 0; i--) {
            if (counts[i] >= 2) {
                std::vector<Card::Rank> kickers = {};
                for (std::size_t j = counts.size() - 1; j > 0; j--) {
                    if (counts[j] >= 1 && i != j) {
                        kickers.push_back(static_cast<Card::Rank>(j));
                    }
                    if (kickers.size() >= 3) {
                        return { true, { static_cast<Card::Rank>(i), kickers } };
                    }
                }
            }
        }
        return { false, {} };
    }

    std::vector<Card::Rank> Hand::HighCard() const {
        std::vector<Card::Rank> kickers = {};
        for (auto& card : cards) { kickers.push_back(card.rank); }
        std::sort(std::begin(kickers), std::end(kickers), [](Card::Rank& a, Card::Rank& b) {
            return static_cast<int>(a) > static_cast<int>(b);
            });
        return kickers;
    }

    // ---------------------------- Operators ----------------------------
    Hand Hand::operator+(const Hand& other) {
        Hand h = {};
        for (auto& card : cards) {
            h.cards.push_back(card);
        }
        for (auto& card : other.cards) {
            h.cards.push_back(card);
        }
        return h;
    }

    bool operator<(const Hand& lhs, const Hand& rhs) {
        auto lhsScore = lhs.Score();
        auto rhsScore = rhs.Score();
        if (lhsScore.first < rhsScore.first) {
            return true;
        }
        if (lhsScore.first > rhsScore.first) {
            return false;
        }
        for (std::size_t i = 0; i < lhsScore.second.size(); i++) {
            if (lhsScore.second[i] < rhsScore.second[i]) {
                return true;
            }
            if (lhsScore.second[i] > rhsScore.second[i]) {
                return false;
            }
        }
        return false;
    }

    bool operator>(const Hand& lhs, const Hand& rhs) {
        return rhs < lhs;
    }

    bool operator<=(const Hand& lhs, const Hand& rhs) {
        return !(lhs > rhs);
    }

    bool operator>=(const Hand& lhs, const Hand& rhs) {
        return !(lhs < rhs);
    }

    bool operator==(const Hand& lhs, const Hand& rhs) {
        auto lhsScore = lhs.Score();
        auto rhsScore = rhs.Score();
        if (lhsScore.first == rhsScore.first) {
            for (std::size_t i = 0; i < lhsScore.second.size(); i++) {
                if (lhsScore.second[i] != rhsScore.second[i]) {
                    return false;
                }
            }
            return true;
        }
        return false;
    }

    bool operator!=(const Hand& lhs, const Hand& rhs) {
        return !(lhs == rhs);
    }

    std::ostream& operator<<(std::ostream& os, const Hand& hand) {
        os << "Cards:\n";
        for (auto& card : hand.cards) {
            os << "\t" << card << "\n";
        }
        return os;
    }
}