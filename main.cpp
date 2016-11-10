// Copyright Roy Redman IV

#include <iostream>
#include <string>
#include "dynamic_array.h"
#include "Attendence_Sheet.h"


using namespace std;

int main() {
  string  name,ID;
  char answer;
  Attendence_Sheet Sheet;
  int same_Name_Count = 0;
  do {
    cout << "Please enter your name: ";
    getline(cin, name);

    Sheet.SpaceReplacer(name);
    same_Name_Count = Sheet.Same_Name_Count(name);

    if (same_Name_Count == 1)
        Sheet.Add_Student(name);

    else {
        cout << "Please enter your ID Number: ";
        cin >> ID;
        Sheet.Add_Student(name, ID);
    }
    cout << endl << "Would you like to add another Student? (y/n): ";
    cin >> answer;
    cin.ignore();
   } while (answer == 'y' || answer == 'Y');

  //cout << "Here is your Overall Students" << endl << endl;
  //Sheet.Display();
  Sheet.Exit();
  cin.get();

  return 0;
}
