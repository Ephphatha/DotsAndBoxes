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
** Date Created: 2008/10/24
** Last Modified: 2008/10/28
** paint.cpp
** Function definitions to do with painting. Anything with Draw in the name is
** only to be called during WM_PAINT.
*/
#include "paint.h"

void CalculateDotPitch(HWND hwnd,
                       RECT* windowSize,
                       int gridWidth,
                       int gridHeight,
                       int sideBarWidth,
                       int* segmentSize,
                       int* gridPadding)
{   //Sets the segment size to the minimum of the windows x dimension, minus
    //the sidebar, all divided by the horizontal number of dots, and the
    //windows y direction divided by the vertical number of dots.
    GetClientRect(hwnd, windowSize);
    (*segmentSize) = min((windowSize->right - sideBarWidth)
                          / ((int)floor(gridWidth / 2.0) + 1),
                          windowSize->bottom
                          / ((int)floor(gridHeight / 2.0) + 1));
    (*gridPadding) = (*segmentSize) / 2;
}

BOOL DrawDots(HDC hdc,
              int** gameState,
              const int gridWidth,
              const int gridHeight,
              Player* players,
              int totalPlayers,
              const int interval,
              const int padding)
{   //Draws the dots, if the gameState indicates that a player has selected
    //it and the player exists, changes to the players colour. If the player
    //doesn't exist, sets the colour as grey, and if no player has selected the
    //dot, sets the colour as black.
    HPEN pen, initialPen;
    HBRUSH brush, initialBrush;
    int player = 0;
    for(int i = 0; i < ((int)floor(gridWidth / 2.0) + 1); i++)
    {
        for(int j = 0; j < ((int)floor(gridHeight / 2.0) + 1); j++)
        {
            if(gameState[i * 2][j * 2])
            {
                player = gameState[i * 2][j * 2] - 1;
                if(player < totalPlayers)
                {
                    pen = CreatePen(PS_SOLID, 1, players[player].colour);
                    brush = CreateSolidBrush(players[player].colour);
                }
                else
                {
                    pen = CreatePen(PS_SOLID, 1, 0x00999999);
                    brush = CreateSolidBrush(0x00999999);
                }
            }
            else
            {
                pen = CreatePen(PS_SOLID, 1, 0x00000000);
                brush = CreateSolidBrush(0x00000000);
            }
            initialPen = (HPEN)SelectObject(hdc, pen);
            initialBrush = (HBRUSH)SelectObject(hdc, brush);

            int x = (i * interval) + padding - 5;
            int y = (j * interval) + padding - 5;
            Ellipse(hdc, x, y, x + 10, y + 10);

            SelectObject(hdc, initialPen);
            DeleteObject(pen);
            
            SelectObject(hdc, initialBrush);
            DeleteObject(brush);
        }
    }
    return 1;
}

BOOL DrawLines(HDC hdc,
               int** gameState,
               const int gridWidth,
               const int gridHeight,
               Player* players,
               int totalPlayers,
               const int interval,
               const int padding)
{   //Draws the lines with similar logic to the previous function when it comes
    //to setting colours. The pen is set to white with a two pixel border to
    //make the grid look more pretty :P. When it comes to drawing lines it
    //detects whether the lines should be drawn horizontal or vertical, and
    //then draws a rectangle.
    HPEN pen, initialPen;
    HBRUSH brush, initialBrush;
    int player = 0;
    for(int i = 0; i < gridWidth; i++)
    {
        for(int j = 0; j < gridHeight; j++)
        {
            if(gameState[i][j])
            {
                player = gameState[i][j] - 1;
                pen = CreatePen(PS_SOLID, 2, 0x00FFFFFF);

                if(player < totalPlayers)
                {
                    brush = CreateSolidBrush(players[player].colour);
                }
                else
                {
                    brush = CreateSolidBrush(0x00999999);
                }

                initialPen = (HPEN)SelectObject(hdc, pen);
                initialBrush = (HBRUSH)SelectObject(hdc, brush);

                if((i % 2) != (j % 2))
                {
                    int x = ((int)floor(i / 2.0) * interval) +
                            padding;
                    int y = ((int)floor(j / 2.0) * interval) +
                            padding;

                    Rectangle(hdc, x - 3, y - 3,
                              x + 4 + ((i % 2) * interval),
                              y + 4 + ((j % 2) * interval));
                }

                SelectObject(hdc, initialPen);
                DeleteObject(pen);

                SelectObject(hdc, initialBrush);
                DeleteObject(brush);
            }
        }
    }
    return 1;
}

