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

    HWND hwndButton = CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Send Request",      // Button text 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_NOTIFY,  // Styles 
        10,         // x position 
        10,         // y position 
        100,        // Button width
        100,        // Button heighth
        hWnd,     // Parent window
        NULL,       // No menu.
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

        // Here your application is laid out.
        // For this introduction, we just print out "Hello, Windows desktop!"
        // in the top left corner.
        TextOut(hdc,
            5, 5,
            greeting, _tcslen(greeting));
        // End application-specific layout section.

        EndPaint(hWnd, &ps);
        break;
    case WM_COMMAND: // when an action happens

        switch (LOWORD(wParam))
        {
            case BN_CLICKED:
                ::MessageBeep(MB_ICONERROR);
                ::MessageBox(hWnd, TEXT("Button was clicked"), TEXT("CS180 Project"), MB_OK);

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
                        break;
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