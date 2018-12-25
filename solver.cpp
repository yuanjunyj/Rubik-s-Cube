#include "solver.h"

using namespace std;

char ch;
int hActive = 0, vActive = 0;

//FBLRUD
//012345

Solver::Solver()
{

}

inline void Solver::Rotate_Surface(RUBIK &rubik, int id)
{
    int pp[] = {0, 1, 1, 1, 3, 3, 3, 3, 1};
    int qq[] = {0, 1, 2, 2, 3, 3, 2, 2, 1};
    int sf[4][4];

    for (int i = 1; i <= 8; i += 2)
    {
        int prev = i - 2;
        if (prev <= 0) prev += 8;
        sf[pp[i]][pp[i + 1]] = rubik.color[id][pp[prev]][pp[prev + 1]];
        sf[qq[i]][qq[i + 1]] = rubik.color[id][qq[prev]][qq[prev + 1]];
    }
    for (int i = 1; i <= 3; i++)
        for (int j = 1; j <= 3; j++)
        {
            if (!(i == 2 && j == 2)) rubik.color[id][i][j] = sf[i][j];
        }
}

inline void Solver::Rotate_Edge(RUBIK &rubik, int id)
{
    TUPLE group[20];
    int r0[7][4][4];

    int cnt = 0;
    switch(id)
    {
        case 1:
            for (int j = 1; j <= 3; j++) group[++cnt] = TUPLE(5, 3, j);
            for (int j = 1; j <= 3; j++) group[++cnt] = TUPLE(4, j, 1);
            for (int j = 1; j <= 3; j++) group[++cnt] = TUPLE(6, 1, 4 - j);
            for (int j = 1; j <= 3; j++) group[++cnt] = TUPLE(3, 4 - j, 3);
            break;
        case 2:
            for (int j = 1; j <= 3; j++) group[++cnt] = TUPLE(5, 1, 4 - j);
            for (int j = 1; j <= 3; j++) group[++cnt] = TUPLE(3, j, 1);
            for (int j = 1; j <= 3; j++) group[++cnt] = TUPLE(6, 3, j);
            for (int j = 1; j <= 3; j++) group[++cnt] = TUPLE(4, 4 - j, 3);
            break;
        case 3:
            for (int j = 1; j <= 3; j++) group[++cnt] = TUPLE(5, j, 1);
            for (int j = 1; j <= 3; j++) group[++cnt] = TUPLE(1, j, 1);
            for (int j = 1; j <= 3; j++) group[++cnt] = TUPLE(6, j, 1);
            for (int j = 1; j <= 3; j++) group[++cnt] = TUPLE(2, j, 1);
            break;
        case 4:
            for (int j = 1; j <= 3; j++) group[++cnt] = TUPLE(5, 4 - j, 3);
            for (int j = 1; j <= 3; j++) group[++cnt] = TUPLE(2, 4 - j, 3);
            for (int j = 1; j <= 3; j++) group[++cnt] = TUPLE(6, 4 - j, 3);
            for (int j = 1; j <= 3; j++) group[++cnt] = TUPLE(1, 4 - j, 3);
            break;
        case 5:
            for (int j = 1; j <= 3; j++) group[++cnt] = TUPLE(1, 1, 4 - j);
            for (int j = 1; j <= 3; j++) group[++cnt] = TUPLE(3, 1, 4 - j);
            for (int j = 1; j <= 3; j++) group[++cnt] = TUPLE(2, 3, j);
            for (int j = 1; j <= 3; j++) group[++cnt] = TUPLE(4, 1, 4 - j);
            break;
        case 6:
            for (int j = 1; j <= 3; j++) group[++cnt] = TUPLE(1, 3, j);
            for (int j = 1; j <= 3; j++) group[++cnt] = TUPLE(4, 3, j);
            for (int j = 1; j <= 3; j++) group[++cnt] = TUPLE(2, 1, 4 - j);
            for (int j = 1; j <= 3; j++) group[++cnt] = TUPLE(3, 3, j);
            break;
        case 7:
            for (int j = 1; j <= 3; j++) group[++cnt] = TUPLE(1, 2, 4 - j);
            for (int j = 1; j <= 3; j++) group[++cnt] = TUPLE(3, 2, 4 - j);
            for (int j = 1; j <= 3; j++) group[++cnt] = TUPLE(2, 2, j);
            for (int j = 1; j <= 3; j++) group[++cnt] = TUPLE(4, 2, 4 - j);
            break;
        case 8:
            for (int j = 1; j <= 3; j++) group[++cnt] = TUPLE(5, 4 - j, 2);
            for (int j = 1; j <= 3; j++) group[++cnt] = TUPLE(2, 4 - j, 2);
            for (int j = 1; j <= 3; j++) group[++cnt] = TUPLE(6, 4 - j, 2);
            for (int j = 1; j <= 3; j++) group[++cnt] = TUPLE(1, 4 - j, 2);
            break;
    }
    for (int i = 1; i <= 12; i++)
    {
        int prev = i - 3;
        if (prev <= 0) prev += 12;
        r0[group[i].x][group[i].y][group[i].z] = rubik.color[group[prev].x][group[prev].y][group[prev].z];
    }
    for (int i = 1; i <= 12; i++)
    {
        rubik.color[group[i].x][group[i].y][group[i].z] = r0[group[i].x][group[i].y][group[i].z];
    }
}

