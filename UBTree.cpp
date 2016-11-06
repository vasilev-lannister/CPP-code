#include <iostream>
#include <assert.h>
#include <string.h>
#include <fstream>


using namespace std;

template <class T>
using mapFn = T (*)(const T&);

template <class T>
struct Node
{
	Node<T> *left, *right;
	T data;

	Node (const T& d,Node<T> *l, Node<T> *r):
	    left(l),right(r),data(d)
    {
    	makeID();
    }
	Node ():left(NULL),right(NULL)
	{
		makeID();
	}

	int getID() const
	{
		return id;
	}

private:
	int id;

	void makeID ()
	{
		static int maxID = 0;
		maxID++;
    	id = maxID;

	}

};

template <class T>
class BTree
{
private:
	Node<T> *root;
	void simplePrint (Node<T> *subTreeRoot) const;
	bool add (const T& data, const char *trace, Node<T>*&);
	void deleteAll (Node<T> *subTreeRoot);
	bool member (const T&,Node<T> *subTreeRoot) const;
	void map (mapFn<T>,Node<T> *subTreeRoot);

	void dottyPrint (Node<T> *subTreeRoot,ostream& out) const;

	void serialize (Node<T>*, ostream&) const;
	Node<T>* parseTree (istream &in);

	int count (Node<T>*) const;
	int countEvens(Node<T>*) const;
    int searchCount (bool (*)(const T&), Node<T>*) const;
    int height(Node<T>*) const;
    int countLeaves(Node<T>*) const;

public:
	BTree();
	BTree<T>& add (const T& data, const char *trace);

	void deserialize (istream&);

	void simplePrint () const;
	void dottyPrint (ostream&);
	//bool isEmpty();
	bool member (const T&) const;

	void map (mapFn<T>);

	void serialize (ostream&)const;

	int count () const;
	int countEvens() const;
	int searchCount (bool (*)(const T&)) const;
	int height() const;
	int countLeaves() const;

	~BTree();

};


template<class T>
int BTree<T>::height(Node<T>* subTreeRoot) const
{
    if(subTreeRoot == nullptr)
        return 0;
    int left= height(subTreeRoot->left);
    int right= height(subTreeRoot->right);

    return (left > right) ? left+1 : right+1;
}


template<class T>
int BTree<T>::height () const
{
    return height(root);
}


template<class T>
int BTree<T>::countLeaves(Node<T> *subTreeRoot) const
{
    if(subTreeRoot == nullptr)
        return 0;
    if(subTreeRoot->left == nullptr && subTreeRoot->right ==nullptr)
        return 1;
    return (countLeaves(subTreeRoot->left) + countLeaves(subTreeRoot->right));
}

template<class T>
int BTree<T>::countLeaves() const
{
    return countLeaves(root);
}


template<class T>
int BTree<T>::count(Node<T> *subTreeRoot) const
{
    if(subTreeRoot == nullptr)
        return 0;
    return (1 + count(subTreeRoot->left) + count(subTreeRoot->right));
}

template<class T>
int BTree<T>::count() const
{
    return count(root);
}

template<class T>
int BTree<T>::countEvens(Node<T> *subTreeRoot) const
{
    if(subTreeRoot == nullptr)
    {
        return 0;
    }

    if(!(subTreeRoot->data%2))
    {
        return (1 + countEvens(subTreeRoot->left) + countEvens(subTreeRoot->right));
    }
    return (countEvens(subTreeRoot->left) + countEvens(subTreeRoot->right));

}

template<class T>
int BTree<T>::countEvens() const
{
    return countEvens(root);
}


template<class T>
void BTree<T>::serialize (Node<T> *subTreeRoot, ostream &out) const
{
	if (subTreeRoot == NULL)
	{
		out << "null ";
		return;
	}

	out << subTreeRoot->data << " ";

	serialize (subTreeRoot->left,out);
	serialize (subTreeRoot->right,out);

}

template<class T>
void BTree<T>::serialize (ostream &out) const
{
	serialize (root,out);
	cout << endl;
}



template<class T>
void BTree<T>::dottyPrint (ostream &out)
{
	out << "digraph G {" << endl;
	dottyPrint (root,out);
	out << "}" << endl;
}



