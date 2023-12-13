
#include "Functions.h"
#include "Tree.h"



int main() {
	Tree<DTO*> tree = Tree<DTO*>(nullptr);
	readXMl("../x64/Debug/vs_sample_simple.xml", tree);
	cout << "Testing" << endl;
	cout << tree.data->name << endl;
	cout << tree.count() << endl;
	cout<< tree.children->getIterator().item()->data->name << endl;

	string path = "ADS_Single_LinkedList_Exercises/.git/config";
	Tree<DTO*> foundItem = Tree<DTO*>(nullptr);
	getItemBasedOnPath(&tree, path, foundItem);

	displayFolder(TreeIterator<DTO*>(&tree));

	string searched = "config";
	path = "";
	TreeIterator<DTO*> iter(&tree);

	getFilePath(searched,iter, path,true);
	cout << "\n" << path << endl;
	cout << endl;

	iter.down();
	displayFolder(iter);

	path = "ADS_Single_LinkedList_Exercises";
	displayFolderByPath(&tree, path);

	TreeIterator<DTO*> iter2(&tree);
	prune(iter2);

	cout << "\n\nThe Pruned tree should be here" << endl;
	path = "ADS_Single_LinkedList_Exercises";
	displayFolderByPath(&tree, path);

	return 0;
}