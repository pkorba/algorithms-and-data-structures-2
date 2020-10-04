#include <iostream>
#include <set>

using namespace std;

class Map
{
public:
    struct Node
    {
        Node *left, *right, *parent;
        int key;
        Node(int key)
        {
            this->key = key;
            left = right = parent = nullptr;
        }
        Node (int key, Node *parent)
        {
            this->key = key;
            this->parent = parent;
            left = right = nullptr;
        }
    };
private:
    Node *root;
    Node *maxNode(Node *root);
    Node *minNode(Node *root);
    std::set<int> s;
    bool findPairHelper(Node *nd, int sum);
public:
    Map();
    void insert(int key);
    void remove(int key);
    Node *search(int key);
    void findPair(int sum);
};

Map::Node *Map::maxNode(Node *vertex)
{
    while (vertex->right != nullptr)
    {
        vertex = vertex->right;
    }
    return vertex;
}

Map::Node *Map::minNode(Node *vertex)
{
    while (vertex->left != nullptr)
    {
        vertex = vertex->left;
    }
    return vertex;
}

bool Map::findPairHelper(Node *nd, int sum)
{
    if (nd == nullptr)
        return false;
    if (findPairHelper(nd->left, sum))
        return true;
    if (s.find(sum - nd->key) == s.end())
    {
        s.insert(nd->key);
        return findPairHelper(nd->right, sum);
    }
    else
    {
        int res1 = sum - nd->key;
        int res2 = nd->key;
        if (res1 > res2)
            cout << res2 << " " << res1 << "\n";
        else
            cout << res1 << " " << res2 << "\n";
        return true;
    }
}

Map::Map()
{
    root = nullptr;
}

Map::Node *Map::search(int key)
{
    Node *vertex = root;
    while (vertex != nullptr && vertex->key != key)
    {
        if (vertex->key < key)
            vertex = vertex->right;
        else
            vertex = vertex->left;
    }
    return vertex;
}

void Map::findPair(int sum)
{
    if (!findPairHelper(root, sum))
        cout << "NIE ZNALEZIONO\n";
}

void Map::insert(int key)
{
    Node *vertex;
    if (root == nullptr)
    {
        root = new Node(key);
    }
    else
    {
        vertex = root;
        // Wędruj aż do liścia
        while (!(vertex->key < key && vertex->right == nullptr) ||
               !(vertex->key >= key && vertex->left == nullptr))
        {
            if (vertex->key < key)
            {
                if (vertex->right != nullptr)
                    vertex = vertex->right;
                else
                    break;
            }
            else
            {
                if (vertex->left != nullptr)
                    vertex = vertex->left;
                else
                    break;
            }
        }
        // Po dotarciu do liścia, utwórz nowy liść jako jego dziecko.
        if (vertex->key < key)
        {
            vertex->right = new Node(key, vertex);
        }
        else
        {
            vertex->left = new Node(key, vertex);
        }
    }
}

void Map::remove(int key)
{
    Node *vertex = search(key);
    Node *tmp = nullptr;
    int tempKey;
    /*
     * Jeśli wierzchołek ma dwoje dzieci, zamień jego wartość z wartością
     * wierzchołka największego spośród swoich dzieci w lewym synu.
     */
    if (vertex->left != nullptr && vertex->right != nullptr)
    {
        tmp = maxNode(vertex->left);
        tempKey = vertex->key;
        vertex->key = tmp->key;
        tmp->key = tempKey;
        vertex = tmp;
    }
    // Dalej przypadki jednego dziecka lub braku dzieci.

    // Zmiana rodzica w dziecku.
    if (vertex->left != nullptr)
    {
        vertex->left->parent = vertex->parent;
        if (vertex == root)
            root = vertex->left;
    }
    if (vertex->right != nullptr)
    {
        vertex->right->parent = vertex->parent;
        if (vertex == root)
            root = vertex->right;
    }

    // Zmiana dziecka w rodzicu, jeśli rodzic istnieje.
    if (vertex->parent != nullptr && vertex->parent->left == vertex)
    {
        if (vertex->left != nullptr)
            vertex->parent->left = vertex->left;
        else
            vertex->parent->left = vertex->right;
    }
    if (vertex->parent != nullptr && vertex->parent->right == vertex)
    {
        if (vertex->left != nullptr)
            vertex->parent->right = vertex->left;
        else
            vertex->parent->right = vertex->right;
    }
    // Przypadek, gdy wierzchołek jest bezdzietnym rootem.
    if (vertex->left == nullptr && vertex->right == nullptr && vertex == root)
        root = nullptr;
    delete vertex;
}

int main()
{
    ios_base::sync_with_stdio(false);
    int clueNo, clueValue, minVal, maxVal, anticipatedSum;
    cin >> clueNo;
    Map m;
    int *clues = new int [clueNo];
    for (int i = 0; i < clueNo; ++i)
    {
        cin >> clueValue;
        clues[i] = clueValue;
        m.insert(clueValue);
    }
    cin >> minVal >> maxVal;
    cin >> anticipatedSum;
    for (int i = 0; i < clueNo; ++i)
    {
        if (clues[i] < minVal || clues[i] > maxVal)
        {
            m.remove(clues[i]);
        }
    }
    m.findPair(anticipatedSum);
    return 0;
}
