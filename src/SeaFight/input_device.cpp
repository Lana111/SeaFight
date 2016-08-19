#include "stdafx.h"
#include "input_device.h"

InputDevice::InputDevice()
	: handle_(GetStdHandle(STD_INPUT_HANDLE))
{
	GetConsoleMode(handle_, &console_mode_);
	SetConsoleMode(handle_, ENABLE_MOUSE_INPUT);
}

InputDevice::~InputDevice()
{
	SetConsoleMode(handle_, console_mode_);
	handle_ = 0;
}

void InputDevice::GetEvent(EventInfo &info) const
{
	INPUT_RECORD all_events[2];
	DWORD read_event;

	while (1) {
		ReadConsoleInput(handle_, all_events, 2, &read_event);
		for (DWORD i = 0; i < read_event; i++)
		{
			if (all_events[i].EventType == MOUSE_EVENT)
			{
				info.coord.X = all_events[i].Event.MouseEvent.dwMousePosition.X;
				info.coord.Y = all_events[i].Event.MouseEvent.dwMousePosition.Y;
				if (all_events[i].Event.MouseEvent.dwButtonState == VK_LBUTTON) {
					info.key = VK_LBUTTON;
					return;
				}
			}
		}
	}
}