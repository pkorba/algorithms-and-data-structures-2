#include <iostream>
#include <string>

using namespace std;

// ------------------------------ Lista ------------------------------

class List
{
private:
    struct NodeIt
    {
        string value;
        NodeIt *next;
        NodeIt *prev;
        NodeIt()
        {
            this->value = "";
        }
        NodeIt(const string &value)
        {
            this->value = value;
        }
    };
    NodeIt *front;
    NodeIt *back;
public:
    class iterator
    {
    private:
        NodeIt *ptr;
    public:
        friend class List;
        iterator()
        {
            ptr = nullptr;
        }
        iterator(NodeIt *ptr)
        {
            this->ptr = ptr;
        }
        string &operator *()
        {
            return ptr->value;
        }
        string *operator ->()
        {
            return &(ptr->value);
        }
        iterator& operator ++()
        {
            ptr = ptr->next;
            return *this;
        }
        iterator operator ++(int)
        {
            iterator returnValue = *this;
            ptr = ptr->next;
            return returnValue;
        }
        iterator& operator --()
        {
            ptr = ptr->prev;
            return *this;
        }
        iterator operator --(int)
        {
            iterator returnValue = *this;
            ptr = ptr->prev;
            return returnValue;
        }
        bool operator ==(const iterator &other)
        {
            return ptr == other.ptr;
        }
        bool operator !=(const iterator &other)
        {
            return ptr != other.ptr;
        }
        void operator =(const iterator &other)
        {
            ptr = other.ptr;
        }
    };
    List();
    ~List();
    void push_back(const string &value);
    iterator insert(const iterator &it, const string &value);
    iterator erase(iterator &it);
    iterator begin();
    iterator end();
};

List::List()
{
    front = new NodeIt();
    back = new NodeIt();
    front->prev = nullptr;
    front->next = back;
    back->prev = front;
    back->next = nullptr;
}

List::~List()
{
    NodeIt * elem = front->next;
    NodeIt *toRemove;
    while(elem != back)
    {
        toRemove = elem;
        elem = elem->next;
        delete toRemove;
    }
    delete front;
    delete back;
}

void List::push_back(const string &value)
{
    insert(end(), value);
}


List::iterator List::insert(const iterator &it, const string &value)
{
    NodeIt *prev = it.ptr->prev;
    NodeIt *newElem = new NodeIt(value);
    newElem->next = it.ptr;
    newElem->prev = prev;
    prev->next = newElem;
    it.ptr->prev = newElem;
    return iterator(newElem);
}

List::iterator List::erase(iterator &it)
{
    if (it == back)
    {
        return back;
    }
    else
    {
        NodeIt *prev = it.ptr->prev;
        NodeIt *next = it.ptr->next;
        prev->next = next;
        next->prev = prev;
        iterator toRemove = it++;
        delete toRemove.ptr;
        return it;
    }
}

List::iterator List::begin()
{
    return iterator(front->next);
}

List::iterator List::end()
{
    return iterator(back);
}

// ------------------------------ Koniec listy ------------------------------

// ------------------------------ Mapa ------------------------------

class Map
{
public:
    struct Node
    {
        Node *left, *right, *parent;
        string key;
        char color;
        List::iterator value;
        Node(const string &name, List::iterator value)
        {
            this->key = name;
            color = 'r';
            this->value = value;
            left = right = parent = nullptr;
        }
        Node (const string &name, List::iterator value, Node *parent)
        {
            this->key = name;
            color = 'r';
            this->value = value;
            this->parent = parent;
            left = right = nullptr;
        }
    };
private:
    Node *root;
    List *l;
    void rightRotate(Node *vertex);
    void leftRotate(Node *vertex);
    void insertFix(Node *vertex);
public:
    Map(List *l);
    Node *search(const string &key);
    void insert(const string &key, List::iterator value);
    void move(const string &key, int distance);
    void rotate(const string &key1, const string &key2);
};

