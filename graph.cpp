#include "Graph.h"
#include <iostream>
using namespace std;
Graph* createGraph() {
Graph* g = new Graph();
for (int i = 0; i < 1000; i++)
g->adj[i] = NULL;
return g;
}
void addConnection(Graph* g, SystemData& data, int fromId, int toId) {
int fromIdx = getStepIndex(data, fromId);
int toIdx = getStepIndex(data, toId);
if (fromIdx == -1 || toIdx == -1) {
cout << "[ERROR] Step ID not found.\n";
return;
}
// Check for duplicate
Edge* e = g->adj[fromIdx];
while (e) {
if (e->destIdx == toIdx) {
// Silent duplicate – no output
return;
}
e = e->next;
}
Edge* newEdge = new Edge();
newEdge->destIdx = toIdx;
newEdge->next = g->adj[fromIdx];
g->adj[fromIdx] = newEdge;
// No output on successful connection
}
void showGraph(Graph* g, Step* head, SystemData& data) {
cout << "\n==================================================\n";
cout << " WORKFLOW GRAPH\n";
cout << "==================================================\n";
for (int i = 0; i < data.stepCount; i++) {
Step* step = data.stepList[i];
if (!step) continue;
Edge* e = g->adj[i];
if (e) {
cout << "[" << step->id << "] " << step->name << "\n";
while (e) {
Step* destStep = data.stepList[e->destIdx];
if (destStep) {
cout << " |\n";
cout << " ---> [" << destStep->id << "] " << destStep->name;
if (e->next) cout << "\n |";
}
e = e->next;
}
cout << "\n";
}
}
cout << "==================================================\n";
}
void freeGraph(Graph* g) {
for (int i = 0; i < 1000; i++) {
Edge* e = g->adj[i];
while (e) {
Edge* temp = e;
e = e->next;
delete temp;
}
}
delete g;
}