#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <utility>
#include <vector>

struct Elem
{
    Elem* ptr;
    int value;
    Elem();
};

Elem::Elem()
{
    ptr = nullptr;
    int value = -1;
}

class DisjointSet
{
private:
    int size;
    Elem *elems;            // tablica wierzchołków
    std::list<Elem*> **arr; // tablica zbiorów (zbiór na każdy wierzchołek)
public:
    DisjointSet(int size);
    int find(int value) const;
    void union_(int val1, int val2);
};

DisjointSet::DisjointSet(int size)
{
    this->size = size;
    elems = new Elem[size];
    arr = new std::list<Elem*> *[size];
    for (int i = 0; i < size; ++i)
    {
        elems[i].value = i;
        arr[i] = new std::list<Elem*>;
        arr[i]->push_back(&elems[i]);
    }
}

int DisjointSet::find(int value) const
{
    // Zwróć wartość pierwszego elementu zbioru.
    if (elems[value].ptr != nullptr)
    {
        return elems[value].ptr->value;
    }
    return value;
}

void DisjointSet::union_(int val1, int val2)
{
    // Połącz zbiory zmieniając wskaźniki na początek nowego zbioru.
    int len1, len2;
    len1 = arr[val1]->size();
    len2 = arr[val2]->size();
    if (len1 >= len2)
    {
        for (auto &e: *arr[val2])
        {
            e->ptr = arr[val1]->front();
        }
        arr[val1]->splice(arr[val1]->end(), *arr[val2], arr[val2]->begin(), arr[val2]->end());
        delete arr[val2];
        arr[val2] = arr[val1];
    }
    else
    {
        for (auto &e: *arr[val1])
        {
            e->ptr = arr[val2]->front();
        }
        arr[val2]->splice(arr[val2]->end(), *arr[val1], arr[val1]->begin(), arr[val1]->end());
        delete arr[val1];
        arr[val1] = arr[val2];
    }
}

void containsCycle(std::vector<std::pair<int, int>> &edgeList, int len)
{
    int f1, f2;
    DisjointSet zb(len);
    std::pair<int, int> tmp;
    for (int i = 0; i < edgeList.size(); ++i)
    {
        tmp = edgeList[i];
        f1 = zb.find(tmp.first);
        f2 = zb.find(tmp.second);
        if (f1 == f2)
        {
            std::cout << "TAK\n";
            return;
        }
        else
        {
            zb.union_(f1, f2);
        }
    }
    std::cout << "NIE\n";
}

int main()
{
    int verticesNum, interestsNum, interest;
    std::cin >> verticesNum;
    std::map<int, std::vector<int>> m;
    for (int i = 0; i < verticesNum; ++i)
    {
        std::cin >> interestsNum;
        for (int j = 0; j < interestsNum; ++j)
        {
            std::cin >> interest;
            m[interest].push_back(i);
        }
    }

    int p1, p2;
    std::pair<int, int> tmp;
    std::vector<std::pair<int, int>> edgeList;  // wektor krawędzi grafu
    for (auto &vec: m)
    {
        if (vec.second.size() >= 2)
        {
            for (int i = 0; i < vec.second.size(); ++i)
            {
                for (auto j = i + 1; j < vec.second.size(); ++j)
                {
                    p1 = vec.second[i];
                    p2 = vec.second[j];
                    if (p1 > p2) std::swap(p1, p2);
                    tmp = std::make_pair(p1, p2);
                    if (find(edgeList.begin(), edgeList.end(), tmp) == edgeList.end())
                        edgeList.push_back(tmp);
                }
            }
        }
    }

    containsCycle(edgeList, verticesNum);

    return 0;
}
