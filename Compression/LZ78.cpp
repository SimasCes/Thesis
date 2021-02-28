#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
using namespace std;


// To help with the coding I found some online resources and then edited them so they will be
// useful for my dissertation (can print the compression ration). This code was used:
// https://gist.github.com/MajedSiefALnasr/32cff52b01d764ee158f2c36e4e84d95


// A node which storing index and data
struct Node
{
	int index;
	string data;
	Node *next;
};

// So we can compare the nodes later
void dictNode(Node *head, int index, string data)
{
	head->index = index;
	head->data = data;
	head->next = NULL;
};


// Will insert a new node 
void insertNode(Node *head, int index, string data)
{
    // To construct the node 
	Node *newNode = new Node;
	newNode->index = index;
	newNode->data = data;
	newNode->next = NULL;

	Node *current = head;
    // Loops and inserts node at end
	while (current != NULL)
	{
		if (current->next == NULL)
		{
			current->next = newNode;
			return;
		}
		current = current->next;
	}
};


// Search for a node
Node *searchNode(Node *head, string data)
{
	Node *curr = head;
	while (curr != NULL)
	{
		if (data.compare(curr->data) == 0)
			return curr;
		else
			curr = curr->next;
	}
	return NULL;
};


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


// Run the main algorithm and return the compressed size
double LZ78(string input)
{
    // Constructs dictionary
    Node *dictionary = new Node;

    string word;
    string result;
	int length;
    int last_seen;
    int index = 1;
    length = (int)input.length();
    word = input[0];
    dictNode(dictionary, 1, word);
    result += "0," + word;
    // To keep count for the compression ratio
    int ratio = 1;

    // Loop thorugh all the characters in the input text
    for (int i = 1; i < length; i++)
    {
        string data;
        data = input[i];

    // Used as a checkpoint
    reCheck:
        Node *search = searchNode(dictionary, data);
        // If node exists (see code line above)
        if(search)
        {
            i++;
            data += input[i];
            last_seen = search->index;
            // Go back to checkpoint
            goto reCheck;
        }
        // Otherwise
        else
        {
            char zero;
            // If the input is an empty stirng
            if(input[i] == ' ')
            {
                zero = '0';
            }
            else
            {
                zero = input[i];
            }

            // Calculate result here
            if((int)data.length() < 2)
            {
                result += " " + to_string(0) + "," + zero;
                // Update count of the number of compression pairs
                ratio++;
            }
            else
            {
                result += " " + to_string(last_seen) + "," + zero;
                // Update count of the number of compression pairs
                ratio++;
            }

            index++;
            if (i != length)
            {
                // Insert a node into the dictionary
                insertNode(dictionary, index, data);
            }  
        }
    }

    // Calculate each pair (so times 2) and times 8 bits. This is the size of the compressed file
    double compressed_size = (ratio * 2) * 8;

    // Return the results
    return compressed_size;
};


int main()
{
    // Store the string, which has the document to compress in text
    string text = fileToString();

    // Calculate the uncompressed size of the file (all characters times 8 bits which is used to store them)
    double uncompressed_size = text.size() * 8;

    // Calculate the compressed size with the algorithm
    double compressed_size = LZ78(text);

    //Find the compression ration
    double compressed_ratio = uncompressed_size / compressed_size;

    cout << compressed_ratio;

    return 1;
};