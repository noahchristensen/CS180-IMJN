#include <iostream>
#include<fstream>
#include <sstream>
#include <string>
#include <vector>
#include "search.h"
using namespace std;



void parseClient(string buf, Storage& csvData, Storage& csvData1, Storage& csvData2, vector<vector<string>> foilStore, vector<vector<string>>& results) {
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
    // app comparison
    int compMostTime = 0; // compares top 10 most used times "MostTCompare"
    int compMostLoc = 0; // compares top 10 most used locations expects a location lat/long
    int compLeastTime = 0; // compares top 10 least times  "LeastTCompare"
    int compLeastLoc = 0; // compares top 10 least used locations  expects location lat/long
    int compareApp = 0; // just finding count for specific day or location expects "Comparison"


    vector<string> clientDat;
    stringstream X(buf);
    string columns;
    string column;
    vector<string> searchInputs;
    cout << buf << endl;
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
        if (columns.find("Comparison") != string::npos) {
            compareApp = 1;
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
        compare = "MostTCompare: ";//for comparison
        if (clientDat.at(i).find(compare) != string::npos) {
            if (compare != clientDat.at(i)) {
                cout << "Found Compare By Most Time: " << clientDat.at(i) << endl;
                string::size_type f = clientDat.at(i).find(compare);
                clientDat.at(i).erase(f, compare.length());
                searchInputs.push_back(clientDat.at(i));
                compMostTime = 1;
            }
        }
        compare = "LeastTCompare: ";
        if (clientDat.at(i).find(compare) != string::npos) {
            if (compare != clientDat.at(i)) {
                cout << "Found Compare By Least Time: " << clientDat.at(i) << endl;
                string::size_type f = clientDat.at(i).find(compare);
                clientDat.at(i).erase(f, compare.length());
                searchInputs.push_back(clientDat.at(i));
                compLeastTime = 1;
            }
        }
        compare = "MostLCompare: ";
        if (clientDat.at(i).find(compare) != string::npos) {
            if (compare != clientDat.at(i)) {
                cout << "Found Compare By Most Location: " << clientDat.at(i) << endl;
                string::size_type f = clientDat.at(i).find(compare);
                clientDat.at(i).erase(f, compare.length());
                searchInputs.push_back(clientDat.at(i));
                compMostLoc = 1;
            }
        }
        compare = "LeastLCompare: ";
        if (clientDat.at(i).find(compare) != string::npos) {
            if (compare != clientDat.at(i)) {
                cout << "Found Compare By Least Location: " << clientDat.at(i) << endl;
                string::size_type f = clientDat.at(i).find(compare);
                clientDat.at(i).erase(f, compare.length());
                searchInputs.push_back(clientDat.at(i));
                compLeastLoc = 1;
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
            cout << "We retruned from delete specific" << endl;
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
    else if (compareApp) {
        if (dateFlag) {
            //count uses for specific date
            compDay(csvData1, csvData2, results, searchInputs); // returns count of uses for day of both apps
        }
        else if (locationFlag1 && locationFlag2) {
            //count uses for a specific location
            compLoc(csvData1, csvData2, results, searchInputs); //returns count of uses for specific location for both apps
        }
        else if (compMostTime) {
            //gets top 10 times
            compareMostUseTime(results, csvData1, csvData2, searchInputs);
        }
        else if (compLeastTime) {
            //gets lowest 10 times
            compareLeastUseTime(results, csvData1, csvData2, searchInputs);
        }
        else if (compMostLoc) {
            //gets top 10 locations
            compareMostLoc(results, csvData1, csvData2, searchInputs);
        }
        else if (compLeastLoc) {
            //gets lowest 10 locations
            compareLeastLoc(results, csvData1, csvData2, searchInputs);
        }
    }

    return;
};
