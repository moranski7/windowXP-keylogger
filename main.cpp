#include <iostream>
#include <Windows.h>
#include <stdio.h>
using namespace std;

HHOOK hhk; // variable to store the HANDLE to the hook. Must be desclared globally

LRESULT __stdcall HookCallBack(int nCode, WPARAM wParam, LPARAM lParam) {
	KBDLLHOOKSTRUCT keyboardStruct;
	char buff[100];
	if (nCode >= 0){
		if (wParam == WM_KEYDOWN) {
			keyboardStruct = *((KBDLLHOOKSTRUCT*) lParam);
			MessageBox(NULL, "Key Press Detected!", "key pressed", MB_ICONINFORMATION);
			UnhookWindowsHookEx(hhk);
			PostQuitMessage (0);
		}
	}
	return CallNextHookEx(hhk, nCode, wParam, lParam);
}

int main() {
	MSG msg;
	BOOL bRet;
	hhk = SetWindowsHookEx(WH_KEYBOARD_LL, HookCallBack,  GetModuleHandle(NULL), 0);
	if (hhk != NULL) {
		cout << "Success!" << endl;
		 while(GetMessage(&msg, NULL, 0, 0) != 0) {
  			TranslateMessage( &msg );
  			DispatchMessage( &msg );
 		}
	}
	else {
		DWORD errorMessageID = ::GetLastError();
		LPSTR messageBuffer = nullptr;
		size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
		std::string message(messageBuffer, size);
		LocalFree(messageBuffer);
		cout << message << endl;
	}
	return 0;
}
