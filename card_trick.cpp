#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>

using namespace std;

// Define constants
const string HEARTS = "\033[31m\u2665\033[0m";
const string DIAMONDS = "\033[31m\u2666\033[0m";
const string SPADES = "\u2660";
const string CLUBS = "\u2663";

// Function prototypes
void greet_user();
vector<pair<string, string>> get_deck();
vector<vector<pair<string, string>>> get_bundle();
vector<vector<pair<string, string>>> get_shuffled_bundle(vector<vector<pair<string, string>>> cards_bundle, vector<pair<string, string>> chosen_row);
void display_cards(const vector<vector<pair<string, string>>>& cards_bundle);
int choose_row();

int main() {
    greet_user();

    vector<vector<pair<string, string>>> cards_bundle = get_bundle();
    display_cards(cards_bundle);

    cout << "\nAlright, which row holds your card? (1/2/3/4): ";
    int row_choice = choose_row();
    vector<pair<string, string>> chosen_row = cards_bundle[row_choice];
    cards_bundle.erase(cards_bundle.begin() + row_choice);

    cards_bundle = get_shuffled_bundle(cards_bundle, chosen_row);

    cout << "\nI'm going to shuffle the cards, then you'll tell me again which "
         << "ROW holds your card - the SAME CARD you picked before!\n";

    if (cin.get() == 'q') {
        exit(0);
    }

    display_cards(cards_bundle);

    cout << "\nAlright, which row holds your chosen card? (1/2/3/4): ";
    row_choice = choose_row();
    const auto& chosen_card = cards_bundle[row_choice].back();

    cout << "\033[31m\nA\033[32mB\033[33mR\033[34mA\033[35mC\033[37mA\033[31mD\033[32mA\033[33mB\033[34mR\033[35mA\033[0m, here's your card:\n";

    display_cards({{chosen_card}});
    cout << endl;

    return 0;
}


// Function to greet user and explain the trick
void greet_user() {
    std::cout << "Wanna see a magic trick? (y/n): ";
    string response;
    cin >> response;

    if (response == "n" || response == "q") {
        exit(0);
    }

    cout << "\nOK, here's how it works. I'm going to lay down four rows of cards.\n";
    this_thread::sleep_for(chrono::seconds(1));
    cout << "\nYou pick one card, but don't tell me which one it is.\n"
         << "Just enter the ROW NUMBER it's in! (1/2/3/4)\n";
    this_thread::sleep_for(chrono::seconds(1));
    cout << "(Enter 'q' at any time to quit)\n";
    this_thread::sleep_for(chrono::seconds(1));
    cout << "\nReady for some \033[31mM\033[32mA\033[33mG\033[34mI\033[35mC\033[0m?\n";

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (cin.get() == 'q') {
        exit(0);
    }

    cout << "Here we go!\n";
}

// Function to create a full deck of cards
vector<pair<string, string>> get_deck() {
    vector<pair<string, string>> deck;
    for (const string& suit : {HEARTS, DIAMONDS, SPADES, CLUBS}) {
        for (int rank = 2; rank <= 10; ++rank) {
            deck.push_back(make_pair(to_string(rank), suit));
        }
        for (const string& rank : {"J", "Q", "K", "A"}) {
            deck.push_back(make_pair(rank, suit));
        }
    }
    shuffle(deck.begin(), deck.end(), default_random_engine(time(0)));
    return deck;
}

// Function to get four rows of cards
vector<vector<pair<string, string>>> get_bundle() {
    vector<vector<pair<string, string>>> cards_bundle;
    vector<pair<string, string>> deck = get_deck();

    for (int i = 0; i < 4; ++i) {
        vector<pair<string, string>> row;
        for (int j = 0; j < 4; ++j) {
            row.push_back(deck.back());
            deck.pop_back();
        }
        cards_bundle.push_back(row);
    }

    return cards_bundle;
}

// Function to shuffle the cards
vector<vector<pair<string, string>>> get_shuffled_bundle(vector<vector<pair<string, string>>> cards_bundle, vector<pair<string, string>> chosen_row) {
    vector<pair<string, string>> shuffled_cards;

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 4; ++j) {
            shuffled_cards.push_back(cards_bundle[i][j]);
        }
    }

    shuffle(shuffled_cards.begin(), shuffled_cards.end(), default_random_engine(time(0)));
    cards_bundle.clear();

    for (int i = 0; i < 4; ++i) {
        vector<pair<string, string>> row;
        for (int j = 0; j < 3; ++j) {
            row.push_back(shuffled_cards.back());
            shuffled_cards.pop_back();
        }
        row.push_back(chosen_row.back());
        chosen_row.pop_back();
        cards_bundle.push_back(row);
    }

    return cards_bundle;
}

// Function to display the cards
void display_cards(const vector<vector<pair<string, string>>>& cards_bundle) {
    for (size_t i = 0; i < cards_bundle.size(); ++i) {
        this_thread::sleep_for(chrono::seconds(1));
        cout << endl;
        if (cards_bundle.size() > 1) {
            cout << "Row nr. " << (i + 1) << ":\n";
        }
        vector<string> rows(4, ""); // Reduced to 4 lines for each card presentation

        for (const auto& card : cards_bundle[i]) {
            const string& rank = card.first;
            const string& suit = card.second;
            // Adjust output based on length of the rank (handling case for "10")
            if (rank.length() == 2) { // "10" case
                rows[0] += " ___  ";
                rows[1] += "|"+ rank +" | "; 
                rows[2] += "| " + suit + " | ";
                rows[3] += "|_"+ rank +"| ";
            } else { // Other cards
                rows[0] += " ___  ";
                rows[1] += "|"+ rank +"  | ";
                rows[2] += "| " + suit + " | ";
                rows[3] += "|__"+ rank +"| ";
            }
        }

        for (const string& row : rows) {
            cout << row << endl;
        }
    }
}

// Function to ask user to choose a row
int choose_row() {
    int row_choice = -1;

    while (true) {
        cout << "> ";
        string num;
        getline(cin, num);

        if (num == "q") {
            exit(0);
        } else if (num != "1" && num != "2" && num != "3" && num != "4") {
            cout << "Invalid input, try again!\n";
        } else {
            row_choice = stoi(num) - 1;
            break;
        }
    }

    return row_choice;
}