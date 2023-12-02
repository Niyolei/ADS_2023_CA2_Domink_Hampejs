#include <fstream>
#include <iostream>
#include <sstream>
#include <stack>
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
	closeType
};


struct DTO
{
	DataTag tag;
	string name;
	int length;
	string type;

	DTO() {
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
};

DataTag stringToDataTag(string str) {
	if (str == "<dir") {
		return directory;
	}
	else if (str == "<file") {
		return file;
	}
	else if (str == "/dir") {
		return closeDir;
	}
	else if (str == "/file") {
		return closeFile;
	}
	else if (str == "<name") {
		return name;
	}
	else if (str == "/name") {
		return closeName;
	}
	else if (str == "<length") {
		return length;
	}
	else if (str == "/length") {
		return closeLength;
	}
	else if (str == "<type") {
		return type;
	}
	else if (str == "/type") {
		return closeType;
	}
	else {
		throw logic_error("Invalid tag");
	}

}

string getDataFromTag(DataTag tag, stringstream& ss) {
	string item;
	string data;
	getline(ss, data, '<');
	getline(ss, item, '>');
	if (stringToDataTag(item) == tag)
	{
		return data;
	}
	return "";
}

DTO* getDirData(ifstream& fileStream) {
	string line;
	getline(fileStream, line);

	stringstream ss(line);

	string name = getDataFromTag(closeName , ss);

	if (name != "")
	{
		return new DTO(directory, name);
	}
	else
	{
		return nullptr;
	}
}

DTO* getFileData(ifstream& fileStream) {
	string line;
	getline(fileStream, line);

	stringstream ss(line);

	string name = getDataFromTag(closeName, ss);
	getline(fileStream, line);
	string length = getDataFromTag(closeLength, ss);
	getline(fileStream, line);
	string type = getDataFromTag(closeType, ss);

	if (name != "" && length != "" && type != "")
	{
		return new DTO(file, name, stoi(length), type);
	}
	else
	{
		return nullptr;
	}
}

void handleStuff(TreeIterator<DTO*>* iter, ifstream& fileStream) {

	string line;
	getline(fileStream, line);
	stringstream ss(line);
	string item;
	getline(ss, item, '>');
	DataTag tag = stringToDataTag(item);

	switch (tag)
	{
	case directory: 
	{
		iter->insertChildAfter(getDirData(fileStream));
		iter->childForth();
		TreeIterator<DTO*>* childIter = new TreeIterator<DTO*>(*iter);
		handleStuff(childIter, fileStream);
	}
		break;
	case closeDir:
		//Check
		break;
	case file:
		//Handle file data
		//Add new tree with data
		//Advance iterator
		break;
	case closeFile:
		//Check
		break;
	default:
		break;
	}
}


DataTag parseTag(stringstream& ss) {
	string item;
	getline(ss, item, '>');
	return stringToDataTag(item);
}

string parseName(stringstream& ss) {
	string item;
	getline(ss, item, '<');
	return item;
}

Tree<DTO*>* parseFile(ifstream& fileStream) {
	string line;
	getline(fileStream, line);

	stringstream ss(line);

	Tree<DTO*> tree = Tree<DTO*>(nullptr);
	DTO temp;

	temp.tag = parseTag(ss);
	temp.name = parseName(ss);

	if (temp.tag == directory)
	{
		tree = Tree<DTO*>(new DTO(temp));
		if (getline(fileStream, line))
		{
			temp.tag = parseTag(ss);
			if (temp.tag != closeDir)
			{
				TreeIterator<DTO*> iter = TreeIterator<DTO*>(&tree);
				
			}

		}
	}
	else
	{
		logic_error("No main directory");
	}

	

	return &tree;
}

void parseLine(string line) {
	stringstream ss(line);
	string item;
	while (getline(ss, item, ' ')) {
		cout << item << endl;
	}
}

Tree<DTO*>* readXMl(const string& fileName) {
	stack<string> tagStack;
	ifstream fileStream(fileName);

	if (fileStream)
	{
		Tree<DTO*>* tree = parseFile(fileStream);
		fileStream.close();
		return tree;
	}
	else
	{
		cout << "File not found" << endl;
		Tree<DTO*> tree = Tree<DTO*>(nullptr);
		return &tree;
	}
}

