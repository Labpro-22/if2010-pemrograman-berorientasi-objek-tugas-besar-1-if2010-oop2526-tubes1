#include <stack>
#include <vector>
#include <algorithm>
#include <random>
#include "Card.hpp"
#include "ActionCard.hpp"
#include "SkillCard.hpp"
template <class T>
class Deck
{
private:
    std::stack<T> cards;
    std::vector<T> discard;
public:
    T drawCard(){
        if(cards.empty()){
            throw "placeholder";
        }
        T temp= cards.top();
        cards.pop();
        return temp;
    }
    void shuffle(){
        if(discard.empty()){
            throw "placeholder";
        }
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(discard.begin(), discard.end(), g);
        for (auto& card : discard) {
            cards.push(card);
        }
        discard.clear();
    }
    void AddToDiscard(T &card){ //belum ada di m1
        discard.push_back(card);
    }

};
template <class ActionCard>
ActionCard Deck<ActionCard>::drawCard(){
    if(cards.empty()){
        throw "placeholder";
    }
    ActionCard temp= cards.top();
    cards.pop();
    this->AddToDiscard(temp);
    return temp;
}