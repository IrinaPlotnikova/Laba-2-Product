#include "Storage.h"


Storage::Storage() {
	_name = "�� �������";
	_count = 0;
}


Storage::Storage(string name) {
	_name = name;
	_count = 0;
	_sortType = ProductFields::None;
}


// ���������� ���������� ������� �� ������
int Storage::size() {
	return _count;
}


// �������� ������ �� �������
// ���������� true, ���� ����� ����, ����� � false
bool Storage::isEmpty() {
	return _count == 0;
}


// �������� ���� ������� �� ������
void Storage::clear() {
	_count = 0;
}


// ���������� �������� ������
string Storage::name() {
	return _name;
}


// ������������� ����� ��������
void Storage::rename(string& newName) {
	_name = newName;
}


// ���������� ������ 
void Storage::add(Product& product) {
	if (_count == _goods.size())
		_goods.push_back(product);
	else
		_goods[_count] = product;
	_count++;
	_sortType = ProductFields::None;
}


// ���������� ��������� ������� � ���������� numberOfProducts
void Storage::addRandom(int numberOfProducts) {
	for (int i = 0; i < numberOfProducts; i++) {
		Product product; // ��������� ��������� �����
		add(product);
	}
}


// �������� ����������������� ������� �� ���� field
// true ���� �������������, ����� � false
bool Storage::isSorted(ProductFields field) {
	return _sortType == field;
}


// ��������� ����, �� �������� ������������� ������
void Storage::markAsSorted(ProductFields field) {
	_sortType = field;
}


// ���������� ������� �� ���� field
void Storage::sortProducts(ProductFields field) {
	int index = static_cast<int>(field);
	vector<function<bool(const Product&, const Product&)>> comparators = {
		[](const Product& lhs, const Product& rhs) {return lhs.code < rhs.code; },
		[](const Product& lhs, const Product& rhs) {return lhs.name < rhs.name; },
		[](const Product& lhs, const Product& rhs) {return lhs.price < rhs.price; },
		[](const Product& lhs, const Product& rhs) {return lhs.quantity < rhs.quantity; },
		[](const Product& lhs, const Product& rhs) {return lhs.receiptDate < rhs.receiptDate; },
		[](const Product& lhs, const Product& rhs) {return lhs.expiryDate < rhs.expiryDate; }
	};
	sort(_goods.begin(), _goods.begin() + _count, comparators[index]);
}


// �������� ����� ������ �� ������
// ���������� �������, � ������� ��������� ���������� ������
vector<int> Storage::searchLinear(Predicate predicate) {
	vector<int> result;
	for (int i = 0; i < _count; i++) {
		if (predicate(_goods[i]))
			result.push_back(i);
	}
	return result;
}


// �������� ����� ������� �� ������
// ������������ �������� [;), �������� �������� ������������� �������
pair<int, int> Storage::searchBinary(Comparator comparator, CompareType compareType, ProductFields field) {
	if (!isSorted(field)) {
		sortProducts(field);
		markAsSorted(field);
	}

	if (compareType == CompareType::Less)
		return { 0, binarySearchLeftBorder(comparator) + 1 };
	if (compareType == CompareType::LessOrEqual)
		return { 0, binarySearchRightBorder(comparator) };
	if (compareType == CompareType::Equal)
		return { binarySearchLeftBorder(comparator) + 1, binarySearchRightBorder(comparator) };
	if (compareType == CompareType::BiggerOrEqual)
		return { binarySearchLeftBorder(comparator) + 1, _count };
	return { binarySearchRightBorder(comparator), _count };
}


// ���� ������ ���������� ��������, ������� ������ ������ ����������
// ���� ������ ���, �� ���������� -1
int Storage::binarySearchLeftBorder(Comparator comparator) {
	int left = -1;
	int right = _count - 1;
	while (left < right) {
		int middle = (left + right + 1) / 2;
		if (comparator(_goods[middle]) == -1)
			left = middle;
		else
			right = middle - 1;
	}
	return left;
}


