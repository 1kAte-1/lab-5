#include <iostream>
#include <cmath>
#include <algorithm>
#include <random>
#include <ctime>
#include <stdexcept>
#include <cstdlib>   // для std::abs
#include <string>
#include <limits>

using namespace std;

// ============================ КЛАСС POINT ============================
class Point {
private:
    double x, y;
public:
    // Конструкторы
    Point() : x(0.0), y(0.0) {}
    Point(double xCoord, double yCoord) : x(xCoord), y(yCoord) {}

    // Геттеры (получить координаты)
    double getX() const { return x; }
    double getY() const { return y; }

    // Сеттеры (установить координаты)
    void setX(double xCoord) { x = xCoord; }
    void setY(double yCoord) { y = yCoord; }

    // Методы перемещения
    void up(double delta) { y += delta; }      // вверх
    void down(double delta) { y -= delta; }      // вниз
    void left(double delta) { x -= delta; }      // влево
    void right(double delta) { x += delta; }      // вправо

    // Печать точки
    void print() const { cout << "(" << x << ", " << y << ")"; }
};

// ============================ КЛАСС FRACTION ============================
class Fraction {
private:
    int numerator;   // числитель
    int denominator; // знаменатель

    // Собственная функция НОД (GCD)
    static int gcd(int a, int b) {
        a = abs(a);
        b = abs(b);
        while (b != 0) {
            int temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    }

    // Сокращение дроби
    void reduce() {
        if (denominator == 0) throw invalid_argument("Знаменатель не может быть нулём");
        if (denominator < 0) {
            numerator = -numerator;
            denominator = -denominator;
        }
        int g = gcd(numerator, denominator);
        numerator /= g;
        denominator /= g;
    }

public:
    // Конструктор по умолчанию (0/1)
    Fraction() : numerator(0), denominator(1) {}

    // Конструктор от double
    Fraction(double value) {
        double eps = 1e-9;
        int den = 1;
        while (abs(value - round(value)) > eps && den < 1000000) {
            value *= 10;
            den *= 10;
        }
        numerator = static_cast<int>(round(value));
        denominator = den;
        reduce();
    }

    // Конструктор от числителя и знаменателя
    Fraction(int num, int den) : numerator(num), denominator(den) {
        if (den == 0) throw invalid_argument("Знаменатель не может быть нулём");
        reduce();
    }

    // Геттеры
    int getNumerator()   const { return numerator; }
    int getDenominator() const { return denominator; }

    // Сеттеры
    void setNumerator(int num) {
        numerator = num;
        reduce();
    }
    void setDenominator(int den) {
        if (den == 0) throw invalid_argument("Знаменатель не может быть нулём");
        denominator = den;
        reduce();
    }

    // Преобразование в double
    double toDouble() const {
        return static_cast<double>(numerator) / denominator;
    }

    // Печать дроби
    void print() const {
        cout << numerator << "/" << denominator;
    }

    // Операторы сравнения
    bool operator==(const Fraction& other) const {
        return numerator == other.numerator && denominator == other.denominator;
    }
    bool operator!=(const Fraction& other) const { return !(*this == other); }
    bool operator<(const Fraction& other) const {
        return numerator * other.denominator < other.numerator * denominator;
    }
    bool operator>(const Fraction& other) const { return other < *this; }
    bool operator<=(const Fraction& other) const { return !(*this > other); }
    bool operator>=(const Fraction& other) const { return !(*this < other); }

    // Арифметические операции
    Fraction operator+(const Fraction& other) const {
        return Fraction(numerator * other.denominator + other.numerator * denominator,
            denominator * other.denominator);
    }
    Fraction operator-(const Fraction& other) const {
        return Fraction(numerator * other.denominator - other.numerator * denominator,
            denominator * other.denominator);
    }
    Fraction operator*(const Fraction& other) const {
        return Fraction(numerator * other.numerator, denominator * other.denominator);
    }
    Fraction operator/(const Fraction& other) const {
        if (other.numerator == 0) throw invalid_argument("Деление на ноль");
        return Fraction(numerator * other.denominator, denominator * other.numerator);
    }

    // Присваивание с операцией
    Fraction& operator+=(const Fraction& other) { *this = *this + other; return *this; }
    Fraction& operator-=(const Fraction& other) { *this = *this - other; return *this; }
    Fraction& operator*=(const Fraction& other) { *this = *this * other; return *this; }
    Fraction& operator/=(const Fraction& other) { *this = *this / other; return *this; }
};

// ============================ КЛАСС MYVECTOR ============================
class MyVector {
private:
    int* data;        // указатель на массив
    int size;         // текущее количество элементов
    int capacity;     // зарезервированная память

