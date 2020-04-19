// compile with: /D_UNICODE /DUNICODE /DWIN32 /D_WINDOWS /c

//#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <iostream>
#include <WS2tcpip.h>

                // Include the Winsock library (lib) file
#pragma comment (lib, "ws2_32.lib")

using namespace std;

// Global variables

// The main window class name.
static TCHAR szWindowClass[] = _T("DesktopApp");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("CS180 Project");

HINSTANCE hInst;

// Identifiers (buttons, windows, etc)
#define ID_BUTTON_1 1 // Modify
#define ID_BUTTON_2 2 // Search most
#define ID_BUTTON_3 3 // Search least
#define ID_BUTTON_4 4 // time between
#define ID_BUTTON_5 5 // search active
#define ID_BUTTON_6 6 // pickup ratio
#define ID_BUTTON_7 7 // usage comparison
#define ID_BUTTON_8 8 // busiest location

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int CALLBACK WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nCmdShow
)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL,
            _T("Call to RegisterClassEx failed!"),
            _T("CS180 Project"),
            NULL);

        return 1;
    }

    // Store instance handle in our global variable
    hInst = hInstance;

    // The parameters to CreateWindow explained:
    // szWindowClass: the name of the application
    // szTitle: the text that appears in the title bar
    // WS_OVERLAPPEDWINDOW: the type of window to create
    // CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
    // 500, 100: initial size (width, length)
    // NULL: the parent of this window
    // NULL: this application does not have a menu bar
    // hInstance: the first parameter from WinMain
    // NULL: not used in this application
    HWND hWnd = CreateWindow(
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        500, 100,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    HWND hwndTitle = CreateWindow(
        L"STATIC",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Uber Data",      // Button text 
        WS_CHILD | WS_VISIBLE | WS_BORDER |SS_CENTER,  // Styles 
        850,         // x position(center)
        10,         // y position 
        300,        // Button width
        100,        // Button heighth
        hWnd,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );

    HWND hwndButton1 = CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Modify the Data",      // Button text 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_NOTIFY,  // Styles 
        250,         // x position 
        300,         // y position 
        180,        // Button width
        150,        // Button heighth
        hWnd,     // Parent window
        (HMENU) ID_BUTTON_1,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    HWND hwndButton2 = CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Search Most Used",      // Button text 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_NOTIFY,  // Styles 
        650,         // x position 
        300,         // y position 
        180,        // Button width
        150,        // Button heighth
        hWnd,     // Parent window
        (HMENU)ID_BUTTON_2,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    HWND hwndButton3 = CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Search Least Used",      // Button text 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_NOTIFY,  // Styles 
        1050,         // x position 
        300,         // y position 
        180,        // Button width
        150,        // Button heighth
        hWnd,     // Parent window
        (HMENU)ID_BUTTON_3,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    HWND hwndButton4 = CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Time Between Pickups",      // Button text 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_NOTIFY,  // Styles 
        1450,         // x position 
        300,         // y position 
        180,        // Button width
        150,        // Button heighth
        hWnd,     // Parent window
        (HMENU)ID_BUTTON_4,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    HWND hwndButton5 = CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Search Active Vehicles",      // Button text 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_NOTIFY,  // Styles 
        250,         // x position 
        600,         // y position 
        180,        // Button width
        150,        // Button heighth
        hWnd,     // Parent window
        (HMENU)ID_BUTTON_5,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    HWND hwndButton6 = CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Ratio for Pickups",      // Button text 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_NOTIFY,  // Styles 
        650,         // x position 
        600,         // y position 
        180,        // Button width
        150,        // Button heighth
        hWnd,     // Parent window
        (HMENU)ID_BUTTON_6,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    HWND hwndButton7 = CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Two App Comparison",      // Button text 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_NOTIFY,  // Styles 
        1050,         // x position 
        600,         // y position 
        180,        // Button width
        150,        // Button heighth
        hWnd,     // Parent window
        (HMENU)ID_BUTTON_7,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    HWND hwndButton8 = CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Busiest Location",      // Button text 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_NOTIFY,  // Styles 
        1450,         // x position 
        600,         // y position 
        180,        // Button width
        150,        // Button heighth
        hWnd,     // Parent window
        (HMENU)ID_BUTTON_8,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    if (!hWnd)
    {
        MessageBox(NULL,
            _T("Call to CreateWindow failed!"),
            _T("CS180 Project"),
            NULL);

        return 1;
    }

    // The parameters to ShowWindow explained:
    // hWnd: the value returned from CreateWindow
    // nCmdShow: the fourth parameter from WinMain
    ShowWindow(hWnd,
        nCmdShow);
    UpdateWindow(hWnd);

    // Main message loop:
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

void SendRequest();

//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    TCHAR greeting[] = _T("");

    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        // Here application is laid out.
        TextOut(hdc,
            5, 5,
            greeting, _tcslen(greeting));
        // End application-specific layout section.

        EndPaint(hWnd, &ps);
        break;
    case WM_COMMAND: // when an action happens
            switch(LOWORD(wParam))
            {
                case ID_BUTTON_1:
                    ::MessageBeep(MB_ICONERROR);
                    ::MessageBox(hWnd, TEXT("Modify the Data was clicked"), TEXT("CS180 Project"), MB_OK);

                    //SendRequest();

                    break;
                case ID_BUTTON_2:
                    ::MessageBeep(MB_ICONERROR);
                    ::MessageBox(hWnd, TEXT("Search Most Used"), TEXT("CS180 Project"), MB_OK);

                    //SendRequest();

                    break;
                case ID_BUTTON_3:
                    ::MessageBeep(MB_ICONERROR);
                    ::MessageBox(hWnd, TEXT("Search Least Used was clicked"), TEXT("CS180 Project"), MB_OK);

                    //SendRequest();

                    break;
                case ID_BUTTON_4:
                    ::MessageBeep(MB_ICONERROR);
                    ::MessageBox(hWnd, TEXT("Time Between Pickups was clicked"), TEXT("CS180 Project"), MB_OK);

                    //SendRequest();

                    break;
                case ID_BUTTON_5:
                    ::MessageBeep(MB_ICONERROR);
                    ::MessageBox(hWnd, TEXT("Search Active Vehicles was clicked"), TEXT("CS180 Project"), MB_OK);

                    //SendRequest();

                    break;
                case ID_BUTTON_6:
                    ::MessageBeep(MB_ICONERROR);
                    ::MessageBox(hWnd, TEXT("Ratio for Pickups was clicked"), TEXT("CS180 Project"), MB_OK);

                    //SendRequest();

                    break;
                case ID_BUTTON_7:
                    ::MessageBeep(MB_ICONERROR);
                    ::MessageBox(hWnd, TEXT("Two App Comparison was clicked"), TEXT("CS180 Project"), MB_OK);

                    //SendRequest();

                    break;
                case ID_BUTTON_8:
                    ::MessageBeep(MB_ICONERROR);
                    ::MessageBox(hWnd, TEXT("Busiest Location was clicked"), TEXT("CS180 Project"), MB_OK);

                    //SendRequest();

                    break;
            }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }

    return 0;
}

