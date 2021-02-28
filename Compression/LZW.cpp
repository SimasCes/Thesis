#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <map>
using namespace std;


// To help with the coding I found some online resources and then edited them so they will be
// useful for my dissertation (can print the compression ration). This code was used:
// geeksforgeeks.org/lzw-lempel-ziv-welch-compression-technique/

// To calculate the compression ratio, I will caluclate 8 (number of bits) by the number of outputs
// (each number value will be 1 output). The de-compressor would contruct the dictionary again when
// de-compressing so this will not need to be added to the final calculation.


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


// The main LZW compression algorithm 
vector<int> LZW(string text) 
{ 
	// Makes a table for the compression
	map<string, int> table; 
	// Loops for all characters (256 total) in ASCII and assigns values
	for (int i = 0; i <= 255; i++) 
	{ 
		string ch = ""; 
		ch += char(i); 
		table[ch] = i; 
	} 

	string cur = "";
	string next = ""; 
	cur += text[0]; 
	int code = 256; 
	vector<int> compressed; 

	// Builds table
	for (int i = 0; i < text.length(); i++) 
	{ 
		if (i != text.length() - 1) 
		{
			// Next character
			next += text[i + 1]; 
		}
			
		if (table.find(cur + next) != table.end()) 
		{ 
			// Adds letters together, e.g. if AB appears and repeats
			cur = cur + next; 
		} 
		else 
		{ 
			// Add codes and letters to table
			compressed.push_back(table[cur]); 
			table[cur + next] = code; 
			code++; 
			cur = next; 
		} 
		next = ""; 
	} 
	// compressed is the final compressed string
	compressed.push_back(table[cur]); 

	return compressed; 
} 


int main() 
{ 
    // Store the string, which has the document to compress in text
    string text = fileToString();

	// Calculate the uncompressed size of the file (all characters times 8 bits which is used to store them)
    double uncompressed_size = text.length() * 8;

	// Run the algorithm for the compressed size
	vector<int> compressed = LZW(text); 
	double compressed_size = compressed.size() * 8;

	//Find the compression ration
    double compressed_ratio = uncompressed_size / compressed_size;

    cout << compressed_ratio;

    return 1;
} 
