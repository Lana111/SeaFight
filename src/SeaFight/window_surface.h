#ifndef WINDOW_SURFACE_H_
#define WINDOW_SURFACE_H_

#include "color.h"
#include "input_device.h"

#define WINSURF WindowSurface::instance()

class WindowSurface
{
public:
	~WindowSurface() = default;
	// Создание объекта
	static const WindowSurface &instance();

	void DrawString(const char *str) const;
	void DrawPlayerGameMap() const;
	void DrawComputerGameMap() const;

	void DrawCharOnPlayerGameMap(char ch, Coord board_c, Color foreground) const;
	void DrawCharOnComputerGameMap(char ch, Coord board_c, Color foreground) const;

	Coord Peak(WhoosBoard w) const;

private:
	WindowSurface();
	WindowSurface(const WindowSurface &);
	WindowSurface &operator=(const WindowSurface &);

	void GotoXY(int x, int y) const;
	void WriteChar(char ch, int x, int y) const;
	void WriteChar(char ch, int x, int y, Color foreground) const;
	void DrawGameMap(int x0, int y0) const;
	WhoosBoard Convert(const Coord &c, Coord &board_c) const;

	HANDLE handle_;
	InputDevice imput_device_;
};

#endif