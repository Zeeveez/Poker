#include "Deck.h"

#include <algorithm>
#include <random>
#include <chrono>

#include <iostream>

namespace Poker {
    // ----------------------------   Public   ----------------------------
    Deck::Deck() {
        for (int suit = static_cast<int>(Card::Suit::CLUBS); suit <= static_cast<int>(Card::Suit::SPADES); suit++) {
            for (int rank = static_cast<int>(Card::Rank::TWO); rank <= static_cast<int>(Card::Rank::ACE); rank++) {
                cards.push_back(Card(static_cast<Card::Rank>(rank), static_cast<Card::Suit>(suit)));
            }
        }
        Shuffle();
    }

    void Deck::Shuffle() {
        std::shuffle(std::begin(cards), std::end(cards), std::default_random_engine(static_cast<long unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count())));
    }

    Card Deck::Draw() {
        if (cards.size() == 0) {
            cards = discardPile;
            Shuffle();
            discardPile = {};
        }
        Card card = cards.back();
        cards.pop_back();
        return card;
    }

    std::vector<Card> Deck::Draw(int n) {
        std::vector<Card> drawnCards = {};
        for (int i = 0; i < n; i++) {
            drawnCards.push_back(Draw());
        }
        return drawnCards;
    }

    void Deck::Discard(Card card) {
        discardPile.push_back(card);
    }

    void Deck::Discard(std::vector<Card> discards) {
        for (auto& card : discards) {
            discardPile.push_back(card);
        }
    }

    // ----------------------------  Private  ----------------------------
    // ---------------------------- Operators ----------------------------
}