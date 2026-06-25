#ifndef GRAPH_H
#define GRAPH_H
#include "Step.h"
struct Edge {
int destIdx;
Edge* next;
};
struct Graph {
Edge* adj[1000];
};
Graph* createGraph();
void addConnection(Graph* g, SystemData& data, int fromId, int toId);
void showGraph(Graph* g, Step* head, SystemData& data);
void freeGraph(Graph* g);
#endif