#pragma once

#include <Windows.h>

class DllHandler {
private:
	HDC hdcScreen = NULL;
	HDC hdcMemDC = NULL;
	HBITMAP hbmScreen = NULL;
	int cx = 0, cy = 0; //screen resolution

	HANDLE hDIB = NULL;
	BITMAP bmpScreen;

public:
	DllHandler() { };
	~DllHandler() { Dispose(); };

	bool Init();
	bool WriteScreenShotToByteArray(unsigned char*& target, int& width, int& height);
	void Dispose();
};