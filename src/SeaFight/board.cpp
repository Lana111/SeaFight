#include "stdafx.h"
#include "board.h"
#include "window_surface.h"


Board::Board()
{
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			player_map[i][j] = empty;
			computer_map[i][j] = empty;
		}
	}
	WINSURF.DrawPlayerGameMap();
	WINSURF.DrawComputerGameMap();
}

Board &Board::instance()
{
	static Board single_instance;
	return single_instance;
}

void Board::AddShip(WhoosBoard w, const Ship & ship)
{
	if (w == Player) {
		player_ship.push_back(ship);

		for (int i = 0; i < ship.GetDecks(); i++) {
			Coord c = ship.GetCoord(i);
			player_map[c.x][c.y] = Cell::ship;
			WINSURF.DrawCharOnPlayerGameMap(178, c, Green);
		}
	}
	else {
		computer_ship.push_back(ship);
		for (int i = 0; i < ship.GetDecks(); i++) {
			Coord c = ship.GetCoord(i);
			computer_map[c.x][c.y] = Cell::ship;
			//WINSURF.DrawCharOnComputerGameMap('C', c, Green);
		}
	}
}

bool Board::AddPlayerShipCandidate(const Ship & ship)
{
	ship_candidate = ship;
	int i = 0;
	for (; i < ship_candidate.GetDecks() && ship_candidate.GetCoord(i).x != -1; ++i) {
		WINSURF.DrawCharOnPlayerGameMap(178, ship_candidate.GetCoord(i), LightBlue);
	}
	if (i == ship_candidate.GetDecks()) {
		if (!ship_candidate.IsValidShip()) {
			WINSURF.DrawString("Непривильная геометрия корабля. Для продолжения пикните в доску.");
			WINSURF.Peak(Player);
			for (int i = 0; i < ship_candidate.GetDecks(); i++) {
				if (ship_candidate.GetCoord(i).x != -1)
					WINSURF.DrawCharOnPlayerGameMap(' ', ship_candidate.GetCoord(i), LightGray);
			}
			ClearShipCandidate();
			return false;
		}
		if (!IsValidShip(Player)) {
			WINSURF.DrawString("Контакт между кораблями. Для продолжения пикните в доску.");
			WINSURF.Peak(Player);
			for (int i = 0; i < ship_candidate.GetDecks(); i++) {
				if (ship_candidate.GetCoord(i).x != -1)
					WINSURF.DrawCharOnPlayerGameMap(' ', ship_candidate.GetCoord(i), LightGray);
			}
			ClearShipCandidate();
			return false;
		}
		for (int i = 0; i < ship_candidate.GetDecks(); i++) {
			if (ship_candidate.GetCoord(i).x != -1)
				WINSURF.DrawCharOnPlayerGameMap(' ', ship_candidate.GetCoord(i), LightGray);
		}
		AddShip(Player, ship_candidate);
		ship_candidate = Ship();
		return true;
	}
	return false;
}

bool Board::AddComputerShipCandidate(const Ship & ship)
{
	ship_candidate = ship;
	if (!IsValidShip(Computer))
		return false;
	AddShip(Computer, ship_candidate);
	return true;
}

Event Board::FireComputer(const Coord & c)
{
	if (player_map[c.x][c.y] == empty) {
		player_map[c.x][c.y] = shot;
		WINSURF.DrawCharOnPlayerGameMap('.', c, LightGray);
		return past;
	}
	else if (player_map[c.x][c.y] == ship) {
		for (int i = 0; i < static_cast<int>(player_ship.size()); i++) {
			for (int j = 0; j < player_ship[i].GetDecks(); j++) {
				if (const_cast<Coord &>(c) == player_ship[i].GetCoord(j)) {
					player_ship[i].SetState(j, red);
					player_map[c.x][c.y] = stricken_ship;
					WINSURF.DrawCharOnPlayerGameMap(178, c, Red);
					if (player_ship[i].IsDead()) {
						DrawCrosses(player_ship[i], player_map);
						return killed;
					}
					return hit;
				}
			}
		}
	}
	return past;
}

Event Board::FirePlayer(const Coord & c)
{
	if (computer_map[c.x][c.y] == empty) {
		computer_map[c.x][c.y] = shot;
		WINSURF.DrawCharOnComputerGameMap('.', c, LightGray);
		return past;
	}
	else if (computer_map[c.x][c.y] == ship) {
		for (int i = 0; i < static_cast<int>(computer_ship.size()); i++) {
			for (int j = 0; j < computer_ship[i].GetDecks(); j++) {
				if (const_cast<Coord &>(c) == computer_ship[i].GetCoord(j)) {
					computer_ship[i].SetState(j, red);
					computer_map[c.x][c.y] = stricken_ship;
					WINSURF.DrawCharOnComputerGameMap(178, c, Red);
					if (computer_ship[i].IsDead()) {
						DrawCrosses(computer_ship[i], computer_map);
						return killed;
					}
					return hit;
				}
			}
		}
	}
	return can_not;
}

Cell Board::WhatInTheCell(Coord c)
{
	Cell cell = player_map[c.x][c.y];
	return cell;
}

int Board::ShipsOnPlayerBoard()
{
	return ShipsOnBoard(Player);
}

int Board::ShipsOnComputerBoard()
{
	return ShipsOnBoard(Computer);
}

bool Board::IsValidShip(WhoosBoard w) const
{
	for (int i = 0; i < (w == Player ? player_ship.size() : computer_ship.size()); i++) {
		if (!ship_candidate.IsValidShip(w == Player ? player_ship.at(i) : computer_ship.at(i)))
			return false;
	}
	return true;
}

void Board::ClearShipCandidate()
{
	for (int i = 0; i < ship_candidate.GetDecks(); i++) {
		WINSURF.DrawCharOnPlayerGameMap(' ', ship_candidate.GetCoord(i), LightGray);
	}
	ship_candidate = Ship();
}

int Board::ShipsOnBoard(WhoosBoard w)
{
	std::vector<Ship> *p_ship = w == Player ? &player_ship : &computer_ship;
	
	int j = 0;
	for (int i = 0; i < p_ship->size(); i++) {
		if (!p_ship->at(i).IsDead())
			j++;
	}
	return j;
}

void Board::DrawCross(const Coord & c, Cell map[10][10])
{
	if (c.x >= 0 && c.x < 10 && c.y >= 0 && c.y < 10 && (map[c.x][c.y] == empty || map[c.x][c.y] == shot)) {
		map[c.x][c.y] = cross;
		if (map == player_map)
			WINSURF.DrawCharOnPlayerGameMap('X', c, LightGray);
		else
			WINSURF.DrawCharOnComputerGameMap('X', c, LightGray);
	}

}

void Board::DrawCrosses(const Ship & ship, Cell map[10][10])
{
	for (int i = 0; i < ship.GetDecks(); i++) {
		Coord c = ship.GetCoord(i);
		for (int x = c.x - 1; x < c.x + 2; x++) {
			int y = c.y - 1;
			DrawCross({ x, y++ }, map);
			DrawCross({ x, y++ }, map);
			DrawCross({ x, y }, map);
		}
	}
}








/*
}*/