void SendRequest() // send request to server
{
    // INITIALIZE WINSOCK

                    // Structure to store the WinSock version. This is filled in
                    // on the call to WSAStartup()
    WSADATA data;

    // To start WinSock, the required version must be passed to
    // WSAStartup(). This server is going to use WinSock version
    // 2 so create a word that will store 2 and 2 in hex i.e.
    // 0x0202
    WORD version = MAKEWORD(2, 2);

    // Start WinSock
    int wsOk = WSAStartup(version, &data);
    if (wsOk != 0)
    {
        // Not ok! Get out quickly
        cout << "Can't start Winsock! " << wsOk;
        return;
    }

    // CONNECT TO THE SERVER

    // Create a hint structure for the server
    sockaddr_in server;
    server.sin_family = AF_INET; // AF_INET = IPv4 addresses
    server.sin_port = htons(54000); // Little to big endian conversion
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr); // Convert from string to byte array

    // Socket creation, note that the socket type is datagram
    SOCKET out = socket(AF_INET, SOCK_DGRAM, 0);

    // Write out to that socket
    string s("message_from_client");
    int sendOk = sendto(out, s.c_str(), s.size() + 1, 0, (sockaddr*)&server, sizeof(server));

    if (sendOk == SOCKET_ERROR)
    {
        cout << "That didn't work! " << WSAGetLastError() << endl;
    }

    // Close the socket
    closesocket(out);

    // Close down Winsock
    WSACleanup();
}