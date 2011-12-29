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
#ifndef __game
#define __game
/* Andrew James
** ITC226 Application Programming for Windows
** Date Created: 2008/10/24
** Last Modified: 2008/10/29
** game.h
** Function protocols dealing with game logic, and the player struct.
*/
#include <windows.h>
#include <time.h>
#include <math.h>

typedef struct _player
{
    WCHAR name[21];
    COLORREF colour;
    BOOL isPlaying;
    int moves;
    int boxes;
    int score;
} Player;

BOOL AddPlayer(Player** players,
               int* totalPlayers,
               int* maxPlayers,
               WCHAR* name,
               COLORREF colour,
               BOOL isPlaying = TRUE,
               int moves = 0,
               int boxes = 0,
               int score = 0);

int CheckBoxSurroundings(int** gameState,
                         const int WIDTH,
                         const int HEIGHT,
                         const int XPOINT,
                         const int YPOINT);

void ClearGame(int** gameState,
               int gridWidth,
               int gridHeight,
               Player* players,
               int totalPlayers,
               int* activePlayer,
               int* gameWon,
               time_t* lastMoveTime = NULL);

int CountBoxes(int** gameState,
               const int WIDTH,
               const int HEIGHT);

BOOL GameInProgress(int** gameState,
                    const int WIDTH,
                    const int HEIGHT);

int HighestScore(const Player* players,
                 const int totalPlayers);

int MouseClick(LPARAM lParam,
               int** gameState,
               int gridWidth,
               int gridHeight,
               int segmentSize,
               int gridPadding,
               int activePlayer,
               Player* players,
               const int THRESHOLD = 10);

int NextPlayer(const Player* players,
               const int totalPlayers,
               int* activePlayer);

int RegisterInput(int** gameState,
                  int gridWidth,
                  int gridHeight,
                  int x,
                  int y,
                  Player* players,
                  int activePlayer);

BOOL ResizeGameState(int*** gameState,
                     int* gridWidth,
                     int* gridHeight,
                     int newWidth,
                     int newHeight);

void UpdatePlayerScores(int** gameState,
                        int gridWidth,
                        int gridHeight,
                        Player* players,
                        int totalPlayers,
                        int* activePlayer);
#endif
