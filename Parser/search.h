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
void searchDateTime(vector<vector<string>>& results, vector<vector<string>>& csvData, vector<string> & searchInputs);
void searchLocation(vector<vector<string>>& results, vector<vector<string>>& csvData, vector<string> & searchInputs);
void searchBase(vector<vector<string>>& results, vector<vector<string>>& csvData, vector<string>& searchInputs);
void searchSpecific(vector<vector<string>>& results, vector<vector<string>>& csvData, vector<string>& searchInputs);
void parseClient(char buf[1024], vector<vector<string>>& csvData , vector<vector<string>>& results);
