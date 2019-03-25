#include <Windows.h>
#include <cstdio>

BYTE mapping[256] = {0};

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == HC_ACTION)
	{
		int eventf = 0;
		switch (wParam)
		{
		case WM_KEYUP:
		case WM_SYSKEYUP:
			eventf = KEYEVENTF_KEYUP;
			/* fall-through */
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
			if (p->dwExtraInfo != 1337 && mapping[p->vkCode] != 0) {
				keybd_event(mapping[p->vkCode], 0, eventf, 1337);
				return 1;
			}
			break;
		}
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{

	mapping[VK_PRIOR] = VK_HOME;
	mapping[VK_NEXT] = VK_END;
	mapping[VK_HOME] = VK_PRIOR;
	mapping[VK_END] = VK_NEXT;

	// Install the low-level keyboard & mouse hooks
	HHOOK hhkLowLevelKybd = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, 0, 0);

	// Keep this app running until we're told to stop
	MSG msg;
	while (!GetMessage(&msg, NULL, NULL, NULL)) {    //this while loop keeps the hook
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	UnhookWindowsHookEx(hhkLowLevelKybd);

	return(0);
}