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

        data1.insertRow(*row1);    //Use row1 object is created, and added to the Storage data1 object

        if (index % 100000 == 0) {

            cout << index << endl;
        }

        index++;
    }
    cout << "5/5 DONE" << endl;
    fin.close();
    return;
};

void readFOIL(Storage& data1, string filename, vector<vector <string>>& foilStore)
{
    ifstream fin;
    string line1;
    string line2;

    // Open an existing file
    fin.open(filename);
    //int index = 0;

    cout << "Parsing..." << endl;
    Parsed* obj;

    while (!fin.eof()) {
        fin >> line1;   //These two are to format the first column of the csv file

        vector<string> results;
        parseLine(line1, results);


       /* if (index % 10000 == 0) {

            cout << index << endl;
        }*/

        foilStore.push_back(results);
        //index++;
    }
    cout << "1/5 DONE" << endl;
    return;
};

void readCsvFiles(Storage& data1, Storage& data2, string filename1, string filename2, string filename3) { //parsed both files for app comparison
    //cout << "We just entered the function" << endl;
    ifstream fin;

    string line1;
    string line2;

    // Open an existing file
    fin.open(filename2);
    //int index = 0;

    Parsed* obj;
    Use* row1;
    //cout << "Parsing multiple files..." << endl;
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

        data1.insertRow(*row1);    //Use row1 object is created, and added to the Storage data1 object

        /*if (index % 10000 == 0) {

            cout << index << endl;
        }*/

        //index++;
    }

    fin.close();
    cout << "2/5 DONE" << endl;
    fin.open(filename1);
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

        data1.insertRow(*row1);    //Use row1 object is created, and added to the Storage data1 object

        /*if (index % 10000 == 0) {

            cout << index << endl;
        }*/

        //index++;
    }

    fin.close();



    // Open an existing file
    cout << "3/5 DONE" << endl;
    fin.open(filename3);
    //int index2 = 0;

    while (!fin.eof()) {
        fin >> line1;   //These two are to format the first column of the csv file
        fin >> line2;
        line1 = line1 + " " + line2; // combines the date column string with the rest of its corresponding row "4/1/2014 + 0:11:00",40.769,-73.9549,

        obj = new Parsed;
        obj->parseData2(line1); // this will parse all the fields
        obj->parseHours(obj->retTime());
        obj->parseMonth(obj->retDate());
        obj->convertToDay();
        row1 = new Use;

        row1->setTime(obj->retTime());
        row1->setDate(obj->retDate());
        row1->setLong(obj->retLongt());
        row1->setLat(obj->retLat());
        row1->setBase("B02510");
        row1->setDay(obj->retDay());
        row1->setHour(obj->retHour());
        row1->setMin(obj->retMin());
        row1->setExists(true);

        data2.insertRow(*row1);

        /*if (index2 % 10000 == 0) {

            cout << index2 << endl;
        }*/

        //index2++;
    }
    fin.close();
    cout << "4/5 DONE" << endl;
    return;
}



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
    return;
};

void Parsed::parseData2(string arg) { //parses data for Lyft csv file
    vector<string> results;
    string temp;
    string temp1;

    parseLine(arg, results); //parses arg int vector with elements: Date Time, Lat, Lon,

    lat = results.at(1); //sets object variables
    longt = results.at(2);
    //base = results.at(3);

    temp = results.at(0);
    results.clear();
    parseLine1(temp, results); //parses date and time by space => "Date and Time" >>still has quotes
    temp = results.at(0); //Date

    temp1 = results.at(1); // Time


    date = temp;
    time = temp1;
    results.clear();
    return;
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
    return;
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
    return;
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
    return;
}; //maps the date XX/XX/XXXXX to appropriate day: figure out algorithm with days of each month for 2014,2015
