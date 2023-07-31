#include "pqheap.h"
#include "error.h"
#include "random.h"
#include "strlib.h"
#include "datapoint.h"
#include "SimpleTest.h"
using namespace std;

const int INITIAL_CAPACITY = 10;
const int NONE = -1; // used as sentinel index

/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
PQHeap::PQHeap() {
    /* TODO: Implement this function. */
    _numAllocated = INITIAL_CAPACITY;
    _elements = new DataPoint[_numAllocated](); // allocated zero'd memory
    _numFilled = 0;
}

/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
PQHeap::~PQHeap() {
    /* TODO: Implement this function. */
    delete[] _elements;
}

/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
void PQHeap::enqueue(DataPoint elem) {
    /* TODO: Implement this function. */
    if (isFull()) {
        realloc();
    }
    _elements[size()] = elem;
    heapifyUp(size());
    _numFilled++;
}

/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
DataPoint PQHeap::peek() const {
    /* TODO: Implement this function. */
    if (isEmpty()) {
        return {"", 0};
    }
    return _elements[0];
}

/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
DataPoint PQHeap::dequeue() {
    /* TODO: Implement this function. */
    if (isEmpty()) {
        return {"", 0};
    }
    DataPoint res = _elements[0];
    _elements[0] = _elements[size() - 1];
    _numFilled--;
    heapifyDown(0);
    return res;
}

//by Ephmeral
bool PQHeap::isFull() const {
    return _numFilled >= _numAllocated;
}

void PQHeap::realloc() {
    if (_numFilled < _numAllocated) {
        return;
    }

    //DataPoint* elem_new = (DataPoint*)malloc(sizeof(DataPoint) * _numAllocated);
    DataPoint* elem_new = new DataPoint[_numAllocated * 2];
    for (int i = 0; i < _numAllocated; ++i) {
        elem_new[i] = _elements[i];
    }
    delete[] _elements;
    _elements = elem_new;
    _numAllocated <<= 1;
}

/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
bool PQHeap::isEmpty() const {
    /* TODO: Implement this function. */
    return _numFilled == 0;
}

/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
int PQHeap::size() const {
    /* TODO: Implement this function. */
    return _numFilled;
}

/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
void PQHeap::clear() {
    /* TODO: Implement this function. */
    _numFilled = 0;
}

bool PQHeap::isValidIndex(int index) {
    return index > 0 && index <= size();
}

void PQHeap::swap(int indexA, int indexB) {
    DataPoint tmp = _elements[indexA];
    _elements[indexA] = _elements[indexB];
    _elements[indexB] = tmp;
}

int PQHeap::priorer(int indexA, int indexB) {
    if (_elements[indexA].priority < _elements[indexB].priority) {
        return indexA;
    }
    return indexB;
}

int PQHeap::getChild(int parent) {
    int leftChild = getLeftChildIndex(parent);
    int rightChild = getRightChildIndex(parent);
    if (isValidIndex(leftChild) && isValidIndex(rightChild)) {
        return priorer(parent, priorer(leftChild, rightChild));
    }
    else if (isValidIndex(leftChild)){
        return priorer(parent, leftChild);
    }
    else if (isValidIndex(rightChild)) {
        return priorer(parent, rightChild);
    }
    return parent;
}


void PQHeap::heapifyUp(int index) {
    while (index > 0) {
        int parent = getParentIndex(index);
        if (priorer(index, parent) == parent) {
            break;
        }
        swap(parent, index);
        index = parent;
    }
}

void PQHeap::heapifyDown(int index) {
    while (index < size()) {
        int child = getChild(index);
        if (index == child) break;
        swap(index, child);
        index = child;
    }
}
/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
void PQHeap::printDebugInfo(string msg) const {
    cout << msg << endl;
    for (int i = 0; i < size(); i++) {
        cout << "[" << i << "] = " << _elements[i] << endl;
    }
}

/*
 * We strongly recommend implementing this helper function, which
 * should traverse the heap array and ensure that the heap property
 * holds for all elements in the array. If elements are found that
 * violate the heap property, an error should be thrown.
 */
void PQHeap::validateInternalState() const {
    /* TODO: Implement this function. */
    /*
     * If there are more elements than spots in the array, we have a problem.
     */
    if (_numFilled > _numAllocated) error("Too many elements in not enough space!");

    /* Loop over the elements in the array and compare priority of
     * neighboring elements. If current element has larger priority
     * than the previous, array elements are out of order with respect
     * to our intention. Use error to report this problem.
     */
    for (int i = 1; i < size(); i++) {
        if (_elements[i].priority < _elements[getParentIndex(i)].priority) {
            printDebugInfo("validateInternalState");
            error("PQArray has elements out of order at index " + integerToString(i));
        }
    }
}

/*
 * We strongly recommend implementing this helper function, which
 * calculates the index of the element that is the parent of the
 * specified child index. If this child has no parent, return
 * the sentinel value NONE.
 */
int PQHeap::getParentIndex(int child) const {
    /* TODO: Implement this function. */
    if (child == 0) {
        return -1;
    }
    return (child - 1) >> 1;
}

/*
 * We strongly recommend implementing this helper function, which
 * calculates the index of the element that is the left child of the
 * specified parent index. If this parent has no left child, return
 * the sentinel value NONE.
 */
int PQHeap::getLeftChildIndex(int parent) const {
    /* TODO: Implement this function. */
    int leftChild = parent * 2 + 1;
    if (leftChild >= size()) {
        return -1;
    }
    return leftChild;
}

/*
 * We strongly recommend implementing this helper function, which
 * calculates the index of the element that is the right child of the
 * specified parent index. If this parent has no right child, return
 * the sentinel value NONE.
 */
int PQHeap::getRightChildIndex(int parent) const {
    /* TODO: Implement this function. */
    int rightChild = parent * 2 + 2;
    if (rightChild >= size()) {
        return -1;
    }
    return rightChild;
}
/* * * * * * Test Cases Below This Point * * * * * */

/* TODO: Add your own custom tests here! */
STUDENT_TEST("Several Tests for PQHeap.") {
    PQHeap pq;

    EXPECT(pq.isEmpty());
    EXPECT(pq.size() == 0);
    DataPoint point = { "Programming Abstractions", 106 };

    pq.enqueue(point);
    pq.clear();
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);

    Vector<DataPoint> input = {
        { "R", 4 }, { "A", 5 }, { "B", 3 }, { "K", 7 }, { "G", 2 },
        { "V", 9 }, { "T", 1 }, { "O", 8 }, { "S", 6 } };

    for (DataPoint dp : input) {
        pq.enqueue(dp);
        pq.validateInternalState();
    }
    EXPECT_EQUAL(pq.size(), 9);
    EXPECT_EQUAL(pq.isEmpty(), false);
    while (!pq.isEmpty()) {
        pq.dequeue();
        pq.validateInternalState();
    }
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}




/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("PQHeap example from writeup, validate each step") {
    PQHeap pq;
    Vector<DataPoint> input = {
        { "R", 4 }, { "A", 5 }, { "B", 3 }, { "K", 7 }, { "G", 2 },
        { "V", 9 }, { "T", 1 }, { "O", 8 }, { "S", 6 } };

    pq.validateInternalState();
    for (DataPoint dp : input) {
        pq.enqueue(dp);
        pq.validateInternalState();
    }
    while (!pq.isEmpty()) {
        pq.dequeue();
        pq.validateInternalState();
    }
}
