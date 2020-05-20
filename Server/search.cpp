#include <iostream>
#include<fstream>
#include <sstream>
#include <string>
#include <vector>
#include "search.h"
using namespace std;

void parseLine(string line, vector<string>& results) {// parses each row from the csv file
    stringstream X(line);
    string column;
    vector<string> results1;

    while (getline(X, column, ',')) { //splits line into columns 
        results1.push_back(column);
    }

    results = results1;
    return;
}

void parseLine1(string line, vector<string>& results) {// parses date and time
    stringstream X(line);
    string column;
    vector<string> results1;

    while (getline(X, column, ' ')) { //splits line into columns 
        results1.push_back(column);
    }

    results = results1;
    return;
}

void read(Storage& data1, string filename)
{
    ifstream fin;
    string line1;
    string line2;

    // Open an existing file
    fin.open(filename);
    int index = 0;
    //Storage data1;
    //vector<string> results;
    cout << "we are about to open file to parse" << endl;
    Parsed* obj;
    Use* row1;
    while (!fin.eof()) {
        fin >> line1;   //These two are to format the first column of the csv file
        fin >> line2;
        line1 = line1 + " " + line2; // combines the date column string with the rest of its corresponding row "4/1/2014 + 0:11:00",40.769,-73.9549,"B02512"
        obj = new Parsed;
        obj->parseData(line1); // this will parse all the fields
        obj->parseHours(obj->retTime());
        obj->parseMonth(obj->retDate());
        obj->convertToDay();
        row1 = new Use;

        row1->setTime(obj->retTime());
        row1->setDate(obj->retDate());
        row1->setLong(obj->retLongt());
        row1->setLat(obj->retLat());
        row1->setBase(obj->retBase());
        row1->setDay(obj->retDay());
        row1->setHour(obj->retHour());
        row1->setMin(obj->retMin());
        row1->setExists(true);
        //cout << row1->getDate() << endl;
        //row1.setDay(obj.retDay())
        data1.insertRow(*row1);    //Use row1 object is created, and added to the Storage data1 object

        //parseLine(line1, results, index); // parses line1 to results vector with four elements:  "Date Time", Lat, Lon, "Base",
        //result.push_back(results); //pushes results vector to result vector
        if (index % 10000 == 0) {
            //cout << "just pushed " << index << endl;
            cout << index << endl;
        }

        index++;
    }
    return;
};

void readFOIL(Storage& data1, string filename, vector<vector <string>>& foilStore)
{
    ifstream fin;
    string line1;
    string line2;

    // Open an existing file
    fin.open(filename);
    int index = 0;
    //Storage data1;
    //vector<string> results;
    cout << "we are about to open file to parse" << endl;
    Parsed* obj;
    // Use* row1;
    while (!fin.eof()) {
        fin >> line1;   //These two are to format the first column of the csv file

        vector<string> results;
        parseLine(line1, results);


        if (index % 10000 == 0) {
            //cout << "just pushed " << index << endl;
            cout << index << endl;
        }

        foilStore.push_back(results);
        index++;
    }
    return;
};

//class Use

void Use::setTime(string arg) {
    time = arg;
}; //sets time

void Use::setDate(string arg) {
    date = arg;
}; //sets date

void Use::setDay(string arg) {
    day = arg;
}; //sets day: Monday-Sunday

void Use::setLong(string arg) {
    longitude = arg;
}; //sets Longitude

void Use::setLat(string arg) {
    latitude = arg;
}; //sets Latitude

void Use::setBase(string arg) {
    base = arg;
}; //sets base

void Use::setHour(int arg) {
    timeHour = arg;
}; //sets hour

void Use::setMin(int arg) {
    timeMin = arg;
}; //set min

void Use::setExists(bool arg) {
    exists = arg;
}; //sets exists bool variable for an object: either true or false

string Use::getTime() {
    return time;
}; //expecting XX:XX:XX

string Use::getDate() {
    return date;
};

string Use::getDay() {
    return day;
};

string Use::getLong() {
    return longitude;
};

string Use::getLat() {
    return latitude;
};

string Use::getBase() {
    return base;
}

int Use::getHour() {
    return timeHour;
};
int Use::getMin() {
    return timeMin;
};

bool Use::checkExists() {
    return exists;
}; //checks if data exists- used for updating new storage after deleting

void Use::clearUse() {
    time = "";
    date = "";
    day = "";
    longitude = "";
    latitude = "";
    exists = false;
}; // will clear the fields for a Use object - technically deletes

//class Storage

void Storage::insertRow(Use& row) { //not sure if passby referrence FIX?
    data[size] = row;
    size++;
    size2++;
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
    cout << "About to call flagChecks2" << endl;
    flagChecks2(*this,inputs); // handles incremental analytics after a delete
    cout << "Back from flagChecks2" << endl;
    data[index].clearUse();
    size--;
    incCount();
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
        while (!getRow(i).checkExists()) {
            adjustData(i); //switch element from the back to deleted index
            if (i == size2 - 1) {
                return;
            }
        }
    }
};

void Storage::adjustData(int index) {
    data[index] = data[size2 - 1];
    getRow(index).clearUse();
    size2--;
}; // moves element from end to index passed in: for updateDelData

vector <vector<string>>& Storage::getResults() {
    return results;
}; // gets vector with results

//class Parsed

void Parsed::parseData(string arg) {
    vector<string> results;
    string temp;
    string temp1;

    parseLine(arg, results); //parses arg int vector with elements: "Date Time", Lat, Lon, "Base",
    lat = results.at(1); //sets object variables
    longt = results.at(2);
    base = results.at(3);

    temp = results.at(0);
    results.clear();
    parseLine1(temp, results); //parses date and time by space => "Date and Time" >>still has quotes
    temp = results.at(0); //Date
    temp = temp.substr(1, temp.length());
    temp1 = results.at(1); // Time
    temp1 = temp1.substr(0, temp1.length() - 1);

    date = temp;
    time = temp1;
    results.clear();

};

void Parsed::parseMonth(string arg) {
    stringstream X(arg);
    string column;
    vector<string> results1;

    while (getline(X, column, '/')) { //splits line into columns 
        results1.push_back(column);
    }
    // parse XX/XX/XXXX set mth1/d1/yr1
    mth1 = results1.at(0);
    d1 = results1.at(1);
    yr1 = results1.at(2);
};

void Parsed::parseHours(string arg) {
    stringstream X(arg);
    string column;
    vector<string> results1;

    while (getline(X, column, ':')) { //splits line into columns 
        results1.push_back(column);
    } //index 0 in results is hour, index 1 is min and index 2 is seconds
    // parse 00:00:00 set hr1/min1/sec1
    hour1 = results1.at(0);
    min1 = results1.at(1);
    //sec1 = results1.at(2);
};

string Parsed::retDate() {
    return date;
};

string Parsed::retTime() {
    return time;
};

string Parsed::retLongt() {
    return longt;
};

string Parsed::retLat() {
    return lat;
};

string Parsed::retBase() {
    return base;
};

string Parsed::retDay() {
    return day;
};

int Parsed::retHour() {
    return std::stoi(hour1);
};
int Parsed::retMin() {
    return std::stoi(min1);
};

void Parsed::convertToDay() {
    vector <int> monthDays = { 31,28,31,30,31,30,31,31,30,31,30,31, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
 //365 total days first day starts Wednesday last day Wednesday
    int month = std::stoi(mth1); // 1-12
    int day1 = std::stoi(d1); //1-28,30,31
    int year1 = std::stoi(yr1); //2014 or 2015
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

}; //maps the date XX/XX/XXXXX to appropriate day: figure out algorithm with days of each month for 2014,2015



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
};

void searchDateTime(vector<vector<string>>& results1, Storage& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;
    Storage* obj = &csvData;
    string in1;
    //string dateAndTime = "\"" + searchInputs.at(1) + " " + searchInputs.at(0) + ":00" + "\"";
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
};

void deleteTime(vector<vector<string>>& results, Storage& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;
    Storage* obj = &csvData;

    searchInputs.at(0) = " " + searchInputs.at(0) + ":00";
    for (unsigned int k = 0; k < obj->getOrigSize(); k++) { // searches for time 00:00
        Use* row1 = new Use;
        *row1 = obj->getRow(k);
        if ((row1->getTime().find(searchInputs.at(0)) != string::npos)) { //the time is found so we delete this element
            //results.push_back(miniVec); ////pushes elements not satisfying the search into results vector
            obj->deleteRow(k);
        }
        delete row1;
    }
    obj->updateDelData();

};

void deleteDate(vector<vector<string>>& results, Storage& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;
    Storage* obj = &csvData;

    for (unsigned int k = 0; k < obj->getOrigSize(); k++) { // searches for date XX/XX/XXXX 
        Use* row1 = new Use;
        *row1 = obj->getRow(k);
        if (row1->getDate().find(searchInputs.at(0)) != string::npos) { //if search element is found it is deleted
            obj->deleteRow(k);
        }
        delete row1;
    }
    obj->updateDelData();
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
        if (dateAndTime.compare(temp) == 0) { //delete is equal
            obj->deleteRow(k);
        }
        delete row1;
    }
    obj->updateDelData();
};

void deleteLocation(vector<vector<string>>& results, Storage& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;
    Storage* obj = &csvData;

    for (unsigned int k = 0; k < obj->getOrigSize(); k++) { // searches for Location: lon/lat 
        Use* row1 = new Use;
        *row1 = obj->getRow(k);
        if ((searchInputs.at(0).compare(row1->getLat()) == 0 && searchInputs.at(1).compare(row1->getLong()) == 0)) {
            obj->deleteRow(k);
        }
        delete row1;
    }
    obj->updateDelData();
};

void deleteBase(vector<vector<string>>& results, Storage& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;
    searchInputs.at(0) = "\"" + searchInputs.at(0) + "\"";
    Storage* obj = &csvData;

    for (unsigned int k = 0; k < obj->getOrigSize(); k++) {// searches for Base#: BXXXXX 
        Use* row1 = new Use;
        *row1 = obj->getRow(k);
        if ((searchInputs.at(0).compare(row1->getBase()) == 0)) {
            obj->deleteRow(k);
        }
        delete row1;
    }
    obj->updateDelData();
};

void deleteSpecific(vector<vector<string>>& results, Storage& csvData, vector<string>& searchInputs) {
    //csvData.flagChecks2(csvData, searchInputs); // this will handle checking flags and updating any results already calculated.
    //csvData.incCount();
    //if (csvData.checkCount()) {//more than 1000 inputs/deletes. recalculate data
    //    csvData.resetCount();
    //}
    vector<string> miniVec;
    string temp;
    searchInputs.at(4) = "\"" + searchInputs.at(4) + "\"";
    string dateAndTime = "\"" + searchInputs.at(1) + " " + searchInputs.at(0) + ":00" + "\"";
    Storage* obj = &csvData;

    for (unsigned int k = 0; k < obj->getOrigSize(); k++) {         // searches for Location: lon/lat 
        Use* row1 = new Use;
        *row1 = obj->getRow(k);
        temp = "\"" + row1->getDate() + " " + row1->getTime() + "\"";
        if ((dateAndTime.compare(temp) == 0 && searchInputs.at(2).compare(row1->getLat()) == 0 && searchInputs.at(3).compare(row1->getLong()) == 0 && searchInputs.at(4).compare(row1->getBase()) == 0)) {
            obj->deleteRow(k);
        }
        delete row1;
    }
    obj->updateDelData();
    if (obj->checkCount()) {
        obj->resetCount();
    }
};

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
                if (csvData.getLTime().at(csvData.getLTime().size() -1) == results1) {
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
            }
            j = csvData.getMLoc().size(); // break from this for loop
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
            }
            j = csvData.getLLoc().size(); // break from this for loop
        }
    }
    results1.clear();
    index = 0;
    if (csvData.checkFlag("busiestDay")) {
        cout << "We are inside the busiest day incremental analysis" << endl;
        string day;
        for (int k = 0; k < insertInputs.size(); k++) {
            cout << insertInputs.at(k) << endl;
        }
        cout << endl;
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
            cout << "Looking at: " << csvData.getBDay().at(i).at(0) << endl;
            if (csvData.getBDay().at(i).at(0).find(day) != string::npos) { // we found the day
                cout << "We found day in stored results" << endl;
                num = std::stoi(csvData.getBDay().at(i).at(1));
                num = num + 1;
                cout << "New count: " << num << endl;
                csvData.getBDay().at(i).at(1) = std::to_string(num);
                if (csvData.getBDay().at(i).at(0).find(day) != string::npos) { //we incremented the largest, do nothing
                    //nothing
                    cout << "incremented largest: DO NOTHING" << endl;
                }
                else if (std::stoi(csvData.getBDay().at(i + 1).at(1)) > num) { //lower count is bigger: swap
                    cout << "Lower count is bigger: perform swap" << endl;
                    results1.push_back(csvData.getBDay().at(i).at(0));
                    results1.push_back(csvData.getBDay().at(i).at(1));

                    temp = csvData.getBDay().at(i + 1);
                    csvData.getBDay().at(i + 1) = results1;
                    csvData.getBDay().at(i) = temp;
                }
            }
            i = csvData.getBDay().size(); // break from this for loop
        } cout << "We are out of the for loop" << endl;
    }


};

