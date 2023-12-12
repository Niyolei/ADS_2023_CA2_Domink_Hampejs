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

bool verifyTag(DataTag tag, stack<DataTag>& verifier) {
	if (tag == directory || tag == file)
	{
		verifier.push(tag);
		return true;
	}
	else if (tag == closeDir || tag == closeFile)
	{
		if ((int)verifier.top() == (int)tag - 1)
		{
			verifier.pop();
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}

void validateDTO(DTO* data) {
	if (data == nullptr)
	{
		throw logic_error("Invalid Data");
	}
}

void handleSection(TreeIterator<DTO*>* iter, ifstream& fileStream, stack<DataTag>& verifier) {
	string line;
	while (getline(fileStream, line))
	{
		cout << line << endl;

		stringstream ss(line);

		DataTag tag = parseTag(ss);

		if (!verifyTag(tag, verifier))
		{
			throw logic_error("Invalid Nesting");
		}

		switch (tag)
		{
		case directory:
		{
			DTO* data = getDirData(fileStream);

			validateDTO(data);

		
			iter->appendChild(data);
			iter->childEnd();
			
			iter->down();
		}
		break;
		case closeDir:
		{
			iter->up();
			iter->childBack();
		}
		break;
		case file:
		{
			DTO* data = getFileData(fileStream);

			validateDTO(data);

			iter->appendChild(data);
			iter->childEnd();
		}
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
			stack<DataTag> verifier;
			verifier.push(directory);
			handleSection(iter, fileStream, verifier);
			if (!verifier.empty())
			{
				throw logic_error("Invalid Nesting");
			}
		}
		else
		{
			throw logic_error("Invalid main directory");
		}
	}
	else {
		throw logic_error("No main directory");
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
	}
	else
	{
		throw logic_error("File not found");
	}
}

bool getItemBasedOnPath(Tree<DTO*>* tree, string& path, Tree<DTO*>& foundItem) {
	stringstream ss(path);
	string item;
	bool found = true;
	getline(ss, item, '/');
	if (tree->getData()->name == item)
	{
		TreeIterator<DTO*>* iter = new TreeIterator<DTO*>(tree);
		while (getline(ss, item, '/'))
		{
			found = false;
			while (iter->childValid() && !found)
			{
				if (iter->childItem()->name == item)
				{
					iter->down();
					found = true;
				}
				else
				{
					iter->childForth();
				}
			}
		}
		if (found)
		{
			foundItem = *iter->node;
			return true;
		}
		else
			return false;
	}
	else 
		return false;
}

int getFolderContentAmount(Tree<DTO*>* tree) {
	if (tree->data->tag != directory)
	{
		return -1;
	}

	int amount = 0;
	TreeIterator<DTO*>* iter = new TreeIterator<DTO*>(tree);
	while (iter->childValid())
	{
		amount += iter->childIter.item()->count();
		iter->childForth();
	}
	return amount;
}

int getFolderMemoryUsage(Tree<DTO*>* tree)
{
	if (tree->data->tag != directory)
	{
		return -1;
	}
	int amount = 0;
	queue<Tree<DTO*>*> q;
	q.push(tree);
	while (!q.empty()) {
		if (q.front()->data->tag == file)
		{
			amount += q.front()->data->length;
		}
		DListIterator<Tree<DTO*>*> iter = q.front()->children->getIterator();
		while (iter.isValid())
		{
			q.push(iter.item());
			iter.advance();
		}
		q.pop();
	}
	return amount;
}

