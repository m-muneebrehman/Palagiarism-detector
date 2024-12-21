#include <iostream>
#include <fstream>

using namespace std;

class StringOperations{
    public:
    bool extCheck ( const string &fileName, string const &extension) {
    // check the file extension ends with .txt
    if (fileName.length() >= extension.length())
        return (0 == fileName.compare (fileName.length() - extension.length(), extension.length(), extension));
    else 
        return false;
    }