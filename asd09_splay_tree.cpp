#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class SplayTreeMap
{
public:
    struct Node
    {
        Node *lChild, *rChild, *parent;
        string key;
        vector<Node*> references;
        Node()
        {
            lChild = rChild = parent = nullptr;
        }
        Node(const string &str)
        {
            key = str;
            lChild = rChild = parent = nullptr;
        }
    };
private:
    Node *root1, *root2;
    Node *search(const string &key, Node *root);
    Node *splay(Node *vertex, Node *root);
    void leftRotate(Node *vertex, int rootNo);
    void rightRotate(Node *vertex, int rootNo);
public:
    SplayTreeMap();
    void add(const string &bandName, const string &albumTitle);
    void printAlbums(const string &bandName);
    void printBands(const string &albumTitle);
};

SplayTreeMap::SplayTreeMap()
{
    root1 = root2 = nullptr;
}

void SplayTreeMap::add(const string &bandName, const string &albumTitle)
{
    Node *addBandNode = search(bandName, root1);
    Node *addAlbumNode = search(albumTitle, root2);
    if (addBandNode == nullptr)
    {
        root1 = new Node(bandName);
    }
    else if (bandName != addBandNode->key)
    {
        // dodaj nowy element
        Node *newNode = new Node(bandName);
        if (bandName > addBandNode->key)
        {
            newNode->lChild = addBandNode;
            addBandNode->parent = newNode;
            newNode->rChild = addBandNode->rChild;
            if (addBandNode->rChild != nullptr)
                addBandNode->rChild->parent = newNode;
            addBandNode->rChild = nullptr;
            root1 = newNode;
        }
        else
        {
            newNode->rChild = addBandNode;
            addBandNode->parent = newNode;
            newNode->lChild = addBandNode->lChild;
            if (addBandNode->lChild != nullptr)
                addBandNode->lChild->parent = newNode;
            addBandNode->lChild = nullptr;
            root1 = newNode;
        }
    }

    if (addAlbumNode == nullptr)
    {
        root2 = new Node(albumTitle);
    }
    else if (albumTitle != addAlbumNode->key)
    {
        // dodaj nowy element
        Node *newNode = new Node(albumTitle);
        if (albumTitle > addAlbumNode->key)
        {
            newNode->lChild = addAlbumNode;
            addAlbumNode->parent = newNode;
            newNode->rChild = addAlbumNode->rChild;
            if (addAlbumNode->rChild != nullptr)
                addAlbumNode->rChild->parent = newNode;
            addAlbumNode->rChild = nullptr;
            root2 = newNode;
        }
        else
        {
            newNode->rChild = addAlbumNode;
            addAlbumNode->parent = newNode;
            newNode->lChild = addAlbumNode->lChild;
            if (addAlbumNode->lChild != nullptr)
                addAlbumNode->lChild->parent = newNode;
            addAlbumNode->lChild = nullptr;
            root2 = newNode;
        }
    }

    root1->references.push_back(root2);
    root2->references.push_back(root1);
}

void SplayTreeMap::printAlbums(const string &bandName)
{
    Node *node = search(bandName, root1);
    if (node != nullptr && bandName == node->key)
    {
        for (Node *n: node->references)
        {
            cout << n->key << "\n";
        }
    }
}

void SplayTreeMap::printBands(const string &albumTitle)
{
    Node *node = search(albumTitle, root2);
    if (node != nullptr && albumTitle == node->key)
    {
        for (Node *n: node->references)
        {
            cout << n->key << "\n";
        }
    }
}

SplayTreeMap::Node *SplayTreeMap::search(const string &key, Node *root)
{
    Node *prev = nullptr;
    Node *vertex = root;
    while (vertex != nullptr && vertex->key != key)
    {
        if (vertex->key < key)
        {
            prev = vertex;
            vertex = vertex->rChild;
        }
        else
        {
            prev = vertex;
            vertex = vertex->lChild;
        }
    }
    if (vertex == root)
    {
        return vertex;
    }
    if (vertex == nullptr)
    {
        return splay(prev, root);
    }
    else
    {
        return splay(vertex, root);
    }
}