void Storage::flagChecks2(Storage& csvData, vector<string>& insertInputs) { //insertInputs format: time,date,lat,long,base#
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
            }
            j = csvData.getMLoc().size(); // break from this for loop
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
            }
            j = csvData.getLLoc().size(); // break from this for loop
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
                cout << "We found the day: " << csvData.getBDay().at(i).at(0) << endl;
                num = std::stoi(csvData.getBDay().at(i).at(1));
                num = num - 1;
                cout << "New count: " << num << endl;
                csvData.getBDay().at(i).at(1) = std::to_string(num);
                if (csvData.getBDay().at(6).at(0).find(day) != string::npos) { //we decremented the smallest, do nothing
                    cout << "Decremented smallest count: DO NOTHING" << endl;
                    //nothing
                }
                else if (std::stoi(csvData.getBDay().at(i + 1).at(1)) > num) { //: swap
                    cout << "Lower index count is bigger: swap" << endl;
                    results1.push_back(csvData.getBDay().at(i).at(0));
                    results1.push_back(csvData.getBDay().at(i).at(1));

                    temp = csvData.getBDay().at(i + 1);
                    csvData.getBDay().at(i + 1) = results1;
                    csvData.getBDay().at(i) = temp;
                }
            }
            i = csvData.getBDay().size(); // break from this for loop
        }
    }


};

void insertData(vector<vector<string>>& results, Storage& csvData, vector<string>& insertInputs) { // pushes new data to end of csvData vector
    cout << "About to enter flagcheck" << endl;
    csvData.flagChecks1(csvData,insertInputs); // this will handle checking flags and updating any results already calculated.
    cout << "Back from flagcheck" << endl;
    csvData.incCount();
    if (csvData.checkCount()) {//more than 1000 inputs/deletes. recalculate data
        csvData.resetCount();
    }
    insertInputs.at(4) = "\"" + insertInputs.at(4) + "\"";
    string temp;
    temp = insertInputs.at(0) + ":00";
    Use* obj1 = new Use;; //implement set Day Monday -Sunday
    obj1->setTime(temp);
    obj1->setDate(insertInputs.at(1));
    obj1->setLat(insertInputs.at(2));
    obj1->setLong(insertInputs.at(3));
    obj1->setExists(true);
    obj1->setBase(insertInputs.at(4));

    csvData.insertRow(*obj1);
    delete obj1;
    if (csvData.checkCount()) {
        csvData.resetCount();
    }
};

bool checkIfExport(char buf[1024]) {
    string clientMessage(buf);
    if (clientMessage.find("Export") != string::npos) {//checks whether message is for importing
        return true;
    }
    else {
        return false;
    }
}

bool checkIfImport(char buf[1024]) {
    string clientMessage(buf);
    if (clientMessage.find("File") != string::npos) {//checks whether message is for importing
        return true;
    }
    else {
        return false;
    }
}


void importData(char buf[1024], vector<string>& importRows) { // called for each message recieved for import
    string row(buf);
    importRows.push_back(row + "\n");
    return;
}

bool importDone(char buf[1024]) {
    string clientMessage(buf);
    if (clientMessage.find("Done") != string::npos) {//checks whether message is for importing
        return true;
    }
    else {
        return false;
    }
}

void importFunction(vector<string>& importRows) {
    string fileName;
    string compare = "File: ";
    int i = 0;

    string::size_type f = importRows.at(i).find(compare); // this parses the first element to get the file name
    importRows.at(i).erase(f, compare.length());
    fileName = importRows.at(i);

    std::ofstream file("imported-data.csv");
    //file.open(fileName, std::ios_base::app);
    for (i = 1; i < importRows.size(); i++) {
        file << importRows.at(i);
    }
    file.close();

}

//Artifact 5 functions 

void searchMostUseTime(vector<vector<string>>& results1, Storage& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;
    Storage* obj = &csvData;
    string in1;
    int arr[24] = { 0 }; //for each hour of day 
    int count = 0;
    if (obj->checkFlag("mostTime")) {
        for (int var = 0; var < 10; var++) {
            miniVec = obj->getMTime().at(var);
            results1.push_back(miniVec);
            miniVec.clear();
        } // will push back the 10 results in mostTime vector from previous calculation
    }
    else {
        for (unsigned int k = 0; k < obj->getOrigSize(); k++) {//counts 
            Use* row1 = new Use;
            *row1 = obj->getRow(k);
            count = arr[row1->getHour()];
            count = count + 1;
            arr[row1->getHour()] = count;
            delete row1;
        }
        string str1;
        string str2;
        int max;
        int index;
        for (int k = 0; k < 20; k++) {//gets top 20 in order
            max = arr[0];
            index = 0;
            for (int j = 0; j < 24; j++) {
                if (arr[j] > max) {
                    index = j;
                    max = arr[j];
                }
            }//gets largest coubt for particular time
            arr[index] = 0; //set largest to 0 and loop again up to 10 times
            str1 = to_string(max);
            str2 = to_string(index);
            if (str2.size() == 1)
            {
                miniVec.push_back(str2 + "                                               ");
            }
            else if (str2.size() == 2)
            {
                miniVec.push_back(str2 + "                                             ");
            }
            //miniVec.push_back(str2 + "                                             ");//push back the hour
            miniVec.push_back(str1 + " "); //push back the number of occurences for corresponding time - Format of miniVec: 00,15 => hour 00 has 15 occurences
            if (k < 10) {
                results1.push_back(miniVec);
            }
            
            obj->getMTime().push_back(miniVec); // pushes to mostTime Vector to save calculation results for faster future calculations
            miniVec.clear();
        }
        obj->setFlag("mostTime");
    }
    
};

void searchLeastUseTime(vector<vector<string>>& results1, Storage& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;
    Storage* obj = &csvData;
    string in1;
    int arr[24] = { 0 }; //for each hour of day 
    int count = 0;
    if (obj->checkFlag("leastTime")) {
        for (int var = 0; var < 10; var++) {
            miniVec = obj->getLTime().at(var);
            results1.push_back(miniVec);
            miniVec.clear();
        } // will push back the 10 results in leastTime vector from previous calculation
    }
    else {
        for (unsigned int k = 0; k < obj->getOrigSize(); k++) {//counts 
            Use* row1 = new Use;
            *row1 = obj->getRow(k);
            count = arr[row1->getHour()];
            count = count + 1;
            arr[row1->getHour()] = count;
            delete row1;
        }
        string str1;
        string str2;
        int min;
        int index;
        for (int k = 0; k < 20; k++) {//gets top 20 in order
            min = arr[0];
            index = 0;
            for (int j = 0; j < 24; j++) {
                if (arr[j] < min) {
                    index = j;
                    min = arr[j];
                }
            }//gets largest coubt for particular time
            arr[index] = 1000000; //set smallest to 1000000 and loop again up to 10 times
            str1 = to_string(min);
            str2 = to_string(index);
            if (str2.size() == 1)
            {
                miniVec.push_back(str2 + "                                                 ");
            }
            else if (str2.size() == 2)
            {
                miniVec.push_back(str2 + "                                               ");
            }
            //miniVec.push_back(str2 + "                                                 ");//push back the hour
            miniVec.push_back(str1 + " "); //push back the number of occurences for corresponding time - Format of miniVec: 00,15 => hour 00 has 15 occurences
            if (k < 10) {
                results1.push_back(miniVec);
            }
            obj->getLTime().push_back(miniVec);
            miniVec.clear();
        }
        obj->setFlag("leastTime");
    }
};

void searchMostLoc(vector<vector<string>>& results1, Storage& csvData, vector<string>& searchInputs) { //FIXME
    vector<string> miniVec;
    vector<string> temp1;
    Storage* obj = &csvData;
    int temp = 0;
    string in1;
    string in2;
    int arr1[20000] = { 0 };//adding both XXXX and YYYY wouldnt exceed this
    vector<int> eachEntry; // will contain the added long and lat value, lat, long : 3 elements
    vector<vector<int>> allData; //will contain each entry
    int count = 0;
    int lt_val;
    int ln_val;
    int compVal;

    if (obj->checkFlag("mostLoc")) {
        for (int var = 0; var < 10; var++) {
            miniVec = obj->getMLoc().at(var);
            results1.push_back(miniVec);
            miniVec.clear();
        }
    }
    else {
        for (unsigned int k = 0; k < obj->getOrigSize(); k++) {//counts 
            pair<int, vector<int>>pairs;
            Use* row1 = new Use;
            *row1 = obj->getRow(k);
            in1 = row1->getLat(); //XX.XXX
            in2 = row1->getLong();//-YY.YY
            if (k % 1000 == 0) {
                cout << k << endl;
            }
            
            //FIXME make this compatible with longitudes that dont have "-"
            in2 = in2.substr(1, in2.length()); //now YY.YY
            //cout << in2 << endl;
            //parse by "." then check size and if 1 convert to a two digit int
            stringstream X(in1);
            string column;
            vector<string> results1;
            int indx = 0;//index for whole number or decimal

            while (getline(X, column, '.')) { //splits line into columns 
                if (column.size() == 1 && indx == 1) {//we are in decimal number
                    column = column + "0";
                }
                temp1.push_back(column);
                //cout << column << endl;
                indx++;
            }

            if (temp1.size() == 1) { //takes care of whole values
                temp1.push_back("00");
            }

            if (temp1.at(1).size() > 2) {
                in1 = temp1.at(1);
                temp1.at(1) = in1.substr(0, 2);
            }

            in1 = temp1.at(0) + temp1.at(1); //string now XXXX or XXX
            //cout << in1 << endl;
            temp1.clear();
            indx = 0;
            stringstream Y(in2);

            while (getline(Y, column, '.')) { //splits line into columns 
                if (column.size() == 1 && indx == 1) {//we are in decimal number
                    column = column + "0";
                }
                temp1.push_back(column);
                //cout << column << endl;
                indx++;
            }

            if (temp1.size() == 1) { //takes care of whole values
                temp1.push_back("00");
            }

            if (temp1.at(1).size() > 2) {
                in2 = temp1.at(1);
                temp1.at(1) = in2.substr(0, 2);
            }

            in2 = "";
            in2.append(temp1.at(0));
            in2.append(temp1.at(1)); //string now XXXX or XXX

            temp1.clear();

            //in1 and in2 are now XXXX YYYY
            lt_val = std::stoi(in1);
            ln_val = std::stoi(in2);

            compVal = lt_val + ln_val; // add them and increment its appropriate index

            if (arr1[compVal] == 0) {//if it has already been pushed to vector dont repush new entry. just increment count for it
                count = 1;
                arr1[compVal] = count;
                eachEntry.push_back(compVal);
                eachEntry.push_back(lt_val);
                eachEntry.push_back(ln_val);
                allData.push_back(eachEntry);
                eachEntry.clear();
            }
            else { // we already pushed this value into allData vector
                count = arr1[compVal];
                arr1[compVal] = count + 1;
            }

            delete row1;
            temp = compVal;
        }
        string str1;
        string str2;
        string str3;
        int min = arr1[temp];
        int index;

        for (int k = 0; k < 20; k++) {//gets top 20 in order

            index = 0;

            for (int j = 0; j < 20000; j++) {


                if (arr1[j] > 0) {
                    if (arr1[j] > min) {
                        index = j;
                        min = arr1[j];
                    }
                }

            }

            int point = 0;

            for (int f = 0; f < allData.size();f++) {//
                eachEntry = allData.at(f); //compVal long , lat
                //cout << eachEntry.at(0) << " = " << index << endl;
                if (eachEntry.at(0) == index) {
                    //cout << "We found corresponding value " << endl;
                    point = f;
                    f = allData.size();
                }
            }

            str1 = to_string(allData[point][1]);
            str2 = to_string(allData[point][2]);
            str3 = to_string(arr1[index]);
            //str3.append("\n");
            if (str1.size() == 3) { //X.XX
                str1.insert(1, ".");
            }
            else { //XX.XX
                str1.insert(2, ".");
            }
            if (str2.size() == 3) {//Y.YY
                str2.insert(1, ".");
            }
            else {
                str2.insert(2, ".");//YY.YY
            }
            str2.insert(0, "-"); //-YY.YY
            str1.append("  ");
            str2.append("  ");
            arr1[index] = 0; //set smallest to 1000000 and loop again up to 10 times
            miniVec.push_back(str1);//push back the lat
            miniVec.push_back(str2);//push back the lat
            miniVec.push_back(str3);//push back the number of occurences for corresponding location
            //cout << "We push: " << str1 << " " << str2 << " " << str3 << endl;
            if (k < 10) {
                results1.push_back(miniVec);
            }
            
            obj->getMLoc().push_back(miniVec);
            miniVec.clear();
            min = arr1[index];
        }
        obj->setFlag("mostLoc");
    }
    
};

