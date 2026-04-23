#include <string>
class Saveable {
    
    public:
        virtual ~Saveable() = default;
        virtual std::string toSaveFormat() const = 0;

};