#pragma once
#include <stack>
#include <vector>
#include <algorithm>
#include <random>
#include "Card.hpp"
#include "ActionCard.hpp"
#include "SkillCard.hpp"
#include "Saveable.hpp"
template <class T>
class Deck : public Saveable
{
private:
    std::stack<T> cards;
    std::vector<T> discard;
public:
    T drawCard(){
        if(cards.empty()){
            throw "placeholder";
        }

        T temp = std::move(cards.top());
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
            cards.push(std::move(card));
        }
        discard.clear();
    }

    void AddToDiscard(T card) { // belum ada di m1
        discard.push_back(std::move(card));
    }
    std::string toSaveFormat()const override {
        return "";
    }
};
// template<class T>
// std::string Deck<ActionCard>::toSaveFormat() const{

// }
// template<class T>  
// std::string Deck<SkillCard>::toSaveFormat() const{

// }
// template<class T>  
// std::string Deck<Card*>::toSaveFormat() const{

// }

template <>
inline std::unique_ptr<ActionCard> Deck<std::unique_ptr<ActionCard>>::drawCard(){
    if(cards.empty()){
        throw "placeholder";
    }

    std::unique_ptr<ActionCard> temp = std::move(cards.top());
    cards.pop();
    this->AddToDiscard(std::move(temp));
    return temp;
}
