#include "Storages.h"


Storages::Storages() {
	_count = 0;
}


// возвращает количество складов
int Storages::size() {
	return _count;
}


// возаращает true, если склад отсутствуют, иначе — false
bool Storages::isEmpty() {
	return _count == 0;
}


// удаление складов
void Storages::clear() {
	for (int i = 0; i < _count; i++)
		_storages[i].clear();
	_count = 0;
}


// добавление склада
void Storages::addStorage(Storage& storage) {
	if (_count == _storages.size())
		_storages.push_back(storage);
	else
		_storages[_count] = storage;
	_count++;
}


// удаление склада
bool Storages::eraseStorage(int indexStorage) {
	if (indexStorage < 0 || indexStorage >= _count)
		return false;
	_count--;
	_storages[indexStorage] = _storages[_count];
	return true;
}


// линейное удаление товаров со всех складов
// возвращает количество удаленных товаров
int Storages::eraseProductsLinear(Predicate predicate){
	int result = 0;
	for (int i = 0; i < _count; i++)
		result += _storages[i].eraseLinear(predicate);
	return result;

}


// бинарное удаление товаров со всех складов 
// возвращает количество удаленных товаров
int Storages::eraseProductsBinary(Comparator comparator, CompareType compareType, ProductFields field) {
	int result = 0;
	for (int i = 0; i < _count; i++)
		result += _storages[i].eraseBinary(comparator, compareType, field);
	return result;
}


// лмнейный поиск товаров на всех складах
// возвращает массивы с индексами найденных товаров
vector<vector<int>> Storages::searchProductsLinear(Predicate predicate) {
	vector<vector<int>> result(_count);
	for (int i = 0; i < _count; i++)
		result[i] = _storages[i].searchLinear(predicate);
	return result;
}


// бинарный посик товаров на всех складах
// возвращает массив с границами найденных товаров
vector<pair<int, int>> Storages::searchProductsBinary(Comparator comparator, CompareType compareType, ProductFields field) {
	vector<pair<int, int>> result(_count);
	for (int i = 0; i < _count; i++)
		result[i] = _storages[i].searchBinary(comparator, compareType, field);
	return result;
}


// линейное перемещение товаров с одного склада на другой
// возвращает количество перемещенных товаров
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


// линейное перемещение товаров с одного склада на другой
// возвращает количество перемещенных товаров
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


// получение статистики о всех складах
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
	cout << "Количество складов: " << storages._count << '\n';
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

