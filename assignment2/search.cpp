// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include "search.h"
#include "testing/SimpleTest.h"
#include "error.h"
#include "filelib.h"
#include "strlib.h"
#include "map.h"
#include "set.h"
#include "simpio.h"
#include <fstream>
#include <iostream>
using namespace std;

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
string cleanToken(string s)
{
    int begin = 0;
    int end = s.length() - 1;
    while (begin < s.length() && ispunct(s[begin])) {
        begin += 1;
    }
    while (end >= 0 && ispunct(s[end])) {
        end -= 1;
    }
    string s1 = s.substr(begin, end + 1 - begin);
    int count = 0;
    for (int i = 0; i < s1.length(); i++) {
        if (isalpha(s1[i])) {
            count++;
        }
        if (s1[i] >= 'A' && s1[i] <= 'Z') {
            s1[i] = tolower(s1[i]);
        }
    }
    s = (count > 0) ? s1 : "";
    return s;
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
Set<string> gatherTokens(string bodytext)
{
    Vector<string> split_tokens = stringSplit(bodytext, " ");
    Set<string> tokens;
    for (const string &item: split_tokens) {
        string cleanedItem = cleanToken(item);
        if (cleanedItem != "") {
            tokens.add(cleanedItem);
        }
    }
    return tokens;
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
int buildIndex(string dbfile, Map<string, Set<string>>& index)
{
    ifstream in;

    if (!openFile(in, dbfile)) {
        error("Can not read file named" + dbfile);
    }

    Vector<string> lines;
    readEntireFile(in, lines);

    int count = 0;

    for (int i = 0; i < lines.size(); i += 2) {
        // iterate through all the data
        string newAddress = lines[i];
        string newContent = lines[i + 1];
        Set<string> contents = gatherTokens(newContent);
        for (const string& content: contents) {
            Set<string> newSet = index[content];
            // map[key] will not be affected hen key doesn't contains
            newSet.add(newAddress);
            index.put(content, newSet);
        }
        count++;
    }
    return count;
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
Set<string> findQueryMatches(Map<string, Set<string>>& index, string query)
{
    Set<string> result;
    // TODO: your code here
    Vector<string> expressions = stringSplit(query, " ");
    for (const string& item: expressions) {
        string cleanedItem = cleanToken(item);
        Set<string> itemSet = index[cleanedItem];
        if (startsWith(item, "+") != 0) {
            result.intersect(itemSet);
        }
        else if (startsWith(item, "-") != 0) {
            result.difference(itemSet);
        }
        else {
            result.unionWith(itemSet);
        }
    }

    return result;
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
void searchEngine(string dbfile)
{
    // TODO: your code here
    Map<string, Set<string>> index;
    int numPages = buildIndex(dbfile, index);
    cout << "The number of pages: " << numPages << endl;
    cout << index.size() << " unique words in total" << endl;
    // Enter a loop, in which we need to prompt users to enter keywords.
    while (true) {
        string query = getLine("Enter query sentence (RETURN/ENTER to quit): ");
        if (query.empty()) {
            cout << "All done !" << endl;
            break;
        }
        Set<string> result = findQueryMatches(index, query);
        cout << "Found " << result.size() << " matching pages" << endl;
        cout << result << endl;
        cout << endl;
    }



}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("cleanToken on strings with no punctuation at beginning or end") {
    EXPECT_EQUAL(cleanToken("hello"), "hello");
    EXPECT_EQUAL(cleanToken("WORLD"), "world");
    EXPECT_EQUAL(cleanToken("CS*106B"), "cs*106b");
}

PROVIDED_TEST("cleanToken on strings with some punctuation at beginning and end") {
    EXPECT_EQUAL(cleanToken("/hello/"), "hello");
    EXPECT_EQUAL(cleanToken("~woRLD!"), "world");
}

PROVIDED_TEST("cleanToken on non-word strings (no letters)"){
    EXPECT_EQUAL(cleanToken("106"), "");
    EXPECT_EQUAL(cleanToken("~!106!!!"), "");
}

PROVIDED_TEST("gatherTokens from simple string") {
    Set<string> expected = {"go", "gophers"};
    EXPECT_EQUAL(gatherTokens("go go go gophers"), expected);
}

PROVIDED_TEST("gatherTokens correctly cleans tokens") {
    Set<string> expected = {"i", "love", "cs*106b"};
    EXPECT_EQUAL(gatherTokens("I _love_ CS*106B!"), expected);
}

PROVIDED_TEST("gatherTokens from seuss, 5 unique words, mixed case, punctuation") {
    Set<string> tokens = gatherTokens("One Fish Two Fish *Red* fish Blue fish ** 10 RED Fish?");
    EXPECT_EQUAL(tokens.size(), 5);
    EXPECT(tokens.contains("fish"));
    EXPECT(!tokens.contains("Fish"));
}

PROVIDED_TEST("buildIndex from tiny.txt, 4 pages, 11 unique tokens") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/tiny.txt", index);
    EXPECT_EQUAL(nPages, 4);
    EXPECT_EQUAL(index.size(), 11);
    EXPECT(index.containsKey("fish"));
}

PROVIDED_TEST("findQueryMatches from tiny.txt, single word query") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRed = findQueryMatches(index, "red");
    EXPECT_EQUAL(matchesRed.size(), 2);
    EXPECT(matchesRed.contains("www.dr.seuss.net"));
    Set<string> matchesHippo = findQueryMatches(index, "hippo");
    EXPECT(matchesHippo.isEmpty());
}

PROVIDED_TEST("findQueryMatches from tiny.txt, compound queries") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRedOrFish = findQueryMatches(index, "red fish");
    EXPECT_EQUAL(matchesRedOrFish.size(), 4);
    Set<string> matchesRedAndFish = findQueryMatches(index, "red +fish");
    EXPECT_EQUAL(matchesRedAndFish.size(), 1);
    Set<string> matchesRedWithoutFish = findQueryMatches(index, "red -fish");
    EXPECT_EQUAL(matchesRedWithoutFish.size(), 1);
}


// TODO: add your test cases here
STUDENT_TEST("Several tests for cleanToken") {
    EXPECT_EQUAL(cleanToken("CS61a"), "cs61a");
    EXPECT_EQUAL(cleanToken("a[]B%"), "a[]b");
}

STUDENT_TEST("Several tests for gatherTokens") {
    Set<string> expected1 = {"i", "love", "messi10"};
    Set<string> expected2;
    EXPECT_EQUAL(gatherTokens("I love  #Messi10!"), expected1);
    EXPECT_EQUAL(gatherTokens("!@ #$ %% ##"), expected2);
}

STUDENT_TEST("Runtime tests for buildIndex") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    EXPECT_EQUAL(index.size(), 11);
    TIME_OPERATION(11, buildIndex("res/tiny.txt", index));
}


STUDENT_TEST("Search Engine tests for website.txt") {
    Map<string, Set<string>> bigIndex;
    int webPages = buildIndex("res/webSite.txt", bigIndex);
    EXPECT_EQUAL(webPages, 32);
    EXPECT_EQUAL(bigIndex.size(), 3875);

    searchEngine("res/website.txt");

}
