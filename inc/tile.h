#ifndef TILE
#define TILE

class Tile{
public:
    enum Type {DEFAULT, FORBIDDEN, SPECIAL, TERMINAL};

    Type type;
    double reward;
    double utility;

    Tile(Type t = DEFAULT, double r = 0.0, double u = 0.0): type(t), reward(r), utility(u) {}
};

#endif // TILE

