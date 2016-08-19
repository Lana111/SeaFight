#include "stdafx.h"
#include "manager.h"
#include "window_surface.h"
#include "board.h"


Manager::Manager()
{
	board();
}

void Manager::AddPlayerShip(int decks)
{
	std::string str;
	str += "Создайте ";
	str += std::to_string(decks);
	str += "-палубный корабль                        ";
	bool b = false;

	
	while (!b) {
		WINSURF.DrawString(str.c_str());
		Coord c = { -1, -1 };
		Ship s;
		s.SetDecks(decks);

		for (int i = 0; i < decks; i++) {
			c = WINSURF.Peak(Player);
			if (board().WhatInTheCell(c) == empty) {
				s.SetCoord(c);
				b = board().AddPlayerShipCandidate(s);
			}
		}
	}                                                                  
}

void Manager::AddPlayerShips()
{
	AddPlayerShip(4);
	AddPlayerShip(3);
	AddPlayerShip(3);
	AddPlayerShip(2);
	AddPlayerShip(2);
	AddPlayerShip(2);
	AddPlayerShip(1);
	AddPlayerShip(1);
	AddPlayerShip(1);
	AddPlayerShip(1);
}

void Manager::AddComputerShip(int decks)
{
	bool b = false;

	while (!b) {
		b = board().AddComputerShipCandidate(cg.CreatShip(decks));
	}
}

void Manager::AddComputerShips()
{
	AddComputerShip(4);
	AddComputerShip(3);
	AddComputerShip(3);
	AddComputerShip(2);
	AddComputerShip(2);
	AddComputerShip(2);
	AddComputerShip(1);
	AddComputerShip(1);
	AddComputerShip(1);
	AddComputerShip(1);
}

void Manager::PlayerQueue()
{
	Event e = hit;
	std::string str;
	Coord c = { -1, -1 };

	while (e != past && e != game_ower) {
		str = "                                                                            ";
		WINSURF.DrawString(str.c_str());
		str = "Ваш ход";
		WINSURF.DrawString(str.c_str());
		c = WINSURF.Peak(Computer);
		if (c.x != -1)
			e = board().FirePlayer(c);
		if (e == hit) {
			str = "                                                                            ";
			WINSURF.DrawString(str.c_str());
			str = "Ранили. Ваш ход";
			WINSURF.DrawString(str.c_str());
			Sleep(1000);
		}
		else if (e == killed) {
			str = "                                                                            ";
			WINSURF.DrawString(str.c_str());
			str = "Убили. Ваш ход";
			WINSURF.DrawString(str.c_str());
			Sleep(1000);
			if (board().ShipsOnComputerBoard() == 0)
				e = game_ower;
		}
		else if (e == can_not) {
			continue;
		}
		else {
			str = "                                                                            ";
			WINSURF.DrawString(str.c_str());
			str = "Мимо. Ход передаеться компьютеру";
			WINSURF.DrawString(str.c_str());
			Sleep(1000);
		}
	}
}

void Manager::Run()
{
	bool b = false;
	std::string str;

	AddPlayerShips();
	AddComputerShips();

	while (!b) {
		PlayerQueue();
		if (board().ShipsOnComputerBoard() == 0) {
			str = "Вы выиграли. ";
			b = true;
		}
		cg.Fire();
		if (board().ShipsOnPlayerBoard() == 0) {
			str = "Компьютер выиграл. ";
			b = true;
		}
	}
	str += "Игра оконченна! \n";
	WINSURF.DrawString(str.c_str());
}
