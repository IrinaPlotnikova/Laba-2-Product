#pragma once
#include <iostream>
#include <sstream>
#include <ctime>
#include <tuple>
using namespace std;

class Date {
private:
	int _year;
	int _month;
	int _day;
public:
	Date();
	Date(string& dateStr);
	string ToString() const;
	void SetDate(string& dateStr);
	
	friend bool operator==(const Date& lhs, const Date& rhs);
	friend bool operator<(const Date& lhs, const Date& rhs);
	friend bool operator<=(const Date& lhs, const Date& rhs);
	friend ostream& operator<<(ostream& cout, const Date& rhs);
	friend istream& operator>>(istream& cin, Date& rhs);
};

