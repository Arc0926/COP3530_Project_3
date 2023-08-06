#ifndef MIN_HEAP
#define MIN_HEAP
using namespace std;

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
    //constructors

    MinHeap(unsigned int capacity);
    MinHeap(char data[], unsigned int freq[], unsigned int size);

    // Creates a new min heap node with a specified byte of data and its frequency
    // Input: data, frequency (optional left and right children ptrs)
    // output: node object
    MinHeapNode* newNode(char data, unsigned int freq, MinHeapNode* left = nullptr, MinHeapNode* right = nullptr);

    // Swap positions of node a and b in the heap
    void swapNodes(MinHeapNode* &a, MinHeapNode* &b);

    // Swaps node at index with smaller children in order to maintain min heap properties
    void heapify(int index);

    // Returns the minimum node (root) and removes it from the min heap
    // Input: none
    // Output: minimum node
    MinHeapNode* extractMin();

    // Inserts a node with the given properties into the heap
    // Input: data, frequency, (optional left and right children ptrs)
    // Output: none
    void insert(char data, unsigned int freq, MinHeapNode* left = nullptr, MinHeapNode* right = nullptr);

    // Arranges array elements into a min heap structure
    void buildMinHeap();

    // Returns number of nodes in the min heap
    unsigned int getSize();

    // Returns the array storing each of the nodes of the min heap
    MinHeapNode** getArray();

    //Uses nodes from min heap to build a huffman tree
    // Input: none
    // Output: pointer to root of huffman tree
    MinHeapNode* buildHuffmanTree();

    // Recursively traverses the huffman tree to get each of the binary codes
    // Input: pointer to root, path string, reference to map to store binary codes for each data
    // Output: none
    void traverseHuffmanTree(MinHeapNode* root, string path, map<char, string>& codes);
};

// Creates huffman tree and encodes text file
// Input: input file path, output file path
// Output: pair containing a map of the binary codes and how many bits at the end of the encode file should be disregarded
pair<map<char, string>, int> huffmanEncodeTextFile(string inputFileName, string outputFileName);

// Creates huffman tree and encodes YUV file
// Input: input file path, output file path
// Output: pair containing a map of the binary codes and how many bits at the end of the encode file should be disregarded
pair<map<char, string>, int> huffmanEncodeYUVFile(const string& inputFileName, string outputFileName);

// Function to replace huffman-encoded data with text data
// Input: input file path, output file path, huffman code map, number of bits to disregard at the end
void huffmanDecodeTextFile(string inputFileName, string outputFileName, map<char, string> huffmanCodes, int dummyBits);

// Function to replace huffman-encoded data with YUV data
// Input: input file path, output file path, huffman code map, number of bits to disregard at the end
void huffmanDecodeYUVFile(string inputFileName, string outputFileName, map<char, string> huffmanCodes, int dummyBits);

// Replaces txt data with huffman encoded data
// Input: input file path, output file path, huffman code map
// Output: how many bits at the end of the encode file should be disregarded
int replaceTextWithHuffmanCodes(string inputFilename, string outputFilename, map<char, string> huffmanCodes);

// Replaces YUV data with huffman encoded data
// Input: input file path, output file path, huffman code map
// Output: how many bits at the end of the encode file should be disregarded
int replaceYUVWithHuffmanCodes(string inputFilename, string outputFilename, map<char, string> huffmanCodes);
#endif