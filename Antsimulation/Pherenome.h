#include <unordered_map>
#include <variant>
//#include "World.h"
// enum
namespace Pherenome
{



    enum class paths : uint8_t { food, danger, war, colony, };
    enum class jobs : uint8_t { cleaning =13, patrol, food_finding, transport, };
    
    // This is like a union of these 2 types
    using PherenomeType = std::variant<paths, Job>;
};


struct PherenomeMarker {
    Pherenome::PherenomeType Ptype;
    uint8_t lifetime=256;
};



class PherenomeFactory
{
private:
// This is a doubly linked list of active Pherenomes*...
// Every "pixel" of the world has a Pherenome value but only so many would be active, and we need to loop over and decrease the lifetime till its gone.
// PherenomeMarker* references will exist for the lfie of the program since theyre never deleted (world is never deleted) so no dangling ptr situation
// Ants are supposed to have a shared (static) pherenomeFactory and the simulation will have one too, thats where tick() will be called.
class PherenomeList {

    struct Node {
        PherenomeMarker* data;
        // Node* next;
        // Node* prev;
        Ant* debug_source; //identify the source of this pherenome.
        
    };

    std::list<Node> list;
    
    public:
    // decrement lifetime, when lifetime is 0 remove it from list.
    void tick();
};

    int colonyid;
    static PherenomeList pl;

public:


    PherenomeFactory(int colonyid) : colonyid(colonyid) {};

    void Spray(int strength, Pherenome::PherenomeType type1) {
    
    };
};