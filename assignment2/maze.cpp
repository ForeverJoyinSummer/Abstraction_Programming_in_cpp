// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include "maze.h"
#include "testing/SimpleTest.h"
#include "error.h"
#include "filelib.h"
#include "grid.h"
#include "mazegraphics.h"
#include "set.h"
#include "stack.h"
#include "queue.h"
#include "vector.h"
#include <fstream>
#include <iostream>
using namespace std;


/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
const int drow[4] = {1, 0, -1, 0};
const int dcol[4] = {0, -1, 0, 1};

Set<GridLocation> generateValidMoves(Grid<bool>& maze, GridLocation cur) {
    Set<GridLocation> neighbors;
    /* TODO: Fill in the remainder of this function. */
    if (!maze.inBounds(cur) || maze.get(cur) == false) {
        return neighbors;
    }

    for (int i = 0; i < 4; i++) {
        int new_row = cur.row + drow[i];
        int new_col = cur.col + dcol[i];
        GridLocation new_pos = {new_row, new_col};
        if (maze.inBounds(new_pos) && maze.get(new_pos) == true) {
            neighbors.add(new_pos);
        }
    }
    return neighbors;
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
void validatePath(Grid<bool>& maze, Stack<GridLocation> path) {
    GridLocation mazeExit = {maze.numRows()-1,  maze.numCols()-1};
    GridLocation mazeEntry = {0, 0};
    if (path.peek() != mazeExit) {
        error("Path does not end at maze exit");
    }
    /* TODO: Fill in the remainder of this function. */

    /* If you find a problem with the path, call error() to report it.
     * If the path is a valid solution, then this function should run to completion
     * without throwing any errors.
     */
    Set<GridLocation> visited;
    GridLocation init = path.pop();
    while (!path.isEmpty()) {
        GridLocation cur = path.pop();
        if (visited.contains(cur)) {
            error("This path has been visited!");
        }
        visited.add(cur);
        Set<GridLocation> initNeighbors = generateValidMoves(maze, init);
        if (!initNeighbors.contains(cur)) {
            error("There is not a valid move!");
        }
        init = cur;

        if (path.isEmpty() && cur != mazeEntry) {
            error("The maze does not start from the entry!");
        }
    }

}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */


Stack<GridLocation> solveMaze(Grid<bool>& maze) {
    MazeGraphics::drawGrid(maze);
    /* TODO: Fill in the remainder of this function. */
    GridLocation mazeExit = {maze.numRows()-1,  maze.numCols()-1};
    GridLocation mazeEntry = {0, 0};
    Stack<GridLocation> path = {mazeEntry};
    Set<GridLocation> visited;
    Queue<Stack<GridLocation>> pathQueue = {path};
    while (!pathQueue.isEmpty()) {
        path = pathQueue.dequeue();
        GridLocation cur = path.peek();
        MazeGraphics::highlightPath(path, "GREEN", 1);
        // If already arrives the exit
        if (cur == mazeExit) {
            // search ended
            break;
        }
        visited.add(cur);
        Set<GridLocation> neighbors = generateValidMoves(maze, cur);
        for (auto neighbor : neighbors) {
            if (!visited.contains(neighbor)) {
                path.push(neighbor);
                pathQueue.enqueue(path);
                path.pop();
            }
        }

    }

    return path;
}

/*
 * The given readMazeFile function correctly reads a well-formed
 * maze from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readMazeFile(string filename, Grid<bool>& maze) {
    /* The following code reads data from the file into a Vector
     * of strings representing the lines of the file.
     */
    ifstream in;

    if (!openFile(in, filename))
        error("Cannot open file named " + filename);

    Vector<string> lines;
    readEntireFile(in, lines);

    /* Now that the file data has been read into the Vector, populate
     * the maze grid.
     */
    int numRows = lines.size();        // rows is count of lines
    int numCols = lines[0].length();   // cols is length of line
    maze.resize(numRows, numCols);     // resize grid dimensions

    for (int r = 0; r < numRows; r++) {
        if (lines[r].length() != numCols) {
            error("Maze row has inconsistent number of columns");
        }
        for (int c = 0; c < numCols; c++) {
            char ch = lines[r][c];
            if (ch == '@') {        // wall
                maze[r][c] = false;
            } else if (ch == '-') { // corridor
                maze[r][c] = true;
            } else {
                error("Maze location has invalid character: '" + charToString(ch) + "'");
            }
        }
    }
}

