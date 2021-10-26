#include "Game.h"
#include "getch.h"

#include <iostream>
#include <algorithm>
#include <cstdio>



namespace Poker {
    // ----------------------------   Public   ----------------------------
    Game::Game(int players) :deck(Deck()), buttonPos(0), pot(0), minimumBet(STARTING_CASH / 10), hands({}), communityCards({}), cash({}), bets({}), inRound({}) {
        for (int i = 0; i < players; i++) {
            hands.push_back({});
        }
        for (int i = 0; i < players; i++) {
            cash.push_back(STARTING_CASH);
        }
        for (int i = 0; i < players; i++) {
            bets.push_back(0);
        }
        for (int i = 0; i < players; i++) {
            inRound.push_back(true);
        }
    }

    void Game::Start() {
        buttonPos = 0;
        minimumBet = STARTING_CASH / 6;
        while (GetInput("Play Round? ([Y]es, [N]o): ", { 'Y', 'N' }) != 'N') {
            // Play
            PlayRound();


            // Reset
            pot = 0;
            buttonPos = (buttonPos + 1) % hands.size();
            for (int i = 0; i < hands.size(); i++) {
                bets[i] = 0;
                inRound[i] = true;
            }
            for (auto& hand : hands) {
                hand.Discard(deck);
            }
            communityCards.Discard(deck);


            // Check finished
            int winner = -1;
            for (int i = 0; i < hands.size(); i++) {
                if (cash[i] > 0 && winner != -1) {
                    winner = -1;
                    break; // No winner yet
                }
                else if (cash[i] > 0) {
                    winner = i;
                }
            }
            if (winner != -1) {
                std::cout << "Player " << winner + 1 << " won the game!";
                break;
            }
        }
    }

    // ----------------------------  Private  ----------------------------

    void Game::PlayRound() {
        // Deal
        PlayerDeal();
        DrawTable();
        BettingRound();

        // Flop
        TableDeal(3);
        DrawTable();
        BettingRound();

        // Turn
        TableDeal(1);
        DrawTable();
        BettingRound();

        // River
        TableDeal(1);
        DrawTable();
        BettingRound();

        // Showdown
        for (int i = 0; i < hands.size(); i++) {
            if (inRound[i]) {
                std::cout << "Player " << i + 1 << " " << hands[i] << "\n";
            }
        }
        std::cout << "Community " << communityCards << "\n";

        int winner = std::distance(std::begin(hands), std::max_element(
            std::begin(hands),
            std::end(hands),
            [this](Hand& a, Hand& b) {
                return (a + communityCards) < (b + communityCards);
            }));
        std::cout << "Player " << winner + 1 << " Wins with " << Hand::TYPE_NAMES[(hands[winner] + communityCards).Score().first]((hands[winner] + communityCards).Score().second) << "!\n=========================================\n";
        cash[winner] += pot;
    }

    void Game::BettingRound() {
        std::size_t smallBlindPos = (buttonPos + 1) % hands.size();
        std::size_t bigBlindPos = (buttonPos + 2) % hands.size();
        std::size_t currentBetter = (buttonPos + 3) % hands.size();

        int currentBet = minimumBet;
        bets[smallBlindPos] = minimumBet / 2 < cash[smallBlindPos] ? currentBet / 2 : cash[smallBlindPos];
        cash[smallBlindPos] -= bets[smallBlindPos];
        bets[bigBlindPos] = minimumBet / 2 < cash[bigBlindPos] ? currentBet : cash[bigBlindPos];
        cash[bigBlindPos] -= bets[bigBlindPos];


        while (true) {
            // Check if finished
            // TODO: Only continue after all players have called/checked (allowing big blind to raise on first round for example
            // TODO: Only force bets on first round
            // TODO: Fix infinite loop cause when there's 1 player?
            bool done = true;
            for (std::size_t i = 0; i < hands.size(); i++) {
                if (bets[i] == currentBet || cash[i] == 0 || !inRound[i]) {
                    continue;
                }
                done = false;
            }
            if (done) {
                break;
            }
            if (cash[currentBetter] == 0 || !inRound[currentBetter]) {
                continue;
            }

            // Display current bets
            std::cout << "Current bets:" << "\n";
            for (std::size_t i = 0; i < hands.size(); i++) {
                if (inRound[i]) {
                    std::cout << "\t\b\b\b"
                        << (i == smallBlindPos ? "b" : i == bigBlindPos ? "B" : " ")
                        << (i == currentBetter ? ">" : " ")
                        << " Player " << i + 1 << ": " << bets[i] << "\n";
                }
            }

            // Act
            char action = currentBetter == 0 ? GetInput("Enter an action ([F]old, [C]heck/[C]all, [R]aise): ", { 'F', 'C', 'R' }) : 'C';

            switch (action) {
            case 'C':
            {
                int callAmount = std::min(currentBet - bets[currentBetter], cash[currentBetter]);
                bets[currentBetter] += callAmount;
                cash[currentBetter] -= callAmount;
                std::cout << "Player " << currentBetter + 1 << " Called" << "\n\n";
            }
            break;
            case 'F':
                inRound[currentBetter] = false;
                std::cout << "Player " << currentBetter + 1 << " Folder" << "\n\n";
                break;
            case 'R':
            {
                int raiseAmount = std::min(currentBet + minimumBet - bets[currentBetter], cash[currentBetter]);
                currentBet += minimumBet;
                bets[currentBetter] += raiseAmount;
                cash[currentBetter] -= raiseAmount;
                std::cout << "Player " << currentBetter + 1 << " Raised to " << raiseAmount << "\n\n";
            }
            break;
            }
            currentBetter = (currentBetter + 1) % hands.size();
            GetInput("Press space to continue...", { ' ' });
        }
        for (std::size_t i = 0; i < hands.size(); i++) {
            pot += bets[i];
            bets[i] = 0;
        }
    }

    void Game::PlayerDeal() {
        for (int i = 0; i < 2; i++) {
            for (int player = buttonPos + 1; player < hands.size(); player++) {
                if (cash[player] != 0) {
                    hands[player].Draw(deck, 1);
                }
                else {
                    inRound[player] = false;
                }
            }
            for (int player = 0; player < buttonPos + 1; player++) {
                if (cash[player] != 0) {
                    hands[player].Draw(deck, 1);
                }
                else {
                    inRound[player] = false;
                }
            }
        }
    }

    void Game::TableDeal(int n) {
        deck.Discard(deck.Draw(1));
        communityCards.Draw(deck, n);
    }

    void Game::DrawTable() {
        for (int i = 0; i < hands.size(); i++) {
            if (inRound[i]) {
                if (i == 0) {
                    std::cout << "Player " << i + 1 << "\nCash: " << cash[i] << "\n" << hands[i] << "\n";
                }
                else {
                    std::cout << "Player " << i + 1 << "\nCash: " << cash[i] << "\n" << "Cards:\n    \t#############\n\t#############\n" << "\n";
                }
            }
        }
        std::cout << "Community " << communityCards << "\n";
    }

    char Game::GetInput(std::string prompt, std::vector<char> validInputs) {
        char c;
        std::cout << prompt;
        do {
            c = _getch();
        } while (std::find(std::begin(validInputs), std::end(validInputs), std::toupper(c)) == std::end(validInputs));
        std::cout << c << "\n";
        return std::toupper(c);
    }

    // ---------------------------- Operators ----------------------------
}