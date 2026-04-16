#pragma once 

#include <string> 
#include "Player.hpp" 
#include "../logic/Game.hpp" 
#include <vector> 
using namespace std; 
namespace core { 
class Property { 
	private: 
		string name_; 
		Player* owner_; 
		bool isMortgaged_; 
		int price_; 
		int mortgageValue_; 
	public: 
		Property();
		Property(string name, Player* owner, bool isMortagegd, int price, int mortgageVallue);
		virtual void calculateRent(logic::Game& g); 
		virtual string getType(); 
		void mortgage(); 
		void unmortgage(); 
		void setOwner(Player*); 
		Player* GetOwner(); 
		bool isOwned(); 
		bool isAvailable(); 
}; 
class Street : public Property { 
	private: 
		// ColorGroup colorGroup 
		int houseCount_; 
		int hotelCount_; 
		vector<int> rentValues_; 
		int houseCost_; 
		int hotelCost_; 
		bool hasFestival_; 
	public: 
		void calculateRent(logic::Game& g) override; 
		string getType() override ; 
		void buildHouse(); 
		void buildHotel(); 
		void demolish(); // there's an int in the class diagram but the variable isn't shown. jadi ubah aja ya 
		bool canBuild(logic::Game&) ; 
		bool hasMonopoly(logic::Game&) ; 

};

class Railroad : public Property {
	private: 
		int baseFare_; 
	public: 
		void calculateRent(logic::Game& g) override; 
		string getType() override ; 
		
};

class Utility : public Property { 
	private: 
		int multiplier_; 
	public: 
		void calculateRent(logic::Game& g) override; 
		string getType() override ; 
	

}; 
} 


