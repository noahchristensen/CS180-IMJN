// compile with: /D_UNICODE /DUNICODE /DWIN32 /D_WINDOWS /c

//#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <iostream>
#include<fstream>
#include <sstream>
#include <string>
#include <vector>
#include <WS2tcpip.h>
#include <windows.h>
#include <stdio.h>
#include <shellscalingapi.h>
                // Include the Winsock library (lib) file
#pragma comment (lib, "ws2_32.lib")

using namespace std;

// Global variables
vector<vector<string>> results;

// The main window class name.
static TCHAR szWindowClass[] = _T("DesktopApp");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("CS180 Project");

HINSTANCE hInst;
HMENU hMenu;

// Identifiers (buttons, windows, etc)
#define ID_BUTTON_1 1 // Modify
#define ID_BUTTON_2 2 // Search most
#define ID_BUTTON_3 3 // Search least
#define ID_BUTTON_4 4 // time between
#define ID_BUTTON_5 5 // day pop
#define ID_BUTTON_6 6 // time pop
#define ID_BUTTON_7 7 // location pop
#define ID_BUTTON_8 8 // busiest location

// Search by Time
#define ID_TIMESEARCH_TIME 9 // enter time
#define ID_TIMESEARCH_DAY 10 // enter day
#define ID_TIMESEARCH_SORT 11 // enter sort(most, least)
#define ID_TIMESEARCH_BUTTON 12 // button to commence search
#define ID_TIMESEARCH_DATA 13 // where search data appears

// Search by Location
#define ID_LOCSEARCH_LAT 14 // enter time
#define ID_LOCSEARCH_LONG 15 // enter day
#define ID_LOCSEARCH_SORT 16 // enter sort(most, least)
#define ID_LOCSEARCH_BUTTON 17 // button to commence search
#define ID_LOCSEARCH_DATA 18 // where search data appears

// Modify Data (insert/delete)
// Add specific
#define ID_ADD_LAT 19 // enter time
#define ID_ADD_LONG 20 // enter time
#define ID_ADD_DATE 21 // enter time
#define ID_ADD_TIME 22 // enter time
#define ID_ADD_BASE 23 // enter time
#define ID_ADD_BUTTON 24 // enter time
// Delete specific
#define ID_DEL_LAT 25 // enter time
#define ID_DEL_LONG 26 // enter time
#define ID_DEL_TIME 27 // enter time
#define ID_DEL_DATE 28 // enter time
#define ID_DEL_BASE 29 // enter time
#define ID_DEL_BUTTON 30 // enter time
// Delete all by time
#define ID_DEL_TIME_TIME 31 // enter time
#define ID_DEL_TIME_DATE 32 // enter time
#define ID_DEL_TIME_BUTTON 33 // enter time
// Delete all by location
#define ID_DEL_LOC_LAT 34 // enter time
#define ID_DEL_LOC_LONG 35 // enter time
#define ID_DEL_LOC_BUTTON 36 // enter time

// Import/Export
#define ID_DATA_IMPORT 37
#define ID_DATA_EXPORT 38

// Popularity
#define ID_POP_DAY_BUTTON 39
#define ID_POP_TIME_SWITCH 40
#define ID_POP_TIME_BUTTON 41
#define ID_POP_LOC_SWITCH 42
#define ID_POP_LOC_BUTTON 43

// Interval
#define ID_POP_SWITCH 44
#define ID_POP_BUTTON 45
#define ID_SEARCH_LAT 46
#define ID_SEARCH_LONG 47
#define ID_POP_AVG 48
#define ID_SHORTEST_DATA 49
#define ID_LONGEST_DATA 50
#define ID_AVERAGE_DATA 51


// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// declarations
    void RegisterTimeSearch(HINSTANCE);
    LRESULT CALLBACK TimeSearchProcedure(HWND, UINT, WPARAM, LPARAM);
    void DisplayTimeSearch(HWND);

    void RegisterLocationSearch(HINSTANCE);
    LRESULT CALLBACK LocationSearchProcedure(HWND, UINT, WPARAM, LPARAM);
    void DisplayLocationSearch(HWND);

    void RegisterModifyData(HINSTANCE);
    LRESULT CALLBACK ModifyDataProcedure(HWND, UINT, WPARAM, LPARAM);
    void DisplayModifyData(HWND);

    void RegisterDayPopularity(HINSTANCE);
    LRESULT CALLBACK DayPopularityProcedure(HWND, UINT, WPARAM, LPARAM);
    void DisplayDayPopularity(HWND);

    void RegisterTimePopularity(HINSTANCE);
    LRESULT CALLBACK TimePopularityProcedure(HWND, UINT, WPARAM, LPARAM);
    void DisplayTimePopularity(HWND);

    void RegisterLocationPopularity(HINSTANCE);
    LRESULT CALLBACK LocationPopularityProcedure(HWND, UINT, WPARAM, LPARAM);
    void DisplayLocationPopularity(HWND);

    void RegisterTimeInterval(HINSTANCE);
    LRESULT CALLBACK TimeIntervalProcedure(HWND, UINT, WPARAM, LPARAM);
    void DisplayTimeInterval(HWND);

    // Server Communication
    string SendRequest(string);
    string ImportToServer(char* path);
    string ExportToClient();


