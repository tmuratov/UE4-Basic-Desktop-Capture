#include "pch.h"
#include "DllHandler.h"

bool DllHandler::Init() {
	//Retrieve the handle to a display device context for the main display desktop. 
	hdcScreen = GetDC(NULL);
	//Get the client area for size calculation
	cx = GetSystemMetrics(SM_CXSCREEN);
	cy = GetSystemMetrics(SM_CYSCREEN);

	//Create a compatible DC which is used in a BitBlt from the window DC
	hdcMemDC = CreateCompatibleDC(hdcScreen);
	//Create a compatible bitmap from the Window DC
	hbmScreen = CreateCompatibleBitmap(hdcScreen, cx, cy);
	if (!hbmScreen || !hdcMemDC) {
		Dispose();
		return false;
	}
	//Select the compatible bitmap into the compatible memory DC.
	SelectObject(hdcMemDC, hbmScreen);

	return true;
}

bool DllHandler::WriteScreenShotToByteArray(unsigned char*& target, int& width, int& height) {

	if (!BitBlt(hdcMemDC, 0, 0, cx, cy, hdcScreen, 0, 0, SRCCOPY)) {
		Dispose();
		return false;
	}

	// Get the BITMAP from the HBITMAP
	GetObjectW(hbmScreen, sizeof(BITMAP), &bmpScreen);

	BITMAPINFOHEADER   bi;
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = cx;
	bi.biHeight = cy;
	bi.biPlanes = 1;
	bi.biBitCount = 32;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	DWORD dwBmpSize = (((bi.biWidth + 1) * bi.biBitCount - 1) / bi.biBitCount) * 4 * bi.biHeight;

	// Starting with 32-bit Windows, GlobalAlloc and LocalAlloc are implemented as wrapper functions that 
	// call HeapAlloc using a handle to the process's default heap. Therefore, GlobalAlloc and LocalAlloc 
	// have greater overhead than HeapAlloc.
	HANDLE hDIB = GlobalAlloc(GHND, dwBmpSize);
	unsigned char* lpbitmap = (unsigned char*)GlobalLock(hDIB);

	width = cx;
	height = cy;

	// Gets the "bits" from the bitmap and copies them into a buffer 
	// which is pointed to by lpbitmap.
	GetDIBits(hdcScreen, hbmScreen, 0,
		(UINT)bmpScreen.bmHeight,
		lpbitmap,
		(BITMAPINFO*)&bi, DIB_RGB_COLORS);

	if (target) delete[] target;
	target = new unsigned char[cx * cy * 4l];
	memcpy(target, lpbitmap, (cx * cy * 4l));

	//Unlock and Free the DIB from the heap
	GlobalUnlock(hDIB);
	GlobalFree(hDIB);

	return true;
}

void DllHandler::Dispose() {
	DeleteObject(hbmScreen);
	DeleteObject(hdcMemDC);
	ReleaseDC(NULL, hdcScreen);
}