BOOL DrawRectangles(HDC hdc,
                    int** gameState,
                    const int gridWidth,
                    const int gridHeight,
                    Player* players,
                    int totalPlayers,
                    const int interval,
                    const int padding)
{   //Draws a rectangle with the colour logic as defined above. The rectangle
    //is draw from the top left dot to the bottom right dot.
    HPEN pen, initialPen;
    HBRUSH brush, initialBrush;
    int player = 0;
    for(int i = 1; i < gridWidth; i += 2)
    {
        for(int j = 1; j < gridHeight; j += 2)
        {
            if(gameState[i][j])
            {
                player = gameState[i][j] - 1;
                if(player < totalPlayers)
                {
                    pen = CreatePen(PS_SOLID, 1, players[player].colour);
                    brush = CreateSolidBrush(players[player].colour);
                }
                else
                {
                    pen = CreatePen(PS_SOLID, 1, 0x00999999);
                    brush = CreateSolidBrush(0x00999999);
                }

                initialPen = (HPEN)SelectObject(hdc, pen);
                initialBrush = (HBRUSH)SelectObject(hdc, brush);

                Rectangle(hdc,
                          ((int)floor(i / 2.0) * interval) + padding,
                          ((int)floor(j / 2.0) * interval) + padding,
                          ((int)ceil(i / 2.0) * interval) + padding,
                          ((int)ceil(j / 2.0) * interval) + padding);

                SelectObject(hdc, initialPen);
                DeleteObject(pen);
            
                SelectObject(hdc, initialBrush);
                DeleteObject(brush);
            }
        }
    }
    return 1;
}

BOOL DrawSidebar(HDC hdc,
                 Player* players,
                 const int totalPlayers,
                 const int activePlayer,
                 const int width,
                 const int xAnchor,
                 const int timeLimit,
                 const time_t lastMoveTime,
                 const int gameWon)
{   //Assuming the activeplayer is fed as an index, totalPlayers is the number
    //of entries in the array of players. If the total number of players is 0,
    //prompts the user to add a player. If the total number of players is 1,
    //comes up with a (used to be humorous) message. If there is more than one
    //player and a timelimit, displays the player to move and the time left.
    //Otherwise, just displays the player to move. If the game has been won,
    //Displays "<player> has won.". The playerlist is drawn below that text.
    //TODO: Make this text scrollable.
    COLORREF initialColour;
    UINT initialTextAlignment;
    TEXTMETRIC tm;
    GetTextMetrics(hdc, &tm);
    int lineHeight = tm.tmHeight + tm.tmExternalLeading;
    WCHAR* buffer = (WCHAR*)calloc(100, sizeof(WCHAR));

    initialColour = SetTextColor(hdc, players[activePlayer].colour);
    if(totalPlayers && timeLimit && lastMoveTime)
    {
        swprintf_s(buffer, 100, L"%d seconds to move",
                timeLimit - (time(0) - lastMoveTime));
        TextOut(hdc, xAnchor, lineHeight, buffer, (int)wcslen(buffer));
        swprintf_s(buffer, 100, L"%s has", players[activePlayer].name);
    }
    else if(gameWon == 2)
    {
        swprintf_s(buffer, 100, L"%s wins", players[activePlayer].name);
    }
    else if(gameWon == 1)
    {
        SetTextColor(hdc, players[HighestScore(players, totalPlayers)].colour);
        swprintf_s(buffer, 100, L"%s wins",
                players[HighestScore(players, totalPlayers)].name);
    }
    else if(gameWon == -1)
    {
        wcscpy_s(buffer, 100, L"This game is a draw");
    }
    else if(totalPlayers == 1)
    {
        int lastIndex = wcslen(players[activePlayer].name) - 1;
        WCHAR lastChar = players[activePlayer].name[lastIndex];
        if((lastChar == L'a') || (lastChar == L'e') || (lastChar == L'i'))
        {
            wcscpy_s(buffer, 100, L"against herself.");
        }
        else if(lastChar == L'y')
        {
            wcscpy_s(buffer, 100, L"against themselves.");
        }
        else
        {
            wcscpy_s(buffer, 100, L"against himself.");
        }
        TextOut(hdc, xAnchor, lineHeight, buffer, (int)wcslen(buffer));
        swprintf_s(buffer, 100, L"%s is playing", players[activePlayer].name);
    }
    else if(totalPlayers)
    {
        swprintf_s(buffer, 100, L"It is %s's turn", players[activePlayer].name);
    }
    else
    {
        wcscpy_s(buffer, 100, L"Please add a player");
    }
    TextOut(hdc, xAnchor, 0, buffer, (int)wcslen(buffer));

    for(int i = 0; i < totalPlayers; i++)
    {
        if(players[i].isPlaying)
        {
            SetTextColor(hdc, players[i].colour);
        }
        else
        {
            SetTextColor(hdc, 0x00777777);
        }

        swprintf_s(buffer, 100, L"%s's boxes:", players[i].name);
        TextOut(hdc, xAnchor, (lineHeight * 2) + (i * lineHeight * 2),
                buffer, (int)wcslen(buffer));

        swprintf_s(buffer, 100, L"%s's wins:", players[i].name);
        TextOut(hdc, xAnchor, (lineHeight * 3) + (i * lineHeight * 2),
                buffer, (int)wcslen(buffer));

        initialTextAlignment = SetTextAlign(hdc, TA_RIGHT);
        swprintf_s(buffer, 100, L"%d", players[i].boxes);
        TextOut(hdc, xAnchor + width, (lineHeight * 2) + (i * lineHeight * 2),
                buffer, (int)wcslen(buffer));

        swprintf_s(buffer, 100, L"%d", players[i].score);
        TextOut(hdc, xAnchor + width, (lineHeight * 3) + (i * lineHeight * 2),
                buffer, (int)wcslen(buffer));
        SetTextAlign(hdc, initialTextAlignment);
    }

    SetTextColor(hdc, initialColour);
    free(buffer);
    buffer = NULL;
    return 1;
}
