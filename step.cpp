#include "Step.h"
#include "Utils.h"
#include <iostream>
using namespace std;
// Deep copy for sub-steps
Step* deepCopySubSteps(Step* originalSubHead) {
if (!originalSubHead) return nullptr;
Step* newHead = nullptr;
Step* tail = nullptr;
Step* current = originalSubHead;
while (current) {
Step* copy = new Step(*current);
copy->next = nullptr;
copy->prev = nullptr;
copy->subHead = deepCopySubSteps(current->subHead);
if (!newHead) {
newHead = copy;
tail = copy;
} else {
tail->next = copy;
copy->prev = tail;
tail = copy;
}
current = current->next;
}
return newHead;
}
// Generate next available ID
int generateId(SystemData& data) {
int id = data.nextStepId;
while (true) {
bool exists = false;
for (int i = 0; i < data.stepCount; i++) {
if (data.stepList[i]->id == id) {
exists = true;
break;
}
}
if (!exists) break;
id++;
}
data.nextStepId = id + 1;
return id;
}
// Create step and add to global array
Step* createStep(SystemData& data, int id, const string& name, const string& desc) {
// SAFE ID CHECK – replaced dangerous findStep(data.stepList[0], id)
for (int i = 0; i < data.stepCount; i++) {
if (data.stepList[i] && data.stepList[i]->id == id) {
cout << "[ERROR] ID already exists!\n";
return NULL;
}
}
Step* s = new Step();
s->id = id;
s->name = name;
s->description = desc;
s->testingLevel = 0;
s->validationLevel = 0;
s->safetyLevel = 0;
s->riskScore = 0;
s->completed = false;
s->priority = 0;
s->next = s->prev = NULL;
s->subHead = NULL;
s->idx = data.stepCount;
data.stepList[data.stepCount++] = s;
return s;
}
void insertStep(Step*& head, Step* newStep) {
if (!newStep) return;
if (!head) { head = newStep; return; }
Step* temp = head;
while (temp->next) temp = temp->next;
temp->next = newStep;
newStep->prev = temp;
}
void addSubStep(Step* parent, Step* child) {
if (!parent || !child) return;
child->next = NULL;
if (!parent->subHead) {
parent->subHead = child;
} else {
Step* temp = parent->subHead;
while (temp->next)
temp = temp->next;
temp->next = child;
child->prev = temp;
}
}
void displaySubSteps(Step* sub, int indent) {
for (Step* cur = sub; cur != NULL; cur = cur->next) {
cout << string(indent, ' ') << " -> " << cur->id << ": " << cur->name << endl;
if (cur->subHead)
displaySubSteps(cur->subHead, indent + 4);
}
}
void displaySteps(Step* head) {
if (!head) { cout << "No steps available.\n"; return; }
Step* cur = head;
while (cur) {
cout << "\n==================================================\n";
cout << "STEP DETAILS\n";
cout << "==================================================\n";
cout << "Step ID : " << cur->id << "\n";
cout << "Name : " << cur->name << "\n";
cout << "Description : " << cur->description << "\n\n";
cout << "Testing : " << cur->testingLevel << "\n";
cout << "Validation : " << cur->validationLevel << "\n";
cout << "Safety : " << cur->safetyLevel << "\n";
if (cur->subHead) {
cout << "\nSub-Steps:\n";
displaySubSteps(cur->subHead, 0);
}
cout << "--------------------------------------------------\n";
cur = cur->next;
}
}
Step* findStep(Step* head, int id) {
Step* cur = head;
while (cur) {
if (cur->id == id) return cur;
cur = cur->next;
}
return NULL;
}
int getStepIndex(SystemData& data, int id) {
for (int i = 0; i < data.stepCount; i++) {
if (data.stepList[i]->id == id)
return i;
}
return -1;
}
// Risk calculation using weights from loaded rules
int calculateRiskScore(Step* s, SystemData& data) {
int testWeight = 10, valWeight = 10, safeWeight = 20;
for (int i = 0; i < data.ruleCount; i++) {
string r = data.rules[i];
if (r.find("testing:") == 0) testWeight = stoi(r.substr(8));
else if (r.find("validation:") == 0) valWeight = stoi(r.substr(11));
else if (r.find("safety:") == 0) safeWeight = stoi(r.substr(7));
}
return (3 - s->testingLevel) * testWeight +
(3 - s->validationLevel) * valWeight +
(3 - s->safetyLevel) * safeWeight;
}
void showWarnings(Step* s) {
if (s->testingLevel < 2)
cout << " - Testing level is low\n";
if (s->validationLevel < 2)
cout << " - Validation level is low\n";
if (s->safetyLevel < 2)
cout << " - Safety level is low\n";
}
void deleteStepById(SystemData& data, Step*& head, int id) {
Step* target = findStep(head, id);
if (!target) {
cout << "[ERROR] Step ID " << id << " not found.\n";
return;
}
if (target->prev) target->prev->next = target->next;
else head = target->next;
if (target->next) target->next->prev = target->prev;
int idxToRemove = target->idx;
for (int i = idxToRemove; i < data.stepCount - 1; i++) {
data.stepList[i] = data.stepList[i + 1];
data.stepList[i]->idx = i;
}
data.stepCount--;
freeSubSteps(target->subHead);
delete target;
cout << "[OK] Step deleted successfully.\n";
}
void freeSubSteps(Step* sub) {
Step* cur = sub;
while (cur) {
Step* next = cur->next;
freeSubSteps(cur->subHead);
delete cur;
cur = next;
}
}
void freeAllSteps(Step*& head, SystemData& data) {
Step* cur = head;
while (cur) {
Step* next = cur->next;
freeSubSteps(cur->subHead);
delete cur;
cur = next;
}
head = nullptr;
data.stepCount = 0;
// EXTRA RECOMMENDED FIX: clear dangling pointers in stepList
for (int i = 0; i < 1000; i++) {
data.stepList[i] = nullptr;
}
}
void searchStepByName(Step* head, const string& name) {
bool found = false;
Step* cur = head;
while (cur) {
if (cur->name.find(name) != string::npos) {
if (!found) {
cout << "\n==================================================\n";
cout << "SEARCH RESULTS\n";
cout << "==================================================\n";
found = true;
}
cout << "ID : " << cur->id << "\n";
cout << "Name : " << cur->name << "\n";
cout << "Description : " << cur->description << "\n";
cout << "--------------------------------------------------\n";
}
cur = cur->next;
}
if (!found) cout << "[WARNING] No step contains \"" << name << "\".\n";
}
