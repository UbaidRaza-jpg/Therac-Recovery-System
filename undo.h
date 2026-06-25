#ifndef UNDO_H
#define UNDO_H
#include "Step.h"
struct Action {
int stepId;
Step* snapshot;
Action* next;
};
void pushUndo(Action*& top, Step* s);
void undoLastAction(Action*& top, Step* head, SystemData& data);
void editStep(Step* head, int id, Action*& undoTop, SystemData& data);
#endif