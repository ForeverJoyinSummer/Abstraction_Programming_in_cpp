// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include<numeric>
#include "voting.h"
#include "vector.h"
#include <cmath>
#include "SimpleTest.h"
#include "map.h"
using namespace std;

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior

// Compute the critical votes in blocks at a certain index

void computePowerIndexesRec(const Vector<int> &blocks, int targetBlock, int &targetPower,
                           int totalVotes, int index) {
    // 利用剪枝优化无效分支
    int swing = accumulate(blocks.begin(), blocks.end(), 0) / 2;
    if (totalVotes > swing)
        return;

    // 1. 当前联盟：关于目标选区的两个问题
    if (totalVotes <= swing && totalVotes + blocks[targetBlock] > swing)
        targetPower++;

    // 2. 回溯法：选择 / 探索 / 取消选择模式
    for (int i = index; i < blocks.size(); i++) {
        if (i != targetBlock) { // 跳过目标选区
            computePowerIndexesRec(blocks, targetBlock, targetPower, totalVotes + blocks[i], i + 1);
        }
    }
}


Vector<int> computePowerIndexes(Vector<int>& blocks) {
    Vector<int> result(blocks.size(), 0);

    // 利用 Memorization 优化重复项，也可以放在递归过程中
    Map<int, int> cachedResult;
    for (int i = 0; i < blocks.size(); i++) {
        int power = 0;
        if (cachedResult.containsKey(blocks[i])) {
            result[i] = cachedResult[blocks[i]];
        } else {
            computePowerIndexesRec(blocks, i, power, 0, 0);
            cachedResult[blocks[i]] = power;
            result[i] = power;
        }
    }

    // 2. 生成 Banzhaf Power Index
    double total = accumulate(result.begin(), result.end(), 0);
    for (int i = 0; i < result.size(); i++) {
        result[i] = result[i] / total * 100;
    }

    return result;
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("Test power index, blocks 50-49-1") {
    Vector<int> blocks = {50, 49, 1};
    Vector<int> expected = {60, 20, 20};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks Hempshead 1-1-3-7-9-9") {
    Vector<int> blocks = {1, 1, 3, 7, 9, 9};
    Vector<int> expected = {0, 0, 0, 33, 33, 33};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks CA-TX-NY 55-38-39") {
    Vector<int> blocks = {55, 38, 29};
    Vector<int> expected = {33, 33, 33};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks CA-TX-GA 55-38-16") {
    Vector<int> blocks = {55, 38, 16};
    Vector<int> expected = {100, 0, 0};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks EU post-Nice") {
    // Estonia is one of those 4s!!
    Vector<int> blocks = {29,29,29,29,27,27,14,13,12,12,12,12,12,10,10,10,7,7,7,7,7,4,4,4,4,4,3};
    Vector<int> expected = {8, 8, 8, 8, 7, 7, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Time power index operation") {
    Vector<int> blocks;
    for (int i = 0; i < 15; i++) {
        blocks.add(randomInteger(1, 10));
    }
    TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
}