template<class T>
void BTree<T>::dottyPrint (Node<T> *subTreeRoot,ostream& out) const
{
	if (subTreeRoot == NULL)
		return;

	out << subTreeRoot->getID()
	    << "[label=\""
	    << subTreeRoot->data
	    << "\"];" << endl;

	if (subTreeRoot->left != NULL)
		out << subTreeRoot->getID()
	        <<"->"
	        << subTreeRoot->left->getID()
	        << endl;

	if (subTreeRoot->right != NULL)
		out << subTreeRoot->getID()
	        <<"->"
	        << subTreeRoot->right->getID()
	        << endl;

	dottyPrint (subTreeRoot->left,out);
	dottyPrint (subTreeRoot->right,out);
}



template<class T>
void BTree<T>::map (mapFn<T> f,Node<T> *subTreeRoot)
{
	if (subTreeRoot == NULL)
		return;

	subTreeRoot->data = f (subTreeRoot->data);
	map (f,subTreeRoot->left);
	map (f,subTreeRoot->right);
}

template<class T>
void BTree<T>::map (mapFn<T>f)
{
	map (f,root);
}


template<class T>
bool BTree<T>::member (const T& x) const
{
	return member (x,root);
}


template<class T>
bool BTree<T>::member (const T& x,Node<T> *subTreeRoot) const
{
	if (subTreeRoot == NULL)
		return false;


	return subTreeRoot->data == x ||
	       member (x,subTreeRoot->left) ||
	       member (x,subTreeRoot->right);


}


template<class T>
void BTree<T>::deleteAll (Node<T> *subTreeRoot)
{
	if (subTreeRoot == NULL)
		return;

	deleteAll (subTreeRoot->left);
	deleteAll (subTreeRoot->right);
	delete subTreeRoot;
}

template <class T>
BTree<T>::~BTree()
{
	deleteAll (root);

}


template <class T>
BTree<T>& BTree<T>::add(const T& x, const char *trace)
{
   add (x,trace,root);
   return *this;
}


template <class T>
bool BTree<T>::add(const T& x, const char *trace, Node<T>* &subTreeRoot)
{
	if (subTreeRoot == NULL)
	{
		assert (strlen(trace) == 0);
		subTreeRoot = new Node<T> (x,NULL,NULL);
		return true;
	}

	assert (strlen(trace)>0);

	if (trace[0]=='L')
		return add (x,trace+1,subTreeRoot->left);

	assert (trace[0]=='R');
	return add (x,trace+1,subTreeRoot->right);

	return true;

}

template <class T>
BTree<T>::BTree ():root(NULL){}

template <class T>
void BTree<T>::simplePrint() const
{
	simplePrint (root);
	cout << endl;
}

template <class T>
void BTree<T>::simplePrint(Node<T> *subTreeRoot) const
{
	if (subTreeRoot == NULL)
		return;

	cout << subTreeRoot->data << " ";
	simplePrint (subTreeRoot->left);
	simplePrint (subTreeRoot->right);
}

void removeWhite (istream &in)
{
	while (in.peek() <= 32)
		in.get();
}

template <class T>
Node<T>* BTree<T>::parseTree (istream &in)
{

	removeWhite (in);

	if (in.peek() == 'n')
	{
		string dummy;
		in >> dummy;
		assert (dummy == "null");
		return NULL;
	}

	T data;
	in >> data;

	return new Node<T> (data,
		 				parseTree(in),
		 				parseTree(in));


}


template <class T>
void BTree<T>::deserialize (istream &in)
{
	deleteAll(root);

	root = parseTree (in);


}


void testMember ()
{
	BTree<int> t;

	t.add(10,"").add(12,"L").add(14,"R").add(15,"LR");

	assert (t.member(12) == true);
	assert (t.member(18) == false);
	assert (t.member(15) == true);
}


int plusOne (const int& i)
{
	return i + 1;
}


int main ()
{

    testMember ();

	BTree<int> t;

	t.add(10,"").add(12,"L").add(14,"R").add(15,"LR");
	t.simplePrint();
    t.add(3,"RR");
    t.add(3,"LL");
    t.add(3,"LRR");
    t.add(51,"LRL");
	cout<<t.height()<<endl;
	cout<<t.countLeaves()<<endl;
	return 0;
}
