#pragma once 

#include <string> 
#include <vector>
using namespace std;


namespace core {
class Property; 
class Game; 
class Player { 
	protected: 
		string name_; 
		string token_; 
		int balance_; 
		int position_; 
		bool inJail_; 
		bool isBankrupt_; 
		int jailTurns_; 
		vector<Property*> ownedProperties_;
		//vector<ActionCard*> heldCards_; 
	public: 
		Player(string name); // assume that others will be inputted as defaults and there is no default names. 
		virtual void takeTurn(Game&); 
		virtual bool isHuman(); 
		int getNetWorth();
		bool canAfford(); // there's an int in the parameter but not stated apa isinya. 
		void goToJail(); 
		void releaseFromJail(); 
		void addProperty(Property*); 
		void removeProperty(Property*); 
		bool operator+(Player&); 
		bool operator-=(Player&);
		bool operator<(Player&); 
		bool operator>(Player&); 
		bool operator==(Player&); 
};

class HumanPlayer : public Player {
	public: 
		void takeTurn(Game& g);
		bool isHuman(); 
};

} 



