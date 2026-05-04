#include "../../include/models/Property.hpp"
#include "../../include/utils/GameException.hpp"

Property::Property(const std::string& code, const std::string& name,
                   PropertyType       type, int purchasePrice, int mortgageValue)
    : code(code),
      name(name),
      type(type),
      owner(nullptr),
      status(OwnershipStatus::BANK),
      purchasePrice(purchasePrice),
      mortgageValue(mortgageValue),
      festivalMultiplier(1),
      festivalDuration(0) {}

bool Property::canBuild()    const { return false;         }
int  Property::getSellValue() const { return purchasePrice; }

bool Property::isOwned()     const { return status == OwnershipStatus::OWNED;     }
bool Property::isMortgaged() const { return status == OwnershipStatus::MORTGAGED; }
bool Property::isBank()      const { return status == OwnershipStatus::BANK;      }

void Property::setOwner(Player* p) {
    owner  = p;
    status = (p == nullptr) ? OwnershipStatus::BANK : OwnershipStatus::OWNED;
}

void Property::setStatus(OwnershipStatus s) {
    status = s;
    if (s == OwnershipStatus::BANK) owner = nullptr;
}

Player* Property::getOwner() const { return owner; }

int  Property::getFestivalMultiplier() const { return festivalMultiplier; }
void Property::setFestivalMultiplier(int m)  { festivalMultiplier = m;    }
int  Property::getFestivalDuration()   const { return festivalDuration;   }
void Property::setFestivalDuration(int d)    { festivalDuration = d;      }

std::string     Property::getCode()          const { return code;          }
std::string     Property::getName()          const { return name;          }
PropertyType    Property::getType()          const { return type;          }
OwnershipStatus Property::getStatus()        const { return status;        }
int             Property::getPurchasePrice() const { return purchasePrice; }
int             Property::getMortgageValue() const { return mortgageValue; }