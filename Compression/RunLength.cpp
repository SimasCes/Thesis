#include <iostream>
#include <sstream>
#include <fstream>
using namespace std;


// To help with the coding I found some online resources and then edited them so they will be
// useful for my dissertation and can print the compression ration. These were used:
// https://www.geeksforgeeks.org/run-length-encoding/
// https://www.codespeedy.com/run-length-encoding-of-a-string-in-cpp/


// To put the contents of a text file into a string
string fileToString()
{
    // Allows the user (or me) to enter the file location of the file to compress
    string fileLocation;
    cout << "Enter the location of the file: ";
    cin >> fileLocation;

    // Got help from https://stackoverflow.com/questions/116038/how-do-i-read-an-entire-file-into-a-stdstring-in-c
    // on how to upload a txt files content into a stream (can be converted to string with .str())
    ifstream input(fileLocation);
    stringstream stream;

    string text;
    // Stream.str() will be the string format of the whole text file
    while(input >> stream.rdbuf())
    {
        text = stream.str();
    }

    // Returns the string (which contains the whole file)
    return text;
}


// Runs the main compression algorithm
string runLengthEncoding(string str)
{
    //Finds the length of the string
    int len = str.length();

    // To store the compressed string, right now it is empty
    string compressed = "";

    // Loops through all the elements in the list
    for (int i = 0; i < len; i++) {

        // Count will keep count of number of occurences of each charcater
        int count = 1;
        // While i is less than the string (so we are no at the end)
        // and the current character is = to the next
        while (i < len - 1 && str[i] == str[i + 1]) 
        {
            // Update count and i
            count++;
            i++;
        }

        // Stores the compressed file to this string
        compressed += str[i] + to_string(count);
    }

    // Return the comporessed string/file
    return compressed;
}


int main()
{
    // Store the string, which has the document to compress in text
    string text = fileToString();

    // Run the algoirthm to get the compressed text
    string compressed = runLengthEncoding(text);

    // As 8 bits can be used in abreviataed code, I should times the outputs and inputs by 8 as
    // I did with Huffman. However, the output and input both need to be multiplied. They are both 
    // multiplied by a constant so this can be ignored and just the original size kept.

    // Store the uncompressed and compressed lengths in a string
    double before = text.length();
    double after = compressed.length();

    // Then calculate and output the ratio
    double ratio = before / after;
    cout << ratio;

    return 0;
}