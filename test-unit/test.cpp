//#include "../src/main.cpp"
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
//#include "RLE.h"
#include <vector>
#include <sstream>
#include <fstream>
#include <opencv2/opencv.hpp>
using namespace cv;
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

TEST_CASE("2: OpenCV", "[opencv]") {
	std::string img = "../../test-io/input-files/doggy.jpg";
    Mat srcImage = imread(img);
    REQUIRE(srcImage.data);
    imshow("srcImage", srcImage);
    waitKey(0);
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
	while(getline(file1, line1) && getline(file2, line2)) {
		REQUIRE(line1 == line2);
	};
	file1.close();
	file2.close();
}
