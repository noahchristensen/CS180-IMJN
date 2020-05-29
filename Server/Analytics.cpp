#include <iostream>
#include<fstream>
#include <sstream>
#include <string>
#include <vector>
#include "search.h"
using namespace std;



void insertData(vector<vector<string>>& results, Storage& csvData, vector<string>& insertInputs) { // pushes new data to end of csvData vector
    //cout << "About to enter flagcheck" << endl;
    csvData.flagChecks1(csvData, insertInputs); // this will handle checking flags and updating any results already calculated.
    //cout << "Back from flagcheck" << endl;
    csvData.incCount();
    if (csvData.checkCount()) {//more than 1000 inputs/deletes. recalculate data
        csvData.resetCount();
    }

    string temp;

    temp = "\"" + insertInputs.at(1) + " " + insertInputs.at(0) + ":00" + "\"" + "," + insertInputs.at(2) + "," + insertInputs.at(3) + "," + "\"" + insertInputs.at(4) + "\"";
    Parsed* obj = new Parsed;
    obj->parseData(temp); // this will parse all the fields
    obj->parseHours(obj->retTime());
    obj->parseMonth(obj->retDate());
    obj->convertToDay();
    Use* row1 = new Use;

    row1->setTime(obj->retTime());
    row1->setDate(obj->retDate());
    row1->setLong(obj->retLongt());
    row1->setLat(obj->retLat());
    row1->setBase(obj->retBase());
    row1->setDay(obj->retDay());
    row1->setHour(obj->retHour());
    row1->setMin(obj->retMin());
    row1->setExists(true);


    csvData.insertRow(*row1);
    delete obj;
    if (csvData.checkCount()) {
        csvData.resetCount();
    }
    return;
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
    return;
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
        //cout << "MostTime for loop" << endl;
        for (unsigned int k = 0; k < obj->getOrigSize(); k++) {//counts 
            Use* row1 = new Use;
            *row1 = obj->getRow(k);
            count = arr[row1->getHour()];
            count = count + 1;
            arr[row1->getHour()] = count;
            delete row1;
        }
        //cout << "Past for loop" << endl;
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
                miniVec.push_back(str2 + "                                         ");//minus 8
            }
            else if (str2.size() == 2)
            {
                miniVec.push_back(str2 + "                                           ");//minus 8
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
    return;
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
                miniVec.push_back(str2 + "                                           ");//minus 8
            }
            else if (str2.size() == 2)
            {
                miniVec.push_back(str2 + "                                         ");//minus 8
            }
            //miniVec.push_back(str2 + "                                                 ");//push back the hour
            miniVec.push_back(str1 + "      "); //push back the number of occurences for corresponding time - Format of miniVec: 00,15 => hour 00 has 15 occurences
            if (k < 10) {
                results1.push_back(miniVec);
            }
            obj->getLTime().push_back(miniVec);
            miniVec.clear();
        }
        obj->setFlag("leastTime");
    }
    return;
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
            //cout << obj->getMLoc().at(var).at(2) << endl;
            results1.push_back(miniVec);
            miniVec.clear();
        }
    }
    else {
        //cout << "We are entering mostloc first calculation SIZE: " << obj->getOrigSize() << endl;
        for (unsigned int k = 0; k < obj->getSize(); k++) {//counts 
            pair<int, vector<int>>pairs;
            Use* row1 = new Use;
            *row1 = obj->getRow(k);
            //cout << "Part 1" << endl;
            in1 = row1->getLat(); //XX.XXX
            //cout << "Part 2" << endl;
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
            str2.append("                      ");//added 8
            arr1[index] = 0; //set smallest to 1000000 and loop again up to 10 times
            miniVec.push_back(str1);//push back the lat
            miniVec.push_back(str2);//push back the lat
            miniVec.push_back(str3.append("   "));//push back the number of occurences for corresponding location
            cout << "We push: " << str1 << " " << str2 << " " << str3 << endl;
            if (k < 10) {
                results1.push_back(miniVec);
            }

            obj->getMLoc().push_back(miniVec);
            miniVec.clear();
            min = arr1[index];
        }
        obj->setFlag("mostLoc");
    }
    return;
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
        for (unsigned int k = 0; k < obj->getSize(); k++) {//counts 
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
            str2.append("                      ");//added 5
            arr1[index] = 1000000; //set smallest to 1000000 and loop again up to 10 times
            miniVec.push_back(str1);//push back the lat
            miniVec.push_back(str2);//push back the lat
            miniVec.push_back(str3.append("        "));//push back the number of occurences for corresponding location
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
    return;
};


