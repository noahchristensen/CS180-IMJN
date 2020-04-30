#include <iostream>
#include<fstream>
#include <sstream>
#include <string>
#include <vector>
#include "search.h"
using namespace std;

void parseLine(string line, vector<string>& results, int index) { // parses each row from the csv file
    stringstream X(line);
    string column;
    vector<string> results1;

    while (getline(X, column, ',')) { //splits line into columns 
        results1.push_back(column);
    }

    results = results1;
    return;
}

void read(vector<vector<string>>& result, string filename)
{
    ifstream fin;
    string line1;
    string line2;

    // Open an existing file
    fin.open(filename);
    int index = 0;
    vector<string> results;
    cout << "we are about to open file to parse" << endl;
    while (!fin.eof()) {
        fin >> line1;   //These two are to format the first column of the csv file
        fin >> line2;
        line1 = line1 + " " + line2; // combines the date column string with the rest of its corresponding row "4/1/2014 + 0:11:00",40.769,-73.9549,"B02512"
        parseLine(line1, results, index); // parses line1 to results vector with four elements:  "Date Time", Lat, Lon, "Base",
        result.push_back(results); //pushes results vector to result vector
        if (index % 10000 == 0) {
            cout << "just pushed " << index << endl;
        }

        index++;
    }
    return;
};

void searchDate(vector<vector<string>>& results, vector<vector<string>>& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;
    for (unsigned int k = 0; k < csvData.size(); k++) {// searches for date XX/XX/XXXX 
        miniVec = csvData.at(k);
        if (miniVec.at(0).find(searchInputs.at(0)) != string::npos) {
            results.push_back(miniVec);
        }
    }
};

void searchTime(vector<vector<string>>& results, vector<vector<string>>& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;
    searchInputs.at(0) = " " + searchInputs.at(0) + ":00";
    for (unsigned int k = 0; k < csvData.size(); k++) {// searches for time 00:00
        miniVec = csvData.at(k);
        if (miniVec.at(0).find(searchInputs.at(0)) != string::npos) {
            results.push_back(miniVec); //pushes found searches to results
        }
    }
};

void searchDateTime(vector<vector<string>>& results, vector<vector<string>>& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;
    string dateAndTime = "\"" + searchInputs.at(1) + " " + searchInputs.at(0) + ":00" + "\"";
    for (unsigned int k = 0; k < csvData.size(); k++) { // searches for time and day "XX/XX/XXXX 00:00:00" 
        miniVec = csvData.at(k);
        if (dateAndTime.compare(miniVec.at(0)) == 0) {
            results.push_back(miniVec); //pushes found searches to results
        }
    }
};

void searchLocation(vector<vector<string>>& results, vector<vector<string>>& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;
    for (unsigned int k = 0; k < csvData.size(); k++) {// searches for Location: lon/lat 
        miniVec = csvData.at(k);
        if (searchInputs.at(0).compare(miniVec.at(1)) == 0 && searchInputs.at(1).compare(miniVec.at(2)) == 0) {
            results.push_back(miniVec); //pushes found searches to results
        }
    }
};

void searchBase(vector<vector<string>>& results, vector<vector<string>>& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;
    searchInputs.at(0) = "\"" + searchInputs.at(0) + "\"";
    for (unsigned int k = 0; k < csvData.size(); k++) {// searches for Base#: BXXXXX
        miniVec = csvData.at(k);
        if (searchInputs.at(0).compare(miniVec.at(3)) == 0) {
            results.push_back(miniVec); //pushes found searches to results
        }
    }
};

void searchSpecific(vector<vector<string>>& results, vector<vector<string>>& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;
    searchInputs.at(4) = "\"" + searchInputs.at(4) + "\"";
    string dateAndTime = "\"" + searchInputs.at(1) + " " + searchInputs.at(0) + ":00" + "\"";
    for (unsigned int k = 0; k < csvData.size(); k++) { // searches for Location: lon/lat 
        miniVec = csvData.at(k);
        if (dateAndTime.compare(miniVec.at(0)) == 0 && searchInputs.at(2).compare(miniVec.at(1)) == 0 && searchInputs.at(3).compare(miniVec.at(2)) == 0 && searchInputs.at(4).compare(miniVec.at(3)) == 0) {
            results.push_back(miniVec); //pushes found searches to results
        }
    }
};

