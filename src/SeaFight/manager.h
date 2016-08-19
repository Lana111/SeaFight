#pragma once
#include "ship.h"
#include "computer_gamer.h"

class Manager
{
public:
	Manager();
	~Manager() = default;

	void Run();

private:
	ComputerGamer cg;

	void AddPlayerShip(int decks);
	void AddPlayerShips();
	void AddComputerShip(int decks);
	void AddComputerShips();
	void PlayerQueue();
};

