#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <vector>
#include <limits>
#include "compression-algorithms/HuffmanCoding.h"
using namespace std;

uint8_t Quantize(uint8_t value, uint8_t quantizationLevel);
vector<uint8_t> RLEDecode(const vector<uint8_t>& data);
vector<uint8_t> RLEEncode(const vector<uint8_t>& data, uint8_t quantizationLevel);
vector<vector<uint8_t>> readYUV420File(const string& filename, int width, int height);

pair<int, int> RLECompress(const std::vector<std::vector<uint8_t>>& frames, uint8_t quantizationLevel);
bool RLEDecompress();
void PrintRLECompressionDifference(int start, int final);

bool filesEqual(string filePath1, string filePath2);

const string encodeOutputPath = "../test-io/output-files/main-encoded.txt";
const string decodeOutputPath = "../test-io/output-files/main-decoded.txt";
int main() {
    int choice;
    cout << "Enter 1 for RLE and 2 for Huffman" << endl;
    cin >> choice;
    if (choice == 1) {
        std::string filename;
        std::cout << "Enter the YUV file name: ";
        std::cin >> filename;
        filename = "main-io/test_videos/" + filename;

        int width, height;
        std::cout << "Enter the width and height of the YUV file: ";
        std::cin >> width >> height;

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

uint8_t Quantize(uint8_t value, uint8_t quantizationLevel) {
    // Map 256 values into quantizationLevel bins.
    return (value / quantizationLevel) * quantizationLevel;
}

vector<uint8_t> RLEDecode(const vector<uint8_t>& data) {
    vector<uint8_t> decodedFrame;

    for (auto it = data.begin(); it != data.end(); ++it) {
        uint8_t pixel = *it;
        uint8_t count = *++it;
        decodedFrame.insert(decodedFrame.end(), count, pixel);
    }

    return decodedFrame;
}

vector<uint8_t> RLEEncode(const vector<uint8_t>& data, uint8_t quantizationLevel) {
    if (data.empty()) {
        return {};
    }

    vector<uint8_t> result;
    uint8_t count = 1;
    uint8_t current = Quantize(data[0], quantizationLevel);
    const uint8_t maxRunLength = numeric_limits<uint8_t>::max();

    for (size_t i = 1; i < data.size(); ++i) {
        uint8_t quantized = Quantize(data[i], quantizationLevel);
        if (quantized == current) {
            if (count < maxRunLength) {  // Avoid overflow
                ++count;
            } else {
                // Write out current run before it's lost.
                result.push_back(current);
                result.push_back(count);
                count = 1;
            }
        } else {
            result.push_back(current);
            result.push_back(count);
            current = quantized;
            count = 1;
        }
    }

    // Write out the last run.
    result.push_back(current);
    result.push_back(count);

    return result;
}


vector<vector<uint8_t>> readYUV420File(const string& filename, int width, int height) {
    // Size of each frame for YUV420.
    int frameSize = width * height * 1.5;

    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "Failed to open file\n";
        return {};
    }

    // Read frames one by one.
    vector<vector<uint8_t>> frames;
    while (!file.eof()) {
        vector<uint8_t> frame(frameSize);
        file.read(reinterpret_cast<char*>(frame.data()), frameSize);

        // Check if a frame was read successfully.
        if (file.gcount() != frameSize) {
            break;
        }

        frames.push_back(move(frame));
    }

    file.close();

    return frames;
}

pair <int, int> RLECompress(const std::vector<std::vector<uint8_t>>& frames, uint8_t quantizationLevel) {
    std::cout << "Size of the input YUV file: " << (frames.size() * frames[0].size()) << " bytes\n";

    std::ofstream outFile("main-io/compressed_rle.rle", std::ios::binary);
    if (!outFile) {
        std::cerr << "Failed to open output file\n";
        return make_pair(0,0);
    }

    // Encode frames
    size_t totalSize = 0;
    for (const auto& frame : frames) {
        auto encoded = RLEEncode(frame, quantizationLevel);
        uint32_t size = encoded.size();
        totalSize += size;
        outFile.write(reinterpret_cast<const char*>(&size), sizeof(size));
        outFile.write(reinterpret_cast<const char*>(encoded.data()), encoded.size());
    }
    outFile.close();

    std::cout << "Size of the compressed RLE file: " << totalSize << " bytes\n";

    return make_pair((frames.size() * frames[0].size()), totalSize);
}

bool RLEDecompress() {
    // Decode the compressed frames
    std::ifstream inCompressedFile("main-io/compressed_rle.rle", std::ios::binary);
    std::ofstream outDecodedFile("main-io/decoded.yuv", std::ios::binary);

    uint32_t size;
    while (inCompressedFile.read(reinterpret_cast<char *>(&size), sizeof(size))) {
        std::vector<uint8_t> compressedFrame(size);
        if (!inCompressedFile.read(reinterpret_cast<char *>(compressedFrame.data()), compressedFrame.size())) {
            std::cerr << "Failed to read a frame\n";
            return false;
        }

        auto decoded = RLEDecode(compressedFrame);
        outDecodedFile.write(reinterpret_cast<const char *>(decoded.data()), decoded.size());
    }

    inCompressedFile.close();
    outDecodedFile.close();

    return true;
}

void PrintRLECompressionDifference (int start, int final) {
    if (start > final) {
        cout << "file size reduced by " << (start - final)*100 / start << "%" << endl;
    } else {
        cout << "file size increased by " << (final - start)*100 / start << "%" << endl;
    }
}