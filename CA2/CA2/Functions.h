#include <fstream>
#include <iostream>
#include <sstream>
#include <stack>
#include <queue>
using namespace std;
#include "Tree.h"
#include"TreeIterator.h"

enum DataTag
{
	directory,
	closeDir,
	file,
	closeFile,
	name,
	closeName,
	length,
	closeLength,
	type,
	closeType,
	invalid
};



struct DTO
{
	DataTag tag;
	string name;
	int length;
	string type;

	DTO(DTO& other) {
		this->tag = other.tag;
		this->name = other.name;
		this->length = other.length;
		this->type = other.type;
	}

	DTO(DataTag tag, string name) {
		this->tag = tag;
		this->name = name;
	}

	DTO(DataTag tag, string name, int length, string type) {
		this->tag = tag;
		this->name = name;
		this->length = length;
		this->type = type;
	}

	void operator=(DTO& other) {
		this->tag = other.tag;
		this->name = other.name;
		this->length = other.length;
		this->type = other.type;
	}

};

DataTag stringToDataTag(string str);
DataTag parseTag(stringstream& ss);
string getDataFromTag(DataTag closeTag, stringstream& ss);
DTO* getDirData(ifstream& fileStream);
DTO* getFileData(ifstream& fileStream);
bool verifyTag(DataTag tag, stack<DataTag>& verifier);
void handleSection(TreeIterator<DTO*>* iter, ifstream& fileStream, stack<DataTag>& verifier);
void parseFile(ifstream& fileStream, Tree<DTO*>& emptyTree);
void parseLine(string line);
void readXMl(const string& fileName, Tree<DTO*>& emptyTree);

bool getItemBasedOnPath(Tree<DTO*>* tree, string& path, Tree<DTO*>& foundItem);
int getFolderContentAmount(Tree<DTO*>* tree);
int getFolderMemoryUsage(Tree<DTO*>* tree);
bool getFilePath(string& searched, TreeIterator<DTO*> tree,string& path, bool isTop);
void displayFolder(TreeIterator<DTO*> tree);
bool displayFolderByPath(Tree<DTO*>* tree, string& path);