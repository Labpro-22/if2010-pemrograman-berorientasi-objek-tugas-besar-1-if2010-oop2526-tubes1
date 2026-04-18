#include "RailroadProperty.hpp"

RailroadProperty::RailroadProperty()
{
}

RailroadProperty::RailroadProperty(const string &id, const string &code, const string &name, const string &colorGroup, double purchasePrice, double mortageValue, PropertyStatus status, const string &ownerId, map<int, int> rentFactor) : Property(id, code, name, colorGroup, purchasePrice, mortageValue, status, ownerId), rentFactor(rentFactor)
{
}

RailroadProperty::~RailroadProperty()
{
}

double RailroadProperty::calculateRentPrice()
{
}

double RailroadProperty::calculateSellPrice()
{
}

ostream &operator<<(ostream &os, const RailroadProperty &p)
{
}
