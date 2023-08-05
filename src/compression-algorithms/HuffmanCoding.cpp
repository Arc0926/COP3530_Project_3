#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <filesystem>
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

void MinHeap::swapNodes(MinHeapNode* &a, MinHeapNode* &b)
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

    // If left child is smaller, set smallest to left index
    if (left < size
        && array[left]->freq < array[smallest]->freq)
        smallest = left;

    // If right child is smaller, set smallest to right index
    if (right < size
        && array[right]->freq < array[smallest]->freq)
        smallest = right;

    // If either child is smaller than parent, swap parent with smallest child
    if (smallest != index)
    {
        swapNodes(array[smallest], array[index]);
        // Recursively apply heapify down the tree
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
    // While the index is valid and the new node's frequency is smaller
    // than its parent's frequency
    MinHeapNode* node = newNode(c, freq, left, right);
    while (index && node->freq < array[(index-1)/2]->freq)
    {
        // Move the parent node down by overwriting the current index
        array[index] = array[(index-1)/2];
        index = (index - 1)/2;
    }
    // Update the index to be the parent's index
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

void MinHeap::traverseHuffmanTree(MinHeapNode* root, string path, map<char, string>& codes)
{
    if (root == nullptr)
        return;
    // If the node is a leaf, it contains a character
    if (root->left == nullptr && root->right == nullptr) 
        codes[root->data] = path;
    // Traverse left and append '0' to the path
    traverseHuffmanTree(root->left, path + "0", codes);

    // Traverse right and append '1' to the path
    traverseHuffmanTree(root->right, path + "1", codes);
}



pair<map<char, string>, int> huffmanEncodeTextFile(string inputFileName, string outputFileName)
{
    ifstream input(inputFileName);
    MinHeap minHeap(500);
	
	map<char, int> m;
	string line;
	while(getline(input, line))
	{
        line += '\n'; //reappend the '\n' that getline removed
		for(auto& c : line)
		{  
			// If the character is not already in the map, add it with a frequency of 1.
			if (m.find(c) == m.end())
			{
				m.insert({c, 1});
			}
			// If the character is already in the map, increment its frequency.
			else
			{
				m[c]++;
			}
		}
	}
    input.close();
	for(auto& pair : m)
	{
		minHeap.insert(pair.first, pair.second);
	}
    MinHeapNode* root = minHeap.buildHuffmanTree();
	map<char, string> codes;
	minHeap.traverseHuffmanTree(root, "", codes);
    int dummyBits = replaceTextWithHuffmanCodes(inputFileName, outputFileName, codes);

    return {codes, dummyBits};
}

void huffmanDecodeTextFile(string inputFileName, string outputFileName, map<char, string> huffmanCodes, int dummyBits)
{
    //reverse the keys and values for the huffmanCodes
    map<string, char> reversedCodes;
    for (auto& pair : huffmanCodes)
    {
        reversedCodes.insert({pair.second, pair.first});
    }
    ifstream input(inputFileName, ios::binary);
    ofstream output(outputFileName, ios::binary);

    // Read binary data and convert huffman codes to ascii value of char
    string binaryString;
    string test;
    char byte;
    int end = 0;
    while (input.get(byte)) {
        if (input.peek() == EOF) 
        {
            end = dummyBits;
        } 
        for (int bit = 7; bit >= end; --bit) {
            binaryString += ((byte >> bit) & 1) ? '1' : '0';
            if (reversedCodes.find(binaryString) != reversedCodes.end())
            {   
                output.write(&reversedCodes[binaryString], 1);
                test += reversedCodes[binaryString];
                binaryString = "";
            }
        }
    }
    input.close();
    output.close();
}

int replaceTextWithHuffmanCodes(string inputFileName, string outputFileName, map<char, string> huffmanCodes) {
    ifstream input(inputFileName);
    ofstream output(outputFileName, ios::binary);

    char bitBuffer = 0; // Buffer to accumulate bits
    int bitCount = 0;   // Count of bits in the buffer

    char c;
    string test;
    while (input.get(c)) {
        test += c;
        // if (c == '\n') {
        //     continue;
        // }
        const string& huffmanCode = huffmanCodes.at(c);

        for (char codeBit : huffmanCode) {
            bitBuffer <<= 1; // Shift the buffer to the left
            bitBuffer |= (codeBit - '0'); // Set the least significant bit

            ++bitCount;
            if (bitCount == 8) {
                output.write(&bitBuffer, 1);
                bitBuffer = 0;
                bitCount = 0;
            }
        }
    }

    if (bitCount > 0)
    {
        bitBuffer <<= (8 - bitCount);
        output.write(&bitBuffer, 1);
    }

    input.close();
    output.close();
    return 8 - bitCount;
}

bool filesEqual(string filePath1, string filePath2) {
    ifstream file1(filePath1);
    ifstream file2(filePath2);
	string line1;
	string line2;
	//require either both files not be empty or both files be empty
	if(((file1.peek() == ifstream::traits_type::eof() && file2.peek() != std::ifstream::traits_type::eof())
		|| (file1.peek() != ifstream::traits_type::eof() && file2.peek() == std::ifstream::traits_type::eof())))
            return false;
	while(getline(file1, line1) && getline(file2, line2)) 
	{
		if (line1 != line2)
            return false;
	};
	
	file1.close();
	file2.close();
    return true;
}


