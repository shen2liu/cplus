// 
// Graph Definition:
//   A graph G is defined as a collection of a set V, of vectics (or nodes),
//   and a set E, of edges, then for a finite n, G = {V, E}, where
//   V = {V1, V2,...,Vn};  E = { Eij = (Vi, Vj), 1 <= i, j <= n }
//
// Graph Types:
//   + undirected graph: in which the edges have no direction.
//     directed graph: in which the edges have direction.
//   + connected graph: there is a path (a set of vertices) between any two vertices.
//     disconnected graph: at least one node cannot reach to some of other vertices.
//   + acyclic graph: contains no cycle.
//     cycle graph: contains at least one cycle.
//   - complete graph: there is a connection between any two nodes.
//   - multi-graph: at least two vertices have more than one connections between them.
//   - weighted graph: numeric values (weights, costs) associate with each edge.
// 
// Vertex Types:
//   - leaf vertex, also called pendant vertex, only has one neighbor.
//   - universal vertex: adjacent to every other vertex in the graph.
//   - cut vertex: the removal of this vertex would disconnect the graph.
//
// Edge Types:
//   - undirected edge: a connection between a apir of nodes (A, B).
//   - directed edge: from initial vertex A to terminal vertex B, <A, B>.
//   - self-edge: have a connection to itself.
//   - hyperedge: more than one vertex is connected by the same one edge.
//
// Graph Representations:
//   - adjacency matrics
//   - adjacency lists
//   - adjacency multilists     // not in use
//
// Graph Traversals:
//   - Depth First Search
//   - Breadth First Search
//
// Shortest Path
//   + single pair: from one node to another
//     - Shortest Path BFS
//   + single source: from one node to all other nodes
//     - Dijkstra's Algorithm
//     - Bellman Ford Algorithm
//   - signle destination: from all nodes to one node
//   - all pairs: between every pairs
//
// Topological Sort (Ordering)
//   - it is a linear ordering of vertices such that for every edge u->v
//     in a directed graph, the vertex u always comes before vertex v.
//   - Topo Sort only works for a directed acyclic graph (DAG),
//   - DAG has at least one vertex with indegree  as 0.
//     DAG has at least one vertex with outdegree as 0. 
//
// Minimum Spanning Tree (MST)
//   a spinning tree of a graph is a subgraph that has all the vertices (n)
//   and some (n - 1) edges sufficient to build a tree.
//   + the minimum spanning tree has the minimum weight than all other
//     spanning trees of the same graph.
//   + a connected graph always has one or more spinning trees. 
//     a graph havin a spinning tree is connected.
//   + algorithms to find the minimum spinning tree of a graph:
//     - Prim Algorithm
//     - Kruskal Algorithm
//
// Detect Cycle Methods
//     - DFS
//     - BFS
//     - Disjoint Set
//     - Topological Sort
//     - Bellman Ford (Negative Cycle)
// 
// Graph Implementation
//   - the vertex is unique, indexed during the creation of the graph.
//   - the adjacent list is in the order of the source vertex indexes.
//

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <set>
#include <list>
#include <algorithm>
#include <climits>
#include <cmath>

using namespace std;

/* using an integer to present a vertex
 */
typedef  unsigned int   Vertex;
typedef  int            Weight;

/* a structure to represent a weighted edge in graph
 * + does not define the direction in this structure
 */
struct Edge { 
    Vertex  src;        // the source vertex of an edge in directed graph
    Vertex  dst;        // the destination vertex of an edge in directed graph
    Weight  weight;     // the cost from the source vertex to the destination vertext

    Edge(Vertex s, Vertex d) : src(s), dst(d), weight(1) { }
    Edge(Vertex s, Vertex d, Weight w) : src(s), dst(d), weight(w) { }

    bool equal_directed(Edge e) { return this->src == e.src && this->dst == e.dst; } 
    bool equal_undirected(Edge e) { return (this->src == e.src && this->dst == e.dst) ||
                                           (this->src == e.dst && this->dst == e.src); }
    
    bool operator == (const Edge& e) const { return src == e.src && dst == e.dst; }
    bool operator != (const Edge& e) const { return src != e.src || dst != e.dst; }

    void print_edge() { 
        cout << "{" << char(src) << ", " << char(dst) << ", " << weight <<"}"; }
};
/* a structure to represent a connected, directed and weighted graph.
 */
