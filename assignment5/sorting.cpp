#include "listnode.h"
#include "vector.h"
#include "SimpleTest.h"
using namespace std;

/*
 * TODO: Replace this comment with a descriptive function
 * header comment about your implementation of the function.
 */

void partition(ListNode *front, ListNode *&less, ListNode *&equal, ListNode *&greater){
    equal = front;
    less = greater = nullptr;
    front = front->next;
    equal->next = nullptr;

    while(front){
        //将下一元素的地址保存在next指针中
        ListNode* next = front->next;
        if(front->data < equal->data){
            //将front插入less链表的头部
            ListNode *node = less;
            less = front;
            front->next = node;
        }else if(front->data > equal->data){
            //将front插入greater链表的头部
            ListNode *node = greater;
            greater = front;
            front->next = node;
        }else{
            //将front插入equal链表的头部
            ListNode *node = equal;
            equal = front;
            front->next = node;
        }
        //front指向下一元素
        front = next;
    }

}



//将已排好序的三个链表重新链接到front链表上
void concatenate(ListNode *&front, ListNode *less, ListNode *equal, ListNode *greater){
    if(less){
        front = less;
        while(less->next){
            less = less->next;
        }
        //now less->next is nullptr;
        // linklist equal can't be empty
        less->next = equal;
    }else{
        front = equal;
    }
    while(equal->next){
        equal = equal->next;
    }
    // now eqaul->next is nullptr
    equal->next = greater;
}

/*
 * TODO: Replace this comment with a descriptive function
 * header comment about your implementation of the function.
 */
void quickSort(ListNode*& front) {
    if(!front || !front->next){//若链表为空或只有一个元素
        return;
    }
    ListNode *less, *equal, *greater;
    //链表至少有两个元素
    partition(front, less, equal, greater);
    quickSort(less);
    quickSort(greater);
    concatenate(front, less, equal, greater);
}

/* * * * * * Test Code Below This Point * * * * * */

/* TODO: Write your own student tests below to test your sorting helper
 * functions and overall sorting algorithm correctness. We have provided
 * the skeleton of some tests below to help you get started with this
 * process.
 */

/*
 * We have provided this utility function as a convenience. This
 * function traverses the linked list represented by the provided
 * front pointer and prints out the data stored in the list along
 * the way in a convenient format. You may find it to be helpful to
 * use this function as a debugging aid.
 */
void printList(ListNode* front) {
   cout << "{";
   for (ListNode *cur = front; cur != nullptr; cur = cur->next) {
       cout << cur->data;
       if (cur->next != nullptr){
           cout << ", ";
       }
   }
   cout << "}" << endl;
}

/*
 * This utility function deallocates the memory for all the nodes in a
 * given linked list. It can be used to recycle the memory allocated
 * during a test case and avoid memory leaks.
 */
void deallocateList(ListNode* front) {
    /* TODO: Implement this function. */
    while(front != nullptr) {
        ListNode* tmp = front;
        front = front->next;
        delete tmp;
    }
}

/*
 * This utility function is given a vector of values and constructs
 * an equivalent linked list containing the same values in the same order.
 * It can be used to construct a linked list to use as an input to
 * a function being tested.
 */

ListNode* createList(Vector<int> values){
    /* TODO: Implement this function. */
    if(values.isEmpty()) {
        return nullptr;
    }
    ListNode* head = new ListNode(values[0], nullptr);
    ListNode* cur = head;
    for (int i = 1; i < values.size(); i++) {
        cur->next = new ListNode(values[i], nullptr);
        cur = cur->next;
    }
    return head;
}

/*
 * This utility function compares a linked list and a vector for
 * equivalence and returns true if both contain the same values in the
 * same order. It can be used to confirm a linked list's contents match
 * the expected.
 */
bool areEquivalent(ListNode* front, Vector<int> v){
    /* TODO: Implement this function. */
    if (front == nullptr) {
        if (v.isEmpty()) {
            return true;
        }
        else {
            return false;
        }
    }
    else {
        // front != null
        if (v.isEmpty()) {
            return false;
        }
        else {
            if (front->data == v[0]) {
                return areEquivalent(front->next, v.subList(1));
            }
            else {
                return false;
            }
        }
    }
}


STUDENT_TEST("This is a skeleton to demonstrate an end-to-end student test."){
    /* Initialize vector to sequence of values */
    Vector<int> values = {1, 2, 3, 4};
    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}

PROVIDED_TEST("This tests some of the testing helper functions you are recommended to implement."){
    /* Creates a small test list containing the values 1->2->3. */
    ListNode* testList = nullptr;
    testList = new ListNode(3, testList);
    testList = new ListNode(2, testList);
    testList = new ListNode(1, testList);

    /* Test equality comparison function. */
    EXPECT(areEquivalent(testList, {1, 2, 3}));
    EXPECT(!areEquivalent(testList, {1, 2}));
    EXPECT(!areEquivalent(testList, {1, 2, 3, 4}));
    EXPECT(!areEquivalent(testList, {1, 3, 2}));

    /* Test create list function. */
    ListNode* studentList = createList({1, 2, 3, 4});
    printList(studentList);
    ListNode* cur = studentList;
    for (int i = 1; i <= 4; i++){
        EXPECT(cur != nullptr);
        EXPECT_EQUAL(cur->data, i);
        cur = cur->next;
    }

    /* Test deallocate list functions. There should be no memory leaks from this test. */
    deallocateList(studentList);
    deallocateList(testList);
}

/*
 * The provided test case demonstrates a comparsion between the same
 * algorithm (quicksort) operating on a linked list versus a vector.
 */
PROVIDED_TEST("Time linked list quicksort vs vector quicksort") {
    int startSize = 50000;

    for(int n = startSize; n < 10*startSize; n *= 2) {
        Vector<int> v(n);
        ListNode* list = nullptr;

        /* Create linked list and vector with the same random sequence. */
        for (int i = n-1; i >= 0; i--) {
            v[i] = randomInteger(-10000, 10000);
            list = new ListNode(v[i], list);
        }

        /* NOTE: This test does not check correctness of the linked list sort
         * function. It only times the two operations to compare relative speed. */
        TIME_OPERATION(n, quickSort(list));
        TIME_OPERATION(n, v.sort());        /* Standard vector sort operation is backed
                                               with quicksort algorithm. */

        deallocateList(list);
    }
}



