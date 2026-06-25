#ifndef STEP_H
#define STEP_H
#include <string>
using namespace std;
// Constants
const int MAX_STEPS = 1000;
const int MAX_RULES = 10;
// Step structure
struct Step {
int id;
string name;
string description;
int testingLevel;
int validationLevel;
int safetyLevel;
int riskScore;
bool completed;
int priority;
int idx; // array index (0..stepCount-1)
Step* next;
Step* prev;
Step* subHead;
};
// System wide data container (replaces globals)
struct SystemData {
string rules[MAX_RULES];
int ruleCount;
Step* stepList[MAX_STEPS];
int stepCount;
int nextStepId; // for auto-generation
};
// Function prototypes
Step* createStep(SystemData& data, int id, const string& name, const string& desc);
void insertStep(Step*& head, Step* newStep);
void addSubStep(Step* parent, Step* child);
void displaySteps(Step* head);
void displaySubSteps(Step* sub, int indent);
Step* findStep(Step* head, int id);
int getStepIndex(SystemData& data, int id);
int calculateRiskScore(Step* s, SystemData& data);
void showWarnings(Step* s);
Step* deepCopySubSteps(Step* originalSubHead);
int generateId(SystemData& data);
void deleteStepById(SystemData& data, Step*& head, int id);
void searchStepByName(Step* head, const string& name);
void freeAllSteps(Step*& head, SystemData& data);
void freeSubSteps(Step* sub);
#endif