/*
 *  The MIT License
 *
 *  Copyright 2008 Andrew James <ephphatha@thelettereph.com>.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 */
/* Andrew James
** ITC226 Application Programming for Windows
** Date Created: 2008/10/25
** Last Modified: 2008/10/30
** dialog.cpp
** Function definitions for each dialog box procedure.
*/
#include "dialog.h"

INT_PTR CALLBACK DlgProcAddPlayer(HWND hwnd,
                                  UINT message,
                                  WPARAM wParam,
                                  LPARAM lParam)
{
    int red, green, blue;
    WCHAR* name;
    BOOL success = TRUE, last = TRUE;
    switch(message)
    {
        case WM_INITDIALOG:
            return 1;

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case IDC_BTN_ADD_PLAYER:
                    red = (int)GetDlgItemInt(hwnd,
                                             IDC_EDT_COLOUR_RED,
                                             &last,
                                             FALSE);
                    success = success & last;

                    green = (int)GetDlgItemInt(hwnd,
                                               IDC_EDT_COLOUR_GREEN,
                                               &last,
                                               FALSE);
                    success = success & last;

                    blue = (int)GetDlgItemInt(hwnd,
                                              IDC_EDT_COLOUR_BLUE,
                                              &last,
                                              FALSE);
                    success = success & last;

                    name = (WCHAR*)calloc(21, sizeof(WCHAR));
                    if(!GetDlgItemText(hwnd, IDC_EDT_NAME, name, 21))
                    {
                        MessageBox(hwnd,
                                   L"Please enter a name.",
                                   L"Value Error",
                                   MB_OK | MB_ICONWARNING);
                    }
                    else if(!success ||
                            (red > 255) || (red < 0) ||
                            (green > 255) || (green < 0) ||
                            (blue > 255) || (blue < 0))
                    {
                        MessageBox(hwnd,
                                   L"Colour values must be between 255 and 0",
                                   L"Value Error",
                                   MB_OK | MB_ICONWARNING);
                    }
                    else
                    {   //Passing the values back to the WndProc.
                        SendMessage(GetWindow(hwnd, GW_OWNER),
                                    ADDPLAYER,
                                    (WPARAM)name,
                                    (LPARAM)RGB(red, green, blue));
                        PostMessage(hwnd, WM_COMMAND, IDCANCEL, 0);
                    }
                    free(name);
                    break;

                case IDCANCEL:
                    PostMessage(GetWindow(hwnd, GW_OWNER), RESUMETIMER, 0, 0);
                    DestroyWindow(hwnd);
                    break;
            }
            return 1;

        default:
            return 0;
    }
}

INT_PTR CALLBACK DlgProcGameOptions(HWND hwnd,
                                    UINT message,
                                    WPARAM wParam,
                                    LPARAM lParam)
{
    int width, height;
    BOOL success = TRUE, last = TRUE;
    switch(message)
    {
        case WM_INITDIALOG:
            return 1;

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case IDC_BTN_CHANGE_DIMENSIONS:
                    width = (int)GetDlgItemInt(hwnd,
                                               IDC_EDT_WIDTH,
                                               &last,
                                               FALSE);
                    success = success & last;

                    height = (int)GetDlgItemInt(hwnd,
                                                IDC_EDT_HEIGHT,
                                                &last,
                                                FALSE);
                    success = success & last;

                    if(!success ||
                       (width < 2) ||
                       (height < 2))
                    {
                        MessageBox(hwnd,
                                   L"The grid must be at least 2 by 2 dots.",
                                   L"Value Error",
                                   MB_OK | MB_ICONWARNING);
						break;
                    }
                    else if((width > 50) ||
                            (height > 50))
                    {
                        MessageBox(hwnd,
                            L"Each grid dimension must be less than 50.",
                            L"You shall not pass!",
                            MB_OK | MB_ICONWARNING);
						break;
                    }
                    else if((width > 20) ||
                            (height > 20))
                    {
                        if(MessageBox(hwnd,
                            L"Do you really want to make the grid this large?",
                            L"Whoa!",
                            MB_YESNO | MB_ICONWARNING) == IDNO)
                        {
                            break;
                        }
                    }

                    SendMessage(GetWindow(hwnd, GW_OWNER),
                        EDITGAMESTATE,
                        (WPARAM)((width * 2) - 1),
                        (LPARAM)((height * 2) - 1));
                    PostMessage(hwnd, WM_COMMAND, IDCANCEL, 0);
                    break;

                case IDCANCEL:
                    PostMessage(GetWindow(hwnd, GW_OWNER), RESUMETIMER, 0, 0);
                    DestroyWindow(hwnd);
                    break;
            }
            return 1;

        default:
            return 0;
    }
}

INT_PTR CALLBACK DlgProcTimeLimit(HWND hwnd,
                                  UINT message,
                                  WPARAM wParam,
                                  LPARAM lParam)
{
    int timeLimit;
    BOOL success = TRUE;
    switch(message)
    {
        case WM_INITDIALOG:
            return 1;

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case IDC_BTN_SET_LIMIT:
                    timeLimit = (int)GetDlgItemInt(hwnd,
                                                    IDC_EDT_TIME_LIMIT,
                                                    &success,
                                                    FALSE);

                    if(!success)
                    {
                        MessageBox(hwnd,
                                   L"Please enter a time limit.",
                                   L"Value Error",
                                   MB_OK | MB_ICONWARNING);
                    }
                    else
                    {
                        SendMessage(GetWindow(hwnd, GW_OWNER),
                                    CHANGETIMELIMIT,
                                    (WPARAM)timeLimit,
                                    0);
                        PostMessage(hwnd, WM_COMMAND, IDCANCEL, 0);
                    }
                    break;

                case IDCANCEL:
                    PostMessage(GetWindow(hwnd, GW_OWNER), RESUMETIMER, 0, 0);
                    DestroyWindow(hwnd);
                    break;
            }
            return 1;

        default:
            return 0;
    }
}

