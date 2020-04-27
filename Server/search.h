#pragma once
#include <iostream>
#include<fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

void parseLine(string line, vector<string>& results, int index);
void read(vector<vector<string>>& result);

void searchTime(vector<vector<string>>& results, vector<vector<string>>& csvData, vector<string>& searchInputs);
void searchDate(vector<vector<string>>& results, vector<vector<string>>& csvData, vector<string>& searchInputs);
void searchDateTime(vector<vector<string>>& results, vector<vector<string>>& csvData, vector<string>& searchInputs);
void searchLocation(vector<vector<string>>& results, vector<vector<string>>& csvData, vector<string>& searchInputs);
void searchBase(vector<vector<string>>& results, vector<vector<string>>& csvData, vector<string>& searchInputs);
void searchSpecific(vector<vector<string>>& results, vector<vector<string>>& csvData, vector<string>& searchInputs);

void deleteTime(vector<vector<string>>& results, vector<vector<string>>& csvData, vector<string>& searchInputs); // delete with time
void deleteDate(vector<vector<string>>& results, vector<vector<string>>& csvData, vector<string>& searchInputs); // delete with Date
void deleteTimeAndDate(vector<vector<string>>& results, vector<vector<string>>& csvData, vector<string>& searchInputs); // delete with time and date
void deleteLocation(vector<vector<string>>& results, vector<vector<string>>& csvData, vector<string>& searchInputs); // delete with Location
void deleteBase(vector<vector<string>>& results, vector<vector<string>>& csvData, vector<string>& searchInputs); // delete with Base
void deleteSpecific(vector<vector<string>>& results, vector<vector<string>>& csvData, vector<string>& searchInputs); // delete specific

void insertData(vector<vector<string>>& results, vector<vector<string>>& csvData, vector<string>& searchInputs); // insert
void parseClient(char buf[1024], vector<vector<string>>& csvData, vector<vector<string>>& results);
