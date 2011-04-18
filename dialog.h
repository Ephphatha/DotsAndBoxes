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
#ifndef __dialog
#define __dialog
/* Andrew James
** ITC226 Application Programming for Windows
** Date Created: 2008/10/25
** Last Modified: 2008/10/30
** dialog.h
** Function protocols for the dialog box procedures.
** Declared in a different style to the rest of my headers, I know, but the
** variable names I use are the same as the type, sans case.
** Defines are in here as they are the values for the messages I use to
** communicate with my WndProc().
*/
#include <windows.h>
#include "game.h"
#include "resource.h"
#define ADDPLAYER 0x8042
#define CHANGETIMELIMIT 0x8084
#define EDITGAMESTATE 0x80C6
#define LOADGAME 0x8108
#define SAVEGAME 0x814A
#define NEWGAME 0x818C
#define QUITGAME 0x81CE
#define LOADPLAYERS 0x8210
#define SAVEPLAYERS 0x8252
#define PAUSETIMER 0x8294
#define RESUMETIMER 0x82D6

INT_PTR CALLBACK DlgProcAddPlayer(HWND, UINT, WPARAM, LPARAM);

INT_PTR CALLBACK DlgProcGameOptions(HWND, UINT, WPARAM, LPARAM);

INT_PTR CALLBACK DlgProcTimeLimit(HWND, UINT, WPARAM, LPARAM);

BOOL MenuCommands(HWND, HINSTANCE, WPARAM);
#endif
