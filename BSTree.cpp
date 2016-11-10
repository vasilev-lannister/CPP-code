#include <iostream>
#include <vector>
#include <assert.h>

using namespace std;

template <typename T>
struct Node {
    T data;
    Node* left;
    Node* right;
    Node(T data, Node* left=NULL, Node* right = NULL): data(data), left(left), right(right) {}
};

template <typename T>
class BSTree
{
    Node<T>* root;

    public:
        BSTree();
        BSTree(const BSTree<T>&);
        BSTree<T>& operator=(const BSTree<T>&);
        virtual ~BSTree();
        bool contains(const T&) const;
        void add(const T&);
        void remove(const T&);
        void print() const;
        vector<T> preorderTraversal() const;
        vector<T> inorderTraversal() const;
        vector<T> postorderTraversal() const;
        static BSTree<T>* reconstruct(const vector<T>& inorder,const vector<T>& preorder);
        static Node<T>* reconstructNodes(const vector<T>& inorder,const vector<T>& preorder);
        int getHeight();

    private:
        void deleteAll(Node<T>*);
        Node<T>*& find(const T&, Node<T>*&);
        void print(Node<T>*, int) const;
        void preorderTraverse(Node<T>*, vector<T>&) const;
        void inorderTraverse(Node<T>*, vector<T>&) const;
        void postorderTraverse(Node<T>*, vector<T>&) const;
        int getHeight(Node<T>*);
        Node<T>* copy(const Node<T>*);
};

template <typename T>
BSTree<T>::BSTree():root(NULL) {}

template <typename T>
Node<T>* BSTree<T>::copy(const Node<T>* node)
{
    if(node == nullptr)
    {
        return nullptr;
    }
    Node<T>* result = new Node<T>(node->data);
    result->left = copy(node->left);
    result->right = copy(node->right);
    return result;
}

template <typename T>
BSTree<T>::BSTree(const BSTree<T>& other)
{

}

template <typename T>
BSTree<T>& BSTree<T>::operator=(const BSTree<T>& other) {
    if(this != &other)
    {
        deleteAll(root);
        copy(root);
    }
    return *this;
}

template <typename T>
BSTree<T>::~BSTree() {
    deleteAll(root);
}

template <typename T>
void BSTree<T>::deleteAll(Node<T>* node) {
    if (node == NULL) {
        return;
    }
    deleteAll(node->left);
    deleteAll(node->right);
    delete node;
}

template <typename T>
Node<T>*& BSTree<T>::find(const T& el, Node<T>*& node) {
    if (node == NULL || node->data == el) {
        return node;
    }
    if (node->data < el) {
        return find(el, node->right);
    }
    return find(el, node->left);
}

template <typename T>
bool BSTree<T>::contains(const T& el) const {
    return find(el, root) != NULL;
}

template <typename T>
void BSTree<T>::add(const T& el) {
    Node<T>*& existing = find(el, root);
    if (existing == NULL) {
        existing = new Node<T>(el);
    }
}

template <typename T>
void BSTree<T>::remove(const T& el) {
    Node<T>*& existing = find(el, root);
    if (existing == NULL) {
        // nothing to do
        return;
    }
    // find rightmost node of left child
    // and attach the right child to it
    Node<T>* toDelete = existing;
    Node<T>* replacement = existing->left;
    if (replacement == NULL) {
        replacement = existing->right;
    } else {
        Node<T>* rightmost = existing->left;
        while(rightmost->right != NULL) {
            rightmost = rightmost->right;
        }
        rightmost->right = existing->right;
    }
    // This will update the pointer in the parent because existing is a reference
    existing = replacement;
    // this deletes the old pointer
    delete toDelete;
}

template <typename T>
void BSTree<T>::print() const {
    print(root, 0);
}
template <typename T>
void BSTree<T>::print(Node<T>* node, int indent) const {
    if (node == NULL) {
        return;
    }
    print(node->right, indent + 1);
    for (int i = 0; i < indent; i++) {
        cout << '\t';
    }
    cout << node->data << endl;
    print(node->left, indent + 1);
}

template <typename T>
vector<T> BSTree<T>::preorderTraversal() const {
    vector<T> result;
    preorderTraverse(root, result);
    return result;
}

template <typename T>
void BSTree<T>::preorderTraverse(Node<T>* node, vector<T>& result) const {
    if (node == NULL) {
        return;
    }
    result.push_back(node->data);
    preorderTraverse(node->left, result);
    preorderTraverse(node->right, result);
}

