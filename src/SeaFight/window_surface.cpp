#include "stdafx.h"
#include "window_surface.h"

static const char middlRight = '\271', vertical = '\272', rUpCorner = '\273', rDownCorner = '\274',
lDownCorner = '\310', lUpCorner = '\311', middlDown = '\312', middlUp = '\313',
middlLeft = '\314', horizontal = '\315', cross = '\316';

static const int PlayerX0 = 2;
static const int PlayerY0 = 2;
static const int ComputerX0 = 25;
static const int ComputerY0 = 2;

WindowSurface::WindowSurface()
	: handle_(GetStdHandle(STD_OUTPUT_HANDLE))
{
	CONSOLE_CURSOR_INFO cci = { sizeof(cci), false };
	SetConsoleCursorInfo(handle_, &cci);
}

// Создание объекта
const WindowSurface &WindowSurface::instance()
{
	static const WindowSurface single_instance;
	return single_instance;
}

void WindowSurface::GotoXY(int x, int y) const
{
	COORD coord = { static_cast<short>(x), static_cast<short>(y) };
	SetConsoleCursorPosition(handle_, coord);
}

void WindowSurface::WriteChar(char ch, int x, int y) const
{
	SetConsoleTextAttribute(handle_, LightGray);
	GotoXY(x, y);
	std::cout << ch;
}

void WindowSurface::WriteChar(char ch, int x, int y, Color foreground) const
{
	SetConsoleTextAttribute(handle_, foreground);
	GotoXY(x, y);
	std::cout << ch;
}
void WindowSurface::DrawGameMap(int x, int y) const
{
	int i = 0, j = 0;

	//Верх поля
	WriteChar(lUpCorner, x + i, y + j);
	i++;
	WriteChar(horizontal, x + i, y + j);
	i++;
	for (; i < 20; i++) {
		WriteChar(middlUp, x + i, y + j);
		i++;
		WriteChar(horizontal, x + i, y + j);
	}
	WriteChar(rUpCorner, x + i, y + j);

	//Середина

	for (j = 1; j < 20; j += 2) {
		for (i = 0; i <= 20; i += 2) {
			WriteChar(vertical, x + i, y + j);
		}
		i = 0;
		WriteChar(middlLeft, x + i, y + j + 1);
		for (i = 1; i <= 20; i++) {
			WriteChar(horizontal, x + i, y + j + 1);
			i++;
			WriteChar(cross, x + i, y + j + 1);
		}
		WriteChar(middlRight, x + i - 1, y + j + 1);
	}

	//Низ
	i = 0; j--;
	WriteChar(lDownCorner, x + i, y + j);
	i++;
	WriteChar(horizontal, x + i, y + j);
	i++;
	for (; i < 19; i++) {
		WriteChar(middlDown, x + i, y + j);
		i++;
		WriteChar(horizontal, x + i, y + j);
	}
	WriteChar(rDownCorner, x + i, y + j);
}

void WindowSurface::DrawString(const char *str) const
{
	char *buf = new char[strlen(str) + 1];
	CharToOemA(str, buf);

	SetConsoleTextAttribute(handle_, LightGray);
	GotoXY(PlayerX0 + 2, PlayerY0 + 25);
	std::cout << buf;

	return;
}

void WindowSurface::DrawPlayerGameMap() const
{
	DrawGameMap(PlayerX0, PlayerY0);
}

void WindowSurface::DrawComputerGameMap() const
{
	DrawGameMap(ComputerX0, ComputerY0);
}

void WindowSurface::DrawCharOnPlayerGameMap(char ch, Coord board_c, Color foreground) const
{
	WriteChar(ch, board_c.x * 2 + PlayerX0 + 1, board_c.y * 2 + PlayerY0 + 1, foreground);
}

void WindowSurface::DrawCharOnComputerGameMap(char ch, Coord board_c, Color foreground) const
{
	WriteChar(ch, board_c.x * 2 + ComputerX0 + 1, board_c.y * 2 + ComputerY0 + 1, foreground);
}

WhoosBoard WindowSurface::Convert(const Coord & c, Coord & board_c) const
{
	WhoosBoard w = Player;

	double x = (c.x - PlayerX0 - 1) / 2.0;
	int xi = static_cast<int>(x);
	double y = (c.y - PlayerY0 - 1) / 2.0;
	int yi = static_cast<int>(y);

	if (xi >= 0 && xi <= 9 && xi == x &&
		yi >= 0 && yi <= 9 && yi == y) {
		board_c.x = xi;
		board_c.y = yi;
		return w;
	}

	w = Computer;

	x = (c.x - ComputerX0 - 1) / 2.0;
	xi = static_cast<int>(x);
	y = (c.y - ComputerY0 - 1) / 2.0;
	yi = static_cast<int>(y);

	if (xi >= 0 && xi <= 9 && xi == x &&
		yi >= 0 && yi <= 9 && yi == y) {
		board_c.x = xi;
		board_c.y = yi;
		return w;
	}

	board_c = { -1, -1 };
	return w;
}


Coord WindowSurface::Peak(WhoosBoard w) const
{
	Coord board_c = { -1, -1 };
	InputDevice::EventInfo einfo;
	WhoosBoard w1 = Player;
	while (board_c.x == -1 || w != w1) {
		imput_device_.GetEvent(einfo);
		w1 = Convert({ einfo.coord.X,  einfo.coord.Y }, board_c);
	}
	std::string s = "                                                                      " + std::to_string(board_c.x) + " " + std::to_string(board_c.y);
	DrawString(s.c_str());
	return board_c;
}
