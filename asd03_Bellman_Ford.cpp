#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <utility>
#include <vector>

using namespace std;

struct Node
{
    int v, dist;
};

struct Coordinates
{
    int x, y, v, id;
};

enum Side {left, right};

inline int calculateDistance(const vector<Coordinates> &vec, Side side, int index)
{
    int sideIndex, absDistX, absDistY;
    if (side == Side::right)
        sideIndex = index + 1;
    else
        sideIndex = index - 1;

    absDistX = abs(vec[index].x - vec[sideIndex].x);
    absDistY = abs(vec[index].y - vec[sideIndex].y);
    return absDistX < absDistY ? absDistX - vec[sideIndex].v : absDistY - vec[sideIndex].v;
}

void bellmanFord(vector<vector<pair<int, int>>> &adjacencyList, Node *control, int dest, int len)
{
    for (int i = 0; i < len - 1; ++i)
    {
        for (int j = 0; j < len; ++j)
        {
            for (auto vertex: adjacencyList[j])
            {
                if (control[j].dist != numeric_limits<int>::max() &&
                        control[vertex.first].dist > control[j].dist + vertex.second)
                {
                    control[vertex.first].dist = control[j].dist + vertex.second;
                }
            }
        }
    }
    // Wykrywanie cyklu ujemnego
    for (int j = 0; j < len; ++j)
    {
        for (auto vertex: adjacencyList[j])
        {
            if (control[j].dist != numeric_limits<int>::max() &&
                    control[vertex.first].dist > control[j].dist + vertex.second)
            {
                cout << "NIE ZA DOBRZE?\n";
                return;
            }
        }
    }
    cout << control[dest].dist << "\n";
}

int main()
{
    ios_base::sync_with_stdio(false);
    int len, start, destination;
    cin >> len;
    vector<Coordinates> elemsx(len);
    vector<Coordinates> elemsy(len);
    auto *control = new Node [len];
    for (int i = 0; i < len; ++i)
    {
        cin >> elemsx[i].x >> elemsx[i].y >> elemsx[i].v;
        elemsx[i].id = i;
        control[i].v = elemsx[i].v;
        control[i].dist = numeric_limits<int>::max();
    }

    cin >> start >> destination;
    control[start].dist = 0;

    elemsy = elemsx;

    sort(elemsx.begin(), elemsx.end(), [](const Coordinates &a, const Coordinates &b){
        return a.x < b.x;
    });

    sort(elemsy.begin(), elemsy.end(), [](const Coordinates &a, const Coordinates &b){
        return a.y < b.y;
    });

    vector<vector<pair<int, int>>> adjacencyList(len);
    int dist;
    for (int i = 0; i < len; ++i)
    {
        if (i == 0)
        {
            dist = calculateDistance(elemsx, Side::right, i);
            adjacencyList[elemsx[i].id].push_back({elemsx[i + 1].id, dist});

            dist = calculateDistance(elemsy, Side::right, i);
            adjacencyList[elemsy[i].id].push_back({elemsy[i + 1].id, dist});
        }
        else if (i == len - 1)
        {
            dist = calculateDistance(elemsx, Side::left, i);
            adjacencyList[elemsx[i].id].push_back({elemsx[i - 1].id, dist});

            dist = calculateDistance(elemsy, Side::left, i);
            adjacencyList[elemsy[i].id].push_back({elemsy[i - 1].id, dist});
        }
        else
        {
            dist = calculateDistance(elemsx, Side::left, i);
            adjacencyList[elemsx[i].id].push_back({elemsx[i - 1].id, dist});

            dist = calculateDistance(elemsx, Side::right, i);
            adjacencyList[elemsx[i].id].push_back({elemsx[i + 1].id, dist});

            dist = calculateDistance(elemsy, Side::left, i);
            adjacencyList[elemsy[i].id].push_back({elemsy[i - 1].id, dist});

            dist = calculateDistance(elemsy, Side::right, i);
            adjacencyList[elemsy[i].id].push_back({elemsy[i + 1].id, dist});
        }
    }

    bellmanFord(adjacencyList, control, destination, len);

    return 0;
}
