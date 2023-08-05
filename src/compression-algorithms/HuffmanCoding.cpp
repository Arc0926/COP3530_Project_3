#include <string>
#include <unordered_map>
#include "HuffmanCoding.h"
using namespace std;

MinHeap::MinHeap(unsigned int capacity)
{
    size = 0;
    this->capacity = capacity;
    array = new MinHeapNode*[capacity];
}

MinHeap::MinHeap(char data[], unsigned int freq[], unsigned int size)
{
    this->size = size;
    capacity = size;
    array = new MinHeapNode*[size];
    for (int i = 0; i < size; i ++)
    {
        array[i] = newNode(data[i], freq[i]);
    }
    buildMinHeap();
}

MinHeapNode* MinHeap::newNode(char data, unsigned int freq, MinHeapNode* left, MinHeapNode* right)
{
    MinHeapNode* temp = new MinHeapNode;
    temp->left = left;
    temp->right = right;
    temp->data = data;
    temp->freq = freq;

    return temp;
}

void MinHeap::swapNodes(MinHeapNode* &a,
                     MinHeapNode* &b)
{
 
    struct MinHeapNode* t = a;
    a = b;
    b = t;
}

void MinHeap::heapify(int index)
{
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < size
        && array[left]->freq < array[smallest]->freq)
        smallest = left;

    if (right < size
        && array[right]->freq < array[smallest]->freq)
        smallest = right;
    if (smallest != index)
    {
        swapNodes(array[smallest], array[index]);
        heapify(smallest);
    }
}

MinHeapNode* MinHeap::extractMin()
{
    MinHeapNode* temp = array[0];
    array[0] = array[size-1];
    size--;
    heapify(0);
    return temp;
}

void MinHeap::insert(char c, unsigned int freq, MinHeapNode* left, MinHeapNode* right)
{
    
    int index = size;
    size++;
    MinHeapNode* node = newNode(c, freq, left, right);
    while (index && node->freq < array[(index-1)/2]->freq)
    {
        array[index] = array[(index-1)/2];
        index = (index - 1)/2;
    }
    array[index] = node;
}

void MinHeap::buildMinHeap()
{
    int n = size - 1;
    int i;
    for (i = (n-1)/2; i >=0; --i)
    {
        heapify(i);
    }
}


unsigned int MinHeap::getSize()
{
    return size;
}

MinHeapNode** MinHeap::getArray()
{
    return array;
}

MinHeapNode* MinHeap::buildHuffmanTree() 
{
    MinHeapNode* left;
    MinHeapNode* right;
    while(size > 1) {
        left = extractMin();
        right = extractMin();
        insert('!', left->freq + right->freq, left, right);
    }

    return extractMin();
}

void MinHeap::traverseHuffmanTree(MinHeapNode* root, string path, unordered_map<char, string>& codes)
{
    if (root == nullptr)
        return;
    // If the node is a leaf, it contains a character
    if (!(root->left) && !(root->right)) {
        codes[root->data] = path;
    // Traverse left and append '0' to the path
    traverseHuffmanTree(root->left, path + "0", codes);

    // Traverse right and append '1' to the path
    traverseHuffmanTree(root->right, path + "1", codes);
    }
}

