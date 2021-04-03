#include <iostream>
#include <fstream>
#include <functional> 
#include "Storages.h"
#include <Windows.h>

using Comparator = function<int(const Product&)>;
using Predicate = function<bool(const Product&)>;

int getNumber(const string& message, int mi, int ma);
bool isBinaryOperation();
string messageIndexOfStorage(Storages& storages);

ProductFields getProductField();
Comparator getComparator(ProductFields field);
CompareType getCompareType();
Predicate getPredicate(Comparator comparator, CompareType compareType);

void printMenuMain(bool isEmpty);
void mainMenuAddStorage(Storages& storages);
void mainMenuReadFromFile(Storages& storages);
void mainMenuDeleteStorage(Storages& storages);
void mainMenuChooseActiveStorage(Storages& storages);
void mainMenuFindProducts(Storages& storages);
void mainMenuMoveProducts(Storages& storages);
void mainMenuDeleteProducts(Storages& storages);
void mainMenuStatistics(Storages& storages);
void mainMenuSaveToFile(Storages& storages);

void workWithStorage(Storage& storage);
void printActiveStorageMenu(bool isEmpty);
void storageMenuRename(Storage& storage);
void storageMenuAddProduct(Storage& storage);
void storageMenuAddRandomProducts(Storage& storage);
void storageMenuChangeProduct(Storage& storage);
void storageMenuFindProducts(Storage& storage);
void storageMenuDeleteProducts(Storage& storages);

int printProducts(Storages& storages, vector<vector<int>>& indices);
int printProducts(Storages& storages, vector<pair<int, int>>& borders);
int printProducts(Storage& storage, vector<int>& indices);
int printProducts(Storage& storage, pair<int, int>& borders);
void printStatistics(vector<int>& statistics);

template < typename T >
int Compare(T& lhs, T& rhs) {
    if (lhs < rhs)
        return -1;
    if (lhs == rhs)
        return 0;
    return 1;
}

bool isChanged = false;


int main()
{
    srand(time(NULL));
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);


    Storages storages;
    printMenuMain(true);
    int code = getNumber("Выберите действие", 0, 2);
    while (code != 0) {
        switch (code) {
        case 1: {
            mainMenuAddStorage(storages);
            break;
        }
        case 2: {
            mainMenuReadFromFile(storages);
            break;
        }
        case 3: {
            mainMenuDeleteStorage(storages);
            break;
        }
        case 4: {
            mainMenuChooseActiveStorage(storages);
            break;
        }
        case 5: {
            mainMenuFindProducts(storages);
            break;
        }
        case 6: {
            mainMenuMoveProducts(storages);
            break;
        }
        case 7: {
            mainMenuDeleteProducts(storages);
            break;
        }
        case 8: {
            mainMenuStatistics(storages);
            break;
        }
        case 9: {
            mainMenuSaveToFile(storages);
            break;
        }
        default: {
            cout << storages;
            break;
        }
        }
        cout << "\n\n";
        printMenuMain(storages.isEmpty());
        code = getNumber("Выберите действие", 0, storages.isEmpty() ? 2 : 10);
    }

    if (isChanged) {
        if (getNumber("Сохранить изменения? [1 - ДА, 0 - НЕТ]", 0, 1)) {
            mainMenuSaveToFile(storages);
            cout << "\n";
        }
    }

    return 0;
}


void printMenuMain(bool isEmpty) {
    cout << "1. Добавить склад\n";
    cout << "2. Считать из файла\n";

    if (!isEmpty) {
        cout << "3. Удалить склад\n";
        cout << "4. Выбрать склад для работы\n";
        cout << "5. Найти товары на всех складах\n";
        cout << "6. Переместить товары\n";
        cout << "7. Удалить товары со всех складов\n";
        cout << "8. Показать статистику\n";
        cout << "9. Сохранить в файл\n";
        cout << "10. Вывести на консоль\n";
    }
    cout << "0. Завершить работу\n";
}

void mainMenuAddStorage(Storages& storages) {
    isChanged = true;
    stringstream name;
    name << "Склад_№" << storages.size() + 1;
    Storage storage(name.str());
    storages.addStorage(storage);
    cout << "Склад добавлен!";
}

void mainMenuReadFromFile(Storages& storages) {
    if (isChanged) {
        if (getNumber("Сохранить изменения? [1 - ДА, 0 - НЕТ]", 0, 1)) {
            mainMenuSaveToFile(storages);
            cout << "\n";
        } 
    }
    isChanged = false;

    string filename;
    cout << "Введите имя файла: ";
    cin >> filename;
    ifstream fin;
    fin.open(filename);
    fin >> storages;
    fin.close();
    cout << "Файл считан!";
}

