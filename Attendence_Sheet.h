//#ifndef _ATTENDENCE_SHEET_H
//#define _ATTENDENCE_SHEET_H

#include "dynamic_array.h"
#include <time.h>
#include <iostream>
#include <fstream>
#include <ctime>

#ifdef __linux__
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#endif

using namespace std;


struct student {
  string StudentName;
  string id_number;
  double attendence_Rate = 0;
  int timesAttended = 0;

};

struct Sessions {
    string Date;
    student * students_Attended;
    int _size;
    Sessions(string Dt, student P[], int _sze) {
        Date = Dt;
        _size = _sze;
        students_Attended = new student[_sze];
        for(int i = 0; i < _sze; i++) {

            students_Attended[i] = P[i];
        }
    }
};


class Attendence_Sheet {

 public:
  Attendence_Sheet();
  void Read_Past_Sheet();
  void Organize_Top_Students();
  void Display();
  string DateFormat(int,int,int);
  void Add_Student(string);
  void Add_Student(string, string);
  int Same_Name_Count(string);
  int Student_Search(string,string);
  void SpaceReplacer(string&);
  void Display_Sessions();
  void Exit();
  void Rewrite_File();
  void SaveSession();
  ~Attendence_Sheet();

 private:
  Dynamic_Array<student> Students;
  Dynamic_Array<student> Todays_Students;
  int daily_Student_count;
  Dynamic_Array<student *> Top_Students;
  Dynamic_Array<Sessions> Si_Sessions;
  int Total_Sessions;
  string Todays_Date;
  string File_Location;
  bool Today_Created;
  const double Percentage_Cap = 10.0;

};

//#endif
