// lab_supplementary.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <limits>

using namespace std;

struct Product {
    string name;
    string manufacturer;
    vector<string> composition;
    double weight;
    double price;
    
    Product(string n, string m, vector<string> c, double w, double p)
        : name(n), manufacturer(m), composition(c), weight(w), price(p) {}
};

vector<Product> products;

void loadProductsFromFile();
void saveProductsToFile();
void initializeDefaultProducts();
void displayAllProducts();
void editProductsMenu();
void editProduct(int index);
void addProduct();
bool validateName(const string& name);
bool validateWeight(double weight);
bool validatePrice(double price);

void displayMainMenu();
void handleMainMenuChoice(int choice);

int main() {
    setlocale(LC_ALL, "RU");
    
    loadProductsFromFile();

    if (products.empty()) {
        initializeDefaultProducts();
        saveProductsToFile();
    }
    
    int choice;
    
    do {
        displayMainMenu();
        cout << "Выберите пункт меню: ";
        cin >> choice;
        
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        handleMainMenuChoice(choice);
        
    } while (choice != 99);
    
    return 0;
}

void loadProductsFromFile() {
    ifstream file("products.txt");
    
    if (!file.is_open()) {
        return;
    }
    
    products.clear();
    
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        string name, manufacturer;
        double weight, price;
        getline(ss, name, '|');
        getline(ss, manufacturer, '|');
        ss >> weight;
        ss.ignore();
        ss >> price;
        ss.ignore();
        
        vector<string> composition;
        string compItem;
        while (getline(ss, compItem, ',')) {
            if (!compItem.empty()) {
                composition.push_back(compItem);
            }
        }
        
        products.emplace_back(name, manufacturer, composition, weight, price);
    }
    
    file.close();
}

void saveProductsToFile() {
    setlocale(LC_ALL, "RU");
    ofstream file("products.txt");
    
    if (!file.is_open()) {
        cout << "Ошибка при сохранении файла!" << endl;
        return;
    }
    
    for (const auto& product : products) {
        file << product.name << "|" 
             << product.manufacturer << "|" 
             << product.weight << "|" 
             << product.price << "|";

        for (size_t i = 0; i < product.composition.size(); ++i) {
            file << product.composition[i];
            if (i != product.composition.size() - 1) {
                file << ",";
            }
        }
        
        file << endl;
    }
    
    file.close();
}

void initializeDefaultProducts() {
    products.clear();
    
    products.emplace_back(
        "Сахар \"Сладкий\"",
        "ИП Пирогов",
        vector<string>{"сахар песок"},
        1000,
        105.60
    );
    
    products.emplace_back(
        "Кофе \"Яшка\"",
        "ООО Яшка Кофе",
        vector<string>{"молотый кофе"},
        150,
        299.99
    );
    
    products.emplace_back(
        "Кофе \"Чашка\"",
        "ЗАО Чашка",
        vector<string>{"растворимый кофе", "сахар"},
        250,
        189.50
    );
    
    products.emplace_back(
        "Чай \"Цейлон\"",
        "ИП Иванов",
        vector<string>{"чай черный листовой", "аромат бергамота"},
        100,
        150.00
    );
    
    products.emplace_back(
        "Шоколад \"Аленка\"",
        "ОАО Красный Октябрь",
        vector<string>{"какао-порошок", "сахар", "сухое молоко", "лецитин"},
        100,
        89.90
    );
}

void displayAllProducts() {
    if (products.empty()) {
        cout << "\nСписок товаров пуст\n" << endl;
        return;
    }
    
    cout << "\nCписок товаров" << endl;
    
    for (size_t i = 0; i < products.size(); ++i) {
        const auto& product = products[i];
        
        cout << "\nТовар #" << (i + 1) << endl;
        cout << "Производитель: " << product.manufacturer << endl;
        cout << "Название: " << product.name << endl;
        cout << "Состав: ";
        
        for (size_t j = 0; j < product.composition.size(); ++j) {
            cout << product.composition[j];
            if (j != product.composition.size() - 1) {
                cout << " + ";
            }
        }
        
        cout << fixed << setprecision(2);
        cout << "\nВес: " << product.weight << "гр" << endl;
        cout << "Цена: " << product.price << " руб." << endl;
    }
    
    cout << "\nВсего товаров: " << products.size() << endl;
}

