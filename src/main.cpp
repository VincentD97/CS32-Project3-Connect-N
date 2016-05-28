// main.cpp

#include "provided.h"
#include <iostream>
#include <cassert>
using namespace std;

void doScaffoldTests()
{
    cerr << "----------- Testing  Scaffold  tests ! -----------" << endl;

    Scaffold s(3, 2);                                                       // construct a new Scaffold
    assert(s.cols() == 3  &&  s.levels() == 2  && s.numberEmpty() == 6);    // test cols(), levels(), and numberEmpty()
    assert(s.checkerAt(2, 1) == VACANT);                                    // test checkerAt(). At beginning all blocks are VACANT
    s.display();                                                            // test display()
    assert(!s.makeMove(0, RED) && s.makeMove(1, RED) && s.makeMove(3, RED) && !s.makeMove(4, RED));    // makeMove should return false if column is out of boundary, true otherwise
    assert(!s.makeMove(2, 3) && !s.makeMove(2, VACANT) && s.makeMove(2, RED) && s.makeMove(2, BLACK));// makeMove should return false if color != red or black, true otherwise
    s.display();                                                            // now s has a RED in 1st col, a RED in 3rd col, and a RED and a BLACK in 2nd col
    assert(!s.makeMove(2, RED));                                            // since col 2 is full, return false
    assert(s.numberEmpty() == 2);                                           // test numberEmpty()
    assert(s.checkerAt(1, 1) == RED && s.checkerAt(2, 2) == BLACK && s.checkerAt(1, 2) == VACANT);                                                            // test checkerAt()
    assert(s.undoMove() == 2 && s.undoMove() == 2);                         // test undoMove()
    s.display();                                                            // now col 2 is empty
    cerr << "--------- Passed all  Scaffold  tests ! ----------" << endl;
}

void doPlayerTests()
{
    cerr << "----------- Testing   Player   tests ! -----------" << endl;

    Scaffold s(3, 2);
    //---------------------- Test HumanPlayer ----------------------
    HumanPlayer hp("Marge");                                                // construct a HumanPlayer
    assert(hp.name() == "Marge" && hp.isInteractive());                     // test name() and isInteractive()
    s.makeMove(1, RED);
    s.makeMove(1, BLACK);
    cerr << "=========" << endl;
    int n = hp.chooseMove(s, 3, RED);
    cerr << "=========" << endl;                                            // nothing more may be written except that between the two rows of equal signs.
    assert(n == 2  ||  n == 3);                                             // since col 1 is full, and input must be inside the range, the User input can only be 2 or 3
    
    //---------------------- Test BadPlayer ----------------------
    BadPlayer bp("Homer");                                                  // construct a BadPlayer
    assert(bp.name() == "Homer" && !bp.isInteractive());                    // test name() and isInteractive()
    n = bp.chooseMove(s, 3, RED);
    assert(n == 2  ||  n == 3);                                             // since col 1 is full, and pick must be inside the range, the BadPlayer's pick can only be 2 or 3
    
    //---------------------- Test SmartPlayer ----------------------
    
    SmartPlayer sp("Lisa");                                                  // construct a SmartPlayer
    assert(sp.name() == "Lisa" && !sp.isInteractive());                    // test name() and isInteractive()
    Scaffold s1(4, 3);
    s1.makeMove(1, RED); s1.makeMove(1, BLACK); s1.makeMove(3, RED); s1.makeMove(4, BLACK);
    //  | | | | |
    //  |B| | | |
    //  |R| |R|B|
    //  +-+-+-+-+
    assert(sp.chooseMove(s1, 3, RED) == 2 && sp.chooseMove(s1, 3, BLACK) == 2);// if I can win in one step I will do that. If I will lose in one step I will avoid that
    s1.makeMove(4, BLACK);
    assert(sp.chooseMove(s1, 3, BLACK) == 4);                               // If the oppoent is going to win I can also win before him
    Scaffold s2(4, 3);
    s2.makeMove(1, BLACK); s2.makeMove(1, BLACK); s2.makeMove(1, RED); s2.makeMove(3, RED); s2.makeMove(4, BLACK); s2.makeMove(3, RED); s2.makeMove(3, BLACK);
    //  |R| |B| |
    //  |B| |R| |
    //  |B| |R|B|
    //  +-+-+-+-+
    assert(sp.chooseMove(s2, 3, BLACK) == 4);                               // If I'm going to win I will try to delay the loss
    cerr << "--------- Passed all   Player   tests ! ----------" << endl;
}

