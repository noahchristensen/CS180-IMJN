#include <iostream>
#include<fstream>
#include <sstream>
#include <string>
#include <vector>
#include "search.h"
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
        index++;
    }
    return;
};

void searchDate(vector<vector<string>>& results, vector<vector<string>>& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;
    for (unsigned int k = 0; k < csvData.size(); k++) {         // searches for date XX/XX/XXXX 
        miniVec = csvData.at(k);
        if (miniVec.at(0).find(searchInputs.at(0)) != string::npos) {
            results.push_back(miniVec);
        }
    }
};

void searchTime(vector<vector<string>>& results, vector<vector<string>>& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;
    searchInputs.at(0) = searchInputs.at(0) + ":00";
    for (unsigned int k = 0; k < csvData.size(); k++) {         // searches for time 00:00
        miniVec = csvData.at(k);
        if (miniVec.at(0).find(searchInputs.at(0)) != string::npos) {
            results.push_back(miniVec);
        }
    }
};

void searchDateTime(vector<vector<string>>& results, vector<vector<string>>& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;
    string dateAndTime = "\"" + searchInputs.at(1) + " " + searchInputs.at(0)  + ":00" + "\"";
    for (unsigned int k = 0; k < csvData.size(); k++) {         // searches for time and day "XX/XX/XXXX 00:00:00" 
        miniVec = csvData.at(k);
        if (dateAndTime.compare(miniVec.at(0)) == 0) {
            results.push_back(miniVec);
        }
    }
};

void searchLocation(vector<vector<string>>& results, vector<vector<string>>& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;

    for (unsigned int k = 0; k < csvData.size(); k++) {         // searches for Location: lon/lat 
        miniVec = csvData.at(k);
        if (searchInputs.at(0).compare(miniVec.at(1)) == 0 && searchInputs.at(1).compare(miniVec.at(2)) == 0) {
            results.push_back(miniVec);
        }
    }
};

void searchBase(vector<vector<string>>& results, vector<vector<string>>& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;
    searchInputs.at(0) = "\"" + searchInputs.at(0) + "\"";
    for (unsigned int k = 0; k < csvData.size(); k++) {         // searches for Base#: BXXXXX
        miniVec = csvData.at(k);
        if (searchInputs.at(0).compare(miniVec.at(3)) == 0) {
            results.push_back(miniVec);
        }
    }
};

void searchSpecific(vector<vector<string>>& results, vector<vector<string>>& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;
    searchInputs.at(4) = "\"" + searchInputs.at(4) + "\"";
    string dateAndTime = "\"" + searchInputs.at(1) + " " + searchInputs.at(0) + ":00" + "\"";
    for (unsigned int k = 0; k < csvData.size(); k++) {         // searches for Location: lon/lat 
        miniVec = csvData.at(k);
        if (dateAndTime.compare(miniVec.at(0)) == 0 && searchInputs.at(2).compare(miniVec.at(1)) == 0 && searchInputs.at(3).compare(miniVec.at(2)) == 0 && searchInputs.at(4).compare(miniVec.at(3)) == 0) {
            results.push_back(miniVec);
        }
    }
};

void parseClient(char buf[1024], vector<vector<string>>& csvData, vector<vector<string>>& results) {
    int timeFlag = 0;
    int dateFlag = 0;
    int locationFlag1 = 0;
    int locationFlag2 = 0;
    int baseFlag = 0;
    int mostFlag = 0;
    int leastFlag = 0;
    vector<string> clientDat;
    stringstream X(buf);
    string columns;
    string column;
    vector<string> searchInputs;
    while (getline(X, columns, ',')) {
        clientDat.push_back(columns);
    }
    for (unsigned i = 0; i < clientDat.size(); i++) {

        string compare = "Time: ";
        if (clientDat.at(i).find(compare) != string::npos) {

            if (compare != clientDat.at(i)) {

                string::size_type f = clientDat.at(i).find(compare);
                clientDat.at(i).erase(f, compare.length());
                searchInputs.push_back(clientDat.at(i));
                timeFlag = 1;
            }
        }
        compare = "Date: ";
        if (compare.find(clientDat.at(i)) != string::npos) {
            if (compare != clientDat.at(i)) {

                string::size_type f = clientDat.at(i).find(compare);
                clientDat.at(i).erase(f, compare.length());
                searchInputs.push_back(clientDat.at(i));
                dateFlag = 1;
            }
         }
        compare = "Base: ";
        if (compare.find(clientDat.at(i)) != string::npos) {
            if (compare != clientDat.at(i)) {

                string::size_type f = clientDat.at(i).find(compare);
                clientDat.at(i).erase(f, compare.length());
                searchInputs.push_back(clientDat.at(i));
                baseFlag = 1;
            }
        }
        compare = "Latitude: ";
        if (compare.find(clientDat.at(i)) != string::npos) {
            if (compare != clientDat.at(i)) {

                string::size_type f = clientDat.at(i).find(compare);
                clientDat.at(i).erase(f, compare.length());
                searchInputs.push_back(clientDat.at(i));
                locationFlag1 = 1;
            }
        }
        compare = "Longitude: ";
        if (compare.find(clientDat.at(i)) != string::npos) {
            if (compare != clientDat.at(i)) {

                string::size_type f = clientDat.at(i).find(compare);
                clientDat.at(i).erase(f, compare.length());
                searchInputs.push_back(clientDat.at(i));
                locationFlag2 = 1;
            }
        }

        for (unsigned h = 0; h < searchInputs.size(); h++) {
            cout << searchInputs.at(h) << " " << endl;
        }

        if (timeFlag && dateFlag && locationFlag1 && locationFlag2 && baseFlag) {
            searchSpecific(results, csvData, searchInputs);
        }
        else if (timeFlag && dateFlag) {
            searchDateTime(results, csvData, searchInputs);
        }
        else if (timeFlag) {
            searchTime(results, csvData, searchInputs);
        }
        else if (dateFlag) {
            searchDate(results, csvData, searchInputs);
        }
        else if (locationFlag1 && locationFlag2) {
            searchLocation(results, csvData, searchInputs);
        }
        else if (baseFlag) {
            searchBase(results, csvData, searchInputs);
        }
    }
    return;
};
