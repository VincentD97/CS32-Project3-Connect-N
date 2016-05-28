// Game.cpp

#include "provided.h"
#include "support.h"
#include <iostream>
#include <climits>
using namespace std;

class GameImpl
{
  public:
    GameImpl(int nColumns, int nLevels, int N, Player* red, Player* black); 
    bool completed(int& winner) const; 
    bool takeTurn(); 
    void play(); 
    int checkerAt(int c, int r) const;
  private:
    Scaffold s;
    int goal;
    Player* m_red;
    Player* m_black;
    int whoseTurn;
    bool gameCompleted;
    int winner;
};

GameImpl::GameImpl(int nColumns, int nLevels, int N, Player* red, Player* black)
 : s(nColumns, nLevels), goal(N), m_red(red), m_black(black), whoseTurn(0), gameCompleted(false), winner(0)                                     // construct/initialize the private data
{
}

bool GameImpl::completed(int& winner) const
{
    int i, j, color;
    bool allsame;
    // in a row
    for (i = 1; i <= s.cols() + 1 - goal; i++)
        for (j = 1; j <= s.levels(); j++)                       // start from each block
        {
            color = s.checkerAt(i, j);
            if (color != VACANT)                                // if nonempty
            {
                allsame = true;
                for (int k = 1; k < goal; k++)
                    if (s.checkerAt(i+k, j) != color)
                    {
                        allsame = false;
                        break;
                    }
                if (allsame)
                {
                    winner = color;
                    return true;
                }
            }
        }
    // in a column                                                          // other directions
    for (i = 1; i <= s.cols(); i++)
        for (j = 1; j <= s.levels() + 1 - goal; j++)
        {
            color = s.checkerAt(i, j);
            if (color != VACANT)
            {
                allsame = true;
                for (int k = 1; k < goal; k++)
                    if (s.checkerAt(i, j+k) != color)
                    {
                        allsame = false;
                        break;
                    }
                if (allsame)
                {
                    winner = color;
                    return true;
                }
            }
        }
    // in a "/"
    for (i = 1; i <= s.cols() + 1 - goal; i++)
        for (j = 1; j <= s.levels() + 1 - goal; j++)
        {
            color = s.checkerAt(i, j);
            if (color != VACANT)
            {
                allsame = true;
                for (int k = 1; k < goal; k++)
                    if (s.checkerAt(i+k, j+k) != color)
                    {
                        allsame = false;
                        break;
                    }
                if (allsame)
                {
                    winner = color;
                    return true;
                }
            }
        }
    // in a "\"
    for (i = 1; i <= s.cols() + 1 - goal; i++)
        for (j = goal; j <= s.levels(); j++)
        {
            color = s.checkerAt(i, j);
            if (color != VACANT)
            {
                allsame = true;
                for (int k = 1; k < goal; k++)
                    if (s.checkerAt(i+k, j-k) != color)
                    {
                        allsame = false;
                        break;
                    }
                if (allsame)
                {
                    winner = color;
                    return true;
                }
            }
        }
    if (s.numberEmpty() == 0)
    {
        winner = TIE_GAME;
        return true;
    }
    return false;
}

bool GameImpl::takeTurn()
{
    int col;
    if (gameCompleted) return false;
    if (whoseTurn == RED)
        col = m_red->chooseMove(s, goal, RED);
    else                                                        // black
        col = m_black->chooseMove(s, goal, BLACK);              // choose the move
    if (col != -1)                                              // if move is possible
    {
        s.makeMove(col, whoseTurn);
        s.display();                                            // make move and display
    }
    
    whoseTurn = 1 - whoseTurn;                                  // change turn
    if (leadToWin(s, goal, winner, col)) gameCompleted = true;  // if wins, update the monitor
    return true;
}

void GameImpl::play()
{
    cout << "\n\nConnect " << goal << "   with a " << s.cols() << " col * " << s.levels() << " lvl Scaffold\n\n";
    s.display();
    cout << "\n" << m_red->name() << " (red) goes first and then " << m_black->name() << " (black).\n\n";
    cout << "Press ENTER to start ...  ";
    cin.ignore(INT_MAX, '\n');
    for (;;)
    {
        if (!takeTurn()) break;
        if (!m_red->isInteractive() && !m_black->isInteractive())
        {
            cout << "Press ENTER to continue ...  ";                            // pause if both are not interactive
            cin.ignore(INT_MAX, '\n');
        }
    }
    if (gameCompleted)
    {
        if (winner == RED) cout << "\n  " << m_red->name() << " (red) wins !\n";
        if (winner == BLACK) cout << "\n  " << m_black->name() << " (black) wins !\n";
        if (winner == TIE_GAME) cout << "\n  TIE game !\n";
    }
    else cout << "\n  Something must be wrong !\n";
}

int GameImpl::checkerAt(int c, int r) const
{
    return s.checkerAt(c, r);                               // copy the checkerAt() function from Scaffold
} 

//******************** Game functions *******************************

// These functions simply delegate to GameImpl's functions.
// You probably don't want to change any of this code.

Game::Game(int nColumns, int nLevels, int N, Player* red, Player* black)
{
    m_impl = new GameImpl(nColumns, nLevels, N, red, black);
}
 
Game::~Game()
{
    delete m_impl;
}
 
bool Game::completed(int& winner) const
{
    return m_impl->completed(winner);
}

bool Game::takeTurn()
{
    return m_impl->takeTurn();
}

void Game::play()
{
    m_impl->play();
}
 
int Game::checkerAt(int c, int r) const
{
    return m_impl->checkerAt(c, r);
}
