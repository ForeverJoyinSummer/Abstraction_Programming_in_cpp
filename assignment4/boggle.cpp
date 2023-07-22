/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "backtracking.h"
#include "gridlocation.h"
#include "grid.h"
#include "set.h"
#include "lexicon.h"
#include "SimpleTest.h"
using namespace std;

/*
 * TODO: Implementing the points function
 *
 */
int points(string str) {
    /* TODO: Rate a word by the following point function. */
    return (str.length() < 4) ? 0 : (str.length() - 3);
}

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */

const int drow[8] = {0, 0, 1, -1, 1, 1, -1, -1};
const int dcol[8] = {1, -1, 0, 0, 1, -1, -1, 1};

bool isValidWord(Grid<char> &board,string &word, Lexicon &lex, int row, int col) {
    return board.inBounds(row, col) && lex.containsPrefix(word);
}

void dfs(Grid<char>& board, Lexicon& lex, Vector<Vector<int>> &visited,
         Set<string> &result, int row, int col, string &word) {
    if (!isValidWord(board, word, lex, row, col)) {
        return;
    }
    if (word.size() >= 4 && lex.contains(word)) {
        result.add(word);
    }
    for (int i = 0; i < 8; i++) {
        int new_row = row + drow[i];
        int new_col = col + dcol[i];
        if (board.inBounds(new_row, new_col) && !visited[new_row][new_col]
                && isalpha(board.get(new_row, new_col))) {
            word += tolower(board.get(new_row, new_col));
            visited[new_row][new_col] = true; // Set the new postion as visited
            dfs(board, lex, visited, result, new_row, new_col, word);
            visited[new_row][new_col] = false;
            word.pop_back(); // pop_back() method returns a new string without the last char
        }
    }
}

int scoreBoard(Grid<char>& board, Lexicon& lex) {
    int m = board.numRows(), n = board.numCols();
    Vector<Vector<int>> visited(m, Vector<int>(n));
    Set<string> result;
    string word;
    for (int row = 0; row < m; row++) {
        for (int col = 0; col < n; col++) {
            dfs(board, lex, visited, result, row, col, word);
        }
    }
    int score = 0;
    for (auto &str : result) {
        score += points(str);
    }
    return score;
}

/* * * * * * Test Cases * * * * * */

/* Test helper function to return shared copy of Lexicon. Use to
 * avoid (expensive) re-load of word list on each test case. */
static Lexicon& sharedLexicon() {
    static Lexicon lex("res/EnglishWords.txt");
    return lex;
}

PROVIDED_TEST("Load shared Lexicon, confirm number of words") {
    Lexicon lex = sharedLexicon();
    EXPECT_EQUAL(lex.size(), 127145);
}

PROVIDED_TEST("Test point scoring") {
    EXPECT_EQUAL(points("and"), 0);
    EXPECT_EQUAL(points("quad"), 1);
    EXPECT_EQUAL(points("quint"), 2);
    EXPECT_EQUAL(points("sextet"), 3);
    EXPECT_EQUAL(points("seventh"), 4);
    EXPECT_EQUAL(points("supercomputer"), 10);
}

PROVIDED_TEST("Test scoreBoard, board contains no words, score of zero") {
    Grid<char> board = {{'B','C','D','F'}, //no vowels, no words
                        {'G','H','J','K'},
                        {'L','M','N','P'},
                        {'Q','R','S','T'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 0);
}

PROVIDED_TEST("Test scoreBoard, board contains one word, score of 1") {
    Grid<char> board = {{'C','_','_','_'},
                        {'Z','_','_','_'},
                        {'_','A','_','_'},
                        {'_','_','R','_'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}

PROVIDED_TEST("Test scoreBoard, alternate paths for same word, still score of 1") {
    Grid<char> board = {{'C','C','_','_'},
                        {'C','Z','C','_'},
                        {'_','A','_','_'},
                        {'R','_','R','_'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}

PROVIDED_TEST("Test scoreBoard, small number of words in corner of board") {
    Grid<char> board = {{'L','I','_','_'},
                        {'M','E','_','_'},
                        {'_','S','_','_'},
                        {'_','_','_','_'}};
    Set<string> words = {"SMILE", "LIMES", "MILES", "MILE", "MIES", "LIME", "LIES", "ELMS", "SEMI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()),  2 + 2 + 2 + 1 + 1 + 1 + 1 + 1 + 1);
}

PROVIDED_TEST("Test scoreBoard, full board, small number of words") {
    Grid<char> board = {{'E','Z','R','R'},
                        {'O','H','I','O'},
                        {'N','J','I','H'},
                        {'Y','A','H','O'}};
    Set<string> words = { "HORIZON", "OHIA", "ORZO", "JOHN", "HAJI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 4 + 1 + 1 + 1 + 1);
}

PROVIDED_TEST("Test scoreBoard, full board, medium number of words") {
    Grid<char> board = {{'O','T','H','X'},
                        {'T','H','T','P'},
                        {'S','S','F','E'},
                        {'N','A','L','T'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 76);
}

PROVIDED_TEST("Test scoreBoard, full board, large number of words") {
    Grid<char> board = {{'E','A','A','R'},
                        {'L','V','T','S'},
                        {'R','A','A','N'},
                        {'O','I','S','E'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 234);
}
