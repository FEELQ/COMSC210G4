// Group 4

#include <iostream>
#include <string>
using namespace std;

#include <fstream>
#include <cstring>
#include <iomanip>

#include "FP_AssociativeArray.h"
#include "DynamicArray.h"

#include <algorithm> //

struct reqCourses{
    DynamicArray<string> courses;
};

struct Major
{
    string name;
    string department;
    double admitGpaHigh;
    double admitGpaLow;
    double enrollGpaHigh;
    double enrollGpaLow;

    string applicationNum;
    string admitNum;
    string enrollNum;

    string admitRate;
    string yieldRate;
    reqCourses* requires;
};



// prototype
vector<string> displayMajorInfo(AssociativeArray<string, AssociativeArray<string, Major>>& , vector<vector<string>>&, vector<string>);
void specificInfo(AssociativeArray<string, AssociativeArray<string, Major>>& , string , string );
void checkMyGpa(double , Major);

//from 9
void associativeBubbleSorting(AssociativeArray<int, string>&);
void vectorBubbleSorting(vector<string>&);


int main()
{
    AssociativeArray<string, AssociativeArray<string, Major>> schools;

    AssociativeArray<string,string> filenames;
    filenames["UCB"] = "UC_Berkeley.txt";
    filenames["UCLA"] = "UC_LosAngeles.txt";
    filenames["UCSB"] = "UC_Santa Barbara.txt"; 
    filenames["UCSD"] = "UC_San Diego.txt"; 
    filenames["UCD"] = "UC_Davis.txt"; 
    filenames["UCI"] = "UC_Irvine.txt";  // need to add all

    vector<string> v_fstring = filenames.keys();

    // Input Declaration
    char *token;
    char buf[1000];
    const char *const tab = "\t";


    ifstream fin;
    // Loop & Parsing
    for (int i = 0; i < v_fstring.size(); i++)
    {
        fin.open(filenames[v_fstring.at(i)]);

        if (!fin.good())
            throw "I/O error";

        while (fin.good())
        {
            string line;
            getline(fin, line);
            strcpy(buf, line.c_str());

            if (buf[0] == 0)
                continue;

            // parsing
            const string broadDiscipline(token = strtok(buf, tab));
            if (broadDiscipline == "Broad Discipline ")
            {
                continue;
            }
            const string college((token = strtok(0, tab)) ? token : "");
            const string majorName((token = strtok(0, tab)) ? token : "");
            const string admitGPA((token = strtok(0, tab)) ? token : "");
            const string admitR((token = strtok(0, tab)) ? token : "");
            const string enrollGPA((token = strtok(0, tab)) ? token : "");
            const string yieldR((token = strtok(0, tab)) ? token : "");
            const string admitNum((token = strtok(0, tab)) ? token : "");
            const string applicantNum((token = strtok(0, tab)) ? token : "");
            const string enrollNum((token = strtok(0, tab)) ? token : "");

            // store strings
            string enrollGpaL;
            string enrollGpaH;
            if (enrollGPA.find('-') == string::npos) // if data is masked
            {
                enrollGpaL = "0";
                enrollGpaH = "0";
            }
            else
            {
                string temp1(enrollGPA.begin(), enrollGPA.begin() + enrollGPA.find('-'));
                enrollGpaL = temp1;
                string temp2(enrollGPA.begin() + enrollGPA.find('-') + 1, enrollGPA.end());
                enrollGpaH = temp2;
            }

            string admitGpaL;
            string admitGpaH;
            if (admitGPA.find('-') == string::npos)
            {
                admitGpaL = "0";
                admitGpaH = "0";
            }
            else
            {
                string temp1(admitGPA.begin(), admitGPA.begin() + admitGPA.find('-'));
                admitGpaL = temp1;
                string temp2(admitGPA.begin() + admitGPA.find('-') + 1, admitGPA.end());
                admitGpaH = temp2;
            }

            // check duplicates & store key & counts
            Major tempMajor = {majorName, college, 
            atof(admitGpaH.c_str()), 
            atof(admitGpaL.c_str()), 
            atof(enrollGpaH.c_str()), 
            atof(enrollGpaL.c_str()),
            applicantNum,
            admitNum,
            enrollNum,
            admitR,
            yieldR};
            schools[v_fstring.at(i)][majorName] = tempMajor;
        }
        fin.close();
    }
    //----------------------------------------------------------------------------------------------------------------------------------------------------


    // sort and store
    vector<vector<string>> v_majors;

    for(int i = 0; i < schools.size(); i++)
    {
            v_majors.push_back(schools[v_fstring.at(i)].keys());
    }
    
    for(int i = 0; i < v_majors.size(); i++)
    {
        vectorBubbleSorting(v_majors.at(i));
    }

    while(true)
    {
        // user interface begins
        vector<string> v_major = displayMajorInfo(schools, v_majors, v_fstring);

        string keyword;
        cout << "\nEnter The Number to Look Specific Information(Q/q to Quit): ";
        getline(cin, keyword);
        int num = atoi(keyword.c_str());
        specificInfo(schools, v_major.at(num * 2 - 2), v_major.at(num * 2 - 1)); // array, school, major

        cout << "1. Check Required Course" << endl;
        cout << "2. Check My GPA" << endl;
        cout << "Enter Q/q to Quit: ";

        double gpa;
        while (true)
        {
            string choice;
            getline(cin, choice);
            if (choice == "Q" || choice == "q")
                return 0;
            // else if (choice == "1")
            //     showRequired();
            else if (choice == "2")
            {
                cout << "\nEnter your GPA: ";
                getline(cin, keyword);
                gpa = atof(keyword.c_str());
                checkMyGpa(gpa, schools[v_major.at(num * 2 - 2)][v_major.at(num * 2 - 1)]);
                break;
            }
        }
    }

    cout << "\n\n    GOOD BYE ! \n\n";

    return 0;
}