// ���� ������ ������� ��������, ������� ������ ������ ����������
// ���� ������ ���, �� ���������� ����� ������
int Storage::binarySearchRightBorder(Comparator comparator) {
	int left = 0;
	int right = _count;
	while (left < right) {
		int middle = (left + right) / 2;
		if (comparator(_goods[middle]) <= 0)
			left = middle + 1;
		else
			right = middle;
	}
	return left;
}


// �������� �������� �������
// ���������� ���������� ��������� �������
int Storage::eraseLinear(Predicate predicate) {
	vector<int> indices = searchLinear(predicate);
	return erase(indices);
}


// �������� �������, ������������� � ��������, ������������ � ������� indices
// ��������� ������ ������� indices
int Storage::erase(const vector<int>& indices) {
	for (int i = indices.size() - 1; i >= 0; i--)
		eraseAt(indices[i]);
	return indices.size();
}


// �������� ������ �� ������� index
bool Storage::eraseAt(int index) {
	if (index < 0 || index >= _count)
		return false;
	_count--;
	if (index != _count) {
		_goods[index] = _goods[_count];
		_sortType = ProductFields::None;
	}
	return true;
}


// �������� �������� �������
// ���������� ���������� ��������� �������
int Storage::eraseBinary(Comparator comparator, CompareType compareType, ProductFields field) {
	pair<int, int> borders = searchBinary(comparator, compareType, field);
	return erase(borders);
}


// �������� ���� �������, ������������� � �������� [borders.first, borders.second) 
int Storage::erase(const pair<int, int> borders) {
	int leftBorder = borders.first;
	int rightBorder = borders.second;
	if (leftBorder == rightBorder)
		return 0;
	int i = 0;
	while (rightBorder + i < _count){
		_goods[leftBorder + i] = _goods[rightBorder + i];
		i++;
	}
	_count -= rightBorder - leftBorder;
	return rightBorder - leftBorder;
}


// ��������� ������ �� ������� index 
bool Storage::change(int index, const Product& product) {
	if (index < 0 || index >= _count)
		return false;
	_goods[index] = product;
	return true;
}


// ��������� ���������� ������ �� ��������� �� ���� daty
vector<int> Storage::statistics(Date date) {
	vector<int> result(6);
	for (int i = 0; i < _count; i++) {
		if (_goods[i].receiptDate <= date) {
			if (date <= _goods[i].expiryDate) {
				result[0]++; // ���������� ����� �������������� �������
				result[1] += _goods[i].price * _goods[i].quantity; // ��������� �������������� �������
				result[2] += _goods[i].quantity; // ���������� ����� �������������� �������
			}
			else {
				result[3]++; // ���������� ����� ������������ �������
				result[4] += _goods[i].price * _goods[i].quantity; // ��������� ������������ �������
				result[5] += _goods[i].quantity;  // ���������� ����� ������������ �������
			}
		} 
	}
	return result;
}


Product& Storage::operator[](const int index) {
	return _goods[index];
}


ostream& operator<<(ostream& cout, Storage& storage) {
	setlocale(0, "Russian");
	cout << "�������� ������: " << storage._name << '\n';
	cout << "���������� �������: " << storage._count << '\n';
	
	if (storage._count != 0) {
		cout.setf(ios::left);
		cout << setw(4) << "�";
		cout << setw(5) << "���";
		cout << setw(15) << "��������";
		cout << setw(7) << "����";
		cout << setw(8) << "���-��";
		cout << setw(15) << "���� ���������";
		cout << setw(15) << "���� ��������";
		cout << '\n';
		for (int i = 0; i < storage._count; i++) {
			stringstream ss;
			ss << i + 1 << ". ";
			cout << setw(4) << ss.str();
			cout << storage[i] << '\n';
		}
	}
	
	return cout;
}


istream& operator>>(istream& cin, Storage& storage) {
	string tmp;
	int n;
	storage.clear();
	cin >> tmp >> tmp >> storage._name;
	cin >> tmp >> tmp >> n;
	getline(cin, tmp);
	if (n != 0) {
		getline(cin, tmp);
		for (int i = 0; i < n; i++) {
			Product product;
			cin >> tmp >> product;
			storage.add(product);
		}
	}
	return cin;
}