BOOL MenuCommands(HWND hwnd,
                  HINSTANCE hInstance,
                  WPARAM wParam)
{   //TODO: Split this function sensibly.
    HWND hwndDialog = NULL;
    OPENFILENAME ofn;
    WCHAR* buffer;
    switch(LOWORD(wParam))
    {
        case IDM_NEW_GAME:
            PostMessage(hwnd, NEWGAME, 0, 0);
            return 0;

        case IDM_LOAD_GAME:
            buffer = (WCHAR*)calloc(MAX_PATH, sizeof(WCHAR));
            ZeroMemory(&ofn, sizeof(ofn));
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = hwnd;
            ofn.lpstrFile = buffer;
            ofn.lpstrFile[0] = '\0';
            ofn.nMaxFile = MAX_PATH * sizeof(WCHAR);
            ofn.lpstrFilter = L"Dots and Boxes Game (.dbg)\0*.DBG\0\0";
            ofn.nFilterIndex = 1;
            ofn.lpstrFileTitle = NULL;
            ofn.nMaxFileTitle = 0;
            ofn.lpstrInitialDir = NULL;
            ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

            if(GetOpenFileName(&ofn))
            {
                PostMessage(hwnd, LOADGAME, (WPARAM)ofn.lpstrFile, 0);
            }
            return 0;

        case IDM_SAVE_GAME:
            buffer = (WCHAR*)calloc(MAX_PATH, sizeof(WCHAR));
            ZeroMemory(&ofn, sizeof(ofn));
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = hwnd;
            ofn.lpstrFile = buffer;
            ofn.lpstrFile[0] = '\0';
            ofn.lpstrDefExt = L"dbg";
            ofn.nMaxFile = MAX_PATH * sizeof(WCHAR);
            ofn.lpstrFilter = L"Dots and Boxes Game (.dbg)\0*.DBG\0\0";
            ofn.nFilterIndex = 1;
            ofn.lpstrFileTitle = NULL;
            ofn.nMaxFileTitle = 0;
            ofn.lpstrInitialDir = NULL;
            ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;

            if(GetSaveFileName(&ofn))
            {
                PostMessage(hwnd, SAVEGAME, (WPARAM)ofn.lpstrFile, 0);
                PostMessage(hwnd, RESUMETIMER, 0, 0);
            }
            return 0;

        case IDM_ADD_PLAYER:
            if(!IsWindow(hwndDialog)) 
            {
                hwndDialog = CreateDialog(hInstance,
                                          MAKEINTRESOURCE(IDD_ADD_PLAYER),
                                          hwnd,
                                          (DLGPROC)DlgProcAddPlayer);
                ShowWindow(hwndDialog, SW_SHOW);
            }
            return 0;

        case IDM_LOAD_PLAYERS:
            buffer = (WCHAR*)calloc(MAX_PATH, sizeof(WCHAR));
            ZeroMemory(&ofn, sizeof(ofn));
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = hwnd;
            ofn.lpstrFile = buffer;
            ofn.lpstrFile[0] = '\0';
            ofn.nMaxFile = MAX_PATH * sizeof(WCHAR);
            ofn.lpstrFilter = L"Dots and Boxes Players (.dbp)\0*.DBP\0\0";
            ofn.nFilterIndex = 1;
            ofn.lpstrFileTitle = NULL;
            ofn.nMaxFileTitle = 0;
            ofn.lpstrInitialDir = NULL;
            ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

            if(GetOpenFileName(&ofn))
            {
                PostMessage(hwnd, LOADPLAYERS, (WPARAM)ofn.lpstrFile, 0);
            }
            return 0;

        case IDM_SAVE_PLAYERS:
            buffer = (WCHAR*)calloc(MAX_PATH, sizeof(WCHAR));
            ZeroMemory(&ofn, sizeof(ofn));
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = hwnd;
            ofn.lpstrFile = buffer;
            ofn.lpstrFile[0] = '\0';
            ofn.lpstrDefExt = L"dbp";
            ofn.nMaxFile = MAX_PATH * sizeof(WCHAR);
            ofn.lpstrFilter = L"Dots and Boxes Players (.dbp)\0*.DBP\0\0";
            ofn.nFilterIndex = 1;
            ofn.lpstrFileTitle = NULL;
            ofn.nMaxFileTitle = 0;
            ofn.lpstrInitialDir = NULL;
            ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;

            if(GetSaveFileName(&ofn))
            {
                PostMessage(hwnd, SAVEPLAYERS, (WPARAM)ofn.lpstrFile, 0);
                PostMessage(hwnd, RESUMETIMER, 0, 0);
            }
            return 0;

        case IDM_CHANGE_SIZE:
            if(!IsWindow(hwndDialog))
            {
                hwndDialog = CreateDialog(hInstance,
                                          MAKEINTRESOURCE(IDD_CHANGE_SIZE),
                                          hwnd,
                                          (DLGPROC)DlgProcGameOptions);
                ShowWindow(hwndDialog, SW_SHOW);
            }
            return 0;

        case IDM_TIME_LIMIT:
            if(!IsWindow(hwndDialog))
            {
                hwndDialog = CreateDialog(hInstance,
                                          MAKEINTRESOURCE(IDD_TIME_LIMIT),
                                          hwnd,
                                          (DLGPROC)DlgProcTimeLimit);
                ShowWindow(hwndDialog, SW_SHOW);
            }
            return 0;

        case IDM_EXIT:
            PostMessage(hwnd, QUITGAME, 0, 0);
            return 0;

        default:
            return 0;
    }
}
