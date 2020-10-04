#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class CompressedTrie
{
public:
    struct Node
    {
        string key;
        Node *parent;
        vector<Node*> children;
        vector<Node*> references;
        Node()
        {
            key = "";
            parent = nullptr;
        }
        Node(const string &str)
        {
            key = str;
            parent = nullptr;
        }
    };
private:
    Node *root1, *root2;
    unsigned keyCompare(const string &key, const string &child, unsigned pos) const;
    string printReference(Node *vertex, unsigned whichRoot) const;
    Node *search(const string &key, Node *root) const;
    Node *insert(const string &key, Node *vertex);
public:
    CompressedTrie();
    void add(const string &bandName, const string &albumTitle);
    void printAlbums(const string &bandName) const;
    void printBands(const string &albumTitle) const;
};

CompressedTrie::CompressedTrie()
{
    root1 = new Node;
    root2 = new Node;
}

unsigned CompressedTrie::keyCompare(const string &key, const string &child, unsigned pos) const
{
    unsigned i;
    for (i = 0; ((i + pos) < key.size()) && (i < child.size() && (key[pos + i] == child[i])); ++i);
    return i;
}

string CompressedTrie::printReference(Node *vertex, unsigned whichRoot) const
{
    // Konstruowanie nazwy pożądanego elementu przechodząc od liścia do roota.
    string result = "";
    // Drzewo zespołów
    if (whichRoot == 1)
    {
        while (vertex != root1)
        {
            result.insert(0, vertex->key);
            vertex = vertex->parent;
        }
    }
    else    // Drzewo albumów
    {
        while (vertex != root2)
        {
            result.insert(0, vertex->key);
            vertex = vertex->parent;
        }
    }
    return result;
}

CompressedTrie::Node *CompressedTrie::search(const string &key, Node *root) const
{
    Node *vertex = root;
    unsigned pos = 0;
    bool found = false;
    while (pos < key.size())
    {
        found = false;
        for (Node *n : vertex->children)
        {
            if (key.rfind(n->key, pos) == pos)  // Działa jak starts_with
            {
                // Jeśli cały klucz z n i końcówka key są ze sobą całkiem zgodne.
                if (pos + n->key.size() == key.size())
                {
                    // n jest poszukiwanym elementem.
                    return n;
                }
                else
                {
                    // Znaleziono kontynuację części klucza key w kluczu z n
                    // Przejdź do kolejnego wierzchołka.
                    pos += n->key.size();
                    vertex = n;
                    found = true;
                    break;
                }
            }
        }
        if (found)
        {
            // Przy znalezieniu częściowej lub kompletnej kontynuacji, kontynuuj.
            continue;
        }
        else
        {
            // Nie znaleziono odpowiadającego kluczowi wierzchołka.
            return nullptr;
        }
    }
}

CompressedTrie::Node *CompressedTrie::insert(const string &key, CompressedTrie::Node *vertex)
{
    unsigned sharedPartLength;
    unsigned pos = 0;
    bool addNew = true;
    while (pos < key.size())
    {
        addNew = true;
        for (vector<Node*>::iterator n = vertex->children.begin(); n < vertex->children.end(); ++n)
        {
            sharedPartLength = keyCompare(key, (*n)->key, pos);
            // Klucze nie mają części wspólnej. Szukaj w kolejnym dziecku.
            if (sharedPartLength == 0)
            {
                continue;
            }
			/*
			 * Pozostałość key i klucz z n całkowicie równe.
			 * Key istnieje w drzewie. Zwróć n jako element, do którego
			 * należy dodać referencję na element drugiego drzewa.
			 */
            else if (sharedPartLength == (*n)->key.size() && sharedPartLength == key.size() - pos)
            {
                return *n;
            }
            // Cały klucz z n zawiera się w key. Idź do kolejnego wierzchołka.
            else if (sharedPartLength == (*n)->key.size() && sharedPartLength < key.size() - pos)
            {
                pos += (*n)->key.size();
                vertex = *n;
                addNew = false;
                break;
            }
            // Częściowa zgodność kluczy. Dodanie nowego wierchołka i podział istniejącego.
            else if (sharedPartLength < (*n)->key.size() && sharedPartLength < key.size() - pos)
            {
                // Nowa część dzielonego wierzchołka (zawiera wspólną część klucza).
                Node *newNode = new Node((*n)->key.substr(0, sharedPartLength));
                newNode->children.push_back(*n);
                newNode->parent = vertex;
                (*n)->parent = newNode;
                (*n)->key = (*n)->key.substr(sharedPartLength, (*n)->key.size());
                vertex->children.erase(n);
                vertex->children.push_back(newNode);
                // Nowy wierzchołek zawierający pozostałość nowego klucza.
                Node *newNode2 = new Node(key.substr(pos + sharedPartLength, key.size()));
                newNode2->parent = newNode;
                newNode->children.push_back(newNode2);
                return newNode2;
            }
            // Wstawiana część klucza jest prefiksem klucza z n.
            else if (sharedPartLength < (*n)->key.size() && sharedPartLength == key.size() - pos)
            {
                // Nowy wierzchołek, ze wspólną częścią klucza.
                Node *newNode = new Node((*n)->key.substr(0, sharedPartLength));
                newNode->parent = vertex;
                newNode->children.push_back(*n);
                // Stary wierzchołek otrzymuje zmieniony klucz.
                (*n)->key = (*n)->key.substr(sharedPartLength, (*n)->key.size());
                (*n)->parent = newNode;
                vertex->children.erase(n);
                vertex->children.push_back(newNode);
                return newNode;
            }
        }
        /*
         * Żaden z wierzchołków wśród dzieci badanego wierzchołka nie zawierał
         * części klucza. Wstawienie nowego wierzchołka do listy jego dzieci.
         */
        if (addNew == true)
        {
            Node *newNode = new Node(key.substr(pos, key.size()));
            vertex->children.push_back(newNode);
            newNode->parent = vertex;
            return newNode;
        }
    }
}

void CompressedTrie::add(const string &bandName, const string &albumTitle)
{
    Node *addBandNode = insert(bandName, root1);
    Node *addAlbumNode = insert(albumTitle, root2);
    addBandNode->references.push_back(addAlbumNode);
    addAlbumNode->references.push_back(addBandNode);
}

void CompressedTrie::printAlbums(const string &bandName) const
{
    Node *node = search(bandName, root1);
    if (node != nullptr)
    {
        for (Node *n: node->references)
        {
            cout << printReference(n, 2) << "\n";
        }
    }
}

void CompressedTrie::printBands(const string &albumTitle) const
{
    Node *node = search(albumTitle, root2);
    if (node != nullptr)
    {
        for (Node *n: node->references)
        {
            cout << printReference(n, 1) << "\n";
        }
    }
}

int main()
{
    ios_base::sync_with_stdio(false);
    CompressedTrie m;
    int operationsNo;
    cin >> operationsNo;
    cin.ignore();
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
