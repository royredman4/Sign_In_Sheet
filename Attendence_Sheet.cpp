// Copyright Roy Redman IV
#include "Attendence_Sheet.h"
#include <fstream>

Attendence_Sheet::Attendence_Sheet() {
  Today_Created = false;
  Total_Sessions = 0;
  daily_Student_count = 0;
  time_t now = time(0);

  struct tm *date = localtime(&now);
  Todays_Date = DateFormat(date->tm_mon,date->tm_mday,1900+ date->tm_year);
  //cout << "Todays Date is " << Todays_Date;

  //#if __linux__
  File_Location = "/home/";
  string username = getpwuid(getuid())->pw_name;
  File_Location = File_Location + username + "/Sign_In_Sheet_Program/";
  // cout << "File location is " << File_Location;
  system("mkdir -p ~/Sign_In_Sheet_Program/");

  Read_Past_Sheet();
  Organize_Top_Students();

}

void Attendence_Sheet::Read_Past_Sheet() {
  ifstream IstudentSession, Istudent;
  student temp;
  student * Temp_Students;
  string Temp_Location = File_Location + "Attendence.txt";
  string Sessions_Location = File_Location + "Session_Count.txt";
  string Temp_Date;
  int index;
  //bool Student_Exists = false;

  IstudentSession.open(Sessions_Location);

  if (!IstudentSession.is_open()) {
  ofstream New_Student(Sessions_Location);
  New_Student << "Total_Sessions= 0";
  New_Student.close();
    }
  else {
  // Ignores "Total_Sessions="
  string throwaway;
  IstudentSession >> throwaway;
  //IstudentSession.ignore();
  IstudentSession >> Total_Sessions;
  IstudentSession.close();

  Istudent.open(Temp_Location);
  }
  if (!Istudent.is_open()) {
    ofstream Temp_Student(Temp_Location);
    Temp_Student.close();
    }

  // Make a while loop starting here to identify each session
for (int i = 0; i < Total_Sessions;i++) {
    if (i == 0)
        Istudent.ignore(1,' ');
    else
        Istudent.ignore(3,' ');

  // Need to find a way to check if the date is today.
  // if It is, don't create a new day on the output
  getline(Istudent,Temp_Date);
  // Use a get date function and compare it to that date. If it is
  // todays date, check off a boolean value "Today_Created"
  // This will be used for when we output to the file we wont make a second
  // of the same date

  //Istudent >> Temp_Date;
  if (Temp_Date == Todays_Date) {
      Today_Created = true;
  }


  //Ignores Session_Student_Count="
  Istudent.ignore(256, ' ');
  Istudent >> daily_Student_count;
  Istudent.ignore();

Temp_Students = new student[daily_Student_count];


for (int q = 0; q < daily_Student_count; q++) {
        Istudent >> temp.StudentName;
        Istudent.ignore();
        Istudent >> temp.id_number;



     index = Student_Search(temp.StudentName, temp.id_number);
  if (index >= 0) {
    Students[index].timesAttended++;
    //
    Temp_Students[q] = Students[index];
  }
  else {
    temp.timesAttended = 1 ;
    Students.push_back(temp); 
    index = Student_Search(temp.StudentName,temp.id_number);
    //
    Temp_Students[q] = Students[index];
  }
  if (Today_Created == true) {
      Todays_Students.push_back(temp);
  }
}
    Si_Sessions.push_back(Sessions(Temp_Date,Temp_Students, daily_Student_count));
    delete[] Temp_Students;
}
    //Display_Sessions();
  Istudent.close();
}

void Attendence_Sheet::Display_Sessions() {
    cout << "Here is session for the date: ";
    for (int i = Si_Sessions.size(), q = 0;q < i; q++ )
    {
     cout << Si_Sessions[q].Date << endl;

     for (int t = 0; t < Si_Sessions[q]._size; t++ )
     {

         cout << Si_Sessions[q].students_Attended[t].StudentName << endl;

     }
      cout << endl << endl;

    }
}

void Attendence_Sheet::Organize_Top_Students() {
    int q = Students.size();
    for (int i = 0; i < q;i++) {
        Students[i].attendence_Rate = double(Students[i].timesAttended)/Total_Sessions * 100;

        //This is a fun trick to only keep two decimal points after the value is calculated
        Students[i].attendence_Rate = int(Students[i].attendence_Rate* 100);
        Students[i].attendence_Rate /= 100;

        if (Students[i].attendence_Rate >= Percentage_Cap)
            Top_Students.push_back(&Students[i]);
    }
    int p = Top_Students.size();
    for (int j = 0; j < p; j++) {
        for(int k = j+1; k < p; k++) {
            if (Top_Students[j]->attendence_Rate < Top_Students[k]->attendence_Rate) {
                student* temp = Top_Students[j];
                Top_Students[j] = Top_Students[k];
                Top_Students[k] = temp;

            }
        }
    }
    cout << "Students from most attended to least attended" << endl;
    for (int i = 0; i < Top_Students.size(); i++) {
        cout << "Student: " << Top_Students[i]->StudentName << " has a %" << Top_Students[i]->attendence_Rate << " Attendence rate" << endl;
    }
}

