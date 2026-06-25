#include "Risk.h"
#include <iostream>
#include <fstream>
using namespace std;
void loadRules(SystemData& data) {
ifstream file("risk.txt");
if (!file.is_open()) {
data.ruleCount = 3;
data.rules[0] = "testing:10";
data.rules[1] = "validation:10";
data.rules[2] = "safety:20";
return;
}
string line;
data.ruleCount = 0;
while (getline(file, line) && data.ruleCount < MAX_RULES) {
if (!line.empty() && line[0] != '#') {
while (!line.empty() && (line.back() == '\r' || line.back() == ' '))
line.pop_back();
data.rules[data.ruleCount++] = line;
}
}
file.close();
}
void validateSteps(Step* head, Graph* g, SystemData& data) {
cout << "\n==================================================\n";
cout << " SAFETY REPORT\n";
cout << "==================================================\n";
for (Step* cur = head; cur; cur = cur->next) {
int risk = calculateRiskScore(cur, data);
cur->riskScore = risk;
cout << "\nSTEP ID : " << cur->id << "\n";
cout << "STEP NAME : " << cur->name << "\n\n";
cout << "Testing : " << cur->testingLevel << "\n";
cout << "Validation : " << cur->validationLevel << "\n";
cout << "Safety : " << cur->safetyLevel << "\n\n";
cout << "Risk Score : " << risk << " / 120\n";
string state;
if (risk <= 30) state = "SAFE";
else if (risk <= 60) state = "MODERATE";
else state = "CRITICAL";
cout << "System State : " << state << "\n";
if (state == "CRITICAL" || state == "MODERATE") {
cout << "\nWarnings:\n";
showWarnings(cur);
}
int idx = cur->idx;
Edge* e = g->adj[idx];
if (e) {
cout << "\nDependencies:\n";
while (e) {
Step* dep = data.stepList[e->destIdx];
if (dep) {
cout << " - Depends on Step " << dep->id << " (" << dep->name << ")\n";
}
e = e->next;
}
}
cout << "--------------------------------------------------\n";
}
cout << "==================================================\n";
}
bool isSystemSafe(Step* head, SystemData& data) {
for (Step* cur = head; cur != nullptr; cur = cur->next) {
int risk = calculateRiskScore(cur, data);
if (risk > 60) {
cout << "\n[ERROR] High-risk step detected: Step " << cur->id
<< " risk = " << risk << "\n";
return false;
}
}
return true;
}
void exportSafetyReport(Step* head, Graph* g, SystemData& data) {
ofstream out("report.txt");
if (!out) {
cout << "[ERROR] Could not create report.txt\n";
return;
}
out << "==================================================\n";
out << " SAFETY REPORT (EXPORT)\n";
out << "==================================================\n";
for (Step* cur = head; cur; cur = cur->next) {
int risk = calculateRiskScore(cur, data);
out << "\nSTEP ID : " << cur->id << "\n";
out << "STEP NAME : " << cur->name << "\n";
out << "Risk Score : " << risk << "/120\n";
out << "--------------------------------------------------\n";
}
out.close();
cout << "\n----------------------------------------\n";
cout << "EXPORT REPORT\n";
cout << "----------------------------------------\n";
cout << "[OK] Report exported successfully.\n";
cout << "File Name: report.txt\n";
cout << "----------------------------------------\n";
}