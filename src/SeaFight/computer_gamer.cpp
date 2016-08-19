#include "stdafx.h"
#include "computer_gamer.h"

#include "window_surface.h"  // потом убрать


ComputerGamer::ComputerGamer()
{
	Coord c;
	for (int i = 0; i < 10; i++) {
		c.x = i;
		for (int j = 0; j < 10; j++) {
			c.y = j;
			possible_cell.push_back(c);
		}
	}
}

Coord ComputerGamer::Random()
{
	//std::srand(time(0));
	Coord c;
	c.x = rand() % 10;
	Sleep(100);
	c.y = rand() % 10;
	return c;
}

Coord ComputerGamer::DirectionRandom(Coord c)
{
	Coord c2 = c;
	bool b = true;
	while (b) {
		b = false;
		int r = rand() % 4;
		Sleep(100);
		switch (r) {
		case 0:
			c2.x--;    // лево
			break;
		case 1:
			c2.x++;		//право
			break;
		case 2:
			c2.y--;		//верх
			break;
		case 3:
			c2.y++;		//низ
			break;
		}

		for (int i = 0; i < 4; i++) {
			if (c2 == created_ship.GetCoord(i)) {
				b = true;
				break;
			}
		}
	}
	return c2;
}

int ComputerGamer::SelectDeck(int deck_index)
{
	if (deck_index == 0)
		return 0;
	return rand() % deck_index;
}

void ComputerGamer::ClearCrossCells()
{
	for (int i = possible_cell.size() - 1; i >= 0 ; i--) {
		if (cross == board().WhatInTheCell(possible_cell.at(i)))
			possible_cell.erase(possible_cell.begin() + i);
	}
}

void ComputerGamer::SearchPossibleLocationDecks()
{
	possible_location_decks.clear();
	std::vector<Coord>::iterator it;
	Coord c;

	for (int i = 0; i < used_ship.size(); i++) {
		c = used_ship.at(i);

		c.x--;																	// »щем левее
		if (c.x >= 0) {
			it = find(possible_cell.begin(), possible_cell.end(), c);
			if (it != possible_cell.end())
				possible_location_decks.push_back(c);
		}

		c.x += 2;																// »щем правее
		if (c.x < 10) {
			it = find(possible_cell.begin(), possible_cell.end(), c);
			if (it != possible_cell.end())
				possible_location_decks.push_back(c);
		}

		c.x--;																	// »щем выше
		c.y--;
		if (c.y >= 0) {
			it = find(possible_cell.begin(), possible_cell.end(), c);
			if (it != possible_cell.end())
				possible_location_decks.push_back(c);
		}

		c.y += 2;																// »щем ниже
		if (c.y < 10) {
			it = find(possible_cell.begin(), possible_cell.end(), c);
			if (it != possible_cell.end())
				possible_location_decks.push_back(c);
		}
	}
}

Ship ComputerGamer::CreatShip(int decks)
{
	created_ship = Ship();
	created_ship.SetDecks(decks);
	created_ship.SetCoord(Random());

	for (int i = 1; i < decks; i++) {
		Coord c = DirectionRandom(created_ship.GetCoord(SelectDeck(i)));
		if (c.x >= 0 && c.x < 10 && c.y >= 0 && c.y < 10) {
			created_ship.SetCoord(c);
		}
		else
			i--;
	}

	return created_ship;																			
}

void ComputerGamer::Fire()
{
	Event e = hit;
	int r;
	std::string str;
	str = "                                                                            ";
	WINSURF.DrawString(str.c_str());
	str = "’од компьютера";
	WINSURF.DrawString(str.c_str());
	Sleep(1000);

	while (e != past && e != game_ower) {
		if (used_ship.size() == 0) {
			r = rand() % possible_cell.size();
			e = board().FireComputer(possible_cell.at(r));
			if (e == hit) {
				used_ship.push_back(possible_cell.at(r));
				possible_cell.erase(possible_cell.begin() + r);
				Sleep(1000);
			}
			else if (e == killed) {
				possible_cell.erase(possible_cell.begin() + r);
				ClearCrossCells();
				used_ship.clear();
				if (board().ShipsOnPlayerBoard() == 0)
					e = game_ower;
				Sleep(1000);
			}
			else {
				possible_cell.erase(possible_cell.begin() + r);
				Sleep(1000);
			}
		}
		else {
			std::vector<Coord>::iterator it;
			Coord c;
			SearchPossibleLocationDecks();
			r = rand() % possible_location_decks.size();
			c = possible_location_decks.at(r);
			e = board().FireComputer(c);
			if (e == hit) {
				used_ship.push_back(c);
				it = find(possible_cell.begin(), possible_cell.end(), c);
				possible_cell.erase(possible_cell.begin() + (it - possible_cell.begin()));
				Sleep(1000);
			}
			else if (e == killed) {
				used_ship.clear();
				ClearCrossCells();
				it = find(possible_cell.begin(), possible_cell.end(), c);
				possible_cell.erase(possible_cell.begin() + (it - possible_cell.begin()));
				possible_location_decks.clear();
				if (board().ShipsOnPlayerBoard() == 0)
					e = game_ower;
				Sleep(1000);

			}
			else {
				it = find(possible_cell.begin(), possible_cell.end(), c);
				possible_cell.erase(possible_cell.begin() + (it - possible_cell.begin()));
				Sleep(1000);
			}
		}
	}

}


