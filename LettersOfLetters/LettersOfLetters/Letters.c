#include "Letters.h"

const DWORD windowStyle = WS_OVERLAPPED & !WS_SIZEBOX | WS_SYSMENU;
const DWORD inputStyle = WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_CENTER;
const DWORD outputStyle = WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_CENTER | WS_BORDER | WS_SYSMENU;
const int maxInputStringLength = 128, width = 3, height = 5;

RECT clientRect = {0, 0, 1000, 200};
HWND hMainWindow, hInputForm, hOutputForm;
wchar_t *inputText, *outputText;

WNDCLASSEXW classEx = 
{
	sizeof(WNDCLASSEX), 0, (WNDPROC)WindowProc, 0, 0, 0, 0, 0, 0, 0, (LPCWSTR)CLASS_NAME, 0
};

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	HFONT hFont;
	int part;

	classEx.hInstance = hInstance;
	classEx.hCursor = LoadCursor(0, IDC_ARROW);
	RegisterClassExW(&classEx);

	hMainWindow = CreateWindowExW(0, CLASS_NAME, (LPCWSTR)CAPTION, windowStyle, 0, 0, clientRect.right, clientRect.bottom, 0, 0, 0, 0);
	ShowWindow(hMainWindow, SW_SHOWNORMAL);
	UpdateWindow(hMainWindow);

	GetClientRect(hMainWindow, &clientRect);
	part = clientRect.bottom / 4;
	hInputForm = CreateWindowExW(0, EDIT, NULL, inputStyle, 0, 0, clientRect.right, part, hMainWindow, 0, 0, 0);
	hOutputForm = CreateWindowExW(0, EDIT, NULL, outputStyle, 0, part, clientRect.right, clientRect.bottom - part, hMainWindow, 0, 0, 0);
	hFont = CreateFontW(0, 0, 0, 0, FW_NORMAL, 0, 0, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH | FF_MODERN, COURIER_NEW);
	SendMessageW(hOutputForm, WM_SETFONT, (WPARAM)hFont, TRUE);

	while (GetMessageW(&msg, 0, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	return 0;
}

LRESULT WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int i, j;
	int index;
	int sourceIndex;
	int tableIndex;
	int size;
	int length;
	wchar_t c;

	switch (uMsg) {
	case WM_DESTROY:
		ExitProcess(0);
	case WM_COMMAND:
		inputText = (wchar_t *)calloc(maxInputStringLength + 1, sizeof(wchar_t));
		if (length = GetWindowTextW(hInputForm, inputText, maxInputStringLength)) {
			size = (maxInputStringLength * ((width + 1) * 2 + 2) * height + 1) * sizeof(wchar_t);
			outputText = (wchar_t *)calloc(size, sizeof(wchar_t));
			index = 0;
			for (i = 0; i < height; i++) {
				for (sourceIndex = 0; sourceIndex < length; sourceIndex++) {
					c = inputText[sourceIndex];
					tableIndex;
					if (c >= 'A' && c <= 'Z') {
						tableIndex = c - 'A';
					} else if (c >= 'a' && c <= 'z') {
						tableIndex = c - 'a';
						c = tableIndex + 'A';
					} else {
						for (j = 0; j < (width + 1) * 2; j++) {
							outputText[index++] = SPACE;
						}
						continue;
					}
					for (j = 0; j < width; j++) {
						if (letters[tableIndex * width * height + j + i * width]) {
							outputText[index++] = c;
							outputText[index++] = c;
						} else {
							outputText[index++] = SPACE;
							outputText[index++] = SPACE;
						}
					}
					outputText[index++] = SPACE;
					outputText[index++] = SPACE;
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

	return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}