class Graph {
    bool  directed;             // directed graph, false by default
    bool  weighted;             // edges have weights (costs)
    vector<Vertex>  vertices;   // sorted dynamic list (vector)

public:
    vector<list<Edge>> edge_lists;    // adjacent_list, unsorted, represents the graph

    Graph() : directed(false), weighted(false)  { edge_lists.clear(); }
    Graph(bool dt, bool wt) : directed(dt), weighted(wt)  { edge_lists.clear(); }

    bool  is_directed() { return directed; }
    bool  is_weighted() { return weighted; }
    int   number_vertices() { return vertices.size(); }
    int   number_edges();

    Vertex get_vertex(int index);
    int    get_index(Vertex v);
    int    add_vertex(Vertex v); 
    void   delete_vertex(int index);
    void   delete_vertex(Vertex v) { delete_vertex(get_index(v)); }
    bool   find_vertex(Vertex v) { return get_index(v) >= 0; }
    void   print_vertices();

    void  add_edge(Vertex s, Vertex d, Weight w);
    void  add_edge(Vertex s, Vertex d) { add_edge(s, d, 1); }
    void  add_edge(Edge e)  { add_edge(e.src, e.dst, e.weight); };
    void  delete_edge(Vertex s, Vertex d);
    void  delete_edge(Edge e) { delete_edge(e.src, e.dst); }
    bool  equal_edge(Edge e1, Edge e2);

    void  print_graph();

    int   algorithm_shortest_path(Vertex s, Vertex d, vector<Vertex>& path);
    void  algorithm_bellman_ford(Vertex start, int *distances);
    void  algorithm_mst_prim(Vertex v0, vector<Edge>& mst);
    void  topological_sort(vector<Vertex>& topo_sort);
    bool  detect_cycle_dfs(int vi, int pi, vector<int>& visited);
};

#define COST_MAX   (UINT_MAX >> 1)  // avoid overflow
#define NOT_VERTEX  '*'             // invalid vertex

/**********************************************************************
 * get_index()
 * return the index (position) of the vertex
 * improve: 
 *   define a vertex structure to include its index, maybe label too
 */
int Graph::get_index(Vertex v)
{
    int nv = number_vertices();
    for (int i = 0; i < nv; ++i) {
        if (vertices[i] == v) {
            return i;
        }
    }
    return -1;
}
/**********************************************************************
 * get_vertex()
 * return the vertex value by the index
 */
Vertex Graph::get_vertex(int index)
{
    if (index >= 0 && index < vertices.size()) {
        return vertices[index];
    }
    return UINT_MAX;
}
/**********************************************************************
 * add_vertex()
 * append the vertex to the vertices list (vector).
 * return the index of the vertex.
 */
int Graph::add_vertex(Vertex v)
{
    vertices.push_back(v);
    return vertices.size() - 1;
}
/**********************************************************************
 * delete_vertex()
 * delete the vertex, but not the edges extending to/from it
 */
void Graph::delete_vertex(int index)
{
    if (index >= 0 && index < vertices.size()) {
        vertices.erase(vertices.begin() + index);
    }
}
/**********************************************************************
 * print_vertices()
 */
void Graph::print_vertices()
{
    cout << "Vertices: ";
    int  nv = vertices.size();
    if (nv) {
        const int ww = log10(nv) + 1;
        for(int i = 0; i < nv; ++i) { 
            cout.width(ww); cout << char(vertices[i]) << "(" << i << "), ";  
        } 
        cout << endl;
    }
    else {
        cout << "{ }" << endl;
    }
}
/**********************************************************************
 * number_edges()
 * return the total number of edges in the graph
 */
int Graph::number_edges()
{
    int num = 0;
    for (auto elist : edge_lists) {
        num += elist.size();
    }
    return is_directed() ? num : num >> 1;
}
/**********************************************************************
 * equal_edge()
 * check if the two edges are the same which have the same vertices.
 * return true or false
 */
bool Graph::equal_edge(Edge e1, Edge e2)
{
    return is_directed() ? e1.equal_directed(e2) : e1.equal_undirected(e2);
}
/**********************************************************************
 * add_edge()
 * - every edge appends to the adjacent list of the source vertex.
 * - an empty list is added for destination vertex if not found.
 * - also add the edge to the adjacent list of the destination vertex 
 *   for undirected graph.
 */
