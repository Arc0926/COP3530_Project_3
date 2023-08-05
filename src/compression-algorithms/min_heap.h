#ifndef MIN_HEAP
#define MIN_HEAP

#include <string>
//huffman tree node
struct MinHeapNode
{
    char data;
    unsigned int freq;
    //left and right children
    MinHeapNode *left, *right;
};

class MinHeap
{
private:
    unsigned int size;
    unsigned int capacity;
    MinHeapNode** array;
    
public:
    MinHeap(unsigned int capacity);
    
    MinHeap(char data[], unsigned int freq[], unsigned int size);
    MinHeapNode* newNode(char data, unsigned int freq);
    void swapNodes(MinHeapNode* a, MinHeapNode* b);
    void heapify(int index);
    MinHeapNode* extractMin();
    void insert(char c, unsigned int freq);
    void buildMinHeap();
    bool isLeaf(MinHeapNode* root);
    unsigned int getSize();
    MinHeapNode** getArray();
};

#endif