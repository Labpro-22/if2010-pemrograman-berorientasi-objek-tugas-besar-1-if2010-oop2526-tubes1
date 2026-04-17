#include "UtilityProperty.hpp"

UtilityProperty::UtilityProperty()
{
}

UtilityProperty::UtilityProperty(const string &id, const string &code, const string &name, const string &colorGroup, double purchasePrice, double mortageValue, PropertyStatus status, const string &ownerId, map<int, int> rentPrice) : Property(id, code, name, colorGroup, purchasePrice, mortageValue, status, ownerId), rentPrice(rentPrice)
{
}

UtilityProperty::~UtilityProperty()
{
}

double UtilityProperty::calculateRentPrice()
{
}

double UtilityProperty::calculateSellPrice()
{
}

ostream &operator<<(ostream &os, const UtilityProperty &p)
{
}