void Graph::add_edge(Vertex s, Vertex d, Weight w)
{
    if ( find_vertex(s) ) {
        int src_index = get_index(s); 
        edge_lists[src_index].emplace_back(s, d, w);
    }
    else {
        int src_index = add_vertex(s);
        list<Edge> elist;
        elist.emplace_back(s,d, w);
        edge_lists.insert(edge_lists.begin() + src_index, elist);
    }
    
    if ( !find_vertex(d) ) {
        int dst_index = add_vertex(d);
        list<Edge> elist;
        if ( !is_directed() ) {
            elist.emplace_back(d, s, w);
        }
        edge_lists.insert(edge_lists.begin() + dst_index, elist);
    }
    else if ( !is_directed() ) {
        int dst_index = get_index(d); 
        edge_lists[dst_index].emplace_back(d, s, w);
    }
}
/**********************************************************************
 * delete_edge()
 * remove the edge(s) that source vertex is s, destination vertex is d.
 * only delete the edges, does not remove the vertices of the edge.
 */
void Graph::delete_edge(Vertex s, Vertex d)
{
    int si = get_index(s);
    for (Edge e : edge_lists[si]) {
        if (e.dst == d) {
            edge_lists[si].remove(e);
            break;
        }
    }
}
/**********************************************************************
 * print_graph()
 */
void Graph::print_graph()
{
    int nv = edge_lists.size();
    int ne = number_edges();
    cout << "  Vertex List: " << nv << " "; print_vertices();
    cout << "  Edge (Adjacent) Lists: " << ne << " Edges, Edge {src, dst, weight}" << endl;
    for (int i = 0; i < nv; ++i) {
        cout.width(5); cout << i << ": ";
        for (Edge e: edge_lists[i]) {
            // e.print_edge(); if ( !e.equal_directed(edge_lists[i].back()) ) { cout << ", "; }
            e.print_edge(); if (e != edge_lists[i].back()) { cout << ", "; }
        }
        cout << endl;
    }
}
/**********************************************************************
 * algorithm_shortest_path
 * + implemented using BFS
 * + the algorithm can be enhanced to check cycle
 * + time complexity: best O(E), worst O(VE)
 *   V = number of vertices; E = number of edges
 *   spce complexity: 
 */
int Graph::algorithm_shortest_path(Vertex s, Vertex d, vector<Vertex>& path)
{
    int  nv = number_vertices();

    vector<int>  distances(nv, COST_MAX);
    int si = get_index(s);
    distances[si] = 0;

    vector<Vertex> previous(nv, NOT_VERTEX);
    
    queue<int> Q;
    Q.push(si);

    vector<bool> visited(nv, false);
    visited[si] = true;

    // BFS
    while (!Q.empty()) {
        int vi = Q.front();
         Q.pop();

        for (Edge e : edge_lists[vi]) {
            int di = get_index(e.dst);
            if (distances[vi] + e.weight < distances[di]) {
                distances[di] = distances[vi] + e.weight;
                previous[di] = e.src;
                if (!visited[di]) {
                    Q.push(di);
                    visited[di] = true;
                }
            }
            if (e.dst == d) {
                break;
            }
        }
    }

    // obtain the shortest path (vertices)
    int di = get_index(d);
    int vi = di;
    if (distances[vi] < COST_MAX) {
        Vertex v = get_vertex(vi);
        while (v != NOT_VERTEX) {
            path.push_back(v);
            if (v == s) {
                break;
            }
            v = previous[vi];
            if (v != NOT_VERTEX) {
                vi = get_index(v);
            }
        }
        reverse(path.begin(), path.end());
    }
    return  distances[di];
}
/**********************************************************************
 * Algorithm Bellman Ford: 
 * + sortest path for single source, dynamic programming
 * + relax on every edge for every vertex.
 * - works for the negtive weights, can be used to check the cycles.
 * - different: go to the next vertex, not the neighbor vertex
 * - cannot break out when there is nothing changed for every vertex.
 * 
 * + time complexity:  best O(E), average O(VE), worst O(VE)
 *   space complexity: O(V)
 *   V = number of vertices; E = number of edges
 * 
 * input:
 *   (Vertex)start - the start (source) vertex
 * return:
 *   (int*)distances - the shortest distances to other vertices.
 *                     the caller must allocate the memory of ditances[]
 */
