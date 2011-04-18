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
** Date Created: 2008/10/27
** Last Modified: 2008/10/30
** fileio.cpp
** Function definitions to do with saving and loading
*/
#include "fileio.h"

BOOL LoadGame(char* filename,
              int*** gameState,
              int* gridWidth,
              int* gridHeight,
              int* activePlayer,
              int* timeLimit)
{   //Wee, now has error checking due to a stupid flipping error I was getting.
    //Reads the first 12 bytes of the file as ints, then sees how large the
    //file is. If it isn't equal to 4 ints, plus the width * height given in
    //the file returns 0 and closes the file. Otherwise, it reads the rest of
    //the data into the gamestate and returns true.
    FILE *inputFilePtr = fopen(filename, "rb");
    if(inputFilePtr)
    {
        int newGridWidth = 0, newGridHeight = 0;
        int newActivePlayer = 0, newTimeLimit = 0;
        int sizeOfFile, expectedSize;
        fpos_t gameStateStart;

        fread(&newGridWidth, sizeof(int), 1, inputFilePtr);
        fread(&newGridHeight, sizeof(int), 1, inputFilePtr);

        expectedSize = (((newGridWidth * newGridHeight) * sizeof(int)) +
                        (sizeof(int) * 4));

        fgetpos(inputFilePtr, &gameStateStart);
        fseek(inputFilePtr, 0, SEEK_END);
        sizeOfFile = ftell(inputFilePtr);
        fsetpos(inputFilePtr, &gameStateStart);

        if(sizeOfFile == expectedSize)
        {
            if((newGridWidth != (*gridWidth)) ||
               (newGridHeight != (*gridHeight)))
            {
                if(!ResizeGameState(gameState, gridWidth, gridHeight,
                                    newGridWidth, newGridHeight))
                {
                    fclose(inputFilePtr);
                    return 0;
                }
            }

            fread(&newActivePlayer, sizeof(int), 1, inputFilePtr);
            fread(&newTimeLimit, sizeof(int), 1, inputFilePtr);

            for(int i = 0; i < newGridWidth; i++)
            {
                fread((*gameState)[i], sizeof(int),
                      newGridHeight, inputFilePtr);
            }

            (*activePlayer) = newActivePlayer;
            (*timeLimit) = newTimeLimit;
            fclose(inputFilePtr);
            return 1;
        }
        else
        {
            fclose(inputFilePtr);
            return 0;
        }
    }

    return 0;
}

BOOL SaveGame(char* filename,
              int** gameState,
              int gridWidth,
              int gridHeight,
              int activePlayer,
              int timeLimit)
{   //Writes the width and height to the start of the file, the active player,
    //the timeLimit per turn, then the gamestate immediately follows.
    FILE *outputFilePtr = fopen(filename, "wb");
    if(outputFilePtr)
    {
        fwrite(&gridWidth, sizeof(int), 1, outputFilePtr);
        fwrite(&gridHeight, sizeof(int), 1, outputFilePtr);
        fwrite(&activePlayer, sizeof(int), 1, outputFilePtr);
        fwrite(&timeLimit, sizeof(int), 1, outputFilePtr);

        for(int i = 0; i < gridWidth; i++)
        {
            fwrite(gameState[i], sizeof(int), gridHeight, outputFilePtr);
        }
        fclose(outputFilePtr);
        return 1;
    }

    return 0;
}

BOOL LoadPlayers(char* filename,
                 Player** players,
                 int* totalPlayers,
                 int* maxPlayers)
{   //Reads the expected number of players from the start of the file, then
    //checks to make sure the filesize is equal to that number of player
    //structs. If the file is the right size, reads it in, otherwise returns 0.
    FILE *inputFilePtr = fopen(filename, "rb");
    if(inputFilePtr)
    {
        int newTotalPlayers;
        int sizeOfFile;
        fpos_t playerInfoStart;

        fread(&newTotalPlayers, sizeof(int), 1, inputFilePtr);

        fgetpos(inputFilePtr, &playerInfoStart);
        fseek(inputFilePtr, 0, SEEK_END);
        sizeOfFile = ftell(inputFilePtr);
        fsetpos(inputFilePtr, &playerInfoStart);

        if(((sizeOfFile - sizeof(int)) / sizeof(Player)) == newTotalPlayers)
        {
            if(newTotalPlayers > (*maxPlayers))
            {   //If there are more players in the file than we have memory
                //allocated for, this reallocs exactly  the amount of memory we
                //need and then overwrites the memory with the contents of the
                //file.
                if(realloc((*players), newTotalPlayers * sizeof(Player)))
                {
                    (*maxPlayers) = newTotalPlayers;
                }
                else
                {
                    fclose(inputFilePtr);
                    return 0;
                }
            }

            fread((*players), sizeof(Player), newTotalPlayers, inputFilePtr);
            (*totalPlayers) = newTotalPlayers;
            fclose(inputFilePtr);
            return 1;
        }
        else
        {
            fclose(inputFilePtr);
            return 0;
        }
    }

    return 0;
}

BOOL SavePlayers(char* filename, Player* players, int totalPlayers)
{   //Writes the number of players in the player struct, then the contents of
    //the array of players.
    FILE *outputFilePtr = fopen(filename, "wb");
    if(outputFilePtr && totalPlayers)
    {
        fwrite(&totalPlayers, sizeof(int), 1, outputFilePtr);
        fwrite(players, sizeof(Player), totalPlayers, outputFilePtr);
        fclose(outputFilePtr);
        return 1;
    }

    return 0;
}