void searchLeastLoc(vector<vector<string>>& results1, Storage& csvData, vector<string>& searchInputs) { //FIXME
    vector<string> miniVec;
    vector<string> temp1;
    Storage* obj = &csvData;
    int temp = 0;
    string in1;
    string in2;
    int arr1[20000] = { 0 };//adding both XXXX and YYYY wouldnt exceed this
    vector<int> eachEntry; // will contain the added long and lat value, lat, long : 3 elements
    vector<vector<int>> allData; //will contain each entry
    int count = 0;
    int lt_val;
    int ln_val;
    int compVal;

    if (obj->checkFlag("leastLoc")) {
        for (int var = 0; var < 10; var++) {
            miniVec = obj->getLLoc().at(var);
            results1.push_back(miniVec);
            miniVec.clear();
        }
    }
    else {
        for (unsigned int k = 0; k < obj->getOrigSize(); k++) {//counts 
            pair<int, vector<int>>pairs;
            Use* row1 = new Use;
            *row1 = obj->getRow(k);
            in1 = row1->getLat(); //XX.XXX
            in2 = row1->getLong();//-YY.YY
            if (k % 10000 == 0) {
                cout << k << endl;
            }
            //FIXME make this compatible with longitudes that dont have "-"
            in2 = in2.substr(1, in2.length()); //now YY.YY
            //cout << in2 << endl;
            //parse by "." then check size and if 1 convert to a two digit int
            stringstream X(in1);
            string column;
            vector<string> results1;
            int indx = 0;//index for whole number or decimal

            while (getline(X, column, '.')) { //splits line into columns 
                if (column.size() == 1 && indx == 1) {//we are in decimal number
                    column = column + "0";
                }
                temp1.push_back(column);
                //cout << column << endl;
                indx++;
            }
            if (temp1.at(1).size() > 2) {
                in1 = temp1.at(1);
                temp1.at(1) = in1.substr(0, 2);
            }
            if (temp1.size() == 1) { //takes care of whole values
                temp1.push_back("00");
            }

            in1 = temp1.at(0) + temp1.at(1); //string now XXXX or XXX
            //cout << in1 << endl;
            temp1.clear();
            indx = 0;
            stringstream Y(in2);

            while (getline(Y, column, '.')) { //splits line into columns 
                if (column.size() == 1 && indx == 1) {//we are in decimal number
                    column = column + "0";
                }
                temp1.push_back(column);
                //cout << column << endl;
                indx++;
            }
            if (temp1.size() == 1) { //takes care of whole values
                temp1.push_back("00");
            }

            if (temp1.at(1).size() > 2) {
                in2 = temp1.at(1);
                temp1.at(1) = in2.substr(0, 2);
            }

            in2 = "";
            in2.append(temp1.at(0));
            in2.append(temp1.at(1)); //string now XXXX or XXX

            temp1.clear();

            //in1 and in2 are now XXXX YYYY
            lt_val = std::stoi(in1);
            ln_val = std::stoi(in2);

            compVal = lt_val + ln_val; // add them and increment its appropriate index

            if (arr1[compVal] == 0) {//if it has already been pushed to vector dont repush new entry. just increment count for it
                count = 1;
                arr1[compVal] = count;
                eachEntry.push_back(compVal);
                eachEntry.push_back(lt_val);
                eachEntry.push_back(ln_val);
                allData.push_back(eachEntry);
                eachEntry.clear();
            }
            else { // we already pushed this value into allData vector
                //count = arr1[compVal];
                arr1[compVal] = count + 1;
            }

            if (allData.size() % 10000 == 0) {
                cout << allData.size() << endl;
            }
            delete row1;
            temp = compVal;
        }
        string str1;
        string str2;
        string str3;
        int min = arr1[temp];
        int index;

        for (int k = 0; k < 20; k++) {//gets top 15 in order

            index = 0;

            for (int j = 0; j < 20000; j++) {


                if (arr1[j] > 0) {
                    if (arr1[j] < min) {
                        index = j;
                        min = arr1[j];
                    }
                }

            }

            int point = 0;

            for (int f = 0; f < allData.size();f++) {//
                eachEntry = allData.at(f); //compVal long , lat
                //cout << eachEntry.at(0) << " = " << index << endl;
                if (eachEntry.at(0) == index) {
                    //cout << "We found corresponding value " << endl;
                    point = f;
                    f = allData.size();
                }
            }

            str1 = to_string(allData[point][1]);
            str2 = to_string(allData[point][2]);
            str3 = to_string(arr1[index]);
            str3.append("\n");
            if (str1.size() == 3) { //X.XX
                str1.insert(1, ".");
            }
            else { //XX.XX
                str1.insert(2, ".");
            }
            if (str2.size() == 3) {//Y.YY
                str2.insert(1, ".");
            }
            else {
                str2.insert(2, ".");//YY.YY
            }
            str2.insert(0, "-"); //-YY.YY
            str1.append("  ");
            str2.append("  ");
            arr1[index] = 1000000; //set smallest to 1000000 and loop again up to 10 times
            miniVec.push_back(str1);//push back the lat
            miniVec.push_back(str2);//push back the lat
            miniVec.push_back(str3);//push back the number of occurences for corresponding location
            cout << "We push: " << str1 << " " << str2 << " " << str3 << endl;
            if (k < 10) {
                results1.push_back(miniVec);
            }
            obj->getLLoc().push_back(miniVec);
            miniVec.clear();
            min = arr1[index];
        }
        obj->setFlag("leastLoc");
    }
};


void calculateBusiestDay(vector<vector<string>>& results1, Storage& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;
    Storage* obj = &csvData;
    string in1;
    int arr[7] = { 0 }; //for each day 
    string arr1[7] = { "Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday" }; // each day of week for comparison check
    int count = 0;
    cout << "Right before check " << endl;
    if (obj->checkFlag("busiestDay")) {
        cout << "flag check was true" << endl;
        for (int var = 0; var < 7; var++) {
            miniVec = obj->getBDay().at(var);
            results1.push_back(miniVec);
            miniVec.clear();
        }
    }
    else {
        cout << "flag check was false" << endl;
        for (unsigned int k = 0; k < obj->getOrigSize(); k++) {//counts 
            Use* row1 = new Use;
            *row1 = obj->getRow(k);

            for (int g = 0; g < 7; g++) {
                if (row1->getDay() == arr1[g]) {
                    count = arr[g] + 1;
                    arr[g] = count; //increment count for day
                }
            }
            delete row1;
        }
        string str1;
        string str2;
        int max;
        int index;
        for (int k = 0; k < 7; k++) {//gets top 7 in order
            max = arr[0];
            index = 0;
            for (int j = 0; j < 7; j++) {
                if (arr[j] > max) {
                    index = j;
                    max = arr[j];
                }
            }//gets largest coubt for particular time
            arr[index] = 0; //set largest to 0 and loop again up to 10 times
            str1 = to_string(max);
            //str2 = to_string(index);
            if (arr1[index].compare("Sunday") == 0)
            {
                miniVec.push_back(arr1[index] + "                                    ");
            }
            else if (arr1[index].compare("Monday") == 0)
            {
                miniVec.push_back(arr1[index] + "                                   ");
            }
            else if (arr1[index].compare("Tuesday") == 0)
            {
                miniVec.push_back(arr1[index] + "                                  ");
            }
            else if (arr1[index].compare("Wednesday") == 0)
            {
                miniVec.push_back(arr1[index] + "                            ");
            }
            else if (arr1[index].compare("Thursday") == 0)
            {
                miniVec.push_back(arr1[index] + "                                 ");
            }
            else if (arr1[index].compare("Friday") == 0)
            {
                miniVec.push_back(arr1[index] + "                                      ");
            }
            else if (arr1[index].compare("Saturday") == 0)
            {
                miniVec.push_back(arr1[index] + "                                  ");
            }
            else
            {
                miniVec.push_back(arr1[index] + "                                ");
            }
            //miniVec.push_back(arr1[index] + "                         ");//push back the day
            miniVec.push_back(str1); //push back the number of occurences for corresponding day - Format of miniVec: "Monday","234" => Monday has 234 occurences
            results1.push_back(miniVec);
            obj->getBDay().push_back(miniVec);
            miniVec.clear();
        }
        obj->setFlag("busiestDay");
    }
};



void searchMostVehicles(vector<vector<string>>& results, vector<vector<string>> foilStore, vector<string>& searchInputs)
{
    // given date find base ID with largest number of vehicles
    string date = searchInputs.at(0);
    date = date.substr(1, date.length());
    cout << date << endl;

    vector<string> current;
    vector<vector<string>> unsorted;
    for (int i = 0; i < foilStore.size(); i++)
    {
        current = foilStore.at(i);
        if (current.at(1) == date)
        {
            cout << current.at(1) << endl;
            unsorted.push_back(current);
        }
    }

    int max = 0;
    vector<string> maxEntry;
    int iterate = unsorted.size();
    int maxPos = 0;
    for (int i = 0; i < iterate; i++)
    {
        max = 0;
        for (int j = 0; j < unsorted.size(); j++)
        {
            current = unsorted.at(j);
            if (stoi(current.at(2)) > max)
            {
                max = stoi(current.at(2));
                maxEntry = current;
                maxPos = j;
            }
        }

        vector<string> resultRow;
        resultRow.push_back(maxEntry.at(0) + "                                                                                  ");
        resultRow.push_back(maxEntry.at(3) + "                                      ");
        results.push_back(resultRow);

        /*for (int k = 0; k < resultRow.size(); k++)
        {
            cout << resultRow.at(k) << " ";
        }
        cout << endl;*/

        unsorted.erase(unsorted.begin() + maxPos);
    }
};

void searchLeastVehicles(vector<vector<string>>& results, vector<vector<string>> foilStore, vector<string>& searchInputs)
{
    // given date find base ID with largest number of vehicles
    string date = searchInputs.at(0);
    date = date.substr(1, date.length());
    cout << date << endl;

    vector<string> current;
    vector<vector<string>> unsorted;
    for (int i = 0; i < foilStore.size(); i++)
    {
        current = foilStore.at(i);
        if (current.at(1) == date)
        {
            cout << current.at(1) << endl;
            unsorted.push_back(current);
        }
    }

    int min = 100000000;
    vector<string> minEntry;
    int iterate = unsorted.size();
    int minPos = 0;
    for (int i = 0; i < iterate; i++)
    {
        min = 100000000;
        for (int j = 0; j < unsorted.size(); j++)
        {
            current = unsorted.at(j);
            if (stoi(current.at(2)) < min)
            {
                min = stoi(current.at(2));
                minEntry = current;
                minPos = j;
            }
        }
        vector<string> resultRow;
        resultRow.push_back(minEntry.at(0) + "                                                                                  ");
        resultRow.push_back(minEntry.at(3) + "                                      ");
        results.push_back(resultRow);

        /*for (int k = 0; k < resultRow.size(); k++)
        {
            cout << resultRow.at(k) << " ";
        }
        cout << endl;*/

        unsorted.erase(unsorted.begin() + minPos);
    }
};

