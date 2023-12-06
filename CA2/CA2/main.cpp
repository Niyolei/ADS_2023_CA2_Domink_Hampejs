
#include "Functions.h"
#include "Tree.h"

int main() {
	Tree<DTO*> tree = Tree<DTO*>(nullptr);
	readXMl("../CA2_Test/vs_sample_simple.xml", tree);
	cout << "Testing" << endl;
	cout << tree.data->name << endl;
	cout << tree.count() << endl;
	cout<< tree.children->getIterator().item()->data->name << endl;
	return 0;
}