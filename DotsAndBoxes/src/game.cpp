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
** Last Modified: 2008/10/29
** game.cpp
** Function definitions dealing with game logic.
*/
#include "game.h"

BOOL AddPlayer(Player** players,
               int* totalPlayers,
               int* maxPlayers,
               WCHAR* name,
               COLORREF colour,
               BOOL isPlaying,
               int moves,
               int boxes,
               int score)
{   //Takes the total number of players and the maximum number of players as
    //pointers so it can update them if needed.
    if((*totalPlayers) >= (*maxPlayers))
    {   //Heap corruption is fixed. Works roughly like C++ vectors, doubling
        //the available memory if I've hit the limit.
        (*maxPlayers) = (*maxPlayers) * 2;
        Player* newPlayerList = (Player*)calloc((*maxPlayers), sizeof(Player));
        if(!newPlayerList)
        {
            (*maxPlayers) = (*maxPlayers) / 2;
            return 0;
        }
        for(int i = 0; i < (*totalPlayers); i++)
        {   //Roll your own realloc... Will need to be updated if I change the
            //Player struct, or maybe I'll make it a binary copy instead.
            wcscpy_s(newPlayerList[i].name, (*players)[i].name);
            newPlayerList[i].colour = (*players)[i].colour;
            newPlayerList[i].isPlaying = (*players)[i].isPlaying;
            newPlayerList[i].moves = (*players)[i].moves;
            newPlayerList[i].boxes = (*players)[i].boxes;
            newPlayerList[i].score = (*players)[i].score;
        }
        free((*players));
        (*players) = newPlayerList;
    }
    if(name && ((int)wcslen(name) < 21))
    {
        wcscpy_s((*players)[(*totalPlayers)].name, name);
        (*players)[(*totalPlayers)].colour = colour;
        (*players)[(*totalPlayers)].isPlaying = isPlaying;
        (*players)[(*totalPlayers)].moves = moves;
        (*players)[(*totalPlayers)].boxes = boxes;
        (*players)[(*totalPlayers)].score = score;
        (*totalPlayers)++;
        return 1;
    }
    else
    {
        return 0;
    }
}

int CheckBoxSurroundings(int** gameState,
                         const int WIDTH,
                         const int HEIGHT,
                         const int XPOINT,
                         const int YPOINT)
{   //Making assumptions. Width is the number of entries in the array of
    //pointers. Height is the number of entries in each array of ints.
    //Returns the number of lines set that surround the specified box.
    int lines = 0;
    if(((XPOINT - 1) >= 0) &&
       ((YPOINT - 1) >= 0) &&
       ((XPOINT + 1) < WIDTH) &&
       ((YPOINT + 1) < HEIGHT))
    {
        if(gameState[XPOINT - 1][YPOINT])
        {
            lines++;
        }
        if(gameState[XPOINT + 1][YPOINT])
        {
            lines++;
        }
        if(gameState[XPOINT][YPOINT - 1])
        {
            lines++;
        }
        if(gameState[XPOINT][YPOINT + 1])
        {
            lines++;
        }
    }
    return lines;
}

void ClearGame(int** gameState,
               int gridWidth,
               int gridHeight,
               Player* players,
               int totalPlayers,
               int* activePlayer,
               int* gameWon,
               time_t* lastMoveTime)
{   //Sets the gamestate to zero, the hard way. I could probably use
    //ZeroMemory() to do the same thing without the nested loop. Also
    //reinstates all players and clears their moves and boxes count.
    for(int i = 0; i < gridWidth; i++)
    {
        for(int j = 0; j < gridHeight; j++)
        {
            gameState[i][j] = 0;
        }
    }

    for(int i = 0; i < totalPlayers; i++)
    {
        players[i].moves = 0;
        players[i].boxes = 0;
        players[i].isPlaying = TRUE;
    }

    if(activePlayer)
    {
        (*activePlayer) = 1;
    }

    if(lastMoveTime)
    {
        (*lastMoveTime) = 0;
    }

    if(gameWon)
    {
        (*gameWon) = 0;
    }
}

int CountBoxes(int** gameState, const int WIDTH, const int HEIGHT)
{   //Returns the number of boxes filled... fancy that.
    int boxesFilled = 0;
    for(int i = 1; i < WIDTH; i += 2)
    {
        for(int j = 1; j < HEIGHT; j += 2)
        {
            if(gameState[i][j])
            {
                boxesFilled++;
            }
        }
    }
    return boxesFilled;
}

BOOL GameInProgress(int** gameState, const int WIDTH, const int HEIGHT)
{   //Returns true if it finds any point in the gamestate that is not 0
    for(int i = 0; i < WIDTH; i++)
    {
        for(int j = 0; j < HEIGHT; j++)
        {
            if(((i % 2) != (j % 2)) && gameState[i][j])
            {
                return 1;
            }
        }
    }
    return 0;
}

