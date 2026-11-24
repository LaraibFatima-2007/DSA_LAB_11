/*
 * Name: Laraib Fatima
 * Lab 11 - Binary Tree Applications and AVL
 * Task 1 - Huffman Tree Construction and Preorder Traversal using Binary Heap 
 */

#include <iostream>
using namespace std;

// Node structure for the Huffman Tree
struct HuffmanNode {
    char data;
    unsigned freq;
    HuffmanNode *left, *right;

    HuffmanNode(char data, unsigned freq) {
        this->data = data;
        this->freq = freq;
        left = right = nullptr;
    }
};

// Binary Min-Heap using dynamic array
class MinHeap {
private:
    HuffmanNode** heap;
    int capacity;
    int currentSize;

    void swapNodes(HuffmanNode*& a, HuffmanNode*& b) {
        HuffmanNode* temp = a;
        a = b;
        b = temp;
    }

    void heapifyDown(int idx) {
        int smallest = idx;
        int left = 2 * idx + 1;
        int right = 2 * idx + 2;
        if (left < currentSize && heap[left]->freq < heap[smallest]->freq)
            smallest = left;
        if (right < currentSize && heap[right]->freq < heap[smallest]->freq)
            smallest = right;
        if (smallest != idx) {
            swapNodes(heap[idx], heap[smallest]);
            heapifyDown(smallest);
        }
    }
    void heapifyUp(int idx) {
        while (idx != 0 && heap[(idx - 1) / 2]->freq > heap[idx]->freq) {
            swapNodes(heap[idx], heap[(idx - 1) / 2]);
            idx = (idx - 1) / 2;
        }
    }

public:
    MinHeap(int cap) {
        capacity = cap;
        currentSize = 0;
        heap = new HuffmanNode*[capacity];
    }
    ~MinHeap() {
        delete[] heap;
    }
    int size() const {
        return currentSize;
    }

    void insert(HuffmanNode* node) {
        if (currentSize == capacity) return;
        heap[currentSize] = node;
        heapifyUp(currentSize);
        currentSize++;
    }

    HuffmanNode* extractMin() {
        if (currentSize <= 0) return nullptr;
        HuffmanNode* root = heap[0];
        heap[0] = heap[currentSize - 1];
        currentSize--;
        if (currentSize > 0) {
            heapifyDown(0);
        }
        return root;
    }
};

// Preorder traversal to print Huffman codes by checking leaf nodes structurally
void printPreorderCodes(HuffmanNode* root, char code[], int top) {
    if (!root) return;

    // A leaf node has no left and no right child
    if (!root->left && !root->right) {
        cout << root->data << " : ";
        if (top == 0) {
            cout << "0";
        } else {
            for (int i = 0; i < top; i++) {
                cout << code[i];
            }
        }
        cout << endl;
        return;
    }

    // Traverse left with '0'
    if (root->left) {
        code[top] = '0';
        printPreorderCodes(root->left, code, top + 1);
    }

    // Traverse right with '1'
    if (root->right) {
        code[top] = '1';
        printPreorderCodes(root->right, code, top + 1);
    }
}

// Free dynamically allocated tree memory
void deleteTree(HuffmanNode* root) {
    if (!root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main() {
    int n;
    cout << "Enter the number of distinct characters (N): ";
    cin >> n;

    if (n <= 0) {
        cout << "Invalid size." << endl;
        return 0;
    }

    char* S = new char[n + 1];
    cout << "Enter the string of " << n << " distinct characters: ";
    cin >> S;

    // Verify length manually
    int len = 0;
    while (S[len] != '\0') len++;

    if (len != n) {
        cout << "Error: String length does not match N." << endl;
        delete[] S;
        return 0;
    }

    unsigned* freq = new unsigned[n];
    cout << "Enter the corresponding frequencies:\n";
    for (int i = 0; i < n; i++) {
        cout << "Frequency of '" << S[i] << "': ";
        cin >> freq[i];
    }

    // Build Min-Heap
    MinHeap minHeap(n);
    for (int i = 0; i < n; i++) {
        minHeap.insert(new HuffmanNode(S[i], freq[i]));
    }

    // Construct Huffman Tree
    while (minHeap.size() > 1) {
        HuffmanNode* left = minHeap.extractMin();
        HuffmanNode* right = minHeap.extractMin();

        // Internal node holds null character '\0' and combined frequency
        HuffmanNode* top = new HuffmanNode('\0', left->freq + right->freq);
        top->left = left;
        top->right = right;

        minHeap.insert(top);
    }

    HuffmanNode* root = minHeap.extractMin();

    char* codeBuffer = new char[n + 1];
    cout << "\nHuffman Codes:\n";
    printPreorderCodes(root, codeBuffer, 0);

    // Clean up all dynamic memory
    delete[] codeBuffer;
    delete[] freq;
    delete[] S;
    deleteTree(root);

    return 0;
}