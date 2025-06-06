#include <iostream>
#include <math.h>
#include <string>
#include <fstream>                                         //Used for file handling
// Atiya, Hassan, Ayesha
using namespace std;

const int maxstudents = 100;                             //Max number of students
const int maxassignments = 10;                           //Max number of assignments
const int maxquizzes = 10;                              //Max number of quizzes

struct Student {                                       //holds student information
    string name;                                       //holds student name
    string rollnumber;                                 //holds student roll number
    string grade;                                      //holds student grade
    int quizzes[maxquizzes];                           //holds array of quizzes
    int assignments[maxassignments];                   //holds array of assignments
    int midterm, finalterm, finalproject;              //holds student's midterm, final term and final project marks
    double total;                                      // holds student total score
};

int main() {
    string filename;                                   //variable to store filename
    cout << "Enter file name: ";
    cin >> filename;
    ifstream file(filename.c_str());                   //opens file to read data
    if (!file) {                                       //checks whether the file will open or not
        cout << "Error opening file.\n";
        return 1;
    }

    string line;                                        //variable to hold each line read from each file
    getline(file, line);                                // Read header line from file
    int Quizcount = 0, Assignmentcount = 0;             //counter for quizzes and assignments
    int commaposition = 0;                              //counter for comma in header

    for (char c : line) {                               
        if (c == ',') commaposition++;                  //count commas
        else if (c == 'Q') Quizcount++;                  //count quizzes
        else if (c == 'A') Assignmentcount++;            //count assignments
    }

    int quizweightage, assignmentweightage, midweightage, finalweightage, projectweightage;
    do {
        cout << "Total weightages should be 100.\n";
        cout << "Enter weightage of Quizzes: "; cin >> quizweightage;
        cout << "Enter weightage of Assignments: "; cin >> assignmentweightage;
        cout << "Enter weightage of Midterm: "; cin >> midweightage;
        cout << "Enter weightage of Finalterm: "; cin >> finalweightage;
        cout << "Enter weightage of Finalproject: "; cin >> projectweightage;
        if (quizweightage + assignmentweightage + midweightage + finalweightage + projectweightage != 100)     
            cout << "Invalid total. Try again.\n";                                                 //checks whether weightages equals to 100 or not
    } while (quizweightage + assignmentweightage + midweightage + finalweightage + projectweightage != 100);  //when weightages equals to 100

    Student students[maxstudents];                        //array to hold student records
    int Studentcount = 0;                                 //counter to count number of students

    while (getline(file, line)) {                         
        Student s;                                       //creates a new student object
        int position = 0, start = 0;                     //variable for parsing the line 
        int field = 0;                                   //field counter
        string fields[50];                               //array to hold parsed field
        int Fieldcount = 0;                             //Field counter

        for (int i = 0; i <= line.length(); i++) {                          
            if (line[i] == ',' || i == line.length()) {                 //check for comma or end of the line
                fields[Fieldcount++] = line.substr(start, i - start);        //extract field
                start = i + 1;                                            //update start position for the next field
            }
        }

        s.name = fields[0];                                             //read names
        s.rollnumber = fields[1];                                      //read roll numbers
        int index = 2;

        for (int i = 0; i < Quizcount; i++)
            s.quizzes[i] = atoi(fields[index++].c_str());                 //read quiz scores
        for (int i = 0; i < Assignmentcount; i++)
            s.assignments[i] = atoi(fields[index++].c_str());            //read assignments score

        s.midterm = atoi(fields[index++].c_str());                      //read midterm score
        s.finalterm = atoi(fields[index++].c_str());                    //read final term score 
        s.finalproject = atoi(fields[index++].c_str());                 //read final project score

        int quizsum = 0, assignmentsum = 0;
        for (int i = 0; i < Quizcount; i++) quizsum += s.quizzes[i];     //sum of quiz scores
        for (int i = 0; i < Assignmentcount; i++) assignmentsum += s.assignments[i]; //sum of assignment scores

        double qTotal = ((double)quizsum / (Quizcount * 10)) * quizweightage;    //calculate total quiz marks
        double aTotal = ((double)assignmentsum / (Assignmentcount * 10)) * assignmentweightage;   //calculate assignment total marks
        double mTotal = ((double)s.midterm / 100) * midweightage;                //calculate midterm total marks
        double fTotal = ((double)s.finalterm / 100) * finalweightage;            //calculate final term total marks
        double pTotal = ((double)s.finalproject / 40) * projectweightage;        //calculate project total marks

        s.total = qTotal + aTotal + mTotal + fTotal + pTotal;                //calculate overall total
        students[Studentcount++] = s;                               //store student record
    }

    file.close();        //close input file
    double sum = 0;
    for (int i = 0; i < Studentcount; i++)
        sum += students[i].total;                     //sum total scores

    double average = round(sum / Studentcount);    //calculate average score

    for (int i = 0; i < Studentcount; i++) {       //display grades based on total marks and comparing to class average
        double m = students[i].total;                        //get total score
        if (m <= average - 21) students[i].grade = "F";       
        else if (m <= average - 17) students[i].grade = "D";
        else if (m <= average - 13) students[i].grade = "C-";
        else if (m <= average - 9)  students[i].grade = "C";
        else if (m <= average - 5)  students[i].grade = "C+";
        else if (m <= average - 1)  students[i].grade = "B-";
        else if (m <= average + 3)  students[i].grade = "B";
        else if (m <= average + 7)  students[i].grade = "B+";
        else if (m <= average + 11) students[i].grade = "A-";
        else students[i].grade = "A";
    }

    int choice;
    do {
        cout << "\n1. Display Class Result \n2. Save to CSV \n3. Search by Roll Number \n4. Exit \nEnter choice: ";
        cin >> choice;

        if (choice == 1) {                                                  //if option 1 selected
            cout << "\nName\t\tRoll\t\tTotal\tGrade\n";
            for (int i = 0; i < Studentcount; i++) {
                cout << students[i].name << "\t" << students[i].rollnumber << "\t" << students[i].total << "\t" << students[i].grade << "\n";
            }                                                                 //display class result
        } else if (choice == 2) {                                              //if option 2 selected
            string outFile = filename.substr(0, filename.find(".")) + "_result.csv";  //create output file
            ofstream out(outFile.c_str());                             //open output file
            out << "Name,Roll No,Total,Grade\n";
            for (int i = 0; i < Studentcount; i++) {
                out << students[i].name << "," << students[i].rollnumber << "," << students[i].total << "," << students[i].grade << "\n";
            }                                                                      
            out.close();                                               //file close
            cout << "Results saved to " << outFile << "\n";             //confirm save
        } else if (choice == 3) {                                       //if option 3 selected
            string rollnumber;
            cout << "Enter Roll Number: ";
            cin >> rollnumber;
            bool found = false;                  //checks whether student is found or not
            for (int i = 0; i < Studentcount; i++) {
                if (students[i].rollnumber == rollnumber) {                              //check for matching roll number
                    cout << "\nName: " << students[i].name << "\nRoll: " << students[i].rollnumber
                         << "\nTotal: " << students[i].total << "\nGrade: " << students[i].grade << "\n"; //display student information
                    found = true;        //set found flag
                    break;         //exit loop
                }
            }
            if (!found) cout << "Student not found.\n";   // if roll number does not exist
        }
    } while (choice != 4);                                               //if selected choice 4
    return 0;
}

