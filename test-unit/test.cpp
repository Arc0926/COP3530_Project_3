#include "../src/main.cpp"
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
//#include "RLE.h"

#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include "../src/compression-algorithms/HuffmanCoding.h"
using namespace std;

/*
	To check output (At the Project1 directory):
		g++ -std=c++14 -Werror -Wuninitialized -o build/test test-unit/test.cpp && build/test
*/

void test(istream& input, ostream& output);
void filesEqual(ifstream& file1, ifstream& file2);

TEST_CASE("1: dummy test", "[dummy]") {
	ifstream input("../test-io/input-files/1.txt");
	ofstream out("../test-io/output-files/1.txt");
	test(input, out);
	input.close();
	out.close();
	ifstream output("../test-io/output-files/1.txt");
	ifstream expectedOutput("../test-io/expected-output-files/1.txt");
	filesEqual(output, expectedOutput);
}

/*
TEST_CASE("2: OpenCV", "[opencv]") {
	std::string img = "../../test-io/input-files/doggy.jpg";
    Mat srcImage = imread(img);
    REQUIRE(srcImage.data);
    imshow("srcImage", srcImage);
    waitKey(0);
}
*/

TEST_CASE("3: MinHeap insert", "[compression]") {
	ifstream input("../test-io/input-files/3.txt");
	ofstream out("../test-io/output-files/3.txt");
	MinHeap minHeap(500);
	
	map<char, int> m;
	string line;
	while(getline(input, line))
	{
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
	for(auto& pair : m)
	{
		minHeap.insert(pair.first, pair.second);
	}
	MinHeapNode** array = minHeap.getArray();
	unsigned int size = minHeap.getSize();
	for(unsigned int i = 0; i < size; i++)
	{
		out << array[i]->data << " : " << array[i]->freq << endl;
	}
	input.close();
	out.close();
	ifstream output("../test-io/output-files/3.txt");
	ifstream expectedOutput("../test-io/expected-output-files/3.txt");
	filesEqual(output, expectedOutput);
}

TEST_CASE("4: MinHeap heapify", "[compression]") {
	ifstream input("../test-io/input-files/4.txt");
	ofstream out("../test-io/output-files/4.txt");
	MinHeap minHeap(500);
	
	map<char, int> m;
	string line;
	while(getline(input, line))
	{
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
	for(auto& pair : m)
	{
		minHeap.insert(pair.first, pair.second);
	}
	MinHeapNode** array = minHeap.getArray();
	unsigned int size = minHeap.getSize();
	for (int i = 0; i < size; i++)
	{
		MinHeapNode* min = minHeap.extractMin();
		out << min->data << " : " << min->freq << endl;
	}
	
	input.close();
	out.close();
	ifstream output("../test-io/output-files/4.txt");
	ifstream expectedOutput("../test-io/expected-output-files/4.txt");
	filesEqual(output, expectedOutput);
}

TEST_CASE("5: buildHuffmanTree and traverseHuffmanTree", "[compression]") {
	ifstream input("../test-io/input-files/5.txt");
	ofstream out("../test-io/output-files/5.txt");
	MinHeap minHeap(500);
	
	map<char, int> m;
	string line;
	while(getline(input, line))
	{
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
	for(auto& pair : m)
	{
		minHeap.insert(pair.first, pair.second);
	}
	MinHeapNode* root = minHeap.buildHuffmanTree();
	map<char, string> codes;
	minHeap.traverseHuffmanTree(root, "", codes);
	for (auto& pair : codes)
	{
		out << pair.first << " : " << pair.second << endl;
	}

	input.close();
	out.close();
	ifstream output("../test-io/output-files/5.txt");
	ifstream expectedOutput("../test-io/expected-output-files/5.txt");
	filesEqual(output, expectedOutput);
	
}
/*
TEST_CASE("3: Run Length Encoding", "[compression]") {
	// Define video dimensions and frame rate
	const int width = 640;
	const int height = 480;
	const int frameRate = 30;

	ifstream inputFile("3_input_video.raw", ios::binary);
    REQUIRE(inputFile);

    ofstream outputFile("3_compressed_video.bin", ios::binary);
    REQUIRE(outputFile);

    vector<char> frameData(width * height * 3); // Assuming 24-bit RGB frames

    // Read frames from the input video file and compress them
    while (inputFile.read(frameData.data(), frameData.size())) {
        vector<char> compressedFrame = compressRLE(frameData);
        int compressedSize = compressedFrame.size();
        outputFile.write(reinterpret_cast<char*>(&compressedSize), sizeof(int));
        outputFile.write(compressedFrame.data(), compressedSize);
    }
    inputFile.close();
    outputFile.close();

    cout << "Video compression completed.\n";
}
*/

//a copy of main() in main.cpp to test for console outputs
void test(istream& input, ostream& output) {
}

void filesEqual(ifstream& file1, ifstream& file2) {
	string line1;
	string line2;
	//require either both files not be empty or both files be empty
	REQUIRE(((file1.peek() != ifstream::traits_type::eof() && file2.peek() != std::ifstream::traits_type::eof())
			|| (file1.peek() == ifstream::traits_type::eof() && file2.peek() == std::ifstream::traits_type::eof())));
	while(getline(file1, line1) && getline(file2, line2)) 
	{
		REQUIRE(line1 == line2);
	};
	
	file1.close();
	file2.close();
}