inline void Solver::Update(RUBIK &rubik, int id, int tm)
{
    for (int i = 1; i <= tm; i++)
    {
        if (id <= 6) Rotate_Surface(rubik, id);
        Rotate_Edge(rubik, id);
    }
}

inline std::string Solver::Operation(COMMAND command, RUBIK &rubik, int &hActive, int &vActive)
{
    char sigma[] = " FBLRUDHV";

    std::string op;

    for (int k = 0; k < command.length; k++)
    {
        for (int i = 1; i <= 6; i++)
        {
            if (command.st[k] == sigma[i])
            {
                Update(rubik, i, 1);
                op += opTranslate(i, hActive, vActive);
            }
            else if (toupper(command.st[k]) == sigma[i])
            {
                Update(rubik, i, 3);
                op += opTranslate(i + 6, hActive, vActive);
            }
        }
        if (command.st[k] == sigma[7])
        {
            Update(rubik, 5, 1);
            Update(rubik, 6, 3);
            Update(rubik, 7, 1);
            hActive++;
        }
        if (command.st[k] == sigma[8])
        {
            Update(rubik, 3, 3);
            Update(rubik, 4, 1);
            Update(rubik, 8, 1);
            vActive++;
        }
    }

    return op;
}

inline int Solver::Translate(int a, int b)
{
    int B = std::max(a, b);
    int S = std::min(a, b);
    return B * 10 + S;
}

inline int Solver::Translate(int a, int b, int c)
{
    int B = std::max(std::max(a, b), c);
    int S = std::min(std::min(a, b), c);
    int M = a + b + c - B - S;
    return B * 100 + S * 10 + M;
}

inline char Solver::opTranslate(int op, int hActive, int vActive)
{
    char image[8][14] = {" FBLRUDfblrud", " RLFBUDrlfbud", " BFRLUDbfrlud", " LRBFUDlrbfud",
                         " BFLRDUbflrdu", " RLBFDUrlbfdu", " FBRLDUfbrldu", " LRFBDUlrfbdu"};
    int label = (vActive > 0) * 4 + (hActive % 4);
    return image[label][op];
}

inline bool Solver::Complete(RUBIK &rubik)
{
    bool ret = true;
    for (int k = 1; k <= 6; k++)
    {
        int base = rubik.color[k][2][2];
        for (int i = 1; i <= 3; i++)
            for (int j = 1; j <= 3; j++)
            {
                if (rubik.color[k][i][j] != base) ret = false;
            }
    }
    return ret;
}

inline bool Solver::Complete(RUBIK &rubik, int k)
{
    bool ret = true;
    int base = rubik.color[k][2][2];
    for (int i = 1; i <= 3; i++)
        for (int j = 1; j <= 3; j++)
        {
            if (rubik.color[k][i][j] != base) ret = false;
        }
    return ret;
}

