#include "min_heap.h"

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

MinHeapNode* MinHeap::newNode(char data, unsigned int freq)
{
    MinHeapNode* temp = new MinHeapNode;
    temp->left = nullptr;
    temp->right = nullptr;
    temp->data = data;
    temp->freq = freq;

    return temp;
}

void MinHeap::swapNodes(MinHeapNode* a,
                     MinHeapNode* b)
{
 
    struct MinHeapNode* t = a;
    *a = *b;
    *b = *t;
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

void MinHeap::insert(char c, unsigned int freq)
{
    
    int index = size;
    size++;
    MinHeapNode* node = newNode(c, freq);
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

bool MinHeap::isLeaf(MinHeapNode* root)
{
    return !(root->left) && !(root->right);
}

unsigned int MinHeap::getSize()
{
    return size;
}

MinHeapNode** MinHeap::getArray()
{
    return array;
}

