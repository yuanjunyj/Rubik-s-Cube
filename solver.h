#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cmath>
#include <cstring>
#include <string>
#include <algorithm>

class Solver {

public:
    struct RUBIK
    {
        int color[7][4][4];
    };

    struct COMMAND
    {
        int length;
        std::string st;
        COMMAND() {}
        COMMAND(int _l, std::string _s) {length = _l, st = _s;}
    };

    struct TUPLE
{
	int x, y, z;
	TUPLE() {}
	TUPLE(int _x, int _y, int _z) {x = _x, y = _y, z = _z;}
};

private:
    inline void Rotate_Surface(RUBIK &rubik, int id);
    inline void Rotate_Edge(RUBIK &rubik, int id);
    inline void Update(RUBIK &rubik, int id, int tm);
    inline std::string Operation(COMMAND command, RUBIK &rubik, int &hActive, int &vActive);

    inline int Translate(int a, int b);
    inline int Translate(int a, int b, int c);
    inline char opTranslate(int op, int hActive, int vActive);

    inline bool Complete(RUBIK &rubik);
    inline bool Complete(RUBIK &rubik, int n);

    inline std::string LayerOneCross(RUBIK &rubik);
    inline std::string LayerOneCover(RUBIK &rubik);
    inline std::string ToptoBottom(RUBIK &rubik);
    inline std::string LayerTwoCover(RUBIK &rubik);
    inline std::string LayerThreeCross(RUBIK &rubik);
    inline std::string LayerThreeCover(RUBIK &rubik);
    inline std::string CornerAdjustment(RUBIK &rubik);
    inline std::string EdgeAdjustment(RUBIK &rubik);

public:
    Solver();
    std::string solve(RUBIK& rubik);

};
