#include <iostream>
#include<fstream>
#include <sstream>
#include <string>
#include <vector>
#include "search.h"
using namespace std;

void deleteTime(vector<vector<string>>& results, Storage& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;
    Storage* obj = &csvData;
    string check;
    string input;
    string indx;
    searchInputs.at(0) = " " + searchInputs.at(0) + ":00";
    input = searchInputs.at(0);
    for (unsigned int k = 0; k < obj->getOrigSize(); k++) { // searches for time 00:00 //getOrigSize
        Use* row1 = new Use;
        *row1 = obj->getRow(k);
       
        check = row1->getTime();
        while (input.find(check) != std::string::npos) {
            indx = std::to_string(k);
            cout << "We delete at index: " << indx << endl;
            obj->deleteRow(k);
            obj->updateDelData();
            *row1 = obj->getRow(k);
            check = row1->getTime();
        }
        
        delete row1;
    }
    if (obj->checkCount()) {
        obj->resetCount();
    }
    return;
};

void deleteDate(vector<vector<string>>& results, Storage& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;
    Storage* obj = &csvData;
    string check;
    string input;

    input = searchInputs.at(0);
    for (unsigned int k = 0; k < obj->getOrigSize(); k++) { // searches for date XX/XX/XXXX 
        Use* row1 = new Use;
        *row1 = obj->getRow(k);
        check = row1->getDate();
        while (input.find(check) != string::npos) {
            obj->deleteRow(k);
            obj->updateDelData();
            *row1 = obj->getRow(k);
            check = row1->getDate();
        }
        //if (input.find(check) != string::npos) { //if search element is found it is deleted
        //    obj->deleteRow(k);
        //}
        delete row1;
        //obj->updateDelData();
    }
    if (obj->checkCount()) {
        obj->resetCount();
    }
    return;
};

void deleteTimeAndDate(vector<vector<string>>& results, Storage& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;
    string temp;
    string dateAndTime = "\"" + searchInputs.at(1) + " " + searchInputs.at(0) + ":00" + "\"";
    Storage* obj = &csvData;

    for (unsigned int k = 0; k < obj->getOrigSize(); k++) {// searches for time and day "XX/XX/XXXX 00:00:00" 
        Use* row1 = new Use;
        *row1 = obj->getRow(k);
        temp = "\"" + row1->getDate() + " " + row1->getTime() + "\"";
        while (dateAndTime.compare(temp) == 0) {
            obj->deleteRow(k);
            obj->updateDelData();
            *row1 = obj->getRow(k);
            temp = "\"" + row1->getDate() + " " + row1->getTime() + "\"";
        }
        //if (dateAndTime.compare(temp) == 0) { //delete is equal
        //    obj->deleteRow(k);
        //}
        
        delete row1;
        //obj->updateDelData();
    }
    if (obj->checkCount()) {
        obj->resetCount();
    }
    return;
};

void deleteLocation(vector<vector<string>>& results, Storage& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;
    Storage* obj = &csvData;
    string inpLat;
    string inpLon;
    string ltde;
    string lgtde;

    inpLat = searchInputs.at(0);
    inpLon = searchInputs.at(1);
    for (unsigned int k = 0; k < obj->getOrigSize(); k++) { // searches for Location: lon/lat 
        Use* row1 = new Use;
        *row1 = obj->getRow(k);
        ltde = row1->getLat();
        lgtde = row1->getLong();
        while ((inpLat.compare(ltde) == 0 && inpLon.compare(lgtde) == 0)) {
            obj->deleteRow(k);
            obj->updateDelData();
            *row1 = obj->getRow(k);
            ltde = row1->getLat();
            lgtde = row1->getLong();
        }
        delete row1;
        //obj->updateDelData();
    }
    if (obj->checkCount()) {
        obj->resetCount();
    }
    return;
};

void deleteBase(vector<vector<string>>& results, Storage& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;
    searchInputs.at(0) = "\"" + searchInputs.at(0) + "\"";
    Storage* obj = &csvData;
    string input;
    string baseId;

    input = searchInputs.at(0);
    for (unsigned int k = 0; k < obj->getOrigSize(); k++) {// searches for Base#: BXXXXX 
        Use* row1 = new Use;
        *row1 = obj->getRow(k);
        baseId = row1->getBase();
        while((input.compare(baseId) == 0)) {
            obj->deleteRow(k);
            obj->updateDelData();
            *row1 = obj->getRow(k);
            baseId = row1->getBase();
        }
        delete row1;
       // obj->updateDelData();
    }
   
    return;
};

void deleteSpecific(vector<vector<string>>& results, Storage& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;
    string temp;
    searchInputs.at(4) = "\"" + searchInputs.at(4) + "\"";
    string dateAndTime = "\"" + searchInputs.at(1) + " " + searchInputs.at(0) + ":00" + "\"";
    Storage* obj = &csvData;
    string inputBase;
    string inputLt;
    string inputLg;
    string ltde;
    string lgtde;
    string baseId;
    
    inputLt = searchInputs.at(2);
    inputLg = searchInputs.at(3);
    inputBase = searchInputs.at(4);
    for (unsigned int k = 0; k < obj->getOrigSize(); k++) {         // searches for Location: lon/lat 
        Use* row1 = new Use;
        *row1 = obj->getRow(k);
        temp = "\"" + row1->getDate() + " " + row1->getTime() + "\"";
        ltde = row1->getLat();
        lgtde = row1->getLong();
        baseId = row1->getBase();
        if ((dateAndTime.compare(temp) == 0 && inputLt.compare(ltde) == 0 && inputLg.compare(lgtde) == 0 && inputBase.compare(baseId) == 0)) {
            obj->deleteRow(k);
            //cout << "we returned from delete row" << endl;
            k = obj->getOrigSize();
        }
        delete row1;
        obj->updateDelData();
    }
    //cout << "We exited the for loop" << endl;
    
    //cout << "we returned from updateDelData" << endl;
    if (obj->checkCount()) {
        obj->resetCount();
    }
    return;
};
