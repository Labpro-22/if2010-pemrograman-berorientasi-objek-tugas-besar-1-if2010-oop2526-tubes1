#include "core/GameException/GameException.hpp"

class NoAccessToPropertyException : public GameException{
public:
    NoAccessToPropertyException(): GameException(GameErrorID::NOACCESSTOPROPERTYEXCEPTION,
        "Anda tidak memiliki akses untuk properti ini.") {}
};