void calculateBusiestDay(vector<vector<string>>& results1, Storage& csvData, vector<string>& searchInputs) {
    vector<string> miniVec;
    Storage* obj = &csvData;
    string in1;
    int arr[7] = { 0 }; //for each day 
    string arr1[7] = { "Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday" }; // each day of week for comparison check
    int count = 0;
    //cout << "Right before check " << endl;
    if (obj->checkFlag("busiestDay")) {
        //cout << "flag check was true" << endl;
        for (int var = 0; var < 7; var++) {
            miniVec = obj->getBDay().at(var);
            results1.push_back(miniVec);
            miniVec.clear();
        }
    }
    else {
        //cout << "flag check was false" << endl;
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
                miniVec.push_back(arr1[index] + "                            ");//minus 8 spaces
            }
            else if (arr1[index].compare("Monday") == 0)
            {
                miniVec.push_back(arr1[index] + "                           ");
            }
            else if (arr1[index].compare("Tuesday") == 0)
            {
                miniVec.push_back(arr1[index] + "                          ");
            }
            else if (arr1[index].compare("Wednesday") == 0)
            {
                miniVec.push_back(arr1[index] + "                    ");
            }
            else if (arr1[index].compare("Thursday") == 0)
            {
                miniVec.push_back(arr1[index] + "                         ");
            }
            else if (arr1[index].compare("Friday") == 0)
            {
                miniVec.push_back(arr1[index] + "                              ");
            }
            else if (arr1[index].compare("Saturday") == 0)
            {
                miniVec.push_back(arr1[index] + "                          ");
            }
            else
            {
                miniVec.push_back(arr1[index] + "                        ");
            }
            //miniVec.push_back(arr1[index] + "                         ");//push back the day
            miniVec.push_back(str1); //push back the number of occurences for corresponding day - Format of miniVec: "Monday","234" => Monday has 234 occurences
            results1.push_back(miniVec);
            obj->getBDay().push_back(miniVec);
            miniVec.clear();
        }
        obj->setFlag("busiestDay");
    }
    return;
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
    return;
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



//app comparison functions
void compDay(Storage& csvData1, Storage& csvData2, vector<vector<string>>& results, vector<string>& searchInputs) {
    vector<string>miniVec;
    int count1 = 0;
    int count2 = 0;
    int i = 0;
    string string1;
    Storage* obj1 = &csvData1;//uber data
    Storage* obj2 = &csvData2;//lyft data

    Use* row1 = new Use;

    for (i = 0;i < obj1->getSize();i++) {
        *row1 = obj1->getRow(i);
        if (row1->getDate() == searchInputs.at(0)) {
            count1++;
        }
    }
    for (i = 0;i < obj2->getSize();i++) {
        *row1 = obj2->getRow(i);
        if (row1->getDate() == searchInputs.at(0)) {
            count2++;
        }
    }
    string1 = std::to_string(count1);
    string1 = "UBER: " + string1 + "    ";
    miniVec.push_back(string1);
    cout << string1 << endl;
    string1 = std::to_string(count2);
    string1 = "LYFT: " + string1 + "    ";
    cout << string1 << endl;
    miniVec.push_back(string1);

    results.push_back(miniVec);
    miniVec.clear();
    return;

};//given a day compares rides that day

void compLoc(Storage& csvData1, Storage& csvData2, vector<vector<string>>& results, vector<string>& searchInputs) {
    vector<string>miniVec;
    int count1 = 0;
    int count2 = 0;
    int i = 0;
    string string1;
    Storage* obj1 = &csvData1;//uber data
    Storage* obj2 = &csvData2;//lyft data

    Use* row1 = new Use;

    for (i = 0;i < obj1->getSize();i++) {
        *row1 = obj1->getRow(i);
        if (row1->getLat() == searchInputs.at(0) && row1->getLong() == searchInputs.at(1)) {
            count1++;
        }
    }
    for (i = 0;i < obj2->getSize();i++) {
        *row1 = obj2->getRow(i);
        if (row1->getLat() == searchInputs.at(0) && row1->getLong() == searchInputs.at(1)) {
            count2++;
        }
    }
    string1 = std::to_string(count1);
    string1 = "UBER: " + string1 + "    ";
    cout << string1 << endl;
    miniVec.push_back(string1);
    string1 = std::to_string(count2);
    string1 = "LYFT: " + string1 + "    ";
    cout << string1 << endl;
    miniVec.push_back(string1);

    results.push_back(miniVec);
    miniVec.clear();
    return;


};//given a location compares locations for that day