int CALLBACK WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nCmdShow
)
{
    SetProcessDPIAware();


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

    RegisterTimeSearch(hInstance);
    RegisterLocationSearch(hInstance);
    RegisterModifyData(hInstance);
    RegisterDayPopularity(hInstance);
    RegisterTimePopularity(hInstance);
    RegisterLocationPopularity(hInstance);
    RegisterTimeInterval(hInstance);

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
        WS_VISIBLE | WS_CHILD,  // Styles 
        250,         // x position 
        300,         // y position 
        180,        // Button width
        150,        // Button heighth
        hWnd,     // Parent window
        (HMENU) ID_BUTTON_1,       
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    HWND hwndButton2 = CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Search by Time",      // Button text 
        WS_VISIBLE | WS_CHILD,  // Styles 
        650,         // x position 
        300,         // y position 
        180,        // Button width
        150,        // Button heighth
        hWnd,     // Parent window
        (HMENU)ID_BUTTON_2,       
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    HWND hwndButton3 = CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Search by Location",      // Button text 
        WS_VISIBLE | WS_CHILD,  // Styles 
        1050,         // x position 
        300,         // y position 
        180,        // Button width
        150,        // Button heighth
        hWnd,     // Parent window
        (HMENU)ID_BUTTON_3,       
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    HWND hwndButton4 = CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Uber Popularity by Day",      // Button text 
        WS_VISIBLE | WS_CHILD,  // Styles 
        1450,         // x position 
        300,         // y position 
        180,        // Button width
        150,        // Button heighth
        hWnd,     // Parent window
        (HMENU)ID_BUTTON_4,       
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    HWND hwndButton5 = CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Uber Popularity by Time",      // Button text 
        WS_VISIBLE | WS_CHILD,  // Styles 
        250,         // x position hwndButton
        600,         // y position 
        180,        // Button width
        150,        // Button heighth
        hWnd,     // Parent window
        (HMENU)ID_BUTTON_5,      
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    HWND hwndButton6 = CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Uber Popularity by Location",      // Button text 
        WS_VISIBLE | WS_CHILD,  // Styles 
        650,         // x position 
        600,         // y position 
        180,        // Button width
        150,        // Button heighth
        hWnd,     // Parent window
        (HMENU)ID_BUTTON_6,       
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    HWND hwndButton7 = CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Time Interval b/w Pickup",      // Button text 
        WS_VISIBLE | WS_CHILD,  // Styles 
        1050,         // x position 
        600,         // y position 
        180,        // Button width
        150,        // Button heighth
        hWnd,     // Parent window
        (HMENU)ID_BUTTON_7,      
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    HWND hwndButton8 = CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed //STATIC, Edit
        L"",      // Button text 
        WS_VISIBLE | WS_CHILD,  // Styles 
        1450,         // x position 
        600,         // y position 
        180,        // Button width
        150,        // Button heighth
        hWnd,     // Parent window
        (HMENU)ID_BUTTON_8,       
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

            // Here application is laid out.
            TextOut(hdc,
                5, 5,
                greeting, _tcslen(greeting));
            // End application-specific layout section.

            EndPaint(hWnd, &ps);

            break;
        case WM_CREATE:
        {
            hMenu = CreateMenu();
            HMENU hFileMenu = CreateMenu();

            string importS = "Import";
            wstring wideSIM = wstring(importS.begin(), importS.end());
            const wchar_t* wideCIM = wideSIM.c_str();
            string exportS = "Export";
            wstring wideSEM = wstring(exportS.begin(), exportS.end());
            const wchar_t* wideCEM = wideSEM.c_str();

            AppendMenu(hMenu, MF_STRING, ID_DATA_IMPORT, wideCIM);
            AppendMenu(hMenu, MF_STRING, ID_DATA_EXPORT, wideCEM);

            SetMenu(hWnd, hMenu);

            break;
        }
        case WM_COMMAND: // when an action happens
                switch(LOWORD(wParam))
                {
                    case ID_BUTTON_1:
                        //::MessageBeep(MB_ICONERROR);
                        //::MessageBox(hWnd, TEXT("Modify the Data not yet implemented"), TEXT("CS180 Project"), MB_OK);
                        DisplayModifyData(hWnd);

                        //SendRequest();

                        break;
                    case ID_BUTTON_2:
                        //::MessageBox(hWnd, TEXT("Search by Time was clicked"), TEXT("CS180 Project"), MB_OK);

                        DisplayTimeSearch(hWnd);
                        //SendRequest();

                        break;
                    case ID_BUTTON_3:
                        //::MessageBeep(MB_ICONERROR);
                        //::MessageBox(hWnd, TEXT("Search by Location not yet implemented"), TEXT("CS180 Project"), MB_OK);
                        DisplayLocationSearch(hWnd);

                        //SendRequest();

                        break;
                    case ID_BUTTON_4:
                        //::MessageBeep(MB_ICONERROR);
                        //::MessageBox(hWnd, TEXT("Time Between Pickups not yet implemented"), TEXT("CS180 Project"), MB_OK);

                        DisplayDayPopularity(hWnd);
                        //SendRequest();

                        break;
                    case ID_BUTTON_5:
                        //::MessageBeep(MB_ICONERROR);
                        //::MessageBox(hWnd, TEXT("Search Active Vehicles not yet implemented"), TEXT("CS180 Project"), MB_OK);

                        DisplayTimePopularity(hWnd);
                        //SendRequest();

                        break;
                    case ID_BUTTON_6:
                        //::MessageBeep(MB_ICONERROR);
                        //::MessageBox(hWnd, TEXT("Ratio for Pickups not yet implemented"), TEXT("CS180 Project"), MB_OK);

                        DisplayLocationPopularity(hWnd);
                        //SendRequest();

                        break;
                    case ID_BUTTON_7:
                        //::MessageBeep(MB_ICONERROR);
                        //::MessageBox(hWnd, TEXT("Two App Comparison not yet implemented"), TEXT("CS180 Project"), MB_OK);

                        DisplayTimeInterval(hWnd);
                        //SendRequest();

                        break;
                    case ID_BUTTON_8:
                        ::MessageBeep(MB_ICONERROR);
                        ::MessageBox(hWnd, TEXT("Busiest Location not yet implemented"), TEXT("CS180 Project"), MB_OK);

                        //SendRequest();

                        break;
                    case ID_DATA_IMPORT:
                    {

                        OPENFILENAME ofn;

                        char fileNameC[100];
                        size_t size = strlen(fileNameC) + 1;
                        wchar_t* fileName = new wchar_t[100];

                        ZeroMemory(&ofn, sizeof(OPENFILENAME));

                        ofn.lStructSize = sizeof(OPENFILENAME);
                        ofn.hwndOwner = hWnd;
                        ofn.lpstrFile = fileName;
                        ofn.lpstrFile[0] = '\0';
                        ofn.nMaxFile = 100;
                        //ofn.lpstrFilter = filter;
                        ofn.nFilterIndex = 1;

                        GetOpenFileName(&ofn);

                        MessageBox(NULL, ofn.lpstrFile, TEXT("CS180 Project - File Path"), MB_OK);

                        wcstombs(fileNameC, ofn.lpstrFile, 100);

                        string serverMessage = ImportToServer(fileNameC);
                        wstring wideSM = wstring(serverMessage.begin(), serverMessage.end());
                        const wchar_t* wideCSM = wideSM.c_str();
                        ::MessageBox(hWnd, wideCSM, TEXT("CS180 Project - Server Response"), MB_OK);

                        break;
                    }
                    case ID_DATA_EXPORT:
                        string serverMessage = ExportToClient();
                        wstring wideSM = wstring(serverMessage.begin(), serverMessage.end());
                        const wchar_t* wideCSM = wideSM.c_str();
                        ::MessageBox(hWnd, wideCSM, TEXT("CS180 Project - Server Response"), MB_OK);

                        break;
                }
                break;
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


//SEARCH BY TIME
void RegisterTimeSearch(HINSTANCE hInstance)
{
    WNDCLASSW search = { 0 };

    search.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    search.hCursor = LoadCursor(NULL, IDC_ARROW);
    search.style = CS_HREDRAW | CS_VREDRAW;
    search.hInstance = hInstance;
    search.lpszClassName = L"myTimeSearchClass";
    search.lpfnWndProc = TimeSearchProcedure;

    RegisterClassW(&search);
}

HWND hwndTimeField;
HWND hwndDayField;
HWND hwndSortField;
HWND hwndSearchData;

LRESULT CALLBACK TimeSearchProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lp)
{
    switch (msg)
    {
        case WM_CLOSE:
            DestroyWindow(hWnd);
            break;
        case WM_COMMAND: // when an action happens
            switch (LOWORD(wParam))
            {
            case ID_TIMESEARCH_BUTTON:
                wchar_t hourText[20];
                GetWindowText(hwndTimeField, hourText, 10);
                wchar_t dayText[20];
                GetWindowText(hwndDayField, dayText, 10);
                wchar_t sortText[20];
                GetWindowText(hwndSortField, sortText, 10);

                string strButton = "Now Loading";
                wstring wideSBM = wstring(strButton.begin(), strButton.end());
                const wchar_t* wideCSBM = wideSBM.c_str();
                ::MessageBox(hWnd, wideCSBM, TEXT("CS180 Project - Loading Data"), MB_OK);

                wstring wsHour(hourText);
                string strHour(wsHour.begin(), wsHour.end());
                wstring wsDay(dayText);
                string strDay(wsDay.begin(), wsDay.end());
                string strTime = "Time: ";
                strTime = strTime.append(strHour);
                strTime = strTime.append(",Date: ");
                strTime = strTime.append(strDay);
                strTime = strTime.append(",Latitude: ,Longitude: ,Base: ,Sort: ,Search");
                //"Time: 0:11,Date: 4/1/2014,Latitude: ,Longitude: ,Base: ,Sort: ,Search ";
                //SendRequest(strTime);
                //wstring wsSort(sortText);
                //string strSort(wsSort.begin(), wsSort.end());

                vector<string> miniVec;

                //Server Response
                string serverMessage = SendRequest(strTime);
                string searchComplete;
                if (serverMessage.compare("unable to connect to server") != 0)
                {
                    searchComplete = "Search Completed";
                }
                else
                {
                    searchComplete = "unable to connect to server";
                }

                wstring wideSSM = wstring(searchComplete.begin(), searchComplete.end());
                const wchar_t* wideCSSM = wideSSM.c_str();
                ::MessageBox(hWnd, wideCSSM, TEXT("CS180 Project - Server Response"), MB_OK);

                wstring wideSM = wstring(serverMessage.begin(), serverMessage.end());
                const wchar_t* wideCSM = wideSM.c_str();

                HWND hwndSearchData = CreateWindow(
                    L"EDIT",  // Predefined class; Unicode assumed //STATIC, Edit
                    L"",      // Button text 
                    WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_WANTRETURN | WS_VSCROLL,  // Styles 
                    50,         // x position 
                    200,         // y position 
                    350,        // Button width
                    800,        // Button heighth
                    hWnd,     // Parent window
                    (HMENU)ID_TIMESEARCH_DATA,
                    (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
                    NULL
                );

                SetWindowText(hwndSearchData, wideCSM);

                break;
            }
            break;
        default:
            return DefWindowProcW(hWnd, msg, wParam, lp);
    }
}

void DisplayTimeSearch(HWND hWnd)
{
    HWND hWndSearch = CreateWindowW(
        L"myTimeSearchClass",
        L"CS180 Project - Time Search",
        WS_VISIBLE | WS_OVERLAPPEDWINDOW,
        400, 400, 200, 200,
        hWnd,
        NULL,
        NULL,
        NULL
    );

    HWND hwndTimeLabel = CreateWindow(
        L"Static",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Time of Day (\"XX:XX\")",      // Button text 
        WS_VISIBLE | WS_CHILD | BS_CENTER,  // Styles 
        50,         // x position 
        50,         // y position 
        400,        // Button width
        50,        // Button heighth
        hWndSearch,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    hwndTimeField = CreateWindow(
        L"EDIT",  // Predefined class; Unicode assumed //STATIC, Edit
        L"",      // Button text 
        WS_VISIBLE | WS_CHILD | WS_BORDER,  // Styles 
        50,         // x position 
        100,         // y position 
        400,        // Button width
        50,        // Button heighth
        hWndSearch,     // Parent window
        (HMENU)ID_TIMESEARCH_TIME,       
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    HWND hwndDayLabel = CreateWindow(
        L"Static",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Day (XX/XX/XXXX)",      // Button text 
        WS_VISIBLE | WS_CHILD | BS_CENTER,  // Styles 
        600,         // x position 
        50,         // y position 
        400,        // Button width
        50,        // Button heighth
        hWndSearch,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    hwndDayField = CreateWindow(
        L"EDIT",  // Predefined class; Unicode assumed //STATIC, Edit
        L"",      // Button text 
        WS_VISIBLE | WS_CHILD | WS_BORDER,  // Styles 
        600,         // x position 
        100,         // y position 
        400,        // Button width
        50,        // Button heighth
        hWndSearch,     // Parent window
        (HMENU)ID_TIMESEARCH_DAY,       
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    HWND hwndSortLabel = CreateWindow(
        L"Static",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Sort By Usage (\"most/least\")",      // Button text 
        WS_VISIBLE | WS_CHILD | BS_CENTER,  // Styles 
        1150,         // x position 
        50,         // y position 
        400,        // Button width
        50,        // Button heighth
        hWndSearch,     // Parent window
        NULL,       
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    hwndSortField = CreateWindow(
        L"EDIT",  // Predefined class; Unicode assumed //STATIC, Edit
        L"",      // Button text 
        WS_VISIBLE | WS_CHILD | WS_BORDER,  // Styles 
        1150,         // x position 
        100,         // y position 
        400,        // Button width
        50,        // Button heighth
        hWndSearch,     // Parent window
        (HMENU)ID_TIMESEARCH_SORT,       
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    HWND hwndSearchButton = CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Search",      // Button text 
        WS_VISIBLE | WS_CHILD,  // Styles 
        1650,         // x position 
        50,         // y position 
        100,        // Button width
        100,        // Button heighth
        hWndSearch,     // Parent window
        (HMENU)ID_TIMESEARCH_BUTTON,       
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    /*HWND hwndSearchData = CreateWindow(
        L"EDIT",  // Predefined class; Unicode assumed //STATIC, Edit
        L"",      // Button text 
        WS_VISIBLE | WS_CHILD | WS_BORDER,  // Styles 
        50,         // x position 
        200,         // y position 
        1500,        // Button width
        800,        // Button heighth
        hWndSearch,     // Parent window
        (HMENU)ID_SEARCH_DATA,       
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.*/
}


// SEARCH BY LOCATION
void RegisterLocationSearch(HINSTANCE hInstance)
{
    WNDCLASSW search = { 0 };

    search.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    search.hCursor = LoadCursor(NULL, IDC_ARROW);
    search.style = CS_HREDRAW | CS_VREDRAW;
    search.hInstance = hInstance;
    search.lpszClassName = L"myLocationSearchClass";
    search.lpfnWndProc = LocationSearchProcedure;

    RegisterClassW(&search);
}

HWND hwndLatField;
HWND hwndLongField;
HWND hwndSortLocField;
HWND hwndSearchLocData;

LRESULT CALLBACK LocationSearchProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lp)
{
    switch (msg)
    {
    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;
    case WM_COMMAND: // when an action happens
        switch (LOWORD(wParam))
        {
        case ID_LOCSEARCH_BUTTON:
            wchar_t latText[20];
            GetWindowText(hwndLatField, latText, 10);
            wchar_t longText[20];
            GetWindowText(hwndLongField, longText, 10);
            wchar_t sortText[20];
            GetWindowText(hwndSortField, sortText, 10);

            string strButton = "Now Loading";
            wstring wideSBM = wstring(strButton.begin(), strButton.end());
            const wchar_t* wideCSBM = wideSBM.c_str();
            ::MessageBox(hWnd, wideCSBM, TEXT("CS180 Project - Loading Data"), MB_OK);

            wstring wsLat(latText);
            string strLat(wsLat.begin(), wsLat.end());
            wstring wsLong(longText);
            string strLong(wsLong.begin(), wsLong.end());
            string strLoc = "Time: ,Date: ";
            strLoc = strLoc.append(",Latitude: ");
            strLoc = strLoc.append(strLat);
            strLoc = strLoc.append(",Longitude: ");
            strLoc = strLoc.append(strLong);
            //"Time: 0:11,Date: 4/1/2014,Latitude: ,Longitude: ,Base: ,Sort: ,Search ";
            strLoc = strLoc.append(",Base: ,Search");
            //SendRequest(strTime);
            //wstring wsSort(sortText);
            //string strSort(wsSort.begin(), wsSort.end());

            vector<string> miniVec;

            //Server Response
            string serverMessage = SendRequest(strLoc);
            string searchComplete;
            if (serverMessage.compare("unable to connect to server") != 0)
            {
                searchComplete = "Search Completed";
            }
            else
            {
                searchComplete = "unable to connect to server";
            }

            wstring wideSSM = wstring(searchComplete.begin(), searchComplete.end());
            const wchar_t* wideCSSM = wideSSM.c_str();
            ::MessageBox(hWnd, wideCSSM, TEXT("CS180 Project - Server Response"), MB_OK);

            wstring wideSM = wstring(serverMessage.begin(), serverMessage.end());
            const wchar_t* wideCSM = wideSM.c_str();

            HWND hwndSearchData = CreateWindow(
                L"EDIT",  // Predefined class; Unicode assumed //STATIC, Edit
                L"",      // Button text 
                WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_WANTRETURN | WS_VSCROLL,  // Styles 
                50,         // x position 
                200,         // y position 
                400,        // Button width
                800,        // Button heighth
                hWnd,     // Parent window
                (HMENU)ID_LOCSEARCH_DATA,
                (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
                NULL
            );

            SetWindowText(hwndSearchData, wideCSM);

            break;
        }
        break;
    default:
        return DefWindowProcW(hWnd, msg, wParam, lp);
    }
}

void DisplayLocationSearch(HWND hWnd)
{
    HWND hWndSearch = CreateWindowW(
        L"myLocationSearchClass",
        L"CS180 Project - Location Search",
        WS_VISIBLE | WS_OVERLAPPEDWINDOW,
        400, 400, 200, 200,
        hWnd,
        NULL,
        NULL,
        NULL
    );

    HWND hwndLatLabel = CreateWindow(
        L"Static",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Latitude",      // Button text 
        WS_VISIBLE | WS_CHILD | BS_CENTER,  // Styles 
        50,         // x position 
        50,         // y position 
        400,        // Button width
        50,        // Button heighth
        hWndSearch,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    hwndLatField = CreateWindow(
        L"EDIT",  // Predefined class; Unicode assumed //STATIC, Edit
        L"",      // Button text 
        WS_VISIBLE | WS_CHILD | WS_BORDER,  // Styles 
        50,         // x position 
        100,         // y position 
        400,        // Button width
        50,        // Button heighth
        hWndSearch,     // Parent window
        (HMENU)ID_LOCSEARCH_LAT,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    HWND hwndLongLabel = CreateWindow(
        L"Static",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Longitude",      // Button text 
        WS_VISIBLE | WS_CHILD | BS_CENTER,  // Styles 
        600,         // x position 
        50,         // y position 
        400,        // Button width
        50,        // Button heighth
        hWndSearch,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    hwndLongField = CreateWindow(
        L"EDIT",  // Predefined class; Unicode assumed //STATIC, Edit
        L"",      // Button text 
        WS_VISIBLE | WS_CHILD | WS_BORDER,  // Styles 
        600,         // x position 
        100,         // y position 
        400,        // Button width
        50,        // Button heighth
        hWndSearch,     // Parent window
        (HMENU)ID_LOCSEARCH_LONG,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    HWND hwndSortLabel = CreateWindow(
        L"Static",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Sort By Usage (\"most/least\")",      // Button text 
        WS_VISIBLE | WS_CHILD | BS_CENTER,  // Styles 
        1150,         // x position 
        50,         // y position 
        400,        // Button width
        50,        // Button heighth
        hWndSearch,     // Parent window
        NULL,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    hwndSortField = CreateWindow(
        L"EDIT",  // Predefined class; Unicode assumed //STATIC, Edit
        L"",      // Button text 
        WS_VISIBLE | WS_CHILD | WS_BORDER,  // Styles 
        1150,         // x position 
        100,         // y position 
        400,        // Button width
        50,        // Button heighth
        hWndSearch,     // Parent window
        (HMENU)ID_LOCSEARCH_SORT,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    HWND hwndSearchButton = CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Search",      // Button text 
        WS_VISIBLE | WS_CHILD,  // Styles 
        1650,         // x position 
        50,         // y position 
        100,        // Button width
        100,        // Button heighth
        hWndSearch,     // Parent window
        (HMENU)ID_LOCSEARCH_BUTTON,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    /*HWND hwndSearchData = CreateWindow(
        L"EDIT",  // Predefined class; Unicode assumed //STATIC, Edit
        L"",      // Button text
        WS_VISIBLE | WS_CHILD | WS_BORDER,  // Styles
        50,         // x position
        200,         // y position
        1500,        // Button width
        800,        // Button heighth
        hWndSearch,     // Parent window
        (HMENU)ID_SEARCH_DATA,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.*/
}


// MODIFY DATA
void RegisterModifyData(HINSTANCE hInstance)
{
    WNDCLASSW modify = { 0 };

    modify.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    modify.hCursor = LoadCursor(NULL, IDC_ARROW);
    modify.style = CS_HREDRAW | CS_VREDRAW;
    modify.hInstance = hInstance;
    modify.lpszClassName = L"myModifyDataClass";
    modify.lpfnWndProc = ModifyDataProcedure;

    RegisterClassW(&modify);
}

HWND hwndAddDateField;
HWND hwndAddTimeField;
HWND hwndAddLatField;
HWND hwndAddLongField;
HWND hwndAddBaseField;
HWND hwndAddButton;

HWND hwndDelDateField;
HWND hwndDelTimeField;
HWND hwndDelLatField;
HWND hwndDelLongField;
HWND hwndDelBaseField;
HWND hwndDelButton;

HWND hwndDelTimeDateField;
HWND hwndDelTimeTimeField;
HWND hwndDelTimeButton;

HWND hwndDelLocLatField;
HWND hwndDelLocLongField;
HWND hwndDelLocButton;

LRESULT CALLBACK ModifyDataProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lp)
{
    switch (msg)
    {
    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;
    case WM_COMMAND: // when an action happens
        switch (LOWORD(wParam))
        {
            case ID_ADD_BUTTON:
            {
                wchar_t dateText[20];
                wchar_t timeText[20];
                wchar_t latText[20];
                wchar_t longText[20];
                wchar_t baseText[20];

                GetWindowText(hwndAddDateField, dateText, 10);
                GetWindowText(hwndAddTimeField, timeText, 10);
                GetWindowText(hwndAddLatField, latText, 10);
                GetWindowText(hwndAddLongField, longText, 10);
                GetWindowText(hwndAddBaseField, baseText, 10);

                wstring wsHour(timeText);
                string strHour(wsHour.begin(), wsHour.end());
                wstring wsDay(dateText);
                string strDay(wsDay.begin(), wsDay.end());
                wstring wsLat(latText);
                string strLat(wsLat.begin(), wsLat.end());
                wstring wsLong(longText);
                string strLong(wsLong.begin(), wsLong.end());
                wstring wsBase(baseText);
                string strBase(wsBase.begin(), wsBase.end());
                string strMod = "Time: ";
                strMod = strMod.append(strHour);
                strMod = strMod.append(",Date: ");
                strMod = strMod.append(strDay);
                strMod = strMod.append(",Latitude: ");
                strMod = strMod.append(strLat);
                strMod = strMod.append(",Longitude: ");
                strMod = strMod.append(strLong);
                strMod = strMod.append(",Base: ");
                strMod = strMod.append(strBase);
                strMod = strMod.append(" ,Insert");
                //"Time: 0:11,Date: 4/1/2014,Latitude: ,Longitude: ,Base: ,Sort: ,Search ";

                string serverMessage = SendRequest(strMod);
                if (serverMessage.compare("unable to connect to server") != 0)
                {
                    serverMessage = "Success! Data Added";
                }

                wstring wideSM = wstring(serverMessage.begin(), serverMessage.end());
                const wchar_t* wideCSM = wideSM.c_str();
                ::MessageBox(hWnd, wideCSM, TEXT("CS180 Project - Server Response"), MB_OK);
                break;
            }
            case ID_DEL_BUTTON:
            {
                wchar_t dateText[20];
                wchar_t timeText[20];
                wchar_t latText[20];
                wchar_t longText[20];
                wchar_t baseText[20];
                GetWindowText(hwndDelDateField, dateText, 10);
                GetWindowText(hwndDelTimeField, timeText, 10);
                GetWindowText(hwndDelLatField, latText, 10);
                GetWindowText(hwndDelLongField, longText, 10);
                GetWindowText(hwndDelBaseField, baseText, 10);
                wstring wsHour(timeText);
                string strHour(wsHour.begin(), wsHour.end());
                wstring wsDay(dateText);
                string strDay(wsDay.begin(), wsDay.end());
                wstring wsLat(latText);
                string strLat(wsLat.begin(), wsLat.end());
                wstring wsLong(longText);
                string strLong(wsLong.begin(), wsLong.end());
                wstring wsBase(baseText);
                string strBase(wsBase.begin(), wsBase.end());
                string strMod = "Time: ";
                strMod = strMod.append(strHour);
                strMod = strMod.append(",Date: ");
                strMod = strMod.append(strDay);
                strMod = strMod.append(",Latitude: ");
                strMod = strMod.append(strLat);
                strMod = strMod.append(",Longitude: ");
                strMod = strMod.append(strLong);
                strMod = strMod.append(",Base: ");
                strMod = strMod.append(strBase);
                //"Time: 0:11,Date: 4/1/2014,Latitude: ,Longitude: ,Base: ,Sort: ,Search ";
                strMod = strMod.append(" ,Delete");

                string serverMessage = SendRequest(strMod);
                if (serverMessage.compare("unable to connect to server") != 0)
                {
                    serverMessage = "Success! Data Deleted";
                }

                wstring wideSM = wstring(serverMessage.begin(), serverMessage.end());
                const wchar_t* wideCSM = wideSM.c_str();
                ::MessageBox(hWnd, wideCSM, TEXT("CS180 Project - Server Response"), MB_OK);
                break;
            }
            case ID_DEL_TIME_BUTTON:
            {
                wchar_t dateText[20];
                wchar_t timeText[20];
                GetWindowText(hwndDelTimeDateField, dateText, 10);
                GetWindowText(hwndDelTimeTimeField, timeText, 10);

                wstring wsHour(timeText);
                string strHour(wsHour.begin(), wsHour.end());
                wstring wsDay(dateText);
                string strDay(wsDay.begin(), wsDay.end());
                string strMod = "Time: ";
                strMod = strMod.append(strHour);
                strMod = strMod.append(",Date: ");
                strMod = strMod.append(strDay);
                strMod = strMod.append(",Latitude: ,Longitude: ,Base: ,Sort: ,Delete");
                //"Time: 0:11,Date: 4/1/2014,Latitude: ,Longitude: ,Base: ,Sort: ,Search ";

                string serverMessage = SendRequest(strMod);
                if (serverMessage.compare("unable to connect to server") != 0)
                {
                    serverMessage = "Success! Data Deleted";
                }

                wstring wideSM = wstring(serverMessage.begin(), serverMessage.end());
                const wchar_t* wideCSM = wideSM.c_str();
                ::MessageBox(hWnd, wideCSM, TEXT("CS180 Project - Server Response"), MB_OK);
                break;
            }
            case ID_DEL_LOC_BUTTON:
            {
                wchar_t latText[20];
                wchar_t longText[20];
                GetWindowText(hwndDelLocLatField, latText, 10);
                GetWindowText(hwndDelLocLongField, longText, 10);

                wstring wsLat(latText);
                string strLat(wsLat.begin(), wsLat.end());
                wstring wsLong(longText);
                string strLong(wsLong.begin(), wsLong.end());

                string strMod = "Time: ,Date: ";
                strMod = strMod.append(",Latitude: ");
                strMod = strMod.append(strLat);
                strMod = strMod.append(",Longitude: ");
                strMod = strMod.append(strLong);
                strMod = strMod.append(",Base: ,Sort: ,Delete");
                //"Time: 0:11,Date: 4/1/2014,Latitude: ,Longitude: ,Base: ,Sort: ,Search ";

                string serverMessage = SendRequest(strMod);
                if (serverMessage.compare("unable to connect to server") != 0)
                {
                    serverMessage = "Success! Data Deleted";
                }

                wstring wideSM = wstring(serverMessage.begin(), serverMessage.end());
                const wchar_t* wideCSM = wideSM.c_str();
                
                ::MessageBox(hWnd, wideCSM, TEXT("CS180 Project - Server Response"), MB_OK);
                break;
            }
        }
        break;
    default:
        return DefWindowProcW(hWnd, msg, wParam, lp);
    }
}

void DisplayModifyData(HWND hWnd)
{
    HWND hWndModify = CreateWindowW(
        L"myModifyDataClass",
        L"CS180 Project - Modify",
        WS_VISIBLE | WS_OVERLAPPEDWINDOW,
        400, 400, 200, 200,
        hWnd,
        NULL,
        NULL,
        NULL
    );

    HWND hwndAddLabel = CreateWindow(
        L"Static",  // Predefined class; Unicode assumed //STATIC, Edit
        L"ADD SPECIFIC DATA:",      // Button text 
        WS_VISIBLE | WS_CHILD | BS_CENTER,  // Styles 
        25,         // x position 
        50,         // y position 
        200,        // Button width
        40,        // Button heighth
        hWndModify,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    HWND hwndAddDateLabel = CreateWindow(
        L"Static",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Date",      // Button text 
        WS_VISIBLE | WS_CHILD | BS_CENTER,  // Styles 
        50,         // x position 
        100,         // y position 
        200,        // Button width
        50,        // Button heighth
        hWndModify,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    hwndAddDateField = CreateWindow(
        L"EDIT",  // Predefined class; Unicode assumed //STATIC, Edit
        L"",      // Button text 
        WS_VISIBLE | WS_CHILD | WS_BORDER,  // Styles 
        50,         // x position 
        150,         // y position 
        200,        // Button width
        50,        // Button heighth
        hWndModify,     // Parent window
        (HMENU)ID_ADD_DATE,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );

    HWND hwndAddTimeLabel = CreateWindow(
        L"Static",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Time ",      // Button text 
        WS_VISIBLE | WS_CHILD | BS_CENTER,  // Styles 
        350,         // x position 
        100,         // y position 
        200,        // Button width
        50,        // Button heighth
        hWndModify,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    hwndAddTimeField = CreateWindow(
        L"EDIT",  // Predefined class; Unicode assumed //STATIC, Edit
        L"",      // Button text 
        WS_VISIBLE | WS_CHILD | WS_BORDER,  // Styles 
        350,         // x position 
        150,         // y position 
        200,        // Button width
        50,        // Button heighth
        hWndModify,     // Parent window
        (HMENU)ID_ADD_TIME,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );

    HWND hwndAddLatLabel = CreateWindow(
        L"Static",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Latitude ",      // Button text 
        WS_VISIBLE | WS_CHILD | BS_CENTER,  // Styles 
        650,         // x position 
        100,         // y position 
        200,        // Button width
        50,        // Button heighth
        hWndModify,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    hwndAddLatField = CreateWindow(
        L"EDIT",  // Predefined class; Unicode assumed //STATIC, Edit
        L"",      // Button text 
        WS_VISIBLE | WS_CHILD | WS_BORDER,  // Styles 
        650,         // x position 
        150,         // y position 
        200,        // Button width
        50,        // Button heighth
        hWndModify,     // Parent window
        (HMENU)ID_ADD_LAT,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );

    HWND hwndAddLongLabel = CreateWindow(
        L"Static",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Longitude ",      // Button text 
        WS_VISIBLE | WS_CHILD | BS_CENTER,  // Styles 
        950,         // x position 
        100,         // y position 
        200,        // Button width
        50,        // Button heighth
        hWndModify,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    hwndAddLongField = CreateWindow(
        L"EDIT",  // Predefined class; Unicode assumed //STATIC, Edit
        L"",      // Button text 
        WS_VISIBLE | WS_CHILD | WS_BORDER,  // Styles 
        950,         // x position 
        150,         // y position 
        200,        // Button width
        50,        // Button heighth
        hWndModify,     // Parent window
        (HMENU)ID_ADD_LONG,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );

    HWND hwndAddBaseLabel = CreateWindow(
        L"Static",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Base # ",      // Button text 
        WS_VISIBLE | WS_CHILD | BS_CENTER,  // Styles 
        1250,         // x position 
        100,         // y position 
        200,        // Button width
        50,        // Button heighth
        hWndModify,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    hwndAddBaseField = CreateWindow(
        L"EDIT",  // Predefined class; Unicode assumed //STATIC, Edit
        L"",      // Button text 
        WS_VISIBLE | WS_CHILD | WS_BORDER,  // Styles 
        1250,         // x position 
        150,         // y position 
        200,        // Button width
        50,        // Button heighth
        hWndModify,     // Parent window
        (HMENU)ID_ADD_BASE,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );

    HWND hwndAddButton = CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed //STATIC, Edit
        L"ADD",      // Button text 
        WS_VISIBLE | WS_CHILD,  // Styles 
        1650,         // x position 
        150,         // y position 
        100,        // Button width
        50,        // Button heighth
        hWndModify,     // Parent window
        (HMENU)ID_ADD_BUTTON,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.



    // Delete
    HWND hwndDelLabel = CreateWindow(
        L"Static",  // Predefined class; Unicode assumed //STATIC, Edit
        L"DELETE SPECIFIC DATA:",      // Button text 
        WS_VISIBLE | WS_CHILD | BS_CENTER,  // Styles 
        25,         // x position 
        250,         // y position 
        200,        // Button width
        40,        // Button heighth
        hWndModify,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    HWND hwndDelDateLabel = CreateWindow(
        L"Static",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Date",      // Button text 
        WS_VISIBLE | WS_CHILD | BS_CENTER,  // Styles 
        50,         // x position 
        300,         // y position 
        200,        // Button width
        50,        // Button heighth
        hWndModify,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    hwndDelDateField = CreateWindow(
        L"EDIT",  // Predefined class; Unicode assumed //STATIC, Edit
        L"",      // Button text 
        WS_VISIBLE | WS_CHILD | WS_BORDER,  // Styles 
        50,         // x position 
        350,         // y position 
        200,        // Button width
        50,        // Button heighth
        hWndModify,     // Parent window
        (HMENU)ID_DEL_DATE,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );

    HWND hwndDelTimeLabel = CreateWindow(
        L"Static",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Time ",      // Button text 
        WS_VISIBLE | WS_CHILD | BS_CENTER,  // Styles 
        350,         // x position 
        300,         // y position 
        200,        // Button width
        50,        // Button heighth
        hWndModify,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    hwndDelTimeField = CreateWindow(
        L"EDIT",  // Predefined class; Unicode assumed //STATIC, Edit
        L"",      // Button text 
        WS_VISIBLE | WS_CHILD | WS_BORDER,  // Styles 
        350,         // x position 
        350,         // y position 
        200,        // Button width
        50,        // Button heighth
        hWndModify,     // Parent window
        (HMENU)ID_DEL_TIME,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );

    HWND hwndDelLatLabel = CreateWindow(
        L"Static",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Latitude ",      // Button text 
        WS_VISIBLE | WS_CHILD | BS_CENTER,  // Styles 
        650,         // x position 
        300,         // y position 
        200,        // Button width
        50,        // Button heighth
        hWndModify,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    hwndDelLatField = CreateWindow(
        L"EDIT",  // Predefined class; Unicode assumed //STATIC, Edit
        L"",      // Button text 
        WS_VISIBLE | WS_CHILD | WS_BORDER,  // Styles 
        650,         // x position 
        350,         // y position 
        200,        // Button width
        50,        // Button heighth
        hWndModify,     // Parent window
        (HMENU)ID_DEL_LAT,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );

    HWND hwndDelLongLabel = CreateWindow(
        L"Static",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Longitude ",      // Button text 
        WS_VISIBLE | WS_CHILD | BS_CENTER,  // Styles 
        950,         // x position 
        300,         // y position 
        200,        // Button width
        50,        // Button heighth
        hWndModify,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    hwndDelLongField = CreateWindow(
        L"EDIT",  // Predefined class; Unicode assumed //STATIC, Edit
        L"",      // Button text 
        WS_VISIBLE | WS_CHILD | WS_BORDER,  // Styles 
        950,         // x position 
        350,         // y position 
        200,        // Button width
        50,        // Button heighth
        hWndModify,     // Parent window
        (HMENU)ID_DEL_LONG,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );

    HWND hwndDelBaseLabel = CreateWindow(
        L"Static",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Base # ",      // Button text 
        WS_VISIBLE | WS_CHILD | BS_CENTER,  // Styles 
        1250,         // x position 
        300,         // y position 
        200,        // Button width
        50,        // Button heighth
        hWndModify,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    hwndDelBaseField = CreateWindow(
        L"EDIT",  // Predefined class; Unicode assumed //STATIC, Edit
        L"",      // Button text 
        WS_VISIBLE | WS_CHILD | WS_BORDER,  // Styles 
        1250,         // x position 
        350,         // y position 
        200,        // Button width
        50,        // Button heighth
        hWndModify,     // Parent window
        (HMENU)ID_DEL_BASE,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );

    HWND hwndDelButton = CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed //STATIC, Edit
        L"DELETE",      // Button text 
        WS_VISIBLE | WS_CHILD,  // Styles 
        1650,         // x position 
        350,         // y position 
        100,        // Button width
        50,        // Button heighth
        hWndModify,     // Parent window
        (HMENU)ID_DEL_BUTTON,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.




    // Delete Time
    HWND hwndDelTimeTLabel = CreateWindow(
        L"Static",  // Predefined class; Unicode assumed //STATIC, Edit
        L"DELTE DATA BY TIME:",      // Button text 
        WS_VISIBLE | WS_CHILD | BS_CENTER,  // Styles 
        25,         // x position 
        450,         // y position 
        200,        // Button width
        40,        // Button heighth
        hWndModify,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    HWND hwndDelTimeDateLabel = CreateWindow(
        L"Static",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Date",      // Button text 
        WS_VISIBLE | WS_CHILD | BS_CENTER,  // Styles 
        50,         // x position 
        500,         // y position 
        200,        // Button width
        50,        // Button heighth
        hWndModify,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    hwndDelTimeDateField = CreateWindow(
        L"EDIT",  // Predefined class; Unicode assumed //STATIC, Edit
        L"",      // Button text 
        WS_VISIBLE | WS_CHILD | WS_BORDER,  // Styles 
        50,         // x position 
        550,         // y position 
        200,        // Button width
        50,        // Button heighth
        hWndModify,     // Parent window
        (HMENU)ID_DEL_TIME_DATE,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );

    HWND hwndDelTimeTimeLabel = CreateWindow(
        L"Static",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Time ",      // Button text 
        WS_VISIBLE | WS_CHILD | BS_CENTER,  // Styles 
        350,         // x position 
        500,         // y position 
        200,        // Button width
        50,        // Button heighth
        hWndModify,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    hwndDelTimeTimeField = CreateWindow(
        L"EDIT",  // Predefined class; Unicode assumed //STATIC, Edit
        L"",      // Button text 
        WS_VISIBLE | WS_CHILD | WS_BORDER,  // Styles 
        350,         // x position 
        550,         // y position 
        200,        // Button width
        50,        // Button heighth
        hWndModify,     // Parent window
        (HMENU)ID_DEL_TIME_TIME,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );

    HWND hwndDelTimeButton = CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed //STATIC, Edit
        L"DELETE",      // Button text 
        WS_VISIBLE | WS_CHILD,  // Styles 
        650,         // x position 
        550,         // y position 
        100,        // Button width
        50,        // Button heighth
        hWndModify,     // Parent window
        (HMENU)ID_DEL_TIME_BUTTON,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.*/


    // DELETE LOCATION
    HWND hwndDelLocLabel = CreateWindow(
        L"Static",  // Predefined class; Unicode assumed //STATIC, Edit
        L"DELETE DATA BY LOCATION:",      // Button text 
        WS_VISIBLE | WS_CHILD | BS_CENTER,  // Styles 
        25,         // x position 
        650,         // y position 
        200,        // Button width
        40,        // Button heighth
        hWndModify,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    HWND hwndDelLocLatLabel = CreateWindow(
        L"Static",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Latitude",      // Button text
        WS_VISIBLE | WS_CHILD | BS_CENTER,  // Styles
        50,         // x position
        700,         // y position
        200,        // Button width
        50,        // Button heighth
        hWndModify,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    hwndDelLocLatField = CreateWindow(
        L"EDIT",  // Predefined class; Unicode assumed //STATIC, Edit
        L"",      // Button text
        WS_VISIBLE | WS_CHILD | WS_BORDER,  // Styles
        50,         // x position
        750,         // y position
        200,        // Button width
        50,        // Button heighth
        hWndModify,     // Parent window
        (HMENU)ID_DEL_LOC_LAT,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );

    HWND hwndDelLocLongLabel = CreateWindow(
        L"Static",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Longitude ",      // Button text
        WS_VISIBLE | WS_CHILD | BS_CENTER,  // Styles
        350,         // x position
        700,         // y position
        200,        // Button width
        50,        // Button heighth
        hWndModify,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    hwndDelLocLongField = CreateWindow(
        L"EDIT",  // Predefined class; Unicode assumed //STATIC, Edit
        L"",      // Button text
        WS_VISIBLE | WS_CHILD | WS_BORDER,  // Styles
        350,         // x position
        750,         // y position
        200,        // Button width
        50,        // Button heighth
        hWndModify,     // Parent window
        (HMENU)ID_DEL_LOC_LONG,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );

    HWND hwndDelLocButton = CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed //STATIC, Edit
        L"DELETE",      // Button text
        WS_VISIBLE | WS_CHILD,  // Styles
        650,         // x position
        750,         // y position
        100,        // Button width
        50,        // Button heighth
        hWndModify,     // Parent window
        (HMENU)ID_DEL_LOC_BUTTON,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.
}

void RegisterDayPopularity(HINSTANCE hInstance)
{
    WNDCLASSW popularity = { 0 };

    popularity.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    popularity.hCursor = LoadCursor(NULL, IDC_ARROW);
    popularity.style = CS_HREDRAW | CS_VREDRAW;
    popularity.hInstance = hInstance;
    popularity.lpszClassName = L"myDayPopClass";
    popularity.lpfnWndProc = DayPopularityProcedure;

    RegisterClassW(&popularity);
}

HWND hwndDayPopField;
HWND hwndDayUseField;

LRESULT CALLBACK DayPopularityProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lp)
{
    switch (msg)
    {
    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;
    case WM_COMMAND: // when an action happens
        switch (LOWORD(wParam))
        {
            case ID_POP_DAY_BUTTON:

                string strButton = "Press OK to load results";
                wstring wideSBM = wstring(strButton.begin(), strButton.end());
                const wchar_t* wideCSBM = wideSBM.c_str();
                ::MessageBox(hWnd, wideCSBM, TEXT("CS180 Project - Loading Data"), MB_OK);

                string serverMessage = SendRequest("Search: MostDay: ");

                string searchComplete;
                if (serverMessage.compare("unable to connect to server") != 0)
                {
                    searchComplete = "Search Completed";
                }
                else
                {
                    searchComplete = "unable to connect to server";
                }

                wstring wideSSM = wstring(searchComplete.begin(), searchComplete.end());
                const wchar_t* wideCSSM = wideSSM.c_str();
                ::MessageBox(hWnd, wideCSSM, TEXT("CS180 Project - Server Response"), MB_OK);

                wstring wideSM = wstring(serverMessage.begin(), serverMessage.end());
                const wchar_t* wideCSM = wideSM.c_str();

                SetWindowText(hwndDayPopField, wideCSM);
                SetWindowText(hwndDayUseField, wideCSM);
                break;
        }
        break;
    default:
        return DefWindowProcW(hWnd, msg, wParam, lp);
    }
}

void DisplayDayPopularity(HWND hWnd)
{
    HWND hWndPopularity = CreateWindowW(
        L"myDayPopClass",
        L"CS180 Project - Day Popularity",
        WS_VISIBLE | WS_OVERLAPPEDWINDOW,
        400, 400, 200, 200,
        hWnd,
        NULL,
        NULL,
        NULL
    );

    HWND hwndDaySortLabel = CreateWindow(
        L"Static",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Most Popular",      // Button text 
        WS_VISIBLE | WS_CHILD | SS_CENTER | BS_CENTER | WS_BORDER,  // Styles 
        100,         // x position 
        50,         // y position 
        150,        // Button width
        60,        // Button heighth
        hWndPopularity,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    HWND hwndDayCalcButton = CreateWindow(
        L"Button",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Calculate",      // Button text 
        WS_VISIBLE | WS_CHILD | BS_CENTER,  // Styles 
        350,         // x position 
        60,         // y position 
        100,        // Button width
        40,        // Button heighth
        hWndPopularity,     // Parent window
        (HMENU)ID_POP_DAY_BUTTON,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    HWND hwndDayLabel = CreateWindow(
        L"Static",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Day",      // Button text 
        WS_VISIBLE | WS_CHILD | SS_CENTER,  // Styles 
        80,         // x position 
        200,         // y position 
        100,        // Button width
        40,        // Button heighth
        hWndPopularity,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    hwndDayPopField = CreateWindow(
        L"EDIT",  // Predefined class; Unicode assumed //STATIC, Edit
        L"",      // Button text 
        WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_WANTRETURN,  // Styles 
        100,         // x position 
        250,         // y position 
        250,        // Button width
        400,        // Button heighth
        hWndPopularity,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    HWND hwndUseLabel = CreateWindow(
        L"Static",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Uses",      // Button text 
        WS_VISIBLE | WS_CHILD | SS_CENTER,  // Styles 
        200,         // x position 
        200,         // y position 
        200,        // Button width
        40,        // Button heighth
        hWndPopularity,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.
}

void RegisterTimePopularity(HINSTANCE hInstance)
{
    WNDCLASSW popularity = { 0 };

    popularity.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    popularity.hCursor = LoadCursor(NULL, IDC_ARROW);
    popularity.style = CS_HREDRAW | CS_VREDRAW;
    popularity.hInstance = hInstance;
    popularity.lpszClassName = L"myTimePopClass";
    popularity.lpfnWndProc = TimePopularityProcedure;

    RegisterClassW(&popularity);
}

HWND hwndTimeSortLabel;
HWND hwndTimePopField;
HWND hwndTimeUseField;
bool timeCalcMost = true;

LRESULT CALLBACK TimePopularityProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lp)
{
    switch (msg)
    {
    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;
    case WM_COMMAND: // when an action happens
        switch (LOWORD(wParam))
        {
            case ID_POP_TIME_SWITCH:
            {
                timeCalcMost = !timeCalcMost;
                string switchStr;
                if (timeCalcMost)
                {
                    switchStr = "Most Popular";
                }
                else
                {
                    switchStr = "Least Popular";
                }

                wstring wideSM = wstring(switchStr.begin(), switchStr.end());
                const wchar_t* wideCSM = wideSM.c_str();

                SetWindowText(hwndTimeSortLabel, wideCSM);

                break;
            }
            case ID_POP_TIME_BUTTON:
                string strButton = "Press OK to load results";
                wstring wideSBM = wstring(strButton.begin(), strButton.end());
                const wchar_t* wideCSBM = wideSBM.c_str();
                ::MessageBox(hWnd, wideCSBM, TEXT("CS180 Project - Loading Data"), MB_OK);

                string serverMessage;
                if (timeCalcMost)
                {
                    serverMessage = SendRequest("Search: Mosttime: ");
                }
                else
                {
                    serverMessage = SendRequest("Search: Leasttime: ");
                }


                string searchComplete;
                if (serverMessage.compare("unable to connect to server") != 0)
                {
                    searchComplete = "Search Completed";
                }
                else
                {
                    searchComplete = "unable to connect to server";
                }

                wstring wideSSM = wstring(searchComplete.begin(), searchComplete.end());
                const wchar_t* wideCSSM = wideSSM.c_str();
                ::MessageBox(hWnd, wideCSSM, TEXT("CS180 Project - Server Response"), MB_OK);

                wstring wideSM = wstring(serverMessage.begin(), serverMessage.end());
                const wchar_t* wideCSM = wideSM.c_str();

                SetWindowText(hwndTimePopField, wideCSM);
                SetWindowText(hwndTimeUseField, wideCSM);
                break;
        }
        break;
    default:
        return DefWindowProcW(hWnd, msg, wParam, lp);
    }
}

void DisplayTimePopularity(HWND hWnd)
{
    HWND hWndPopularity = CreateWindowW(
        L"myTimePopClass",
        L"CS180 Project - Time Popularity",
        WS_VISIBLE | WS_OVERLAPPEDWINDOW,
        400, 400, 200, 200,
        hWnd,
        NULL,
        NULL,
        NULL
    );

    hwndTimeSortLabel = CreateWindow(
        L"STATIC",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Most Popular",      // Button text 
        WS_VISIBLE | WS_CHILD | SS_CENTER | BS_CENTER | WS_BORDER,  // Styles 
        100,         // x position 
        50,         // y position 
        150,        // Button width
        60,        // Button heighth
        hWndPopularity,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    HWND hwndTimeSwitchButton = CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Switch",      // Button text 
        WS_VISIBLE | WS_CHILD | BS_CENTER,  // Styles 
        350,         // x position 
        60,         // y position 
        100,        // Button width
        40,        // Button heighth
        hWndPopularity,     // Parent window
        (HMENU) ID_POP_TIME_SWITCH,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    HWND hwndTimeCalcButton = CreateWindow(
        L"Button",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Calculate",      // Button text 
        WS_VISIBLE | WS_CHILD | BS_CENTER,  // Styles 
        550,         // x position 
        60,         // y position 
        100,        // Button width
        40,        // Button heighth
        hWndPopularity,     // Parent window
        (HMENU)ID_POP_TIME_BUTTON,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    HWND hwndTimeLabel = CreateWindow(
        L"Static",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Hour",      // Button text 
        WS_VISIBLE | WS_CHILD | SS_CENTER,  // Styles 
        80,         // x position 
        200,         // y position 
        100,        // Button width
        40,        // Button heighth
        hWndPopularity,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    hwndTimePopField = CreateWindow(
        L"EDIT",  // Predefined class; Unicode assumed //STATIC, Edit
        L"",      // Button text 
        WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_WANTRETURN,  // Styles 
        100,         // x position 
        250,         // y position 
        250,        // Button width
        400,        // Button heighth
        hWndPopularity,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    HWND hwndUseLabel = CreateWindow(
        L"Static",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Uses",      // Button text 
        WS_VISIBLE | WS_CHILD | SS_CENTER,  // Styles 
        200,         // x position 
        200,         // y position 
        200,        // Button width
        40,        // Button heighth
        hWndPopularity,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.
}

void RegisterLocationPopularity(HINSTANCE hInstance)
{
    WNDCLASSW popularity = { 0 };

    popularity.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    popularity.hCursor = LoadCursor(NULL, IDC_ARROW);
    popularity.style = CS_HREDRAW | CS_VREDRAW;
    popularity.hInstance = hInstance;
    popularity.lpszClassName = L"myLocPopClass";
    popularity.lpfnWndProc = LocationPopularityProcedure;

    RegisterClassW(&popularity);
}

HWND hwndLocSortLabel;
HWND hwndLocField;
HWND hwndLocUseField;
bool locCalcMost = true;

LRESULT CALLBACK LocationPopularityProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lp)
{
    switch (msg)
    {
    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;
    case WM_COMMAND: // when an action happens
        switch (LOWORD(wParam))
        {
            case ID_POP_LOC_SWITCH:
            {
                locCalcMost = !locCalcMost;
                string switchStr;
                if (locCalcMost)
                {
                    switchStr = "Most Popular";
                }
                else
                {
                    switchStr = "Least Popular";
                }

                wstring wideSM = wstring(switchStr.begin(), switchStr.end());
                const wchar_t* wideCSM = wideSM.c_str();

                SetWindowText(hwndLocSortLabel, wideCSM);

                break;
            }
            case ID_POP_LOC_BUTTON:

                string strButton = "Press OK to load results";
                wstring wideSBM = wstring(strButton.begin(), strButton.end());
                const wchar_t* wideCSBM = wideSBM.c_str();
                ::MessageBox(hWnd, wideCSBM, TEXT("CS180 Project - Loading Data"), MB_OK);

                string serverMessage;
                if (locCalcMost)
                {
                    serverMessage = SendRequest("Search: MostLoc: ");
                }
                else
                {
                    serverMessage = SendRequest("Search: LeastLoc: ");
                }

                string searchComplete;
                if (serverMessage.compare("unable to connect to server") != 0)
                {
                    searchComplete = "Search Completed";
                }
                else
                {
                    searchComplete = "unable to connect to server";
                }

                wstring wideSSM = wstring(searchComplete.begin(), searchComplete.end());
                const wchar_t* wideCSSM = wideSSM.c_str();
                ::MessageBox(hWnd, wideCSSM, TEXT("CS180 Project - Server Response"), MB_OK);

                wstring wideSM = wstring(serverMessage.begin(), serverMessage.end());
                const wchar_t* wideCSM = wideSM.c_str();

                SetWindowText(hwndLocField, wideCSM);
                SetWindowText(hwndLocUseField, wideCSM);
                break;
        }
        break;
    default:
        return DefWindowProcW(hWnd, msg, wParam, lp);
    }
}

void DisplayLocationPopularity(HWND hWnd)
{
    HWND hWndPopularity = CreateWindowW(
        L"myLocPopClass",
        L"CS180 Project - Location Popularity",
        WS_VISIBLE | WS_OVERLAPPEDWINDOW,
        400, 400, 200, 200,
        hWnd,
        NULL,
        NULL,
        NULL
    );

    hwndLocSortLabel = CreateWindow(
        L"Static",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Most Popular",      // Button text 
        WS_VISIBLE | WS_CHILD | SS_CENTER | BS_CENTER | WS_BORDER,  // Styles 
        100,         // x position 
        50,         // y position 
        150,        // Button width
        60,        // Button heighth
        hWndPopularity,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    HWND hwndLocSwitchButton = CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Switch",      // Button text 
        WS_VISIBLE | WS_CHILD | BS_CENTER,  // Styles 
        350,         // x position 
        60,         // y position 
        100,        // Button width
        40,        // Button heighth
        hWndPopularity,     // Parent window
        (HMENU) ID_POP_LOC_SWITCH,    
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    HWND hwndLocCalcButton = CreateWindow(
        L"Button",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Calculate",      // Button text 
        WS_VISIBLE | WS_CHILD | BS_CENTER,  // Styles 
        550,         // x position 
        60,         // y position 
        100,        // Button width
        40,        // Button heighth
        hWndPopularity,     // Parent window
        (HMENU)ID_POP_LOC_BUTTON,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    HWND hwndLocLabel = CreateWindow(
        L"Static",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Location",      // Button text 
        WS_VISIBLE | WS_CHILD | SS_CENTER,  // Styles 
        80,         // x position 
        200,         // y position 
        100,        // Button width
        40,        // Button heighth
        hWndPopularity,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    hwndLocField = CreateWindow(
        L"EDIT",  // Predefined class; Unicode assumed //STATIC, Edit
        L"",      // Button text 
        WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_WANTRETURN,  // Styles 
        100,         // x position 
        250,         // y position 
        250,        // Button width
        400,        // Button heighth
        hWndPopularity,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    HWND hwndUseLabel = CreateWindow(
        L"Static",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Uses",      // Button text 
        WS_VISIBLE | WS_CHILD | SS_CENTER,  // Styles 
        200,         // x position 
        200,         // y position 
        200,        // Button width
        40,        // Button heighth
        hWndPopularity,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.
}
//mingi
void RegisterTimeInterval(HINSTANCE hInstance)
{
    WNDCLASSW popularity = { 0 };

    popularity.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    popularity.hCursor = LoadCursor(NULL, IDC_ARROW);
    popularity.style = CS_HREDRAW | CS_VREDRAW;
    popularity.hInstance = hInstance;
    popularity.lpszClassName = L"myIntervalPopClass";
    popularity.lpfnWndProc = TimeIntervalProcedure;

    RegisterClassW(&popularity);
}

HWND hwndSortLabel;
HWND hwndField;
HWND hwndResultField1;
HWND hwndResultField2;
HWND hwndUseField;
  bool CheckSwitch = true;

LRESULT CALLBACK TimeIntervalProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lp)
{
    switch (msg)
    {
    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;
    case WM_COMMAND: // when an action happens
        switch (LOWORD(wParam))
        {
        case ID_POP_SWITCH:
        {
            CheckSwitch = !CheckSwitch;
            string switchStr;
            if (CheckSwitch)
            {
                switchStr = "Shortest";
            }
            else
            {
                switchStr = "Longest";
            }

            wstring wideSM = wstring(switchStr.begin(), switchStr.end());
            const wchar_t* wideCSM = wideSM.c_str();

            SetWindowText(hwndSortLabel, wideCSM);

            break;
        }
        case ID_POP_AVG:
        {
            wchar_t latText[20];
            GetWindowText(hwndLatField, latText, 10);
            wchar_t longText[20];
            GetWindowText(hwndLongField, longText, 10);
            string strButton = "Press OK to load results";
            wstring wideSBM = wstring(strButton.begin(), strButton.end());
            const wchar_t* wideCSBM = wideSBM.c_str();
            ::MessageBox(hWnd, wideCSBM, TEXT("CS180 Project - Loading Data"), MB_OK);

            wstring wsLat(latText);
            string strLat(wsLat.begin(), wsLat.end());
            wstring wsLong(longText);
            string strLong(wsLong.begin(), wsLong.end());
            string strLoc = "Time: ,Date: ";
            strLoc = strLoc.append(",Latitude: ");
            strLoc = strLoc.append(strLat);
            strLoc = strLoc.append(",Longitude: ");
            strLoc = strLoc.append(strLong);
            //"Time: ,Date: ,Latitude: ,Longitude: ,Base: ,Sort: ,Search ";
            strLoc = strLoc.append(",Base: ,Search: Average: ");

            string serverMessage = SendRequest(strLoc);


            string calculateComplete;
            if (serverMessage.compare("unable to connect to server") != 0)
            {
                calculateComplete = "Calculate Completed";
            }
            else
            {
                calculateComplete = "unable to connect to server";
            }

            wstring wideSSM = wstring(calculateComplete.begin(), calculateComplete.end());
            const wchar_t* wideCSSM = wideSSM.c_str();
            ::MessageBox(hWnd, wideCSSM, TEXT("CS180 Project - Server Response"), MB_OK);

            wstring wideSM = wstring(serverMessage.begin(), serverMessage.begin() + serverMessage.find_last_of("day") - 3);
            const wchar_t* wideCSM = wideSM.c_str();
            SetWindowText(hwndField, wideCSM);
            SetWindowText(hwndUseField, wideCSM);
            wideSM = wstring(serverMessage.begin() + serverMessage.find_last_of("day") - 3, serverMessage.end());
            wideCSM = wideSM.c_str();
            SetWindowText(hwndResultField2, wideCSM);
            
            break;
        }
        
        case ID_POP_BUTTON:
            wchar_t latText[20];
            GetWindowText(hwndLatField, latText, 10);
            wchar_t longText[20];
            GetWindowText(hwndLongField, longText, 10);
            string strButton = "Press OK to load results";
            wstring wideSBM = wstring(strButton.begin(), strButton.end());
            const wchar_t* wideCSBM = wideSBM.c_str();
            ::MessageBox(hWnd, wideCSBM, TEXT("CS180 Project - Loading Data"), MB_OK);

            wstring wsLat(latText);
            string strLat(wsLat.begin(), wsLat.end());
            wstring wsLong(longText);
            string strLong(wsLong.begin(), wsLong.end());
            string strLoc = "Time: ,Date: ";
            strLoc = strLoc.append(",Latitude: ");
            strLoc = strLoc.append(strLat);
            strLoc = strLoc.append(",Longitude: ");
            strLoc = strLoc.append(strLong);
            //"Time: ,Date: ,Latitude: ,Longitude: ,Base: ,Sort: ,Search ";
            strLoc = strLoc.append(",Base: ,Search");
            if (CheckSwitch)
            {
                strLoc = strLoc.append(": Shortest: ");
            }
            else
            {
                strLoc = strLoc.append(": Longest: ");
            }

            string serverMessage = SendRequest(strLoc);
            

            string searchComplete;
            if (serverMessage.compare("unable to connect to server") != 0)
            {
                searchComplete = "Search Completed";
            }
            else
            {
                searchComplete = "unable to connect to server";
            }

            wstring wideSSM = wstring(searchComplete.begin(), searchComplete.end());
            const wchar_t* wideCSSM = wideSSM.c_str();
            ::MessageBox(hWnd, wideCSSM, TEXT("CS180 Project - Server Response"), MB_OK);

            wstring wideSM = wstring(serverMessage.begin(), serverMessage.begin()+ serverMessage.find_last_of("day") - 3);
            const wchar_t* wideCSM = wideSM.c_str();

            SetWindowText(hwndField, wideCSM);
            SetWindowText(hwndUseField, wideCSM);
            wideSM = wstring(serverMessage.begin() + serverMessage.find_last_of("day") - 3, serverMessage.end());
            wideCSM = wideSM.c_str();
            SetWindowText(hwndResultField1, wideCSM);

            break;
        }
        break;
        
    default:
        return DefWindowProcW(hWnd, msg, wParam, lp);
    }
}

void DisplayTimeInterval(HWND hWnd)
{
    HWND hWndInterval = CreateWindowW(
        L"myIntervalPopClass",
        L"CS180 Project - Location Popularity",
        WS_VISIBLE | WS_OVERLAPPEDWINDOW,
        400, 400, 200, 200,
        hWnd,
        NULL,
        NULL,
        NULL
    );

    HWND hwndTitle = CreateWindow(
        L"STATIC",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Time Interval b/w Pickups",      // Button text 
        WS_CHILD | WS_VISIBLE | SS_CENTER | WS_BORDER | BS_CENTER,  // Styles 
        700,         // x position(center)
        10,         // y position 
        300,        // Button width
        70,        // Button heighth
        hWndInterval,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );

    HWND hwndLatLabel = CreateWindow(
        L"Static",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Enter Latitude",      // Button text 
        WS_VISIBLE | WS_CHILD | BS_CENTER,  // Styles 
        50,         // x position 
        110,         // y position 
        400,        // Button width
        50,        // Button heighth
        hWndInterval,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    hwndLatField = CreateWindow(
        L"EDIT",  // Predefined class; Unicode assumed //STATIC, Edit
        L"",      // Button text 
        WS_VISIBLE | WS_CHILD | WS_BORDER,  // Styles 
        50,         // x position 
        160,         // y position 
        400,        // Button width
        50,        // Button heighth
        hWndInterval,     // Parent window
        (HMENU)ID_SEARCH_LAT,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    HWND hwndLongLabel = CreateWindow(
        L"Static",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Enter Longitude",      // Button text 
        WS_VISIBLE | WS_CHILD | BS_CENTER,  // Styles 
        500,         // x position 
        110,         // y position 
        400,        // Button width
        50,        // Button heighth
        hWndInterval,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    hwndLongField = CreateWindow(
        L"EDIT",  // Predefined class; Unicode assumed //STATIC, Edit
        L"",      // Button text 
        WS_VISIBLE | WS_CHILD | WS_BORDER,  // Styles 
        500,         // x position 
        160,         // y position 
        400,        // Button width
        50,        // Button heighth
        hWndInterval,     // Parent window
        (HMENU)ID_SEARCH_LONG,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    hwndSortLabel = CreateWindow(
        L"Static",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Shortest",      // Button text 
        WS_VISIBLE | WS_CHILD | SS_CENTER | BS_CENTER | WS_BORDER,  // Styles 
        950,         // x position 
        110,         // y position 
        150,        // Button width
        50,        // Button heighth
        hWndInterval,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.
    
    HWND hwndSortSwitchButton = CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Switch",      // Button text 
        WS_VISIBLE | WS_CHILD | BS_CENTER,  // Styles 
        1150,         // x position 
        110,         // y position 
        100,        // Button width
        40,        // Button heighth
        hWndInterval,     // Parent window
        (HMENU)ID_POP_SWITCH,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    HWND hwndSortAvgButton = CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Average",      // Button text 
        WS_VISIBLE | WS_CHILD | BS_CENTER,  // Styles 
        1300,         // x position 
        110,         // y position 
        100,        // Button width
        40,        // Button heighth
        hWndInterval,     // Parent window
        (HMENU)ID_POP_AVG,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    HWND hwndSortCalButton = CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Calculate",      // Button text 
        WS_VISIBLE | WS_CHILD | BS_CENTER,  // Styles 
        1450,         // x position 
        110,         // y position 
        100,        // Button width
        40,        // Button heighth
        hWndInterval,     // Parent window
        (HMENU)ID_POP_BUTTON,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.
    hwndField = CreateWindow(
        L"EDIT",  // Predefined class; Unicode assumed //STATIC, Edit
        L"",      // Button text 
        WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_WANTRETURN,  // Styles 
        100,         // x position 
        250,         // y position 
        170,        // Button width
        600,        // Button heighth
        hWndInterval,     // Parent window
        (HMENU)ID_SHORTEST_DATA,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    HWND hwndResultLabel1 = CreateWindow(
        L"Static",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Shortest/Longest",      // Button text 
        WS_VISIBLE | WS_CHILD | BS_CENTER,  // Styles 
        400,         // x position 
        250,         // y position 
        200,        // Button width
        50,        // Button heighth
        hWndInterval,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    hwndResultField1 = CreateWindow(
        L"EDIT",  // Predefined class; Unicode assumed //STATIC, Edit
        L"",      // Button text 
        WS_VISIBLE | WS_CHILD | WS_BORDER,  // Styles 
        400,         // x position 
        350,         // y position 
        200,        // Button width
        50,        // Button heighth
        hWndInterval,     // Parent window
        (HMENU)ID_SHORTEST_DATA,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    HWND hwndResultLabel2 = CreateWindow(
        L"Static",  // Predefined class; Unicode assumed //STATIC, Edit
        L"Average",      // Button text 
        WS_VISIBLE | WS_CHILD | BS_CENTER,  // Styles 
        650,         // x position 
        250,         // y position 
        200,        // Button width
        50,        // Button heighth
        hWndInterval,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.

    hwndResultField2 = CreateWindow(
        L"EDIT",  // Predefined class; Unicode assumed //STATIC, Edit
        L"",      // Button text 
        WS_VISIBLE | WS_CHILD | WS_BORDER,  // Styles 
        650,         // x position 
        350,         // y position 
        200,        // Button width
        50,        // Button heighth
        hWndInterval,     // Parent window
        (HMENU)ID_AVERAGE_DATA,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL
    );      // Pointer not needed.
}

string SendRequest(string message) // send request to server
{
    string serverMessage;
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
        return "unable to connect to server";
    }

    // CONNECT TO THE SERVER

    // Create a hint structure for the server
    sockaddr_in server;
    server.sin_family = AF_INET; // AF_INET = IPv4 addresses
    server.sin_port = htons(54000); // Little to big endian conversion
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr); // Convert from string to byte array

    // Socket creation, note that the socket type is datagram
    SOCKET out = socket(AF_INET, SOCK_DGRAM, 0);

    int connResult = connect(out, (sockaddr*)&server, sizeof(server));
    if (out == INVALID_SOCKET)
    {
        WSACleanup();
        return "unable to connect to server";
    }
    // Write out to that socket
    //string s("message_from_client");
    int sendOk = sendto(out, message.c_str(), message.size() + 1, 0, (sockaddr*)&server, sizeof(server));
    char buf[10000]; // data sent by server

    if (sendOk == SOCKET_ERROR)
    {
        cout << "That didn't work! " << WSAGetLastError() << endl;
    }
    else
    {
        int serverLength = sizeof(server);
        ZeroMemory(&server, serverLength); // Clear the client structure
		ZeroMemory(buf, 10000); // Clear the receive buffer

		// Wait for message from server
		int bytesIn = recvfrom(out, buf, 10000, 0, (sockaddr*)&server, &serverLength);
		if (bytesIn == SOCKET_ERROR)
		{
			cout << "Error receiving from client " << WSAGetLastError() << endl;
            return "unable to connect to server";
		}
    }

    // Close the socket
    closesocket(out);

    // Close down Winsock
    WSACleanup();

    return buf;
}

string ImportToServer(char * path)
{
    string serverMessage;
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
        return "unable to connect to server";
    }

    // CONNECT TO THE SERVER

    // Create a hint structure for the server
    sockaddr_in server;
    server.sin_family = AF_INET; // AF_INET = IPv4 addresses
    server.sin_port = htons(54000); // Little to big endian conversion
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr); // Convert from string to byte array

    // Socket creation, note that the socket type is datagram
    SOCKET out = socket(AF_INET, SOCK_DGRAM, 0);

    int connResult = connect(out, (sockaddr*)&server, sizeof(server));
    if (out == INVALID_SOCKET)
    {
        WSACleanup();
        return "unable to connect to server";
    }
    // Write out to that socket
    //string s("message_from_client");

    /*if (in.is_open())
    {
        while (1)
        {
            in.read(Buffer, 350);
            if (in.eof())
            {
                cout << "End of File sending from Client" << endl;
                in.close();
                break;
            }
            else
            {
                send(out, Buffer, bufSize, 0);
                ZeroMemory(&Buffer, bufSize);
            }
        }
    }*/

    string key = "File: ";
    send(out, key.c_str(), key.size() + 1, 0);
    //int serverLength = sizeof(server);
    //ZeroMemory(&server, serverLength);

    //ifstream in(path, ios::binary);
    //ZeroMemory(&key, key.size() + 1);

    ifstream fin;
    string line1;
    string line2;

    char bufS[10000];
    // Open an existing file
    fin.open(path);
    int index = 0;
    cout << "we are about to open file to parse" << endl;
    while (!fin.eof()) {
        fin >> line1;   //These two are to format the first column of the csv file
        fin >> line2;
        line1 = line1 + " " + line2; // combines the date column string with the rest of its corresponding row "4/1/2014 + 0:11:00",40.769,-73.9549,"B02512"
            
        string message = line1;
        send(out, message.c_str(), message.size() + 1, 0);
        //ZeroMemory(&message, message.size());

        if (index % 10000 == 0) {
            cout << "just pushed " << index << endl;
        }

        index++;

        int serverLength = sizeof(server);
        ZeroMemory(&server, serverLength);
        int bytesIn = recvfrom(out, bufS, 10000, 0, (sockaddr*)&server, &serverLength);
        if (bytesIn == SOCKET_ERROR)
        {
            cout << "Error receiving from client " << WSAGetLastError() << endl;
            continue;
        }

    }

    string done = "Done";
    //ZeroMemory(&server, serverLength);
    int sendOk = sendto(out, done.c_str(), done.size() + 1, 0, (sockaddr*)&server, sizeof(server));

    fin.close();


    // Close the socket
    closesocket(out);

    // Close down Winsock
    WSACleanup();

    return serverMessage;
}

void exportData(char buf[1024], vector<string>& importRows) { // called for each message recieved for import
    string row(buf);
    importRows.push_back(row + "\n");
    return;
}

bool exportDone(char buf[1024]) {
    string clientMessage(buf);
    if (clientMessage.find("Done") != string::npos) {//checks whether message is for importing
        return true;
    }
    else {
        return false;
    }
}

void exportFunction(vector<string>& importRows) {
    string fileName;
    string compare = "Export ";
    int i = 0;

    std::ofstream file("exported-data.txt");
    //file.open(fileName, std::ios_base::app);
    for (i = 1; i < importRows.size(); i++) {
        file << importRows.at(i);
    }
    file.close();

}

string ExportToClient()
{
    string serverMessage;
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
        return "unable to connect to server";
    }

    // CONNECT TO THE SERVER

    // Create a hint structure for the server
    sockaddr_in server;
    int serverLength = sizeof(server);
    server.sin_family = AF_INET; // AF_INET = IPv4 addresses
    server.sin_port = htons(54000); // Little to big endian conversion
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr); // Convert from string to byte array

    // Socket creation, note that the socket type is datagram
    SOCKET out = socket(AF_INET, SOCK_DGRAM, 0);

    int connResult = connect(out, (sockaddr*)&server, sizeof(server));
    if (out == INVALID_SOCKET)
    {
        WSACleanup();
        return "unable to connect to server";
    }
    // Write out to that socket
    //string s("message_from_client");
    char buf[10000]; // data sent by client
    char bufS[10000]; // data to be sent by server

    vector<string> importVector;

    int index = 0;

    string key = "Export ";
    send(out, key.c_str(), key.size() + 1, 0);

    while (!exportDone(buf))
    {
        ZeroMemory(&server, serverLength); // Clear the client structure
        ZeroMemory(buf, 10000); // Clear the receive buffer
        ZeroMemory(bufS, 10000); // Clear the receive buffer

        // Wait for message
        int bytesIn = recvfrom(out, buf, 10000, 0, (sockaddr*)&server, &serverLength);
        if (bytesIn == SOCKET_ERROR)
        {
            cout << "Error receiving from client " << WSAGetLastError() << endl;
            continue;
        }

        // Display the message / who sent it
        //cout << "Message to file recv from " << clientIp << " : " << buf << endl;

        exportData(buf, importVector);

        int sendOk = sendto(out, bufS, 10000, 0, (sockaddr*)&server, sizeof(server));

        if (index % 10000 == 0) {
            cout << "just pushed " << index << endl;
        }

        index++;
    }

    cout << "file end" << endl;
    exportFunction(importVector);
    cout << "function complete" << endl;

    // Close the socket
    closesocket(out);

    // Close down Winsock
    WSACleanup();

    return buf;
}

