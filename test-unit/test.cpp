#include "../src/main.cpp"
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <sstream>
#include <fstream>
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
