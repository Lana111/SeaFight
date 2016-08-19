#include "stdafx.h"
#include "ship.h"


Ship::Ship()
{
	decks = 0;

	for (int i = 0; i < 4; i++) {
		coord[i] = { -1, -1 };
		state[i] = green;
	}
}

void Ship::SetDecks(int d)
{
	decks = d;
}

int Ship::GetDecks() const
{
	return decks;
}

void Ship::SetCoord(Coord c)
{
	int i = 0;
	while (i < decks && coord[i].x != -1)
		i++;
	if (i < decks)
		coord[i] = c;
}

Coord Ship::GetCoord(int i) const
{
	return coord[i];
}

void Ship::SetState(int i, State s)
{
	state[i] = s;
}

State Ship::GetState(int i) const
{
	return state[i];
}

bool Ship::IsValidShip() const
{
	int sum = 0;  // Сумма связей палуб
	int i = 0;

	while (i < decks) {
		for (int j = i + 1; j < decks; j++) {
			if (coord[i].x == coord[j].x && (coord[i].y + 1 == coord[j].y || coord[i].y - 1 == coord[j].y) ||
				coord[i].y == coord[j].y && (coord[i].x + 1 == coord[j].x || coord[i].x - 1 == coord[j].x))
				sum += 2;
			if (coord[i].x == coord[j].x && coord[i].y == coord[j].y)
				return false;
		}

		i++;
	}

	if (sum >= 2 * (decks - 1))
		return true;

	return false;
}

bool Ship::IsValidShip(const Ship & s) const
{
	for (int i = 0; i < decks; i++) {
		for (int j = 0; j < s.decks; j++) {
			if (coord[i].x >= s.coord[j].x - 1 && coord[i].x <= s.coord[j].x + 1 &&
				coord[i].y >= s.coord[j].y - 1 && coord[i].y <= s.coord[j].y + 1)
				return false;
		}
	}

	return true;
}

bool Ship::IsDead() const
{
	for (int i = 0; i < decks; i++) {
		if (state[i] == green)
			return false;
	}
	return true;
}
