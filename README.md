This code can run the appropriate single-source shortest path algorithm for a given graph. It applies Dijkstra's algorithm and the Bellman-Ford algorithm, each suitable for a different type of graph.

### Tutorial

1. **Read from a file named `input.txt`**, which specifies the graph's details. (The input format is described below.)
2. **Determine the type of graph specified in the input file.** It could be:
     - A graph with negative weight cycles
     - A graph with negative weight edges but no negative weight cycles
     - A graph with no negative weight edges
     - A directed acyclic graph
3. **Apply the best single-source shortest path algorithm** (i.e., the one with the lowest time complexity) to identify all shortest paths from the source vertex 0. Note that if a negative weight cycle is reachable from vertex 0, the program should stop with a warning message like “No shortest paths can be found.”

**Input format**: The first line indicates the number of vertices and edges. Each subsequent line represents an edge, where the first two numbers indicate the two ends of the edge (direction is specified by the order), and the last number is the edge weight.

For example:


```
4	5
0	1	10
0	2	3
0	3	2
1	3	7
2	3	6
```

This indicates a graph with four vertices (0 to 3) and five edges, as shown below.

 <img src="https://github.com/user-attachments/assets/674dd333-097e-4cbe-9762-e29b259ba694" width=20% height=20%>

Below is an explanation of how the code handles different situations.

### Detecting Negative Edges
To detect negative edges during input, we check if `w < 0`, which quickly identifies any negative weights.

```cpp
bool hasNegativeWeight = false;
for (ll i = 0; i < m; ++i) {
    ll u, v, w;
    inFile >> u >> v >> w;
    adj[u].emplace_back(v, w);
    ++inDegree[v];
    if (w < 0) hasNegativeWeight = true;
}
```
### If there were no negative edges
In this case, we check for cycles using a topological sort.
```cpp
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
```
We cannot apply topological sort if there were cycles. Vice versa, if there were any cycles, the topological sort wouldn't work properly.We can determine this by checking if `topo.size() != n`.

### If there weren't any cycles
If no cycles are present, we can apply BFS to find the shortest path.
```cpp
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
```
### If there were any cycles
For graphs with cycles, BFS alone may lead to an infinite loop, so instead, we use Dijkstra method. The reason why Dijkstra works is because we check each vertex only once, and use it to relax other vertices. Since it won't go back, it will not go into an infinite loop.
```cpp
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
```

### If there were negative edges
  When negative edges are present, we apply the Bellman-Ford algorithm. Bellman-Ford relaxes edges up to `n - 1` times, making all edges shortest. After this, if any edge weight can still be reduced, a negative cycle exists, meaning no shortest path can be found.
```cpp
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
```
You can use the input in the folder to give this code a try!
