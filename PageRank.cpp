#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <omp.h>
#include <assert.h>
#include <vector>
#include <queue>
#include "node.h"

using std::cout;
using std::endl;
using std::ifstream;
using std::priority_queue;
using std::string;
using std::unordered_map;
using std::vector;

struct drand48_data randBuffer;
// switch for debugging mode (NO GDB)
int debug = 0;

void load_graph(string filename, unordered_map<int, node> &graph, vector<int> &keys);
bool coin_toss(double d);
void traverse(unordered_map<int, node> &graph, vector<int> &keys, int node_id, int L, double D);
void page_rank(unordered_map<int, node> &graph, vector<int> &keys, int L, double D, double &time_elapsed);

// takes 4 inputs
// graph filename
// L: walk length
// D: damping factor
// # of threads
// assumes N = n; where N = number of vertices |V| and n = number of random surfers
int main(int argc, char *argv[])
{
    int rank, L, p = 1;
    double D, time_elapsed;

    string filename;
    // hash table nodeID -> struct Node
    unordered_map<int, node> graph;
    // set of vertices
    // serves as a random generator for initial webpage
    vector<int> keys;

    srand48_r(time(NULL), &randBuffer);
    if (argc < 5)
    {
        cout << "Usage: pagerank graphfilename {L: length of walk} {D: damping ratio}";
        cout << " {p: num threads}" << endl;
        exit(1);
    }

    if (argc == 5)
    {
        L = atoi(argv[2]);
        D = atof(argv[3]);
        p = atoi(argv[4]);
        assert(p >= 1);
        filename = argv[1];
        if (debug)
        {
            cout << "Filename: " << filename;
            cout << " L: " << L;
            cout << " D: " << D;
            cout << " Procs: " << p << endl;
        }
    }

    omp_set_num_threads(p);

    // #pragma omp parallel
    //     {
    //         assert(p == omp_get_num_threads());
    //     }

    if (debug)
        cout << "Loading graph..." << endl;
    load_graph(filename, graph, keys);

    if (debug)
        cout << "Ranking pages..." << endl;
    page_rank(graph, keys, L, D, time_elapsed);

    if (debug)
        cout << "Time (ms): " << time_elapsed * 1000 << endl;
    // csv: p,d,k,runtime,filename
    cout << p << "," << D << "," << L << "," << time_elapsed * 1000;
    cout << "," << filename << endl;
}

void load_graph(string filename, unordered_map<int, node> &graph, vector<int> &keys)
{
    ifstream graphfile;
    char line[100];
    graphfile.open(filename);

    int src_id, dest_id;

    // while not end of file, keep reading
    while (!graphfile.eof())
    {
        // load in buffer 'line' of size 100
        graphfile.getline(line, 100);

        // skip over commented lines
        if (line[0] == '#')
            continue;

        // graph u -> v, edge from u to v
        sscanf(line, "%d %d", &src_id, &dest_id);

        // not found
        if (graph.find(src_id) == graph.end())
        {
            // insert
            graph[src_id] = node(src_id);
            keys.push_back(src_id);
        }
        // Node not found
        if (graph.find(dest_id) == graph.end())
        {
            // insert
            graph[dest_id] = node(dest_id);
            keys.push_back(dest_id);
        }

        // generate adjacency list
        graph[src_id].outlinks.push_back(dest_id);
    }
    // close fstream
    graphfile.close();
}

bool coin_toss(double d)
{
    double coin;
    drand48_r(&randBuffer, &coin);

    if (coin <= d)
        return true; //jump
    return false;    //dont jump
}

void traverse(unordered_map<int, node> &graph, vector<int> &keys, int node_id, int L, double D)
{
    // for walk length
    for (int i = 0; i < L; i++)
    {
// atomic method:
#pragma omp atomic
        graph[node_id].visit_count += 1;

        if (coin_toss(D)) // jump
            // pick node at random
            node_id = graph[keys[rand() % keys.size()]].id;
        // edge case: consider implicit self link and stay at the curr webpage for another "step"
        else // pick neighbor
        {
            int index = rand() % (graph[node_id].outlinks.size() + 1);
            // out of bound error check
            if (index != graph[node_id].outlinks.size())
                node_id = graph[node_id].outlinks[index];
        }
    }
}

void page_rank(unordered_map<int, node> &graph, vector<int> &keys, int L, double D, double &time_elapsed)
{
    srand(time(0));
    // heap
    priority_queue<node, vector<node>, LessThanByVisit> rankings;
    int i;

    time_elapsed = omp_get_wtime();
#pragma omp parallel for schedule(dynamic) shared(keys, graph) private(i)
    // n = |V| (assumption)
    for (i = 0; i < keys.size(); i++)
    {
        // keys[i] ==  random initial start vertex
        traverse(graph, keys, keys[i], L, D);
    }
    time_elapsed = omp_get_wtime() - time_elapsed;

    // could be made more efficient by keeping size of fixed
    // operations to use would be push followed by pop
    for (unordered_map<int, node>::iterator it = graph.begin(); it != graph.end(); it++)
        rankings.push(it->second);

    if (debug)
    {
        cout << "Top 5 pages: " << endl;
        for (int i = 0; i < 5; i++)
        {
            cout << "id: " << rankings.top().id << " visit_count: " << rankings.top().visit_count << endl;
            rankings.pop();
        }
    }
}
