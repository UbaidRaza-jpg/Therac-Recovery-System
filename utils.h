#ifndef UTILS_H
#define UTILS_H
#include <string>
using namespace std;
void pauseEnter();
int safeGetInt(const string& prompt);
int getValidatedLevel(const string& levelName);
#endif