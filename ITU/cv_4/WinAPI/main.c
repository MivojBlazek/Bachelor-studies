#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <math.h>


// Global variable
HINSTANCE hInst;
UINT  MessageCount = 0;
UINT  Count = 0;
int posX = 0;
int posY = 0;
COLORREF carColor = RGB(200, 0, 0);

// Function prototypes.
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
void paintObject(HWND hWnd, HDC hDC, PAINTSTRUCT ps, int posX, int posY, POINT cursorPosition);
void paintPosition(HWND hWnd, HDC hDC, PAINTSTRUCT ps);

// Application entry point. This is the same as main() in standart C.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	BOOL bRet;
	WNDCLASS wcx;          // register class
	HWND hWnd;

	hInst = hInstance;     // Save the application-instance handle.
		// Fill in the window class structure with parameters that describe the main window.

	wcx.style = CS_HREDRAW | CS_VREDRAW;              // redraw if size changes
	wcx.lpfnWndProc = (WNDPROC)MainWndProc;          // points to window procedure
	wcx.cbClsExtra = 0;                               // no extra class memory
	wcx.cbWndExtra = 0;                               // no extra window memory
	wcx.hInstance = hInstance;                        // handle to instance
	wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);      // predefined app. icon
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);        // predefined arrow
	wcx.hbrBackground = GetStockObject(WHITE_BRUSH);  // white background brush
	wcx.lpszMenuName = (LPCSTR)"MainMenu";          // name of menu resource
	wcx.lpszClassName = (LPCSTR)"MainWClass";        // name of window class

	// Register the window class.

	if (!RegisterClass(&wcx)) return FALSE;

	// create window of registered class

	hWnd = CreateWindow(
		"MainWClass",        // name of window class
		"ITU",               // title-bar string
		WS_OVERLAPPEDWINDOW, // top-level window
		200,                  // default horizontal position
		25,                 // default vertical position
		1000,                // default width
		700,                 // default height
		(HWND)NULL,         // no owner window
		(HMENU)NULL,        // use class menu
		hInstance,           // handle to application instance
		(LPVOID)NULL);      // no window-creation data
	if (!hWnd) return FALSE;

	// Show the window and send a WM_PAINT message to the window procedure.
	// Record the current cursor position.

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// loop of message processing
	while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (bRet == -1)
		{
			// handle the error and possibly exit
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}


