
#include "Functions.h"
#include "Tree.h"

void displayTree(TreeIterator<DTO*> iter, string indent)
{
	cout << indent << iter.item()->name << endl;
	while (iter.childValid()) {
		TreeIterator<DTO*> child(iter);
		child.down();
		displayTree(child, indent + "\t");
		iter.childForth();
	}
}

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

	displayTree(TreeIterator<DTO*>(&tree), "");

	string searched = "config";
	path = "";
	TreeIterator<DTO*> iter(&tree);

	getFilePath(searched,iter, path,true);
	cout << path << endl;

	return 0;
}