void mainMenuDeleteStorage(Storages& storages) {
    isChanged = true;
    int index = 0;
    if (storages.size() > 1)
        index = getNumber(messageIndexOfStorage(storages), 1, storages.size()) - 1;
    storages.eraseStorage(index);
    cout << "Склад удалён!";
}

void mainMenuChooseActiveStorage(Storages& storages) {
    int index = 0;
    if (storages.size() > 1)
        index = getNumber(messageIndexOfStorage(storages), 1, storages.size()) - 1;
    workWithStorage(storages[index]);
}

void mainMenuFindProducts(Storages& storages) {
    ProductFields field = getProductField();
    Comparator comparator = getComparator(field);
    CompareType compareType = getCompareType();

    int numberOfPrintedProducts = 0;
    if (isBinaryOperation()) {
        vector<pair<int, int>> indices = storages.searchProductsBinary(comparator, compareType, field);
        numberOfPrintedProducts = printProducts(storages, indices);
    }
    else {
        Predicate predicate = getPredicate(comparator, compareType);
        vector<vector<int>> indices = storages.searchProductsLinear(predicate);
        numberOfPrintedProducts = printProducts(storages, indices);
    }

    if (numberOfPrintedProducts == 0)
        cout << "Товары не найдены";
    else
        cout << "Товары найдены и напечатаны (" << numberOfPrintedProducts << " шт.)";
}

void mainMenuMoveProducts(Storages& storages) {
    if (storages.size() == 1) {
        cout << "Товары не перемещены";
        return;
    }
    cout << "(ИЗ)\n";
    int indexStorage1 = getNumber(messageIndexOfStorage(storages), 1, storages.size()) - 1;
    cout << "(В)\n";
    int indexStorage2 = getNumber(messageIndexOfStorage(storages), 1, storages.size()) - 1;
    if (indexStorage1 == indexStorage2) {
        cout << "Товары не перемещены";
        return;
    }

    ProductFields field = getProductField();
    Comparator comparator = getComparator(field);
    CompareType compareType = getCompareType();

    int numberOfProducts = 0;
    if (isBinaryOperation())
        numberOfProducts = storages.moveProductsBinary(indexStorage1, indexStorage2, comparator, compareType, field);
    else {
        Predicate predicate = getPredicate(comparator, compareType);
        numberOfProducts = storages.moveProductsLinear(indexStorage1, indexStorage2, predicate);
    }

    if (numberOfProducts == 0)
        cout << "Товары не найдены";
    else {
        cout << "Товары перемещены (" << numberOfProducts << " шт.)";
        isChanged = true;
    }
        
}


void mainMenuDeleteProducts(Storages& storages) {
    ProductFields field = getProductField();
    Comparator comparator = getComparator(field);
    CompareType compareType = getCompareType();

    int numberOfDeletedProducts = 0;
    if (isBinaryOperation())
        numberOfDeletedProducts = storages.eraseProductsBinary(comparator, compareType, field);
    else {
        Predicate predicate = getPredicate(comparator, compareType);
        numberOfDeletedProducts = storages.eraseProductsLinear(predicate);
    }

    if (numberOfDeletedProducts == 0)
        cout << "Товары не найдены";
    else {
        isChanged = true;
        cout << "Товары удалены (" << numberOfDeletedProducts << " шт.)";
    }
}

void mainMenuStatistics(Storages& storages) {
    cout << "Введите дату (ГГГГ-ММ-ДД): ";
    Date date;
    cin >> date;
    vector<vector<int>> statistics = storages.statistics(date);
    for (int i = 0; i < statistics.size(); i++) {
        cout << storages[i].name() << ":\n";
        printStatistics(statistics[i]);
        cout << '\n';
    }
}

void printStatistics(vector<int>& statistics) {
    cout << "Количество видов непросроченных товаров: " << statistics[0] << '\n';
    cout << "Стоимость  непросроченных товаров: " << statistics[1] << '\n';
    cout << "Количество непросроченных товаров: " << statistics[2] << '\n';
    cout << "Количество видов просроченных товаров: " << statistics[3] << '\n';
    cout << "Стоимость  просроченных товаров: " << statistics[4] << '\n';
    cout << "Количество просроченных товаров: " << statistics[5] << '\n';
}

