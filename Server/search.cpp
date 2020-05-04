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
        cout << row1->getDay() << endl;
        //cout << row1->getDate() << endl;
        //row1.setDay(obj.retDay())
        data1.insertRow(*row1);    //Use row1 object is created, and added to the Storage data1 object

        //parseLine(line1, results, index); // parses line1 to results vector with four elements:  "Date Time", Lat, Lon, "Base",
        //result.push_back(results); //pushes results vector to result vector
        if (index % 1 == 0) {
            //cout << "just pushed " << index << endl;
            cout << index << endl;
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
    vector <int> monthDays = { 31,28,31,30,31,30,31,31,30,31,30,31 }; //365 total days first day starts Wednesday last day Wednesday
    int month = std::stoi(mth1); // 1-12
    int day1 = std::stoi(d1); //1-28,30,31
    int totDays = 0;
    string dayOfWeek = "";

    for (int i = 0;i < month; i++) {//calculates range of days out of 365
        if (i == month - 1) {
            totDays = day1 + totDays;
        }
        else {
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
    
}; //maps the date XX/XX/XXXXX to appropriate day: figure out algorithm with days of each month for 2014



void searchDate(vector<vector<string>>& results1, Storage& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;
    Storage *obj = &csvData;
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
        if (searchInputs.at(0).compare(row1->getLat()) == 0 && searchInputs.at(1).compare(row1->getLat()) == 0) {
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
};

void insertData(vector<vector<string>>& results, Storage& csvData, vector<string>& insertInputs) { // pushes new data to end of csvData vector
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
    int max ;
    int index;
    for (int k = 0; k < 10; k++) {//gets top 10 in order
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
        miniVec.push_back(str2);//push back the hour
        miniVec.push_back(str1); //push back the number of occurences for corresponding time - Format of miniVec: 00,15 => hour 00 has 15 occurences
        results1.push_back(miniVec);
        miniVec.clear();
    }
    

};

void searchLeastUseTime(vector<vector<string>>& results1, Storage& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;
    Storage* obj = &csvData;
    string in1;
    int arr[24] = { 0 }; //for each hour of day 
    int count = 0;

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
    for (int k = 0; k < 10; k++) {//gets top 10 in order
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
        miniVec.push_back(str2);//push back the hour
        miniVec.push_back(str1); //push back the number of occurences for corresponding time - Format of miniVec: 00,15 => hour 00 has 15 occurences
        results1.push_back(miniVec);
        miniVec.clear();
    }


};

void searchMostLoc(vector<vector<string>>& results1, Storage& csvData, vector<string>& searchInputs) { //FIXME
    vector<string> miniVec; 
    vector<string> temp1;
    Storage* obj = &csvData;
    string in1;
    string in2;
    int arr1[20000] = { 0 };//adding both XXXX and YYYY wouldnt exceed this
    vector<int> eachEntry; // will contain the added long and lat value, lat, long : 3 elements
    vector<vector<int>> allData; //will contain each entry
    int count = 0;
    int lt_val;
    int ln_val;
    int compVal;

    for (unsigned int k = 0; k < obj->getOrigSize(); k++) {//counts 
        pair<int, vector<int>>pairs;
        Use* row1 = new Use;
        *row1 = obj->getRow(k);
        in1 = row1->getLat(); //XX.XXX
        in2 = row1->getLong();//-YY.YY
        //FIXME make this compatible with longitudes that dont have "-"
        in2 = in2.substr(1, in2.length()); //now YY.YY
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
            indx++;
        }
        
        in1 = temp1.at(0) + temp1.at(1); //string now XXXX or XXX
        temp1.clear();
        indx = 0;
        stringstream Y(in2);
        while (getline(X, column, '.')) { //splits line into columns 
            if (column.size() == 1 && indx == 1) {//we are in decimal number
                column = column + "0";
            }
            temp1.push_back(column);
            indx++;
        }
        in2 = temp1.at(0) + temp1.at(1); //string now XXXX or XXX
        temp1.clear();

        //in1 and in2 are now XXXX YYYY
        lt_val = std::stoi(in1);
        ln_val = std::stoi(in2);
        compVal = lt_val + ln_val; // ad them and increment its appropriate index
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
    }
    string str1;
    string str2;
    string str3;
    int max;
    int index;
    for (int k = 0; k < 10; k++) {//gets top 10 in order
        max = arr1[0];
        index = 0;
        for (int j = 0; j < 20000; j++) {
            if (arr1[j] > max) {
                index = j; // this is the value of the lon and lat added
                max = arr1[j];
            }
        }//gets largest coubt for particular time
        //now we have to find corresponding coordinates 
        int point = 0;

        while (allData[point][0] != index) {
            point++;
        }
        str1 = to_string(allData[point][1]);
        str2 = to_string(allData[point][2]);
        str3 = to_string(max);
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

        arr1[index] = 0; //set smallest to 1000000 and loop again up to 10 times
        miniVec.push_back(str1);//push back the lat
        miniVec.push_back(str2);//push back the lat
        miniVec.push_back(str3);//push back the number of occurences for corresponding location
        results1.push_back(miniVec);
        miniVec.clear();
    }


};

void searchLeastLoc(vector<vector<string>>& results1, Storage& csvData, vector<string>& searchInputs) { //FIXME
    vector<string> miniVec;
    vector<string> temp1;
    Storage* obj = &csvData;
    string in1;
    string in2;
    int arr1[20000] = { 0 };//adding both XXXX and YYYY wouldnt exceed this
    vector<int> eachEntry; // will contain the added long and lat value, lat, long : 3 elements
    vector<vector<int>> allData; //will contain each entry
    int count = 0;
    int lt_val;
    int ln_val;
    int compVal;

    for (unsigned int k = 0; k < obj->getOrigSize(); k++) {//counts 
        pair<int, vector<int>>pairs;
        Use* row1 = new Use;
        *row1 = obj->getRow(k);
        in1 = row1->getLat(); //XX.XXX
        in2 = row1->getLong();//-YY.YY
        //FIXME make this compatible with longitudes that dont have "-"
        in2 = in2.substr(1, in2.length()); //now YY.YY
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
            indx++;
        }

        in1 = temp1.at(0) + temp1.at(1); //string now XXXX or XXX
        temp1.clear();
        indx = 0;
        stringstream Y(in2);
        while (getline(X, column, '.')) { //splits line into columns 
            if (column.size() == 1 && indx == 1) {//we are in decimal number
                column = column + "0";
            }
            temp1.push_back(column);
            indx++;
        }
        in2 = temp1.at(0) + temp1.at(1); //string now XXXX or XXX
        temp1.clear();

        //in1 and in2 are now XXXX YYYY
        lt_val = std::stoi(in1);
        ln_val = std::stoi(in2);
        compVal = lt_val + ln_val; // ad them and increment its appropriate index
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
    }
    string str1;
    string str2;
    string str3;
    int min;
    int index;
    for (int k = 0; k < 10; k++) {//gets top 10 in order
        min = arr1[0];
        index = 0;
        for (int j = 0; j < 20000; j++) {
            if (arr1[j] < min) {
                index = j; // this is the value of the lon and lat added
                min = arr1[j];
            }
        }//gets largest coubt for particular time
        //now we have to find corresponding coordinates 
        int point = 0;

        while (allData[point][0] != index) {
            point++;
        }
        str1 = to_string(allData[point][1]);
        str2 = to_string(allData[point][2]);
        str3 = to_string(min);
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

        arr1[index] = 1000000; //set smallest to 1000000 and loop again up to 10 times
        miniVec.push_back(str1);//push back the lat
        miniVec.push_back(str2);//push back the lat
        miniVec.push_back(str3);//push back the number of occurences for corresponding location
        results1.push_back(miniVec);
        miniVec.clear();
    }


};

void calculateBusiestDay(vector<vector<string>>& results1, Storage& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;
    Storage* obj = &csvData;
    string in1;
    int arr[7] = { 0 }; //for each day 
    string arr1[7] = { "Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday" }; // each day of week for comparison check
    int count = 0;

    for (unsigned int k = 0; k < obj->getOrigSize(); k++) {//counts 
        Use* row1 = new Use;
        *row1 = obj->getRow(k);

        for (int g = 0;g < 7; g++) {
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
        miniVec.push_back(arr1[index]);//push back the day
        miniVec.push_back(str1); //push back the number of occurences for corresponding day - Format of miniVec: "Monday","234" => Monday has 234 occurences
        results1.push_back(miniVec);
        miniVec.clear();
    }
};

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
   //new flags 
    int searchMostLocFlag = 0; //expects "MostLoc"
    int searchLeastLocFlag = 0; //expects "LeastLoc"
    int searchMostTimeFlag = 0; //expects "MostTime"
    int searchLeastTimeFlag = 0; //expects "LeastTime"
    int searchMostDayFlag = 0; //expects "MostDay"

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
        compare = "MostTime: ";
        if (clientDat.at(i).find(compare) != string::npos) {
            if (compare != clientDat.at(i)) {
                cout << "Found MostTime: " << clientDat.at(i) << endl;
                string::size_type f = clientDat.at(i).find(compare);
                clientDat.at(i).erase(f, compare.length());
                searchInputs.push_back(clientDat.at(i));
                searchMostTimeFlag = 1;
            }
        }
        compare = "LeastTime: ";
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
        else if (searchMostLocFlag) { //searches for artifact 5
            searchMostLoc(results, csvData, searchInputs);
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
