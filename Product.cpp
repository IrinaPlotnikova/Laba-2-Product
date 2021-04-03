#include "Product.h";

Product::Product() {
	code = rand() % 101;
	int size = rand() % 10 + 1;
	for (int i = 0; i < size; i++)
		name += static_cast<char>('a' + rand() % 26);
	price = rand() % 1000 + 1;
	quantity = rand() % 500 + 1;
	if (expiryDate < receiptDate)
		swap(receiptDate, expiryDate);
}

Product::Product(string& info) {
	stringstream ss(info);
	string tmp;
	ss >> tmp;
	code = atoi(tmp.c_str());
	ss >> name;
	ss >> tmp;
	price = atoi(tmp.c_str());
	ss >> tmp;
	quantity = atoi(tmp.c_str());
	ss >> tmp;
	receiptDate.SetDate(tmp);
	ss >> tmp;
	expiryDate.SetDate(tmp);
}

void Product::Read() {
	cout << "Введите данные о товаре:\n";
	cout << "Код: ";
	cin >> code;
	cout << "Название: ";
	cin >> name;
	cout << "Цена: ";
	cin >> price;
	cout << "Количество: ";
	cin >> quantity;
	cout << "Дата получения (ГГГГ-ММ-ДД): ";
	cin >> receiptDate;
	cout << "Срок годности: ";
	cin >> expiryDate;
}

string Product::ToString() const {
	stringstream ss;
	ss.setf(ios::left);
	ss << setw(5) << code;
	ss << setw(15) << name;
	ss << setw(7) << price;
	ss << setw(8) << quantity;
	ss << setw(15) << receiptDate.ToString();
	ss << setw(15) << expiryDate.ToString();
	return ss.str();
}

ostream& operator<<(ostream& cout, const Product& goods) {
	cout << goods.ToString();
	return cout;
}

istream& operator>>(istream& cin, Product& goods) {
	string s;
	getline(cin, s);
	goods = Product(s);
	return cin;
}

