#include"../../Graph/Graph.h"
#include <iostream>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <vector>
#include <climits>
#include "../../vector.xxx/vector.h"

using namespace std;

unordered_map<char, vector<char>> graph = {
    {'A', {'B', 'C', 'D'}},
    {'B', {'E', 'F'}},
    {'D', {'G', 'H'}},
    {'F', {'I', 'J'}}
};

vector<char> bfs(unordered_map<char, vector<char>>& graph, char start) {
    vector<char> visited;
    queue<char> q;

    q.push(start);

    while (!q.empty()) {
        char node = q.front();
        q.pop();

        if (find(visited.begin(), visited.end(), node) == visited.end()) {
            visited.push_back(node);

            for (char neighbor : graph[node]) {
                q.push(neighbor);
            }
        }
    }

    return visited;
}

vector<char> dfs(unordered_map<char, vector<char>>& graph, char start, vector<char> visited) {
    if (find(visited.begin(), visited.end(), start) == visited.end()) {
        visited.push_back(start);

        for (char neighbor : graph[start]) {
            visited = dfs(graph, neighbor, visited);
        }
    }

    return visited;
}

template <typename Tv, typename Te> struct PrimPU { //针对Prim算法的顶点优先级更新器
    virtual void operator()(Graph<Tv, Te>* g, Rank v, Rank u) {
        if (UNDISCOVERED == g->status(u)) //对于v每一尚未被发现的邻接顶点u
            if (g->priority(u) > g->weight(v, u)) { //按Prim策略做松弛
                g->priority(u) = g->weight(v, u); //更新优先级（数）
                g->parent(u) = v; //更新父节点
            }
    }
};

//最短路径
template <typename Tv, typename Te> struct DijkPU { //针对Dijkstra算法的顶点优先级更新器
    virtual void operator()(Graph<Tv, Te>* g, Rank v, Rank u) {
        if (UNDISCOVERED == g->status(u)) //对于v每一尚未被发现的邻接顶点u，按Dijkstra策略
            if (g->priority(u) > g->priority(v) + g->weight(v, u)) { //做松弛
                g->priority(u) = g->priority(v) + g->weight(v, u); //更新优先级（数）
                g->parent(u) = v; //并同时更新父节点
            }
    }
};

const int MAXN = 1010;
vector<pair<int, int>> G[MAXN];
int dis[MAXN];
bool vis[MAXN];

void dijkstra(int s) {
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    for (int i = 0; i < MAXN; i++) {
        dis[i] = INT_MAX;
        vis[i] = false;
    }

    dis[s] = 0;
    pq.push({ 0, s });

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        if (vis[u]) {
            continue;
        }
        vis[u] = true;

        for (auto& edge : G[u]) {
            int v = edge.first;
            int w = edge.second;

            if (dis[v] > dis[u] + w) {
                dis[v] = dis[u] + w;
                pq.push({ dis[v], v });
            }
        }
    }
}

const int MAX = 1010;
int fa[MAX];

int find(int x) {
    if (fa[x] == x) {
        return x;
    }
    fa[x] = find(fa[x]);

    return fa[x];
}

void merge(int x, int y) {
    int f1 = find(x);
    int f2 = find(y);

    fa[f1] = f2;
}

struct Edge {
    int u, v, w;
};

bool cmp(Edge a, Edge b) {
    return a.w < b.w;
}

vector<Edge> edges;
int n, m;


int PrimPU() {
    for (int i = 1; i <= n; i++) {
        fa[i] = i;
    }

    sort(edges.begin(), edges.end(), cmp);

    int res = 0;
    int cnt = 0;

    for (int i = 0; i < m; i++) {
        int u = edges[i].u;
        int v = edges[i].v;
        int w = edges[i].w;

        if (find(u) == find(v)) {
            continue;
        }

        merge(u, v);
        res += w;
        cnt++;

        if (cnt == n - 1) {
            break;
        }
    }

    return res;
}

int main() {
    vector<char> result1 = bfs(graph, 'A');

    for (char node : result1) {
        cout << node << " ";
    }

    cout << endl;
    vector<char> result2 = dfs(graph, 'A', {});

    for (char node : result2) {
        cout << node << " ";
    }
    cout << endl;

    int n, m, s, t;

    cin >> n >> m >> s >> t;

    for (int i = 0; i < m; i++) {
        int u, v, w;

        cin >> u >> v >> w;

        G[u].push_back({ v, w });
        G[v].push_back({ u, w });
    }
    dijkstra(s);
    cout << dis[t] << endl;

    cin >> n >> m;

    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        edges.push_back({ u, v, w });
    }
    cout << PrimPU() << endl;

}