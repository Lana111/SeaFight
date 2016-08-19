#pragma once
#include "ship.h"

enum Cell {
	empty,
	ship,
	stricken_ship,
	shot,
	cross
};

enum Event {
	past,
	hit,
	killed,
	can_not,
	game_ower
};

#define BOARD Board::instance()

class Board
{
public:
	~Board() = default;
	static Board &instance();

	bool AddPlayerShipCandidate(const Ship &ship);
	bool AddComputerShipCandidate(const Ship &ship);
	Event FireComputer(const Coord &c);
	Event FirePlayer(const Coord &c);

	Cell WhatInTheCell(Coord c);
	int ShipsOnPlayerBoard();
	int ShipsOnComputerBoard();

private:
	Board();
	Board(const Board &);
	Board &operator=(const Board &);

	Cell player_map[10][10];
	Cell computer_map[10][10];
	std::vector<Ship> player_ship;
	std::vector<Ship> computer_ship;
	Ship ship_candidate;

	void DrawCross(const Coord &c, Cell map[10][10]);
	void DrawCrosses(const Ship &ship, Cell map[10][10]);
	void AddShip(WhoosBoard w, const Ship &ship);
	bool IsValidShip(WhoosBoard w) const;
	void ClearShipCandidate();

	int ShipsOnBoard(WhoosBoard w);
};

inline Board &board() {
	return Board::instance();
}