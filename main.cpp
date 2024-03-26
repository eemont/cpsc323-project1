#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// The code below simply just reads the file and outputs it into the terminal
// The code we'll be testing is input.cpp

int main() {
    string filename;

    // Open the file
    ifstream file("input.cpp");

    // Check if the file is opened successfully
    if (!file.is_open()) {
        cout << "Error opening file: " << filename << endl;
        return 1; // Return error code
    }

    // Read and output the contents of the file
    string line;
  
    while (getline(file, line)) {
        cout << line << endl;
    }

    // Close the file
    file.close();

    return 0; // Return success
}