inline std::string Solver::LayerOneCross(RUBIK &rubik)
{
    std::string result;
    for (int k = 1; k <= 4; k++)
    {
        int ec1 = rubik.color[5][2][2] * 10 + rubik.color[1][2][2];
        int ec2 = rubik.color[1][2][2] * 10 + rubik.color[5][2][2];
        int ec;

        ec = rubik.color[5][1][2] * 10 + rubik.color[2][3][2];
        if (ec == ec1 || ec == ec2)
        {
            result += Operation(COMMAND(6, "BBDDFF"), rubik, hActive, vActive);
        }

        ec = rubik.color[5][2][1] * 10 + rubik.color[3][1][2];
        if (ec == ec1 || ec == ec2)
        {
            result += Operation(COMMAND(5, "LLDFF"), rubik, hActive, vActive);
        }

        ec = rubik.color[5][2][3] * 10 + rubik.color[4][1][2];
        if (ec == ec1 || ec == ec2)
        {
            result += Operation(COMMAND(5, "RRdFF"), rubik, hActive, vActive);
        }

        ec = rubik.color[1][2][3] * 10 + rubik.color[4][2][1];
        if (ec == ec1 || ec == ec2)
        {
            result += Operation(COMMAND(1, "f"), rubik, hActive, vActive);
        }

        ec = rubik.color[4][2][3] * 10 + rubik.color[2][2][3];
        if (ec == ec1 || ec == ec2)
        {
            result += Operation(COMMAND(5, "RdFFr"), rubik, hActive, vActive);
        }

        ec = rubik.color[2][2][1] * 10 + rubik.color[3][2][1];
        if (ec == ec1 || ec == ec2)
        {
            result += Operation(COMMAND(5, "lDFFL"), rubik, hActive, vActive);
        }

        ec = rubik.color[3][2][3] * 10 + rubik.color[1][2][1];
        if (ec == ec1 || ec == ec2)
        {
            result += Operation(COMMAND(1, "F"), rubik, hActive, vActive);
        }

        ec = rubik.color[6][1][2] * 10 + rubik.color[1][3][2];
        if (ec == ec1 || ec == ec2)
        {
            result += Operation(COMMAND(2, "FF"), rubik, hActive, vActive);
        }

        ec = rubik.color[6][2][1] * 10 + rubik.color[3][3][2];
        if (ec == ec1 || ec == ec2)
        {
            result += Operation(COMMAND(3, "DFF"), rubik, hActive, vActive);
        }

        ec = rubik.color[6][2][3] * 10 + rubik.color[4][3][2];
        if (ec == ec1 || ec == ec2)
        {
            result += Operation(COMMAND(3, "dFF"), rubik, hActive, vActive);
        }

        ec = rubik.color[6][3][2] * 10 + rubik.color[2][1][2];
        if (ec == ec1 || ec == ec2)
        {
            result += Operation(COMMAND(4, "DDFF"), rubik, hActive, vActive);
        }

        ec = rubik.color[5][3][2] * 10 + rubik.color[1][1][2];
        if (ec == ec2)
        {
            result += Operation(COMMAND(4, "fUlu"), rubik, hActive, vActive);
        }

        result += Operation(COMMAND(1, "H"), rubik, hActive, vActive);
    }
    return result;
}

inline std::string Solver::LayerOneCover(RUBIK &rubik)
{
    std::string result;
    for (int k = 1; k <= 4; k++)
    {
        int ec1 = Translate(rubik.color[5][2][2], rubik.color[1][1][2], rubik.color[4][1][2]);

        int ec;

        ec = Translate(rubik.color[5][3][3], rubik.color[1][1][3], rubik.color[4][1][1]);
        if (ec == ec1)
        {
            result += Operation(COMMAND(4, "rdRD"), rubik, hActive, vActive);
        }

        ec = Translate(rubik.color[5][1][3], rubik.color[4][1][3], rubik.color[2][3][3]);
        if (ec == ec1)
        {
            result += Operation(COMMAND(3, "Rdr"), rubik, hActive, vActive);
        }

        ec = Translate(rubik.color[5][1][1], rubik.color[2][3][1], rubik.color[3][1][1]);
        if (ec == ec1)
        {
            result += Operation(COMMAND(4, "lDLD"), rubik, hActive, vActive);
        }

        ec = Translate(rubik.color[5][3][1], rubik.color[3][1][3], rubik.color[1][1][1]);
        if (ec == ec1)
        {
            result += Operation(COMMAND(3, "LDl"), rubik, hActive, vActive);
        }

        ec = Translate(rubik.color[6][1][1], rubik.color[3][3][3], rubik.color[1][3][1]);
        if (ec == ec1)
        {
            result += Operation(COMMAND(1, "D"), rubik, hActive, vActive);
        }

        ec = Translate(rubik.color[6][3][3], rubik.color[4][3][3], rubik.color[2][1][3]);
        if (ec == ec1)
        {
            result += Operation(COMMAND(1, "d"), rubik, hActive, vActive);
        }

        ec = Translate(rubik.color[6][3][1], rubik.color[2][1][1], rubik.color[3][3][1]);
        if (ec == ec1)
        {
            result += Operation(COMMAND(2, "DD"), rubik, hActive, vActive);
        }

        while (rubik.color[5][2][2] != rubik.color[5][3][3] ||
               rubik.color[1][2][2] != rubik.color[1][1][3] ||
               rubik.color[4][2][2] != rubik.color[4][1][1])
        {
            result += Operation(COMMAND(4, "rdRD"), rubik, hActive, vActive);
        }
        result += Operation(COMMAND(1, "H"), rubik, hActive, vActive);
    }
    return result;
}

