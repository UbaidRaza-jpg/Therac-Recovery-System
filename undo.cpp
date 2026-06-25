#include "Undo.h"
#include "Utils.h"
#include <iostream>
using namespace std;
void pushUndo(Action*& top, Step* s) {
Action* a = new Action();
a->stepId = s->id;
Step* copy = new Step(*s);
copy->next = nullptr;
copy->prev = nullptr;
copy->subHead = deepCopySubSteps(s->subHead);
a->snapshot = copy;
a->next = top;
top = a;
}
void undoLastAction(Action*& top, Step* head, SystemData& data) {
if (!top) {
cout << "[WARNING] Nothing to undo.\n";
return;
}
Action* temp = top;
top = top->next;
Step* target = findStep(head, temp->stepId);
if (target) {
Step* restored = temp->snapshot;
target->id = restored->id;
target->name = restored->name;
target->description = restored->description;
target->testingLevel = restored->testingLevel;
target->validationLevel = restored->validationLevel;
target->safetyLevel = restored->safetyLevel;
target->riskScore = restored->riskScore;
target->completed = restored->completed;
target->priority = restored->priority;
freeSubSteps(target->subHead);
target->subHead = deepCopySubSteps(restored->subHead);
cout << "\n----------------------------------------\n";
cout << "UNDO OPERATION\n";
cout << "----------------------------------------\n";
cout << "[OK] Previous changes restored successfully.\n";
cout << "Restored Step:\n";
cout << "ID : " << target->id << "\n";
cout << "Name : " << target->name << "\n";
cout << "----------------------------------------\n";
}
delete temp->snapshot;
delete temp;
}
void editStep(Step* head, int id, Action*& undoTop, SystemData& data) {
Step* s = findStep(head, id);
if (!s) {
cout << "[ERROR] Step not found.\n";
return;
}
pushUndo(undoTop, s);
cout << "\n----------------------------------------\n";
cout << "EDIT STEP\n";
cout << "----------------------------------------\n";
cout << "Editing Step : " << s->name << "\n";
cout << "Step ID : " << s->id << "\n\n";
cout << "Current Levels:\n";
cout << "Testing : " << s->testingLevel << "\n";
cout << "Validation : " << s->validationLevel << "\n";
cout << "Safety : " << s->safetyLevel << "\n";
cout << "\nEnter New Levels\n";
cout << "----------------------------------------\n";
s->testingLevel = getValidatedLevel("Testing");
s->validationLevel = getValidatedLevel("Validation");
s->safetyLevel = getValidatedLevel("Safety");
cout << "\n[OK] Step updated successfully.\n";
}