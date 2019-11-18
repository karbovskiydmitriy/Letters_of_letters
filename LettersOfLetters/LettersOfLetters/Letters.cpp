#include "Letters.h"

const DWORD windowStyle = WS_OVERLAPPED & !WS_SIZEBOX | WS_SYSMENU;
const DWORD inputStyle = WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_CENTER;
const DWORD outputStyle = WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_CENTER | WS_BORDER | WS_SYSMENU;
const int maxInputStringLength = 128;
RECT clientRect = {0, 0, 800, 200};
HWND hMainWindow, hInputForm, hOutputForm;
const wchar_t space = L' ';
wchar_t *inputText, *outputText;
const wchar_t *const editClass = L"EDIT";
const wchar_t *const fontName = L"Courier New";
const wchar_t *const caption = L"Letters of letters";
const wchar_t *const mainClassName = L"MainClass";

WNDCLASSEXW WndClassEx = {sizeof(WNDCLASSEX), 0, (WNDPROC)WindowProc, 0, 0, 0, 0, 0, 0, 0, (LPCWSTR)mainClassName, 0};

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	WndClassEx.hInstance = hInstance;
	WndClassEx.hCursor = LoadCursor(0, IDC_ARROW);
	RegisterClassEx(&WndClassEx);
	hMainWindow = CreateWindowExW(0, mainClassName, (LPCWSTR)caption, windowStyle, 0, 0, clientRect.right, clientRect.bottom, 0, 0, hInstance, 0);
	ShowWindow(hMainWindow, SW_SHOWNORMAL);
	UpdateWindow(hMainWindow);
	GetClientRect(hMainWindow, &clientRect);
	int part = clientRect.bottom / 4;
	hInputForm = CreateWindowExW(0, editClass, NULL, inputStyle, 0, 0, clientRect.right, part, hMainWindow, 0, 0, 0);
	hOutputForm = CreateWindowExW(0, editClass, NULL, outputStyle, 0, part, clientRect.right, clientRect.bottom - part, hMainWindow, 0, 0, 0);
	SendMessage(hOutputForm, WM_SETFONT, (WPARAM)CreateFontW(0, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH | FF_MODERN, fontName), true);

	MSG msg;
	while (GetMessage(&msg, 0, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	ExitProcess(0);

}

LRESULT WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch (uMsg) {
	case WM_SIZE:
		GetClientRect(hWnd, &clientRect);
		return 0;
	case WM_DESTROY:
		ExitProcess(0);
	case WM_COMMAND:
		inputText = (wchar_t *)calloc(maxInputStringLength + 1, sizeof(wchar_t));
		if (int length = GetWindowTextW(hInputForm, inputText, maxInputStringLength)) {
			int size = maxInputStringLength * 3 * (5 + 1) * 2 + 5 * 2 + 1;
			outputText = (wchar_t *)calloc(size, sizeof(wchar_t));
			int index = 0;
			for (int i = 0; i < 5; i++) {
				for (int sourceIndex = 0; sourceIndex < length; sourceIndex++) {
					wchar_t c = inputText[sourceIndex];
					int tableIndex;
					if (c >= 'A' && c <= 'Z') {
						tableIndex = c - 'A';
					} else if (c >= 'a' && c <= 'z') {
						tableIndex = c - 'a';
						c = tableIndex + 'A';
					} else {
						for (int j = 0; j < 8; j++) {
							outputText[index++] = space;
						}
						continue;
					}
					for (int j = 0; j < 3; j++) {
						if (letters[tableIndex * 15 + j + i * 3]) {
							outputText[index++] = c;
							outputText[index++] = c;
						} else {
							outputText[index++] = space;
							outputText[index++] = space;
						}
					}
					outputText[index++] = space;
					outputText[index++] = space;
				}
				outputText[index++] = L'\r';
				outputText[index++] = L'\n';
			}
			SetWindowText(hOutputForm, outputText);
			free(outputText);
			outputText = (wchar_t *)NULL;
		}
		free(inputText);
		inputText = (wchar_t *)NULL;
		return 0;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
			PostQuitMessage(0);
			return 0;
		}
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);

}