void editProductsMenu() {
    setlocale(LC_ALL, "RU");
    int choice;
    
    do {
        cout << "\nРедактирование товаров" << endl;
        
        if (products.empty()) {
            cout << "\nСписок товаров пуст\n" << endl;
            return;
        }
        
        for (size_t i = 0; i < products.size(); ++i) {
            cout << (i + 1) << ". " << products[i].name 
                 << " " << products[i].weight << "гр" << endl;
        }
        
        cout << "90. Назад" << endl;
        cout << "99. Выход из программы" << endl;
        cout << "Введите номер: ";
        cin >> choice;
        
        if (choice == 90) {
            return; 
        } else if (choice == 99) {
            saveProductsToFile();
            exit(0); 
        } else if (choice >= 1 && choice <= static_cast<int>(products.size())) {
            editProduct(choice - 1);
        } else {
            cout << "Неверный выбор" << endl;
        }
        
    } while (true);
}

void editProduct(int index) {
    setlocale(LC_ALL, "RU");
    Product& product = products[index];
    int choice;
    
    do {
        cout << "\nРедактирование товара" << endl;
        cout << "1. Название: " << product.name << endl;
        cout << "2. Производитель: " << product.manufacturer << endl;
        cout << "3. Состав (элементов: " << product.composition.size() << ")" << endl;
        cout << "4. Вес: " << product.weight << "гр" << endl;
        cout << "5. Цена: " << product.price << " руб." << endl;
        cout << "90. Назад" << endl;
        cout << "99. Выход из программы" << endl;
        cout << "Выберите поле для редактирования: ";
        cin >> choice;
        
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        if (choice == 90) {
            return;
        } else if (choice == 99) {
            saveProductsToFile();
            exit(0);
        }
        
        switch (choice) {
            case 1: { 
                string newName;
                cout << "Текущее название: " << product.name << endl;
                cout << "Введите новое название (не более 15 символов): ";
                getline(cin, newName);
                
                if (validateName(newName)) {
                    product.name = newName;
                    cout << "Название успешно изменено!" << endl;
                } else {
                    cout << "Ошибка: название должно содержать от 1 до 15 символов!" << endl;
                }
                break;
            }
            
            case 2: { 
                cout << "Текущий производитель: " << product.manufacturer << endl;
                cout << "Введите нового производителя: ";
                getline(cin, product.manufacturer);
                cout << "Производитель успешно изменен!" << endl;
                break;
            }
            
            case 3: { 
                int compChoice;
                do {
                    cout << "\nРедактирование состава" << endl;
                    cout << "Текущий состав:" << endl;
                    
                    if (product.composition.empty()) {
                        cout << "Пусто" << endl;
                    } else {
                        for (size_t i = 0; i < product.composition.size(); ++i) {
                            cout << "  " << (i + 1) << ". " << product.composition[i] << endl;
                        }
                    }
                    
                    cout << "\n1. Добавить ингредиент" << endl;
                    cout << "2. Удалить последний ингредиент" << endl;
                    cout << "90. Назад" << endl;
                    cout << "Выберите действие: ";
                    cin >> compChoice;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    
                    if (compChoice == 1) {
                        string ingredient;
                        cout << "Введите новый ингредиент: ";
                        getline(cin, ingredient);
                        
                        if (!ingredient.empty()) {
                            product.composition.push_back(ingredient);
                            cout << "Ингредиент успешно добавлен" << endl;
                        } else {
                            cout << "Ошибка: ингредиент не может быть пустым" << endl;
                        }
                    } else if (compChoice == 2) {
                        if (!product.composition.empty()) {
                            cout << "Удален ингредиент: " << product.composition.back() << endl;
                            product.composition.pop_back();
                        } else {
                            cout << "Ошибка: состав уже пустой" << endl;
                        }
                    }
                    
                } while (compChoice != 90);
                break;
            }
            
            case 4: {
                double newWeight;
                cout << "Текущий вес: " << product.weight << "гр" << endl;
                cout << "Введите новый вес в граммах (не более 1000000гр = 1т): ";
                cin >> newWeight;
                
                if (validateWeight(newWeight)) {
                    product.weight = newWeight;
                    cout << "Вес успешно изменен" << endl;
                } else {
                    cout << "Ошибка: вес должен быть в диапазоне 0-1000000 грамм" << endl;
                }
                break;
            }
            
            case 5: {
                double newPrice;
                cout << "Текущая цена: " << product.price << " руб." << endl;
                cout << "Введите новую цену в рублях: ";
                cin >> newPrice;
                
                if (validatePrice(newPrice)) {
                    product.price = newPrice;
                    cout << "Цена успешно изменена" << endl;
                } else {
                    cout << "Ошибка: цена должна быть положительным числом" << endl;
                }
                break;
            }
            
            default:
                cout << "Ошибка" << endl;
        }

        saveProductsToFile();
        
    } while (choice != 90);
}

