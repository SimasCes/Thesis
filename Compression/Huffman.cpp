#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <list>
#include <queue>
using namespace std;


// To help with the huffman coding, and coding the Node these sites and tutorials were used:
// https://www.guru99.com/cpp-structures.html
// https://oanaunciuleanu.wixsite.com/artofcoding/post/huffman-coding-and-decoding-algorithm-in-c
// https://www.geeksforgeeks.org/huffman-coding-greedy-algo-3/
// some code was also used and some re-weritten to be intergrated with my program 


//Global variable to calculate the compressed size
double compressed_size;


// A node which is used to store frequency and characters
struct Node
{
    // One of the input characters 
    char data; 
    // Frequency of the character 
    double freq; 
    // Left and right child 
    Node *left;
    Node *right;

    // A constructor for the Node
    Node(char data, double freq)
    {
        left = NULL;
        right = NULL;
        this->data = data;
        this->freq = freq;
    }

};


// Compares which of the 2 inputs has a higher frequency
struct compare
{
    bool operator()(Node* left, Node* right)
    {
        //Returns true or false
        return (left->freq > right->freq);
    }
};


// Prints huffman codes from the root of Huffman Tree (edited from desplay fuction to compute compression ratio)
void getCompressedSize(struct Node* root, string str) 
{ 
    // Stops the loop
    if (!root) 
    {
        return; 
    }
    
    // If there is data (the root is not finsihed)
    if (root->data != '$') 
    {
        // Calculate the frequency * bits you will be using
        // The + 8 and str.length() is to add the size of the dictionary
        // as this is needed for decompression
        compressed_size += (root->freq * str.length()) + 8 + str.length();
    }
  
    // Keep searching the tree in left and right branches
    getCompressedSize(root->left, str + "0"); 
    getCompressedSize(root->right, str + "1"); 
} 


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


// Find the frequency of each letter/character in the text
pair<list<char>, list<double> > frequencyCalc(string text)
{
    // Create a map to store frequency and characters
    map<char, double> freqMap;

    // For the for loop, this site helped me undesrtand how to code this in c++:
    // https://www.geeksforgeeks.org/frequency-of-each-character-in-a-string-using-unordered_map-in-c/
    
    // Traverse the whole text to be able to find the requency of the characters
    for(int i = 0; i < text.length(); i++)
    {
        // If you cannot find the character in the map
        if(freqMap.find(text[i]) == freqMap.end())
        {
            // Insert that element into the map with frequency 1
            freqMap.insert(make_pair(text[i], 1));
        }
        else
        {
            //If you do find the character, update the frequency by 1
            freqMap[text[i]] ++;
        }
    }

    // To store the characters and the freqeuncises in 2 lists
    list<char> element;
    list<double> frequency;

    //This will loop through the map
    for (map<char,double>::iterator it = freqMap.begin(); it != freqMap.end(); ++it)
    {
        // Update each list with the elements
        element.push_back(it->first);
        frequency.push_back(it->second);
    }

    //This is so both of the lists can be returned by the fucntion
    pair<list<char>, list<double> > final;
    final.first = element;
    final.second = frequency;

    return final;
}


// The main function that builds a Huffman Tree and 
// print codes by traversing the built Huffman Tree 
void HuffmanCodes(list<char> data, list<double> frequency) 
{ 
    Node *left;
    Node *right;
    Node *top ;
  
    // Create a minHeap (using a priority queue)
    priority_queue<Node*, vector<Node*>, compare> minHeap; 
    
    // This is for the loop 
    int temp = data.size();
    // Loop through all the elements and insert the charcaters and frequencies into Nodes
    for(int i = 0; i < temp; i++)
    {
        minHeap.push(new Node(data.front(), frequency.front())); 
        data.pop_front();
        frequency.pop_front();
    }
            
    // Iterate until heap size is 1 (while the size is not 1 run)
    while (minHeap.size() != 1) { 
  
        // Get the 2 minimum frequency items (just the top of the min heap)
        left = minHeap.top(); 
        minHeap.pop(); 
        right = minHeap.top(); 
        minHeap.pop(); 
  
        // A new node top will be created, the frequency will equal the sum frequency of the
        // other 2 nodes combined (left freq + right freq). These 2 extracted nodes will
        // then be set as the children of node top. This will be added to the main heap
        // with the '$' symbol which is a special value for internal nodes (not included nodes).
        top = new Node('$', left->freq + right->freq); 
  
        top->left = left; 
        top->right = right; 
  
        minHeap.push(top); 
    } 
  
    // Find the compressed size of the file and store it in the global variable
    getCompressedSize(minHeap.top(), ""); 
} 


int main()
{
    // Store the string, which has the document to compress in text
    string text = fileToString();

    // Calculate the uncompressed size of the file (all characters times 8 bits which is used to store them)
    double uncompressed_size = text.length() * 8;

    //Calculate the frequencies and characters
    pair<list<char>, list<double> > final = frequencyCalc(text);
    // Store the characters and frequencies in different lists
    list<char> characters = final.first;
    list<double> frequency = final.second;

    // Make the Huffman tree
    HuffmanCodes(characters, frequency); 

    //Find the compression ration
    double compressed_ratio = uncompressed_size / compressed_size;

    cout << compressed_ratio;

    return 1;
}