void Graph::algorithm_bellman_ford(Vertex start, int *distances)
{
    int nv = number_vertices();  
    for (int i = 0; i < nv; i++) {
        distances[i] = COST_MAX;
    }

    int s = get_index(start);
    distances[s] = 0;
    
    // relax all vertices
    for (int i = 0; i < nv; ++i) {
        int vi = (i + s) % nv;
        for (Edge e : edge_lists[vi]) {
            int si = get_index(e.src);      // vi == si
            int di = get_index(e.dst);
            if (distances[si] != COST_MAX && distances[si] + e.weight < distances[di]) {
                distances[di] = distances[si] + e.weight;
            }
        }
    }  
}
/**********************************************************************
 * Kahn's Topological Sorting
 *   it is impossible if the graph is not a directed acyclic graph (DAG)
 * algorithm:
 *   + compute indegree for every vertex
 *   + put all the vertices (indexes) with indegree as 0 in a queue
 *   + pop a vertex from the queue until queue is empty
 *     - decrease the indegree by 1 for the neighbor vertices
 *     - put the vertex into the queue if its indegree become 0
 *   + repeat the last step until the queue is empty
 *   *
 */
void Graph::topological_sort(vector<Vertex>& topo_sort)
{
    int nv = number_vertices();

    // calculate the indegrees for every vertex
    vector<int> indegrees(nv, 0);
    for (int i = 0; i < nv; i++) { 
        for (Edge e : edge_lists[i]) { 
            int di = get_index(e.dst);
            indegrees[di]++;
        }
    }
    
    // find vertices tht have no incoming edges
    queue<int> no_incoming;
    for (int i = 0; i < nv; i++) {
        if (indegrees[i] == 0) {
            no_incoming.push(i);
        }
    }

    while ( !no_incoming.empty() ) {
        int i = no_incoming.front();
        no_incoming.pop();
        Vertex v = get_vertex(i);
        topo_sort.push_back(v);
        for (auto e : edge_lists[i]) {
            int di = get_index(e.dst);
            indegrees[di]--;
            if (indegrees[di] == 0) {
                no_incoming.push(di);
            }
        }
    }
}
/**********************************************************************
 * Prim's Minimum Spanning Tree (Greedy Algorithm)
 * Algorithm:
 *   1. remove the self-loops and parallel edges
 *   2. choose a node as the root node
 *   3. select the outgoing edge with less weight
 *   4. 
 */
void Graph::algorithm_mst_prim(Vertex v0, vector<Edge>& mst)
{
    int nv = number_vertices();
    bool selected[nv] = { false };

/*
    // find the vertex having the minimum weight, should be done by min-heap
    int min_vertex_si, min_vertex_di;
    int min_weight = INT_MAX;
    for (int vi = 0; vi < nv; ++vi) {
        for (Edge e : edge_lists[vi]) {
            if (e.weight < min_weight) {
                min_weight = e.weight;
                min_vertex_si = vi;
                min_vertex_di = get_index(e.dst);
            }
        }
    }
    Vertex v0 = get_vertex(min_vertex_si);
    mst.push_back(v0);
    visited[min_vertex_si] = true;
    cout << "Min Vertex: " << char(v0) << " (" << min_vertex_si << ", " << min_vertex_di << ")" << endl;
*/
    int vi = get_index(v0);
    selected[vi] = true;
    vector<int> vertices; 
    vertices.push_back(vi);
    
    for (int edge_count = 0; edge_count < nv - 1; ++edge_count) {
        Edge min_edge(v0, NOT_VERTEX, INT_MAX);
        for (int vi : vertices) {
            for(Edge e : edge_lists[vi]) {
                int di = get_index(e.dst);
                if ( !selected[di] && e.weight < min_edge.weight) {
                    min_edge = e;
                }
            }
        }
        mst.push_back(min_edge);
        vi = get_index(min_edge.dst);
        vertices.push_back(vi);
        selected[vi] = true;
    }   
}
/**********************************************************************
 * Detect Cycle using DFS (or DFT)
 * algorithm:
 *   - go through all the vertices using DFS, we have a cycle when
 *     we see a vertex that has been visited before.
 * implementation:
 *   (int)vi - the index of the vertex being worked on
 *   (int)pi - the index of the vertex (parent) where we came from
 *   (int[])visited - the collection of the vertices that have ben visited.
 */