void compareMostUseTime(vector<vector<string>>& results, Storage& csvData1, Storage& csvData2, vector<string>& searchInputs) {
    vector<vector<string>> results1;
    vector<vector<string>> results2;
    vector<string> miniVec;
    string temp1;
    string temp2;
    searchMostUseTime(results1, csvData1, searchInputs);//Uber top 10 
    searchMostUseTime(results2, csvData2, searchInputs);//Lyft top 10 
    string counter;
    for (int i = 0;i < results1.size();i++) {// pushes back Uber "Time      count" and Lyft "Time     count"
       /* miniVec = results1.at(i);
        results.push_back(miniVec);
        miniVec = results2.at(i);
        results.push_back(miniVec);*/
        temp1 = results1.at(i).at(0);
        temp2 = results2.at(i).at(0);
        counter = std::to_string(i + 1);
        temp2 = counter + ". Uber:" + temp1 + "  Lyft:" + temp2;
        cout << temp2 << endl;
        miniVec.push_back(temp2);
        results.push_back(miniVec);
        miniVec.clear();
    }
    return;
};

void compareLeastUseTime(vector<vector<string>>& results, Storage& csvData1, Storage& csvData2, vector<string>& searchInputs) {
    vector<vector<string>> results1;
    vector<vector<string>> results2;
    vector<string> miniVec;
    string temp1;
    string temp2;
    searchLeastUseTime(results1, csvData1, searchInputs);//Uber top 10 
    searchLeastUseTime(results2, csvData2, searchInputs);//Lyft top 10 
    string counter;
    for (int i = 0;i < results1.size();i++) {// pushes back Uber "Time      count" and Lyft "Time     count"
       /* miniVec = results1.at(i);
        results.push_back(miniVec);
        miniVec = results2.at(i);
        results.push_back(miniVec);*/
        temp1 = results1.at(i).at(0);
        temp2 = results2.at(i).at(0);
        counter = std::to_string(i + 1);
        temp2 = counter + ". Uber:" + temp1 + "  Lyft:" + temp2;
        cout << temp2 << endl;
        miniVec.push_back(temp2);
        results.push_back(miniVec);
        miniVec.clear();
    }
    return;

};

void compareMostLoc(vector<vector<string>>& results, Storage& csvData1, Storage& csvData2, vector<string>& searchInputs) {
    vector<vector<string>> results1;
    vector<vector<string>> results2;
    vector<string> miniVec;
    string temp1;
    string temp2;
    string temp3;
    string temp4;
    searchMostLoc(results1, csvData1, searchInputs);//Uber top 10 
    searchMostLoc(results2, csvData2, searchInputs);//Lyft top 10 
    string counter;
    for (int i = 0;i < results1.size();i++) {// pushes back Uber "Location count" and Lyft "Location count"
        temp1 = results1.at(i).at(0);
        temp2 = results2.at(i).at(0);
        temp3 = results1.at(i).at(1);
        temp4 = results2.at(i).at(1);
        counter = std::to_string(i + 1);
        temp2 = counter + ". Uber:" + temp1 + "  " + temp3 + "  Lyft:" + temp2 + "  " + temp4 ;
        cout << temp2 << endl;
        miniVec.push_back(temp2);
        //miniVec = results1.at(i);
        results.push_back(miniVec);
        //miniVec = results2.at(i);
        //results.push_back(miniVec);
        miniVec.clear();
    }
    return;

};

void compareLeastLoc(vector<vector<string>>& results, Storage& csvData1, Storage& csvData2, vector<string>& searchInputs) {
    vector<vector<string>> results1;
    vector<vector<string>> results2;
    vector<string> miniVec;
    string temp1;
    string temp2;
    string temp3;
    string temp4;
    searchLeastLoc(results1, csvData1, searchInputs);//Uber top 10 
    searchLeastLoc(results2, csvData2, searchInputs);//Lyft top 10 
    string counter;
    for (int i = 0;i < results1.size();i++) {// pushes back Uber "Location count" and Lyft "Location count"
        temp1 = results1.at(i).at(0);
        temp2 = results2.at(i).at(0);
        temp3 = results1.at(i).at(1);
        temp4 = results2.at(i).at(1);
        counter = std::to_string(i + 1);
        temp2 = counter + ". Uber:" + temp1 + "  " + temp3 + "  Lyft:" + temp2 + "  " + temp4 ;
        cout << temp2 << endl;
        miniVec.push_back(temp2);
        //miniVec = results1.at(i);
        results.push_back(miniVec);
        //miniVec = results2.at(i);
        //results.push_back(miniVec);
        miniVec.clear();
    }
    return;
};


