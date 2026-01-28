// lab_supplementary.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <limits>
#include <iomanip>

using namespace std;

struct Product {
    string manufacturer;
    string name;
    vector<string>composition;
    double weight;
    double price;

    Product(string n, string m, vector<string>o, double p, double r):
        manufacturer(n), name(m), composition(o), weight(p), price(r){}
};

vector<Product> products;

bool validate(const string& name) {
    return !name.empty() && name.length() <= 15;
}

void load() {
    ifstream file("products.txt");
    if (!file.is_open()) {
        return;
    }
    products.clear();

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string manufacturer, name;
        double weight, price;
        getline(ss, manufacturer, '|');
        getline(ss, name, '|');
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
        products.emplace_back(manufacturer, name, composition, weight, price);
    }
    file.close();
}

void save() {
    ofstream file("products.txt");
    if (!file.is_open()) {
        return;
    }
    for (const auto& product : products) {
        file << product.manufacturer << "|"
            << product.name << "|"
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

void initialize() {
    products.clear();

    products.emplace_back(
        "Cахар \"Cладкий\"",
        "ИП Пирогов",
        vector<string>{"сахар", "подсластители"},
        1000,
        105
    );
    products.emplace_back(
        "Кофе \"Яшка\"",
        "ООО Яшка Кофе",
        vector<string>{"молотый кофе", "ароматизатель"},
        150,
        299.99
    );
    products.emplace_back(
        "Кофе \"Чашка\"",
        "ЗАО Чашка",
        vector<string>{"растворимый кофе", "сухое молоко"},
        250,
        189.50
    );
    products.emplace_back(
        "Энергетик \"Тихий Дэн\"",
        "ООО Озон",
        vector<string>{"сахар", "кофеин"},
        300,
        149.99
    );
    products.emplace_back(
        "Батончик \"Крутой\"",
        "Завод Мисье",
        vector<string>{"вафля", "шоколад"},
        30,
        99.99
    );
}

void display() {
    if (products.empty()) {
        cout << "Ошибка" << endl;
        return;
    }
    cout << "\n Cписок товаров" << endl;
    for (size_t i = 0; i < products.size(); ++i) {
        const auto& product = products[i];
        cout << "\n Товар №" << (i + 1) << endl;
        cout << "\n Производитель: " << product.manufacturer << endl;
        cout << "\n Название: " << product.name << endl;
        cout << "\n Состав: ";
        for (size_t j = 0; j < product.composition.size(); ++j) {
            cout << product.composition[j];
            if (j != product.composition.size() - 1) {
                cout << " + ";
            }
        }
        cout << fixed << setprecision(2);
        cout << "\n Вес: " << product.weight << "гр" << endl;
        cout << "Цена: " << product.price << "руб. " << endl;
    }
    cout << "\n Всего товаров: " << products.size() << endl;
}

void editMenu() {
    int choice;
    do {
        cout << "\nРедактирование товаров." << endl;
        if (products.empty()) {
            cout << "Ошибка" << endl;
            return;
        }
        for (size_t i = 0; i < products.size(); ++i) {
            cout << (i + 1) << ". " << products[i].name << " " << products[i].weight << "гр" << endl;
        }
        cout << "90. Назад" << endl;
        cout << "99. Выход из программы" << endl;
        cout << "Введите номер: ";
        cin >> choice;
        if (choice == 90) {
            return;
        }
        else if (choice == 99) {
            save();
            exit(0);
        }
        else if (choice >= 1 && choice <= static_cast<int>(products.size())) {
            editProduct(choice - 1);
        }
        else {
            cout << "Неверный выбор" << endl;
        }
    } while (true);
}

void editProduct(int index) {
    Product& product = products[index];
    int choice;
    do {
        cout << "Редактирование товара" << endl;
        cout << "1. Производитель: " << product.manufacturer << endl;
        cout << "2. Название: " << product.name << endl;
        cout << "3. Состав продукта: " << product.composition.size() << endl;
        cout << "4. Вес: " << product.weight << endl;
        cout << "5. Цена: " << product.price << endl;
        cout << "90. Назад" << endl;
        cout << "99. Выход из программы" << endl;
        cout << "Выберите, что отредактировать";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (choice == 90) {
            return;
        }
        else if (choice == 99) {
            save();
            exit(0);
            }
        switch (choice) {
        case 1: {
                cout << product.manufacturer << endl;
                cout << "Введите нового производителя";
                getline(cin, product.manufacturer);
                break;
            }
        case 2: {
            string newName;
            cout << product.name << endl;
            cout << "Введите новое имя. Не больше 15 символов";
            getline(cin, newName);
            if (validate(newName)) {
                product.name = newName;
                cout << "Изменено" << endl;
            }
            else {
                cout << "Error: must not be longer than 15 symbols" << endl;
            }
            break;
        }
        case 3: {
            int choice;
            do {
                cout << "\nСостав" << endl;
                if (product.composition.empty()) {
                    cout << "Empty" << endl;
                }
                else {
                    for (size_t i = 0; i < product.composition.size(); ++i) {
                        cout << " " << (i + 1) << ". " << product.composition[i] << endl;
                    }
                }
                cout << "\n1. Добавить" << endl;
                cout << "\n2. Удалить" << endl;
                cout << "90. Назад" << endl;
                cout << "Выберите действие" << endl;
                cin >> choice;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if (choice == 1) {
                    string ingredient;
                    cout << "Введите новый ингредиент";
                    getline(cin, ingredient);
                    if (!ingredient.empty()) {
                        product.composition.push_back(ingredient);
                        cout << "Added" << endl;
                    }
                    else {
                        cout << "Empty" << endl;
                    }
                    } else if (choice == 2) {
                        if (!product.composition.empty()) {
                            cout << "Deleted" << endl;
                            product.composition.pop_back();
                        }
                        else {
                            cout << "Error" << endl;
                        }
                }
            } while (choice != 90);
            break;
        }
        case 4: {
        }
        }
    }
}


int main()
{
    setlocale(LC_ALL, "RU");
    
    int choice;

    do {
        cout << "Выберите пункт меню: ";
        cin >> choice;
    } while (choice != 99);
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
