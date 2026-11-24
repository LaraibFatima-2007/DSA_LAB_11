/*
 * Name: Laraib Fatima
 * Lab 11 - Binary Tree Applications and AVL
 * Task 2 - Merging k Sorted Linked Lists using Binary Heap
 */

#include <iostream>
using namespace std;

// Singly Linked List Node
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};
// Custom Binary Min-Heap for ListNodes using dynamic array
class NodeMinHeap {
private:
    ListNode** heap;
    int capacity;
    int currentSize;

    void swapNodes(ListNode*& a, ListNode*& b) {
        ListNode* temp = a;
        a = b;
        b = temp;
    }
    void heapifyDown(int idx) {
        int smallest = idx;
        int left = 2 * idx + 1;
        int right = 2 * idx + 2;
        if (left < currentSize && heap[left]->val < heap[smallest]->val)
            smallest = left;
        if (right < currentSize && heap[right]->val < heap[smallest]->val)
            smallest = right;
        if (smallest != idx) {
            swapNodes(heap[idx], heap[smallest]);
            heapifyDown(smallest);
        }
    }
    void heapifyUp(int idx) {
        while (idx != 0 && heap[(idx - 1) / 2]->val > heap[idx]->val) {
            swapNodes(heap[idx], heap[(idx - 1) / 2]);
            idx = (idx - 1) / 2;
        }
    }

public:
    NodeMinHeap(int cap) {
        capacity = cap;
        currentSize = 0;
        heap = new ListNode*[capacity];
    }
    ~NodeMinHeap() {
        delete[] heap;
    }
    bool empty() const {
        return currentSize == 0;
    }
    void push(ListNode* node) {
        if (!node || currentSize == capacity) return;
        heap[currentSize] = node;
        heapifyUp(currentSize);
        currentSize++;
    }
    ListNode* pop() {
        if (currentSize <= 0) return nullptr;
        ListNode* minNode = heap[0];
        heap[0] = heap[currentSize - 1];
        currentSize--;
        if (currentSize > 0) {
            heapifyDown(0);
        }
        return minNode;
    }
};

// Function to merge k sorted linked lists
ListNode* mergeKLists(ListNode** lists, int k) {
    NodeMinHeap minHeap(k);
    // Push the head of each non-empty list into the Min-Heap
    for (int i = 0; i < k; i++) {
        if (lists[i] != nullptr) {
            minHeap.push(lists[i]);
        }
    }
    ListNode dummy(0);
    ListNode* tail = &dummy;
    while (!minHeap.empty()) {
        ListNode* smallest = minHeap.pop();
        tail->next = smallest;
        tail = tail->next;

        if (smallest->next != nullptr) {
            minHeap.push(smallest->next);
        }
    }

    return dummy.next;
}
//print a linked list
void printList(ListNode* head) {
    if (!head) {
        cout << "Empty List" << endl;
        return;
    }
    while (head) {
        cout << head->val;
        if (head->next) cout << " -> ";
        head = head->next;
    }
    cout << endl;
}
//  free memory
void freeList(ListNode* head) {
    while (head) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
    }
}

int main() {
    int k;
    cout << "Enter the number of sorted linked lists (k): ";
    cin >> k;

    if (k <= 0) {
        cout << "No lists to merge." << endl;
        return 0;
    }
    ListNode** lists = new ListNode*[k];
    for (int i = 0; i < k; i++) {
        lists[i] = nullptr;
    }
    for (int i = 0; i < k; i++) {
        int len;
        cout << "\nEnter number of elements in List " << i + 1 << ": ";
        cin >> len;
        ListNode* head = nullptr;
        ListNode* tail = nullptr;
        if (len > 0) {
            cout << "Enter " << len << " sorted elements: ";
            for (int j = 0; j < len; j++) {
                int val;
                cin >> val;
                ListNode* newNode = new ListNode(val);
                if (!head) {
                    head = tail = newNode;
                } else {
                    tail->next = newNode;
                    tail = newNode;
                }
            }
        }
        lists[i] = head;
    }

    cout << "\nInput Lists:\n";
    for (int i = 0; i < k; i++) {
        cout << "List " << i + 1 << ": ";
        printList(lists[i]);
    }
    ListNode* mergedHead = mergeKLists(lists, k);
    cout << "\nMerged Sorted Linked List:\n";
    printList(mergedHead);
    freeList(mergedHead);
    delete[] lists;
    return 0;
}