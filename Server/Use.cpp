#include <iostream>
#include<fstream>
#include <sstream>
#include <string>
#include <vector>
#include "search.h"
using namespace std;

void Use::setTime(string arg) {
    time = arg;
    return;
}; //sets time

void Use::setDate(string arg) {
    date = arg;
    return;
}; //sets date

void Use::setDay(string arg) {
    day = arg;
    return;
}; //sets day: Monday-Sunday

void Use::setLong(string arg) {
    longitude = arg;
    return;
}; //sets Longitude

void Use::setLat(string arg) {
    latitude = arg;
    return;
}; //sets Latitude

void Use::setBase(string arg) {
    base = arg;
    return;
}; //sets base

void Use::setHour(int arg) {
    timeHour = arg;
    return;
}; //sets hour

void Use::setMin(int arg) {
    timeMin = arg;
    return;
}; //set min

void Use::setExists(bool arg) {
    exists = arg;
    return;
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
    return;
}; // will clear the fields for a Use object - technically deletes