bool Graph::detect_cycle_dfs(int vi, int pi, vector<int>& visited)
{
    visited[vi] = true;
    cout << vi << "(" << char(get_vertex(vi)) << "), ";
    for (Edge e : edge_lists[vi]) {
        int di = get_index(e.dst);
        if (!visited[di]) {
            return detect_cycle_dfs(di, vi, visited);
        }
        else if (di != pi) {
            cout << di << "(" << char(get_vertex(di)) << ") = cycle ";
            return true;
        }
    }
    return false;
}

/**********************************************************************
 * the following code uses the adjacent tables, like ...
 */
vector<vector<int>> adjacent_table
{
//   0  1  2  3  4  5  6        //
    {0, 0, 1, 1, 0, 0, 0},      //   0---2---1
    {0, 0, 1, 0, 0, 1, 0},      //   |  /|   |
    {1, 1, 0, 1, 1, 0, 0},      //   | / |   |
    {1, 0, 1, 0, 0, 0, 1},      //   3   4---5
    {0, 0, 1, 0, 0, 1, 0},      //    \     /
    {0, 1, 0, 0, 1, 0, 1},      //     \   /
    {0, 0, 0, 1, 0, 1, 0}       //       6
};
/**********************************************************************
 * Depth First Traversal (Search) - DFT/DFS
 * traverses the nodes in a depthward motion, then move backward to 
 * get the next node for traversal. it visits every node exactly once.
 * 
 * the DFS recursive implementation uses the idea of backtracking.
 * it is smilar to the preorder traversal of a binary tree.
 * it exhausts all the nodes by going ahead in the current path,
 * then move backward when no more unvisited nodes on the path.
 *
 * Backtracking means that when you are moving forward and there are 
 * no more nodes along the current path, you move backwards on the same 
 * path to find nodes to traverse.
 * 
 * the DFT assume every edge has the same cost (weight).
 * the DFT may not work when there is an undirected cycle.
 */
void graph_DFT_recursive(int v, vector<bool>& vertex_visited)
{
    if (vertex_visited[v]) {
        return;
    }
    vertex_visited[v] = true;
    cout << v << " ";
    for (int i = adjacent_table[v].size() - 1; i >= 0; --i) {
        if (adjacent_table[v][i]) {
            graph_DFT_recursive(i, vertex_visited);
        }
    }
}
/* the DFT iterative implemention uses a stack to backtrack the nodes.
 * + it goes forward (visit every node) along one path, 
 * + save (push into a stack) all the adjacent nodes of the nodes on 
 *   the path for backtracking.
 */
void graph_DFT_iterative(int v)
{
    vector<bool> vertices_visited(adjacent_table.size(), false);

    vertices_visited[v] = false; 
    stack<int> S;
    S.push(v);

    while (!S.empty()) 
    { 
        v = S.top(); 
        S.pop(); 
  
        if (!vertices_visited[v])
        { 
            cout << v << " "; 
            vertices_visited[v] = true; 
        } 
  
        for (int i = 0; i < adjacent_table[v].size(); ++i) {
            if (adjacent_table[v][i] && !vertices_visited[i]) {
                S.push(i);
            }
        }
    }
}
/**********************************************************************
 * Breadth First Traversal (Search)
 * explores the vertices in order of the distance from the source vertex,
 * where the distance is the minimum length (number of vertices) of the path
 * from the source vertex.
 * 
 * BFT works best when there is a concept of layers or levels in the graph.
 * BFT assume every edge has the same cost (weight).
 */