int HighestScore(const Player* players, const int totalPlayers)
{   //Returns the index of the player with the highest score, or -1 in the case
    //of a draw.
    int highScorer = 0, nextHighest = totalPlayers - 1;
    for(int i = 1; i < totalPlayers; i++)
    {
        if(players[i].boxes >= players[highScorer].boxes)
        {
            nextHighest = highScorer;
            highScorer = i;
        }
    }
    if(players[highScorer].boxes > players[nextHighest].boxes)
    {
        return highScorer;
    }
    else
    {
        return -1;
    }
}

int MouseClick(LPARAM lParam,
               int** gameState,
               int gridWidth,
               int gridHeight,
               int segmentSize,
               int gridPadding,
               int activePlayer,
               Player* players,
               const int THRESHOLD)
{   //Handles mouse input. Feeds x and y co-ords to the RegisterInput()
    //function. Performs checking to make sure the mouse has clicked within the
    //grid before it calls RegisterInput().
    int boxCompleted = -1;
    POINT mouseClick = {(LOWORD(lParam) - gridPadding) + THRESHOLD,
                        (HIWORD(lParam) - gridPadding) + THRESHOLD};
    static POINT lastSelected = {-1,-1};

    if((mouseClick.x < 0) || (mouseClick.y < 0) ||
       (mouseClick.x > ((int)ceil(gridWidth / 2.0) * segmentSize)) ||
       (mouseClick.y > ((int)ceil(gridHeight / 2.0) * segmentSize)))
    {
        //Do nothing...
    }
    else if((mouseClick.x % segmentSize <= (THRESHOLD * 2)) &&
            (mouseClick.y % segmentSize <= (THRESHOLD * 2)))
    {   //If the user clicked on a point.
        if(lastSelected.x < 0 && lastSelected.y < 0)
        {   //If this is the first point the user has selected, update the
            //gamestate temporarily.
            lastSelected.x = (LONG)floor((mouseClick.x * 1.0)
                                          / segmentSize) * 2;
            lastSelected.y = (LONG)floor((mouseClick.y * 1.0)
                                          / segmentSize) * 2;
            gameState[lastSelected.x][lastSelected.y] = activePlayer;
        }
        else    
        {   //if the user has previously selected a point, clear the gamestate
            //for that point and pass the input to the RegisterInput() function
            POINT current = {(LONG)floor((mouseClick.x * 1.0)
                             / segmentSize) * 2,
                             (LONG)floor((mouseClick.y * 1.0)
                             / segmentSize) * 2};
            gameState[lastSelected.x][lastSelected.y] = 0;

            if(((lastSelected.x - current.x == 0) &&
               (abs(lastSelected.y - current.y) == 2)) ||
               ((lastSelected.y - current.y == 0) &&
               (abs(lastSelected.x - current.x) == 2)))
            {
                int x = (lastSelected.x / 2) + (current.x / 2);
                int y = (lastSelected.y / 2) + (current.y / 2);
                boxCompleted = RegisterInput(gameState, gridWidth, gridHeight,
                                             x, y, players, activePlayer);
            }

            lastSelected.x = -1;
            lastSelected.y = -1;
        }
    }
    else if(((mouseClick.x % segmentSize > (THRESHOLD * 2)) !=
            (mouseClick.y % segmentSize > (THRESHOLD * 2))) &&
            ((lastSelected.x < 0) &&
            (lastSelected.y < 0)))
    {   //Code for clicking on a line segment. Follows similar logic to the
        //previous section.
        int x = ((int)floor((mouseClick.x * 1.0) / segmentSize) * 2);
        int y = ((int)floor((mouseClick.y * 1.0) / segmentSize) * 2);

        if(mouseClick.x % segmentSize <= (THRESHOLD * 2))
        {   //If the user clicked on a vertical line.
            y++;
        }
        else if(mouseClick.y % segmentSize <= (THRESHOLD * 2))
        {   //If the user clicked on a horizontal line.
            x++;
        }

        boxCompleted = RegisterInput(gameState, gridWidth, gridHeight,
                                     x, y, players, activePlayer);
    }

    return boxCompleted;
}

int NextPlayer(const Player* players,
               const int totalPlayers,
               int* activePlayer)
{   //Returns the number of valid players, and updates the activePlayer if
    //there are more than one valid players.
    int validPlayers = 0;
    int playerToMove = (*activePlayer);
    for(int i = 0; i < totalPlayers; i++)
    {
        if(players[i].isPlaying)
        {
            validPlayers++;
        }
    }

    if(validPlayers)
    {
        do
        {
            if(playerToMove >= totalPlayers)
            {
                playerToMove = 1;
            }
            else
            {
                playerToMove++;
            }
        }
        while(!players[playerToMove - 1].isPlaying);
    }

    (*activePlayer) = playerToMove;

    return validPlayers;
}

