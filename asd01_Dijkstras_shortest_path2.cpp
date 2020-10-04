#include <iostream>
#include <limits>

using namespace std;

struct Node
{
    int heapID, dist;
    bool visited;
};

class HeapQueue
{
private:
    pair<int, int> *heapArr;    // id wierzchołka w elems i heapControl; dystans
    int pos;                    // pozycja za-ostatniego elementu w kopcu
    Node *heapControl;          // tablica kontrolna z info o pozycji elementu w kopcu
    int len;                    // capacity obu tablic
public:
    HeapQueue(Node *heapControl, int len);
    void push(int id, int dist);
    void pop();
    int top() const;
    bool empty() const;
};

HeapQueue::HeapQueue(Node *heapControl, int len)
{
    this->heapControl = heapControl;
    this->len = len;
    heapArr = new pair<int, int> [len];
    pos = 0;
}

bool HeapQueue::empty() const
{
    if (pos == 0) return true;
    return false;
}

int HeapQueue::top() const
{
    return heapArr[0].first;
}

void HeapQueue::pop()
{
    heapControl[heapArr[0].first].heapID = -1;  // oznaczenie usuniecia w heapControl
    heapArr[0] = heapArr[pos - 1];              // usuwanie roota
    --pos;
    // przesiewanie nowego roota w dół
    bool max = false;
    pair<int, int> tmp;
    int v = 0;
    while (!max)
    {
        if (2*v+2 < pos)
        {
            if (heapArr[2*v+1].second <= heapArr[2*v+2].second)
            {
                if (heapArr[v].second > heapArr[2*v+1].second)
                {
                    heapControl[heapArr[v].first].heapID = 2*v+1;
                    heapControl[heapArr[2*v+1].first].heapID = v;
                    tmp = heapArr[2*v+1];
                    heapArr[2*v+1] = heapArr[v];
                    heapArr[v] = tmp;
                    v = 2*v+1;
                }
                else
                {
                    max = true;
                }
            }
            else if (heapArr[2*v+1].second > heapArr[2*v+2].second)
            {
                if (heapArr[v].second > heapArr[2*v+2].second)
                {
                    heapControl[heapArr[v].first].heapID = 2*v+2;
                    heapControl[heapArr[2*v+2].first].heapID = v;
                    tmp = heapArr[2*v+2];
                    heapArr[2*v+2] = heapArr[v];
                    heapArr[v] = tmp;
                    v = 2*v+2;
                }
                else
                {
                    max = true;
                }
            }
        }
        else if (2*v+1 < pos)  // ostatni element w lewej gałęzi
        {
            if (heapArr[v].second > heapArr[2*v+1].second)
            {
                heapControl[heapArr[v].first].heapID = 2*v+1;
                heapControl[heapArr[2*v+1].first].heapID = v;
                tmp = heapArr[2*v+1];
                heapArr[2*v+1] = heapArr[v];
                heapArr[v] = tmp;
                v = 2*v+1;
            }
            else
            {
                max = true;
            }
        }
        else
        {
            max = true;
        }
    }
}

void HeapQueue::push(int id, int dist)
{
    bool max = false;
    pair<int, int> tmp;
    int v;
    if (heapControl[id].heapID == -1) // dodaj do tablicy i przesiewaj w górę
    {
        heapArr[pos].first = id;
        heapArr[pos].second = dist;
        heapControl[id].heapID = pos;
        v = pos;
        ++pos;
    }
    else    // zaktualizuj pozycję (przesiewanie w górę) i dystans
    {
        v = heapControl[id].heapID;
        heapArr[v].second = dist;
    }
    // przesiewanie w górę
    while (!max && v != 0)
    {
        if (heapArr[v].second < heapArr[(v - 1) / 2].second)
        {
            heapControl[heapArr[v].first].heapID = (v - 1) / 2;
            heapControl[heapArr[(v - 1) / 2].first].heapID = v;
            tmp = heapArr[(v - 1) / 2];
            heapArr[(v - 1) / 2] = heapArr[v];
            heapArr[v] = tmp;
            v = (v - 1) / 2;
        }
        else
        {
            max = true;
        }
    }
}

int dijkstra(pair<int, int> *elems, Node *heapControl, int len, int start, int dest)
{
    HeapQueue q(heapControl, len);
    int curr, dist, r1, r2;
    q.push(start, 0);
    while (!q.empty())
    {
        curr = q.top();
        q.pop();
        heapControl[curr].heapID = -1;
        if (!heapControl[curr].visited)
        {
            for (int i = 0; i < len; ++i)
            {
                if (!heapControl[i].visited && i != curr)
                {
                    r1 = elems[curr].first - elems[i].first;
                    r2 = elems[curr].second - elems[i].second;
                    if (r1 < 0) r1 = -r1;
                    if (r2 < 0) r2 = -r2;
                    dist = r1 < r2 ? r1 : r2;
                    if (heapControl[i].dist > heapControl[curr].dist + dist)
                    {
                        heapControl[i].dist = heapControl[curr].dist + dist;
                        q.push(i, heapControl[i].dist);
                    }
                }
            }
            heapControl[curr].visited = true;
            if (curr == dest)
                return heapControl[curr].dist;
        }
    }
    return heapControl[dest].dist;
}

int main()
{
    ios_base::sync_with_stdio(false);
    int len;
    cin >> len;
    int start, destination;
    auto *heapControl = new Node [len];
    auto *elems = new pair<int, int> [len]; // współrzędne
    for (int i = 0; i < len; ++i)
    {
        cin >> elems[i].first >> elems[i].second;
        heapControl[i].heapID = -1;
        heapControl[i].visited = false;
        heapControl[i].dist = numeric_limits<int>::max();
    }

    cin >> start >> destination;
    heapControl[start].dist = 0;

    cout << dijkstra(elems, heapControl, len, start, destination) << "\n";

    return 0;
}