inline std::string Solver::ToptoBottom(RUBIK &rubik)
{
    std::string result;
    result += Operation(COMMAND(2, "VV"), rubik, hActive, vActive);
    return result;
}

inline std::string Solver::LayerTwoCover(RUBIK &rubik)
{
    std::string result;
    for (int k = 1; k <= 4; k++)
    {
        int ec1 = Translate(rubik.color[1][2][2], rubik.color[4][2][2]);
        for (int i = 1; i <= 4; i++)
        {
            int ec = Translate(rubik.color[1][2][3], rubik.color[4][2][1]);
            if (ec1 == ec) result += Operation(COMMAND(8, "URurufUF"), rubik, hActive, vActive);
            result += Operation(COMMAND(1, "H"), rubik, hActive, vActive);
        }
        for (int i = 1; i <= 4; i++)
        {
            if (rubik.color[1][2][2] == rubik.color[1][1][2] &&
                rubik.color[4][2][2] == rubik.color[5][3][2])
            {
                result += Operation(COMMAND(8, "URurufUF"), rubik, hActive, vActive);
                break;
            }
            if (rubik.color[4][2][2] == rubik.color[4][1][2] &&
                rubik.color[1][2][2] == rubik.color[5][2][3])
            {
                result += Operation(COMMAND(8, "ufUFURur"), rubik, hActive, vActive);
                break;
            }
            result += Operation(COMMAND(1, "U"), rubik, hActive, vActive);
        }
        result += Operation(COMMAND(1, "H"), rubik, hActive, vActive);
    }
    return result;
}

inline std::string Solver::LayerThreeCross(RUBIK &rubik)
{
    std::string result;
    int topcolor = rubik.color[5][2][2];
    while (1)
    {
        int topcnt = 0;
        for (int i = 1; i <= 3; i++)
            for (int j = 1; j <= 3; j++)
            {
                if ((i + j) % 2 == 1)
                {
                    topcnt += (rubik.color[5][i][j] == topcolor);
                }
            }
        if (topcnt == 4) break;
        if (topcnt == 0)
        {
            while ((rubik.color[1][1][2] == topcolor) + (rubik.color[4][1][2] == topcolor) != 2)
            {
                result += Operation(COMMAND(1, "U"), rubik, hActive, vActive);
            }
        }
        else if (topcnt == 2)
        {
            for (int i = 1; i <= 4; i++)
            {
                if ((rubik.color[5][2][3] == topcolor) + (rubik.color[5][3][2] == topcolor) == 2) break;
                result += Operation(COMMAND(1, "U"), rubik, hActive, vActive);
            }
            if ((rubik.color[5][2][3] == topcolor) + (rubik.color[5][3][2] == topcolor) != 2)
            {
                for (int i = 1; i <= 4; i++)
                {
                    if ((rubik.color[5][2][1] == topcolor) + (rubik.color[5][2][3] == topcolor) + (rubik.color[1][1][2] == topcolor) == 3) break;
                    result += Operation(COMMAND(1, "U"), rubik, hActive, vActive);
                }
            }
        }
        result += Operation(COMMAND(6, "FRUruf"), rubik, hActive, vActive);
    }
    return result;
}