void graph_BFT_iterative(int v)
{
    vector<bool> vertices_visited(adjacent_table.size(), false);
  
    queue<int> Q;

    vertices_visited[v] = true; 
    Q.push(v);
  
    while (!Q.empty())
    { 
        v = Q.front(); 
        cout << v << " "; 
        Q.pop(); 
  
        for (int i = 0; i < adjacent_table[v].size(); ++i) 
        { 
            if ( adjacent_table[v][i] && !vertices_visited[i])
            { 
                vertices_visited[i] = true; 
                Q.push(i); 
            } 
        }
    } 
}
/**********************************************************************
 * Dijkstra Algorithm (single source shortest Path)
 *   find the sortest path (respect to the cost) from the start node 
 *   to every node in the graph.
 *   + greedy programming
 * Algorithm: 
 *   cost[][] is the same as G[][] except changing the 0 to COST_MAX.
 *   distance[] will include the results of all the nodes.
 *     initialize to COST_MAX except the starting node [0].
 *   pred[] remembers the previous node that leads to the shortest distance.
 *   the outside while loop make sure every node is visited.
 *   the first for loop finds the shortest path in unvisited nodes
 *   the second for loop does the relaxation, 
 *   Relaxation: the process that calculates the distances of the vertex from
 *               its neighbors, keep the smallest one for the vertex.
 */
void graph_dijkstra_algorithm(vector<vector<unsigned int>>& g, 
                              unsigned int start,
                              vector<unsigned int>& distance)
{
    int sz = g.size();
    int i, j, next;
    unsigned int cost[sz][sz];
    bool visited[sz] = {false};

    for (i = 0; i < sz; i++) {
        for (j = 0; j < sz; j++) {
            cost[i][j] = g[i][j] == 0 ? COST_MAX : g[i][j];
        }
    }

    for (i = 0; i < sz; i++) {
        distance[i] = cost[start][i];
    }

    distance[start] = 0;
    visited[start] = true;
    int count = 1;

    while (count < sz - 1) {
        unsigned long minDistance = COST_MAX;
        for (i = 0; i < sz; ++i) {
            if (distance[i] < minDistance && !visited[i]) {
                minDistance = distance[i];
                next = i;
            }
        }
        visited[next] = true;
        for (i = 0; i < sz; ++i) {
            if (!visited[i]) {
                if (minDistance + cost[next][i] < distance[i]) {
                    distance[i] = minDistance + cost[next][i];
                }
            }
        }
        ++count;
    }
    // cout << "visited: ";  for (auto v : visited) { cout << v << ", "; }  cout << endl;
}
/**********************************************************************
 * Kruskal's Spanning Tree Algorithm
 * algorithm:
 *   1. remove self circle and parallel edges.
 *   2. build and sort the edges according to their weights.
 *   3. treat each edge as a single node tree, 
 *      when the edge e.dst on a different tree (has a different tid),
 *      merge the trees by assign it with the same tree id (tid).
 *   4. go through the ordered edges from the begin to the end.
 * note:
 *   this impelemtation can be improved by using Disjoint Set Data Structure.
 */
void graph_kruskal_algorithm(vector<vector<unsigned int>>& g, 
                             vector<Edge>& mst)
{
    int sz = g.size();
    vector<Edge> edges;
    for (int u = 0; u < sz; u++) {
        for (int v = u; v < sz; v++) {
            if ( g[u][v] ) {
                Edge e(u, v, g[u][v]);
                if ( edges.empty() || edges.back().weight <= e.weight ) {
                    edges.push_back(e);
                }
                else { 
                    vector<Edge>::iterator it;
                    for (it = edges.begin(); it < edges.end(); it++) {
                        if ((*it).weight > e.weight) {
                            edges.insert(it, e);
                            break;
                        }
                    }
                }
            }
        }
    }
    // for (auto e : edges) {
    //     cout << e.src << "-" << e.dst << "." << e.weight << ", ";
    // }
    // cout << endl;

    vector<int> tid(sz);
    for (int i = 0; i < sz; ++i) {
        tid[i] = i;
    }

    for (Edge e : edges) {
        if (tid[e.src] != tid[e.dst]) {
            mst.push_back(e);
            int ts = tid[e.src], td = tid[e.dst];
            for (int i = 0; i < sz; ++i) {
                if (tid[i] == ts) {
                    tid[i] = td;
                }
            }
        }
    }
}

/* testing driver code
 */
