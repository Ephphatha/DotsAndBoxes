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
** Date Created: Unknown
** Last Modified: 2008/10/30
** main.cpp
** Assignment 2 is a game. hooray! Dots and Boxes to be precise.
*******************************************************************************
** CHANGELOG:                                                                **
** DATE:      NOTES:                                                         **
** 2008/10/21 -Changelog created. I've reworked the paint structure so it is **
**             incredibly wasteful but easier to read. Mouse support should  **
**             be done today... I hope.                                      **
**            -Horrible, horrible, horrible logic when it comes to mouse     **
**             input. At least it works...                                   **
**            -Need to work on checking if two adjacent dots were selected.  **
**             Later.                                                        **
**            -The two lines following each occurence of BUG 001 need to be  **
**             tweaked. Currently don't equate to proper values when the user**
**             clicks south or west of a dot.                                **
** 2008/10/22 -Changed the logic for mouse detection. Should work now...     **
**             This fixes BUG 001.                                           **
**            -Fixed behaviour when player input is handled. Clicking on a   **
**             dot means the player has to select another dot to deselect it.**
**            -Line 192 through 215 needs to be fixed. Checking if a line    **
**             causes the completion of a box.                               **
** 2008/10/23 -Added icons and (soon) a menu. Nothing really added to the    **
**             functionality though.                                         **
**            -Now updates the gamestate when a box is enclosed. I need to   **
**             work on how I paint my lines to make it easier to see though. **
**            -BUG 002 Heap corruption when the grid is bigger than 5 by 5?  **
**             Fixed. Used gridwidth where I should've used gridheight and vv**
** 2008/10/24 -Grids of m x n dimension drawn correctly (thanks to dwaine).  **
**            -Player struct added.                                          **
**            -#defines now made into variables. Grid size should be         **
**             changable at runtime, more players added, etc.                **
**            -Function decomposition for paint logic done as of 4am...      **
**             I'm going to bed.                                             **
**            -Function decomposition for game logic started. Paint logic    **
**             updated. AddPlayer function created, should work correctly.   **
**            -Now detects draws and handles them correctly.                 **
**            -Dialog window for adding players created. Need to work on how **
**             I'm passing parameters from the DlgProc to the WndProc.       **
**            -BUG 003 Seriously need to work on the dialog box. Move it's   **
**             declaration out of the WndProc?                               **
** 2008/10/25 -Dialog boxes designed, implentation issues still...           **
**            -Now traps messages from dialog boxes correctly. I've got to   **
**             fix BUG 004 or enforce a hard limit on the number of players. **
**             No idea how long I've had that bug for...                     **
**            -Wee, fixed. Should've been taking the address of the Player*  **
**             variable when changing the memory allocated to it.            **
**            -Dialog procedures moved to dialog.cpp.                        **
**            -Inputs from dialogs now handled with DialogCommand()          **
**            -Function decomposition complete.                              **
** 2008/10/26 -Time limit added, game initialises with no players. Need to   **
**             change the sidebar function to paint on multiple lines. I'll  **
**             have to do that later because the landlady is being a BEEP.   **
**            -Time limit customisable through a dialog.                     **
**            -Sidebar paints on multiple lines, slight hack though.         **
**            -Added gender support (if you can call it that). Only evident  **
**             in VERY specific circumstances.                               **
** 2008/10/27 -Open and save dialogs added, saving files implemented, loading**
**             still to be done...                                           **
**            -Loading game files sort of works... maybe switch to binary    **
**             read mode and just copy memory?                               **
**            -Games saved and loaded from binary files.                     **
** 2008/10/28 -Tidied up the saving and loading functions, commented them.   **
**            -Commented the paint functions.                                **
**            -Commented the game functions. Should probably split them into **
**             two files for the players and gamestate.                      **
**            -Found and fixed BUG 008.                                      **
**            -Found and fixed BUG 009.                                      **
**            -More bugs found, need to be fixed. Save timelimit and         **
**             active player data.                                           **
** 2008/10/29 -Fixed BUG 010.                                                **
**            -Started splitting the MenuCommands function. To be continued. **
**            -Split the menu command function. Unfortunately this has added **
**             more cases to the WndProc() but I think it makes more sense   **
**             to have it this way.                                          **
**            -Rewritten the load function for the third time...             **
**            -Woo, saving a game should save the player info as well.       **
**            -BUG 012 is fixed, BUG 013 split from it.                      **
**            -Just wrote a function to update the player scores. Slightly   **
**             wasteful, but it lets me add to it later if I decide to start **
**             tracking moves as well.                                       **
**            -ARRRRRRRRRRRRRRRRRRRGH. Should not have changed the save/load **
**             functions to deal with the gamestate. Restoring from a backup **
**             later. :'(                                                    **
**            -Fixed BUG 013 and BUG 014.                                    **
** 2008/10/30 -Hopefully made the timer pause while a menu is open and resume**
**             when it closes. It works!                                     **
**            -Changed the save/load game functions to only save and load the**
**             gamestate.                                                    **
**            -Added AI players. Unfortunately there is a fair bit of        **
**             repeated code, and I can't really make it more efficient...   **
*******************************************************************************
** BUGLIST:                                                                  **
** ID:     DESCRIPTION:                                                      **
** BUG 001 -FIXED AS OF 2008/10/22                                           **
**         -Error detecting mouse input left or above the centerline.        **
** BUG 002 -FIXED AS OF 2008/10/22                                           **
**         -Heap corruption with uneven grid sizes.                          **
** BUG 003 -FIXED AS OF 2008/10/25                                           **
**         -Dialog boxes aren't passing messages properly.                   **
** BUG 004 -FIXED AS OF 2008/10/25                                           **
**         -Heap corruption when expanding the list of players.              **
** BUG 005 -FIXED AS OF 2008/10/25                                           **
**         -Mouse input causing access out of bounds when the user clicks    **
**          to the right or south of a dot within the threshold for line     **
**          detection. A temporary fix was applied immediately, but I want to**
**          do it better.                                                    **
** BUG 006 -FIXED AS OF 2008/10/27                                           **
**         -Clicking on a previously selected line causes the active player  **
**          to change.                                                       **
** BUG 007 -FIXED AS OF 2008/10/27                                           **
**         -Opening files does not change the gamestate correctly...         **
** BUG 008 -FIXED AS OF 2008/10/28                                           **
**         -The last player to move was incorrectly being credited for the   **
**          win on the sidebar as opposed to the player with the highest     **
**          score. The player struct was still being updated correctly though**
** BUG 009 -FIXED AS OF 2008/10/28                                           **
**         -Resizing the game to a larger size when the game was in a win    **
**          state causes some amusing errors...                              **
** BUG 010 -FIXED AS OF 2008/10/29                                           **
**         -Win by default not being displayed on the sidebar properly.      **
** BUG 011 -UNABLE TO REPRODUCE                                              **
**         -Score is erratically incremented in rare conditions. Hard to     **
**          reproduce, I don't think I'll be able to fix this...             **
** BUG 012 -FIXED AS OF 2008/10/29                                           **
**          However, games and playerlists are now tied, but the players can **
**          be saved seperately.                                             **
** BUG 013 -FIXED AS OF 2008/10/29                                           **
**         -Loading games in progress doesn't update player boxes count,     **
**          loading players with a game in progress does the same. Possibly  **
**          combine player and game data together?                           **
** BUG 014 -FIXED AS OF 2008/10/29                                           **
**         -Game hangs while exiting. Only when players are loaded?          **
*******************************************************************************
** Ctrl-F TODO in the code for things which need to be fixed.                **
** TODO: Serious bugtesting.                                                 **
** TODO: Input threshold and sidebar width dialogs.                          **
** TODO: Scrollable playerlist.                                              **
** TODO: Ability to manually remove players?                                 **
** TODO: Click and drag?                                                     **
** TODO: Add networking?                                                     **
** DONE: Add AI.                                                             **
** DONE: Time limit pauses while a menu is open, instead of stopping         **
** DONE: Split the MenuCommand() function.                                   **
** DONE: Limit grid size to 50.                                              **
** DONE: Add comments...                                                     **
** DONE: Saving and loading players in Binary mode.                          **
** DONE: Save, Load dialogs. still need to implement loading.                **
** DONE: Function decomposition. Nearly complete. Basically complete.        **
**       Seriously, not much else I can do to move things out of the WndProc **
*******************************************************************************
*/
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "ai.h"
#include "dialog.h"
#include "fileio.h"
#include "game.h"
#include "paint.h"
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE hInstance;
char szClassName[ ] = "WindowsApp";

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nFunsterStil)