// show major info by every school
vector<string> displayMajorInfo(AssociativeArray<string, AssociativeArray<string, Major>>& array, vector<vector<string>>& v_majors, vector<string> univNames) 
{
    string tempmajor;
    cout << "Enter Name of Major: ";
    getline(cin, tempmajor);

    string keyword = string();
    for(int i = 0; i < tempmajor.length(); i++)
    {
        keyword += toupper(tempmajor.at(i));
    }
    int listNum = 1;

    vector<string> mlist;

    vector<vector<string>> major2D = v_majors;
    vector<string> temp_UnivName = univNames;
    for(int i = 0; i < univNames.size(); i++)
    {   
        cout << "\n" << temp_UnivName.at(i) << ":" << endl;
        for(int j = 0; j < v_majors.at(i).size(); j++){
            if (strstr(array[temp_UnivName.at(i)][major2D.at(i).at(j)].name.c_str(),keyword.c_str()) != NULL)
            {
                cout << "(" << listNum++ << ") > " << setw(45) << left << array[temp_UnivName.at(i)][major2D.at(i).at(j)].name << endl;
                mlist.push_back(temp_UnivName.at(i));
                mlist.push_back(array[temp_UnivName.at(i)][major2D.at(i).at(j)].name);
            }
        }
    }
    return mlist;
}

// sort vector
void vectorBubbleSorting(vector<string>& array)
{
    for (int i = 0; i < array.size(); i++)
    {
        for (int j = i + 1; j < array.size(); j++)
        {
            if (array[j] < array[i])
            {
                swap(array[i], array[j]);
            }
        }
    }
}

// sort AssociativeArray
void associativeBubbleSorting(AssociativeArray<int, string>& array)
{
    for (int i = 0; i < array.size(); i++)
    {
        for (int j = i + 1; j < array.size(); j++)
        {
            if (array[j] < array[i])
            {
                swap(array[i], array[j]);
            }
        }
    }
}

void specificInfo(AssociativeArray<string, AssociativeArray<string, Major>>& array, string school, string major)
{
    cout << "\n > " << school << ",\n   " << major << endl;
    cout << setw(45) << "" << "  Department:   " <<  array[school][major].department << endl;
    cout << setw(45) << "" << "   Admit GPA:   "; 
    if(array[school][major].admitGpaHigh == 0)
    {
        cout << "masked" << endl;
    }else{
      cout << setprecision(2) << fixed << array[school][major].admitGpaLow << "-" << array[school][major].admitGpaHigh << endl;
    }
    cout << setw(45) << "" << "  Enroll GPA:   ";
    if(array[school][major].enrollGpaHigh == 0)
    {
        cout << "masked" << endl;
    }else{
      cout << setprecision(2) << fixed << array[school][major].enrollGpaLow << "-" << array[school][major].enrollGpaHigh << endl;
    }
    cout << setw(45) << "" << "  Applicants:   ";
    if(array[school][major].applicationNum == "masked")
    {
        cout << "masked" << endl;
    }else{
      cout << array[school][major].applicationNum << endl;
    }
    cout << setw(45) << "" << "      Admits:   ";
    if(array[school][major].admitNum == "masked")
    {
        cout << "masked" << endl;
    }else{
      cout << array[school][major].admitNum << endl;
    }
    cout << setw(45) << "" << "     Enrolls:   ";
    if(array[school][major].enrollNum == "masked")
    {
        cout << "masked" << endl;
    }else{
      cout << array[school][major].enrollNum << endl;
    }
    cout << setw(45) << "" << "   Admit Rate:  ";
    if(array[school][major].admitRate == "masked")
    {
        cout << "masked" << endl;
    }else{
        cout.precision(2);
      cout << atof(array[school][major].admitRate.c_str()) * 100 << "%" << endl;
    }
    cout << setw(45) << "" << "   Yield Rate:  ";
    if(array[school][major].yieldRate == "masked")
    {
        cout << "masked" << endl;
    }else{
        cout.precision(2);
      cout << atof(array[school][major].yieldRate.c_str()) * 100 << "%" << endl;
    }
    
}

void checkMyGpa(double myGpa, Major major)
{
    if(myGpa > major.admitGpaHigh)
        cout << "You are in perfect shape";
    else if(myGpa > major.admitGpaLow)
        cout << "Keep maintaining your GPA";
    else
        cout << "Put effort in your PIQ essay";

    cout << endl << endl;
}

void showRequired()
{
    
}
