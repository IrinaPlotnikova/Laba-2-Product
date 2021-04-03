#include "Storages.h"


Storages::Storages() {
	_count = 0;
}


// ���������� ���������� �������
int Storages::size() {
	return _count;
}


// ���������� true, ���� ����� �����������, ����� � false
bool Storages::isEmpty() {
	return _count == 0;
}


// �������� �������
void Storages::clear() {
	for (int i = 0; i < _count; i++)
		_storages[i].clear();
	_count = 0;
}


// ���������� ������
void Storages::addStorage(Storage& storage) {
	if (_count == _storages.size())
		_storages.push_back(storage);
	else
		_storages[_count] = storage;
	_count++;
}


// �������� ������
bool Storages::eraseStorage(int indexStorage) {
	if (indexStorage < 0 || indexStorage >= _count)
		return false;
	_count--;
	_storages[indexStorage] = _storages[_count];
	return true;
}


// �������� �������� ������� �� ���� �������
// ���������� ���������� ��������� �������
int Storages::eraseProductsLinear(Predicate predicate){
	int result = 0;
	for (int i = 0; i < _count; i++)
		result += _storages[i].eraseLinear(predicate);
	return result;

}


// �������� �������� ������� �� ���� ������� 
// ���������� ���������� ��������� �������
int Storages::eraseProductsBinary(Comparator comparator, CompareType compareType, ProductFields field) {
	int result = 0;
	for (int i = 0; i < _count; i++)
		result += _storages[i].eraseBinary(comparator, compareType, field);
	return result;
}


// �������� ����� ������� �� ���� �������
// ���������� ������� � ��������� ��������� �������
vector<vector<int>> Storages::searchProductsLinear(Predicate predicate) {
	vector<vector<int>> result(_count);
	for (int i = 0; i < _count; i++)
		result[i] = _storages[i].searchLinear(predicate);
	return result;
}


// �������� ����� ������� �� ���� �������
// ���������� ������ � ��������� ��������� �������
vector<pair<int, int>> Storages::searchProductsBinary(Comparator comparator, CompareType compareType, ProductFields field) {
	vector<pair<int, int>> result(_count);
	for (int i = 0; i < _count; i++)
		result[i] = _storages[i].searchBinary(comparator, compareType, field);
	return result;
}


// �������� ����������� ������� � ������ ������ �� ������
// ���������� ���������� ������������ �������
int Storages::moveProductsLinear(int indexStorage1, int indexStorage2, Predicate predicate) {
	if (indexStorage1 < 0 || indexStorage2 < 0 || indexStorage1 >= _count || indexStorage2 >= _count ||
		indexStorage1 == indexStorage2)
		return 0;
	vector<int> indices = _storages[indexStorage1].searchLinear(predicate);
	for (int i = 0; i < _count; i++)
		_storages[indexStorage2].add(_storages[indexStorage1][indices[i]]);
	_storages[indexStorage1].erase(indices);
	return indices.size();
}


// �������� ����������� ������� � ������ ������ �� ������
// ���������� ���������� ������������ �������
int Storages::moveProductsBinary(int indexStorage1, int indexStorage2, Comparator comparator, 
								 CompareType compareType, ProductFields field) {
	if (indexStorage1 < 0 || indexStorage2 < 0 || indexStorage1 >= _count || indexStorage2 >= _count ||
		indexStorage1 == indexStorage2)
		return 0;
	pair<int, int> borders = _storages[indexStorage1].searchBinary(comparator, compareType, field);
	for (int i = borders.first; i < borders.second; i++)
		_storages[indexStorage2].add(_storages[indexStorage1][i]);
	_storages[indexStorage1].erase(borders);
	return borders.second - borders.first;
}


// ��������� ���������� � ���� �������
vector<vector<int>> Storages::statistics(Date date) {
	vector<vector<int>> result(_count);
	for (int i = 0; i < _count; i++) {
		result[i] = _storages[i].statistics(date);
	}
	return result;
}


Storage& Storages::operator[](const int index) {
	return _storages[index];
}


ostream& operator<<(ostream& cout, Storages& storages) {
	cout << "���������� �������: " << storages._count << '\n';
	for (int i = 0; i < storages._count; i++)
		cout << storages._storages[i];
	return cout;
}


istream& operator>>(istream& cin, Storages& storages) {
	storages.clear();
	string tmp;
	int n;
	cin >> tmp >> tmp >> n;
	for (int i = 0; i < n; i++) {
		Storage storage;
		cin >> storage;
		storages.addStorage(storage);
	}
	return cin;
}

