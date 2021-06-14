// testQueue.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
// Module2Step5TestGenerator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <random>
#include <climits>
#include <string>
#include "templatedDynamicQueue.h"
#include "stdQueue.h"
#include <queue>
#include "llQueue.h"

void randomStream(std::string filename, int length);
int testStream(std::string fileName);
bool statusCheck(farmingdale::statusCode fsc1, farmingdale::statusCode fsc2, farmingdale::statusCode fsc3, farmingdale::statusCode fsc4);
void resultCheck(int result);

int main()
{
    //instance variables
    std::string file;
    int lines;

    //menu code
    bool openMenu = true;
    int choice;
    while (openMenu) {
        std::cout << "1: Exit \n2:Run randomStream() \n3:Run testStream \n4:Run both\n"; //Menu Text
        std::cin >> choice;

        switch (choice) {
        case 1:
            std::cout << "Exiting program...";
            openMenu = false;
            break;
        case 2:
            std::cout << "Enter the file name:  ";
            std::cin >> file;
            std::cout << "Enter your desired number of iterations:  ";
            std::cin >> lines;
            randomStream(file, lines);
            break;
        case 3:
            std::cout << "Enter the file name:  ";
            std::cin >> file;
            resultCheck(testStream(file));
            break;
        case 4:
            std::cout << "Enter the file name:  ";
            std::cin >> file;
            std::cout << "Enter your desired number of iterations:  ";
            std::cin >> lines;
            randomStream(file, lines);
            resultCheck(testStream(file));
            break;
        default:
            std::cout << "Please select a valid option by entering a number from 1 to 4.\n";
        }
    }


}

void randomStream(std::string filename, int length) {

    std::random_device rd; //obtains seed for random number engine
    std::mt19937 gen(rd());//standard mersenne_twister_engine seeded with rd()

    std::uniform_int_distribution<> distrib(0, 5); //generates random number with inputted bounds
    std::uniform_int_distribution<> distrib2(INT_MIN, INT_MAX); //generates random number with predefined climit values

    std::string chara; //stores randomly selected character

    std::ofstream myfile;
    myfile.open(filename);
    for (int i = 0; i < length; i++) {

        switch (distrib(gen)) {
        case 0:
            chara = "A";
            break;
        case 1:
            chara = "D";
            break;
        case 2:
            chara = "P";
            break;
        case 3:
            chara = "E";
            break;
        case 4:
            chara = "C";
            break;
        case 5:
            chara = "S";
            break;
        }

        if (chara == "A") {
            myfile << chara << " " << distrib2(gen);
        }
        else {
            myfile << chara;
        }

        //checks if the end of the file has been reached, if not then a new line is added, otherwise the file would have one more line that it should
        if (i != length - 1) {
            myfile << "\n";
        }
    }
    myfile.close();

}

 int testStream(std::string fileName) {

     //create 4 dynamic queues
     farmingdale::queue<std::string> q1;
     farmingdale::queue<std::string> q2;
     farmingdale::stdQueue<std::string> q3;
     farmingdale::llQueue<std::string> q4;

     //open file
     std::ifstream testFile;
     testFile.open(fileName);

     //strings for storing each line as the loop iterates
     std::string testString;
     //stores line number for error reporting
     int line = 1;

     //https://stackoverflow.com/questions/3482064/counting-the-number-of-lines-in-a-text-file learned about std::getline from this
     while (std::getline(testFile, testString)) { //using getLine will exit the loop 

         //I really wanted to use a switch statement since I prefer them to nested ifs or if/elses but I couldn't get that to work 
         //so I opted for the nested if/else instead
         
         //code for 'A'
         
         if (testString[0] == 'A') {
             //stores number after A'
             int stringSize = testString.size();
             std::string intString = "";
             for (int i = 2; i < stringSize; i++)
             {
                 intString += testString[i];
             }

             //calls the statusCheck method using the enqueue calls, if the call returns false, return the line number
             if (!statusCheck(q1.enqueue(intString), q2.enqueue(intString), q3.enqueue(intString), q4.enqueue(intString))){
                 return line;
             }

         }
         else if(testString[0] == 'P') { //P CODE

             //strings for peek storage
             std::string pq1;
             std::string pq2;
             std::string pq3;
             std::string pq4;

             //calls statusCheck using the peek calls
             if (!statusCheck(q1.peek(pq1), q2.peek(pq2), q3.peek(pq3), q4.peek(pq4))) {
                 return line;
             }
             else {
                 if (pq1 == pq2 && pq1 == pq3 && pq1 == pq4) {

                 }
                 else {
                     return line;
                 }
             }

         }//copy/pasted the 'P' code and changed it to work for dequeue
         else if (testString[0] == 'D') { //D CODE

             //strings for dequeue storage
             std::string dq1;
             std::string dq2;
             std::string dq3;
             std::string dq4;

             //calls statusCheck using the dequeue calls
             if (!statusCheck(q1.dequeue(dq1), q2.dequeue(dq2), q3.dequeue(dq3), q4.dequeue(dq4))) {
                 return line;
             }
             else {
                 if (dq1 == dq2 && dq1 == dq3 && dq1 == dq4) {

                 }
                 else {
                     return line;
                 }
             }

         }
         else if (testString[0] == 'E')
         {
             // compare the calls to check for similar states between queues 
             if (q1.isEmpty() == q2.isEmpty() && q1.isEmpty() == q3.isEmpty() && q1.isEmpty() == q4.isEmpty()){}
             else
             {
                 return line;
             }
         }

         line++;
     }

     //almost forgot this
     testFile.close();

     //flawless execution? return 0
     return 0;
}

 //wrote this method so I wouldn't have to write this if statement repeatedly
 bool statusCheck(farmingdale::statusCode fsc1, farmingdale::statusCode fsc2, farmingdale::statusCode fsc3, farmingdale::statusCode fsc4) {
     //get the status codes for each queue, compare them, and either return true or false
     
     //I wrote this condition out of curiosity and kept it because it worked
     if (fsc1 == fsc2 && fsc1 == fsc3 && fsc1 == fsc4) {
         return true;
     }
     else
     {
         return false;
     }
 }

 //method that checks if testStream was successful or not
 void resultCheck(int result) {
     if (result == 0) {
         std::cout << "There were no errors, congratz!\n";
     }
     else
     {
         std::cout << "Error on line " << result << "! Revise your code!\n";
     }
 }