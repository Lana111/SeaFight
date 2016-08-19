// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <list>
#include <algorithm>

struct Coord
{
	int x;  // ������� �� �����������
	int y;  // ������� �� ���������
	bool operator == (const Coord &other) {
		if (x == other.x && y == other.y)
			return true;
		else
			return false;
	}
};

enum WhoosBoard {  // NOTE: ��� �����?
	Player,
	Computer
};

