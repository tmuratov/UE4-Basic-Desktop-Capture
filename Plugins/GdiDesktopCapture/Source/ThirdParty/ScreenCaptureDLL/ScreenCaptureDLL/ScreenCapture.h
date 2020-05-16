#pragma once

#ifdef SCREENCAPTUREDLL_EXPORTS
#define SCREENCAPTUREDLL_API __declspec(dllexport)
#else
#define SCREENCAPTUREDLL_API __declspec(dllimport)
#endif

using namespace std;

namespace ScreenCapture {

	struct SCREENCAPTUREDLL_API FInputFrameDescription
	{
		unsigned char* FrameBuffer; //byte sequence
		int FrameWidth;
		int FrameHeight;

		void Dispose() {
			if (FrameBuffer != nullptr) {
				delete[] FrameBuffer;
				FrameBuffer = nullptr;
			}
		}
	};

	bool SCREENCAPTUREDLL_API InitHandler();

	bool SCREENCAPTUREDLL_API GetDesktopScreenshot(FInputFrameDescription& target);
	
	bool SCREENCAPTUREDLL_API DisposeHandler();
}
