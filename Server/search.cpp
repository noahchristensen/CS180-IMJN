#include <iostream>
#include<fstream>
#include <sstream>
#include <string>
#include <vector>
#include "search.h"
using namespace std;

void searchDate(vector<vector<string>>& results1, Storage& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;
    Storage* obj = &csvData;
    string in1;

    for (int k = 0; k < obj->getOrigSize(); k++) {// searches for date XX/XX/XXXX
        Use* row1 = new Use;
        *row1 = obj->getRow(k);

        if (row1->getDate().compare(searchInputs.at(0)) == 0) {

            in1 = "\"" + row1->getDate() + " " + row1->getTime() + "\"";

            miniVec.push_back(in1);
            in1 = row1->getLat();

            miniVec.push_back(in1);
            in1 = row1->getLong();

            miniVec.push_back(in1);
            in1 = row1->getBase();

            miniVec.push_back(in1);
            results1.push_back(miniVec);
            miniVec.clear();
        }
        delete row1;
    }

    return;
};

void searchTime(vector<vector<string>>& results1, Storage& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;
    Storage* obj = &csvData;
    string in1;

    searchInputs.at(0) = searchInputs.at(0) + ":00";
    cout << searchInputs.at(0) << endl;
    for (unsigned int k = 0; k < obj->getOrigSize(); k++) {// searches for time 00:00
        Use* row1 = new Use;
        *row1 = obj->getRow(k);
        if (row1->getTime().compare(searchInputs.at(0)) == 0) {
            in1 = "\"" + row1->getDate() + " " + row1->getTime() + "\"";

            miniVec.push_back(in1);
            in1 = row1->getLat();

            miniVec.push_back(in1);
            in1 = row1->getLong();

            miniVec.push_back(in1);
            in1 = row1->getBase();

            miniVec.push_back(in1);
            results1.push_back(miniVec);
            miniVec.clear();
            //pushes found searches to results
        }
        delete row1;
    }
    return;
};

void searchDateTime(vector<vector<string>>& results1, Storage& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;
    Storage* obj = &csvData;
    string in1;

    for (unsigned int k = 0; k < obj->getOrigSize(); k++) { // searches for time and day "XX/XX/XXXX 00:00:00" 
        Use* row1 = new Use;
        *row1 = obj->getRow(k);
        if (searchInputs.at(1).compare(row1->getDate()) == 0 && searchInputs.at(0).compare(row1->getTime()) == 0) {
            in1 = "\"" + row1->getDate() + " " + row1->getTime() + "\"";

            miniVec.push_back(in1);
            in1 = row1->getLat();

            miniVec.push_back(in1);
            in1 = row1->getLong();

            miniVec.push_back(in1);
            in1 = row1->getBase();

            miniVec.push_back(in1);
            results1.push_back(miniVec);
            miniVec.clear();
            //pushes found searches to results
        }
        delete row1;
    }
    return;
};

void searchLocation(vector<vector<string>>& results1, Storage& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;
    Storage* obj = &csvData;
    string in1;
    for (unsigned int k = 0; k < obj->getOrigSize(); k++) {// searches for Location: lon/lat 
        Use* row1 = new Use;
        *row1 = obj->getRow(k);
        if (searchInputs.at(0).compare(row1->getLat()) == 0 && searchInputs.at(1).compare(row1->getLong()) == 0) {
            in1 = "\"" + row1->getDate() + " " + row1->getTime() + "\"";

            miniVec.push_back(in1);
            in1 = row1->getLat();

            miniVec.push_back(in1);
            in1 = row1->getLong();

            miniVec.push_back(in1);
            in1 = row1->getBase();

            miniVec.push_back(in1);
            results1.push_back(miniVec);
            miniVec.clear();
            //pushes found searches to results
        }
        delete row1;
    }
    return;
};

void searchBase(vector<vector<string>>& results1, Storage& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;
    Storage* obj = &csvData;
    string in1;
    searchInputs.at(0) = "\"" + searchInputs.at(0) + "\"";
    for (unsigned int k = 0; k < obj->getOrigSize(); k++) {// searches for Base#: BXXXXX
        Use* row1 = new Use;
        *row1 = obj->getRow(k);
        if (searchInputs.at(0).compare(row1->getBase()) == 0) {
            in1 = "\"" + row1->getDate() + " " + row1->getTime() + "\"";

            miniVec.push_back(in1);
            in1 = row1->getLat();

            miniVec.push_back(in1);
            in1 = row1->getLong();

            miniVec.push_back(in1);
            in1 = row1->getBase();

            miniVec.push_back(in1);
            results1.push_back(miniVec);
            miniVec.clear();
            //pushes found searches to results
        }
        delete row1;
    }
    return;
};

void searchSpecific(vector<vector<string>>& results1, Storage& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;
    string temp;
    Storage* obj = &csvData;
    string in1;

    searchInputs.at(4) = "\"" + searchInputs.at(4) + "\"";
    string dateAndTime = "\"" + searchInputs.at(1) + " " + searchInputs.at(0) + ":00" + "\"";
    for (unsigned int k = 0; k < obj->getOrigSize(); k++) { //
        Use* row1 = new Use;
        *row1 = obj->getRow(k);
        temp = "\"" + row1->getDate() + " " + row1->getTime() + "\"";
        if (dateAndTime.compare(temp) == 0 && searchInputs.at(2).compare(row1->getLat()) == 0 && searchInputs.at(3).compare(row1->getLong()) == 0 && searchInputs.at(4).compare(row1->getBase()) == 0) {
            in1 = "\"" + row1->getDate() + " " + row1->getTime() + "\"";

            miniVec.push_back(in1);
            in1 = row1->getLat();

            miniVec.push_back(in1);
            in1 = row1->getLong();

            miniVec.push_back(in1);
            in1 = row1->getBase();

            miniVec.push_back(in1);
            results1.push_back(miniVec);
            miniVec.clear();
            //pushes found searches to results
        }
        delete row1;
    }
    return;
};

