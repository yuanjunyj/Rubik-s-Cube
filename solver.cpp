#include "solver.h"

using namespace std;

char ch;
int hActive = 0, vActive = 0;

//FBLRUD
//012345

inline std::string LayerOneCross(RUBIK &rubik);
inline void LayerOneCover(RUBIK &rubik);
inline void LayerTwoCover(RUBIK &rubik);
inline void LayerThreeCross(RUBIK &rubik);
inline void LayerThreeCover(RUBIK &rubik);
inline void CornerAdjustment(RUBIK &rubik);
inline void EdgeAdjustment(RUBIK &rubik);

inline std::string LayerOneCross(RUBIK &rubik)
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
            Operation(COMMAND(6, "BBDDFF"), rubik, hActive, vActive);
        }

        ec = rubik.color[5][2][1] * 10 + rubik.color[3][1][2];
        if (ec == ec1 || ec == ec2)
        {
            Operation(COMMAND(5, "LLDFF"), rubik, hActive, vActive);
        }

        ec = rubik.color[5][2][3] * 10 + rubik.color[4][1][2];
        if (ec == ec1 || ec == ec2)
        {
            Operation(COMMAND(5, "RRdFF"), rubik, hActive, vActive);
        }

        ec = rubik.color[1][2][3] * 10 + rubik.color[4][2][1];
        if (ec == ec1 || ec == ec2)
        {
            Operation(COMMAND(1, "f"), rubik, hActive, vActive);
        }

        ec = rubik.color[4][2][3] * 10 + rubik.color[2][2][3];
        if (ec == ec1 || ec == ec2)
        {
            Operation(COMMAND(5, "RdFFr"), rubik, hActive, vActive);
        }

        ec = rubik.color[2][2][1] * 10 + rubik.color[3][2][1];
        if (ec == ec1 || ec == ec2)
        {
            Operation(COMMAND(6, "lDFFL"), rubik, hActive, vActive);
        }

        ec = rubik.color[3][2][3] * 10 + rubik.color[1][2][1];
        if (ec == ec1 || ec == ec2)
        {
            Operation(COMMAND(1, "F"), rubik, hActive, vActive);
        }

        ec = rubik.color[6][1][2] * 10 + rubik.color[1][3][2];
        if (ec == ec1 || ec == ec2)
        {
            Operation(COMMAND(2, "FF"), rubik, hActive, vActive);
        }

        ec = rubik.color[6][2][1] * 10 + rubik.color[3][3][2];
        if (ec == ec1 || ec == ec2)
        {
            Operation(COMMAND(3, "DFF"), rubik, hActive, vActive);
        }

        ec = rubik.color[6][2][3] * 10 + rubik.color[4][3][2];
        if (ec == ec1 || ec == ec2)
        {
            Operation(COMMAND(3, "dFF"), rubik, hActive, vActive);
        }

        ec = rubik.color[6][3][2] * 10 + rubik.color[2][1][2];
        if (ec == ec1 || ec == ec2)
        {
            Operation(COMMAND(4, "DDFF"), rubik, hActive, vActive);
        }

        ec = rubik.color[5][3][2] * 10 + rubik.color[1][1][2];
        if (ec == ec2)
        {
            Operation(COMMAND(4, "fUlu"), rubik, hActive, vActive);
        }

        Operation(COMMAND(1, "H"), rubik, hActive, vActive);
        printf(" ");
    }
    printf("\n");
}

inline void LayerOneCover(RUBIK &rubik)
{
    for (int k = 1; k <= 4; k++)
    {
        int ec1 = Translate(rubik.color[5][2][2], rubik.color[1][1][2], rubik.color[4][1][2]);

        int ec;

        ec = Translate(rubik.color[5][3][3], rubik.color[1][1][3], rubik.color[4][1][1]);
        if (ec == ec1)
        {
            Operation(COMMAND(4, "rdRD"), rubik, hActive, vActive);
        }

        ec = Translate(rubik.color[5][1][3], rubik.color[4][1][3], rubik.color[2][3][3]);
        if (ec == ec1)
        {
            Operation(COMMAND(3, "Rdr"), rubik, hActive, vActive);
        }

        ec = Translate(rubik.color[5][1][1], rubik.color[2][3][1], rubik.color[3][1][1]);
        if (ec == ec1)
        {
            Operation(COMMAND(4, "lDLD"), rubik, hActive, vActive);
        }

        ec = Translate(rubik.color[5][3][1], rubik.color[3][1][3], rubik.color[1][1][1]);
        if (ec == ec1)
        {
            Operation(COMMAND(3, "LDl"), rubik, hActive, vActive);
        }

        ec = Translate(rubik.color[6][1][1], rubik.color[3][3][3], rubik.color[1][3][1]);
        if (ec == ec1)
        {
            Operation(COMMAND(1, "D"), rubik, hActive, vActive);
        }

        ec = Translate(rubik.color[6][3][3], rubik.color[4][3][3], rubik.color[2][1][3]);
        if (ec == ec1)
        {
            Operation(COMMAND(1, "d"), rubik, hActive, vActive);
        }

        ec = Translate(rubik.color[6][3][1], rubik.color[2][1][1], rubik.color[3][3][1]);
        if (ec == ec1)
        {
            Operation(COMMAND(2, "DD"), rubik, hActive, vActive);
        }

        while (rubik.color[5][2][2] != rubik.color[5][3][3] ||
               rubik.color[1][2][2] != rubik.color[1][1][3] ||
               rubik.color[4][2][2] != rubik.color[4][1][1])
        {
            Operation(COMMAND(4, "rdRD"), rubik, hActive, vActive);
        }
        Operation(COMMAND(1, "H"), rubik, hActive, vActive);
        printf(" ");
    }
    printf("\n");
}