{
    HWND hwnd;
    MSG messages;
    WNDCLASSEX wincl;

    hInstance = hThisInstance;
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WndProc;
    wincl.style = CS_DBLCLKS;
    wincl.cbSize = sizeof(WNDCLASSEX);

    wincl.hIcon = (HICON)LoadImage(hThisInstance,
                                   MAKEINTRESOURCE(IDI_DOTS_ICON),
                                   IMAGE_ICON,
                                   0,
                                   0,
                                   LR_SHARED);
    wincl.hIconSm = (HICON)LoadImage(hThisInstance,
                                     MAKEINTRESOURCE(IDI_DOTS_ICON),
                                     IMAGE_ICON,
                                     0,
                                     0,
                                     LR_SHARED);
    wincl.hCursor = (HCURSOR)LoadImage(NULL,
                                       IDC_ARROW,
                                       IMAGE_CURSOR,
                                       0,
                                       0,
                                       LR_SHARED);
    wincl.lpszMenuName = NULL;
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;
    wincl.hbrBackground = CreateSolidBrush(0x00FFFFFF);

    if (!RegisterClassEx (&wincl))
        return 0;

    hwnd = CreateWindowEx (
           0,
           szClassName,
           "Dots And Boxes",
           WS_OVERLAPPEDWINDOW,
           CW_USEDEFAULT,
           CW_USEDEFAULT,
           640,
           480,
           HWND_DESKTOP,
           LoadMenu(hThisInstance, MAKEINTRESOURCE(IDR_MENU_BAR)),
           hThisInstance,
           NULL
           );

    ShowWindow (hwnd, nFunsterStil);

    while(GetMessage(&messages, NULL, 0, 0))
    {
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }

    return (int)messages.wParam;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    //To deal with the gamestate
    static int** gameState = NULL;
    static int gridWidth = 9, gridHeight = 9;
    static time_t lastMoveTime = 0, lastMoveTimeAtPause = 0, timePaused = 0;
    static int gameWon = 0;

    //Information about players
    static Player* players = NULL;
    static int activePlayer = 1, totalPlayers = 0, maxPlayers = 4;

    //Information used for painting
    HDC hdc, hdcMem;
    HBITMAP bitmap;
    PAINTSTRUCT ps; //Unused except for BeginPaint()
    HBRUSH brush;
    #ifdef DEBUG
    UINT initialTextAlignment;
    #endif
    static int segmentSize = 0, gridPadding = 0;
    static RECT windowSize;

    //User customisable options
    static int inputThreshold = 10; //TODO: Make a dialog for these two.
    static int sideBarWidth = 160;
    static int timeLimit = 0;

    //Text buffers.
    static char* buffer = NULL;

    switch (message)
    {
        case WM_CREATE:
            SetTimer(hwnd, 1, 50, NULL);

            gameState = (int**)calloc(gridWidth, sizeof(int*));
            for(int i = 0; i < gridWidth; i++)
            {
                gameState[i] = (int*)calloc(gridHeight, sizeof(int));
            }

            players = (Player*)calloc(maxPlayers, sizeof(Player));

            CalculateDotPitch(hwnd, &windowSize, gridWidth, gridHeight,
                              sideBarWidth, &segmentSize, &gridPadding);
            return 0;

        case WM_SIZE:
            CalculateDotPitch(hwnd, &windowSize, gridWidth, gridHeight,
                              sideBarWidth, &segmentSize, &gridPadding);
            InvalidateRect(hwnd, NULL, TRUE);
            return 0;

        case ADDPLAYER:
            if(!AddPlayer(&players, &totalPlayers, &maxPlayers,
                          (char*)wParam, (COLORREF)lParam))
            {
                MessageBox(hwnd, "Couldn't add the player.", "Fail",
                           MB_OK | MB_ICONWARNING);
            }
            else if(GameInProgress(gameState, gridWidth, gridHeight))
            {
                UpdatePlayerScores(gameState, gridWidth, gridHeight,
                                   players, totalPlayers, &activePlayer);
            }
            return 0;

        case CHANGETIMELIMIT:
            timeLimit = (int)wParam;
            return 0;

        case EDITGAMESTATE: //TODO: Put this in a seperate function?
            if(GameInProgress(gameState, gridWidth, gridHeight))
            {
                if(((int)wParam < gridWidth) || ((int)lParam < gridHeight))
                {
                    if(MessageBox(hwnd,
                              "If you continue the game will be reset",
                              "A grid dimension is decreasing.",
                              MB_OKCANCEL | MB_ICONWARNING) == IDOK)
                    {
                        ClearGame(gameState, gridWidth, gridHeight, players,
                                  totalPlayers, &activePlayer, &gameWon,
                                  &lastMoveTime);
                    }
                    else
                    {
                        return 0;
                    }
                }
                else if(MessageBox(hwnd,
                              "Select no to play on with the current gamestate",
                              "Would you like to start a new game?",
                              MB_YESNO | MB_ICONWARNING) == IDYES)
                {
                    ClearGame(gameState, gridWidth, gridHeight, players,
                              totalPlayers, &activePlayer, &gameWon,
                              &lastMoveTime);
                }
            }

            if(ResizeGameState(&gameState, &gridWidth, &gridHeight,
                               (int)wParam, (int)lParam))
            {
                CalculateDotPitch(hwnd, &windowSize, gridWidth, gridHeight,
                                  sideBarWidth, &segmentSize, &gridPadding);
                if(gameWon > 0)
                {
                    players[HighestScore(players, totalPlayers)].score--;
                }
                gameWon = 0;
            }
            return 0;

        case LOADGAME:
            if(!LoadGame((char*)wParam, &gameState, &gridWidth, &gridHeight,
                         &activePlayer, &timeLimit))
            {
                MessageBox(hwnd, "Couldn't open the file.",
                           "Invalid file.",
                           MB_OK | MB_ICONWARNING);
            }
            else
            {
                if(totalPlayers)
                {
                    UpdatePlayerScores(gameState, gridWidth, gridHeight,
                                       players, totalPlayers, &activePlayer);
                }
            }
            free((char*)wParam);
            return 0;

        case SAVEGAME:
            if(!SaveGame((char*)wParam, gameState, gridWidth, gridHeight,
                         activePlayer, timeLimit))
            {
                MessageBox(hwnd, "Your game was not saved.",
                           "Error saving file.",
                           MB_OK | MB_ICONWARNING);
            }
            free((char*)wParam);
            return 0;

        case NEWGAME:
            if(GameInProgress(gameState, gridWidth, gridHeight) &&
               (MessageBox(hwnd,
                           "This will clear the game in progress.",
                           "Start a new game?",
                           MB_YESNO | MB_ICONWARNING) == IDYES))
            {
                ClearGame(gameState, gridWidth, gridHeight,
                        players, totalPlayers, &activePlayer,
                        &gameWon, &lastMoveTime);
            }
            return 0;

        case QUITGAME:
            if(!GameInProgress(gameState, gridWidth, gridHeight) ||
               (MessageBox(hwnd,
                          "Please remember to save your game and player data.",
                          "Are you sure you want to exit?",
                          MB_YESNO | MB_ICONWARNING) == IDYES))
            {
                PostMessage(hwnd, WM_DESTROY, 0, 0);
            }
            return 0;

        case LOADPLAYERS:
            if(!LoadPlayers((char*)wParam, &players,
                            &totalPlayers, &maxPlayers))
            {
                MessageBox(hwnd, "Maybe your player data is corrupt.",
                           "Couldn't load the player data",
                           MB_OK | MB_ICONWARNING);
            }
            else
            {
                if(GameInProgress(gameState, gridWidth, gridHeight))
                {
                    UpdatePlayerScores(gameState, gridWidth, gridHeight,
                                       players, totalPlayers, &activePlayer);
                }
            }
            free((char*)wParam);
            return 0;

        case SAVEPLAYERS:
            if(!SavePlayers((char*)wParam, players, totalPlayers))
            {
                MessageBox(hwnd, "Couldn't save the player data",
                           "Error saving player data.",
                           MB_OK | MB_ICONWARNING);
            }
            free((char*)wParam);
            return 0;

        case PAUSETIMER:
            if(lastMoveTime)
            {
                timePaused = time(0);
                lastMoveTimeAtPause = lastMoveTime;
                lastMoveTime = 0;
            }
            return 0;

        case RESUMETIMER:
            if(timePaused)
            {
                lastMoveTime = lastMoveTimeAtPause + (time(0) - timePaused);
                lastMoveTimeAtPause = 0;
                timePaused = 0;
            }
            return 0;

        case WM_COMMAND:
            PostMessage(hwnd, PAUSETIMER, 0, 0);
            return MenuCommands(hwnd, hInstance, wParam);

        case WM_LBUTTONDOWN:
            if(totalPlayers)
            {
                switch(MouseClick(lParam, gameState, gridWidth, gridHeight,
                                  segmentSize, gridPadding, activePlayer,
                                  players, inputThreshold))
                {
                    case -1:
                        break;

                    case 0:
                        players[activePlayer - 1].moves++;
                        NextPlayer(players, totalPlayers, &activePlayer);
                        lastMoveTime = time(0);
                        break;

                    default:
                        players[activePlayer - 1].moves++;
                        lastMoveTime = time(0);
                        break;
                }
            }

            if(CountBoxes(gameState, gridWidth, gridHeight) >=
               ((int)floor(gridWidth / 2.0) * (int)floor(gridHeight / 2.0)))
            {
                int highScorer = HighestScore(players, totalPlayers);
                lastMoveTime = 0;

                if(highScorer >= 0)
                {
                    players[highScorer].score++;
                    gameWon = 1;
                }
                else
                {
                    gameWon = -1;
                }
                PostMessage(hwnd, WM_COMMAND, IDM_NEW_GAME, 0);
            }
            return 0;

        case WM_LBUTTONUP:
            //TODO: Possibly let users click and drag.
            //Shouldn't be too hard to implement.
            return 0;

        case WM_KEYDOWN:
            switch(wParam)
            {
                case VK_ESCAPE:
                    PostMessage(hwnd, WM_COMMAND, IDM_EXIT, 0);
                    break;
            }
            return 0;

        case WM_CHAR:
            switch(wParam)
            {   //Just a little easter egg for people who don't have access
                //to the source.
                case 't':
                    MessageBox(NULL, "Boo Accounting.", "T says:", MB_OK);
                    break;
            }
            return 0;

        case WM_TIMER:
            if(timeLimit && (totalPlayers > 1) && lastMoveTime)
            {
                if((time(0) - lastMoveTime) > timeLimit)
                {
                    players[activePlayer - 1].isPlaying = FALSE;
					lastMoveTime = time(0);
                    if(NextPlayer(players, totalPlayers, &activePlayer) == 1)
                    {
                        lastMoveTime = 0;
                        players[activePlayer - 1].score++;
                        gameWon = 2;
                        PostMessage(hwnd, WM_COMMAND, IDM_NEW_GAME, 0);
                    }
                }
            }

            if((totalPlayers > 1) && (!gameWon) &&
               strstr(players[activePlayer - 1].name, "[AI]"))
            {
                POINT aiMove = PlayMove(gameState, gridWidth, gridHeight);
                switch(RegisterInput(gameState, gridWidth, gridHeight,
                                     aiMove.x, aiMove.y,
                                     players, activePlayer) > 0)
                {
                    case -1:
                        break;

                    case 0:
                        players[activePlayer - 1].moves++;
                        NextPlayer(players, totalPlayers, &activePlayer);
                        lastMoveTime = time(0);
                        break;

                    default:
                        players[activePlayer - 1].moves++;
                        lastMoveTime = time(0);
                        break;
                }

                if(CountBoxes(gameState, gridWidth, gridHeight) >=
                   ((int)floor(gridWidth / 2.0) *
                   (int)floor(gridHeight / 2.0)))
                {
                    int highScorer = HighestScore(players, totalPlayers);
                    lastMoveTime = 0;

                    if(highScorer >= 0)
                    {
                        players[highScorer].score++;
                        gameWon = 1;
                    }
                    else
                    {
                        gameWon = -1;
                    }
                    PostMessage(hwnd, WM_COMMAND, IDM_NEW_GAME, 0);
                }
            }
            InvalidateRect(hwnd, NULL, TRUE);
            return 0;

        case WM_PAINT:
            //This is slightly more complicated than the basic win32 app. I've
            //implemented paging so that everything is drawn to a DC in memory
            //then copied to the DC returned by BeginPaint().
            hdc = BeginPaint(hwnd, &ps);
            hdcMem = CreateCompatibleDC(hdc);
            bitmap = CreateCompatibleBitmap(hdc, windowSize.right,
                                            windowSize.bottom);
            SelectObject(hdcMem, bitmap);
            SetBkMode(hdcMem, TRANSPARENT);

            brush = CreateSolidBrush(0x00FFFFFF);
            FillRect(hdcMem, &windowSize, brush);
            DeleteObject(brush);

            DrawRectangles(hdcMem, gameState, gridWidth, gridHeight,
                           players, totalPlayers, segmentSize, gridPadding);

            DrawLines(hdcMem, gameState, gridWidth, gridHeight,
                      players, totalPlayers, segmentSize, gridPadding);

            DrawDots(hdcMem, gameState, gridWidth, gridHeight,
                     players, totalPlayers, segmentSize, gridPadding);

            DrawSidebar(hdcMem, players, totalPlayers, activePlayer - 1,
                        sideBarWidth, windowSize.right - sideBarWidth,
                        timeLimit, lastMoveTime, gameWon);

            #ifdef DEBUG
            //Just a bit of debug information.
            buffer = (char*)calloc(((int)floor(gridWidth / 2.0) + 1) * 4,
                                    sizeof(char));

            initialTextAlignment = SetTextAlign(hdcMem, TA_RIGHT);
            for(int i = 0; i < gridHeight; i++)
            {
                sprintf(buffer, "%d", gameState[0][i]);
                for(int j = 1; j < gridWidth; j++)
                {
                    sprintf(buffer, "%s %d", buffer, gameState[j][i]);
                }

                TextOut(hdcMem,
                        windowSize.right,
                        windowSize.bottom - ((gridHeight - i) * 20),
                        buffer,
                        (int)strlen(buffer));
            }
            SetTextAlign(hdcMem, initialTextAlignment);
            free(buffer);
            buffer = NULL;

            buffer = (char*)calloc(100, sizeof(char));
            sprintf(buffer, "Boxes filled: %d",
                    CountBoxes(gameState, gridWidth, gridHeight));
            TextOut(hdcMem, 0, 0, buffer, (int)strlen(buffer));
            free(buffer);
            buffer = NULL;
            #endif

            BitBlt(hdc, 0, 0, windowSize.right, windowSize.bottom,
                   hdcMem, 0, 0, SRCCOPY);

            DeleteDC(hdcMem);
            DeleteObject(bitmap);
            EndPaint(hwnd, &ps);
			return 0;

        case WM_ERASEBKGND:
            return 1;

        case WM_DESTROY:
            KillTimer(hwnd, 1);
            for(int i = 0; i < gridWidth; i++)
            {
                if(gameState[i])
                {
                    free(gameState[i]);
                }
            }

            if(gameState)
            {
                free(gameState);
            }

            if(players)
            {
                free(players);
            }

            if(buffer)
            {
                free(buffer);
            }
            PostQuitMessage(0);
            return 0;

        default:
            return DefWindowProc(hwnd, message, wParam, lParam);
    }

    return 0;
}