void searchHighestRatio(vector<vector<string>>& results, vector<vector<string>> foilStore, vector<string>& searchInputs)
{
    // given base ID, find date with highest ratio of trips to vehicles
    string base = searchInputs.at(0);
    base = base.substr(1, base.length());
    cout << base << endl;

    vector<string> current;
    vector<vector<string>> unsorted;
    for (int i = 0; i < foilStore.size(); i++)
    {
        current = foilStore.at(i);
        if (current.at(0) == base)
        {
            cout << current.at(1) << endl;
            unsorted.push_back(current);
        }
    }

    float max = 0;
    vector<string> maxEntry;
    int iterate = unsorted.size();
    int maxPos = 0;
    for (int i = 0; i < iterate; i++)
    {
        max = 0;
        for (int j = 0; j < unsorted.size(); j++)
        {
            current = unsorted.at(j);
            if ((stof(current.at(2)) / stof(current.at(3))) > max)
            {
                max = stof(current.at(2)) / stof(current.at(3));
                maxEntry = current;
                maxPos = j;
            }
        }

        vector<string> resultRow;
        resultRow.push_back(maxEntry.at(1) + "                                                                                  ");
        resultRow.push_back(maxEntry.at(3) + ":" + maxEntry.at(2) + "                    ");
        results.push_back(resultRow);

        for (int k = 0; k < resultRow.size(); k++)
        {
            cout << resultRow.at(k) << " ";
        }
        cout << endl;

        unsorted.erase(unsorted.begin() + maxPos);
    }
};

void searchLowestRatio(vector<vector<string>>& results, vector<vector<string>> foilStore, vector<string>& searchInputs)
{
    // given base ID, find date with highest ratio of trips to vehicles
    string base = searchInputs.at(0);
    base = base.substr(1, base.length());
    cout << base << endl;

    vector<string> current;
    vector<vector<string>> unsorted;
    for (int i = 0; i < foilStore.size(); i++)
    {
        current = foilStore.at(i);
        if (current.at(0) == base)
        {
            cout << current.at(1) << endl;
            unsorted.push_back(current);
        }
    }

    float min = 100000000;
    vector<string> minEntry;
    int iterate = unsorted.size();
    int minPos = 0;
    for (int i = 0; i < iterate; i++)
    {
        min = 100000000;
        for (int j = 0; j < unsorted.size(); j++)
        {
            current = unsorted.at(j);
            if ((stof(current.at(2)) / stof(current.at(3))) < min)
            {
                min = stof(current.at(2)) / stof(current.at(3));
                minEntry = current;
                minPos = j;
            }
        }

        vector<string> resultRow;
        resultRow.push_back(minEntry.at(1) + "                                                                                  ");
        resultRow.push_back(minEntry.at(3) + ":" + minEntry.at(2) + "                   ");
        results.push_back(resultRow);

        for (int k = 0; k < resultRow.size(); k++)
        {
            cout << resultRow.at(k) << " ";
        }
        cout << endl;

        unsorted.erase(unsorted.begin() + minPos);
    }
};

void calculateShortestInterval(vector<vector<string>>& results1, Storage& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;
    Storage* obj = &csvData;
    string in1;
    vector<string> temp1;
    vector<string> temp2;
    vector<string> saveMin;
    string save1;
    string save2;
    int daydiff = 0, hourdiff = 0, mindiff = 0;
    int minimum = 0, mimtemp = 0;
    string tempresult;
    for (unsigned int k = 0; k < obj->getOrigSize(); k++) {// searches for Location: lon/lat 
        Use* row1 = new Use;
        *row1 = obj->getRow(k);
        if (searchInputs.at(0).compare(row1->getLat()) == 0 && searchInputs.at(1).compare(row1->getLong()) == 0) {
            in1 = row1->getDate() + " " + row1->getTime() + " ";

            temp2.push_back(in1);
            in1 = row1->getLat() + " ";

            temp2.push_back(in1);
            in1 = row1->getLong() + " ";

            temp2.push_back(in1);
            in1 = row1->getBase();

            //          miniVec.push_back(in1);
            temp2.push_back(in1);

            if (temp1.empty()) {
                temp1 = temp2;
            }
            else {
                for (vector<string>::iterator i = temp1.begin(); i != temp1.end(); ++i) {
                    save1 += *i;
                }
                for (vector<string>::iterator i = temp2.begin(); i != temp2.end(); ++i) {
                    save2 += *i;
                }
                if (save1[3] == '/' && save2[3] == '/') {   // 4/1/2014 9:58:00 && 4/1/2014 9:58:00 or 4/1/2014 10:01:00
                    daydiff = save2[2] - save1[2];
                    if (save1[10] == ':' && save2[10] == ':') {
                        hourdiff = save2[9] - save1[9];
                        if (save2[11] >= save1[11] && save2[12] >= save1[12]) {
                            mindiff = (save2[11] - save1[11]) * 10 + (save2[12] - save1[12]);
                        }
                        else if (save2[11] >= save1[11] && save2[12] < save1[12]) {
                            mindiff = (save2[11] - save1[11] - 1) * 10 + (10 + save2[12] - save1[12]);
                        }
                        else if (save2[11] < save1[11] && save2[12] >= save1[12]) {
                            mindiff = (save2[11] - save1[11] + 6) * 10 + (save2[12] - save1[12]);
                            hourdiff = hourdiff - 1;
                        }
                        else if (save2[11] < save1[11] && save2[12] < save1[12]) {
                            mindiff = (save2[11] - save1[11] + 6 - 1) * 10 + (10 + save2[12] - save1[12]);
                            hourdiff = hourdiff - 1;
                        }
                    }
                    else if (save1[10] == ':' && save2[10] != ':') {    // 4/1/2014 9:58:00 && 4/1/2014 10:01:00
                        hourdiff = (save2[9] - 48) * 10 + save2[10] - save1[9];
                        if (save2[12] >= save1[11] && save2[13] >= save1[12]) {
                            mindiff = (save2[12] - save1[11]) * 10 + (save2[13] - save1[12]);
                        }
                        else if (save2[12] >= save1[11] && save2[13] < save1[12]) {
                            mindiff = (save2[12] - save1[11] - 1) * 10 + (10 + save2[13] - save1[12]);
                        }
                        else if (save2[12] < save1[11] && save2[13] >= save1[12]) {
                            mindiff = (save2[12] - save1[11] + 6) * 10 + (save2[13] - save1[12]);
                            hourdiff = hourdiff - 1;
                        }
                        else if (save2[12] < save1[11] && save2[13] < save1[12]) {
                            mindiff = (save2[12] - save1[11] + 6 - 1) * 10 + (10 + save2[13] - save1[12]);
                            hourdiff = hourdiff - 1;
                        }
                    }
                    else if (save1[10] != ':' && save2[10] != ':') {    // 4/1/2014 10:01:00 && 4/1/2014 10:01:00
                        if (save2[9] >= save1[9] && save2[10] > save1[10]) {
                            hourdiff = (save2[9] - save1[9]) * 10 + save2[10] - save1[10];
                        }
                        else if (save2[9] > save1[9] && save2[10] < save1[10]) {
                            hourdiff = (save2[9] - save1[9] - 1) * 10 + 10 + save2[10] - save1[10];
                        }

                        if (save2[12] >= save1[12] && save2[13] >= save1[13]) {
                            mindiff = (save2[12] - save1[12]) * 10 + (save2[13] - save1[13]);
                        }
                        else if (save2[12] >= save1[12] && save2[13] < save1[13]) {
                            mindiff = (save2[12] - save1[12] - 1) * 10 + (10 + save2[13] - save1[13]);
                        }
                        else if (save2[12] < save1[12] && save2[13] >= save1[13]) {
                            mindiff = (save2[12] - save1[12] + 6) * 10 + (save2[13] - save1[13]);
                            hourdiff = hourdiff - 1;
                        }
                        else if (save2[12] < save1[12] && save2[13] < save1[13]) {
                            mindiff = (save2[12] - save1[12] + 6 - 1) * 10 + (10 + save2[13] - save1[13]);
                            hourdiff = hourdiff - 1;
                        }
                    }
                    save1 = "";
                    save2 = "";
                    temp1 = temp2;
                    temp2.clear();

                    tempresult = to_string(daydiff);
                    tempresult.append("day ");
                    tempresult += to_string(hourdiff);
                    tempresult.append("hour ");
                    tempresult += to_string(mindiff);
                    tempresult.append("minute");
                    miniVec.push_back(tempresult);
                    mimtemp = (1440 * daydiff) + (60 * hourdiff) + (mindiff);
                    if (minimum == 0) {
                        minimum = mimtemp;
                        saveMin.push_back(tempresult);
                    }
                    else if (minimum > mimtemp) {
                        minimum = mimtemp;
                        saveMin.clear();
                        saveMin.push_back(tempresult);
                    }

                    results1.push_back(miniVec);
                    miniVec.clear();
                }
                else if (save1[3] == '/' && save2[3] != '/') {  // 4/1/2014 9:58:00 && 4/11/2014 9:58:00 or 4/11/2014 10:01:00
                    daydiff = (save2[2] - 48) * 10 + save2[3] - save1[2];
                    if (save1[10] == ':' && save2[11] == ':') {
                        hourdiff = save2[10] - save1[9];
                        if (save2[12] >= save1[11] && save2[13] >= save1[12]) {
                            mindiff = (save2[12] - save1[11]) * 10 + (save2[13] - save1[12]);
                        }
                        else if (save2[12] >= save1[11] && save2[13] < save1[12]) {
                            mindiff = (save2[12] - save1[11] - 1) * 10 + (10 + save2[13] - save1[12]);
                        }
                        else if (save2[12] < save1[11] && save2[13] >= save1[12]) {
                            mindiff = (save2[12] - save1[11] + 6) * 10 + (save2[13] - save1[12]);
                            hourdiff = hourdiff - 1;
                        }
                        else if (save2[12] < save1[11] && save2[13] < save1[12]) {
                            mindiff = (save2[12] - save1[11] + 6 - 1) * 10 + (10 + save2[13] - save1[12]);
                            hourdiff = hourdiff - 1;
                        }
                    }
                    else if (save1[10] == ':' && save2[11] != ':') {    // 4/1/2014 9:58:00 && 4/1/2014 10:01:00
                        hourdiff = (save2[10] - 48) * 10 + save2[11] - save1[9];
                        if (save2[13] >= save1[11] && save2[14] >= save1[12]) {
                            mindiff = (save2[13] - save1[11]) * 10 + (save2[14] - save1[12]);
                        }
                        else if (save2[13] >= save1[11] && save2[14] < save1[12]) {
                            mindiff = (save2[13] - save1[11] - 1) * 10 + (10 + save2[14] - save1[12]);
                        }
                        else if (save2[13] < save1[11] && save2[14] >= save1[12]) {
                            mindiff = (save2[13] - save1[11] + 6) * 10 + (save2[14] - save1[12]);
                            hourdiff = hourdiff - 1;
                        }
                        else if (save2[13] < save1[11] && save2[14] < save1[12]) {
                            mindiff = (save2[13] - save1[11] + 6 - 1) * 10 + (10 + save2[14] - save1[12]);
                            hourdiff = hourdiff - 1;
                        }
                    }
                    else if (save1[10] != ':' && save2[11] != ':') {    // 4/1/2014 10:01:00 && 4/1/2014 10:01:00
                        if (save2[10] >= save1[9] && save2[11] > save1[10]) {
                            hourdiff = (save2[10] - save1[9]) * 10 + save2[11] - save1[10];
                        }
                        else if (save2[10] > save1[9] && save2[11] < save1[10]) {
                            hourdiff = (save2[10] - save1[9] - 1) * 10 + 10 + save2[11] - save1[10];
                        }

                        if (save2[13] >= save1[12] && save2[14] >= save1[13]) {
                            mindiff = (save2[13] - save1[12]) * 10 + (save2[14] - save1[13]);
                        }
                        else if (save2[13] >= save1[12] && save2[14] < save1[13]) {
                            mindiff = (save2[13] - save1[12] - 1) * 10 + (10 + save2[14] - save1[13]);
                        }
                        else if (save2[13] < save1[12] && save2[14] >= save1[13]) {
                            mindiff = (save2[13] - save1[12] + 6) * 10 + (save2[14] - save1[13]);
                            hourdiff = hourdiff - 1;
                        }
                        else if (save2[13] < save1[12] && save2[14] < save1[13]) {
                            mindiff = (save2[13] - save1[12] + 6 - 1) * 10 + (10 + save2[14] - save1[13]);
                            hourdiff = hourdiff - 1;
                        }
                    }
                    save1 = "";
                    save2 = "";
                    temp1 = temp2;
                    temp2.clear();
                    tempresult = to_string(daydiff);
                    tempresult.append("day ");
                    tempresult += to_string(hourdiff);
                    tempresult.append("hour ");
                    tempresult += to_string(mindiff);
                    tempresult.append("minute");
                    miniVec.push_back(tempresult);
                    mimtemp = (1440 * daydiff) + (60 * hourdiff) + (mindiff);
                    if (minimum == 0) {
                        minimum = mimtemp;
                        saveMin.push_back(tempresult);
                    }
                    else if (minimum > mimtemp) {
                        minimum = mimtemp;
                        saveMin.clear();
                        saveMin.push_back(tempresult);
                    }

                    results1.push_back(miniVec);
                    miniVec.clear();
                }
                else if (save1[3] != '/' && save2[3] != '/') {  // 4/10/2014 9:58:00 && 4/11/2014 9:58:00 or 4/11/2014 10:01:00
                    daydiff = (save2[2] - save1[2]) * 10 + save2[3] - save1[3];
                    if (save1[11] == ':' && save2[11] == ':') {
                        hourdiff = save2[10] - save1[10];
                        if (save2[12] >= save1[12] && save2[13] >= save1[13]) {
                            mindiff = (save2[12] - save1[12]) * 10 + (save2[13] - save1[13]);
                        }
                        else if (save2[12] >= save1[12] && save2[13] < save1[13]) {
                            mindiff = (save2[12] - save1[12] - 1) * 10 + (10 + save2[13] - save1[13]);
                        }
                        else if (save2[12] < save1[12] && save2[13] >= save1[13]) {
                            mindiff = (save2[12] - save1[12] + 6) * 10 + (save2[13] - save1[13]);
                            hourdiff = hourdiff - 1;
                        }
                        else if (save2[12] < save1[12] && save2[13] < save1[13]) {
                            mindiff = (save2[12] - save1[12] + 6 - 1) * 10 + (10 + save2[13] - save1[13]);
                            hourdiff = hourdiff - 1;
                        }
                    }
                    else if (save1[11] == ':' && save2[11] != ':') {    // 4/1/2014 9:58:00 && 4/1/2014 10:01:00
                        hourdiff = (save2[11] - 48) * 10 + save2[11] - save1[10];
                        if (save2[13] >= save1[12] && save2[14] >= save1[13]) {
                            mindiff = (save2[13] - save1[12]) * 10 + (save2[14] - save1[13]);
                        }
                        else if (save2[13] >= save1[12] && save2[14] < save1[13]) {
                            mindiff = (save2[13] - save1[12] - 1) * 10 + (10 + save2[14] - save1[13]);
                        }
                        else if (save2[13] < save1[12] && save2[14] >= save1[13]) {
                            mindiff = (save2[13] - save1[12] + 6) * 10 + (save2[14] - save1[13]);
                            hourdiff = hourdiff - 1;
                        }
                        else if (save2[13] < save1[12] && save2[14] < save1[13]) {
                            mindiff = (save2[13] - save1[12] + 6 - 1) * 10 + (10 + save2[14] - save1[13]);
                            hourdiff = hourdiff - 1;
                        }
                    }
                    else if (save1[11] != ':' && save2[11] != ':') {    // 4/1/2014 10:01:00 && 4/1/2014 10:01:00
                        if (save2[10] >= save1[10] && save2[11] > save1[11]) {
                            hourdiff = (save2[10] - save1[10]) * 10 + save2[11] - save1[11];
                        }
                        else if (save2[10] > save1[10] && save2[11] < save1[11]) {
                            hourdiff = (save2[10] - save1[10] - 1) * 10 + 10 + save2[11] - save1[11];
                        }

                        if (save2[13] >= save1[13] && save2[14] >= save1[14]) {
                            mindiff = (save2[13] - save1[13]) * 10 + (save2[14] - save1[14]);
                        }
                        else if (save2[13] >= save1[13] && save2[14] < save1[14]) {
                            mindiff = (save2[13] - save1[13] - 1) * 10 + (10 + save2[14] - save1[14]);
                        }
                        else if (save2[13] < save1[13] && save2[14] >= save1[14]) {
                            mindiff = (save2[13] - save1[13] + 6) * 10 + (save2[14] - save1[14]);
                            hourdiff = hourdiff - 1;
                        }
                        else if (save2[13] < save1[13] && save2[14] < save1[14]) {
                            mindiff = (save2[13] - save1[13] + 6 - 1) * 10 + (10 + save2[14] - save1[14]);
                            hourdiff = hourdiff - 1;
                        }
                    }
                    save1 = "";
                    save2 = "";
                    temp1 = temp2;
                    temp2.clear();
                    tempresult = to_string(daydiff);
                    tempresult.append("day ");
                    tempresult += to_string(hourdiff);
                    tempresult.append("hour ");
                    tempresult += to_string(mindiff);
                    tempresult.append("minute");
                    miniVec.push_back(tempresult);
                    mimtemp = (1440 * daydiff) + (60 * hourdiff) + (mindiff);
                    if (minimum == 0) {
                        minimum = mimtemp;
                        saveMin.push_back(tempresult);
                    }
                    else if (minimum > mimtemp) {
                        minimum = mimtemp;
                        saveMin.clear();
                        saveMin.push_back(tempresult);
                    }

                    results1.push_back(miniVec);
                    miniVec.clear();
                }
                /*
                std::string s;
                for (std::vector<std::string>::const_iterator i = v.begin(); i != v.end(); ++i)
                s += *i;
                */

            }
            temp2.clear();

            //pushes found searches to results
        }

        delete row1;
    }

    results1.push_back(saveMin);
    saveMin.clear();
};

