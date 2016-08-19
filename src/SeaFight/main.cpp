// SeaFight.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "window_surface.h"
#include "color.h"
#include "board.h"
#include "manager.h"
#include "computer_gamer.h"


int main()
{
	
	std::srand(time(0));
	Manager m;
	m.Run();
	system("pause");
    return 0;
}

