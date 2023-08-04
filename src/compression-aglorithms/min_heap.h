#ifndef MIN_HEAP
#define MIN_HEAP

#include <string>

class MinHeap
{
    //huffman tree node
    struct MinHeapNode
    {
        char data;
        unsigned int freq;
        //left and right children
        MinHeapNode *left, *right;
    };
private:
    unsigned int size;
    MinHeapNode** array;
    
public:
    MinHeap();
    MinHeap(char data[], unsigned int freq[], unsigned int size);
    MinHeapNode* newNode(char data, unsigned int freq);
    void swapNodes(MinHeapNode* a, MinHeapNode* b);
    void heapify(int index);
    MinHeapNode* extractMin();
    void insert(MinHeapNode* root);
    void buildMinHeap();
    bool isLeaf(MinHeapNode* root);
};

#endif