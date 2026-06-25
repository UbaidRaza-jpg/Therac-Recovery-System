#include <iostream>
#include <string>
#include <fstream>
using namespace std;
#include "Step.h"
#include "Graph.h"
#include "Undo.h"
#include "Risk.h"
#include "Utils.h"
void displayMainMenu();
void addNewStep(SystemData& data, Step*& head);
void addSubStepMenu(SystemData& data, Step* head);
void connectWorkflow(Graph* g, SystemData& data);
void executeTreatment(Step* head, SystemData& data);
void saveProject(SystemData& data, Step* head, Graph* g, Action* undoTop);
void loadProject(SystemData& data, Step*& head, Graph*& g, Action*& undoTop);
void deleteStepMenu(SystemData& data, Step*& head);
void searchStepMenu(Step* head);
void printHeader() {
cout << "========================================\n";
cout << " __ __ ________ ___ \n";
cout << " | | | | | _____| / \\ \n";
cout << " | | | | | |___ / ^ \\ \n";
cout << " | | | | | ___| / /_\\ \\ \n";
cout << " | |__| | | | / _____ \\ \n";
cout << " |________| |__| /__/ \\__\\ \n";
cout << "\n SOFTWARE HOUSE\n";
cout << "========================================\n";
}
int main() {
printHeader();
SystemData data;
data.ruleCount = 0;
data.stepCount = 0;
data.nextStepId = 201;
// Silent load of rules
loadRules(data);
Step* head = NULL;
Graph* g = createGraph();
Action* undoTop = NULL;
// System initialization message (only one shown)
cout << "\n----------------------------------------\n";
cout << "SYSTEM INITIALIZATION\n";
cout << "----------------------------------------\n";
cout << "[OK] Risk rules loaded successfully\n";
cout << "[OK] Workflow graph initialized\n";
cout << "[OK] Undo system initialized\n";
cout << "----------------------------------------\n";
// Example steps – no output from addConnection (silent now)
int id1 = generateId(data);
Step* s1 = createStep(data, id1, "Switch Treatment Mode", "Switch e- / X-ray");
insertStep(head, s1);
int id2 = generateId(data);
Step* s2 = createStep(data, id2, "Set Dosage", "Configure radiation dose");
insertStep(head, s2);
int id3 = generateId(data);
Step* s3 = createStep(data, id3, "Activate Beam", "Fire the beam");
insertStep(head, s3);
addConnection(g, data, id1, id2);
addConnection(g, data, id2, id3);
bool running = true;
while (running) {
displayMainMenu();
int choice;
cin >> choice;
if (cin.fail()) {
cin.clear();
cin.ignore(1000, '\n');
cout << "Invalid input.\n";
pauseEnter();
continue;
}
switch (choice) {
case 1:
addNewStep(data, head);
break;
case 2:
addSubStepMenu(data, head);
break;
case 3:
displaySteps(head);
break;
case 4:
connectWorkflow(g, data);
break;
case 5:
showGraph(g, head, data);
break;
case 6:
validateSteps(head, g, data);
break;
case 7:
{
int id = safeGetInt("Enter Step ID to edit: ");
editStep(head, id, undoTop, data);
}
break;
case 8:
undoLastAction(undoTop, head, data);
break;
case 9:
executeTreatment(head, data);
break;
case 10:
saveProject(data, head, g, undoTop);
break;
case 11:
loadProject(data, head, g, undoTop);
break;
case 12:
deleteStepMenu(data, head);
break;
case 13:
searchStepMenu(head);
break;
case 14:
exportSafetyReport(head, g, data);
break;
case 0:
cout << "\n----------------------------------------\n";
cout << "SYSTEM SHUTDOWN\n";
cout << "----------------------------------------\n";
cout << "Cleaning allocated memory...\n";
freeAllSteps(head, data);
freeGraph(g);
cout << "[OK] Graph memory released\n";
cout << "[OK] Sub-step memory released\n";
cout << "Thank you for using THERAC-25 Recovery System\n";
cout << "Goodbye.\n";
cout << "----------------------------------------\n";
running = false;
break;
default:
cout << "Invalid choice.\n";
}
if (running && choice != 0) pauseEnter();
}
return 0;
}
void displayMainMenu() {
cout << "\n==================================================\n";
cout << " THERAC-25 RECOVERY SYSTEM\n";
cout << "==================================================\n\n";
cout << " [1] Add New Step\n";
cout << " [2] Add Sub-Step\n";
cout << " [3] View All Steps\n";
cout << " [4] Connect Workflow Steps\n";
cout << " [5] View Workflow Graph\n\n";
cout << " [6] Run Safety Validation\n";
cout << " [7] Edit Step Levels\n";
cout << " [8] Undo Last Action\n";
cout << " [9] Execute Treatment\n\n";
cout << " [10] Save Project\n";
cout << " [11] Load Project\n";
cout << " [12] Delete Step\n";
cout << " [13] Search Step By Name\n";
cout << " [14] Export Safety Report\n\n";
cout << " [0] Exit System\n";
cout << "==================================================\n";
cout << "Enter Your Choice: ";
}
void addNewStep(SystemData& data, Step*& head) {
cout << "\n----------------------------------------\n";
cout << "ADD NEW STEP\n";
cout << "----------------------------------------\n";
string name, desc;
cout << "Enter Step Name: ";
cin.ignore();
getline(cin, name);
cout << "Enter Description: ";
getline(cin, desc);
int id = generateId(data);
Step* ns = createStep(data, id, name, desc);
if (ns) {
insertStep(head, ns);
cout << "\n[OK] Step created successfully.\n";
cout << "Assigned Step ID: " << id << "\n";
}
cout << "----------------------------------------\n";
}
void addSubStepMenu(SystemData& data, Step* head) {
int pid = safeGetInt("Parent ID: ");
Step* par = findStep(head, pid);
if (!par) {
cout << "[ERROR] Parent not found.\n";
return;
}
string name, desc;
cout << "Sub-step name: ";
cin.ignore();
getline(cin, name);
cout << "Description: ";
getline(cin, desc);
int id = generateId(data);
Step* child = createStep(data, id, name, desc);
if (child) {
addSubStep(par, child);
cout << "[OK] Sub-step added.\n";
}
}
void connectWorkflow(Graph* g, SystemData& data) {
int from = safeGetInt("From Step ID: ");
int to = safeGetInt("To Step ID: ");
addConnection(g, data, from, to);
// No output here because addConnection is silent
cout << "[OK] Connection added (if IDs valid).\n";
}
void executeTreatment(Step* head, SystemData& data) {
cout << "\n==================================================\n";
cout << " TREATMENT EXECUTION STATUS\n";
cout << "==================================================\n";
if (isSystemSafe(head, data)) {
cout << "[OK] System safety validation passed.\n";
cout << "Treatment execution approved.\n";
} else {
cout << "[ERROR] High-risk steps detected.\n";
cout << "Treatment execution blocked for safety.\n";
}
cout << "==================================================\n";
}
void saveProject(SystemData& data, Step* head, Graph* g, Action* undoTop) {
cout << "\n----------------------------------------\n";
cout << "SAVE PROJECT\n";
cout << "----------------------------------------\n";
ofstream out("project.dat", ios::binary);
if (!out) {
cout << "[ERROR] Could not save project.\n";
return;
}
out.write((char*)&data.stepCount, sizeof(int));
Step* cur = head;
while (cur) {
out.write((char*)&cur->id, sizeof(int));
size_t len = cur->name.size();
out.write((char*)&len, sizeof(size_t));
out.write(cur->name.c_str(), len);
len = cur->description.size();
out.write((char*)&len, sizeof(size_t));
out.write(cur->description.c_str(), len);
out.write((char*)&cur->testingLevel, sizeof(int));
out.write((char*)&cur->validationLevel, sizeof(int));
out.write((char*)&cur->safetyLevel, sizeof(int));
cur = cur->next;
}
for (int i = 0; i < data.stepCount; i++) {
Edge* e = g->adj[i];
while (e) {
int fromId = data.stepList[i]->id;
int toId = data.stepList[e->destIdx]->id;
out.write((char*)&fromId, sizeof(int));
out.write((char*)&toId, sizeof(int));
e = e->next;
}
}
int end = -1;
out.write((char*)&end, sizeof(int));
out.close();
cout << "[OK] Project saved successfully.\n";
cout << "File Name: project.dat\n";
cout << "----------------------------------------\n";
}
void loadProject(SystemData& data, Step*& head, Graph*& g, Action*& undoTop) {
cout << "\n----------------------------------------\n";
cout << "LOAD PROJECT\n";
cout << "----------------------------------------\n";
ifstream in("project.dat", ios::binary);
if (!in) {
cout << "[ERROR] No saved project found.\n";
return;
}
freeAllSteps(head, data);
freeGraph(g);
g = createGraph();
while (undoTop) {
Action* temp = undoTop;
undoTop = undoTop->next;
delete temp->snapshot;
delete temp;
}
int count;
in.read((char*)&count, sizeof(int));
data.stepCount = 0;
head = nullptr;
for (int i = 0; i < count; i++) {
int id;
in.read((char*)&id, sizeof(int));
size_t len;
in.read((char*)&len, sizeof(size_t));
char* buf = new char[len + 1];
in.read(buf, len);
buf[len] = 0;
string name(buf);
delete[] buf;
in.read((char*)&len, sizeof(size_t));
buf = new char[len + 1];
in.read(buf, len);
buf[len] = 0;
string desc(buf);
delete[] buf;
int test, val, safe;
in.read((char*)&test, sizeof(int));
in.read((char*)&val, sizeof(int));
in.read((char*)&safe, sizeof(int));
Step* s = createStep(data, id, name, desc);
if (s) {
s->testingLevel = test;
s->validationLevel = val;
s->safetyLevel = safe;
insertStep(head, s);
}
}
while (true) {
int from, to;
in.read((char*)&from, sizeof(int));
if (from == -1) break;
in.read((char*)&to, sizeof(int));
addConnection(g, data, from, to);
}
in.close();
cout << "[OK] Project loaded successfully.\n";
cout << "----------------------------------------\n";
}
void deleteStepMenu(SystemData& data, Step*& head) {
cout << "\n----------------------------------------\n";
cout << "DELETE STEP\n";
cout << "----------------------------------------\n";
int id = safeGetInt("Enter Step ID to delete: ");
deleteStepById(data, head, id);
cout << "----------------------------------------\n";
}
void searchStepMenu(Step* head) {
cout << "\n----------------------------------------\n";
cout << "SEARCH STEP\n";
cout << "----------------------------------------\n";
string name;
cout << "Enter Step Name (or partial): ";
cin.ignore();
getline(cin, name);
searchStepByName(head, name);
cout << "----------------------------------------\n";
}
