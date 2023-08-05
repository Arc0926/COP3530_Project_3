#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <filesystem>
#include "compression-algorithms/HuffmanCoding.h"
using namespace std;

// Function to replace text with Huffman codes
pair<map<char, string>, int> huffmanEncodeTextFile(string inputFileName, string outputFileName);
void huffmanDecodeTextFile(string inputFileName, string outputFileName, map<char, string> huffmanCodes, int dummyBits);
int replaceTextWithHuffmanCodes(string inputFilename, string outputFilename, map<char, string> huffmanCodes);
bool filesEqual(string filePath1, string filePath2);

const string encodeOutputPath = "../test-io/output-files/main-encoded.txt";
const string decodeOutputPath = "../test-io/output-files/main-decoded.txt";
int main() {
    cout << "Enter .txt file path:" << endl;
    string inputFilePath;
    cin >> inputFilePath;

    //compress file using huffman encoding
    pair<map<char, string>, int> codesAndDummyBits = huffmanEncodeTextFile(inputFilePath, encodeOutputPath);
    map<char, string> codes = codesAndDummyBits.first;
    int dummyBits = codesAndDummyBits.second;
    //get file sizes in bytes
    ifstream uncompressedFile(inputFilePath);
    if (!uncompressedFile.is_open()) 
    {
        cout << "Error opening file." << endl;
        return 1;
    }
    uncompressedFile.seekg(0, ios::end);
    streampos uncompressedFileSize = uncompressedFile.tellg();
    uncompressedFile.seekg(0, ios::beg);
    uncompressedFile.close();

    ifstream compressedFile(encodeOutputPath);
    if (!compressedFile.is_open()) 
    {
        cout << "Error opening compressed file." << endl;
        return 1;
    }
    compressedFile.seekg(0, ios::end);
    streampos compressedFileSize = compressedFile.tellg();
    compressedFile.seekg(0, ios::beg);

    cout << "Intput file size is " << uncompressedFileSize << " bytes." << endl;
    cout << "Huffman encoded file size is " << compressedFileSize << " bytes." << endl;
    cout << "File size reduced by " << 100 - (double)compressedFileSize/uncompressedFileSize*100 << "%" << endl;

    huffmanDecodeTextFile(encodeOutputPath, decodeOutputPath, codes, dummyBits);
    if(filesEqual(inputFilePath, decodeOutputPath))
        cout << "File successfully decompressed." << endl;
    else
        cout << "File unsuccessfully decompressed" << endl;
	return 0;
}

pair<map<char, string>, int> huffmanEncodeTextFile(string inputFileName, string outputFileName)
{
    ifstream input(inputFileName);
    cout << "compressing using huffman encoding" << endl;
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
    cout << "decompressing huffman encoded file" << endl;


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
                if (binaryString == "0010")
                {
                    int test2 = 1;
                }
                output.write(&reversedCodes[binaryString], 1);
                test += reversedCodes[binaryString];
                binaryString = "";
            }
        }
    }
    input.close();
    output.close();
}
//returns how many extra bits at the end should be ignored
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



