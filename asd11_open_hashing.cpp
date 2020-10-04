#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

class HashTable
{
public:
	struct Node
	{
		string key;
		vector<Node*> references;
		Node()
		{
			key = "";
		}
		Node (const string &key)
		{
			this->key = key;
		}
	};
private:
	vector<Node*> *bandArr;
	vector<Node*> *albumArr;
	unsigned long size;
	unsigned long hashFun(const string &key, unsigned long size) const;
	Node *search(const string &key, vector<Node*> *array) const;
	Node *insert(const string &key, vector<Node*> *array);
public:
	HashTable(unsigned long size);
	void add(const string &bandName, const string &albumTitle);
	void printAlbums(const string &bandName) const;
	void printBands(const string &albumTitle) const;
};

unsigned long HashTable::hashFun(const string &key, unsigned long size) const
{
	unsigned long result = 0;
	for (int c: key)
	{
		result = (result << 3) + abs(c);
	}
	return result % size;
}

HashTable::Node *HashTable::search(const string &key, vector<Node*> *array) const
{
	unsigned long index = hashFun(key, size);
	Node *node = nullptr;
	for (vector<Node*>::iterator it = array[index].begin(); it != array[index].end(); ++it)
	{
		if ((*it)->key == key)
		{
			node = *it;
			break;
		}
	}
	return node;
}

HashTable::Node *HashTable::insert(const string &key, vector<Node*> *array)
{
	unsigned long index = hashFun(key, size);
	Node *node = nullptr;
	for (vector<Node*>::iterator it = array[index].begin(); it != array[index].end(); ++it)
	{
		if ((*it)->key == key)
		{
			node = *it;
			break;
		}
	}
	if (node == nullptr)
	{
		node = new Node(key);
		array[index].push_back(node);
	}
	return node;
}

HashTable::HashTable(unsigned long size)
{
	this->size = size * 1.5;
	bandArr = new vector<Node*>[this->size];
	albumArr = new vector<Node*>[this->size];
}

void HashTable::add(const string &bandName, const string &albumTitle)
{
	Node *band = insert(bandName, bandArr);
	Node *album = insert(albumTitle, albumArr);
	band->references.push_back(album);
	album->references.push_back(band);
}

void HashTable::printAlbums(const string &bandName) const
{
	Node *band = search(bandName, bandArr);
	if (band != nullptr)
	{
		for (auto album: band->references)
		{
			cout << album->key << "\n";
		}
	}
}

void HashTable::printBands(const string &albumTitle) const
{
	Node *album = search(albumTitle, albumArr);
	if (album != nullptr)
	{
		for (auto band: album->references)
		{
			cout << band->key << "\n";
		}
	}
}

int main()
{
	ios_base::sync_with_stdio(false);
	int operationsNo;
	cin >> operationsNo;
	cin.ignore();
	HashTable m(operationsNo);
	string operationCode;
	string bandName, albumTitle;
	for (int i = 0; i < operationsNo; ++i)
	{
		getline(cin, operationCode);
		operationCode.erase(remove_if(operationCode.begin(), operationCode.end(),
									  [](char c){ return c == ' '; }), operationCode.end());
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
