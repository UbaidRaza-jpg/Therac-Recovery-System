#include "Utils.h"
#include <iostream>
using namespace std;
void pauseEnter() {
cout << "\n----------------------------------------\n";
cout << "Press ENTER to return to main menu...\n";
cout << "----------------------------------------\n";
cin.ignore(1000, '\n');
cin.get();
}
int safeGetInt(const string& prompt) {
int value;
cout << prompt;
while (!(cin >> value)) {
cin.clear();
cin.ignore(1000, '\n');
cout << "Invalid input. Please enter a valid number: ";
}
return value;
}
int getValidatedLevel(const string& levelName) {
int level;
cout << "Enter " << levelName << " Level (0-3): ";
cin >> level;
while (cin.fail() || level < 0 || level > 3) {
if (cin.fail()) {
cin.clear();
cin.ignore(1000, '\n');
}
cout << "Invalid level. Please enter between 0 and 3: ";
cin >> level;
}
return level;
}