void deleteTime(vector<vector<string>>& results, vector<vector<string>>& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;
    searchInputs.at(0) = " " + searchInputs.at(0) + ":00";
    for (unsigned int k = 0; k < csvData.size(); k++) { // searches for time 00:00
        miniVec = csvData.at(k);
        if (!(miniVec.at(0).find(searchInputs.at(0)) != string::npos)) { //the time is not found so we push this element
            results.push_back(miniVec); ////pushes elements not satisfying the search into results vector
        }
    }//results and cvsData should be equal
    csvData.clear(); // clears old vector
    csvData = results; // copies new vector with removed elements to original
};

void deleteDate(vector<vector<string>>& results, vector<vector<string>>& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;
    for (unsigned int k = 0; k < csvData.size(); k++) { // searches for date XX/XX/XXXX 
        miniVec = csvData.at(k);
        if (!(miniVec.at(0).find(searchInputs.at(0)) != string::npos)) { //if search element is found it is not copied into result vector
            results.push_back(miniVec);
        }
    }//results and cvsData should be equal
    csvData.clear();// clears old vector
    csvData = results;// copies new vector with removed elements to original
};

void deleteTimeAndDate(vector<vector<string>>& results, vector<vector<string>>& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;
    string dateAndTime = "\"" + searchInputs.at(1) + " " + searchInputs.at(0) + ":00" + "\"";
    for (unsigned int k = 0; k < csvData.size(); k++) {// searches for time and day "XX/XX/XXXX 00:00:00" 
        miniVec = csvData.at(k);
        if (!(dateAndTime.compare(miniVec.at(0)) == 0)) {
            results.push_back(miniVec);
        }
    }//results and cvsData should be equal
    csvData.clear();// clears old vector
    csvData = results;// copies new vector with removed elements to original
};

void deleteLocation(vector<vector<string>>& results, vector<vector<string>>& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;
    for (unsigned int k = 0; k < csvData.size(); k++) { // searches for Location: lon/lat 
        miniVec = csvData.at(k);
        if (!(searchInputs.at(0).compare(miniVec.at(1)) == 0 && searchInputs.at(1).compare(miniVec.at(2)) == 0)) {
            results.push_back(miniVec);
        }
    }//results and cvsData should be equal
    csvData.clear();// clears old vector
    csvData = results;// copies new vector with removed elements to original
};

void deleteBase(vector<vector<string>>& results, vector<vector<string>>& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;
    searchInputs.at(0) = "\"" + searchInputs.at(0) + "\"";
    for (unsigned int k = 0; k < csvData.size(); k++) {// searches for Base#: BXXXXX 
        miniVec = csvData.at(k);
        if (!(searchInputs.at(0).compare(miniVec.at(3)) == 0)) {
            results.push_back(miniVec);
        }
    }//results and cvsData should be equal
    csvData.clear();// clears old vector
    csvData = results;// copies new vector with removed elements to original
};

void deleteSpecific(vector<vector<string>>& results, vector<vector<string>>& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;
    searchInputs.at(4) = "\"" + searchInputs.at(4) + "\"";
    string dateAndTime = "\"" + searchInputs.at(1) + " " + searchInputs.at(0) + ":00" + "\"";
    for (unsigned int k = 0; k < csvData.size(); k++) {         // searches for Location: lon/lat 
        miniVec = csvData.at(k);
        if (!(dateAndTime.compare(miniVec.at(0)) == 0 && searchInputs.at(2).compare(miniVec.at(1)) == 0 && searchInputs.at(3).compare(miniVec.at(2)) == 0 && searchInputs.at(4).compare(miniVec.at(3)) == 0)) {
            results.push_back(miniVec);
        }
    }//results and cvsData should be equal
    csvData.clear();// clears old vector
    csvData = results;// copies new vector with removed elements to original
};

void insertData(vector<vector<string>>& results, vector<vector<string>>& csvData, vector<string>& insertInputs) { // pushes new data to end of csvData vector
    vector<string> miniVec;
    insertInputs.at(4) = "\"" + insertInputs.at(4) + "\"";
    string dateAndTime = "\"" + insertInputs.at(1) + " " + insertInputs.at(0) + ":00" + "\"";
    miniVec.push_back(dateAndTime);
    miniVec.push_back(insertInputs.at(2));
    miniVec.push_back(insertInputs.at(3));
    miniVec.push_back(insertInputs.at(4));
    csvData.push_back(miniVec);


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

void parseClient(string buf, vector<vector<string>>& csvData, vector<vector<string>>& results) {
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