/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include "recursion.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * This function draws a filled, black triangle on the provided GWindow
 * that is defined by the corner GPoints one, two, and three. This
 * function has been provided for you and does not need to be modified.
 */
void fillBlackTriangle(GWindow& window, GPoint one, GPoint two, GPoint three) {
    window.setColor("black");
    window.fillPolygon( {one, two, three} );
}

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
int drawSierpinskiTriangle(GWindow& window, GPoint one, GPoint two, GPoint three, int order) {
    /* TODO: Implement this function. */
    if (order == 0) {
        fillBlackTriangle(window, one, two, three);
        return 1;
    }
    else {
        GPoint mid12((one.x + two.x) / 2, (one.y + two.y) / 2);
        GPoint mid13((one.x + three.x) / 2, (one.y + three.y) / 2);
        GPoint mid23((two.x + three.x) / 2, (two.y + three.y) / 2);
        return drawSierpinskiTriangle(window, one, mid12, mid13, order - 1)
             + drawSierpinskiTriangle(window, mid12, two, mid23, order - 1)
             + drawSierpinskiTriangle(window, mid13, mid23, three, order - 1);

    }
}


/* * * * * * Test Cases * * * * * */

/*
 * Do not modify the code beyond this line! There are no
 * unit tests for this problem. You should do all testing
 * via the interactive demo.
 */

PROVIDED_TEST("Test fractal drawing interactively using graphical demo") {
    runInteractiveDemo();
}

