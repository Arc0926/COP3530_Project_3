#include <iostream>
#include <string>
#include <fstream>
#include "compression-algorithms/HuffmanCoding.h"
using namespace std;

int main() {
    cout << "Enter .txt file path:" << endl;
    string filePath;
    cin >> filePath;
    ifstream input(filePath);
    ofstream output("../test-io/output-files/main-output.txt");


	return 0;
}

