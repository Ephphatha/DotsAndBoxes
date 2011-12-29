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
** Date Created: 2008/10/30
** Last Modified: 2008/10/30
** ai.cpp
** Function definitons for AI. The only expected parameters are the game state,
** it's width, height, and optionally a time to live. The last parameter is
** unused as this simple algorithm runs very quickly anyway.
*/
#include "ai.h"
POINT PlayMove(int** gameState,
               int gridWidth,
               int gridHeight,
               int timeToLive)
{
    int** aiState = (int**)calloc(gridWidth, sizeof(int*));
    POINT bestMove = {1,0};

    for(int i = 0; i < gridWidth; i++)
    {
        aiState[i] = (int*)calloc(gridHeight, sizeof(int));
        for(int j = 0; j < gridHeight; j++)
        {
            if((i % 2) != (j % 2))
            {
                if(!gameState[i][j])
                {
                    if((i > 1) && (j > 1) &&
                       (i < (gridWidth - 2)) &&
                       (j < (gridHeight - 2)))
                    {
                        aiState[i][j] = 2;
                    }
                    else
                    {
                        aiState[i][j] = 1;
                    }
                }
                else
                {
                    aiState[i][j] = -1;
                }
            }
        }
    }

    for(int x = 0; x < gridWidth; x++)
    {
        for(int y = 0; y < gridHeight; y++)
        {
            if((x % 2) != (y % 2))
            {
                if(((x % 2) == 0) && aiState[x][y] >= 0)
                {
                    if(((x - 1) > 0) &&
                       !gameState[x - 1][y])
                    {
                        switch(CheckBoxSurroundings(gameState,
                                                    gridWidth,
                                                    gridHeight,
                                                    x - 1,
                                                    y))
                        {
                            case 1:
                                if(aiState[x][y])
                                {
                                    aiState[x][y]++;
                                }
                                break;

                            case 2:
                                if(aiState[x][y] < 5)
                                {
                                    aiState[x][y] = 0;
                                }
                                break;

                            case 3:
                                aiState[x][y] = 5;
                                break;
                        }
                    }

                    if(((x + 1) < gridWidth) &&
                       !gameState[x + 1][y])
                    {
                        switch(CheckBoxSurroundings(gameState,
                                                    gridWidth,
                                                    gridHeight,
                                                    x + 1,
                                                    y))
                        {
                            case 1:
                                if(aiState[x][y])
                                {
                                    aiState[x][y]++;
                                }
                                break;

                            case 2:
                                if(aiState[x][y] < 5)
                                {
                                    aiState[x][y] = 0;
                                }
                                break;

                            case 3:
                                aiState[x][y] = 5;
                                break;
                        }
                    }
                }
                if(((y % 2) == 0) && aiState[x][y] >= 0)
                {
                    if(((y - 1) > 0) &&
                       !gameState[x - 1][y])
                    {
                        switch(CheckBoxSurroundings(gameState,
                                                    gridWidth,
                                                    gridHeight,
                                                    x,
                                                    y - 1))
                        {
                            case 1:
                                if(aiState[x][y])
                                {
                                    aiState[x][y]++;
                                }
                                break;

                            case 2:
                                if(aiState[x][y] < 5)
                                {
                                    aiState[x][y] = 0;
                                }
                                break;

                            case 3:
                                aiState[x][y] = 5;
                                break;
                        }
                    }

                    if(((y + 1) < gridWidth) &&
                       !gameState[x + 1][y])
                    {
                        switch(CheckBoxSurroundings(gameState,
                                                    gridWidth,
                                                    gridHeight,
                                                    x,
                                                    y + 1))
                        {
                            case 1:
                                if(aiState[x][y])
                                {
                                    aiState[x][y]++;
                                }
                                break;

                            case 2:
                                if(aiState[x][y] < 5)
                                {
                                    aiState[x][y] = 0;
                                }
                                break;

                            case 3:
                                aiState[x][y] = 5;
                                break;
                        }
                    }
                }
            }
        }
    }

    for(int x = 0; x < gridWidth; x++)
    {
        for(int y = 0; y < gridHeight; y++)
        {
            if((x % 2) != (y % 2))
            {
                if(aiState[x][y] > aiState[bestMove.x][bestMove.y])
                {
                    bestMove.x = x;
                    bestMove.y = y;
                }
            }
        }
    }

    for(int i = 0; i < gridWidth; i++)
    {
        free(aiState[i]);
    }
    free(aiState);
    return bestMove;
}
