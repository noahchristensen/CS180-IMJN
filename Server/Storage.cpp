#include <iostream>
#include<fstream>
#include <sstream>
#include <string>
#include <vector>
#include "search.h"
using namespace std;

void Storage::insertRow(Use& row) { //not sure if passby referrence FIX?
    data[size] = row;
    size++;
    size2++;
    return;
}; //inserts row into end of data[] increases size by 1 

void Storage::deleteRow(int index) {
    //Get index values for comparison and update stored results accordingly 
    vector<string> inputs;
    string time1;
    string loc1;
    time1 = std::to_string(getRow(index).getHour()) + ":" + std::to_string(getRow(index).getMin()); // time now in XX:XX format
    inputs.push_back(time1);
    inputs.push_back(getRow(index).getDate());
    inputs.push_back(getRow(index).getLat());
    inputs.push_back(getRow(index).getLong());
    inputs.push_back(getRow(index).getBase());
    //cout << "About to call flagChecks2" << endl;
    flagChecks2(*this, inputs); // handles incremental analytics after a delete
    //cout << "Back from flagChecks2" << endl;
    data[index].clearUse();
    size--;
    incCount();
    return;
}; // clears fields of an index object & sets its exists = false

void Storage::convertStorage() { // pushes data in format: "Date Time", Lat, Lon, "Base" to results
    vector <string> miniVec;
    string dateTime;
    string baseNum;
    for (unsigned i = 0; i < size2; i++) {
        dateTime = "\"" + getRow(i).getDate() + " " + getRow(i).getTime() + "\"";
        baseNum = "\"" + getRow(i).getBase() + "\"";
        miniVec.push_back(dateTime); //"Date Time"
        miniVec.push_back(getRow(i).getLat()); // XX.XXXX
        miniVec.push_back(getRow(i).getLong()); // -YY.YYYY
        miniVec.push_back(baseNum); // BXXXXX
        results.push_back(miniVec);
        miniVec.clear();

    }
    return;
}; // done right before sending the client data : client imports

Use& Storage::getRow(int index) {
    return data[index];
}; // returns a Use object from Data[] reference 

int Storage::getSize() {
    return size;
}; //checks the size of data[] includes deletes

int Storage::getOrigSize() {
    return size2;
}; //gets original size before deletes 

void Storage::updateDelData() {
    for (unsigned i = 0; i < size2; i++) {
        if (!getRow(i).checkExists()) {
            //cout << "We found a deleted data entry" << endl;
            if (i == size2 - 1) {
                //size2--;
                return;
            }
            adjustData(i); //switch element from the back to deleted index
        }
    }
    return;
};

void Storage::adjustData(int index) {
    data[index] = data[size2 - 1];
    getRow(size2 - 1).clearUse();
    size2--;
    size = size2;
    return;
}; // moves element from end to index passed in: for updateDelData

vector <vector<string>>& Storage::getResults() {
    return results;
}; // gets vector with results