SplayTreeMap::Node *SplayTreeMap::splay(Node *vertex, Node *root)
{
    int whichRoot;
    if (root == nullptr)
        return nullptr;
    if (root == root1)
    {
        whichRoot = 1;
    }
    else
    {
        whichRoot = 2;
    }
    Node *grandParent;
    while (true)
    {
        if (whichRoot == 1)
        {
            if (vertex == root1)
            {
                return root1;
            }
            if (vertex->parent == root1)
            {
                if (vertex == root1->lChild)
                {
                    rightRotate(root1, whichRoot);
                }
                else
                {
                    leftRotate(root1, whichRoot);
                }
                return vertex;
            }
        }
        else
        {
            if (vertex == root2)
            {
                return root2;
            }
            if (vertex->parent == root2)
            {
                if (vertex == root2->lChild)
                {
                    rightRotate(root2, whichRoot);
                }
                else
                {
                    leftRotate(root2, whichRoot);
                }
                return vertex;
            }
        }
        grandParent = vertex->parent->parent;
        // Ojciec lewym synem dziadka
        if (vertex->parent == grandParent->lChild)
        {
            // Syn prawym synem ojca
            if (vertex == vertex->parent->rChild)
            {
                leftRotate(vertex->parent, whichRoot);
                rightRotate(grandParent, whichRoot);
            }
            // Syn lewym synem ojca
            else
            {
                rightRotate(grandParent, whichRoot);
                rightRotate(vertex->parent, whichRoot);
            }
        }
        // Ojciec prawym synem dziadka
        else if (vertex->parent == grandParent->rChild)
        {
            // Syn lewym synem ojca
            if (vertex == vertex->parent->lChild)
            {
                rightRotate(vertex->parent, whichRoot);
                leftRotate(grandParent, whichRoot);
            }
            // Syn prawym synem ojca
            else
            {
                leftRotate(grandParent, whichRoot);
                leftRotate(vertex->parent, whichRoot);
            }
        }
    }
}

void SplayTreeMap::leftRotate(Node *vertex, int rootNo)
{
    Node *rchild = vertex->rChild;
    vertex->rChild = rchild->lChild;
    if (rchild->lChild != nullptr)
    {
        rchild->lChild->parent = vertex;
    }
    if (vertex->parent == nullptr)
    {
        if (rootNo == 1)
        {
            root1 = rchild;
        }
        else
        {
            root2 = rchild;
        }
        rchild->parent = nullptr;
    }
    else if (vertex == vertex->parent->lChild)
    {
        vertex->parent->lChild = rchild;
        rchild->parent = vertex->parent;
    }
    else if (vertex == vertex->parent->rChild)
    {
        vertex->parent->rChild = rchild;
        rchild->parent = vertex->parent;
    }
    vertex->parent = rchild;
    rchild->lChild = vertex;
}

void SplayTreeMap::rightRotate(Node *vertex, int rootNo)
{
    Node *lchild = vertex->lChild;
    vertex->lChild = lchild->rChild;
    if (lchild->rChild != nullptr)
    {
        lchild->rChild->parent = vertex;
    }
    if (vertex->parent == nullptr)
    {
        if (rootNo == 1)
        {
            root1 = lchild;
        }
        else
        {
            root2 = lchild;
        }
        lchild->parent = nullptr;
    }
    else if (vertex == vertex->parent->rChild)
    {
        vertex->parent->rChild = lchild;
        lchild->parent = vertex->parent;
    }
    else if (vertex == vertex->parent->lChild)
    {
        vertex->parent->lChild = lchild;
        lchild->parent = vertex->parent;
    }
    vertex->parent = lchild;
    lchild->rChild = vertex;
}

int main()
{
    ios_base::sync_with_stdio(false);
    SplayTreeMap m;
    int operationsNo;
    cin >> operationsNo;
    cin.ignore();
    /*
    char operationCode;
    string bandName, albumTitle;
    for (int i = 0; i < operationsNo; ++i)
    {
        cin >> operationCode;
        cin.ignore();
        switch (operationCode)
        {
        case 'd':
            // Dodaj do mapy
            getline(cin, bandName);
            getline(cin, albumTitle);
            m.add(bandName, albumTitle);
            break;
        case 'w':
            // Wypisz tytuły albumów zespołu
            getline(cin, bandName);
            m.printAlbums(bandName);
            break;
        case 'z':
            // Wypisz zespoły z albumami o podanym tytule
            getline(cin, albumTitle);
            m.printBands(albumTitle);
            break;
        }
    }
    */
    string operationCode;
    string bandName, albumTitle;
    for (int i = 0; i < operationsNo; ++i)
    {
        getline(cin, operationCode);
        operationCode.erase(remove_if(operationCode.begin(), operationCode.end(), [](char c){ return c == ' '; }), operationCode.end());
        if (operationCode == "d")
        {
            getline(cin, bandName);
            getline(cin, albumTitle);
            m.add(bandName, albumTitle);
        }
        else if (operationCode == "w")
        {
            getline(cin, bandName);
            m.printAlbums(bandName);
        }
        else if (operationCode == "z")
        {
            getline(cin, albumTitle);
            m.printBands(albumTitle);
        }
    }
    return 0;
}
