#pragma once
#include <iostream>
#include "Date.h"
#include <algorithm>
#include <iomanip>
#include <vector>
#include <functional> 

using namespace std;



class Product {
public:
	int code;
	string name;
	int price;
	int quantity;
	Date receiptDate;
	Date expiryDate;

	Product();
	Product(string& info);
	void Read();
	string ToString() const;
	
	friend ostream& operator<<(ostream& cout, const Product& goods);
	friend istream& operator>>(istream& cin, Product& goods);
};