void Storage::flagChecks1(Storage& csvData, vector<string>& insertInputs) { //insertInputs format: time,date,lat,long,base#
    //vector<vector<string>> newResults;
    int num;
    vector<string> temp;
    vector<string> results1;
    string column;
    int i = 0;
    string in1;
    //string tempStr1;
    int index = 0;
    string ltd;
    string lgt;

    if (csvData.checkFlag("mostTime")) {
        stringstream X(insertInputs.at(0));
        while (getline(X, column, ':')) { //splits hour/min into columns 
            results1.push_back(column);
        }
        column = results1.at(0);
        for (i = 0; i < csvData.getMTime().size(); i++) {
            results1 = csvData.getMTime().at(i); // vector for MTime has format time,count
            if (results1.at(0) == column) { //found matching time 
                num = std::stoi(results1.at(1)); // converts string to int
                num = num + 1;
                results1.at(1) = std::to_string(num); //converts int to string and stores value
                csvData.getMTime().at(i) = results1; //update element
                if (csvData.getMTime().at(0) == results1) {
                    //do nothing: we incremented the largest value
                }
                else if (std::stoi(csvData.getMTime().at(i - 1).at(1)) > num) {//upper element is smaller: Swap
                    temp = csvData.getMTime().at(i - 1);
                    csvData.getMTime().at(i - 1) = results1;
                    csvData.getMTime().at(i) = temp;

                }
                i = csvData.getMTime().size(); // break from this for loop
            }
        }
    }
    results1.clear();
    if (csvData.checkFlag("leastTime")) {
        stringstream Y(insertInputs.at(0));
        while (getline(Y, column, ':')) { //splits hour/min into columns 
            results1.push_back(column);
        }
        column = results1.at(0);
        for (i = 0; i < csvData.getLTime().size(); i++) {
            results1 = csvData.getLTime().at(i); // vector for LTime has format time,count
            if (results1.at(0) == column) { //found matching time 
                num = std::stoi(results1.at(1)); // converts string to int
                num = num + 1;
                results1.at(1) = std::to_string(num); //converts int to string and stores value
                csvData.getLTime().at(i) = results1; //update element
                if (csvData.getLTime().at(csvData.getLTime().size() - 1) == results1) {
                    //do nothing: we incremented the last value
                }
                else if (std::stoi(csvData.getLTime().at(i + 1).at(1)) < num) {//lower element is smaller: Swap
                    temp = csvData.getLTime().at(i + 1);
                    csvData.getLTime().at(i + 1) = results1;
                    csvData.getLTime().at(i) = temp;

                }
                i = csvData.getLTime().size(); // break from this for loop
            }
        }
    }
    results1.clear();
    if (csvData.checkFlag("mostLoc")) {
        stringstream Z(insertInputs.at(2)); //lat

        while (getline(Z, column, '.')) { //splits line into columns 
            if (column.size() == 1 && index == 1) {//we are in decimal number
                column = column + "0";
            }
            results1.push_back(column);
            //cout << column << endl;
            index++;
        }
        if (results1.at(1).size() > 2) {
            in1 = results1.at(1);
            results1.at(1) = in1.substr(0, 2);
        }
        if (results1.size() == 1) { //takes care of whole values
            results1.push_back("00");
        }

        ltd = results1.at(0) + "." + results1.at(1) + "  "; //XX.XX or X.XX
        results1.clear();
        stringstream T(insertInputs.at(3)); //longitude
        index = 0;
        while (getline(T, column, '.')) { //splits line into columns 
            if (column.size() == 1 && index == 1) {//we are in decimal number
                column = column + "0";
            }
            results1.push_back(column);
            //cout << column << endl;
            index++;
        }
        if (results1.at(1).size() > 2) {
            in1 = results1.at(1);
            results1.at(1) = in1.substr(0, 2);
        }
        if (results1.size() == 1) { //takes care of whole values
            results1.push_back("00");
        }

        lgt = results1.at(0) + "." + results1.at(1) + "  "; // -Y.YY or -YY.YY
        results1.clear();
        for (int j = 0; j < csvData.getMLoc().size();j++) {
            if (ltd == csvData.getMLoc().at(j).at(0) && lgt == csvData.getMLoc().at(j).at(1)) {
                num = std::stoi(csvData.getMLoc().at(j).at(2));
                num = num + 1;
                csvData.getMLoc().at(j).at(2) = std::to_string(num); //update new count if in results
                if (csvData.getMLoc().at(0).at(0) == ltd && csvData.getMLoc().at(0).at(1) == lgt) {
                    //do nothing we incremented highest count
                }
                else if (num > std::stoi(csvData.getMLoc().at(j - 1).at(2))) { // current num is greater than upper num: swap
                    results1.push_back(csvData.getMLoc().at(j).at(0));
                    results1.push_back(csvData.getMLoc().at(j).at(1));
                    results1.push_back(csvData.getMLoc().at(j).at(2));
                    temp = csvData.getMLoc().at(j - 1);
                    csvData.getMLoc().at(j - 1) = results1;
                    csvData.getMLoc().at(j) = temp;
                }
                j = csvData.getMLoc().size(); // break from this for loop
            }

        }

    }
    index = 0;
    results1.clear();
    if (csvData.checkFlag("leastLoc")) {
        stringstream L(insertInputs.at(2)); //lat

        while (getline(L, column, '.')) { //splits line into columns 
            if (column.size() == 1 && index == 1) {//we are in decimal number
                column = column + "0";
            }
            results1.push_back(column);
            //cout << column << endl;
            index++;
        }
        if (results1.at(1).size() > 2) {
            in1 = results1.at(1);
            results1.at(1) = in1.substr(0, 2);
        }
        if (results1.size() == 1) { //takes care of whole values
            results1.push_back("00");
        }

        ltd = results1.at(0) + "." + results1.at(1) + "  "; //-YY.YY or -Y.YY
        results1.clear();
        stringstream M(insertInputs.at(3)); //longitude
        index = 0;
        while (getline(M, column, '.')) { //splits line into columns 
            if (column.size() == 1 && index == 1) {//we are in decimal number
                column = column + "0";
            }
            results1.push_back(column);
            //cout << column << endl;
            index++;
        }
        if (results1.at(1).size() > 2) {
            in1 = results1.at(1);
            results1.at(1) = in1.substr(0, 2);
        }
        if (results1.size() == 1) { //takes care of whole values
            results1.push_back("00");
        }

        lgt = results1.at(0) + "." + results1.at(1) + "  "; // -Y.YY or -YY.YY
        results1.clear();
        for (int j = 0; j < csvData.getLLoc().size();j++) {
            if (ltd == csvData.getLLoc().at(j).at(0) && lgt == csvData.getLLoc().at(j).at(1)) {
                num = std::stoi(csvData.getLLoc().at(j).at(2));
                num = num + 1;
                csvData.getLLoc().at(j).at(2) = std::to_string(num); //update new count if in results
                if (csvData.getLLoc().at(csvData.getLLoc().size() - 1).at(0) == ltd && csvData.getLLoc().at(csvData.getLLoc().size() - 1).at(1) == lgt) {
                    //do nothing we incremented last count
                }
                else if (num > std::stoi(csvData.getLLoc().at(j + 1).at(2))) { // current num is greater than lower num: swap
                    results1.push_back(csvData.getLLoc().at(j).at(0));
                    results1.push_back(csvData.getLLoc().at(j).at(1));
                    results1.push_back(csvData.getLLoc().at(j).at(2));
                    temp = csvData.getLLoc().at(j + 1);
                    csvData.getLLoc().at(j + 1) = results1;
                    csvData.getLLoc().at(j) = temp;
                }
                j = csvData.getLLoc().size(); // break from this for loop // moved this
            }

        }
    }
    results1.clear();
    index = 0;
    if (csvData.checkFlag("busiestDay")) {
        //cout << "We are inside the busiest day incremental analysis" << endl;
        string day;
        /*for (int k = 0; k < insertInputs.size(); k++) {
            cout << insertInputs.at(k) << endl;
        }
        cout << endl;*/
        stringstream B(insertInputs.at(1));
        while (getline(B, column, '/')) { //splits date into month,day and year
            results1.push_back(column);
        }
        vector <int> monthDays = { 31,28,31,30,31,30,31,31,30,31,30,31, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
        //365 total days first day starts Wednesday last day Wednesday
        int month = std::stoi(results1.at(0)); // 1-12
        int day1 = std::stoi(results1.at(1)); //1-28,30,31
        int year1 = std::stoi(results1.at(2)); //2014 or 2015
        int totDays = 0;
        string dayOfWeek = "";

        if (year1 == 2015) {
            month = month + 12;
        }
        for (int i = 0; i < month; i++) {//calculates range of days out of 365
            if (i == month - 1)
            {
                totDays = day1 + totDays;
            }
            else
            {
                totDays = monthDays.at(i) + totDays;
            }
        }

        if (totDays % 7 == 1) {
            day = "Wednesday";
        }
        else if (totDays % 7 == 2) {
            day = "Thursday";
        }
        else if (totDays % 7 == 3) {
            day = "Friday";
        }
        else if (totDays % 7 == 4) {
            day = "Saturday";
        }
        else if (totDays % 7 == 5) {
            day = "Sunday";
        }
        else if (totDays % 7 == 6) {
            day = "Monday";
        }
        else if (totDays % 7 == 0) {
            day = "Tuesday";
        }
        for (i = 0;i < csvData.getBDay().size();i++) {
            //cout << "Looking at: " << csvData.getBDay().at(i).at(0) << endl;
            if (csvData.getBDay().at(i).at(0).find(day) != string::npos) { // we found the day
                //cout << "We found day in stored results" << endl;
                num = std::stoi(csvData.getBDay().at(i).at(1));
                num = num + 1;
                //cout << "New count: " << num << endl;
                csvData.getBDay().at(i).at(1) = std::to_string(num);
                if (csvData.getBDay().at(i).at(0).find(day) != string::npos) { //we incremented the largest, do nothing
                    //nothing
                    //cout << "incremented largest: DO NOTHING" << endl;
                    i = csvData.getBDay().size(); // break from this for loop
                }
                else if (std::stoi(csvData.getBDay().at(i + 1).at(1)) > num) { //lower count is bigger: swap
                    //cout << "Lower count is bigger: perform swap" << endl;
                    results1.push_back(csvData.getBDay().at(i).at(0));
                    results1.push_back(csvData.getBDay().at(i).at(1));

                    temp = csvData.getBDay().at(i + 1);
                    csvData.getBDay().at(i + 1) = results1;
                    csvData.getBDay().at(i) = temp;
                    i = csvData.getBDay().size(); // break from this for loop
                }
            }

        } //cout << "We are out of the for loop" << endl;
    }
    //cout << "Reached end of flagCheck1" << endl;
    return;
};

void Storage::flagChecks2(Storage& csvData, vector<string>& insertInputs) { //insertDeletes format: time,date,lat,long,base#
    //vector<vector<string>> newResults;
    int num;
    vector<string> temp;
    vector<string> results1;
    string column;
    int i = 0;
    string in1;
    //string tempStr1;
    int index = 0;
    string ltd;
    string lgt;

    if (csvData.checkFlag("mostTime")) {
        stringstream X(insertInputs.at(0));
        while (getline(X, column, ':')) { //splits hour/min into columns 
            results1.push_back(column);
        }
        column = results1.at(0);
        for (i = 0; i < csvData.getMTime().size(); i++) {
            results1 = csvData.getMTime().at(i); // vector for MTime has format time,count
            if (results1.at(0) == column) { //found matching time 
                num = std::stoi(results1.at(1)); // converts string to int
                num = num - 1;
                results1.at(1) = std::to_string(num); //converts int to string and stores value
                csvData.getMTime().at(i) = results1; //update element
                if (csvData.getMTime().at(csvData.getMTime().size() - 1) == results1) {
                    //do nothing: we deccremented the smallest value
                }
                else if (std::stoi(csvData.getMTime().at(i + 1).at(1)) > num) {//lower element is smaller: Swap
                    temp = csvData.getMTime().at(i + 1);
                    csvData.getMTime().at(i + 1) = results1;
                    csvData.getMTime().at(i) = temp;

                }
                i = csvData.getMTime().size(); // break from this for loop
            }
        }
    }
    results1.clear();
    if (csvData.checkFlag("leastTime")) {
        stringstream Y(insertInputs.at(0));
        while (getline(Y, column, ':')) { //splits hour/min into columns 
            results1.push_back(column);
        }
        column = results1.at(0);
        for (i = 0; i < csvData.getLTime().size(); i++) {
            results1 = csvData.getLTime().at(i); // vector for LTime has format time,count
            if (results1.at(0) == column) { //found matching time 
                num = std::stoi(results1.at(1)); // converts string to int
                num = num - 1;
                results1.at(1) = std::to_string(num); //converts int to string and stores value
                csvData.getLTime().at(i) = results1; //update element
                if (csvData.getLTime().at(0) == results1) {
                    //do nothing: we decremented the top value
                }
                else if (std::stoi(csvData.getLTime().at(i - 1).at(1)) < num) {//current element is smaller than upper: Swap
                    temp = csvData.getLTime().at(i - 1);
                    csvData.getLTime().at(i - 1) = results1;
                    csvData.getLTime().at(i) = temp;

                }
                i = csvData.getLTime().size(); // break from this for loop
            }
        }
    }
    results1.clear();
    if (csvData.checkFlag("mostLoc")) {
        stringstream Z(insertInputs.at(2)); //lat

        while (getline(Z, column, '.')) { //splits line into columns 
            if (column.size() == 1 && index == 1) {//we are in decimal number
                column = column + "0";
            }
            results1.push_back(column);
            //cout << column << endl;
            index++;
        }
        if (results1.at(1).size() > 2) {
            in1 = results1.at(1);
            results1.at(1) = in1.substr(0, 2);
        }
        if (results1.size() == 1) { //takes care of whole values
            results1.push_back("00");
        }

        ltd = results1.at(0) + "." + results1.at(1) + "  "; //XX.XX or X.XX
        results1.clear();
        stringstream T(insertInputs.at(3)); //longitude
        index = 0;
        while (getline(T, column, '.')) { //splits line into columns 
            if (column.size() == 1 && index == 1) {//we are in decimal number
                column = column + "0";
            }
            results1.push_back(column);
            //cout << column << endl;
            index++;
        }
        if (results1.at(1).size() > 2) {
            in1 = results1.at(1);
            results1.at(1) = in1.substr(0, 2);
        }
        if (results1.size() == 1) { //takes care of whole values
            results1.push_back("00");
        }

        lgt = results1.at(0) + "." + results1.at(1) + "  "; // -Y.YY or -YY.YY
        results1.clear();
        for (int j = 0; j < csvData.getMLoc().size();j++) {
            if (ltd == csvData.getMLoc().at(j).at(0) && lgt == csvData.getMLoc().at(j).at(1)) {
                num = std::stoi(csvData.getMLoc().at(j).at(2));
                num = num - 1;
                csvData.getMLoc().at(j).at(2) = std::to_string(num); //update new count if in results
                if (csvData.getMLoc().at(0).at(0) == ltd && csvData.getMLoc().at(0).at(1) == lgt) {
                    //do nothing we deccremented last
                }
                else if (num < std::stoi(csvData.getMLoc().at(j + 1).at(2))) { // current num is lower than lower num: swap
                    results1.push_back(csvData.getMLoc().at(j).at(0));
                    results1.push_back(csvData.getMLoc().at(j).at(1));
                    results1.push_back(csvData.getMLoc().at(j).at(2));
                    temp = csvData.getMLoc().at(j + 1);
                    csvData.getMLoc().at(j + 1) = results1;
                    csvData.getMLoc().at(j) = temp;
                }
                j = csvData.getMLoc().size(); // break from this for loop // moved this
            }

        }

    }
    index = 0;
    results1.clear();
    if (csvData.checkFlag("leastLoc")) {
        stringstream L(insertInputs.at(2)); //lat

        while (getline(L, column, '.')) { //splits line into columns 
            if (column.size() == 1 && index == 1) {//we are in decimal number
                column = column + "0";
            }
            results1.push_back(column);
            //cout << column << endl;
            index++;
        }
        if (results1.at(1).size() > 2) {
            in1 = results1.at(1);
            results1.at(1) = in1.substr(0, 2);
        }
        if (results1.size() == 1) { //takes care of whole values
            results1.push_back("00");
        }

        ltd = results1.at(0) + "." + results1.at(1) + "  "; //-YY.YY or -Y.YY
        results1.clear();
        stringstream M(insertInputs.at(3)); //longitude
        index = 0;
        while (getline(M, column, '.')) { //splits line into columns 
            if (column.size() == 1 && index == 1) {//we are in decimal number
                column = column + "0";
            }
            results1.push_back(column);
            //cout << column << endl;
            index++;
        }
        if (results1.at(1).size() > 2) {
            in1 = results1.at(1);
            results1.at(1) = in1.substr(0, 2);
        }
        if (results1.size() == 1) { //takes care of whole values
            results1.push_back("00");
        }

        lgt = results1.at(0) + "." + results1.at(1) + "  "; // -Y.YY or -YY.YY
        results1.clear();
        for (int j = 0; j < csvData.getLLoc().size();j++) {
            if (ltd == csvData.getLLoc().at(j).at(0) && lgt == csvData.getLLoc().at(j).at(1)) {
                num = std::stoi(csvData.getLLoc().at(j).at(2));
                num = num - 1;
                csvData.getLLoc().at(j).at(2) = std::to_string(num); //update new count if in results
                if (csvData.getLLoc().at(0).at(0) == ltd && csvData.getLLoc().at(0).at(1) == lgt) {
                    //do nothing we decremented top count
                }
                else if (num < std::stoi(csvData.getLLoc().at(j - 1).at(2))) { // current num is smaller than upper num: swap
                    results1.push_back(csvData.getLLoc().at(j).at(0));
                    results1.push_back(csvData.getLLoc().at(j).at(1));
                    results1.push_back(csvData.getLLoc().at(j).at(2));
                    temp = csvData.getLLoc().at(j - 1);
                    csvData.getLLoc().at(j - 1) = results1;
                    csvData.getLLoc().at(j) = temp;
                }
                j = csvData.getLLoc().size(); // break from this for loop
            }

        }
    }
    results1.clear();
    index = 0;
    if (csvData.checkFlag("busiestDay")) {
        string day;
        stringstream B(insertInputs.at(1));
        while (getline(B, column, '/')) { //splits hour/min into columns 
            results1.push_back(column);
        }
        vector <int> monthDays = { 31,28,31,30,31,30,31,31,30,31,30,31, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
        //}; //365 total days first day starts Wednesday last day Wednesday
        int month = std::stoi(results1.at(0)); // 1-12
        int day1 = std::stoi(results1.at(1)); //1-28,30,31
        int year1 = std::stoi(results1.at(2)); //2014 or 2015
        int totDays = 0;
        string dayOfWeek = "";
        results1.clear();
        if (year1 == 2015) {
            month = month + 12;
        }
        for (int i = 0; i < month; i++) {//calculates range of days out of 365
            if (i == month - 1)
            {
                totDays = day1 + totDays;
            }
            else
            {
                totDays = monthDays.at(i) + totDays;
            }
        }

        if (totDays % 7 == 1) {
            day = "Wednesday";
        }
        else if (totDays % 7 == 2) {
            day = "Thursday";
        }
        else if (totDays % 7 == 3) {
            day = "Friday";
        }
        else if (totDays % 7 == 4) {
            day = "Saturday";
        }
        else if (totDays % 7 == 5) {
            day = "Sunday";
        }
        else if (totDays % 7 == 6) {
            day = "Monday";
        }
        else if (totDays % 7 == 0) {
            day = "Tuesday";
        }
        for (i = 0;i < csvData.getBDay().size();i++) {
            if (csvData.getBDay().at(i).at(0).find(day) != string::npos) { // we found the day
                //cout << "We found the day: " << csvData.getBDay().at(i).at(0) << endl;
                num = std::stoi(csvData.getBDay().at(i).at(1));
                num = num - 1;
                //cout << "New count: " << num << endl;
                csvData.getBDay().at(i).at(1) = std::to_string(num);
                if (csvData.getBDay().at(6).at(0).find(day) != string::npos) { //we decremented the smallest, do nothing
                    //cout << "Decremented smallest count: DO NOTHING" << endl;
                    //nothing
                }
                else if (std::stoi(csvData.getBDay().at(i + 1).at(1)) > num) { //: swap
                    //cout << "Lower index count is bigger: swap" << endl;
                    results1.push_back(csvData.getBDay().at(i).at(0));
                    results1.push_back(csvData.getBDay().at(i).at(1));

                    temp = csvData.getBDay().at(i + 1);
                    csvData.getBDay().at(i + 1) = results1;
                    csvData.getBDay().at(i) = temp;
                }
                i = csvData.getBDay().size(); // break from this for loop
            }

        }
    }
    //cout << "Reached end of flagCheck2" << endl;
    return;
};


bool Storage::checkCount() {
    if (count >= 10000) {
        return true;
    }
    else {
        return false;
    }
}

void Storage::setFlag(string arg) { //sets flags for incrementality functionality 
    if (arg == "mostTime") {
        //cout << "Most Time flag set" << endl;
        mTimeFlag = 1;
        return;
    }
    else if (arg == "leastTime") {
        //cout << "Least Time flag set" << endl;
        lTimeFlag = 1;
        return;
    }
    else if (arg == "busiestDay") {
        //cout << "Busiest day flag set" << endl;
        bDayFlag = 1;
        return;
    }
    else if (arg == "mostLoc") {
        //cout << "Most Loc flag set" << endl;
        mLocFlag = 1;
        return;
    }
    else if (arg == "leastLoc") {
        //cout << "Least Loc flag set" << endl;
        lLocFlag = 1;
        return;
    }
    return;
};

bool Storage::checkFlag(string arg) { // checks whether passed in flag is raised
    string mt = "mostTime";
    string lt = "leastTime";
    string ml = "mostLoc";
    string ll = "leastLoc";
    string bd = "busiestDay";

    if (arg.find(mt) != string::npos) {
        if (mTimeFlag == 1) {
            //cout << "Most Time flag true" << endl;
            return true;
        }
        else {
            //cout << "Most Time flag was false" << endl;
            return false;
        }
    }
    else if (arg.find(lt) != string::npos) {
        if (lTimeFlag == 1) {
            //cout << "Least Time flag true" << endl;
            return true;
        }
        else {
            //cout << "Least Time flag was false" << endl;
            return false;
        }
    }
    else if (arg.find(bd) != string::npos) {
        //cout << "Compare: " << bDayFlag << " == " << "1" << endl;
        if (bDayFlag == 1) {
            //cout << "Busiest Day flag true" << endl;
            return true;
        }
        else {
            //cout << "Busiest day flag was false" << endl;
            return false;
        }
    }
    else if (arg.find(ml) != string::npos) {
        if (mLocFlag == 1) {
            //cout << "Most Loc flag true" << endl;
            return true;
        }
        else {
            //cout << "Most Loc flag was false" << endl;
            return false;
        }
    }
    else if (arg.find(ll) != string::npos) {
        if (lLocFlag == 1) {
            //cout << "Least Time flag true" << endl;
            return true;
        }
        else {
            //cout << "Least Loc flag was false" << endl;
            return false;
        }
    }
    else {
        //cout << "Every flag was false" << endl;
        return false;
    }

};

void Storage::incCount() {
    count = count + 1;
    return;
}; //increments count

void Storage::resetCount() {
    count = 0;
    mostTime.clear();
    leastTime.clear();
    mostLoc.clear();
    leastLoc.clear();
    busiestDay.clear();
    return;
}; // resets count and vectors

vector <vector<string>>& Storage::getMTime() {
    return mostTime;
};
vector <vector<string>>& Storage::getLTime() {
    return leastTime;
};
vector <vector<string>>& Storage::getMLoc() {
    return mostLoc;
};
vector <vector<string>>& Storage::getLLoc() {
    return leastLoc;
};
vector <vector<string>>& Storage::getBDay() {
    return busiestDay;
};
