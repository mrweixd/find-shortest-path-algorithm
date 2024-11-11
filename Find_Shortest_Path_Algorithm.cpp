#include <bits/stdc++.h>
#define ll long long
using namespace std;

ll n, m;
vector<vector<pair<ll, ll>>> adj; // Adjacency list
vector<ll> d; // Shortest path weights
vector<ll> path; // Records shortest path
vector<ll> inDegree; // Records in-degrees for topological sort

bool detectCycle() {
    queue<ll> q;
    vector<ll> topo;
    for (ll i = 0; i < n; ++i) {
        if (inDegree[i] == 0) q.push(i);
    }
    while (!q.empty()) {
        ll now = q.front();
        q.pop();
        topo.push_back(now);
        for (auto &edge : adj[now]) {
            --inDegree[edge.first];
            if (inDegree[edge.first] == 0) q.push(edge.first);
        }
    }
    return topo.size() != n;
}

void bfs(ll start) {
    fill(d.begin(), d.end(), INT32_MAX);
    fill(path.begin(), path.end(), -1);
    d[start] = 0;
    queue<ll> q;
    q.push(start);
    while (!q.empty()) {
        ll u = q.front();
        q.pop();
        for (auto &edge : adj[u]) {
            ll v = edge.first;
            ll w = edge.second;
            if (d[v] > d[u] + w) {
                d[v] = d[u] + w;
                path[v] = u;
                q.push(v);
            }
        }
    }
}

void dijkstra(ll start) {
    vector<bool> visited(n, false);
    fill(d.begin(), d.end(), INT32_MAX);
    fill(path.begin(), path.end(), -1);
    d[start] = 0;
    priority_queue<pair<ll, ll>, vector<pair<ll, ll>>, greater<>> pq;
    pq.push({0, start});
    while (!pq.empty()) {
        ll u = pq.top().second;
        pq.pop();
        if (visited[u]) continue;
        visited[u] = true;
        for (auto &edge : adj[u]) {
            ll v = edge.first;
            ll w = edge.second;
            if (d[v] > d[u] + w) {
                d[v] = d[u] + w;
                path[v] = u;
                pq.push({d[v], v});
            }
        }
    }
}

bool bellmanFord(ll start) {
    fill(d.begin(), d.end(), INT32_MAX);
    fill(path.begin(), path.end(), -1);
    d[start] = 0;
    for (ll i = 0; i < n - 1; ++i) {
        for (ll u = 0; u < n; ++u) {
            for (auto &edge : adj[u]) {
                ll v = edge.first;
                ll w = edge.second;
                if (d[v] > d[u] + w) {
                    d[v] = d[u] + w;
                    path[v] = u;
                }
            }
        }
    }
    for (ll u = 0; u < n; ++u) {
        for (auto &edge : adj[u]) {
            ll v = edge.first;
            ll w = edge.second;
            if (d[v] > d[u] + w) {
                return false; // Negative cycle detected
            }
        }
    }
    return true;
}

void output(ll start) {

}

signed main() {
    ifstream inFile("input.txt");
    inFile >> n >> m;
    adj.resize(n);
    d.resize(n);
    path.resize(n);
    inDegree.resize(n, 0);
    bool hasNegativeWeight = false;
    for (ll i = 0; i < m; ++i) {
        ll u, v, w;
        inFile >> u >> v >> w;
        adj[u].emplace_back(v, w);
        ++inDegree[v];
        if (w < 0) hasNegativeWeight = true;
    }

    if (!hasNegativeWeight) {
        if (!detectCycle()) {
            cout << "Since there are no negative edges or cycles, using BFS\n";
            bfs(0);
        } else {
            cout << "Since we have no negative edges but cycles exist, using Dijkstra\n";
            dijkstra(0);
        }
    } else {
        if (!bellmanFord(0)) {
            cout << "No shortest paths can be found due to negative cycle.\n";
            return 0;
        }
        cout << "Since negative edges exist but no negative cycle, using Bellman Ford\n";
    }

    for (ll i = 0; i < n; ++i) {
        if (!i) continue;
        stack<ll> temp;
        ll current = i;
        temp.push(current);
        while (path[current] != -1) {
            current = path[current];
            temp.push(current);
        }
        cout << i << ": ";
        while (!temp.empty()) {
            current = temp.top();
            temp.pop();
            cout << current;
            if (!temp.empty()) cout << " -> ";
        }
        cout << " distances: " << d[i] << '\n';
    }
}
