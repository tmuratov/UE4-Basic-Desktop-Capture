# UE4-Basic-Desktop-Capture

This is an UE 4.25 project with simple desktop capture plugin

Desktop capture is implemented as a third party DLL; 
The dll project solution is located inside Plugins/GdiDesktopCapture/ThirdParty/ScreenCaptureDll folder

This dll uses WinAPI functions to obtain desktop bitmap from screen device context.
For detailed WinAPI example see: https://docs.microsoft.com/en-us/windows/win32/gdi/capturing-an-image

Future plans: set up the multithreading and achieve WYSIWYG results
