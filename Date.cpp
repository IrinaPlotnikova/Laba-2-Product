#include "Date.h"

Date::Date() {
	_year = 2000 + rand() % 21;
	_month = 1 + rand() % 12;
	_day = 1 + rand() % 27;
}

Date::Date(string& dateStr) {
	SetDate(dateStr);
}

string Date::ToString() const {
	stringstream result;
	result << _year;
	result << '-';
	if (_month < 10)
		result << '0';
	result << _month << '-';
	if (_day < 10)
		result << '0';
	result << _day;
	return result.str();
}


void Date::SetDate(string& dateStr) {
	char ch;
	stringstream ss(dateStr);
	ss >> _year >> ch >> _month >> ch >> _day;
}

bool operator<(const Date& lhs, const Date& rhs) {
	return tie(lhs._year, lhs._month, lhs._day) < tie(rhs._year, rhs._month, rhs._day);
}

bool operator==(const Date& lhs, const Date& rhs) {
	return lhs._year == rhs._year && lhs._month == rhs._month && lhs._day == rhs._day;
}

bool operator<=(const Date& lhs, const Date& rhs) {
	return tie(lhs._year, lhs._month, lhs._day) <= tie(rhs._year, rhs._month, rhs._day);
}

ostream& operator<<(ostream& cout, const Date& date) {
	return cout << date.ToString();
}
istream& operator>>(istream& cin, Date& date) {
	char ch;
	return cin >> date._year >> ch >> date._month >> ch >> date._day;
}