string Attendence_Sheet::DateFormat(int month, int day, int year) {
    string Month_Names [] = {"January","February","March","April","May","June","July","August","September","October","November","December"};
    string _month = Month_Names[month];
    char _day[3];
    char _year[5] = {'2','0'};
    year -= 2000;

    for (int i = 0; i < 2; i++)
    {
        if (day >= 10) {
            _day[i] = ('0' + (day/10));
            day%=10;
        }
        else {
            _day[i] = '0' + day;
            break;
        }
    }

    for (int i = 2; i < 4; i++) {
        if (year >= 10) {
            _year[i] = ('0' + (year/10));
            year%= 10;
        }
        else {
            _year[i] = '0' + year;
            break;
        }
    }


    return _month + " " + _day + "," + _year;
}
int Attendence_Sheet::Same_Name_Count(string name) {

    int total = 0;
    int _size = Students.size();

    for (int i = 0; i < _size; i++) {
        if (Students[i].StudentName == name)
            total++;
    }
    return total;
}


int Attendence_Sheet::Student_Search(string nme, string ID) {
    int _size = Students.size();
    for (int i = 0; i < _size; i++) {
      if (Students[i].StudentName == nme && Students[i].id_number == ID) {
        return i;
      }
    }
    return -1;
}

void Attendence_Sheet::Display() {
    int _size = Students.size();
    for (int i = 0; i < _size; i++) {
        cout << "Student Name: " << Students[i].StudentName << endl;
        cout << "ID Number: " << Students[i].id_number << endl;
        cout << endl << endl;
    }
}

void Attendence_Sheet::Add_Student(string name) {
    int _size = Students.size();
    for (int i = 0; i < _size; i++) {
        if (Students[i].StudentName == name) {
            Students[i].timesAttended++;
            Todays_Students.push_back(Students[i]);
            return;
        }
    }
}

void Attendence_Sheet::Add_Student(string name, string id) {
  SpaceReplacer(name);
  int index = Student_Search(name,id);
  if (index >= 0) {
    Students[index].timesAttended++;
    Todays_Students.push_back(Students[index]);
  }
  else {
    student temp;
    temp.StudentName = name;
    temp.id_number = id;
    temp.timesAttended = 1;
    Students.push_back(temp);
    Todays_Students.push_back(Students[Students.size()-1]);
  }

  //cout << endl << "Your name is " << Students[0].StudentName << endl;

}

void Attendence_Sheet::SpaceReplacer(string& fileName)
{
    int i = fileName.size();

    for (int j = 0; j < i; j++)
    {
        if (fileName[j] == ' ')
            fileName[j] = '_';
    }

}
void Attendence_Sheet::Exit() {
    if(Today_Created)
        Rewrite_File();
    else
        SaveSession();
}

void Attendence_Sheet::Rewrite_File() {
    string Attendence_Location = File_Location + "Attendence.txt";
    string Session_Location = File_Location + "Session_Count.txt";
    ofstream Output(Attendence_Location);
    Output << endl;

    for(int i = 0; i < Si_Sessions.size() && Si_Sessions[i].Date != Todays_Date; i++) {
        Output << Si_Sessions[i].Date << endl;
        Output << "Session_Student_Count= " << Si_Sessions[i]._size << endl << endl;
        for (int q = 0; q < Si_Sessions[i]._size; q++) {
            Output << Si_Sessions[i].students_Attended[q].StudentName << "\t" << Si_Sessions[i].students_Attended[q].id_number << endl;
        }
        Output << endl << endl;

    }
    Output << Todays_Date << endl;
    Output << "Session_Student_Count= " << Todays_Students.size() << endl << endl;
    for (int k = 0; k < Todays_Students.size();k++) {
        Output << Todays_Students[k].StudentName << "\t" << Todays_Students[k].id_number << endl;
    }
    Output << endl;

    Output.close();

    Output.open(Session_Location);
    Output << "Total_Sessions= " << Si_Sessions.size();
    Output.close();
}

void Attendence_Sheet::SaveSession() {
    string Temp_Location = File_Location + "Attendence.txt";
    string Session_Location = File_Location + "Session_Count.txt";
    ofstream output(Temp_Location,ios::app);
    if (Today_Created == false) {
        output << endl;
        output << Todays_Date << endl;
        output << "Session_Student_Count= " << Todays_Students.size() << endl << endl;
        Total_Sessions++;
    }
    else {
        cout << endl << endl << "CHANGE STUDENT SIZE COUNT FOR TODAY'S DATE!!" << endl << endl;
    }
    int i = Todays_Students.size();


    for (int q = 0; q < i;q++) {
        output << Todays_Students[q].StudentName << "\t" << Todays_Students[q].id_number << endl;
    }
    output << endl;
    output.close();

    ofstream Session_Output(Session_Location);
    Session_Output << "Total_Sessions= " << Total_Sessions;
    Session_Output.close();

}

Attendence_Sheet::~Attendence_Sheet() {

    Students.Empty_Array();
    Todays_Students.Empty_Array();
    Top_Students.Empty_Array();
    for (int i=0; i < Si_Sessions.size(); i++) {
        delete [] Si_Sessions[i].students_Attended;
    }
    Si_Sessions.Empty_Array();


}