    // Приватный метод: проверка и увеличение ёмкости
    void EnsureCapacity(int newSize) {
        if (newSize <= capacity) return;
        int newCap = max(newSize, static_cast<int>(capacity * 1.5) + 1);
        int* newData = new int[newCap];
        for (int i = 0; i < size; ++i) newData[i] = data[i];
        delete[] data;
        data = newData;
        capacity = newCap;
    }

public:
    // Конструктор по умолчанию: capacity = 10, size = 0
    MyVector() : size(0), capacity(10) {
        data = new int[capacity]();
    }

    // Конструктор с заданным capacity
    explicit MyVector(int cap) : size(0), capacity(cap > 0 ? cap : 10) {
        data = new int[capacity]();
    }

    // Конструктор копирования
    MyVector(const MyVector& other) : size(other.size), capacity(other.capacity) {
        data = new int[capacity];
        for (int i = 0; i < size; ++i) data[i] = other.data[i];
    }

    // Деструктор
    ~MyVector() { delete[] data; }

    // Геттеры
    int getSize()     const { return size; }         // получить размер
    int getCapacity() const { return capacity; }     // получить ёмкость

    // Проверка на пустоту
    bool IsEmpty() const { return size == 0; }

    // Печать всех элементов
    void Print() const {
        for (int i = 0; i < size; ++i) cout << data[i] << " ";
        cout << endl;
    }

    // Добавление в конец
    void PushBack(int value) {
        EnsureCapacity(size + 1);
        data[size++] = value;
    }

    // Добавление в начало
    void PushFront(int value) {
        EnsureCapacity(size + 1);
        for (int i = size; i > 0; --i) data[i] = data[i - 1];
        data[0] = value;
        ++size;
    }

    // Вставка по индексу
    void Insert(int index, int value) {
        if (index < 0 || index > size) throw out_of_range("Insert: неверный индекс");
        EnsureCapacity(size + 1);
        for (int i = size; i > index; --i) data[i] = data[i - 1];
        data[index] = value;
        ++size;
    }

    // Удаление по индексу
    void RemoveAt(int index) {
        if (index < 0 || index >= size) throw out_of_range("RemoveAt: неверный индекс");
        for (int i = index; i < size - 1; ++i) data[i] = data[i + 1];
        --size;
    }

    // Удаление одного или всех вхождений значения
    void Remove(int value, bool all = true) {
        int newSize = 0;
        for (int i = 0; i < size; ++i) {
            if (data[i] != value) {
                data[newSize++] = data[i];
            }
            else if (!all) {
                for (int j = i + 1; j < size; ++j) data[newSize++] = data[j];
                size = newSize;
                return;
            }
        }
        size = newSize;
    }

    // Удаление первого элемента
    void PopFront() { if (size > 0) RemoveAt(0); }

    // Удаление последнего элемента
    void PopBack() { if (size > 0) --size; }

    // Очистка (обнуление и size=0)
    void Clear() {
        for (int i = 0; i < size; ++i) data[i] = 0;
        size = 0;
    }

    // Подгонка capacity под size
    void TrimToSize() {
        if (capacity == size) return;
        int* newData = new int[size];
        for (int i = 0; i < size; ++i) newData[i] = data[i];
        delete[] data;
        data = newData;
        capacity = size;
    }

    // Линейный поиск первого вхождения (слева)
    int IndexOf(int value) const {
        for (int i = 0; i < size; ++i) if (data[i] == value) return i;
        return -1;
    }

    // Линейный поиск последнего вхождения (справа)
    int LastIndexOf(int value) const {
        for (int i = size - 1; i >= 0; --i) if (data[i] == value) return i;
        return -1;
    }

