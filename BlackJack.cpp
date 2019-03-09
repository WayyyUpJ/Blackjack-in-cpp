#include <iostream>
#include <numeric>
#include <vector>
#include <algorithm>
#include <random>
#include <unordered_map>
#include <ctime>

namespace BlackJackGame {

enum CardValue {
  ACE = 1,
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
  KING = 13
};
enum Suit {
  SMILE = 1
};

class Card {
public:
  Card(): _value(ACE), _suit(SMILE) {}

  void setValue(CardValue val) {_value = val;}

  std::string toString() const {
    const std::unordered_map<int, std::string> strMap({
      {1, "ACE"},
      {2, "TWO"},
      {3, "THREE"},
      {4, "FOUR"},
      {5, "FIVE"},
      {6, "SIX"},
      {7, "SEVEN"},
      {8, "EIGHT"},
      {9, "NINE"},
      {10, "TEN"},
      {11, "JACK"},
      {12, "QUEEN"},
      {13, "KING"}
    });
    return strMap.at(_value);
  }

  CardValue value() const {
    return _value;
  }

private:
  CardValue _value;
  Suit _suit;
};

class Deck {
public:
  Deck():_deck(52) {
    for (int i = 0; i < 52; i++) {
      _deck[i].setValue(static_cast<CardValue>(i / 4 + 1));
    }
    auto rand_seed = std::default_random_engine(time(0));
    std::shuffle(std::begin(_deck), std::end(_deck), std::move(rand_seed));
  }

  Card render() {
    Card back = _deck.back();
    _deck.pop_back();
    return back;
  }

private:
  std::vector<Card> _deck;
};

using std::cout, std::cin, std::endl, std::vector, std::pair;
class BlackJack {
public:
  void start() {
    cout << "Game Started" << endl;

    std::string status = "1";

    do {
      play();
      if (checkExplosion(playerCards)) {
        cout << "Now you have ";
        printCards(playerCards);
        cout << endl;
        cout << "Sum is " << cardsSum(playerCards).first << endl;
        cout << "You Lost" << endl;
        return;
      }
      cout << "Now you have ";
      printCards(playerCards);
      cout << endl;
      cout << "Type 1 to CONTINUE, 2 to STOP:" << endl;
      cin >> status;
      while (status != "1" && status != "2") {
        cout << "Cannot recognize, try again" << endl;
        cin >> status;
      }
      if (status == "2") {
        cout << "Now you have ";
        printCards(playerCards);
        cout << endl;
        cout << "The enemy has ";
        printCards(enemyCards);
        cout << endl;
        if (checkWin()) {
          cout << "You win!" << endl;
          return;
        } else {
          cout << "You lose!" << endl;
          return;
        }
      }

    } while (status == "1");



  }
private:
  // Ace as 1 and Ace as 11
  pair<size_t, size_t> cardsSum(const vector<Card>& cards) const {
    size_t sum1(0);
    size_t sum2(0);

    for (const auto& card : cards) {
      if (card.value() == ACE) {
        sum1 += 1;
        sum2 += 11;
      } else if (card.value() < JACK) {
        sum1 += static_cast<size_t>(card.value());
        sum2 += static_cast<size_t>(card.value());
      } else {
        sum1 += 10;
        sum2 += 10;
      }
    }

    return {sum1, sum2};
  }

  bool checkExplosion(const vector<Card>& cards) const {
    const pair<size_t, size_t> scores = cardsSum(cards);
    if (scores.first > 21 && scores.second > 21) return true;
    return false;
  }

  bool checkWin() const {
    const pair<size_t, size_t> playerScores = cardsSum(playerCards);
    const pair<size_t, size_t> enemyScores = cardsSum(enemyCards);
    if (enemyScores.second > 21 && enemyScores.second > 21){
      return true;
    } else {
      return (playerScores.second > 21 ?
              playerScores.first : playerScores.second) >
             (enemyScores.second > 21 ?
              enemyScores.first : enemyScores.second);
    }
  }

  void play() {
    Card toPlayer = cardsInDeck.render();
    Card toEnemy = cardsInDeck.render();
    playerCards.push_back(toPlayer);
    enemyCards.push_back(toEnemy);
  }

  void printCards(const vector<Card>& cards) const {
    for (const auto& card : cards) {
      cout << card.toString() << " ";
    }
  }

  Deck cardsInDeck;
  vector<Card> enemyCards;
  vector<Card> playerCards;
};


} // BlackJackGame

using namespace BlackJackGame;
int main(int argc, char** argv) {

  BlackJack game;
  game.start();

  return 0;
}
