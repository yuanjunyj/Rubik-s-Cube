#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cmath>
#include <cstring>
#include <string>
#include <algorithm>

struct RUBIK
{
	int color[7][4][4];
	inline void Print()
	{
		printf("0-Red, 1-Green, 2-Blue, 3-Orange, 4-White, 5-Yellow\n\n");
		for (int i = 1; i <= 6; i += 2)
		{
			if (i == 1) printf("Front:    Back:\n");
			if (i == 3) printf("Left:     Right:\n");
			if (i == 5) printf("Up:       Down:\n");
			for (int j = 1; j <= 3; j++)
			{
				for (int k = 1; k <= 3; k++)
				{
					printf("%d", color[i][j][k]);
				}
				printf("       ");
				for (int k = 1; k <= 3; k++)
				{
					printf("%d", color[i + 1][j][k]);
				}
				printf("\n");
			}
			printf("\n");
		}
	}
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

inline void Rotate_Surface(RUBIK &rubik, int id);
inline void Rotate_Edge(RUBIK &rubik, int id);
inline void Update(RUBIK &rubik, int id, int tm);
inline void Operation(COMMAND command, RUBIK &rubik, int &hActive, int &vActive);

inline int Translate(int a, int b);
inline int Translate(int a, int b, int c);
inline char opTranslate(int op, int hActive, int vActive);

inline bool Complete(RUBIK &rubik);
inline bool Complete(RUBIK &rubik, int n);

inline void Rotate_Surface(RUBIK &rubik, int id)
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

inline void Rotate_Edge(RUBIK &rubik, int id)
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

inline void Update(RUBIK &rubik, int id, int tm)
{
	for (int i = 1; i <= tm; i++)
	{
		if (id <= 6) Rotate_Surface(rubik, id);
		Rotate_Edge(rubik, id);
	}
}

inline void Operation(COMMAND command, RUBIK &rubik, int &hActive, int &vActive)
{
    char sigma[] = " FBLRUDHV";

	for (int k = 0; k < command.length; k++)
	{
		for (int i = 1; i <= 6; i++)
		{
			if (command.st[k] == sigma[i])
			{
				Update(rubik, i, 1);
				std::cout << opTranslate(i, hActive, vActive);
			}
			else if (toupper(command.st[k]) == sigma[i])
			{
				Update(rubik, i, 3);
				std::cout << opTranslate(i + 6, hActive, vActive);
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
}

inline int Translate(int a, int b)
{
	int B = std::max(a, b);
	int S = std::min(a, b);
	return B * 10 + S;
}

inline int Translate(int a, int b, int c)
{
	int B = std::max(std::max(a, b), c);
	int S = std::min(std::min(a, b), c);
	int M = a + b + c - B - S;
	return B * 100 + S * 10 + M;
}

inline char opTranslate(int op, int hActive, int vActive)
{
	char image[8][14] = {" FBLRUDfblrud", " RLFBUDrlfbud", " BFRLUDbfrlud", " LRBFUDlrbfud",
						 " BFLRDUbflrdu", " RLBFDUrlbfdu", " FBRLDUfbrldu", " LRFBDUlrfbdu"};
    int label = (vActive > 0) * 4 + (hActive % 4);
	return image[label][op];
}

inline bool Complete(RUBIK &rubik)
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

inline bool Complete(RUBIK &rubik, int k)
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
