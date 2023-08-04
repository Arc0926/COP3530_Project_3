#include "min_heap.h"

MinHeap::MinHeap()
{
    size = 0;
    array = nullptr;
}

MinHeap::MinHeap(char data[], unsigned int freq[], unsigned int size)
{
    size = size;
    array = nullptr;
    for (int i = 0; i < size; i ++)
    {
        array[i] = newNode(data[i], freq[i]);
    }
    buildMinHeap();
}

MinHeap::MinHeapNode* MinHeap::newNode(char data, unsigned int freq)
{
    struct MinHeapNode* temp;
    temp->left = nullptr;
    temp->right = nullptr;
    temp->data = data;
    temp->freq = freq;

    return temp;
}

void MinHeap::swapNodes(MinHeap::MinHeapNode* a, MinHeap::MinHeapNode* b)
{
    MinHeapNode* t = a;
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

MinHeap::MinHeapNode* MinHeap::extractMin()
{
    MinHeapNode* temp = array[0];
    array[0] = array[size-1];
    size--;
    heapify(0);
    return temp;
}

void MinHeap::insert(MinHeapNode* node)
{
    int index = size;
    size++;
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

