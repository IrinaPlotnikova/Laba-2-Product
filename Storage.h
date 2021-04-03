#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include "Product.h"
using namespace std;

using Comparator = function<int(const Product&)>;
using Predicate = function<bool(const Product&)>;

enum class ProductFields {
	Code,
	Name,
	Price,
	Quantity,
	ReceiptDate,
	ExpiryDate,
	None
};

enum class CompareType {
	Less,
	LessOrEqual,
	Equal,
	BiggerOrEqual,
	Bigger
};

class Storage {
private:
	string _name;
	int _count;
	ProductFields _sortType;
	vector<Product> _goods;

	void markAsSorted(ProductFields field);

	int binarySearchLeftBorder(Comparator comparator);
	int binarySearchRightBorder(Comparator comparator);

public:
	Storage();
	Storage(string name);

	int size();
	bool isEmpty();
	void clear();

	string name();
	void rename(string& newName);

	void add(Product& product);
	void addRandom(int numberOfProducts);

	bool isSorted(ProductFields field);
	void sortProducts(ProductFields field);

	vector<int> searchLinear(Predicate predicate);
	pair<int, int> searchBinary(Comparator comparator, CompareType compareType, ProductFields field);

	bool eraseAt(int index);
	int eraseLinear(Predicate predicate);
	int eraseBinary(Comparator comparator, CompareType compareType, ProductFields field);

	int erase(const vector<int>& indices);
	int erase(const pair<int, int> borders);
	
	bool change(int index, const Product& product);

	vector<int> statistics(Date date);

	Product& operator[](const int index);

	friend ostream& operator<<(ostream& cout, Storage& storage);
	friend istream& operator>>(istream& cin, Storage& storage);
};

