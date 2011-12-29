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
#ifndef __paint
#define __paint
/* Andrew James
** ITC226 Application Programming for Windows
** Date Created: 2008/10/24
** Last Modified: 2008/10/28
** paint.h
** Function protocols to do with painting. Anything with draw in the name is
** only to be called during WM_PAINT.
*/
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "game.h"

void CalculateDotPitch(HWND hwnd,
                       RECT* windowSize,
                       int gridWidth,
                       int gridHeight,
                       int sideBarWidth,
                       int* segmentSize,
                       int* gridPadding);

BOOL DrawDots(HDC hdc,
              int** gameState,
              const int gridWidth,
              const int gridHeight,
              Player* players,
              int totalPlayers,
              const int interval,
              const int padding);

BOOL DrawLines(HDC hdc,
               int** gameState,
               const int gridWidth,
               const int gridHeight,
               Player* players,
               int totalPlayers,
               const int interval,
               const int padding);

BOOL DrawRectangles(HDC hdc,
                    int** gameState,
                    const int gridWidth,
                    const int gridHeight,
                    Player* players,
                    int totalPlayers,
                    const int interval,
                    const int padding);

BOOL DrawSidebar(HDC hdc,
                 Player* players,
                 const int totalPlayers,
                 const int activePlayer,
                 const int width,
                 const int xAnchor,
                 const int timeLimit = 0,
                 const time_t lastMoveTime = 0,
                 const int gameWon = 0);
#endif
