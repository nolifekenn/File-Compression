#include <iostream>
#include <fstream>
#include <unordered_map>
using namespace std;

struct Node {
    char data;
    int freq;
    Node* left;
    Node* right;
};

// Function to create a new node
Node* createNode(char data, int freq, Node* left = nullptr, Node* right = nullptr) {
    Node* newNode = new Node{data, freq, left, right};
    return newNode;
}

// Function to deserialize the Huffman tree from the file
Node* deserializeTree(ifstream& treeFile) {
    char data;
    int freq;
    string nodeType;
    treeFile >> data >> freq >> nodeType;

    if (nodeType == "L") {
        return createNode(data, freq);
    } else {
        Node* left = deserializeTree(treeFile);
        Node* right = deserializeTree(treeFile);
        return createNode('\0', freq, left, right);
    }
}

// Function to decompress the text and save it to a file
void decompressText(Node* root, ifstream& compressedFile, ofstream& outputFile) {
    Node* currentNode = root;
    char bit;
    while (compressedFile >> bit) {
        if (bit == '0') {
            currentNode = currentNode->left;
        } else if (bit == '1') {
            currentNode = currentNode->right;
        }

        if (currentNode->left == nullptr && currentNode->right == nullptr) {
            outputFile << currentNode->data;
            currentNode = root;
        }
    }
}

int main() {
    string compressedFileName = "compressed.txt";
    string treeFileName = "compressedTree.txt";
    string decompressedFileName = "decompressed.txt";

    // Deserialize the Huffman tree from the file
    ifstream treeFile(treeFileName);
    Node* root = deserializeTree(treeFile);
    treeFile.close();

    // Decompress the text and save it to a file
    ifstream compressedFile(compressedFileName, ios::binary);
    ofstream outputFile(decompressedFileName);
    decompressText(root, compressedFile, outputFile);
    compressedFile.close();
    outputFile.close();

    cout << "File decompressed successfully!" << endl;

    return 0;
}