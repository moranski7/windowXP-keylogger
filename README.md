# windowXP-keylogger
A simple windows keylogger. Creates a WH_KEYBOARD_LL hook which calls a callback function. The function get the virtual code for the key and records it in a file. This keylogger is able to record any keystroke regardless of window focus.

## Operating System used
Windows XP 

## IDE Used
Dev-C++ 5.11

## Compile
Be sure to compile with the following flag: 
* -std=c++0x
Command: g++.exe main.cpp -o main.exe -m32 -std=c++0x -static-libgcc -m32


## Reference
https://www.futurelearn.com/info/courses/computer-systems/0/steps/53503 (Keyboard overview)
https://learn.microsoft.com/en-us/windows/win32/inputdev/about-keyboard-input (Keyboard input system)
https://www.techtarget.com/searchenterprisedesktop/definition/device-driver (device Driver)
https://learn.microsoft.com/en-us/windows/win32/winmsg/about-hooks (hook chains)
https://learn.microsoft.com/en-ca/windows/win32/api/winuser/nf-winuser-setwindowshookexa?redirectedfrom=MSDN (SetWindowsHookExA)
https://learn.microsoft.com/en-us/previous-versions/windows/desktop/legacy/ms644985(v=vs.85) (LowLevelKeyboardProc callback function)
https://learn.microsoft.com/en-ca/windows/win32/api/winuser/ns-winuser-kbdllhookstruct?redirectedfrom=MSDN (KBDLLHOOKSTRUCT structure)
hjindal. https://stackoverflow.com/questions/12739278/difference-between-virtual-code-and-scan-code (What is the difference between scan code and virtual key code?)