inline void ToptoBottom(RUBIK &rubik)
{
    Operation(COMMAND(2, "VV"), rubik, hActive, vActive);
}

inline void LayerTwoCover(RUBIK &rubik)
{
    for (int k = 1; k <= 4; k++)
    {
        int ec1 = Translate(rubik.color[1][2][2], rubik.color[4][2][2]);
        for (int i = 1; i <= 4; i++)
        {
            int ec = Translate(rubik.color[1][2][3], rubik.color[4][2][1]);
            if (ec1 == ec) Operation(COMMAND(8, "URurufUF"), rubik, hActive, vActive);
            Operation(COMMAND(1, "H"), rubik, hActive, vActive);
        }
        for (int i = 1; i <= 4; i++)
        {
            if (rubik.color[1][2][2] == rubik.color[1][1][2] &&
                rubik.color[4][2][2] == rubik.color[5][3][2])
            {
                Operation(COMMAND(8, "URurufUF"), rubik, hActive, vActive);
                break;
            }
            if (rubik.color[4][2][2] == rubik.color[4][1][2] &&
                rubik.color[1][2][2] == rubik.color[5][2][3])
            {
                Operation(COMMAND(8, "ufUFURur"), rubik, hActive, vActive);
                break;
            }
            Operation(COMMAND(1, "U"), rubik, hActive, vActive);
        }
        Operation(COMMAND(1, "H"), rubik, hActive, vActive);
        printf(" ");
    }
    printf("\n");
}

inline void LayerThreeCross(RUBIK &rubik)
{
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
                Operation(COMMAND(1, "U"), rubik, hActive, vActive);
            }
        }
        else if (topcnt == 2)
        {
            for (int i = 1; i <= 4; i++)
            {
                if ((rubik.color[5][2][3] == topcolor) + (rubik.color[5][3][2] == topcolor) == 2) break;
                Operation(COMMAND(1, "U"), rubik, hActive, vActive);
            }
            if ((rubik.color[5][2][3] == topcolor) + (rubik.color[5][3][2] == topcolor) != 2)
            {
                for (int i = 1; i <= 4; i++)
                {
                    if ((rubik.color[5][2][1] == topcolor) + (rubik.color[5][2][3] == topcolor) + (rubik.color[1][1][2] == topcolor) == 3) break;
                    Operation(COMMAND(1, "U"), rubik, hActive, vActive);
                }
            }
        }
        Operation(COMMAND(6, "FRUruf"), rubik, hActive, vActive);
    }
    printf("\n");
}

inline void LayerThreeCover(RUBIK &rubik)
{
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
                Operation(COMMAND(1, "U"), rubik, hActive, vActive);
            }
        }
        else if (topcnt == 6)
        {
            while (rubik.color[5][3][1] != topcolor)
            {
                Operation(COMMAND(1, "U"), rubik, hActive, vActive);
            }
        }
        else if (topcnt == 7)
        {
            while (rubik.color[1][1][1] != topcolor)
            {
                Operation(COMMAND(1, "U"), rubik, hActive, vActive);
            }
        }
        Operation(COMMAND(8, "RUrURUUr"), rubik, hActive, vActive);
    }
    printf("\n");
}

inline void CornerAdjustment(RUBIK &rubik)
{
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
            Operation(COMMAND(1, "U"), rubik, hActive, vActive);
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
                                Operation(COMMAND(1, "u"), rubik, hActive, vActive);
                            }
                            break;
                        }
                        Operation(COMMAND(1, "H"), rubik, hActive, vActive);
                    }
                    break;
                }
                Operation(COMMAND(1, "U"), rubik, hActive, vActive);
            }
            Operation(COMMAND(12, "RbRFFrBRFFrr"), rubik, hActive, vActive);
        }
    }
    printf("\n");
}

inline void EdgeAdjustment(RUBIK &rubik)
{
    while (1)
    {
        if (Complete(rubik)) break;
        for (int i = 1; i <= 4; i++)
        {
            if (Complete(rubik, 1))
            {
                Operation(COMMAND(2, "HH"), rubik, hActive, vActive);
                break;
            }
            if (Complete(rubik, 2))
            {
                break;
            }
            if (Complete(rubik, 3))
            {
                Operation(COMMAND(1, "H"), rubik, hActive, vActive);
                break;
            }
            if (Complete(rubik, 4))
            {
                Operation(COMMAND(3, "HHH"), rubik, hActive, vActive);
                break;
            }
            Operation(COMMAND(1, "U"), rubik, hActive, vActive);
        }
        Operation(COMMAND(12, "RuRURURururr"), rubik, hActive, vActive);
    }
    printf("\n");
}

int main()
{
    freopen("rubik.in", "r", stdin);
    freopen("rubik.out", "w", stdout);

    RUBIK rubik;
    for (int i = 1; i <= 6; i++)
    {
        for (int j = 1; j <= 3; j++)
            for (int k = 1; k <= 3; k++)
            {
                scanf(" %c", &ch);
                rubik.color[i][j][k] = ch - 48;
            }
    }

    rubik.Print();
    LayerOneCross(rubik);
    LayerOneCover(rubik);
    ToptoBottom(rubik);
    LayerTwoCover(rubik);
    LayerThreeCross(rubik);
    LayerThreeCover(rubik);
    CornerAdjustment(rubik);
    EdgeAdjustment(rubik);
    rubik.Print();
    return 0;
}
