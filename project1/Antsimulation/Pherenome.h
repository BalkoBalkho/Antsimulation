namespace pherenome
{

    enum class paths { food, danger, war, colony, };
    enum class jobs { cleaning, patrol, food_finding, transport, };


};



class PherenomeFactory
{
private:
    int colonyid;

public:


    PherenomeFactory(int colonyid) : colonyid(colonyid) {};

    void Spray(int strength, enum type1) {
    
    };
};