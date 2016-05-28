// Scaffold.cpp

#include "provided.h"
#include <iostream>
#include <vector>
#include <stack>
using namespace std;

class ScaffoldImpl
{
  public:
    ScaffoldImpl(int nColumns, int nLevels); 
    int cols() const;
    int levels() const;
    int numberEmpty() const;
    int checkerAt(int column, int level) const; 
    void display() const; 
    bool makeMove(int column, int color); 
    int undoMove();
  private:
    int m_col;
    int m_level;
    vector<vector<int>> grid;                                               // record the items
    vector<int> height;                                                     // record the max nonempty level of each column
    stack<int> record;                                                      // record all previous checkers
};

ScaffoldImpl::ScaffoldImpl(int nColumns, int nLevels)                       // initialize m_col, m_level and the grid vector
{
    m_col = nColumns;
    m_level = nLevels;
    grid.resize(m_col);                                             // grid now has m_col empty columns
    for (int i = 0; i < m_col; i++)
        grid[i].resize(m_level,VACANT);                             // column i now has m_level levels
                                                // Now we have an m_col by m_level "array" of doubles.
    height.resize(m_col,0);
}

int ScaffoldImpl::cols() const
{
    return m_col;
}

int ScaffoldImpl::levels() const
{
    return m_level;
}

int ScaffoldImpl::numberEmpty() const                               // calculate number of empty blocks by adding up number of empty blocks in each column.
{
    int sum = 0;
    for (int i = 0; i < m_col; i++)
        sum += m_level - height[i];
    return sum;
}

int ScaffoldImpl::checkerAt(int column, int level) const            // access the item in that block
{
    return grid[column - 1][level - 1];
}

void ScaffoldImpl::display() const                                  // display the game
{
    for (int j = m_level - 1; j >= 0; j--)
    {
        for (int i = 0; i < m_col; i++)
            switch (grid[i][j])
            {
                case RED:
                    cout << "|R";
                    break;
                case BLACK:
                    cout << "|B";
                    break;
                case VACANT:
                    cout << "| ";
                    break;
            }
        cout << "|\n";
    }
    for (int i = 0; i < m_col; i++)                                 // last line
        cout << "+-";
    cout << "+\n";
}

bool ScaffoldImpl::makeMove(int column, int color)                  // put a checker in the exact column. Get the level from height vector
{
    if (column >= 1 && column <= m_col && height[column - 1] < m_level && (color == RED || color == BLACK))
    {
        grid[column - 1][height[column - 1]] = color;
        height[column - 1]++;
        record.push(column - 1);
        return true;
    }
    return false;
}

int ScaffoldImpl::undoMove()                                        // undo the last move, pop the top of the record stack
{
    if (record.empty()) return 0;
    int lastcol = record.top();
    grid[lastcol][height[lastcol] - 1] = VACANT;
    height[lastcol]--;
    record.pop();
    return lastcol + 1;
}



//******************** Scaffold functions *******************************

// These functions simply delegate to ScaffoldImpl's functions.
// You probably don't want to change any of this code.

Scaffold::Scaffold(int nColumns, int nLevels)
{
    m_impl = new ScaffoldImpl(nColumns, nLevels);
}

Scaffold::~Scaffold()
{
    delete m_impl;
}

Scaffold::Scaffold(const Scaffold& other)
{
    m_impl = new ScaffoldImpl(*other.m_impl);
}

Scaffold& Scaffold::operator=(const Scaffold& rhs)
{
    if (this != &rhs)
    {
        Scaffold temp(rhs);
        swap(m_impl, temp.m_impl);
    }
    return *this;
}

int Scaffold::cols() const
{
    return m_impl->cols();
}

int Scaffold::levels() const
{
    return m_impl->levels();
}

int Scaffold::numberEmpty() const
{
    return m_impl->numberEmpty();
}

int Scaffold::checkerAt(int column, int level) const
{
    return m_impl->checkerAt(column, level);
}

void Scaffold::display() const
{
    m_impl->display();
}

bool Scaffold::makeMove(int column, int color)
{
    return m_impl->makeMove(column, color);
}

int Scaffold::undoMove()
{
    return m_impl->undoMove();
}