void mainMenuSaveToFile(Storages& storages) {
    isChanged = false;
    string filename;
    cout << "Введите имя файла: ";
    cin >> filename;
    ofstream fout;
    fout.open(filename);
    fout << storages;
    fout.close();
    cout << "Файл сохранён!";
}


void workWithStorage(Storage& storage) {
    printActiveStorageMenu(storage.isEmpty());
    int code = getNumber("Выберите действие", 0, storage.isEmpty() ? 3 : 7);
    while (code != 0) {
        switch (code) {
        case 1: {
            storageMenuRename(storage);
            break;
        }
        case 2: {
            storageMenuAddProduct(storage);
            break;
        }
        case 3: {
            storageMenuAddRandomProducts(storage);
            break;
        }
        case 4: {
            storageMenuFindProducts(storage);
            break;
        }
        case 5: {
            storageMenuChangeProduct(storage);
            break;
        }
        case 6: {
            storageMenuDeleteProducts(storage);
            break;
        }
        default: {
            cout << storage;
            break;
        }
        }
        cout << "\n\n";
        printActiveStorageMenu(storage.isEmpty());
        code = getNumber("Выберите действие", 0, storage.isEmpty() ? 3 : 7);
    }
}

void printActiveStorageMenu(bool isEmpty) {
    cout << "1. Переименовать\n";
    cout << "2. Добавить товар\n";
    cout << "3. Добавить случайные товары\n"; 
    if (!isEmpty) {
        cout << "4. Найти товары\n";
        cout << "5. Изменить товар\n";
        cout << "6. Удалить товары\n";
        cout << "7. Вывести на консоль\n";
    }
    cout << "0. Вернуться в главное меню\n";
}

void storageMenuRename(Storage& storage) {
    isChanged = true;

    cout << "Текущее название: " << storage.name() << '\n';
    cout << "Введите новое название: ";

    string newName;
    cin >> newName;
    storage.rename(newName);
    cout << "Склад переименован";
}

void storageMenuAddProduct(Storage& storage) {
    isChanged = true;
    Product product;
    product.Read();
    storage.add(product);
    cout << "Товар добавлен";
}

void storageMenuAddRandomProducts(Storage& storage) {
    isChanged = true;
    int numberOfProducts = getNumber("Введите количество товаров: ", 1, 20);
    storage.addRandom(numberOfProducts);
    cout << "Товары добавлены";
}

void storageMenuChangeProduct(Storage& storage) {
    isChanged = true;
    int index = 0;
    if (storage.size() > 1)
        index = getNumber("Введите номер товара: ", 1, storage.size()) - 1;
    Product product;
    product.Read();
    storage.change(index, product);
    cout << "Товар изменен";
}

void storageMenuFindProducts(Storage& storage) {
    ProductFields field = getProductField();
    Comparator comparator = getComparator(field);
    CompareType compareType = getCompareType();

    int numberOfPrintedProducts = 0;
    if (isBinaryOperation()) {
        pair<int, int> borders = storage.searchBinary(comparator, compareType, field);
        numberOfPrintedProducts = printProducts(storage, borders);
    }
    else {
        Predicate predicate = getPredicate(comparator, compareType);
        vector<int> indices = storage.searchLinear(predicate);
        numberOfPrintedProducts = printProducts(storage, indices);
    }

    if (numberOfPrintedProducts == 0)
        cout << "Товары не найдены";
    else
        cout << "Товары найдены и напечатаны (" << numberOfPrintedProducts << " шт.)";
}



void storageMenuDeleteProducts(Storage& storage) {
    cout << "1. Удалить 1 товар\n";
    cout << "2. Удалить товары по критерию\n";
    int eraseType = getNumber("Выберите тип удаления: ", 1, 2);

    if (eraseType == 1) {
        int indexProduct = 0;
        int n = storage.size();
        if (n > 1)
            indexProduct = getNumber("Введите индекс удаляемого товара: [1, " + to_string(n) + ')', 1, n) - 1;
        storage.eraseAt(indexProduct);
        cout << "Товар удален";
        isChanged = true;
    }
    else {
        ProductFields field = getProductField();
        Comparator comparator = getComparator(field);
        CompareType compareType = getCompareType();

        int numberOfDeletedProducts = 0;
        if (isBinaryOperation())
            numberOfDeletedProducts = storage.eraseBinary(comparator, compareType, field);
        else {
            Predicate predicate = getPredicate(comparator, compareType);
            numberOfDeletedProducts = storage.eraseLinear(predicate);
        }

        if (numberOfDeletedProducts == 0)
            cout << "Товары не найдены";
        else {
            isChanged = true;
            cout << "Товары удалены (" << numberOfDeletedProducts << " шт.)";
        }
    }
}



