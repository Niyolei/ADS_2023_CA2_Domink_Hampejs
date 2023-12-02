#pragma once
#include "Tree.h"

template <class T>
class TreeIterator
{
public:	
	Tree<T> *node;
	DListIterator<Tree<T>*> childIter;

	TreeIterator(Tree<T>* root);
	void resetIterator();
	void root();
	void up();
	void down();
	void childBack();
	void childForth();
	void childStart();
	void childEnd();
	void appendChild(T item);
	void prependChild(T item);
	void insertChildBefore(T item);
	void insertChildAfter(T item);
	void removeChild();
	bool childValid();
	T childItem();
	T item();
};
template <class T>
TreeIterator<T>::TreeIterator(Tree<T>* root)
{
	node = root;
	resetIterator();
}

template <class T>
void TreeIterator<T>::resetIterator()
{
	if (node != nullptr && node->children != nullptr)
	{
		childIter = node->children->getIterator();
	}
	
}

template <class T>
void TreeIterator<T>::root()
{
	while (node->parent != nullptr)
	{
		node = node->parent;
	}
	resetIterator();
}

template <class T>
void TreeIterator<T>::up()
{
	if (node->parent != nullptr)
	{
		node = node->parent;
	}
	resetIterator();
}

template <class T>
void TreeIterator<T>::down()
{
	if (childIter.isValid())
	{
		node = childIter.item();
		resetIterator();
	}
	
}

template <class T>
void TreeIterator<T>::childBack()
{
	if (childIter.isValid()) childIter.previous();
}

template <class T>
void TreeIterator<T>::childForth()
{
	if (childIter.isValid()) childIter.advance();
}

template <class T>
void TreeIterator<T>::childStart()
{
	if (childIter.isValid()) childIter.start();
}

template <class T>
void TreeIterator<T>::childEnd()
{
	if (childIter.isValid()) childIter.end();
}

template <class T>
void TreeIterator<T>::appendChild(T item)
{
	Tree<T> *newChild = new Tree<T>(item);
	newChild->parent = node;
	childIter.list->append(newChild);
	resetIterator();
}

template <class T>
void TreeIterator<T>::prependChild(T item)
{
	Tree<T>* newChild = new Tree<T>(item);
	newChild->parent = node;
	childIter.list->prepend(newChild);
	resetIterator();
}

template <class T>
void TreeIterator<T>::insertChildBefore(T item)
{
	Tree<T>* newChild = new Tree<T>(item);
	newChild->parent = node;
	childIter.list->insert(childIter, newChild);
	resetIterator();
}

template <class T>
void TreeIterator<T>::insertChildAfter(T item)
{
	Tree<T>* newChild = new Tree<T>(item);
	newChild->parent = node;
	childIter.advance();
	childIter.list->insert(childIter, newChild);
	childIter.previous();
	childIter.previous();
}

template <class T>
void TreeIterator<T>::removeChild()
{
	if (childIter.isValid())
	{
		childIter.list->remove(childIter);
		resetIterator();
	}
}

template <class T>
bool TreeIterator<T>::childValid()
{
	return childIter.isValid();
}

template <class T>
T TreeIterator<T>::childItem()
{
	return childIter->node->data;
}

template <class T>
T TreeIterator<T>::item()
{
	return node->data;
}