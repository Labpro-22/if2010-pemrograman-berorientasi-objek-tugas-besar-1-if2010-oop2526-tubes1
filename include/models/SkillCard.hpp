#include "Card.hpp"
#include <algorithm>
#include <random>
#include <memory>
#include "Player.hpp"
class SkillCard: public Card
{
private:
    /* data */
public:
    virtual void onDraw(Player& p, Board& b, std::vector<Player>& all){
        p.addSkillCard(std::unique_ptr<SkillCard>(this));
    }
    virtual void useEffect(Player& p, Board& b, std::vector<Player>& all)=0;
};

class MoveSkillCard: public SkillCard
{
private:
    int step;
public:
virtual void onDraw(Player& p, Board& b, std::vector<Player>& all){
        p.addSkillCard(std::unique_ptr<SkillCard>(this));
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(1, 100); 
        this->step=dist(gen);
    }
    void useEffect(Player& p, Board& b, std::vector<Player>& all){
        p.movePlayer(this->step);
    }
};
class DiscountSkillCard: public SkillCard
{
private:
    float percentage;
public:
    void onDraw(Player& p, Board& b, std::vector<Player>& all){
        p.addSkillCard(std::unique_ptr<SkillCard>(this));
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dist(1, 100); 
        this->percentage=dist(gen);
    }
    void useEffect(Player& p, Board& b, std::vector<Player>& all){
        p.addEffect(std::unique_ptr<Effect>(new DiscountEffect(this->percentage)));
    }
};
class ShieldSkillCard: public SkillCard
{
private:
    /* data */
public:
    void useEffect(Player& p, Board& b, std::vector<Player>& all){
        p.addEffect(std::unique_ptr<Effect>(new ShieldEffect()));
    }
};
class TeleportSkillCard: public SkillCard
{
private:
    /* data */
public:
    void useEffect(Player& p, Board& b, std::vector<Player>& all){
        int steps;
        // std::cin>>steps;
        p.movePlayer(steps);
    }
};
class LassoSkillCard: public SkillCard
{
private:
    /* data */
public:
    void useEffect(Player& p, Board& b, std::vector<Player>& all){
        Player *moved;
        int distance=INFINITY;
        for (auto& pl: all){
            int temp=pl.getPosition()-p.getPosition();
            if(temp < distance && temp>=0 && &p!=&pl){
                distance=temp;
                moved=&pl;
            }
        }
        moved->movePlayer(-distance);
    }
};
class DemolitionSkillCard: public SkillCard
{
private:
    /* data */
public:
    void useEffect(Player& p, Board& b, std::vector<Player>& all);
};
