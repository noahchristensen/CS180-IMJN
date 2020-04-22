#include <iostream>
#include<fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

void parseLine(string line, vector<string>& results, int index) {
    stringstream X(line);
    string column;
    vector<string> results1;

    while (getline(X, column, ',')) { //splits line into columns 
         results1.push_back(column);
    }

    results = results1;
    return;
}

void read(vector<vector<string>>& result)
{
    ifstream fin;
    string line1;
    string line2;

    // Open an existing file
    fin.open("uber-raw-data-apr14.csv");
    int index = 0;
    vector<string> results; 

    while (!fin.eof()) {
        fin >> line1;   //These two are to format the first column of the csv file
        fin >> line2;   
        line1 = line1 + " " + line2; // combines the date column string with the rest of its corresponding row "4/1/2014 + 0:11:00",40.769,-73.9549,"B02512"
        parseLine(line1, results, index); // parses line1 to results vector with four elements:  "Date Time", Lat, Lon, "Base",
        result.push_back(results); //pushes results vector to result vector
        if (index % 10000 == 0) {
            cout << "Parsed " << index << " lines" <<  endl;
        }
        index++;
    }
    return;
}



int main()
{
    vector<string> miniVec;
    vector<vector<string>> results;
    cout << "Wait..." << endl;

    read(results); //reads csv file and parses data into vector of vector<string>
    string input1;
    string input2;
    string input3;
    string input4;
    string input5;
    string searchOption;
    vector < vector<string>> searchRes;
    while (searchOption != "q") {
        searchRes.clear();
        cout << "Choose search option: " << endl;
        cout << "a: Time/Date" << endl;
        cout << "b: Location" << endl;
        cout << "c: Base#" << endl;
        cout << "d: Time/Date/Location/Base#" << endl;
        cout << "q: Quit" << endl;

            getline(cin, searchOption);

        if (searchOption == "q") {
            cout << "Bye..." << endl;
            return 0;
        }
        if (searchOption == "a") {
            cout << "Enter date XX/XX/XXXX : " << endl;
            getline(cin, input1);

            cout << "Enter time XX:XX : " << endl;
            getline(cin, input2);

            input1 = "\"" + input1 + " " + input2 + ":00" + "\""; // formats input to "XX/XX/XXXX 00:00:00"
            cout << "Entered: " << input1 << endl;

            for (unsigned int k = 0; k < results.size(); k++) {         // searches for time and day "XX/XX/XXXX 00:00:00" 
                miniVec = results.at(k);
                if (input1.compare(miniVec.at(0)) == 0) {
                    searchRes.push_back(miniVec);
                }
            }
        }
        else if (searchOption == "b") {
            cout << "Enter Longitude: " << endl;
            getline(cin, input1);

            cout << "Enter Latitude: " << endl;
            getline(cin, input2);

            cout << "Entered: " << "Lon: " << input1 << " " << "Lat: " << input2 << endl;

            for (unsigned int k = 0; k < results.size(); k++) {         // searches for Location: lon/lat 
                miniVec = results.at(k);
                if (input1.compare(miniVec.at(1)) == 0 && input2.compare(miniVec.at(2)) == 0) {
                    searchRes.push_back(miniVec);
                }
            }

        }
        else if (searchOption == "c") {
            cout << "Enter Base# BXXXXX: " << endl;
            getline(cin, input1);

            cout << "Entered: " << "Base#: " << input1 << endl;
            input1 = "\"" + input1 + "\""; //formats input to "BXXXXX"

            for (unsigned int k = 0; k < results.size(); k++) {         // searches for Base#: BXXXXX
                miniVec = results.at(k);
                if (input1.compare(miniVec.at(3)) == 0) {
                    searchRes.push_back(miniVec);
                }
            }
        }
        else if (searchOption == "d") {
            cout << "Enter date XX/XX/XXXX : " << endl;
            getline(cin, input1);

            cout << "Enter time XX:XX : " << endl;
            getline(cin, input2);

            cout << "Enter Longitude: " << endl;
            getline(cin, input3);

            cout << "Enter Latitude: " << endl;
            getline(cin, input4);

            cout << "Enter Base# BXXXXX: " << endl;
            getline(cin, input5);

            input1 = "\"" + input1 + " " + input2 + ":00" + "\""; // formats input to "XX/XX/XXXX 00:00:00"
            input5 = "\"" + input5 + "\""; //formats input to "BXXXXX"

            for (unsigned int k = 0; k < results.size(); k++) {         // searches for Location: lon/lat 
                miniVec = results.at(k);
                if (input1.compare(miniVec.at(0)) == 0 && input3.compare(miniVec.at(1)) == 0 && input4.compare(miniVec.at(2)) == 0 && input5.compare(miniVec.at(3)) == 0) {
                   
                    searchRes.push_back(miniVec);
                }
            }

        }

        // prints results stored in searchRes vector
        cout << "Results: " << endl;
        for (unsigned int j = 0; j < searchRes.size(); j++) {
            miniVec = searchRes.at(j);
            for (unsigned int l = 0; l < miniVec.size(); l++) {
                cout << miniVec.at(l) << " ";
            }
            cout << endl;
        }
    }
    return 0;
}