void calculateLongestInterval(vector<vector<string>>& results1, Storage& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;
    Storage* obj = &csvData;
    string in1;
    vector<string> temp1;
    vector<string> temp2;
    vector<string> saveMax;
    string save1;
    string save2;
    int daydiff = 0, hourdiff = 0, mindiff = 0;
    int maximum = 0, maxtemp = 0;
    string tempresult;
    for (unsigned int k = 0; k < obj->getOrigSize(); k++) {// searches for Location: lon/lat 
        Use* row1 = new Use;
        *row1 = obj->getRow(k);
        if (searchInputs.at(0).compare(row1->getLat()) == 0 && searchInputs.at(1).compare(row1->getLong()) == 0) {
            in1 = row1->getDate() + " " + row1->getTime() + " ";

            temp2.push_back(in1);
            in1 = row1->getLat() + " ";

            temp2.push_back(in1);
            in1 = row1->getLong() + " ";

            temp2.push_back(in1);
            in1 = row1->getBase();

            //          miniVec.push_back(in1);
            temp2.push_back(in1);

            if (temp1.empty()) {
                temp1 = temp2;
            }
            else {
                for (vector<string>::iterator i = temp1.begin(); i != temp1.end(); ++i) {
                    save1 += *i;
                }
                for (vector<string>::iterator i = temp2.begin(); i != temp2.end(); ++i) {
                    save2 += *i;
                }
                if (save1[3] == '/' && save2[3] == '/') {   // 4/1/2014 9:58:00 && 4/1/2014 9:58:00 or 4/1/2014 10:01:00
                    daydiff = save2[2] - save1[2];
                    if (save1[10] == ':' && save2[10] == ':') {
                        hourdiff = save2[9] - save1[9];
                        if (save2[11] >= save1[11] && save2[12] >= save1[12]) {
                            mindiff = (save2[11] - save1[11]) * 10 + (save2[12] - save1[12]);
                        }
                        else if (save2[11] >= save1[11] && save2[12] < save1[12]) {
                            mindiff = (save2[11] - save1[11] - 1) * 10 + (10 + save2[12] - save1[12]);
                        }
                        else if (save2[11] < save1[11] && save2[12] >= save1[12]) {
                            mindiff = (save2[11] - save1[11] + 6) * 10 + (save2[12] - save1[12]);
                            hourdiff = hourdiff - 1;
                        }
                        else if (save2[11] < save1[11] && save2[12] < save1[12]) {
                            mindiff = (save2[11] - save1[11] + 6 - 1) * 10 + (10 + save2[12] - save1[12]);
                            hourdiff = hourdiff - 1;
                        }
                    }
                    else if (save1[10] == ':' && save2[10] != ':') {    // 4/1/2014 9:58:00 && 4/1/2014 10:01:00
                        hourdiff = (save2[9] - 48) * 10 + save2[10] - save1[9];
                        if (save2[12] >= save1[11] && save2[13] >= save1[12]) {
                            mindiff = (save2[12] - save1[11]) * 10 + (save2[13] - save1[12]);
                        }
                        else if (save2[12] >= save1[11] && save2[13] < save1[12]) {
                            mindiff = (save2[12] - save1[11] - 1) * 10 + (10 + save2[13] - save1[12]);
                        }
                        else if (save2[12] < save1[11] && save2[13] >= save1[12]) {
                            mindiff = (save2[12] - save1[11] + 6) * 10 + (save2[13] - save1[12]);
                            hourdiff = hourdiff - 1;
                        }
                        else if (save2[12] < save1[11] && save2[13] < save1[12]) {
                            mindiff = (save2[12] - save1[11] + 6 - 1) * 10 + (10 + save2[13] - save1[12]);
                            hourdiff = hourdiff - 1;
                        }
                    }
                    else if (save1[10] != ':' && save2[10] != ':') {    // 4/1/2014 10:01:00 && 4/1/2014 10:01:00
                        if (save2[9] >= save1[9] && save2[10] > save1[10]) {
                            hourdiff = (save2[9] - save1[9]) * 10 + save2[10] - save1[10];
                        }
                        else if (save2[9] > save1[9] && save2[10] < save1[10]) {
                            hourdiff = (save2[9] - save1[9] - 1) * 10 + 10 + save2[10] - save1[10];
                        }

                        if (save2[12] >= save1[12] && save2[13] >= save1[13]) {
                            mindiff = (save2[12] - save1[12]) * 10 + (save2[13] - save1[13]);
                        }
                        else if (save2[12] >= save1[12] && save2[13] < save1[13]) {
                            mindiff = (save2[12] - save1[12] - 1) * 10 + (10 + save2[13] - save1[13]);
                        }
                        else if (save2[12] < save1[12] && save2[13] >= save1[13]) {
                            mindiff = (save2[12] - save1[12] + 6) * 10 + (save2[13] - save1[13]);
                            hourdiff = hourdiff - 1;
                        }
                        else if (save2[12] < save1[12] && save2[13] < save1[13]) {
                            mindiff = (save2[12] - save1[12] + 6 - 1) * 10 + (10 + save2[13] - save1[13]);
                            hourdiff = hourdiff - 1;
                        }
                    }
                    save1 = "";
                    save2 = "";
                    temp1 = temp2;
                    temp2.clear();

                    tempresult = to_string(daydiff);
                    tempresult.append("day ");
                    tempresult += to_string(hourdiff);
                    tempresult.append("hour ");
                    tempresult += to_string(mindiff);
                    tempresult.append("minute");
                    miniVec.push_back(tempresult);
                    maxtemp = (1440 * daydiff) + (60 * hourdiff) + (mindiff);
                    if (maximum == 0) {
                        maximum = maxtemp;
                        saveMax.push_back(tempresult);
                    }
                    else if (maximum < maxtemp) {
                        maximum = maxtemp;
                        saveMax.clear();
                        saveMax.push_back(tempresult);
                    }

                    results1.push_back(miniVec);
                    miniVec.clear();
                }
                else if (save1[3] == '/' && save2[3] != '/') {  // 4/1/2014 9:58:00 && 4/11/2014 9:58:00 or 4/11/2014 10:01:00
                    daydiff = (save2[2] - 48) * 10 + save2[3] - save1[2];
                    if (save1[10] == ':' && save2[11] == ':') {
                        hourdiff = save2[10] - save1[9];
                        if (save2[12] >= save1[11] && save2[13] >= save1[12]) {
                            mindiff = (save2[12] - save1[11]) * 10 + (save2[13] - save1[12]);
                        }
                        else if (save2[12] >= save1[11] && save2[13] < save1[12]) {
                            mindiff = (save2[12] - save1[11] - 1) * 10 + (10 + save2[13] - save1[12]);
                        }
                        else if (save2[12] < save1[11] && save2[13] >= save1[12]) {
                            mindiff = (save2[12] - save1[11] + 6) * 10 + (save2[13] - save1[12]);
                            hourdiff = hourdiff - 1;
                        }
                        else if (save2[12] < save1[11] && save2[13] < save1[12]) {
                            mindiff = (save2[12] - save1[11] + 6 - 1) * 10 + (10 + save2[13] - save1[12]);
                            hourdiff = hourdiff - 1;
                        }
                    }
                    else if (save1[10] == ':' && save2[11] != ':') {    // 4/1/2014 9:58:00 && 4/1/2014 10:01:00
                        hourdiff = (save2[10] - 48) * 10 + save2[11] - save1[9];
                        if (save2[13] >= save1[11] && save2[14] >= save1[12]) {
                            mindiff = (save2[13] - save1[11]) * 10 + (save2[14] - save1[12]);
                        }
                        else if (save2[13] >= save1[11] && save2[14] < save1[12]) {
                            mindiff = (save2[13] - save1[11] - 1) * 10 + (10 + save2[14] - save1[12]);
                        }
                        else if (save2[13] < save1[11] && save2[14] >= save1[12]) {
                            mindiff = (save2[13] - save1[11] + 6) * 10 + (save2[14] - save1[12]);
                            hourdiff = hourdiff - 1;
                        }
                        else if (save2[13] < save1[11] && save2[14] < save1[12]) {
                            mindiff = (save2[13] - save1[11] + 6 - 1) * 10 + (10 + save2[14] - save1[12]);
                            hourdiff = hourdiff - 1;
                        }
                    }
                    else if (save1[10] != ':' && save2[11] != ':') {    // 4/1/2014 10:01:00 && 4/1/2014 10:01:00
                        if (save2[10] >= save1[9] && save2[11] > save1[10]) {
                            hourdiff = (save2[10] - save1[9]) * 10 + save2[11] - save1[10];
                        }
                        else if (save2[10] > save1[9] && save2[11] < save1[10]) {
                            hourdiff = (save2[10] - save1[9] - 1) * 10 + 10 + save2[11] - save1[10];
                        }

                        if (save2[13] >= save1[12] && save2[14] >= save1[13]) {
                            mindiff = (save2[13] - save1[12]) * 10 + (save2[14] - save1[13]);
                        }
                        else if (save2[13] >= save1[12] && save2[14] < save1[13]) {
                            mindiff = (save2[13] - save1[12] - 1) * 10 + (10 + save2[14] - save1[13]);
                        }
                        else if (save2[13] < save1[12] && save2[14] >= save1[13]) {
                            mindiff = (save2[13] - save1[12] + 6) * 10 + (save2[14] - save1[13]);
                            hourdiff = hourdiff - 1;
                        }
                        else if (save2[13] < save1[12] && save2[14] < save1[13]) {
                            mindiff = (save2[13] - save1[12] + 6 - 1) * 10 + (10 + save2[14] - save1[13]);
                            hourdiff = hourdiff - 1;
                        }
                    }
                    save1 = "";
                    save2 = "";
                    temp1 = temp2;
                    temp2.clear();
                    tempresult = to_string(daydiff);
                    tempresult.append("day ");
                    tempresult += to_string(hourdiff);
                    tempresult.append("hour ");
                    tempresult += to_string(mindiff);
                    tempresult.append("minute");
                    miniVec.push_back(tempresult);
                    maxtemp = (1440 * daydiff) + (60 * hourdiff) + (mindiff);
                    if (maximum == 0) {
                        maximum = maxtemp;
                        saveMax.push_back(tempresult);
                    }
                    else if (maximum < maxtemp) {
                        maximum = maxtemp;
                        saveMax.clear();
                        saveMax.push_back(tempresult);
                    }

                    results1.push_back(miniVec);
                    miniVec.clear();
                }
                else if (save1[3] != '/' && save2[3] != '/') {  // 4/10/2014 9:58:00 && 4/11/2014 9:58:00 or 4/11/2014 10:01:00
                    daydiff = (save2[2] - save1[2]) * 10 + save2[3] - save1[3];
                    if (save1[11] == ':' && save2[11] == ':') {
                        hourdiff = save2[10] - save1[10];
                        if (save2[12] >= save1[12] && save2[13] >= save1[13]) {
                            mindiff = (save2[12] - save1[12]) * 10 + (save2[13] - save1[13]);
                        }
                        else if (save2[12] >= save1[12] && save2[13] < save1[13]) {
                            mindiff = (save2[12] - save1[12] - 1) * 10 + (10 + save2[13] - save1[13]);
                        }
                        else if (save2[12] < save1[12] && save2[13] >= save1[13]) {
                            mindiff = (save2[12] - save1[12] + 6) * 10 + (save2[13] - save1[13]);
                            hourdiff = hourdiff - 1;
                        }
                        else if (save2[12] < save1[12] && save2[13] < save1[13]) {
                            mindiff = (save2[12] - save1[12] + 6 - 1) * 10 + (10 + save2[13] - save1[13]);
                            hourdiff = hourdiff - 1;
                        }
                    }
                    else if (save1[11] == ':' && save2[11] != ':') {    // 4/1/2014 9:58:00 && 4/1/2014 10:01:00
                        hourdiff = (save2[11] - 48) * 10 + save2[11] - save1[10];
                        if (save2[13] >= save1[12] && save2[14] >= save1[13]) {
                            mindiff = (save2[13] - save1[12]) * 10 + (save2[14] - save1[13]);
                        }
                        else if (save2[13] >= save1[12] && save2[14] < save1[13]) {
                            mindiff = (save2[13] - save1[12] - 1) * 10 + (10 + save2[14] - save1[13]);
                        }
                        else if (save2[13] < save1[12] && save2[14] >= save1[13]) {
                            mindiff = (save2[13] - save1[12] + 6) * 10 + (save2[14] - save1[13]);
                            hourdiff = hourdiff - 1;
                        }
                        else if (save2[13] < save1[12] && save2[14] < save1[13]) {
                            mindiff = (save2[13] - save1[12] + 6 - 1) * 10 + (10 + save2[14] - save1[13]);
                            hourdiff = hourdiff - 1;
                        }
                    }
                    else if (save1[11] != ':' && save2[11] != ':') {    // 4/1/2014 10:01:00 && 4/1/2014 10:01:00
                        if (save2[10] >= save1[10] && save2[11] > save1[11]) {
                            hourdiff = (save2[10] - save1[10]) * 10 + save2[11] - save1[11];
                        }
                        else if (save2[10] > save1[10] && save2[11] < save1[11]) {
                            hourdiff = (save2[10] - save1[10] - 1) * 10 + 10 + save2[11] - save1[11];
                        }

                        if (save2[13] >= save1[13] && save2[14] >= save1[14]) {
                            mindiff = (save2[13] - save1[13]) * 10 + (save2[14] - save1[14]);
                        }
                        else if (save2[13] >= save1[13] && save2[14] < save1[14]) {
                            mindiff = (save2[13] - save1[13] - 1) * 10 + (10 + save2[14] - save1[14]);
                        }
                        else if (save2[13] < save1[13] && save2[14] >= save1[14]) {
                            mindiff = (save2[13] - save1[13] + 6) * 10 + (save2[14] - save1[14]);
                            hourdiff = hourdiff - 1;
                        }
                        else if (save2[13] < save1[13] && save2[14] < save1[14]) {
                            mindiff = (save2[13] - save1[13] + 6 - 1) * 10 + (10 + save2[14] - save1[14]);
                            hourdiff = hourdiff - 1;
                        }
                    }
                    save1 = "";
                    save2 = "";
                    temp1 = temp2;
                    temp2.clear();
                    tempresult = to_string(daydiff);
                    tempresult.append("day ");
                    tempresult += to_string(hourdiff);
                    tempresult.append("hour ");
                    tempresult += to_string(mindiff);
                    tempresult.append("minute");
                    miniVec.push_back(tempresult);
                    maxtemp = (1440 * daydiff) + (60 * hourdiff) + (mindiff);
                    if (maximum == 0) {
                        maximum = maxtemp;
                        saveMax.push_back(tempresult);
                    }
                    else if (maximum < maxtemp) {
                        maximum = maxtemp;
                        saveMax.clear();
                        saveMax.push_back(tempresult);
                    }

                    results1.push_back(miniVec);
                    miniVec.clear();
                }
                /*
                std::string s;
                for (std::vector<std::string>::const_iterator i = v.begin(); i != v.end(); ++i)
                s += *i;
                */

            }
            temp2.clear();

            //pushes found searches to results
        }

        delete row1;
    }

    results1.push_back(saveMax);
    saveMax.clear();
};