    // Обратный порядок
    void Reverse() {
        for (int i = 0; i < size / 2; ++i) swap(data[i], data[size - 1 - i]);
    }

    // Сортировка по возрастанию
    void SortAsc() { sort(data, data + size); }

    // Сортировка по убыванию
    void SortDesc() { sort(data, data + size, greater<int>()); }

    // Перемешивание
    void Shuffle() {
        static mt19937 rng(static_cast<unsigned>(time(nullptr)));
        shuffle(data, data + size, rng);
    }

    // Заполнение случайными числами
    void RandomFill(int count) {
        Clear();
        static mt19937 rng(static_cast<unsigned>(time(nullptr)));
        uniform_int_distribution<int> dist(0, 99);
        for (int i = 0; i < count; ++i) PushBack(dist(rng));
    }

    // Сравнение с другим вектором
    bool Equals(const MyVector& other) const {
        if (size != other.size) return false;
        for (int i = 0; i < size; ++i) if (data[i] != other.data[i]) return false;
        return true;
    }

    // Получить элемент по индексу
    int GetElementAt(int index) const {
        if (index < 0 || index >= size) throw out_of_range("GetElementAt: неверный индекс");
        return data[index];
    }

    // Создание копии
    MyVector Clone() const {
        MyVector copy;
        copy.EnsureCapacity(size);
        for (int i = 0; i < size; ++i) copy.PushBack(data[i]);
        return copy;
    }

    // Оператор присваивания
    MyVector& operator=(const MyVector& other) {
        if (this == &other) return *this;
        delete[] data;
        size = other.size;
        capacity = other.capacity;
        data = new int[capacity];
        for (int i = 0; i < size; ++i) data[i] = other.data[i];
        return *this;
    }

    // Оператор доступа по индексу
    int& operator[](int index) { return data[index]; }
    const int& operator[](int index) const { return data[index]; }

    // Операторы сравнения векторов
    bool operator==(const MyVector& other) const { return Equals(other); }
    bool operator!=(const MyVector& other) const { return !(*this == other); }

    // Дружественные операторы ввода/вывода
    friend ostream& operator<<(ostream& os, const MyVector& vec) {
        for (int i = 0; i < vec.size; ++i) os << vec.data[i] << " ";
        return os;
    }
    friend istream& operator>>(istream& is, MyVector& vec) {
        int n;
        cout << "Введите количество элементов: ";
        is >> n;
        cout << "Введите " << n << " целых чисел: ";
        for (int i = 0; i < n; ++i) {
            int val;
            is >> val;
            vec.PushBack(val);
        }
        return is;
    }
};

// ============================ ИНТЕРАКТИВНЫЙ MAIN ============================
void demoPoint() {
    cout << "\n========== КЛАСС POINT ==========\n";
    double x, y;
    cout << "Введите начальные координаты точки (x y): ";
    cin >> x >> y;
    Point p(x, y);
    cout << "Точка создана: "; p.print(); cout << endl;

    int choice;
    double delta;
    do {
        cout << "\nМЕНЮ ДЛЯ ТОЧКИ:\n";
        cout << "1. Переместить вверх\n2. Переместить вниз\n3. Влево\n4. Вправо\n";
        cout << "5. Установить новые координаты (setX/setY)\n6. Показать текущие координаты\n0. Выйти\n";
        cout << "Ваш выбор: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "На сколько единиц вверх? "; cin >> delta;
            p.up(delta);
            cout << "Точка теперь: "; p.print(); cout << endl;
            break;
        case 2:
            cout << "На сколько единиц вниз? "; cin >> delta;
            p.down(delta);
            cout << "Точка теперь: "; p.print(); cout << endl;
            break;
        case 3:
            cout << "На сколько единиц влево? "; cin >> delta;
            p.left(delta);
            cout << "Точка теперь: "; p.print(); cout << endl;
            break;
        case 4:
            cout << "На сколько единиц вправо? "; cin >> delta;
            p.right(delta);
            cout << "Точка теперь: "; p.print(); cout << endl;
            break;
        case 5:
            cout << "Введите новые x и y: "; cin >> x >> y;
            p.setX(x); p.setY(y);
            cout << "Точка обновлена: "; p.print(); cout << endl;
            break;
        case 6:
            cout << "Текущее положение: "; p.print(); cout << endl;
            break;
        case 0:
            break;
        default:
            cout << "Неверный ввод!\n";
        }
    } while (choice != 0);
}

