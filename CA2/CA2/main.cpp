
#include "Functions.h"
#include "Tree.h"


void printMenu() {
	cout << "\n1. Get folder content amount" << endl;
	cout << "2. Get folder memory usage" << endl;
	cout << "3. Display folder" << endl;
	cout << "4. Get file path" << endl;
	cout << "5. Prune" << endl;
	cout << "6. Exit" << endl;
	cout << "\nEnter choice: ";
}

void userMenu() {
	Tree<DTO*>* tree = new Tree<DTO*>(nullptr);
	readXMl("../x64/Debug/vs_sample_simple.xml", *tree);
	bool run = true;
	int choice = 0;
	while (run)
	{
		switch (choice)
		{
		case 0:
		{
			printMenu();
			cin >> choice;
		}
		break;
		case 1:
		{
			Tree<DTO*> item(nullptr);
			string path;
			cout << "Enter path: ";
			cin >> path;
			if (!getItemBasedOnPath(tree, path, item))
			{
				cout << "Invalid path" << endl;
			}
			else {
				cout << "Folder content amount: " << getFolderContentAmount(&item) << endl;
			}
			choice = 0;
		}
		break;
		case 2:
		{
			Tree<DTO*> item(nullptr);
			string path;
			cout << "Enter path: ";
			cin >> path;
			if (!getItemBasedOnPath(tree, path, item))
			{
				cout << "Invalid path" << endl;
			}
			else
				cout << "Folder memory usage: " << getFolderMemoryUsage(&item) << endl;
			choice = 0;
		}
		break;
		case 3:
		{
			string path;
			cout << "Enter path: ";
			cin >> path;
			if (!displayFolderByPath(tree, path))
			{
				cout << "Invalid path" << endl;
			}
			choice = 0;
		}
		break;
		case 4:
		{
			string searched;
			cout << "Enter Name: ";
			cin >> searched;
			string path;
			TreeIterator<DTO*> iter(tree);
			getFilePath(searched, iter, path, true);

			cout << "File path: " << path << endl;
			choice = 0;
		}
		break;
		case 5:
		{
			TreeIterator<DTO*> iter(tree);
			prune(iter);
			cout << "Pruned" << endl;
			choice = 0;
		}
		break;
		case 6:
		{
			run = false;
		}
		default:
			choice = 0;
			break;
		}

	}

}



int main() {
	userMenu();
	return 0;
}