void calculateAverageInterval(vector<vector<string>>& results1, Storage& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;
    Storage* obj = &csvData;
    string in1;
    vector<string> temp1;
    vector<string> temp2;
    vector<string> saveAvg;
    string save1;
    string save2;
    string avg;
    int daydiff = 0, hourdiff = 0, mindiff = 0;
    int average = 0, avgcount = 0, avgtemp = 0;
    string tempresult;
    for (unsigned int k = 0; k < obj->getOrigSize(); k++) {// searches for Location: lon/lat 
        Use* row1 = new Use;
        *row1 = obj->getRow(k);
        if (searchInputs.at(0).compare(row1->getLat()) == 0 && searchInputs.at(1).compare(row1->getLong()) == 0) {
            in1 = row1->getDate() + " " + row1->getTime() + " ";

            temp2.push_back(in1);
            in1 = row1->getLat() + " ";

            temp2.push_back(in1);
            in1 = row1->getLong() + " ";

            temp2.push_back(in1);
            in1 = row1->getBase();

            //          miniVec.push_back(in1);
            temp2.push_back(in1);

            if (temp1.empty()) {
                temp1 = temp2;
            }
            else {
                for (vector<string>::iterator i = temp1.begin(); i != temp1.end(); ++i) {
                    save1 += *i;
                }
                for (vector<string>::iterator i = temp2.begin(); i != temp2.end(); ++i) {
                    save2 += *i;
                }
                if (save1[3] == '/' && save2[3] == '/') {   // 4/1/2014 9:58:00 && 4/1/2014 9:58:00 or 4/1/2014 10:01:00
                    daydiff = save2[2] - save1[2];
                    if (save1[10] == ':' && save2[10] == ':') {
                        hourdiff = save2[9] - save1[9];
                        if (save2[11] >= save1[11] && save2[12] >= save1[12]) {
                            mindiff = (save2[11] - save1[11]) * 10 + (save2[12] - save1[12]);
                        }
                        else if (save2[11] >= save1[11] && save2[12] < save1[12]) {
                            mindiff = (save2[11] - save1[11] - 1) * 10 + (10 + save2[12] - save1[12]);
                        }
                        else if (save2[11] < save1[11] && save2[12] >= save1[12]) {
                            mindiff = (save2[11] - save1[11] + 6) * 10 + (save2[12] - save1[12]);
                            hourdiff = hourdiff - 1;
                        }
                        else if (save2[11] < save1[11] && save2[12] < save1[12]) {
                            mindiff = (save2[11] - save1[11] + 6 - 1) * 10 + (10 + save2[12] - save1[12]);
                            hourdiff = hourdiff - 1;
                        }
                    }
                    else if (save1[10] == ':' && save2[10] != ':') {    // 4/1/2014 9:58:00 && 4/1/2014 10:01:00
                        hourdiff = (save2[9] - 48) * 10 + save2[10] - save1[9];
                        if (save2[12] >= save1[11] && save2[13] >= save1[12]) {
                            mindiff = (save2[12] - save1[11]) * 10 + (save2[13] - save1[12]);
                        }
                        else if (save2[12] >= save1[11] && save2[13] < save1[12]) {
                            mindiff = (save2[12] - save1[11] - 1) * 10 + (10 + save2[13] - save1[12]);
                        }
                        else if (save2[12] < save1[11] && save2[13] >= save1[12]) {
                            mindiff = (save2[12] - save1[11] + 6) * 10 + (save2[13] - save1[12]);
                            hourdiff = hourdiff - 1;
                        }
                        else if (save2[12] < save1[11] && save2[13] < save1[12]) {
                            mindiff = (save2[12] - save1[11] + 6 - 1) * 10 + (10 + save2[13] - save1[12]);
                            hourdiff = hourdiff - 1;
                        }
                    }
                    else if (save1[10] != ':' && save2[10] != ':') {    // 4/1/2014 10:01:00 && 4/1/2014 10:01:00
                        if (save2[9] >= save1[9] && save2[10] > save1[10]) {
                            hourdiff = (save2[9] - save1[9]) * 10 + save2[10] - save1[10];
                        }
                        else if (save2[9] > save1[9] && save2[10] < save1[10]) {
                            hourdiff = (save2[9] - save1[9] - 1) * 10 + 10 + save2[10] - save1[10];
                        }

                        if (save2[12] >= save1[12] && save2[13] >= save1[13]) {
                            mindiff = (save2[12] - save1[12]) * 10 + (save2[13] - save1[13]);
                        }
                        else if (save2[12] >= save1[12] && save2[13] < save1[13]) {
                            mindiff = (save2[12] - save1[12] - 1) * 10 + (10 + save2[13] - save1[13]);
                        }
                        else if (save2[12] < save1[12] && save2[13] >= save1[13]) {
                            mindiff = (save2[12] - save1[12] + 6) * 10 + (save2[13] - save1[13]);
                            hourdiff = hourdiff - 1;
                        }
                        else if (save2[12] < save1[12] && save2[13] < save1[13]) {
                            mindiff = (save2[12] - save1[12] + 6 - 1) * 10 + (10 + save2[13] - save1[13]);
                            hourdiff = hourdiff - 1;
                        }
                    }
                    save1 = "";
                    save2 = "";
                    temp1 = temp2;
                    temp2.clear();

                    tempresult = to_string(daydiff);
                    tempresult.append("day ");
                    tempresult += to_string(hourdiff);
                    tempresult.append("hour ");
                    tempresult += to_string(mindiff);
                    tempresult.append("minute");
                    miniVec.push_back(tempresult);
                    avgtemp = (1440 * daydiff) + (60 * hourdiff) + (mindiff);
                    if (average == 0) {
                        average = avgtemp;
                        avgcount = avgcount + 1;
                    }
                    else {
                        average = average + avgtemp;
                        avgcount = avgcount + 1;
                    }

                    results1.push_back(miniVec);
                    miniVec.clear();
                }
                else if (save1[3] == '/' && save2[3] != '/') {  // 4/1/2014 9:58:00 && 4/11/2014 9:58:00 or 4/11/2014 10:01:00
                    daydiff = (save2[2] - 48) * 10 + save2[3] - save1[2];
                    if (save1[10] == ':' && save2[11] == ':') {
                        hourdiff = save2[10] - save1[9];
                        if (save2[12] >= save1[11] && save2[13] >= save1[12]) {
                            mindiff = (save2[12] - save1[11]) * 10 + (save2[13] - save1[12]);
                        }
                        else if (save2[12] >= save1[11] && save2[13] < save1[12]) {
                            mindiff = (save2[12] - save1[11] - 1) * 10 + (10 + save2[13] - save1[12]);
                        }
                        else if (save2[12] < save1[11] && save2[13] >= save1[12]) {
                            mindiff = (save2[12] - save1[11] + 6) * 10 + (save2[13] - save1[12]);
                            hourdiff = hourdiff - 1;
                        }
                        else if (save2[12] < save1[11] && save2[13] < save1[12]) {
                            mindiff = (save2[12] - save1[11] + 6 - 1) * 10 + (10 + save2[13] - save1[12]);
                            hourdiff = hourdiff - 1;
                        }
                    }
                    else if (save1[10] == ':' && save2[11] != ':') {    // 4/1/2014 9:58:00 && 4/1/2014 10:01:00
                        hourdiff = (save2[10] - 48) * 10 + save2[11] - save1[9];
                        if (save2[13] >= save1[11] && save2[14] >= save1[12]) {
                            mindiff = (save2[13] - save1[11]) * 10 + (save2[14] - save1[12]);
                        }
                        else if (save2[13] >= save1[11] && save2[14] < save1[12]) {
                            mindiff = (save2[13] - save1[11] - 1) * 10 + (10 + save2[14] - save1[12]);
                        }
                        else if (save2[13] < save1[11] && save2[14] >= save1[12]) {
                            mindiff = (save2[13] - save1[11] + 6) * 10 + (save2[14] - save1[12]);
                            hourdiff = hourdiff - 1;
                        }
                        else if (save2[13] < save1[11] && save2[14] < save1[12]) {
                            mindiff = (save2[13] - save1[11] + 6 - 1) * 10 + (10 + save2[14] - save1[12]);
                            hourdiff = hourdiff - 1;
                        }
                    }
                    else if (save1[10] != ':' && save2[11] != ':') {    // 4/1/2014 10:01:00 && 4/1/2014 10:01:00
                        if (save2[10] >= save1[9] && save2[11] > save1[10]) {
                            hourdiff = (save2[10] - save1[9]) * 10 + save2[11] - save1[10];
                        }
                        else if (save2[10] > save1[9] && save2[11] < save1[10]) {
                            hourdiff = (save2[10] - save1[9] - 1) * 10 + 10 + save2[11] - save1[10];
                        }

                        if (save2[13] >= save1[12] && save2[14] >= save1[13]) {
                            mindiff = (save2[13] - save1[12]) * 10 + (save2[14] - save1[13]);
                        }
                        else if (save2[13] >= save1[12] && save2[14] < save1[13]) {
                            mindiff = (save2[13] - save1[12] - 1) * 10 + (10 + save2[14] - save1[13]);
                        }
                        else if (save2[13] < save1[12] && save2[14] >= save1[13]) {
                            mindiff = (save2[13] - save1[12] + 6) * 10 + (save2[14] - save1[13]);
                            hourdiff = hourdiff - 1;
                        }
                        else if (save2[13] < save1[12] && save2[14] < save1[13]) {
                            mindiff = (save2[13] - save1[12] + 6 - 1) * 10 + (10 + save2[14] - save1[13]);
                            hourdiff = hourdiff - 1;
                        }
                    }
                    save1 = "";
                    save2 = "";
                    temp1 = temp2;
                    temp2.clear();
                    tempresult = to_string(daydiff);
                    tempresult.append("day ");
                    tempresult += to_string(hourdiff);
                    tempresult.append("hour ");
                    tempresult += to_string(mindiff);
                    tempresult.append("minute");
                    miniVec.push_back(tempresult);
                    avgtemp = (1440 * daydiff) + (60 * hourdiff) + (mindiff);
                    if (average == 0) {
                        average = avgtemp;
                        avgcount = avgcount + 1;
                    }
                    else {
                        average = average + avgtemp;
                        avgcount = avgcount + 1;
                    }

                    results1.push_back(miniVec);
                    miniVec.clear();
                }
                else if (save1[3] != '/' && save2[3] != '/') {  // 4/10/2014 9:58:00 && 4/11/2014 9:58:00 or 4/11/2014 10:01:00
                    daydiff = (save2[2] - save1[2]) * 10 + save2[3] - save1[3];
                    if (save1[11] == ':' && save2[11] == ':') {
                        hourdiff = save2[10] - save1[10];
                        if (save2[12] >= save1[12] && save2[13] >= save1[13]) {
                            mindiff = (save2[12] - save1[12]) * 10 + (save2[13] - save1[13]);
                        }
                        else if (save2[12] >= save1[12] && save2[13] < save1[13]) {
                            mindiff = (save2[12] - save1[12] - 1) * 10 + (10 + save2[13] - save1[13]);
                        }
                        else if (save2[12] < save1[12] && save2[13] >= save1[13]) {
                            mindiff = (save2[12] - save1[12] + 6) * 10 + (save2[13] - save1[13]);
                            hourdiff = hourdiff - 1;
                        }
                        else if (save2[12] < save1[12] && save2[13] < save1[13]) {
                            mindiff = (save2[12] - save1[12] + 6 - 1) * 10 + (10 + save2[13] - save1[13]);
                            hourdiff = hourdiff - 1;
                        }
                    }
                    else if (save1[11] == ':' && save2[11] != ':') {    // 4/1/2014 9:58:00 && 4/1/2014 10:01:00
                        hourdiff = (save2[11] - 48) * 10 + save2[11] - save1[10];
                        if (save2[13] >= save1[12] && save2[14] >= save1[13]) {
                            mindiff = (save2[13] - save1[12]) * 10 + (save2[14] - save1[13]);
                        }
                        else if (save2[13] >= save1[12] && save2[14] < save1[13]) {
                            mindiff = (save2[13] - save1[12] - 1) * 10 + (10 + save2[14] - save1[13]);
                        }
                        else if (save2[13] < save1[12] && save2[14] >= save1[13]) {
                            mindiff = (save2[13] - save1[12] + 6) * 10 + (save2[14] - save1[13]);
                            hourdiff = hourdiff - 1;
                        }
                        else if (save2[13] < save1[12] && save2[14] < save1[13]) {
                            mindiff = (save2[13] - save1[12] + 6 - 1) * 10 + (10 + save2[14] - save1[13]);
                            hourdiff = hourdiff - 1;
                        }
                    }
                    else if (save1[11] != ':' && save2[11] != ':') {    // 4/1/2014 10:01:00 && 4/1/2014 10:01:00
                        if (save2[10] >= save1[10] && save2[11] > save1[11]) {
                            hourdiff = (save2[10] - save1[10]) * 10 + save2[11] - save1[11];
                        }
                        else if (save2[10] > save1[10] && save2[11] < save1[11]) {
                            hourdiff = (save2[10] - save1[10] - 1) * 10 + 10 + save2[11] - save1[11];
                        }

                        if (save2[13] >= save1[13] && save2[14] >= save1[14]) {
                            mindiff = (save2[13] - save1[13]) * 10 + (save2[14] - save1[14]);
                        }
                        else if (save2[13] >= save1[13] && save2[14] < save1[14]) {
                            mindiff = (save2[13] - save1[13] - 1) * 10 + (10 + save2[14] - save1[14]);
                        }
                        else if (save2[13] < save1[13] && save2[14] >= save1[14]) {
                            mindiff = (save2[13] - save1[13] + 6) * 10 + (save2[14] - save1[14]);
                            hourdiff = hourdiff - 1;
                        }
                        else if (save2[13] < save1[13] && save2[14] < save1[14]) {
                            mindiff = (save2[13] - save1[13] + 6 - 1) * 10 + (10 + save2[14] - save1[14]);
                            hourdiff = hourdiff - 1;
                        }
                    }
                    save1 = "";
                    save2 = "";
                    temp1 = temp2;
                    temp2.clear();
                    tempresult = to_string(daydiff);
                    tempresult.append("day ");
                    tempresult += to_string(hourdiff);
                    tempresult.append("hour ");
                    tempresult += to_string(mindiff);
                    tempresult.append("minute");
                    miniVec.push_back(tempresult);
                    avgtemp = (1440 * daydiff) + (60 * hourdiff) + (mindiff);
                    if (average == 0) {
                        average = avgtemp;
                        avgcount = avgcount + 1;
                    }
                    else {
                        average = average + avgtemp;
                        avgcount = avgcount + 1;
                    }

                    results1.push_back(miniVec);
                    miniVec.clear();
                }
                /*
                std::string s;
                for (std::vector<std::string>::const_iterator i = v.begin(); i != v.end(); ++i)
                s += *i;
                */

            }
            temp2.clear();

            //pushes found searches to results
        }

        delete row1;
    }
    avg = to_string(average / avgcount / 1440);
    avg.append("day ");
    avg += to_string(((average / avgcount) % 1440) / 60);
    avg.append("hour ");
    avg += to_string(((average / avgcount) % 1440) % 60);
    avg.append("minute");
    saveAvg.push_back(avg);
    results1.push_back(saveAvg);
    saveAvg.clear();
};