//   A---C---B
//   |  /|   |
//   | / |   |
//   D   E---F
//    \     /
//     \   /
//       G
//
vector<vector<unsigned int>> G1 = { // adajenct matrix
//   0  1  2  3  4  5  6        //
    {0, 0, 1, 2, 0, 0, 0},      //   0---2---1
    {0, 0, 2, 0, 0, 3, 0},      //   |  /|   |
    {1, 2, 0, 1, 3, 0, 0},      //   | / |   |
    {2, 0, 1, 0, 0, 0, 1},      //   3   4---5
    {0, 0, 3, 0, 0, 2, 0},      //    \     /
    {0, 3, 0, 0, 2, 0, 1},      //     \   /
    {0, 0, 0, 1, 0, 1, 0}       //       6 
};
//
Graph* graph_create_G1()
{
    Graph *g = new Graph();
    g->add_edge('A', 'C', 1);   //   A---C---B
    g->add_edge('A', 'D', 2);   //   |  /|   |
    g->add_edge('B', 'C', 2);   //   | / |   | 
    g->add_edge('B', 'F', 3);   //   D   E---F
    g->add_edge('C', 'D', 1);   //    \     /
    g->add_edge('C', 'E', 3);   //     \   /
    g->add_edge('D', 'G', 1);   //       G
    g->add_edge('E', 'F', 2);
    g->add_edge('F', 'G', 1);
    return g;
}
//
Graph* graph_create_G3()
{
    Graph *g = new Graph(true, false);
    g->add_edge('B', 'A');      // B-->A    //  B---->A
    g->add_edge('B', 'C');      // B-->C    //  |   / |
    g->add_edge('A', 'C');      // A-->C    //  v  /  v
    g->add_edge('A', 'D');      // A-->D    //  C<----D
    g->add_edge('D', 'C');      // D-->C    //   \    /
    g->add_edge('C', 'E');      // C-->E    //    \  /
    g->add_edge('D', 'E');      // D-->E    //     E
    // g->add_edge('E', 'D');      // E-->D    //     E
    return g;
}
//
void graph_display_distance(Graph *g, int *distances)
{
    int nv = g->number_vertices();
    int s;
    for (int i = 0; i < nv; ++i) { 
        if (distances[i] == 0) {
            s = i;
            break;
        }
    }
    Vertex start = g->get_vertex(s);

    cout << "  distances from vertex ";
    cout << char(start) << "(" << s << "): ";
    for (int i = 0; i < nv; ++i) {
        distances[i] == COST_MAX ? cout << "-" : cout << char(g->get_vertex(i)) << "=" << distances[i];
        if (i < nv - 1) cout << ", ";
    }
    cout << endl;
}
//
void graph_display_path(Graph *g, vector<Vertex>& path)
{
    int n = path.size();
    if (n == 0) {
        return;
    }
    Vertex s = path[0];
    Vertex d = path[n - 1];

    cout << "  path from vertex ";
    cout << char(s) << "(" << g->get_index(s) << ") to ";
    cout << char(d) << "(" << g->get_index(d) << "): ";
    for (Vertex v : path) {
        cout << char(v);
        if (v == d) break; else cout << "->";
    }
}
//
int main()
{
    int n = 10;
    int B[n];   // up tp 26 nodes for testing
    for(int i = 0; i < n; ++i) { B[i] = 'A' + rand() % 26; }
    for(int i = 0; i < n; ++i) { cout << char(B[i]) << ", ";  if ((i + 1) % 20 == 0) { cout << endl; } } cout << endl;

    Graph g;
    cout << "Add Vertices: ";
    for (int i = 0; i < n; ++i) { g.add_vertex(B[i]); }
    g.print_vertices();

    cout << "Find Vertex: ";
    n = g.number_vertices();
    for (int i = 0; i < n; ++i) {
        Vertex v = g.get_vertex(i);
        cout << g.find_vertex(v) << ", ";
    }
    cout << g.find_vertex(NOT_VERTEX) << endl;

    cout << "Delete Vertices: ";
    g.delete_vertex(NOT_VERTEX);
    n = g.number_vertices();
    for (int i = n - 1; i >= 0; --i) {
        Vertex v = g.get_vertex(i);
        g.delete_vertex(v);
        cout << char(v) << "(" << i << "), ";
    }
    g.print_vertices(); cout << endl;

    // Graph Class
    cout << "Create Undirected Graph:" << endl;
    Graph *g1 = graph_create_G1();
    g1->print_graph(); 
    cout << endl;

    cout << "Delete Edges: " << endl;
    n = g1->number_vertices();
    for (int i = n - 1; i >= 0; --i) {
        int j;
        for (j = 0; j < n; ++j) {
            if (i == j) continue;
            Vertex s = g1->get_vertex(i);
            Vertex d = g1->get_vertex(j);
            g1->delete_edge(s, d);
        }
    }
    g1->print_graph(); 
    cout << endl;

    cout << "Create Undirected Graph:" << endl;
    g1 = graph_create_G1();
    g1->print_graph(); 
    cout << endl;

    int nv = g1->number_vertices();
    int distances[nv];
    vector<Vertex> path;
    
    cout << "Shortest Path: " << endl;
    for (int i = 0; i < nv; ++ i) {
        Vertex start = g1->get_vertex(i);
        for (int j = i + 1; j < nv; ++j) {
            Vertex destination = g1->get_vertex(j);
            int dist = g1->algorithm_shortest_path(start, destination, path);
            graph_display_path(g1, path);
            cout << "  distance = " << dist << endl;
            path.clear();
        }
    }

    cout << "Bellman Ford: " << endl;
    for (int i = 0; i < nv; ++i) {
        Vertex start = g1->get_vertex(i);
        g1->algorithm_bellman_ford(start, distances);
        graph_display_distance(g1, distances);
    }

    cout << "Create Directed Graph: G3" << endl;
    Graph *g3 = graph_create_G3();
    g3->print_graph();

    cout << "Kahn's Topological Sort: " << endl;
    vector<Vertex> topo_sort;
    g3->topological_sort(topo_sort);
    if (topo_sort.size() != g3->number_vertices()) {
        cout << "Error: this graph has a cycle!" << endl;
    }
    else {
        cout << "  ";
        for (auto v : topo_sort) { cout << char(v) << ", "; }  cout << endl;
    }

    cout << "Prim's Min Spinning Tree: " << endl;
    vector<Edge> mst;
    for (int i = 0; i < g1->number_vertices(); ++i) {
        Vertex v0 = g1->get_vertex(i);
        cout << "  Start from Vertex " << char(v0) << "(" << i << "): ";
        g1->algorithm_mst_prim(v0, mst);
        int cost = 0;
        for (auto e : mst) {
            cout << char(e.src) << "-" << char(e.dst) << "." << e.weight << ", ";
            cost += e.weight;
        }
        cout << "total_cost = " << cost << endl;
        mst.clear();
    }

    cout << "Detect Cycle using DFS: " << endl;
    // g1->print_vertices();
    // g1->print_graph();
    cout << "  ";
    int vn = g1->number_vertices();
    vector<int> visited(nv, false );
    if (g1->detect_cycle_dfs(0, -1, visited)) {
        cout << "Found cycle in the graph!" << endl;
    }
    else {
        cout << "No cycle in the graph." << endl;
    }

    delete g1;
    delete g3;

    // adjacent tables
    cout << "Depth First Traversal (recursive): " << endl;
    vector<bool> vertex_visited(adjacent_table.size(), false);
    for (int i = 0; i < adjacent_table.size(); ++i) {
        for (int i = 0; i < adjacent_table.size(); ++i) { vertex_visited[i] = false; }
        cout << "  " << i << ": ";  graph_DFT_recursive(i, vertex_visited);  cout << endl;
    }

    cout << "Depth First Traversal (iterative): " << endl;
    for (int i = 0; i < adjacent_table.size(); ++i) {
        cout << "  " << i << ": ";  graph_DFT_iterative(i);  cout << endl;
    }

    cout << "Breadth First Traversal (iterative): " << endl;
    for (int i = 0; i < adjacent_table.size(); ++i) {
        cout << "  " << i << ": "; graph_BFT_iterative(i); cout << endl;
    }

    cout << "Dijkstra Shortest Path G1: " << endl;
    vector<unsigned int> distances1(G1.size(), COST_MAX);
    int start = 0;
    graph_dijkstra_algorithm(G1, start, distances1);
    cout << "  distance from (" << start << ") to: ";
    for (int i = 0; i < G1.size(); ++i) {
        cout << "(" << i << ")=" << distances1[i] << ", ";
    }
    cout << endl;

    cout << "Kruskal's Min Spinning Tree: " << endl;
    graph_kruskal_algorithm(G1, mst);
    int cost = 0;
    cout << "  ";
    for (auto e : mst) {
        cout << e.src << "-" << e.dst << "." << e.weight << ", ";
        cost += e.weight;
    }
    cout << "total_cost = " << cost << endl;

    return 0;
}