template <typename T>
vector<T> BSTree<T>::inorderTraversal() const {
    vector<T> result;
    inorderTraverse(root, result);
    return result;
}
template <typename T>
void BSTree<T>::inorderTraverse(Node<T>* node, vector<T>& result) const {
    if (node == NULL) {
        return;
    }
    inorderTraverse(node->left, result);
    result.push_back(node->data);
    inorderTraverse(node->right, result);
    }

template <typename T>
vector<T> BSTree<T>::postorderTraversal() const {
    vector<T> result;
    postorderTraverse(root, result);
    return result;
}
template <typename T>
void BSTree<T>::postorderTraverse(Node<T>* node, vector<T>& result) const {
    if (node == NULL) {
        return;
    }
    postorderTraverse(node->left, result);
    postorderTraverse(node->right, result);
    result.push_back(node->data);
}

void printVector(const vector<int>& vec) {
    for (vector<int>::const_iterator it = vec.begin(); it != vec.end(); ++it) {
        cout << *it << " ";
    }
}

template <typename T>
vector<T> getSubVector2(const vector<T> vec,int from,int to) //using vector constructor with params
{
    return vector<T>(vec,vec.begin()+from,vec.begin()+to);
}


template <typename T>
vector<T> getSubVector(const vector<T> vec,int from,int to)
{
    vector<T> result;
    for(int i=from;i<to;i++)
    {
        result.push_back(vec[i]);
    }
    return result;
}

template <typename T>
int findIdx(const T& el,const vector<T>& vec)
{
    for(int i=0;i<vec.size();i++)
    {
        if(el==vec[i])
            return i;
    }
    return -1;
}


template <typename T>
BSTree<T>* BSTree<T>::reconstruct(const vector<T>& inorder,const vector<T>& preorder) // works for unordered BTrees aswell
{
    Node<T>* rootResult = reconstructNodes(inorder,preorder);
    return new BSTree<T>(rootResult);
}

template <typename T>
Node<T>* BSTree<T>::reconstructNodes(const vector<T>& inorder,const vector<T>& preorder) // is static aswell
{
    if(preorder.size() == 0)// no need to check if inorder.size is 0 because they have the same length
    {
        return nullptr;
    }
    T rootData = preorder[0];
    int rootIndex = findIdx(rootData,inorder);
    Node<T>* result = new Node<T>(rootData);
    result->left = reconstructNodes(getSubVector(inorder,0,rootIndex), getSubVector(preorder,1,rootIndex));
    result->rigt = reconstructNodes(getSubVector(inorder,rootIndex+1,inorder.size()),
                                    getSubVector(preorder,rootIndex+1,preorder.size()));

    return result;
}

template <typename T>
int BSTree<T>::getHeight()
{
    return getHeight(root);
}

template <typename T>
int BSTree<T>::getHeight(Node<T>* node)
{
    if(node == nullptr)
    {
        return 0;
    }
    return max(getHeight(node->left),getHeight(node->right))+1;
}




void testTraversals()
{
    BSTree<int> bst;
    bst.add(2);
    bst.add(3);
    bst.add(1);
    bst.add(7);
    bst.add(-4);
    bst.add(5);
    bst.add(0);
    cout << "Constructed tree: " << endl;
    bst.print();
    cout << endl;
    cout << "Inorder traversal: ";
    printVector(bst.inorderTraversal());
    cout << endl;
    cout << "Postorder traversal: ";
    printVector(bst.postorderTraversal());
    cout << endl;
    cout << "Preorder traversal: ";
    printVector(bst.preorderTraversal());
    cout << endl;
}

void testRemovingRoot()
{
    BSTree<int> bst;
    bst.add(10);
    bst.add(-20);
    bst.add(15);
    bst.add(5);
    bst.add(10);
    bst.add(-5);
    bst.add(17);
    cout << endl << "Constructed tree: " << endl;
    bst.print();
    cout << endl;
    bst.remove(10);
    cout << endl << "After removing the root: " << endl;
//    bst.print();
//    cout<<"height"<<bst.getHeight();

}

void testCopyConstructor()
{
    BSTree<int> bst;
    bst.add(10);
    bst.add(-20);
    bst.add(15);
    bst.add(5);
    bst.add(10);
    bst.add(-5);
    bst.add(17);
    BSTree<int> toCopy(bst);
    toCopy.print();
}

int main()
{

    testCopyConstructor();

    //testTraversals();
    //testRemovingRoot();

//    vector<int> bigv;
//    bigv.push_back(1);
//    bigv.push_back(2);
//    bigv.push_back(3);
//    bigv.push_back(4);
//    bigv.push_back(5);
//    vector<int> partv(bigv,bigv.begin()+1,bigv.begin()+3);


    return 0;
}