bool Storage::checkCount() {
    if (count >= 1000) {
        return true;
    }
    else {
        return false;
    }
}

void Storage::setFlag(string arg) { //sets flags for incrementality functionality 
    if (arg == "mostTime") {
        cout << "Most Time flag set" << endl;
        mTimeFlag = 1;
        return;
    }
    else if (arg == "leastTime") {
        cout << "Least Time flag set" << endl;
        lTimeFlag = 1;
        return;
    }
    else if (arg == "busiestDay") {
        cout << "Busiest day flag set" << endl;
        bDayFlag = 1;
        return;
    }
    else if (arg == "mostLoc") {
        cout << "Most Loc flag set" << endl;
        mLocFlag = 1;
        return;
    }
    else if (arg == "leastLoc") {
        cout << "Least Loc flag set" << endl;
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
            cout << "Most Time flag true" << endl;
            return true;
        }
        else {
            cout << "Most Time flag was false" << endl;
            return false;
        }
    }
    else if (arg.find(lt) != string::npos) {
        if (lTimeFlag == 1) {
            cout << "Least Time flag true" << endl;
            return true;
        }
        else {
            cout << "Least Time flag was false" << endl;
            return false;
        }
    }
    else if (arg.find(bd) != string::npos) {
        //cout << "Compare: " << bDayFlag << " == " << "1" << endl;
        if (bDayFlag == 1) {
            cout << "Busiest Day flag true" << endl;
            return true;
        }
        else {
            cout << "Busiest day flag was false" << endl;
            return false;
        }
    }
    else if (arg.find(ml) != string::npos) {
        if (mLocFlag == 1) {
            cout << "Most Loc flag true" << endl;
            return true;
        }
        else {
            cout << "Most Loc flag was false" << endl;
            return false;
        }
    }
    else if (arg.find(ll) != string::npos) {
        if (lLocFlag == 1) {
            cout << "Least Time flag true" << endl;
            return true;
        }
        else {
            cout << "Least Loc flag was false" << endl;
            return false;
        }
    }
    else {
        cout << "Every flag was false" << endl;
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

void parseClient(string buf, Storage& csvData, vector<vector<string>> foilStore, vector<vector<string>>& results) {
    int timeFlag = 0;
    int dateFlag = 0;
    int locationFlag1 = 0;
    int locationFlag2 = 0;
    int baseFlag = 0;
    int mostFlag = 0;
    int leastFlag = 0;
    int searchFlag = 0;
    int deleteFlag = 0;
    int insertFlag = 0;
    //new flags 
    int searchMostLocFlag = 0; //expects "MostLoc"
    int searchLeastLocFlag = 0; //expects "LeastLoc"
    int searchMostTimeFlag = 0; //expects "MostTime"
    int searchLeastTimeFlag = 0; //expects "LeastTime"
    int searchMostDayFlag = 0; //expects "MostDay"
    // 
    int searchMostVehiclesFlag = 0; // expects "MostVehicles"
    int searchLeastVehiclesFlag = 0; // expects "MostVehicles"
    int searchHighestRatioFlag = 0; // expects "HighestRatio"
    int searchLowestRatioFlag = 0; // expects "HighestRatio"
    //
    int searchShortestIntervalFlag = 0; //expects "Shortest Time Interval"
    int searchLongestIntervalFlag = 0; //expects "Longest Time Interval"
    int searchAverageIntervalFlag = 0;


    vector<string> clientDat;
    stringstream X(buf);
    string columns;
    string column;
    vector<string> searchInputs;
    cout <<  buf << endl;
    while (getline(X, columns, ',')) {
        cout << "This is pushed: " << columns << endl;

        if (columns.find("Delete") != string::npos) { //sets flag for what type of request 
            deleteFlag = 1;
        }
        if (columns.find("Insert") != string::npos) {
            insertFlag = 1;
        }
        if (columns.find("Search") != string::npos) {
            searchFlag = 1;
        }
        clientDat.push_back(columns);
    }
    for (unsigned i = 0; i < clientDat.size(); i++) { //sets appropriate flags for populated fields in client's message

        string compare = "Time: ";
        if (clientDat.at(i).find(compare) != string::npos) {
            if (compare != clientDat.at(i)) {
                cout << "Time: " << clientDat.at(i) << endl;
                string::size_type f = clientDat.at(i).find(compare);
                clientDat.at(i).erase(f, compare.length());
                searchInputs.push_back(clientDat.at(i));
                timeFlag = 1;
            }
        }
        compare = "Date: ";
        if (clientDat.at(i).find(compare) != string::npos) {
            if (compare != clientDat.at(i)) {
                cout << "Found Date: " << clientDat.at(i) << endl;
                string::size_type f = clientDat.at(i).find(compare);
                clientDat.at(i).erase(f, compare.length());
                searchInputs.push_back(clientDat.at(i));
                dateFlag = 1;
            }
        }
        compare = "Base: ";
        if (clientDat.at(i).find(compare) != string::npos) {
            if (compare != clientDat.at(i)) {
                cout << "Found Base: " << clientDat.at(i) << endl;
                string::size_type f = clientDat.at(i).find(compare);
                clientDat.at(i).erase(f, compare.length());
                searchInputs.push_back(clientDat.at(i));
                baseFlag = 1;
            }
        }
        compare = "Latitude: ";
        if (clientDat.at(i).find(compare) != string::npos) {
            if (compare != clientDat.at(i)) {
                cout << "Found latitude: " << clientDat.at(i) << endl;
                string::size_type f = clientDat.at(i).find(compare);
                clientDat.at(i).erase(f, compare.length());
                searchInputs.push_back(clientDat.at(i));
                locationFlag1 = 1;
            }
        }
        compare = "Longitude: ";
        if (clientDat.at(i).find(compare) != string::npos) {
            if (compare != clientDat.at(i)) {
                cout << "Found longitde: " << clientDat.at(i) << endl;
                string::size_type f = clientDat.at(i).find(compare);
                clientDat.at(i).erase(f, compare.length());
                searchInputs.push_back(clientDat.at(i));
                locationFlag2 = 1;
            }
        }
        compare = "MostLoc: ";
        if (clientDat.at(i).find(compare) != string::npos) {
            if (compare != clientDat.at(i)) {
                cout << "Found MostLoc: " << clientDat.at(i) << endl;
                string::size_type f = clientDat.at(i).find(compare);
                clientDat.at(i).erase(f, compare.length());
                searchInputs.push_back(clientDat.at(i));
                searchMostLocFlag = 1;
            }
        }
        compare = "LeastLoc: ";
        if (clientDat.at(i).find(compare) != string::npos) {
            if (compare != clientDat.at(i)) {
                cout << "Found LeastLoc: " << clientDat.at(i) << endl;
                string::size_type f = clientDat.at(i).find(compare);
                clientDat.at(i).erase(f, compare.length());
                searchInputs.push_back(clientDat.at(i));
                searchLeastLocFlag = 1;
            }
        }
        compare = "Mosttime: ";
        if (clientDat.at(i).find(compare) != string::npos) {
            if (compare != clientDat.at(i)) {
                cout << "Found MostTime: " << clientDat.at(i) << endl;
                string::size_type f = clientDat.at(i).find(compare);
                clientDat.at(i).erase(f, compare.length());
                searchInputs.push_back(clientDat.at(i));
                searchMostTimeFlag = 1;
            }
        }
        compare = "Leasttime: ";
        if (clientDat.at(i).find(compare) != string::npos) {
            if (compare != clientDat.at(i)) {
                cout << "Found LeastTime: " << clientDat.at(i) << endl;
                string::size_type f = clientDat.at(i).find(compare);
                clientDat.at(i).erase(f, compare.length());
                searchInputs.push_back(clientDat.at(i));
                searchLeastTimeFlag = 1;
            }
        }
        compare = "MostDay: ";
        if (clientDat.at(i).find(compare) != string::npos) {
            if (compare != clientDat.at(i)) {
                cout << "Found MostDay: " << clientDat.at(i) << endl;
                string::size_type f = clientDat.at(i).find(compare);
                clientDat.at(i).erase(f, compare.length());
                searchInputs.push_back(clientDat.at(i));
                searchMostDayFlag = 1;
            }
        }
        compare = "MostVehicles:";
        if (clientDat.at(i).find(compare) != string::npos) {
            if (compare != clientDat.at(i)) {
                cout << "Found MostVehicles: " << clientDat.at(i) << endl;
                string::size_type f = clientDat.at(i).find(compare);
                clientDat.at(i).erase(f, compare.length());
                searchInputs.push_back(clientDat.at(i));
                searchMostVehiclesFlag = 1;
            }
        }
        compare = "LeastVehicles:";
        if (clientDat.at(i).find(compare) != string::npos) {
            if (compare != clientDat.at(i)) {
                cout << "Found LeastVehicles: " << clientDat.at(i) << endl;
                string::size_type f = clientDat.at(i).find(compare);
                clientDat.at(i).erase(f, compare.length());
                searchInputs.push_back(clientDat.at(i));
                searchLeastVehiclesFlag = 1;
            }
        }
        compare = "HighestRatio:";
        if (clientDat.at(i).find(compare) != string::npos) {
            if (compare != clientDat.at(i)) {
                cout << "Found HighestRatio: " << clientDat.at(i) << endl;
                string::size_type f = clientDat.at(i).find(compare);
                clientDat.at(i).erase(f, compare.length());
                searchInputs.push_back(clientDat.at(i));
                searchHighestRatioFlag = 1;
            }
        }
        compare = "LowestRatio:";
        if (clientDat.at(i).find(compare) != string::npos) {
            if (compare != clientDat.at(i)) {
                cout << "Found LowestRatio: " << clientDat.at(i) << endl;
                string::size_type f = clientDat.at(i).find(compare);
                clientDat.at(i).erase(f, compare.length());
                searchInputs.push_back(clientDat.at(i));
                searchLowestRatioFlag = 1;
            }
        }
        compare = "Shortest: ";
        if (clientDat.at(i).find(compare) != string::npos) {
            if (compare != clientDat.at(i)) {
                cout << "Found Shortest Interval: " << clientDat.at(i) << endl;
                string::size_type f = clientDat.at(i).find(compare);
                clientDat.at(i).erase(f, compare.length());
                searchInputs.push_back(clientDat.at(i));
                searchShortestIntervalFlag = 1;
            }
        }
        compare = "Longest: ";
        if (clientDat.at(i).find(compare) != string::npos) {
            if (compare != clientDat.at(i)) {
                cout << "Found Longest Interval: " << clientDat.at(i) << endl;
                string::size_type f = clientDat.at(i).find(compare);
                clientDat.at(i).erase(f, compare.length());
                searchInputs.push_back(clientDat.at(i));
                searchLongestIntervalFlag = 1;
            }
        }
        compare = "Average: ";
        if (clientDat.at(i).find(compare) != string::npos) {
            if (compare != clientDat.at(i)) {
                cout << "Found Average Interval: " << clientDat.at(i) << endl;
                string::size_type f = clientDat.at(i).find(compare);
                clientDat.at(i).erase(f, compare.length());
                searchInputs.push_back(clientDat.at(i));
                searchAverageIntervalFlag = 1;
            }
        }
    }

    if (searchFlag) { //if searchFlag was set: Different search methods
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
        else if (locationFlag1 && locationFlag2 && searchAverageIntervalFlag) {
            calculateAverageInterval(results, csvData, searchInputs);
        }
        else if (locationFlag1 && locationFlag2 && searchShortestIntervalFlag) {
            calculateShortestInterval(results, csvData, searchInputs);
        }
        else if (locationFlag1 && locationFlag2 && searchLongestIntervalFlag) {
            calculateLongestInterval(results, csvData, searchInputs);
        }
        else if (locationFlag1 && locationFlag2) {
            searchLocation(results, csvData, searchInputs);
        }
        else if (baseFlag) {
            searchBase(results, csvData, searchInputs);
        }
        else if (searchMostLocFlag) { //searches for artifact 5
            searchMostLoc(results, csvData, searchInputs);
            //searchLeastLoc(results, csvData, searchInputs);
        }
        else if (searchLeastLocFlag) {
            searchLeastLoc(results, csvData, searchInputs);
        }
        else if (searchMostTimeFlag) {
            searchMostUseTime(results, csvData, searchInputs);
        }
        else if (searchLeastTimeFlag) {
            searchLeastUseTime(results, csvData, searchInputs);
        }
        else if (searchMostDayFlag) {
            calculateBusiestDay(results, csvData, searchInputs);
        }
        else if (searchMostVehiclesFlag)
        {
            searchMostVehicles(results, foilStore, searchInputs);
        }
        else if (searchLeastVehiclesFlag)
        {
            searchLeastVehicles(results, foilStore, searchInputs);
        }
        else if (searchHighestRatioFlag)
        {
            searchHighestRatio(results, foilStore, searchInputs);
        }
        else if (searchLowestRatioFlag)
        {
            searchLowestRatio(results, foilStore, searchInputs);
        }
    }
    else if (deleteFlag) { // if deleteFlag was set: delete with different methods
        if (timeFlag && dateFlag && locationFlag1 && locationFlag2 && baseFlag) {
            deleteSpecific(results, csvData, searchInputs);
        }
        else if (timeFlag && dateFlag) {
            deleteTimeAndDate(results, csvData, searchInputs);
        }
        else if (timeFlag) {
            deleteTime(results, csvData, searchInputs);
        }
        else if (dateFlag) {
            deleteDate(results, csvData, searchInputs);
        }
        else if (locationFlag1 && locationFlag2) {
            deleteLocation(results, csvData, searchInputs);
        }
        else if (baseFlag) {
            deleteBase(results, csvData, searchInputs);
        }
    }
    else if (insertFlag) { // if insertFlag was set: insert new data with all fields
        insertData(results, csvData, searchInputs);
    }

    return;
};
