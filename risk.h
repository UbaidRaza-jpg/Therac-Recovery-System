#ifndef RISK_H
#define RISK_H
#include "Step.h"
#include "Graph.h"
void loadRules(SystemData& data);
void validateSteps(Step* head, Graph* g, SystemData& data);
bool isSystemSafe(Step* head, SystemData& data);
void exportSafetyReport(Step* head, Graph* g, SystemData& data);
#endif