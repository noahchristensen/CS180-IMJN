#include <iostream>
#include<fstream>
#include <sstream>
#include <string>
#include <vector>
#include "search.h"
using namespace std;

void parseLine(string line, vector<string>& results){// parses each row from the csv file
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



void read(Storage & data1, string filename) 
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
        row1 = new Use;
        
        row1->setTime(obj->retTime());
        row1->setDate(obj->retDate());
        row1->setLong(obj->retLongt());
        row1->setLat(obj->retLat());
        row1->setBase(obj->retBase());
        row1->setExists(true);
        cout << row1->getDate() << endl;
        //row1.setDay(obj.retDay())
        data1.insertRow(*row1);    //Use row1 object is created, and added to the Storage data1 object

        //parseLine(line1, results, index); // parses line1 to results vector with four elements:  "Date Time", Lat, Lon, "Base",
        //result.push_back(results); //pushes results vector to result vector
        if (index % 10000 == 0) {
            cout << "just pushed " << index << endl;
        }

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
    data[index].clearUse();
    size--;
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
            if (i == size2 -1) {
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
    //parse XX/XX/XXXX set mth1/d1/yr1 FIXME
};

void Parsed::parseHours(string arg) {
    // parse 00:00:00 set hr1/min1/sec1
}; 

//void Parsed::parseTime(string arg) {
//
//};
//
//void Parsed::parseLongt(string arg) {
//
//};
//
//void Parsed::parseLat(string arg) {
//
//};
//
//void Parsed::parseBase(string arg) {
//
//};

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

void Parsed::convertToDay(string arg) {
    //FIXME
}; //maps the date XX/XX/XXXXX to appropriate day: figure out algorithm with days of each month for 2014



void searchDate(vector<vector<string>>& results, Storage& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;
    //string temp;
    for (unsigned int k = 0; k < csvData.getOrigSize(); k++) {// searches for date XX/XX/XXXX 
        //miniVec = csvData.at(k);
        if (csvData.getRow(k).getDate().find(searchInputs.at(0)) != string::npos) {
            miniVec.at(0) = "\"" + csvData.getRow(k).getDate() + " " + csvData.getRow(k).getTime() + "\"";
            miniVec.at(1) = csvData.getRow(k).getLat();
            miniVec.at(2) = csvData.getRow(k).getLong();
            miniVec.at(3) = csvData.getRow(k).getBase();
            results.push_back(miniVec);
        }
    }
};

void searchTime(vector<vector<string>>& results, Storage& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;

    searchInputs.at(0) = " " + searchInputs.at(0) + ":00";
    for (unsigned int k = 0; k < csvData.getOrigSize(); k++) {// searches for time 00:00
        //miniVec = csvData.at(k);
        if (csvData.getRow(k).getTime().find(searchInputs.at(0)) != string::npos) {
            miniVec.at(0) = "\"" + csvData.getRow(k).getDate() + " " + csvData.getRow(k).getTime() + "\"";
            miniVec.at(1) = csvData.getRow(k).getLat();
            miniVec.at(2) = csvData.getRow(k).getLong();
            miniVec.at(3) = csvData.getRow(k).getBase();
            results.push_back(miniVec); //pushes found searches to results
        }
    }
};

void searchDateTime(vector<vector<string>>& results, Storage& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;
    //string dateAndTime = "\"" + searchInputs.at(1) + " " + searchInputs.at(0) + ":00" + "\"";
    for (unsigned int k = 0; k < csvData.getOrigSize(); k++) { // searches for time and day "XX/XX/XXXX 00:00:00" 
        //miniVec = csvData.at(k);
        if (searchInputs.at(1).compare(csvData.getRow(k).getDate()) == 0 && searchInputs.at(0).compare(csvData.getRow(k).getTime()) == 0) {
            miniVec.at(0) = "\"" + csvData.getRow(k).getDate() + " " + csvData.getRow(k).getTime() + "\"";
            miniVec.at(1) = csvData.getRow(k).getLat();
            miniVec.at(2) = csvData.getRow(k).getLong();
            miniVec.at(3) = csvData.getRow(k).getBase();
            results.push_back(miniVec); //pushes found searches to results
        }
    }
};

void searchLocation(vector<vector<string>>& results, Storage& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;
    for (unsigned int k = 0; k < csvData.getOrigSize(); k++) {// searches for Location: lon/lat 
        //miniVec = csvData.at(k);
        if (searchInputs.at(0).compare(csvData.getRow(k).getLat()) == 0 && searchInputs.at(1).compare(csvData.getRow(k).getLat()) == 0) {
            miniVec.at(0) = "\"" + csvData.getRow(k).getDate() + " " + csvData.getRow(k).getTime() + "\"";
            miniVec.at(1) = csvData.getRow(k).getLat();
            miniVec.at(2) = csvData.getRow(k).getLong();
            miniVec.at(3) = csvData.getRow(k).getBase();
            results.push_back(miniVec); //pushes found searches to results
        }
    }
};

void searchBase(vector<vector<string>>& results, Storage& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;
    searchInputs.at(0) = "\"" + searchInputs.at(0) + "\"";
    for (unsigned int k = 0; k < csvData.getOrigSize(); k++) {// searches for Base#: BXXXXX
        //miniVec = csvData.at(k);
        if (searchInputs.at(0).compare(csvData.getRow(k).getBase()) == 0) {
            miniVec.at(0) = "\"" + csvData.getRow(k).getDate() + " " + csvData.getRow(k).getTime() + "\"";
            miniVec.at(1) = csvData.getRow(k).getLat();
            miniVec.at(2) = csvData.getRow(k).getLong();
            miniVec.at(3) = csvData.getRow(k).getBase();
            results.push_back(miniVec); //pushes found searches to results
        }
    }
};

void searchSpecific(vector<vector<string>>& results, Storage& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;
    string temp;
    
    searchInputs.at(4) = "\"" + searchInputs.at(4) + "\"";
    string dateAndTime = "\"" + searchInputs.at(1) + " " + searchInputs.at(0) + ":00" + "\"";
    for (unsigned int k = 0; k < csvData.getOrigSize(); k++) { //
        temp = "\"" + csvData.getRow(k).getDate() + " " + csvData.getRow(k).getTime() + "\"";
        if (dateAndTime.compare(temp) == 0 && searchInputs.at(2).compare(csvData.getRow(k).getLat()) == 0 && searchInputs.at(3).compare(csvData.getRow(k).getLong()) == 0 && searchInputs.at(4).compare(csvData.getRow(k).getBase()) == 0) {
            miniVec.at(0) = "\"" + csvData.getRow(k).getDate() + " " + csvData.getRow(k).getTime() + "\"";
            miniVec.at(1) = csvData.getRow(k).getLat();
            miniVec.at(2) = csvData.getRow(k).getLong();
            miniVec.at(3) = csvData.getRow(k).getBase();
            results.push_back(miniVec); //pushes found searches to results
        }
    }
};

void deleteTime(vector<vector<string>>& results, Storage& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;
    searchInputs.at(0) = " " + searchInputs.at(0) + ":00";
    for (unsigned int k = 0; k < csvData.getOrigSize(); k++) { // searches for time 00:00
        //miniVec = csvData.at(k);
        if ((csvData.getRow(k).getTime().find(searchInputs.at(0)) != string::npos)) { //the time is found so we delete this element
            //results.push_back(miniVec); ////pushes elements not satisfying the search into results vector
            csvData.deleteRow(k);
        }
    }//results and cvsData should be equal
    csvData.updateDelData();

    //csvData.clear(); // clears old vector
    //csvData = results; // copies new vector with removed elements to original
};

void deleteDate(vector<vector<string>>& results, Storage& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;
    for (unsigned int k = 0; k < csvData.getOrigSize(); k++) { // searches for date XX/XX/XXXX 
        //miniVec = csvData.at(k);
        if (csvData.getRow(k).getDate().find(searchInputs.at(0)) != string::npos) { //if search element is found it is deleted
            csvData.deleteRow(k);
        }
    }//results and cvsData should be equal
    //csvData.clear();// clears old vector
    //csvData = results;// copies new vector with removed elements to original
    csvData.updateDelData();
};

void deleteTimeAndDate(vector<vector<string>>& results, Storage& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;
    string temp;
    string dateAndTime = "\"" + searchInputs.at(1) + " " + searchInputs.at(0) + ":00" + "\"";
    for (unsigned int k = 0; k < csvData.getOrigSize(); k++) {// searches for time and day "XX/XX/XXXX 00:00:00" 
        //miniVec = csvData.at(k);
        temp = "\"" + csvData.getRow(k).getDate() + " " + csvData.getRow(k).getTime() + "\"";
        if (dateAndTime.compare(temp) == 0) { //delete is equal
            csvData.deleteRow(k);
        }
    }//results and cvsData should be equal
    //csvData.clear();// clears old vector
    //csvData = results;// copies new vector with removed elements to original
    csvData.updateDelData();
};

void deleteLocation(vector<vector<string>>& results, Storage& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;
    for (unsigned int k = 0; k < csvData.getOrigSize(); k++) { // searches for Location: lon/lat 
        //miniVec = csvData.at(k);
        if ((searchInputs.at(0).compare(csvData.getRow(k).getLat()) == 0 && searchInputs.at(1).compare(csvData.getRow(k).getLong()) == 0)) {
            //results.push_back(miniVec);
            csvData.deleteRow(k);
        }
    }//results and cvsData should be equal
    //csvData.clear();// clears old vector
    //csvData = results;// copies new vector with removed elements to original
    csvData.updateDelData();
};

void deleteBase(vector<vector<string>>& results, Storage& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;
    searchInputs.at(0) = "\"" + searchInputs.at(0) + "\"";
    for (unsigned int k = 0; k < csvData.getOrigSize(); k++) {// searches for Base#: BXXXXX 
        //miniVec = csvData.at(k);
        if ((searchInputs.at(0).compare(csvData.getRow(k).getBase()) == 0)) {
            //results.push_back(miniVec);
            csvData.deleteRow(k);
        }
    }//results and cvsData should be equal
    //csvData.clear();// clears old vector
    //csvData = results;// copies new vector with removed elements to original
    csvData.updateDelData();
};

void deleteSpecific(vector<vector<string>>& results, Storage& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;
    string temp;
    searchInputs.at(4) = "\"" + searchInputs.at(4) + "\"";
    string dateAndTime = "\"" + searchInputs.at(1) + " " + searchInputs.at(0) + ":00" + "\"";
    for (unsigned int k = 0; k < csvData.getOrigSize(); k++) {         // searches for Location: lon/lat 
        //miniVec = csvData.at(k);
        temp = "\"" + csvData.getRow(k).getDate() + " " + csvData.getRow(k).getTime() + "\"";
        if ((dateAndTime.compare(temp) == 0 && searchInputs.at(2).compare(csvData.getRow(k).getLat()) == 0 && searchInputs.at(3).compare(csvData.getRow(k).getLong()) == 0 && searchInputs.at(4).compare(csvData.getRow(k).getBase()) == 0)) {
            //results.push_back(miniVec);
            csvData.deleteRow(k);
        }
    }//results and cvsData should be equal
    //csvData.clear();// clears old vector
    //csvData = results;// copies new vector with removed elements to original
    csvData.updateDelData();
};

void insertData(vector<vector<string>>& results, Storage& csvData, vector<string>& insertInputs) { // pushes new data to end of csvData vector
    //vector<string> miniVec;
    insertInputs.at(4) = "\"" + insertInputs.at(4) + "\"";
    //string dateAndTime = "\"" + insertInputs.at(1) + " " + insertInputs.at(0) + ":00" + "\"";
    //miniVec.push_back(dateAndTime);
    //miniVec.push_back(insertInputs.at(2));
    //miniVec.push_back(insertInputs.at(3));
    //miniVec.push_back(insertInputs.at(4));
    //csvData.insertRow()push_back(miniVec);
    string temp;
    temp = insertInputs.at(0) + ":00";
    Use obj1; //implement set Day Monday -Sunday
    obj1.setTime(temp);
    obj1.setDate(insertInputs.at(1));
    obj1.setLat(insertInputs.at(2));
    obj1.setLong(insertInputs.at(3));
    obj1.setExists(true);
    obj1.setBase(insertInputs.at(4));

    csvData.insertRow(obj1);

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

void parseClient(string buf, Storage & csvData, vector<vector<string>>& results) {
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
    vector<string> clientDat;
    stringstream X(buf);
    string columns;
    string column;
    vector<string> searchInputs;

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
        else if (locationFlag1 && locationFlag2) {
            searchLocation(results, csvData, searchInputs);
        }
        else if (baseFlag) {
            searchBase(results, csvData, searchInputs);
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