void demoFraction() {
    cout << "\n========== КЛАСС FRACTION ==========\n";
    int num1, den1, num2, den2;
    cout << "Введите первую дробь (числитель знаменатель): ";
    cin >> num1 >> den1;
    cout << "Введите вторую дробь (числитель знаменатель): ";
    cin >> num2 >> den2;

    try {
        Fraction f1(num1, den1), f2(num2, den2);
        cout << "Дробь 1: "; f1.print(); cout << " = " << f1.toDouble() << endl;
        cout << "Дробь 2: "; f2.print(); cout << " = " << f2.toDouble() << endl;

        int op;
        do {
            cout << "\nМЕНЮ ДРОБЕЙ:\n";
            cout << "1. Сложение\n2. Вычитание\n3. Умножение\n4. Деление\n";
            cout << "5. Сравнение (>, <, ==)\n6. Показать дроби\n0. Выйти\n";
            cout << "Ваш выбор: ";
            cin >> op;

            switch (op) {
            case 1: {
                Fraction res = f1 + f2;
                cout << "Сумма: "; res.print(); cout << " = " << res.toDouble() << endl;
                break;
            }
            case 2: {
                Fraction res = f1 - f2;
                cout << "Разность: "; res.print(); cout << " = " << res.toDouble() << endl;
                break;
            }
            case 3: {
                Fraction res = f1 * f2;
                cout << "Произведение: "; res.print(); cout << " = " << res.toDouble() << endl;
                break;
            }
            case 4: {
                try {
                    Fraction res = f1 / f2;
                    cout << "Частное: "; res.print(); cout << " = " << res.toDouble() << endl;
                }
                catch (const exception& e) {
                    cout << "Ошибка: " << e.what() << endl;
                }
                break;
            }
            case 5:
                if (f1 == f2) cout << "Дроби равны\n";
                else if (f1 > f2) cout << "Первая дробь больше\n";
                else cout << "Первая дробь меньше\n";
                break;
            case 6:
                cout << "Дробь 1: "; f1.print(); cout << endl;
                cout << "Дробь 2: "; f2.print(); cout << endl;
                break;
            case 0:
                break;
            default:
                cout << "Неверный ввод\n";
            }
        } while (op != 0);
    }
    catch (const exception& e) {
        cout << "Ошибка: " << e.what() << endl;
    }
}

