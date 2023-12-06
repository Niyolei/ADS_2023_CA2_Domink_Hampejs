#include "Functions.h"

DataTag stringToDataTag(string str) {
	if (str == "dir") {
		return directory;
	}
	else if (str == "file") {
		return file;
	}
	else if (str == "/dir") {
		return closeDir;
	}
	else if (str == "/file") {
		return closeFile;
	}
	else if (str == "name") {
		return name;
	}
	else if (str == "/name") {
		return closeName;
	}
	else if (str == "length") {
		return length;
	}
	else if (str == "/length") {
		return closeLength;
	}
	else if (str == "type") {
		return type;
	}
	else if (str == "/type") {
		return closeType;
	}
	else {
		throw logic_error("Invalid tag");
	}

}

DataTag parseTag(stringstream& ss) {
	string item;
	getline(ss, item, '<');
	getline(ss, item, '>');
	return stringToDataTag(item);
}

string getDataFromTag(DataTag closeTag, stringstream& ss) {
	string item;
	string data;
	getline(ss, data, '<');
	getline(ss, item, '>');
	if (stringToDataTag(item) == closeTag)
	{
		return data;
	}
	return "";
}

DTO* getDirData(ifstream& fileStream) {
	string line;
	getline(fileStream, line);

	cout << line << endl;

	stringstream ss(line);

	DataTag tag = parseTag(ss);

	if (tag == name)
	{
		string name = getDataFromTag(closeName, ss);
		if (name != "")
		{
			return new DTO(directory, name);
		}
	}

	return nullptr;
}

DTO* getFileData(ifstream& fileStream) {
	string line;
	getline(fileStream, line);

	cout << line << endl;

	stringstream ss(line);
	string name;
	string length;
	string type;

	DataTag tag = parseTag(ss);
	if (tag == DataTag::name)
	{
		name = getDataFromTag(closeName, ss);
		getline(fileStream, line);
		ss = stringstream(line);
		cout << line << endl;
		tag = parseTag(ss);
	}
	if (tag == DataTag::length)
	{
		length = getDataFromTag(closeLength, ss);
		getline(fileStream, line);
		ss = stringstream(line);
		cout << line << endl;
		tag = parseTag(ss);
	}
	if (tag == DataTag::type)
	{
		type = getDataFromTag(closeType, ss);
	}


	if (name != "" && length != "" && type != "")
	{
		return new DTO(file, name, stoi(length), type);
	}

	return nullptr;
}

void handleStuff(TreeIterator<DTO*>* iter, ifstream& fileStream) {


	string line;
	if (getline(fileStream, line))
	{
		cout << line << endl;

		stringstream ss(line);

		DataTag tag = parseTag(ss);

		switch (tag)
		{
			case directory:
			{
				DTO* data = getDirData(fileStream);
				if (iter->childValid())
				{
					iter->insertChildAfter(data);
				}
				else
				{
					iter->appendChild(data);
				}
				iter->childForth();
				TreeIterator<DTO*>* childIter = new TreeIterator<DTO*>(*iter);
				handleStuff(childIter, fileStream);
			}
			break;
			case closeDir:
				handleStuff(iter, fileStream);
				break;
			case file:
			{
				DTO* data = getFileData(fileStream);
				if (iter->childValid())
				{
					iter->insertChildAfter(data);
				}
				else
				{
					iter->appendChild(data);
				}
				iter->childForth();
				handleStuff(iter, fileStream);
			}
				break;
			case closeFile:
				handleStuff(iter, fileStream);
				break;
			default:
				break;
		}
	}
}



void parseFile(ifstream& fileStream, Tree<DTO*>& emptyTree) {
	string line;
	getline(fileStream, line);

	cout << line << endl;

	stringstream ss(line);

	DataTag tag = parseTag(ss);

	if (tag == directory) {
		DTO* data = getDirData(fileStream);
		if (data != nullptr)
		{
			emptyTree = Tree<DTO*>(data);
			TreeIterator<DTO*>* iter = new TreeIterator<DTO*>(&emptyTree);
			handleStuff(iter, fileStream);
		}
		else
		{
			logic_error("Invalid main directory");
		}
	}
	else {
		logic_error("No main directory");
	}
}

void parseLine(string line) {
	stringstream ss(line);
	string item;
	while (getline(ss, item, ' ')) {
		cout << item << endl;
	}
}

void readXMl(const string& fileName, Tree<DTO*>& emptyTree) {
	stack<string> tagStack;
	ifstream fileStream(fileName);

	if (fileStream)
	{
		parseFile(fileStream, emptyTree);
		fileStream.close();
		cout << emptyTree.data->name << endl;
	}
	else
	{
		logic_error("File not found");
	}
}