inline std::string Solver::LayerThreeCover(RUBIK &rubik)
{
    std::string result;
    int topcolor = rubik.color[5][2][2];
    while (1)
    {
        int topcnt = 0;
        for (int i = 1; i <= 3; i++)
            for (int j = 1; j <= 3; j++)
            {
                topcnt += (rubik.color[5][i][j] == topcolor);
            }
        if (topcnt == 9) break;
        if (topcnt == 5)
        {
            while (rubik.color[3][1][3] != topcolor)
            {
                result += Operation(COMMAND(1, "U"), rubik, hActive, vActive);
            }
        }
        else if (topcnt == 6)
        {
            while (rubik.color[5][3][1] != topcolor)
            {
                result += Operation(COMMAND(1, "U"), rubik, hActive, vActive);
            }
        }
        else if (topcnt == 7)
        {
            while (rubik.color[1][1][1] != topcolor)
            {
                result += Operation(COMMAND(1, "U"), rubik, hActive, vActive);
            }
        }
        result += Operation(COMMAND(8, "RUrURUUr"), rubik, hActive, vActive);
    }
    return result;
}

inline std::string Solver::CornerAdjustment(RUBIK &rubik)
{
    std::string result;
    while (1)
    {
        bool flag = false;
        for (int i = 1; i <= 4; i++)
        {
            int p1 = (rubik.color[1][2][2] == rubik.color[1][1][1]) + (rubik.color[1][2][2] == rubik.color[1][1][3]);
            int p2 = (rubik.color[4][2][2] == rubik.color[4][1][1]) + (rubik.color[4][2][2] == rubik.color[4][1][3]);
            int p3 = (rubik.color[2][2][2] == rubik.color[2][3][3]) + (rubik.color[2][2][2] == rubik.color[2][3][1]);
            int p4 = (rubik.color[3][2][2] == rubik.color[3][1][1]) + (rubik.color[3][2][2] == rubik.color[3][1][3]);
            if (p1 + p2 + p3 + p4 == 8)
            {
                flag = true;
                break;
            }
            result += Operation(COMMAND(1, "U"), rubik, hActive, vActive);
        }
        if (flag) break;
        if (!flag)
        {
            for (int i = 1; i <= 4; i++)
            {
                if (rubik.color[1][1][1] == rubik.color[1][1][3])
                {
                    int cornercolor = rubik.color[1][1][1];
                    for (int j = 1; j <= 4; j++)
                    {
                        if (rubik.color[1][2][2] == cornercolor)
                        {
                            for (int k = 1; k < j; k++)
                            {
                                result += Operation(COMMAND(1, "u"), rubik, hActive, vActive);
                            }
                            break;
                        }
                        result += Operation(COMMAND(1, "H"), rubik, hActive, vActive);
                    }
                    break;
                }
                result += Operation(COMMAND(1, "U"), rubik, hActive, vActive);
            }
            result += Operation(COMMAND(12, "RbRFFrBRFFrr"), rubik, hActive, vActive);
        }
    }
    return result;
}

inline std::string Solver::EdgeAdjustment(RUBIK &rubik)
{
    std::string result;
    while (1)
    {
        if (Complete(rubik)) break;
        for (int i = 1; i <= 4; i++)
        {
            if (Complete(rubik, 1))
            {
                result += Operation(COMMAND(2, "HH"), rubik, hActive, vActive);
                break;
            }
            if (Complete(rubik, 2))
            {
                break;
            }
            if (Complete(rubik, 3))
            {
                result += Operation(COMMAND(1, "H"), rubik, hActive, vActive);
                break;
            }
            if (Complete(rubik, 4))
            {
                result += Operation(COMMAND(3, "HHH"), rubik, hActive, vActive);
                break;
            }
            result += Operation(COMMAND(1, "U"), rubik, hActive, vActive);
        }
        result += Operation(COMMAND(12, "RuRURURururr"), rubik, hActive, vActive);
    }
    return result;
}

std::string Solver::solve(RUBIK& rubik)
{
    std::string result;
    result += LayerOneCross(rubik);
    result += LayerOneCover(rubik);
    result += ToptoBottom(rubik);
    result += LayerTwoCover(rubik);
    result += LayerThreeCross(rubik);
    result += LayerThreeCover(rubik);
    result += CornerAdjustment(rubik);
    result += EdgeAdjustment(rubik);
    return result;
}