LRESULT CALLBACK MainWndProc(
	HWND hWnd,        // handle to window
	UINT uMsg,        // message identifier
	WPARAM wParam,    // first message parameter
	LPARAM lParam)    // second message parameter
{
	HDC         hDC;
	PAINTSTRUCT ps;
	POINT cursorPosition;

	// init cursor position 
	GetCursorPos(&cursorPosition);
	ScreenToClient(hWnd, &cursorPosition);

	switch (uMsg)
	{
	case WM_CREATE:
		break;

	// character input 
	case WM_CHAR:
		switch (wParam) {
		case 0x08:  // backspace
		case 0x0A:  // linefeed
		case 0x1B:  // escape
			break;

		case 0x09:  // tab
			break;

		default:
			break;
		}
		break;

	// key input
	case WM_KEYDOWN:
		switch (wParam) {
	        // update posX and posY in order to move object
		case VK_LEFT: // left arrow
			posX -= 5;
			break;
		case VK_RIGHT: // right arrow
			posX += 5;
			break;
		case VK_UP: // up arrow
			posY -= 5;
			break;
		case VK_DOWN: // down arrow
			posY += 5;
			break;

		// react on the other pressed keys 
		case VK_SPACE: // space
			break;
		case VK_BACK: // backspace
			break;
		case VK_TAB: // tab
			break;
	        // more virtual codes can be found here: https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	// get cursor position 
	case WM_MOUSEMOVE:
		break;

	// react on mouse clicks
	case WM_LBUTTONDOWN:
		if (carColor == RGB(0, 200, 0))
		{
			carColor = RGB(0, 0, 200);
		}
		else if (carColor == RGB(0, 0, 200))
		{
			carColor = RGB(200, 0, 0);
		}
		else
		{
			carColor = RGB(0, 200, 0);
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_LBUTTONUP:
		break;

	// paint objects
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		paintObject(hWnd, hDC, ps, posX, posY, cursorPosition);
		paintPosition(hWnd, hDC, ps);
		// paint other objects
		// paintObject2(hWnd, hDC, ps, posX, posY, cursorPosition);
		// paintObject3(hWnd, hDC, ps, posX, posY, cursorPosition);
		EndPaint(hWnd, &ps);
		DeleteDC(hDC);
		break;

		//
		// Process other messages.
		//
	
	case WM_CLOSE:
		DestroyWindow(hWnd);
		return 0;
		
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}


void paintObject(HWND hWnd, HDC hDC, PAINTSTRUCT ps, int posX, int posY, POINT cursorPosition)
{
	// Paint rectangles, ellipses, polygons, lines etc.

	posX += 50;
	posY += 150;

	// Wheels
	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
	SelectObject(hDC, hPen);
	SelectObject(hDC, hBrush);
	Ellipse(hDC, posX, posY, posX + 50, posY + 50);
	Ellipse(hDC, posX + 200, posY, posX + 200 + 50, posY + 50);
	DeleteObject(hPen);
	DeleteObject(hBrush);

	hPen = CreatePen(PS_SOLID, 1, RGB(128, 128, 128));
	hBrush = CreateSolidBrush(RGB(128, 128, 128));
	SelectObject(hDC, hPen);
	SelectObject(hDC, hBrush);
	Ellipse(hDC, posX + 10, posY + 10, posX + 10 + 30, posY + 10 + 30);
	Ellipse(hDC, posX + 200 + 10, posY + 10, posX + 200 + 10 + 30, posY + 10 + 30);
	DeleteObject(hPen);
	DeleteObject(hBrush);

	// Body
	hPen = CreatePen(PS_SOLID, 1, carColor);
	hBrush = CreateSolidBrush(carColor);
	SelectObject(hDC, hPen);
	SelectObject(hDC, hBrush);
	Rectangle(hDC, posX + 52, posY + 25, posX + 52 + 146, posY - 25); // Bottom
	Rectangle(hDC, posX + 40, posY - 80, posX + 40 + 146, posY - 76); // Roof

	int posXfender = posX;
	int posYfender = posY;
	POINT fender1[10];
	fender1[0].x = posXfender - 2; fender1[0].y = posYfender + 25;
	fender1[1].x = posXfender - 22; fender1[1].y = posYfender + 25;
	fender1[2].x = posXfender - 22; fender1[2].y = posYfender;
	fender1[3].x = posXfender - 12; fender1[3].y = posYfender - 25;
	fender1[4].x = posXfender + 52; fender1[4].y = posYfender - 25;
	fender1[5].x = posXfender + 52; fender1[5].y = posYfender + 15;
	fender1[6].x = posXfender + 45; fender1[6].y = posYfender + 5;
	fender1[7].x = posXfender + 25; fender1[7].y = posYfender - 5;
	fender1[8].x = posXfender + 5; fender1[8].y = posYfender + 5;
	fender1[9].x = posXfender - 2; fender1[9].y = posYfender + 15;

	posXfender = posX + 200;
	posYfender = posY;
	POINT fender2[11];
	fender2[0].x = posXfender - 2; fender2[0].y = posYfender + 25;
	fender2[1].x = posXfender - 2; fender2[1].y = posYfender - 25;
	fender2[2].x = posXfender + 52; fender2[2].y = posYfender - 25;
	fender2[3].x = posXfender + 72; fender2[3].y = posYfender;
	fender2[4].x = posXfender + 72; fender2[4].y = posYfender + 25;
	fender2[5].x = posXfender + 52; fender2[5].y = posYfender + 25;
	fender2[6].x = posXfender + 52; fender2[6].y = posYfender + 15;
	fender2[7].x = posXfender + 45; fender2[7].y = posYfender + 5;
	fender2[8].x = posXfender + 25; fender2[8].y = posYfender - 5;
	fender2[9].x = posXfender + 5; fender2[9].y = posYfender + 5;
	fender2[10].x = posXfender - 2; fender2[10].y = posYfender + 15;
	Polygon(hDC, fender1, 10); // Fender left
	Polygon(hDC, fender2, 11); // Fender right

	POINT pillarLeft[4];
	pillarLeft[0].x = posX; pillarLeft[0].y = posY - 25;
	pillarLeft[1].x = posX + 3; pillarLeft[1].y = posY - 25;
	pillarLeft[2].x = posX + 40 + 3; pillarLeft[2].y = posY - 80;
	pillarLeft[3].x = posX + 40; pillarLeft[3].y = posY - 80;

	POINT pillarRight[4];
	pillarRight[0].x = posX + 182; pillarRight[0].y = posY - 80;
	pillarRight[1].x = posX + 3 + 182; pillarRight[1].y = posY - 80;
	pillarRight[2].x = posX + 40 + 3 + 200; pillarRight[2].y = posY - 25;
	pillarRight[3].x = posX + 40 + 200; pillarRight[3].y = posY - 25;
	Polygon(hDC, pillarLeft, 4); // Left pillar
	Rectangle(hDC, posX + 52 + 60 - 2, posY - 25, posX + 52 + 60 + 2, posY - 80); // Middle pillar
	Polygon(hDC, pillarRight, 4); // Right pillar
	DeleteObject(hPen);
	DeleteObject(hBrush);

	// Windows
	hPen = CreatePen(PS_SOLID, 1, RGB(145, 222, 250));
	hBrush = CreateSolidBrush(RGB(145, 222, 250));
	SelectObject(hDC, hPen);
	SelectObject(hDC, hBrush);

	POINT leftWindow[4];
	leftWindow[0].x = posX + 3; leftWindow[0].y = posY - 25;
	leftWindow[1].x = posX + 52 + 60 - 2; leftWindow[1].y = posY - 25;
	leftWindow[2].x = posX + 52 + 60 - 2; leftWindow[2].y = posY - 76;
	leftWindow[3].x = posX + 40 + 3; leftWindow[3].y = posY - 76;

	POINT rightWindow[4];
	rightWindow[0].x = posX + 182; rightWindow[0].y = posY - 76;
	rightWindow[1].x = posX + 52 + 60 + 2; rightWindow[1].y = posY - 76;
	rightWindow[2].x = posX + 52 + 60 + 2; rightWindow[2].y = posY - 25;
	rightWindow[3].x = posX + 40 + 200; rightWindow[3].y = posY - 25;
	Polygon(hDC, leftWindow, 4); // Left window
	Polygon(hDC, rightWindow, 4); // Right window
	DeleteObject(hPen);
	DeleteObject(hBrush);

	return;
}

void paintPosition(HWND hWnd, HDC hDC, PAINTSTRUCT ps)
{
	char        text[40];          // buffer to store an output text
	HFONT       font;              // new large font
	HFONT       oldFont;           // saves the previous font

	font = CreateFont(25, 0, 0, 0, 0, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, 0);
	oldFont = (HFONT)SelectObject(hDC, font);
	sprintf(text, "Position -- x:%d, y:%d", posX, posY);
	TextOut(hDC, 50, 600, text, (int)strlen(text));
	SelectObject(hDC, oldFont);
	DeleteObject(font);
}
