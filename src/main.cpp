#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <filesystem>
#include "compression-algorithms/HuffmanCoding.h"
using namespace std;

bool filesEqual(string filePath1, string filePath2);

const string encodeOutputPath = "../test-io/output-files/main-encoded.txt";
const string decodeOutputPath = "../test-io/output-files/main-decoded.txt";
int main() {
    cout << "Enter .txt file path:" << endl;
    string inputFilePath;
    cin >> inputFilePath;

    cout << "compressing using huffman encoding" << endl;
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

    cout << "decompressing huffman encoded file" << endl;
    huffmanDecodeTextFile(encodeOutputPath, decodeOutputPath, codes, dummyBits);
    if(filesEqual(inputFilePath, decodeOutputPath))
        cout << "File successfully decompressed." << endl;
    else
        cout << "File unsuccessfully decompressed" << endl;
	return 0;
}




