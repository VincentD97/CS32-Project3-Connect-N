//
//  support.cpp
//  Connect N
//
//  Created by DongVincent on 5/15/16.
//  Copyright © 2016 UCLA. All rights reserved.
//

#include "support.h"

bool leadToWin(Scaffold s, int N, int& winner, int lastCol)             // tell if the last move leads the player to win
{
    if (N == 1)                                                         // special condition when N == 1
    {
        winner = RED;
        return true;
    }
    int lastLevel;
    lastLevel = s.levels();
    while (s.checkerAt(lastCol, lastLevel) == VACANT)
    {   lastLevel--;    }                                               // find the level of last move (the column is given as a parameter)
    int color = s.checkerAt(lastCol, lastLevel);                        // whose turn. and now check only for that color
    int x, y;
    int sameColor;
    bool flag;
    // check the rows / cols / diagonals including that checker
    // further, we start from that checker, look to its left, right, up, down, and diagonal directions and find the number of consecutive blocks with the same color
    
    // in a row
    x = lastCol + 1; y = lastLevel; sameColor = 1; flag = false;
    while (x <= s.cols() && s.checkerAt(x, y) == color)
    {
        sameColor++;
        x++;
        if (sameColor >= N)
        {
            flag = true;
            break;
        }
    }
    if (!flag)
    {
        x = lastCol - 1;
        while (x >= 1 && s.checkerAt(x, y) == color)
        {
            sameColor++;
            x--;
            if (sameColor >= N)
            {
                flag = true;
                break;
            }
        }
    }
    if (flag) { winner = color; return true; }
    // in a column
    x = lastCol; y = lastLevel + 1; sameColor = 1; flag = false;
    while (y <= s.levels() && s.checkerAt(x, y) == color)
    {
        sameColor++;
        y++;
        if (sameColor >= N)
        {
            flag = true;
            break;
        }
    }
    if (!flag)
    {
        y = lastLevel - 1;
        while (y >= 1 && s.checkerAt(x, y) == color)
        {
            sameColor++;
            y--;
            if (sameColor >= N)
            {
                flag = true;
                break;
            }
        }
    }
    if (flag) { winner = color; return true; }
    // in a "/"
    x = lastCol + 1; y = lastLevel + 1; sameColor = 1; flag = false;
    while (x <= s.cols() && y <= s.levels() && s.checkerAt(x, y) == color)
    {
        sameColor++;
        x++; y++;
        if (sameColor >= N)
        {
            flag = true;
            break;
        }
    }
    if (!flag)
    {
        x = lastCol - 1; y = lastLevel - 1;
        while (x >= 1 && y >= 1 && s.checkerAt(x, y) == color)
        {
            sameColor++;
            x--; y--;
            if (sameColor >= N)
            {
                flag = true;
                break;
            }
        }
    }
    if (flag) { winner = color; return true; }
    // in a "\"
    x = lastCol - 1; y = lastLevel + 1; sameColor = 1; flag = false;
    while (x >= 1 && y <= s.levels() && s.checkerAt(x, y) == color)
    {
        sameColor++;
        x--; y++;
        if (sameColor >= N)
        {
            flag = true;
            break;
        }
    }
    if (!flag)
    {
        x = lastCol + 1; y = lastLevel - 1;
        while (x <= s.cols() && y >= 1 && s.checkerAt(x, y) == color)
        {
            sameColor++;
            x++; y--;
            if (sameColor >= N)
            {
                flag = true;
                break;
            }
        }
    }
    if (flag) { winner = color; return true; }
    // tie
    if (s.numberEmpty() == 0)
    {
        winner = TIE_GAME;
        return true;
    }
    return false;                       // not completed yet
    
}
