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
#ifndef __fileio
#define __fileio
/* Andrew James
** ITC226 Application Programming for Windows
** Date Created: 2008/10/27
** Last Modified: 2008/10/30
** fileio.h
** Function protocols to do with saving and loading
*/
#include <windows.h>
#include <stdio.h>
#include "game.h"
BOOL LoadGame(WCHAR* filename,
              int*** gameState,
              int* gridWidth,
              int* gridHeight,
              int* activePlayer,
              int* timeLimit);

BOOL SaveGame(WCHAR* filename,
              int** gameState,
              int gridWidth,
              int gridHeight,
              int activePlayer,
              int timeLimit);

BOOL LoadPlayers(WCHAR* filename,
                 Player** players,
                 int* totalPlayers,
                 int* maxPlayers);

BOOL SavePlayers(WCHAR* filename,
                 Player* players,
                 int totalPlayers);
#endif
