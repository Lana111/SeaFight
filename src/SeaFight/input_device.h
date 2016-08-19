#ifndef INPUT_DEVICE_H_
#define INPUT_DEVICE_H_

class InputDevice
{
public:
	struct EventInfo
	{
		COORD coord;
		int key;
	};

	InputDevice();
	~InputDevice();

	void GetEvent(EventInfo &info) const;

private:
	HANDLE handle_;
	DWORD console_mode_;

};

#endif