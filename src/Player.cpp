// Player.cpp



//========================================================================
// Timer t;                 // create a timer and start it
// t.start();               // start the timer
// double d = t.elapsed();  // milliseconds since timer was last started
//========================================================================

#include <chrono>

class Timer
{
public:
    Timer()
    {
        start();
    }
    void start()
    {
        m_time = std::chrono::high_resolution_clock::now();
    }
    double elapsed() const
    {
        std::chrono::duration<double,std::milli> diff =
        std::chrono::high_resolution_clock::now() - m_time;
        return diff.count();
    }
private:
    std::chrono::high_resolution_clock::time_point m_time;
};

//========================================================================

#include "provided.h"
#include "support.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

class HumanPlayerImpl
{
  public:
    int chooseMove(const Scaffold& s, int N, int color);
};

class BadPlayerImpl
{
  public:
    int chooseMove(const Scaffold& s, int N, int color);
};

class SmartPlayerImpl
{
  public:
    int chooseMove(const Scaffold& s, int N, int color);
  private:
    struct Decision                     // every time of the recursion, return a Decision with
    {
        int winner;                     // who will be the final winner
        int depth;                      // max depth need to win
        int bestCol;                    // best next step
    };
    int min(int a, int b) { if (a < b) return a; else return b; }   // return min(a,b)
    int max(int a, int b) { if (a > b) return a; else return b; }   // return max(a,b)
    Decision determineBestMove(Scaffold s, int N, int color, int lastCol, int recursion, int maxNecessaryRecursion);
};

int HumanPlayerImpl::chooseMove(const Scaffold& s, int N, int color)    // input from user
{
    cout << "Input column :  ";
    int col;
    cin >> col;
    while (col < 1 || col > s.cols() || s.checkerAt(col, s.levels()) != VACANT)
    {
        cout << "  Invalid !\n";
        cout << "Input column :  ";
        cin >> col;
    }
    return col;
    // return -1;                        // since we check the validity, we will not have the chance to return -1
}

int BadPlayerImpl::chooseMove(const Scaffold& s, int N, int color)       // pick a random nonfull column
{
    if (s.numberEmpty() == 0)
        return -1;
    int col;
    srand(static_cast<int>(time(NULL)));
    do
    {
        col = rand() % s.cols() + 1;
    } while (s.checkerAt(col, s.levels()) != VACANT);
    return col;
}

