#pragma once
#include"Storage.h"

using Comparator = function<int(const Product&)>;
using Predicate = function<bool(const Product&)>;

class Storages {
private:
	int _count;
	vector<Storage> _storages;
public:
	Storages();

	int size();
	bool isEmpty();
	void clear();

	void addStorage(Storage& storage);
	bool eraseStorage(int indexStorage);

	int eraseProductsLinear(Predicate predicate);
	int eraseProductsBinary(Comparator comparator, CompareType compareType, ProductFields field);


	vector<vector<int>> searchProductsLinear(Predicate predicate);
	vector<pair<int, int>> searchProductsBinary(Comparator comparator, CompareType compareType, ProductFields field);

	int moveProductsLinear(int indexStorage1, int indexStorage2, Predicate predicate);
	int moveProductsBinary(int indexStorage1, int indexStorage2, Comparator comparator, 
						   CompareType compareType, ProductFields field);

	vector<vector<int>> statistics(Date date);

	Storage& operator[](const int index);

	friend ostream& operator<<(ostream& cout, Storages& storages);
	friend istream& operator>>(istream& cin, Storages& storages);
};