void addProduct() {
    setlocale(LC_ALL, "RU");
    cout << "\nДобавление нового товара" << endl;
    
    string name, manufacturer;
    double weight, price;
    vector<string> composition;
    
    bool validName = false;
    while (!validName) {
        cout << "Введите название товара (не более 15 символов): ";
        getline(cin, name);
        
        if (validateName(name)) {
            validName = true;
        } else {
            cout << "Ошибка: название должно содержать от 1 до 15 символов" << endl;
        }
    }
    
    cout << "Введите производителя: ";
    getline(cin, manufacturer);

    cout << "\nВвод состава товара (для завершения введите нажмите Enter при пустой строке):" << endl;
    string ingredient;
    int ingredientCount = 1;
    
    while (true) {
        cout << "Ингредиент #" << ingredientCount << ": ";
        getline(cin, ingredient);
        
        if (ingredient.empty()) {
            break;
        }
        
        composition.push_back(ingredient);
        ingredientCount++;
    }
    
    bool validWeight = false;
    while (!validWeight) {
        cout << "Введите вес в граммах (не более 1000000гр = 1т): ";
        cin >> weight;
        cin.ignore();
        
        if (validateWeight(weight)) {
            validWeight = true;
        } else {
            cout << "Ошибка: вес должен быть в диапазоне 0-1000000 грамм" << endl;
        }
    }
    
    bool validPrice = false;
    while (!validPrice) {
        cout << "Введите цену в рублях: ";
        cin >> price;
        cin.ignore();
        
        if (validatePrice(price)) {
            validPrice = true;
        } else {
            cout << "Ошибка: цена должна быть положительным числом" << endl;
        }
    }
    
    products.emplace_back(name, manufacturer, composition, weight, price);
    cout << "\nТовар успешно добавлен" << endl;
    
    saveProductsToFile();
}

bool validateName(const string& name) {
    return !name.empty() && name.length() <= 15;
}

bool validateWeight(double weight) {
    return weight > 0 && weight <= 1000000;
}

bool validatePrice(double price) {
    return price > 0;
}

void displayMainMenu() {
    cout << "\nУправление товарами" << endl;
    cout << "1. Просмотр товаров" << endl;
    cout << "2. Редактирование товаров" << endl;
    cout << "3. Добавить новый товар" << endl;
    cout << "99. Выход из программы" << endl;
}

void handleMainMenuChoice(int choice) {
    switch (choice) {
        case 1:
            displayAllProducts();
            break;
        case 2:
            editProductsMenu();
            break;
        case 3:
            addProduct();
            break;
        case 99:
            saveProductsToFile();
            cout << "\nДанные сохранены" << endl;
            break;
        default:
            cout << "Ошибка" << endl;
    }
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.