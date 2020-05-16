#include "pch.h"
#include "ScreenCapture.h"
#include "DllHandler.h"

namespace ScreenCapture {
	DllHandler* handler = nullptr;

	bool SCREENCAPTUREDLL_API ScreenCapture::InitHandler() {
		if (handler) {
			handler->Dispose();
			handler = nullptr;
		}
		handler = new DllHandler();
		return handler->Init();
	}

	bool SCREENCAPTUREDLL_API ScreenCapture::GetDesktopScreenshot(FInputFrameDescription& target) {
		if (target.FrameBuffer) target.Dispose();
		if (handler) return handler->WriteScreenShotToByteArray(target.FrameBuffer, target.FrameWidth, target.FrameHeight);
		return false;
	}

	bool SCREENCAPTUREDLL_API ScreenCapture::DisposeHandler() {
		if (handler) {
			handler->Dispose();
			delete handler;
			handler = nullptr;
		}
		return true;
	}
}