/* The given readSolutionFile correctly reads a path from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readSolutionFile(string filename, Stack<GridLocation>& soln) {
    ifstream in;

    if (!openFile(in, filename)) {
        error("Cannot open file named " + filename);
    }

    if (!(in >> soln)) {// if not successfully read
        error("Maze solution did not have the correct format.");
    }
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("generateValidMoves on location in the center of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation center = {1, 1};
    Set<GridLocation> expected = {{0, 1}, {1, 0}, {1, 2}, {2, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}

PROVIDED_TEST("generateValidMoves on location on the side of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation side = {0, 1};
    Set<GridLocation> expected = {{0,0}, {0,2}, {1, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, side), expected);
}

PROVIDED_TEST("generateValidMoves on corner of 2x2 grid with walls") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    GridLocation corner = {0, 0};
    Set<GridLocation> expected = {{1, 0}};

    EXPECT_EQUAL(generateValidMoves(maze, corner), expected);
}

PROVIDED_TEST("validatePath on correct solution") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> soln = { {0 ,0}, {1, 0}, {1, 1} };

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on correct solution loaded from file for medium maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/5x7.maze", maze);
    readSolutionFile("res/5x7.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on correct solution loaded from file for large maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/25x33.maze", maze);
    readSolutionFile("res/25x33.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on invalid path should raise error") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> not_end_at_exit = { {1, 0}, {0, 0} };
    Stack<GridLocation> not_begin_at_entry = { {1, 0}, {1, 1} };
    Stack<GridLocation> go_through_wall = { {0 ,0}, {0, 1}, {1, 1} };
    Stack<GridLocation> teleport = { {0 ,0}, {1, 1} };
    Stack<GridLocation> revisit = { {0 ,0}, {1, 0}, {0, 0}, {1, 0}, {1, 1} };

    EXPECT_ERROR(validatePath(maze, not_end_at_exit));
    EXPECT_ERROR(validatePath(maze, not_begin_at_entry));
    EXPECT_ERROR(validatePath(maze, go_through_wall));
    EXPECT_ERROR(validatePath(maze, teleport));
    EXPECT_ERROR(validatePath(maze, revisit));
}

PROVIDED_TEST("solveMaze on file 5x7") {
    Grid<bool> maze;
    readMazeFile("res/5x7.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("solveMaze on file 21x23") {
    Grid<bool> maze;
    readMazeFile("res/21x23.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

// TODO: add your test cases here
STUDENT_TEST("generateValidMoves on location in the center of 5x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true, false, true},
                       {true, true, false, false, true},
                       {true, true, true, false, true}};
    GridLocation center = {1, 1};
    Set<GridLocation> expected = {{0, 1}, {1, 0}, {2, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}

STUDENT_TEST("validatePath , check is true") {
    Grid<bool> maze = {{true, true, true,true},
                       {true, true, false,true},
                       {true, true, true,true},
                       {false,false,true,true}};
    Stack<GridLocation> soln1 = { {0,0}, {3, 3}};
    Stack<GridLocation> soln2 = { {0,0},{1,0},{2,0},{1,2},{2,2},{3,2}, {3, 3} };
    Stack<GridLocation> soln3 = { {0,0},{1,0},{2,0},{2,1},{2,2},{3,2}, {3, 3} };
    EXPECT_ERROR(validatePath(maze, soln1));
    EXPECT_ERROR(validatePath(maze, soln2));
    EXPECT_NO_ERROR(validatePath(maze, soln3));
}
STUDENT_TEST("solveMaze on file 13x39") {
    Grid<bool> maze;
    readMazeFile("res/13x39.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMaze on file 17x37") {
    Grid<bool> maze;
    readMazeFile("res/17x37.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMaze on file 21x23") {
    Grid<bool> maze;
    readMazeFile("res/21x23.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMaze on file 21x25") {
    Grid<bool> maze;
    readMazeFile("res/21x25.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMaze on file 21x35") {
    Grid<bool> maze;
    readMazeFile("res/21x35.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMaze on file 25x15") {
    Grid<bool> maze;
    readMazeFile("res/25x15.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMaze on file 25x33") {
    Grid<bool> maze;
    readMazeFile("res/25x33.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMaze on file 33x41") {
    Grid<bool> maze;
    readMazeFile("res/33x41.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}
