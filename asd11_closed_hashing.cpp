#include <algorithm>
#include <iostream>
#include <string>
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
	Node **bandArr;
	Node **albumArr;
	unsigned size;
	unsigned hashFun(const string &key, unsigned size, unsigned attempt) const;
	Node *search(const string &key, Node **array) const;
	Node *insert(const string &key, Node **array);
public:
	HashTable(unsigned long size);
	void add(const string &bandName, const string &albumTitle);
	void printAlbums(const string &bandName) const;
	void printBands(const string &albumTitle) const;
};

unsigned HashTable::hashFun(const string &key, unsigned size, unsigned attempt) const
{
	unsigned result = 0;
	unsigned c;
	for (c = 0; c < 5 && c < key.size(); ++c)
	{
		result = (result << 5) + key[c];
	}
	return (result + (attempt * attempt + attempt) / 2) % size;
}

HashTable::Node *HashTable::search(const string &key, Node **array) const
{
	unsigned long attempt = 0;
	Node *node = nullptr;
	unsigned index = hashFun(key, size, attempt);
	while (array[index] != nullptr && array[index]->key != key)
	{
		++attempt;
		index = hashFun(key, size, attempt);
	}
	node = array[index];
	return node;
}

HashTable::Node *HashTable::insert(const string &key, Node **array)
{
	unsigned attempt = 0;
	Node *node = nullptr;
	unsigned index = hashFun(key, size, attempt);
	while (array[index] != nullptr && array[index]->key != key)
	{
		++attempt;
		index = hashFun(key, size, attempt);
	}
	if (array[index] == nullptr)
	{
		array[index] = new Node(key);
	}
	node = array[index];
	return node;
}

HashTable::HashTable(unsigned long size)
{
	this->size = size * 3;
	bandArr = new Node *[this->size];
	albumArr = new Node *[this->size];
	for (unsigned long i = 0; i < this->size; ++i)
	{
		bandArr[i] = nullptr;
		albumArr[i] = nullptr;
	}
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