SmartPlayerImpl::Decision SmartPlayerImpl::determineBestMove(Scaffold s, int N, int color, int lastCol, int recursion, int maxNecessaryRecursion )
{
    
    /*
     recursion 深度可变动
     看看recursion这个parameter是不是不需要
     

     */
    Decision decision = {0, 0, 0};
    if ( lastCol != 0 && leadToWin(s, N, decision.winner, lastCol) )   // if lastCol == 0 the leadToWin can be invalid
        return decision;                                               // decision.winner already revised in leadToWin above
    if (recursion >= 8) return {TIE_GAME, 10000,0};                    // return if the depth of recursion is too large
    if (recursion >= maxNecessaryRecursion) return {TIE_GAME, 10000,0};
    if (N == 1) return {color, 1, 1};                                  // condition of Connect 1
    if (N == 2)                                                        // condition of Connect 2
    {
        int a = s.cols() * s.levels() - s.numberEmpty();
        if (a == 0)                                                    // 1st step in the player
        {
            decision = {color, 3, (s.cols() + 1) / 2};
            return decision;
        }
        else if (a == 1)                                               // 2nd step in the player
        {
            int i;
            for (i = 1; i <= s.cols(); i++)
                if (s.checkerAt(i, 1) != VACANT)
                    break;
            if (i > 1)
                decision = {1 - color, 2, i - 1};
            else if (i < s.cols())
                decision = {1 - color, 2, i + 1};
            else if (s.levels() > 1)
                decision = {1 - color, 2, i};
            return decision;
        }
        else if (a == 2)                                               // 3rd step in the game
        {
            int i;
            for (i = 1; i <= s.cols(); i++)
                if (s.checkerAt(i, 1) == color)
                    break;
            if (i > 1 && s.checkerAt(i - 1, 1) == VACANT)
                decision = {color, 1, i - 1};
            else if (i < s.cols() && s.checkerAt(i + 1, 1) == VACANT)
                decision = {color, 1, i + 1};
            else if (s.levels() > 1 && s.checkerAt(i, 2) == VACANT)
                decision = {color, 1, i};
            return decision;
        }
    }
    
    // pick the best choice
    // precedence : color > VACANT > 1 - color > default (-100)
    // then : if color : earlier win > later win
    //        if 1 - color : later loss > earlier loss
    Decision x, best = {-100, 1000, -1};
    int maxIterateCol;
    if (s.numberEmpty() == s.cols() * s.levels())                       // if it's the game's first step, we only need to iterate the left half of the scaffold
        maxIterateCol = (s.cols() + 1) / 2;
    else
        maxIterateCol = s.cols();
    for (int j = 1; j <= maxIterateCol; j++)
    {
        int i;
        if (s.numberEmpty() == s.cols() * s.levels())
            i = (s.cols() + 1) / 2 - (j - 1);
        else
            i = (s.cols() + 1) / 2 + (j / 2) * ( (j % 2 == 0)?1:(-1) );     // so that it searches from the middle
        if (s.checkerAt(i, s.levels()) == VACANT)
        {
            s.makeMove(i, color);
            x = determineBestMove(s, N, 1-color, i, recursion + 1, maxNecessaryRecursion);     // recursion
            
            if (x.winner == color && (best.winner != color || best.depth > x.depth + 1))
            {
                best.winner = x.winner;
                best.depth = x.depth + 1;
                best.bestCol = i;
                maxNecessaryRecursion = min(recursion + x.depth, maxNecessaryRecursion);
            }
            else if ((x.winner == TIE_GAME && (best.winner == 1 - color || best.winner == -100)) || (x.winner == TIE_GAME && best.winner == TIE_GAME && best.depth > x.depth + 1))
            {
                best.winner = x.winner;
                best.depth = x.depth + 1;
                best.bestCol = i;
            }
            else if ((x.winner == 1 - color && best.winner == -100) || (x.winner == 1 - color && best.winner == 1 - color && x.depth + 1 > best.depth))
            {
                best.winner = x.winner;
                best.depth = x.depth + 1;
                best.bestCol = i;
            }
            
            s.undoMove();
        }
    }
    return best; // since best.bestCol is default to be -1, the chooseMove function will return -1 if no move is possible. -1 is included
}

int SmartPlayerImpl::chooseMove(const Scaffold& s, int N, int color)
{
    Timer timer;
    // if (s.numberEmpty() == s.cols() * s.levels()) return (s.cols() + 1) / 2;
    int best = determineBestMove(s, N, color, 0, 0, 8).bestCol;                // the first step
    cerr << "Time elapsed : " << timer.elapsed() << " ms    or    " << timer.elapsed() / 1000.0 << " s." << endl;
    return best;
}

//******************** Player derived class functions *************************

// These functions simply delegate to the Impl classes' functions.
// You probably don't want to change any of this code.

HumanPlayer::HumanPlayer(string nm)
 : Player(nm)
{
    m_impl = new HumanPlayerImpl;
}
 
HumanPlayer::~HumanPlayer()
{
    delete m_impl;
}
 
int HumanPlayer::chooseMove(const Scaffold& s, int N, int color)
{
    return m_impl->chooseMove(s, N, color);
}

BadPlayer::BadPlayer(string nm)
 : Player(nm)
{
    m_impl = new BadPlayerImpl;
}
 
BadPlayer::~BadPlayer()
{
    delete m_impl;
}
 
int BadPlayer::chooseMove(const Scaffold& s, int N, int color)
{
    return m_impl->chooseMove(s, N, color);
}

SmartPlayer::SmartPlayer(string nm)
 : Player(nm)
{
    m_impl = new SmartPlayerImpl;
}
 
SmartPlayer::~SmartPlayer()
{
    delete m_impl;
}
 
int SmartPlayer::chooseMove(const Scaffold& s, int N, int color)
{
    return m_impl->chooseMove(s, N, color);
}
