#ifndef SHIP_H_
#define SHIP_H_

enum State
{
	green,	// Целый
	red		// Попали
};

class Ship
{
public:
	Ship();
	~Ship() = default;

	Ship(const Ship & other) = default;
	Ship & operator = (const Ship & other) = default;

	void SetDecks(int d);
	int GetDecks() const;
	void SetCoord(Coord c);
	Coord GetCoord(int i) const;
	void SetState(int i, State s);
	State GetState(int i) const;
	bool IsValidShip() const;
	bool IsValidShip(const Ship &s) const;
	bool IsDead() const;

private:
	int decks;
	Coord coord[4];
	State state[4];
};

#endif