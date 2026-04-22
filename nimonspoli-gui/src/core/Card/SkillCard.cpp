#include "SkillCard.hpp"

SkillCard::SkillCard()
{
}

SkillCard::SkillCard(const string &type, const string &description, bool used) : Card(type, description), used(used)
{
}

SkillCard::~SkillCard()
{
}

bool SkillCard::getUsed()
{
    return used;
}

void SkillCard::setUsed(bool newUsed)
{
    used = newUsed;
}
