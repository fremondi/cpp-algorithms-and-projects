// Card_Poker_Class.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <random>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <ctime>
#include <cstdlib>

using namespace std;

enum class suit { SPADE, HEART, DIAMOND, CLUB };

class pips {
    int value;
public:
    pips(int v = 1) : value(v) {}
    int get_pips() const { return value; }

    // Allow pips to be printed directly
    friend ostream& operator<<(ostream& out, const pips& p) {
        out << p.value;
        return out;
    }

    // Comparison operator to allow use in maps, sorting, etc.
    bool operator<(const pips& other) const {
        return value < other.value;
    }
};

class card {
public:
    card() : s(suit::SPADE), v(1) {}
    card(suit s, pips v) : s(s), v(v) {}

    suit get_suit() const { return s; }
    pips get_pips() const { return v; }

    friend ostream& operator<<(ostream& out, const card& c) {
        // Custom output assumes enums are handled as ints or symbols
        const char* suits[] = { "S", "H", "D", "C" };
        out << c.v << suits[static_cast<int>(c.s)];
        return out;
    }

private:
    suit s;
    pips v;
};

void init_deck(vector<card>& d) {
    for (int i = 1; i < 14; ++i) {
        card c(suit::SPADE, i);
        d[i - 1] = c;
    }
    for (int i = 1; i < 14; ++i) {
        card c(suit::HEART, i);
        d[i + 12] = c;
    }
    for (int i = 1; i < 14; ++i) {
        card c(suit::DIAMOND, i);
        d[i + 25] = c;
    }
    for (int i = 1; i < 14; ++i) {
        card c(suit::CLUB, i);
        d[i + 38] = c;
    }
}

void print(vector<card>& deck) {
    for (auto p = deck.begin(); p != deck.end(); ++p)
        cout << *p;
    cout << endl;
}

bool is_flush(vector<card>& hand) {
    suit s = hand[0].get_suit();
    for (auto p = hand.begin() + 1; p != hand.end(); ++p)
        if (s != p->get_suit())
            return false;
    return true;
}

bool is_straight(vector<card>& hand) {
    int pips_v[5], i = 0;
    for (auto p = hand.begin(); p != hand.end(); ++p)
        pips_v[i++] = (p->get_pips()).get_pips();

    sort(pips_v, pips_v + 5); // Sort the pip values

    if (pips_v[0] != 1)  // Standard case (no Ace as 1)
        return (pips_v[0] == pips_v[1] - 1 &&
            pips_v[1] == pips_v[2] - 1 &&
            pips_v[2] == pips_v[3] - 1 &&
            pips_v[3] == pips_v[4] - 1);
    else  // Special Ace logic: A-2-3-4-5 OR 10-J-Q-K-A
        return (pips_v[0] == pips_v[1] - 1 &&
            pips_v[1] == pips_v[2] - 1 &&
            pips_v[2] == pips_v[3] - 1 &&
            pips_v[3] == pips_v[4] - 1)
        ||
        (pips_v[1] == 10 && pips_v[2] == 11 &&
            pips_v[3] == 12 && pips_v[4] == 13);
}

bool is_straight_flush(vector<card>& hand)
{
    return is_flush(hand) && is_straight(hand);
}

bool is_4of_akind(vector<card>& hand) {
    map<int, int> pip_counts;

    for (auto& c : hand) {
        int pip = c.get_pips().get_pips();  // Adjust if pips is a primitive
        pip_counts[pip]++;
    }

    for (auto& entry : pip_counts)
        if (entry.second == 4)
            return true;

    return false;
}
bool is_straight_flush_7(vector<card>& hand) {
    map<suit, vector<int>> suited_pips;

    // Step 1: Organize cards by suit
    for (auto& c : hand) {
        suited_pips[c.get_suit()].push_back(c.get_pips().get_pips());
    }

    // Step 2: Check for a straight in each suit
    for (auto& suit_pip_pair : suited_pips) {
        auto& s = suit_pip_pair.first;
        auto& pips = suit_pip_pair.second;

        if (pips.size() < 5) continue;

        sort(pips.begin(), pips.end());
        pips.erase(unique(pips.begin(), pips.end()), pips.end());

        // If Ace is present, add 14 to consider Ace-high
        if (find(pips.begin(), pips.end(), 1) != pips.end()) {
            pips.push_back(14);
            sort(pips.begin(), pips.end());
        }

        int count = 1;
        for (size_t i = 1; i < pips.size(); ++i) {
            if (pips[i] == pips[i - 1] + 1)
                count++;
            else
                count = 1;

            if (count >= 5)
                return true;
        }
    }

    return false;
}


int main()
{
    vector<card> deck(52);       // Create a vector of 52 card objects
    init_deck(deck);             // Fill deck with cards

    // Use C++11 random engine instead of deprecated random_shuffle
    random_device rd;
    mt19937 rng(rd());

    int how_many;
    int flush_count = 0;
    int str_count = 0;
    int str_flush_count = 0;

    cout << "How Many shuffles? ";
    cin >> how_many;

    for (int loop = 0; loop < how_many; ++loop) {
        shuffle(deck.begin(), deck.end(), rng);  // Modern shuffle

        vector<card> hand(deck.begin(), deck.begin() + 5);  // First 5 cards

        // Optional: print hand for debugging
        // for (const auto& c : hand) cout << c << " ";
        // cout << endl;

        if (is_flush(hand)) flush_count++;
        if (is_straight(hand)) str_count++;
        if (is_straight_flush(hand)) str_flush_count++;
    }

    cout << "Flushes: " << flush_count << " out of " << how_many << endl;
    cout << "Straights: " << str_count << " out of " << how_many << endl;
    cout << "Straight Flushes: " << str_flush_count << " out of " << how_many << endl;

    return 0;
}

