
#include "Functions.h"
#include "Tree.h"

int main() {
	Tree<DTO*>* tree = readXMl("../CA2_Test/XML_JustDir.xml");
	cout << "Testing" << endl;
	cout << tree->data->name << endl;
	return 0;
}