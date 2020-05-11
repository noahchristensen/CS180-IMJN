#pragma once
#include <iostream>
#include<fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;


//Added Classes
class Use {
	string time; // "00:00:00"
	string date; // "XX/XX/XXXX"
	string day; // Monday, Tuesday, etc
	string longitude; // -YY.YYYY
	string latitude; // XX.XXXX
	string base; // BXXXXX
	bool exists; // will be false for a deleted object
	int timeHour;
	int timeMin;
public:
	void setTime(string arg); //sets time
	void setDate(string arg); //sets date
	void setDay(string arg); //sets day: Monday-Sunday
	void setLong(string arg); //sets Longitude
	void setLat(string arg); //sets Latitude
	void setBase(string arg); // sets base
	void setHour(int arg); //sets hour
	void setMin(int arg); //set min
	void setExists(bool arg); //sets exists bool variable for an object  
	string getTime(); // returns time 
	string getDate();// returns date
	string getDay(); // returns day
	string getLong(); // returns longitude
	string getLat(); // returns latitude 
	string getBase(); // returns base#
	int getHour(); //gets hour
	int getMin(); //get min
	bool checkExists(); //checks if data exists- used for updating new storage after deleting
	void clearUse(); // will clear the fields for a Use object 
};

class Storage {
	Use data[1200000]; // largest file will be 1.03 Million entries -set size array size 1.2 mil
	int size = 0; //initial size will be 0
	int size2 = 0; // keeps track of original size
	vector <vector<string>> results;
public:
	void insertRow(Use& row); //inserts row into end of data[] increases size by 1 
	void deleteRow(int index); // clears fields of an index object & sets its exists = false
	void convertStorage(); // done right before sending the client data
	Use& getRow(int index); // returns a Use object from Data[]
	int getSize(); //checks the size of data[]
	int getOrigSize(); //gets original size before deletes 
	void updateDelData(); //copies data and removes any Use objects that have exist = false
	void adjustData(int index);
	vector <vector<string>>& getResults(); // gets vector with results
};

class Parsed {
	string date; // XX/XX/XXXX
	string time; // time 00:00:00
	string longt; //longitude
	string lat; //latitude 
	string base; //base "BXXXXX" includes quotes
	string day; //Monday-Sunday
	string d1;
	string mth1;
	string yr1;
	string hour1;
	string min1;
	string sec1;
public:
	void parseData(string arg);
	void parseMonth(string arg);
	void parseHours(string arg);
	void convertToDay(); //maps the date XX/XX/XXXXX to appropriate day FIXME
	string retDate();
	string retTime();
	string retLongt();
	string retLat();
	string retBase();
	string retDay();
	int retHour();
	int retMin();
};

void parseLine(string line, vector<string>& results); //check where this is called
void read(Storage& data1, string filename); //check where this is called

void searchTime(vector<vector<string>>& results, Storage& csvData, vector<string>& searchInputs);
void searchDate(vector<vector<string>>& results, Storage& csvData, vector<string>& searchInputs);
void searchDateTime(vector<vector<string>>& results, Storage& csvData, vector<string>& searchInputs);
void searchLocation(vector<vector<string>>& results, Storage& csvData, vector<string>& searchInputs);
void searchBase(vector<vector<string>>& results, Storage& csvData, vector<string>& searchInputs);
void searchSpecific(vector<vector<string>>& results, Storage& csvData, vector<string>& searchInputs);

void deleteTime(vector<vector<string>>& results, Storage& csvData, vector<string>& searchInputs); // delete with time
void deleteDate(vector<vector<string>>& results, Storage& csvData, vector<string>& searchInputs); // delete with Date
void deleteTimeAndDate(vector<vector<string>>& results, Storage& csvData, vector<string>& searchInputs); // delete with time and date
void deleteLocation(vector<vector<string>>& results, Storage& csvData, vector<string>& searchInputs); // delete with Location
void deleteBase(vector<vector<string>>& results, Storage& csvData, vector<string>& searchInputs); // delete with Base
void deleteSpecific(vector<vector<string>>& results, Storage& csvData, vector<string>& searchInputs); // delete specific

void insertData(vector<vector<string>>& results, Storage& csvData, vector<string>& searchInputs); // insert
void parseClient(string buf, Storage& csvData, vector<vector<string>>& results);

bool checkIfExport(char buf[1024]); // returns triue if message from client is for import

bool checkIfImport(char buf[1024]); // returns triue if message from client is for import
void importData(char buf[1024], vector<string>& importRows); // Pushes all messages to importRows including "File: raw-uber-data.csv" formatted message. 
void importFunction(vector<string>& importRows);//creates file with corresponding file name from client and adds in all data.
bool importDone(char buf[1024]); //checks message for "Done" 

//new functions artificat 5
void searchMostUseTime(vector<vector<string>>& results, Storage& csvData, vector<string>& searchInputs);
void searchLeastUseTime(vector<vector<string>>& results, Storage& csvData, vector<string>& searchInputs);
void searchMostLoc(vector<vector<string>>& results, Storage& csvData, vector<string>& searchInputs);
void searchLeastLoc(vector<vector<string>>& results, Storage& csvData, vector<string>& searchInputs);
void calculateBusiestDay(vector<vector<string>>& results, Storage& csvData, vector<string>& searchInputs);