void demoMyVector() {
    cout << "\n========== КЛАСС MYVECTOR ==========\n";
    MyVector v;
    int choice, val, index, count;

    do {
        cout << "\nМЕНЮ ВЕКТОРА (текущий размер: " << v.getSize() << ", ёмкость: " << v.getCapacity() << ")\n";
        cout << "1. PushBack (добавить в конец)\n";
        cout << "2. PushFront (добавить в начало)\n";
        cout << "3. Insert (вставить по индексу)\n";
        cout << "4. RemoveAt (удалить по индексу)\n";
        cout << "5. Remove (удалить все/первое вхождение значения)\n";
        cout << "6. PopFront (удалить первый)\n";
        cout << "7. PopBack (удалить последний)\n";
        cout << "8. Clear (очистить)\n";
        cout << "9. Print (показать массив)\n";
        cout << "10. Reverse (обратный порядок)\n";
        cout << "11. SortAsc (сортировка по возрастанию)\n";
        cout << "12. SortDesc (сортировка по убыванию)\n";
        cout << "13. Shuffle (перемешать)\n";
        cout << "14. RandomFill (заполнить случайными)\n";
        cout << "15. IndexOf (поиск слева)\n";
        cout << "16. LastIndexOf (поиск справа)\n";
        cout << "17. TrimToSize (подогнать ёмкость)\n";
        cout << "18. GetElementAt (получить элемент по индексу)\n";
        cout << "19. IsEmpty (проверить пустоту)\n";
        cout << "20. Clone (создать копию и показать)\n";
        cout << "21. Оператор [] (доступ по индексу)\n";
        cout << "22. Оператор == (сравнение с другим вектором)\n";
        cout << "23. Ввод через оператор >>\n";
        cout << "0. Выйти\n";
        cout << "Ваш выбор: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Введите число: "; cin >> val;
            v.PushBack(val);
            break;
        case 2:
            cout << "Введите число: "; cin >> val;
            v.PushFront(val);
            break;
        case 3:
            cout << "Введите индекс и значение: "; cin >> index >> val;
            try { v.Insert(index, val); }
            catch (const exception& e) { cout << "Ошибка: " << e.what() << endl; }
            break;
        case 4:
            cout << "Введите индекс: "; cin >> index;
            try { v.RemoveAt(index); }
            catch (const exception& e) { cout << "Ошибка: " << e.what() << endl; }
            break;
        case 5:
            cout << "Введите значение: "; cin >> val;
            cout << "Удалить все вхождения? (1 - да, 0 - только первое): "; cin >> index; // reuse index
            v.Remove(val, index == 1);
            break;
        case 6:
            v.PopFront();
            break;
        case 7:
            v.PopBack();
            break;
        case 8:
            v.Clear();
            break;
        case 9:
            cout << "Вектор: " << v << endl;
            break;
        case 10:
            v.Reverse();
            break;
        case 11:
            v.SortAsc();
            break;
        case 12:
            v.SortDesc();
            break;
        case 13:
            v.Shuffle();
            break;
        case 14:
            cout << "Сколько элементов сгенерировать? "; cin >> count;
            v.RandomFill(count);
            break;
        case 15:
            cout << "Введите значение: "; cin >> val;
            index = v.IndexOf(val);
            if (index == -1) cout << "Не найдено\n";
            else cout << "Индекс первого вхождения: " << index << endl;
            break;
        case 16:
            cout << "Введите значение: "; cin >> val;
            index = v.LastIndexOf(val);
            if (index == -1) cout << "Не найдено\n";
            else cout << "Индекс последнего вхождения: " << index << endl;
            break;
        case 17:
            v.TrimToSize();
            cout << "Новая ёмкость: " << v.getCapacity() << endl;
            break;
        case 18:
            cout << "Введите индекс: "; cin >> index;
            try { cout << "Элемент: " << v.GetElementAt(index) << endl; }
            catch (const exception& e) { cout << "Ошибка: " << e.what() << endl; }
            break;
        case 19:
            cout << (v.IsEmpty() ? "Вектор пуст" : "Вектор не пуст") << endl;
            break;
        case 20: {
            MyVector copy = v.Clone();
            cout << "Копия: " << copy << endl;
            break;
        }
        case 21:
            cout << "Введите индекс: "; cin >> index;
            if (index >= 0 && index < v.getSize())
                cout << "v[" << index << "] = " << v[index] << endl;
            else
                cout << "Индекс вне диапазона\n";
            break;
        case 22: {
            MyVector other;
            cout << "Создадим другой вектор для сравнения.\n";
            cout << "Введите его элементы через оператор >>\n";
            cin >> other;
            cout << "Другой вектор: " << other << endl;
            if (v == other) cout << "Векторы равны\n";
            else cout << "Векторы не равны\n";
            break;
        }
        case 23:
            cin >> v;
            break;
        case 0:
            break;
        default:
            cout << "Неверный выбор\n";
        }
    } while (choice != 0);
}

int main() {
    setlocale(LC_ALL, "Russian");
    int mainChoice;
    do {
        cout << "\n========== ГЛАВНОЕ МЕНЮ ==========\n";
        cout << "1. Тестировать класс Point\n";
        cout << "2. Тестировать класс Fraction\n";
        cout << "3. Тестировать класс MyVector\n";
        cout << "0. Выйти\n";
        cout << "Ваш выбор: ";
        cin >> mainChoice;

        switch (mainChoice) {
        case 1: demoPoint(); break;
        case 2: demoFraction(); break;
        case 3: demoMyVector(); break;
        case 0: cout << "До свидания!\n"; break;
        default: cout << "Неверный ввод\n";
        }
    } while (mainChoice != 0);
    return 0;
}