void Map::insertFix(Node *vertex)
{
    Node *uncle, *grandParent;
    while (true)
    {
        // Argument stopu
        if (vertex == root || vertex->parent->color != 'r')
        {
            root->color = 'b';
            break;
        }
        grandParent = vertex->parent->parent;
        // Ojciec lewym synem dziadka
        if (vertex->parent == grandParent->left)
        {
            uncle = grandParent->right;
            // Wujek czerwony - zamiana kolorów
            if (uncle != nullptr && uncle->color == 'r')
            {
                vertex->parent->color = 'b';
                uncle->color = 'b';
                grandParent->color = 'r';
                vertex = grandParent;
            }
            // Wujek czarny i syn prawym synem swojego ojca
            else if (vertex == vertex->parent->right)
            {
                leftRotate(vertex->parent);
                vertex = vertex->left;
                rightRotate(grandParent);
                vertex->parent->color = 'b';
                grandParent->color = 'r';
                vertex = vertex->parent;
            }
            // Wujek czarny i syn lewym synem swojego ojca
            else
            {
                rightRotate(grandParent);
                vertex->parent->color = 'b';
                grandParent->color = 'r';
                vertex = vertex->parent;
            }
        }
        // Lustrzane odbicie - ojciec prawym synem dziadka
        else if (vertex->parent == grandParent->right)
        {
            uncle = grandParent->left;
            // Wujek czerwony - zamiana kolorów
            if (uncle != nullptr && uncle->color == 'r')
            {
                vertex->parent->color = 'b';
                uncle->color = 'b';
                grandParent->color = 'r';
                vertex = grandParent;
            }
            // Wujek czarny i syn lewym synem swojego ojca
            else if (vertex == vertex->parent->left)
            {
                rightRotate(vertex->parent);
                vertex = vertex->right;
                leftRotate(grandParent);
                vertex->parent->color = 'b';
                grandParent->color = 'r';
                vertex = vertex->parent;
            }
            // Wujek czarny i syn prawym synem swojego ojca
            else
            {
                leftRotate(grandParent);
                vertex->parent->color = 'b';
                grandParent->color = 'r';
                vertex = vertex->parent;
            }
        }
    }
}

void Map::leftRotate(Node *vertex)
{
    Node *rchild = vertex->right;
    vertex->right = rchild->left;
    if (rchild->left != nullptr)
    {
        rchild->left->parent = vertex;
    }
    if (vertex->parent == nullptr)
    {
        root = rchild;
        rchild->parent = nullptr;
    }
    else if (vertex == vertex->parent->left)
    {
        vertex->parent->left = rchild;
        rchild->parent = vertex->parent;
    }
    else if (vertex == vertex->parent->right)
    {
        vertex->parent->right = rchild;
        rchild->parent = vertex->parent;
    }
    vertex->parent = rchild;
    rchild->left = vertex;
}

void Map::rightRotate(Node *vertex)
{
    Node *lchild = vertex->left;
    vertex->left = lchild->right;
    if (lchild->right != nullptr)
    {
        lchild->right->parent = vertex;
    }
    if (vertex->parent == nullptr)
    {
        root = lchild;
        lchild->parent = nullptr;
    }
    else if (vertex == vertex->parent->right)
    {
        vertex->parent->right = lchild;
        lchild->parent = vertex->parent;
    }
    else if (vertex == vertex->parent->left)
    {
        vertex->parent->left = lchild;
        lchild->parent = vertex->parent;
    }
    vertex->parent = lchild;
    lchild->right = vertex;
}

Map::Map(List*l)
{
    root = nullptr;
    this->l = l;
}

Map::Node *Map::search(const string &key)
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

void Map::insert(const string &key, List::iterator value)
{
    Node *vertex;
    if (root == nullptr)
    {
        root = new Node(key, value);
        root->color = 'b';
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
            vertex->right = new Node(key, value, vertex);
            vertex = vertex->right;
        }
        else
        {
            vertex->left = new Node(key, value, vertex);
            vertex = vertex->left;
        }
        insertFix(vertex);
    }
}

void Map::move(const string &key, int distance)
{
    bool negative;
    if (distance < 0)
    {
        negative = true;
        distance = -distance;
    }
    else
    {
        negative = false;
    }
    Node *vertex= search(key);
    List::iterator it = vertex->value;
    List::iterator start = l->erase(it);
    for (int i = 0; i < distance; ++i)
    {
        if (negative)
        {
            ++start;
        }
        else
        {
            --start;
        }
    }
    vertex->value = l->insert(start, key);
}

void Map::rotate(const string &key1, const string &key2)
{
    Node* v1 = search(key1);
    Node* v2 = search(key2);
    List::iterator tmp = v1->value;
    *v1->value = key2;
    *v2->value = key1;
    v1->value = v2->value;
    v2->value = tmp;
}

int main()
{
    int enemyNo;
    cin >> enemyNo;
    List l;
    Map m(&l);
    List::iterator it;
    string name1, name2;
    int i;
    // Wczytywanie elementów
    for (i = 0; i < enemyNo; ++i)
    {
        cin >> name1;
        l.push_back(name1);
        it = l.end();
        m.insert(name1, --it);
    }

    int operationNo, distance;
    char operationCode;
    cin >> operationNo;
    // Wczytywanie poleceń
    for (int j = 0; j < operationNo; ++j)
    {
        cin >> operationCode;
        if (operationCode == 'a')       // Dodaj na koniec listy
        {
            cin >> name1;
            l.push_back(name1);
            it = l.end();
            m.insert(name1, --it);
        }
        else if (operationCode == 'm')  // Przesuń o distance miejsc
        {
            cin >> name1 >> distance;
            m.move(name1, distance);
        }
        else if (operationCode == 'r')  // Zamień elementy miejscami w liście
        {
            cin >> name1 >> name2;
            m.rotate(name1, name2);
        }
    }

    // Wynik
    for (auto i: l)
    {
        cout << i << "\n";
    }

    return 0;
}
