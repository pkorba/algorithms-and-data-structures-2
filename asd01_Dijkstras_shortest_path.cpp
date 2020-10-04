#include <iostream>
#include <cmath>
#include <limits>
#include <queue>
#include <deque>
#include <vector>
#include <algorithm>

using namespace std;

struct Node
{
    bool visited;
    int x, y, dist;
};

struct Coordinates
{
    int x, y, id;
};

int dijkstra(vector<vector<int>> &adjacencyList, Node *control, int start, int dest)
{
    auto comp = [&control](int a, int b){ return control[a].dist > control[b].dist; };
	priority_queue<int, std::deque<int>, decltype (comp)> q(comp);
    int curr, absDistX, absDistY, dist;
    q.push(start);
    while (!q.empty())
    {
        curr = q.top();
        q.pop();
        if (!control[curr].visited)
        {
            for (auto i: adjacencyList[curr])
            {
                if (!control[i].visited)
                {
                    absDistX = abs(control[curr].x - control[i].x);
                    absDistY = abs(control[curr].y - control[i].y);
                    dist = absDistX < absDistY ? absDistX : absDistY;

                    if (control[i].dist > control[curr].dist + dist)
                    {
                        control[i].dist = control[curr].dist + dist;
                        q.push(i);
                    }
                }
            }
            control[curr].visited = true;
            if (curr == dest)
                return control[curr].dist;
        }
    }
    return control[dest].dist;
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
        cin >> elemsx[i].x >> elemsx[i].y;
        elemsx[i].id = i;
        control[i].x = elemsx[i].x;
        control[i].y = elemsx[i].y;
        control[i].dist = numeric_limits<int>::max();
        control[i].visited = false;
    }

    elemsy = elemsx;
    cin >> start >> destination;
    control[start].dist = 0;

    sort(elemsx.begin(), elemsx.end(), [](const Coordinates &a, const Coordinates &b){
        return a.x < b.x;
    });

    sort(elemsy.begin(), elemsy.end(), [](const Coordinates &a, const Coordinates &b){
        return a.y < b.y;
    });

    vector<vector<int>> adjacencyList(len);
    for (int i = 0; i < len; ++i)
    {
        if (i == 0)
        {
            adjacencyList[elemsx[i].id].push_back(elemsx[i + 1].id);
            adjacencyList[elemsy[i].id].push_back(elemsy[i + 1].id);
        }
        else if (i == len - 1)
        {
            adjacencyList[elemsx[i].id].push_back(elemsx[i - 1].id);
            adjacencyList[elemsy[i].id].push_back(elemsy[i - 1].id);
        }
        else
        {
            adjacencyList[elemsx[i].id].push_back(elemsx[i - 1].id);
            adjacencyList[elemsx[i].id].push_back(elemsx[i + 1].id);
            adjacencyList[elemsy[i].id].push_back(elemsy[i - 1].id);
            adjacencyList[elemsy[i].id].push_back(elemsy[i + 1].id);
        }
    }

    cout << dijkstra(adjacencyList, control, start, destination) << "\n";

    return 0;
}