int RegisterInput(int** gameState,
                  int gridWidth,
                  int gridHeight,
                  int x,
                  int y,
                  Player* players,
                  int activePlayer)
{   //Performs checking to make sure the input was valid, then updates the
    //gamestate. Checks if the updated point was a horizontal or vertical line
    //then updates the box above, below, to the left, or to the right if
    //placing a line there enclosed the box.
    int boxCompleted = 0;
    if((x < gridWidth) &&
       (y < gridHeight) &&
       ((x % 2) != (y % 2)) &&
       !gameState[x][y])
    {
        gameState[x][y] = activePlayer;
        if((x % 2) == 0)
        {
            if(((x - 1) > 0) &&
               !gameState[x - 1][y] &&
               (CheckBoxSurroundings(gameState,
                                     gridWidth,
                                     gridHeight,
                                     x - 1,
                                     y) == 4))
            {
                gameState[x - 1][y] = activePlayer;
                boxCompleted++;
            }

            if(((x + 1) < gridWidth) &&
               !gameState[x + 1][y] &&
               (CheckBoxSurroundings(gameState,
                                     gridWidth,
                                     gridHeight,
                                     x + 1,
                                     y) == 4))
            {
                gameState[x + 1][y] = activePlayer;
                boxCompleted++;
            }
        }
        if((y % 2) == 0)
        {
            if(((y - 1) > 0) &&
               !gameState[x - 1][y] &&
               (CheckBoxSurroundings(gameState,
                                     gridWidth,
                                     gridHeight,
                                     x,
                                     y - 1) == 4))
            {
                gameState[x][y - 1] = activePlayer;
                boxCompleted++;
            }

            if(((y + 1) < gridWidth) &&
               !gameState[x + 1][y] &&
               (CheckBoxSurroundings(gameState,
                                     gridWidth,
                                     gridHeight,
                                     x,
                                     y + 1) == 4))
            {
                gameState[x][y + 1] = activePlayer;
                boxCompleted++;
            }
        }

        if(boxCompleted)
        {
            players[activePlayer - 1].boxes += boxCompleted;
        }
    }
    else
    {
        boxCompleted = -1;
    }

    return boxCompleted;
}

BOOL ResizeGameState(int*** gameState,
                     int* gridWidth,
                     int* gridHeight,
                     int newWidth,
                     int newHeight)
{   //Returns 0 if it encounters an error. ie, can't allocate memory.
    //Creates a new gamestate of the specified size, copies values across
    //then frees the old gamestate and updates the pointer to point at the new
    //gamestate.
    if(int** newGameState = (int**)calloc(newWidth, sizeof(int*)))
    {
        for(int i = 0; i < newWidth; i++)
        {
            if(newGameState[i] = (int*)calloc(newHeight, sizeof(int)))
            {
                if(i < (*gridWidth))
                {
                    for(int j = 0; j < min(newHeight, (*gridHeight)); j++)
                    {
                        newGameState[i][j] = (*gameState)[i][j];
                    }
                }
            }
            else
            {
                for(int j = 0; j < i; j++)
                {
                    free(newGameState[j]);
                }
                free(newGameState);
                return 0;
            }
        }
        for(int i = 0; i < (*gridWidth); i++)
        {
            free((*gameState)[i]);
        }
        free((*gameState));
        (*gameState) = newGameState;
        (*gridWidth) = newWidth;
        (*gridHeight) = newHeight;
        return 1;
    }
    else
    {
        return 0;
    }
}

void UpdatePlayerScores(int** gameState,
                        int gridWidth,
                        int gridHeight,
                        Player* players,
                        int totalPlayers,
                        int* activePlayer)
{
    if((*activePlayer) > totalPlayers)
    {
        (*activePlayer) = 1;
    }
    else if(!players[(*activePlayer) - 1].isPlaying)
    {
        NextPlayer(players, totalPlayers, activePlayer);
    }

    for(int i = 0; i < totalPlayers; i++)
    {
        players[i].boxes = 0;
    }

    for(int i = 0; i < gridWidth; i++)
    {
        for(int j = 0; j < gridHeight; j++)
        {
            if(gameState[i][j] <= totalPlayers)
            {   //I'm really tired. This works. It's easy to understand.
                //I've spent a solid week on this, don't ask questions...
                //So easy to understand I wrote it wrong the first time...
                if((i % 2) && (j % 2))
                {
                    players[gameState[i][j] - 1].boxes++;
                }
                else if((i % 2) && (j % 2))
                {
                    players[gameState[i][j] - 1].isPlaying = 1;
                }
            }
        }
    }
}
