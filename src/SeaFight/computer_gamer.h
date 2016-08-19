#pragma once
#include "ship.h"
#include "board.h"

class ComputerGamer
{
public:
	ComputerGamer();
	~ComputerGamer() = default;

	Ship CreatShip(int decks);
	void Fire();

private:
	Ship created_ship;
	std::vector<Coord> used_ship;
	std::vector<Coord> possible_cell;
	std::vector<Coord> possible_location_decks;

	Coord Random();
	Coord DirectionRandom(Coord c);
	int SelectDeck(int deck_index);
	void ClearCrossCells();
	void SearchPossibleLocationDecks();

};