ProductFields getProductField() {
    cout << "Выберите поле, по которому отберутся товары:\n";
    cout << "1. Код\n";
    cout << "2. Название\n";
    cout << "3. Цена\n";
    cout << "4. Количество\n";
    cout << "5. Дата получения\n";
    cout << "6. Срок годности\n";

    int fieldInt = getNumber("", 1, 6) - 1;
    return static_cast<ProductFields>(fieldInt);
}

Comparator getComparator(ProductFields field) {
    cout << "Введите значение поля: ";
    if (field == ProductFields::Code) {
        int code;
        cin >> code;
        return [code](const Product& product) {return Compare(product.code, code); };
    }
    if (field == ProductFields::Name) {
        string name;
        cin >> name;
        return [name](const Product& product) {return Compare(product.name, name); };
    }
    if (field == ProductFields::Price) {
        int price;
        cin >> price;
        return [price](const Product& product) {return Compare(product.price, price); };
    }
    if (field == ProductFields::Quantity) {
        int quantity;
        cin >> quantity;
        return [quantity](const Product& product) { return Compare(product.quantity, quantity); };
    }
    if (field == ProductFields::ReceiptDate) {
        Date receiptDate;
        cin >> receiptDate;
        return[receiptDate](const Product& product) {return Compare(product.receiptDate, receiptDate); };
    }
    Date expiryDate;
    cin >> expiryDate;
    return [expiryDate](const Product& product) {return Compare(product.expiryDate, expiryDate); };
}


CompareType getCompareType() {
    cout << "Выберите отношение, по которому отберутся отвары:\n";
    cout << "1. <\n";
    cout << "2. <=\n";
    cout << "3. ==\n";
    cout << "4. >=\n";
    cout << "5. >\n";
    int compareTypeInt = getNumber("", 1, 5) - 1;

    return static_cast<CompareType>(compareTypeInt);
}

Predicate getPredicate(Comparator comparator, CompareType compareType) {
    if (compareType == CompareType::Less)
        return [comparator](const Product& product) {return comparator(product) < 0; };
    if (compareType == CompareType::LessOrEqual)
        return [comparator](const Product& product) {return comparator(product) <= 0; };
    if (compareType == CompareType::Equal)
        return [comparator](const Product& product) {return comparator(product) == 0; };
    if (compareType == CompareType::BiggerOrEqual)
        return [comparator](const Product& product) {return comparator(product) >= 0; };
    return [comparator](const Product& product) {return comparator(product) > 0; };
}



int getNumber(const string& message, int mi, int ma) {
    int number;
    if (message != "")
        cout << message << '\n';
    cout << "--> ";
    cin >> number;
    while (number < mi || number > ma) {
        cout << "Ошибка!\n";
        cout << "--> ";
        cin >> number;
    }
    cout << '\n';
    return number;
}


bool isBinaryOperation() {
    cout << "Выберите тип операции:\n";
    cout << "1. Линейный\n";
    cout << "2. Бинарный\n";
    return getNumber("", 1, 2) == 2;
}

int printProducts(Storages& storages, vector<vector<int>>& indices) {
    int numberOfPrintedProducts = 0;
    for (int i = 0; i < storages.size(); i++) {
        numberOfPrintedProducts += printProducts(storages[i], indices[i]);
    }
    return numberOfPrintedProducts;
}

int printProducts(Storages& storages, vector<pair<int, int>>& borders) {
    int numberOfPrintedProducts = 0;
    for (int i = 0; i < storages.size(); i++) {
        numberOfPrintedProducts += printProducts(storages[i], borders[i]);
    }
    return numberOfPrintedProducts;
}

int printProducts(Storage& storage, vector<int>& indices) {
    int numberOfPrintedProducts = indices.size();
    if (numberOfPrintedProducts != 0) {
        cout << storage.name() << ":\n";
        for (int j = 0; j < indices.size(); j++) {
            cout << storage[indices[j]] << '\n';
        }
    }
    return numberOfPrintedProducts;
}


int printProducts(Storage& storage, pair<int, int>& borders) {
    int numberOfPrintedProducts = borders.second - borders.first;
    if (numberOfPrintedProducts != 0) {
        cout << storage.name() << ":\n";
        for (int j = borders.first; j < borders.second; j++) {
            cout << storage[j] << '\n';
        }
    }
    return numberOfPrintedProducts;
}

string messageIndexOfStorage(Storages& storages) {
    return "Введите номер склада [1; " + to_string(storages.size()) + "]";
}