void doGameTests()
{
    cerr << "----------- Testing    Game    tests ! -----------" << endl;
    
    BadPlayer bp1("Bart");                                                  // construct BadPlayers
    BadPlayer bp2("Homer");
    Game g(2, 2, 2, &bp1, &bp2);                                            // construct a game
    int winner;
    assert(!g.completed(winner));                                           // test completed
    g.takeTurn();  // Red's first move                                      // test takeTurn()
    assert(!g.completed(winner)  && (g.checkerAt(1, 1) == RED  ||  g.checkerAt(2, 1) == RED));// game has not completed, and the 1st lvl of either col1 or col2 is red
    g.takeTurn();  // Black's first move
    assert(!g.completed(winner));                                           // not completed
    g.takeTurn();  // Red's second move; Red must win
    assert(g.completed(winner)  &&  winner == RED);                         // red wins
    assert(g.checkerAt(1, 1) == RED || g.checkerAt(1, 1) == BLACK);         // test checkerAt
    
    BadPlayer bp("Bad");                                                    // test the given cases for time limit
    SmartPlayer sp1("Smart");
    SmartPlayer sp2("Smarter?");
    Game g1(3, 5, 5, &sp1, &sp2);
    g1.play();
    Game g2(4, 4, 3, &sp1, &sp2);
    g2.play();
    Game g3(5, 3, 2, &sp1, &sp2);
    g3.play();
    Game g4(5, 2, 5, &sp1, &sp2);
    g4.play();
    Game g5(6, 2, 3, &sp1, &sp2);
    g5.play();
    Game g6(10, 2, 1, &sp1, &sp2);
    g6.play();
    Game g7(10, 2, 2, &sp1, &sp2);
    g7.play();
    Game g8(10, 1, 3, &sp1, &sp2);
    g8.play();
    
    
    cerr << "--------- Passed all    Game    tests ! ----------" << endl;

}

int main()
{
    // doScaffoldTests();
    // doPlayerTests();
    // doGameTests();
    // SmartPlayer sp("Mr Smart");
    // HumanPlayer hp1("Vincent");
    // HumanPlayer hp2("Cecilia");
    // Game g(6, 4, 4, &sp, &hp1);
    // g.play();

    cout << "==========================  Connect N  ==========================\n\n";
    int col, level, N, numberOfHuman;
    cout << "How many columns ? (1 - 10)     ";
    cin >> col;
    while (col < 1 || col > 10)
    {
        cout << "Invalid !                       ";
        cin >> col;
    }
    cout << "How many levels  ? (1 - 6)      ";
    cin >> level;
    while (level < 1 || level > 6)
    {
        cout << "Invalid !                       ";
        cin >> level;
    }
    cout << "N ?                             ";
    cin >> N;
    while (N < 1)
    {
        cout << "Invalid !                       ";
        cin >> N;
    }
    cout << "Number of human players ?       ";
    cin >> numberOfHuman;
    while (numberOfHuman < 0 || numberOfHuman > 2)
    {
        cout << "Invalid !                       ";
        cin >> numberOfHuman;
    }
    
    if (numberOfHuman == 0)
    {
        string firstAI, secondAI;
        cout << "Do you want the 1st AI to be stupid / smart ? (input \"stupid\" or \"smart\")   ";
        cin >> firstAI;
        while (firstAI != "stupid" && firstAI != "smart")
        {
            cout << "Invalid !                                                                   ";
            cin >> firstAI;
        }
        cout << "Do you want the 2nd AI to be stupid / smart ? (input \"stupid\" or \"smart\")   ";
        cin >> secondAI;
        while (secondAI != "stupid" && secondAI != "smart")
        {
            cout << "Invalid !                                                                   ";
            cin >> secondAI;
        }
        BadPlayer player1bad("Mr Stupid");
        SmartPlayer player1smart("Mr Smart");
        BadPlayer player2bad("Mrs Stupid");
        SmartPlayer player2smart("Mrs Smart");
        if (firstAI == "stupid")
        {
            if (secondAI == "stupid")
            {
                Game g(col, level, N, &player1bad, &player2bad);
                g.play();
            }
            else
            {
                Game g(col, level, N, &player1bad, &player2smart);
                g.play();
            }
        }
        else
        {
            if (secondAI == "stupid")
            {
                Game g(col, level, N, &player1smart, &player2bad);
                g.play();
            }
            else
            {
                Game g(col, level, N, &player1smart, &player2smart);
                g.play();
            }
        }
    }
    else if (numberOfHuman == 1)
    {
        string AI, firstOrNot;
        cout << "Do you want the AI to be stupid / smart ? (input \"stupid\" or \"smart\")   ";
        cin >> AI;
        while (AI != "stupid" && AI != "smart")
        {
            cout << "Invalid !                                                               ";
            cin >> AI;
        }
        cout << "Do you want to go first ? (y/n) ";
        cin >> firstOrNot;
        while (firstOrNot != "y" && firstOrNot != "n")
        {
            cout << "Invalid !                       ";
            cin >> firstOrNot;
        }
        cout << "What's your name ?              ";
        string name;
        cin.ignore(INT_MAX, '\n');
        getline(cin, name);
        BadPlayer badAI("Mr Stupid");
        SmartPlayer smartAI("Mr Smart");
        HumanPlayer human(name);
        if (firstOrNot == "y")
        {
            if (AI == "stupid")
            {
                Game g(col, level, N, &human, &badAI);
                g.play();
            }
            else
            {
                Game g(col, level, N, &human, &smartAI);
                g.play();
            }
        }
        else
        {
            if (AI == "stupid")
            {
                Game g(col, level, N, &badAI, &human);
                g.play();
            }
            else
            {
                Game g(col, level, N, &smartAI, &human);
                g.play();
            }
        }
    }
    else if (numberOfHuman == 2)
    {
        cout << "What's the first player's name ?       ";
        string name1;
        cin.ignore(INT_MAX, '\n');
        getline(cin, name1);
        cout << "What's the second player's name ?      ";
        string name2;
        getline(cin, name2);
        HumanPlayer human1(name1);
        HumanPlayer human2(name2);
        Game g(col, level, N, &human1, &human2);
        g.play();
    }
}
