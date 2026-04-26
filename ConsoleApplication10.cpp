#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <stdexcept>

class Matrix {
private:
    std::vector<std::vector<double>> data;
    int rows, cols;

    // Вспомогательный метод для создания единичной матрицы
    static Matrix identity(int n) {
        Matrix result(n, n);
        for (int i = 0; i < n; i++)
            result.data[i][i] = 1.0;
        return result;
    }

public:
    // Конструкторы
    Matrix() : rows(0), cols(0) {}

    Matrix(int r, int c) : rows(r), cols(c) {
        data.assign(rows, std::vector<double>(cols, 0.0));
    }

    Matrix(int r, int c, double init) : rows(r), cols(c) {
        data.assign(rows, std::vector<double>(cols, init));
    }

    Matrix(const Matrix& other) : rows(other.rows), cols(other.cols), data(other.data) {}

    // Оператор присваивания
    Matrix& operator=(const Matrix& other) {
        if (this != &other) {
            rows = other.rows;
            cols = other.cols;
            data = other.data;
        }
        return *this;
    }

    // Доступ к элементам
    double& operator()(int i, int j) { return data[i][j]; }
    const double& operator()(int i, int j) const { return data[i][j]; }

    // Геттеры
    int getRows() const { return rows; }
    int getCols() const { return cols; }

    // ==================== АРИФМЕТИЧЕСКИЕ ОПЕРАЦИИ ====================

    Matrix operator+(const Matrix& other) const {
        if (rows != other.rows || cols != other.cols)
            throw std::invalid_argument("Размеры матриц не совпадают для сложения");
        Matrix result(rows, cols);
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                result.data[i][j] = data[i][j] + other.data[i][j];
        return result;
    }

    Matrix operator-(const Matrix& other) const {
        if (rows != other.rows || cols != other.cols)
            throw std::invalid_argument("Размеры матриц не совпадают для вычитания");
        Matrix result(rows, cols);
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                result.data[i][j] = data[i][j] - other.data[i][j];
        return result;
    }

    Matrix operator*(const Matrix& other) const {
        if (cols != other.rows)
            throw std::invalid_argument("Неправильные размеры матриц для умножения");
        Matrix result(rows, other.cols);
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < other.cols; j++)
                for (int k = 0; k < cols; k++)
                    result.data[i][j] += data[i][k] * other.data[k][j];
        return result;
    }

    Matrix operator*(double scalar) const {
        Matrix result(rows, cols);
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                result.data[i][j] = data[i][j] * scalar;
        return result;
    }

    Matrix operator/(double scalar) const {
        if (scalar == 0)
            throw std::invalid_argument("Деление на ноль");
        Matrix result(rows, cols);
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                result.data[i][j] = data[i][j] / scalar;
        return result;
    }

    // ==================== КОМБИНИРОВАННЫЕ ОПЕРАЦИИ ====================

    Matrix& operator+=(const Matrix& other) { *this = *this + other; return *this; }
    Matrix& operator-=(const Matrix& other) { *this = *this - other; return *this; }
    Matrix& operator*=(const Matrix& other) { *this = *this * other; return *this; }
    Matrix& operator*=(double scalar) { *this = *this * scalar; return *this; }
    Matrix& operator/=(double scalar) { *this = *this / scalar; return *this; }

    // ==================== ОПЕРАЦИИ СРАВНЕНИЯ ====================

    bool operator==(const Matrix& other) const {
        if (rows != other.rows || cols != other.cols) return false;
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                if (std::fabs(data[i][j] - other.data[i][j]) > 1e-9)
                    return false;
        return true;
    }

    bool operator!=(const Matrix& other) const { return !(*this == other); }

    // ==================== МАТЕМАТИЧЕСКИЕ ОПЕРАЦИИ ====================

    Matrix transpose() const {
        Matrix result(cols, rows);
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                result.data[j][i] = data[i][j];
        return result;
    }

    double determinant() const {
        if (rows != cols)
            throw std::invalid_argument("Определитель существует только для квадратных матриц");
        Matrix temp = *this;
        double det = 1.0;
        for (int i = 0; i < rows; i++) {
            int pivot = i;
            for (int j = i + 1; j < rows; j++)
                if (std::fabs(temp.data[j][i]) > std::fabs(temp.data[pivot][i]))
                    pivot = j;
            if (std::fabs(temp.data[pivot][i]) < 1e-9)
                return 0.0;
            if (pivot != i) {
                std::swap(temp.data[i], temp.data[pivot]);
                det = -det;
            }
            det *= temp.data[i][i];
            for (int j = i + 1; j < rows; j++) {
                double factor = temp.data[j][i] / temp.data[i][i];
                for (int k = i + 1; k < cols; k++)
                    temp.data[j][k] -= factor * temp.data[i][k];
            }
        }
        return det;
    }

    Matrix inverse() const {
        if (rows != cols)
            throw std::invalid_argument("Обратная матрица существует только для квадратных матриц");
        double det = determinant();
        if (std::fabs(det) < 1e-9)
            throw std::invalid_argument("Матрица вырожденная, обратной не существует");

        int n = rows;
        Matrix aug(n, 2 * n);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++)
                aug.data[i][j] = data[i][j];
            aug.data[i][n + i] = 1.0;
        }

        for (int i = 0; i < n; i++) {
            double pivot = aug.data[i][i];
            if (std::fabs(pivot) < 1e-9) {
                int swapRow = -1;
                for (int j = i + 1; j < n; j++) {
                    if (std::fabs(aug.data[j][i]) > 1e-9) {
                        swapRow = j;
                        break;
                    }
                }
                if (swapRow == -1) throw std::invalid_argument("Обратная матрица не существует");
                std::swap(aug.data[i], aug.data[swapRow]);
                pivot = aug.data[i][i];
            }
            for (int j = 0; j < 2 * n; j++)
                aug.data[i][j] /= pivot;
            for (int j = 0; j < n; j++) {
                if (j != i) {
                    double factor = aug.data[j][i];
                    for (int k = 0; k < 2 * n; k++)
                        aug.data[j][k] -= factor * aug.data[i][k];
                }
            }
        }

        Matrix result(n, n);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                result.data[i][j] = aug.data[i][n + j];
        return result;
    }

    Matrix power(int exponent) const {
        if (rows != cols)
            throw std::invalid_argument("Возведение в степень только для квадратных матриц");
        if (exponent == 0) return identity(rows);
        if (exponent < 0) return inverse().power(-exponent);
        Matrix result = identity(rows);
        Matrix base = *this;
        while (exponent > 0) {
            if (exponent & 1) result = result * base;
            base = base * base;
            exponent >>= 1;
        }
        return result;
    }

    double norm() const {
        double maxSum = 0.0;
        for (int j = 0; j < cols; j++) {
            double colSum = 0.0;
            for (int i = 0; i < rows; i++)
                colSum += std::fabs(data[i][j]);
            if (colSum > maxSum) maxSum = colSum;
        }
        return maxSum;
    }

    // ==================== ПРОВЕРКА ТИПОВ МАТРИЦЫ ====================

    bool isSquare() const { return rows == cols; }

    bool isZero() const {
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                if (std::fabs(data[i][j]) > 1e-9) return false;
        return true;
    }

    bool isIdentity() const {
        if (!isSquare()) return false;
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++) {
                if (i == j && std::fabs(data[i][j] - 1.0) > 1e-9) return false;
                if (i != j && std::fabs(data[i][j]) > 1e-9) return false;
            }
        return true;
    }

    bool isDiagonal() const {
        if (!isSquare()) return false;
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                if (i != j && std::fabs(data[i][j]) > 1e-9) return false;
        return true;
    }

    bool isSymmetric() const {
        if (!isSquare()) return false;
        for (int i = 0; i < rows; i++)
            for (int j = i + 1; j < cols; j++)
                if (std::fabs(data[i][j] - data[j][i]) > 1e-9) return false;
        return true;
    }

    bool isUpperTriangular() const {
        if (!isSquare()) return false;
        for (int i = 1; i < rows; i++)
            for (int j = 0; j < i; j++)
                if (std::fabs(data[i][j]) > 1e-9) return false;
        return true;
    }

    bool isLowerTriangular() const {
        if (!isSquare()) return false;
        for (int i = 0; i < rows; i++)
            for (int j = i + 1; j < cols; j++)
                if (std::fabs(data[i][j]) > 1e-9) return false;
        return true;
    }

    // ==================== ВВОД/ВЫВОД ====================

    void input() {
        std::cout << "  Введите элементы матрицы " << rows << "x" << cols << ":\n";
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                std::cin >> data[i][j];
    }

    void print() const {
        for (int i = 0; i < rows; i++) {
            std::cout << "  ";
            for (int j = 0; j < cols; j++)
                std::cout << std::setw(10) << std::setprecision(4) << data[i][j] << " ";
            std::cout << std::endl;
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrix& m) {
        for (int i = 0; i < m.rows; i++) {
            os << "  ";
            for (int j = 0; j < m.cols; j++)
                os << std::setw(10) << std::setprecision(4) << m.data[i][j] << " ";
            os << std::endl;
        }
        return os;
    }

    friend std::istream& operator>>(std::istream& is, Matrix& m) {
        for (int i = 0; i < m.rows; i++)
            for (int j = 0; j < m.cols; j++)
                is >> m.data[i][j];
        return is;
    }
};

// ==================== ФУНКЦИИ ДЛЯ ОФОРМЛЕНИЯ (только ASCII) ====================

void printHeader(const std::string& title) {
    std::cout << "\n  +--------------------------------------------------------------+\n";
    std::cout << "  |  " << std::left << std::setw(58) << title << "|\n";
    std::cout << "  +--------------------------------------------------------------+\n";
}

void printMenu() {
    std::cout << "\n  +--------------------------------------------------------------+\n";
    std::cout << "  |                         ГЛАВНОЕ МЕНЮ                         |\n";
    std::cout << "  +--------------------------------------------------------------+\n";
    std::cout << "  |  1. Создать матрицу A                   2. Создать матрицу B |\n";
    std::cout << "  |  3. A + B                               4. A - B             |\n";
    std::cout << "  |  5. A * B                               6. A * скаляр        |\n";
    std::cout << "  |  7. A / скаляр                          8. A += B            |\n";
    std::cout << "  |  9. A -= B                             10. A *= B            |\n";
    std::cout << "  | 11. A *= скаляр                        12. A /= скаляр       |\n";
    std::cout << "  | 13. A == B                             14. A != B            |\n";
    std::cout << "  | 15. Транспонирование A                 16. Определитель A    |\n";
    std::cout << "  | 17. Обратная матрица A                 18. A^N (степень)     |\n";
    std::cout << "  | 19. Норма матрицы A                    20. Проверка типов A  |\n";
    std::cout << "  | 21. Вывести A                          22. Вывести B         |\n";
    std::cout << "  | 23. Ввести A заново                    24. Ввести B заново   |\n";
    std::cout << "  |  0. Выход                                                    |\n";
    std::cout << "  +--------------------------------------------------------------+\n";
    std::cout << "  Ваш выбор: ";
}

void printTypeCheck(const Matrix& m, const std::string& name) {
    std::cout << "\n  +------------------------------------------------------------+\n";
    std::cout << "  | Проверка типов матрицы " << name << ":                        |\n";
    std::cout << "  +------------------------------------------------------------+\n";
    std::cout << "  | Квадратная:          " << (m.isSquare() ? "| Да" : "| Нет") << "\n";
    if (m.isSquare()) {
        std::cout << "  | Диагональная:        " << (m.isDiagonal() ? "| Да" : "| Нет") << "\n";
        std::cout << "  | Единичная:           " << (m.isIdentity() ? "| Да" : "| Нет") << "\n";
        std::cout << "  | Симметрическая:      " << (m.isSymmetric() ? "| Да" : "| Нет") << "\n";
        std::cout << "  | Верхнетреугольная:   " << (m.isUpperTriangular() ? "| Да" : "| Нет") << "\n";
        std::cout << "  | Нижнетреугольная:    " << (m.isLowerTriangular() ? "| Да" : "| Нет") << "\n";
    }
    std::cout << "  | Нулевая:             " << (m.isZero() ? "| Да" : "| Нет") << "\n";
    std::cout << "  +------------------------------------------------------------+\n";
}

// ==================== ГЛАВНАЯ ФУНКЦИЯ ====================

int main() {
    setlocale(LC_ALL, "Russian");

    Matrix A, B;
    bool hasA = false, hasB = false;
    int choice, exponent;
    double scalar;

    std::cout << "\n  +--------------------------------------------------------------+\n";
    std::cout << "  |         ЛАБОРАТОРНАЯ РАБОТА №6                                |\n";
    std::cout << "  |         Класс \"Вещественная матрица\"                         |\n";
    std::cout << "  +--------------------------------------------------------------+\n";

    do {
        printMenu();
        std::cin >> choice;

        switch (choice) {
        case 1:
        {
            int r, c;
            std::cout << "  Введите размеры матрицы A (строки столбцы): ";
            std::cin >> r >> c;
            A = Matrix(r, c);
            std::cout << "  Ввод матрицы A:\n";
            A.input();
            hasA = true;
            printHeader("МАТРИЦА A");
            std::cout << A;
        }
        break;

        case 2:
        {
            int r, c;
            std::cout << "  Введите размеры матрицы B (строки столбцы): ";
            std::cin >> r >> c;
            B = Matrix(r, c);
            std::cout << "  Ввод матрицы B:\n";
            B.input();
            hasB = true;
            printHeader("МАТРИЦА B");
            std::cout << B;
        }
        break;

        case 3:
            if (hasA && hasB) {
                printHeader("СЛОЖЕНИЕ МАТРИЦ A + B");
                try { std::cout << (A + B) << std::endl; }
                catch (const std::exception& e) { std::cout << "  Ошибка: " << e.what() << std::endl; }
            }
            else std::cout << "  Сначала создайте обе матрицы!\n";
            break;

        case 4:
            if (hasA && hasB) {
                printHeader("ВЫЧИТАНИЕ МАТРИЦ A - B");
                try { std::cout << (A - B) << std::endl; }
                catch (const std::exception& e) { std::cout << "  Ошибка: " << e.what() << std::endl; }
            }
            else std::cout << "  Сначала создайте обе матрицы!\n";
            break;

        case 5:
            if (hasA && hasB) {
                printHeader("УМНОЖЕНИЕ МАТРИЦ A * B");
                try { std::cout << (A * B) << std::endl; }
                catch (const std::exception& e) { std::cout << "  Ошибка: " << e.what() << std::endl; }
            }
            else std::cout << "  Сначала создайте обе матрицы!\n";
            break;

        case 6:
            if (hasA) {
                printHeader("УМНОЖЕНИЕ A НА СКАЛЯР");
                std::cout << "  Введите скаляр: ";
                std::cin >> scalar;
                std::cout << (A * scalar) << std::endl;
            }
            else std::cout << "  Сначала создайте матрицу A!\n";
            break;

        case 7:
            if (hasA) {
                printHeader("ДЕЛЕНИЕ A НА СКАЛЯР");
                std::cout << "  Введите скаляр: ";
                std::cin >> scalar;
                try { std::cout << (A / scalar) << std::endl; }
                catch (const std::exception& e) { std::cout << "  Ошибка: " << e.what() << std::endl; }
            }
            else std::cout << "  Сначала создайте матрицу A!\n";
            break;

        case 8:
            if (hasA && hasB) {
                printHeader("ОПЕРАЦИЯ A += B");
                try {
                    A += B;
                    std::cout << "  Результат:\n" << A;
                }
                catch (const std::exception& e) { std::cout << "  Ошибка: " << e.what() << std::endl; }
            }
            else std::cout << "  Сначала создайте обе матрицы!\n";
            break;

        case 9:
            if (hasA && hasB) {
                printHeader("ОПЕРАЦИЯ A -= B");
                try {
                    A -= B;
                    std::cout << "  Результат:\n" << A;
                }
                catch (const std::exception& e) { std::cout << "  Ошибка: " << e.what() << std::endl; }
            }
            else std::cout << "  Сначала создайте обе матрицы!\n";
            break;

        case 10:
            if (hasA && hasB) {
                printHeader("ОПЕРАЦИЯ A *= B");
                try {
                    A *= B;
                    std::cout << "  Результат:\n" << A;
                }
                catch (const std::exception& e) { std::cout << "  Ошибка: " << e.what() << std::endl; }
            }
            else std::cout << "  Сначала создайте обе матрицы!\n";
            break;

        case 11:
            if (hasA) {
                printHeader("ОПЕРАЦИЯ A *= СКАЛЯР");
                std::cout << "  Введите скаляр: ";
                std::cin >> scalar;
                A *= scalar;
                std::cout << "  Результат:\n" << A;
            }
            else std::cout << "  Сначала создайте матрицу A!\n";
            break;

        case 12:
            if (hasA) {
                printHeader("ОПЕРАЦИЯ A /= СКАЛЯР");
                std::cout << "  Введите скаляр: ";
                std::cin >> scalar;
                try {
                    A /= scalar;
                    std::cout << "  Результат:\n" << A;
                }
                catch (const std::exception& e) { std::cout << "  Ошибка: " << e.what() << std::endl; }
            }
            else std::cout << "  Сначала создайте матрицу A!\n";
            break;

        case 13:
            if (hasA && hasB) {
                printHeader("СРАВНЕНИЕ A == B");
                std::cout << "  Результат: " << (A == B ? "true (матрицы равны)" : "false (матрицы не равны)") << std::endl;
            }
            else std::cout << "  Сначала создайте обе матрицы!\n";
            break;

        case 14:
            if (hasA && hasB) {
                printHeader("СРАВНЕНИЕ A != B");
                std::cout << "  Результат: " << (A != B ? "true (матрицы не равны)" : "false (матрицы равны)") << std::endl;
            }
            else std::cout << "  Сначала создайте обе матрицы!\n";
            break;

        case 15:
            if (hasA) {
                printHeader("ТРАНСПОНИРОВАНИЕ МАТРИЦЫ A");
                std::cout << A.transpose() << std::endl;
            }
            else std::cout << "  Сначала создайте матрицу A!\n";
            break;

        case 16:
            if (hasA) {
                printHeader("ОПРЕДЕЛИТЕЛЬ МАТРИЦЫ A");
                try { std::cout << "  det(A) = " << std::setprecision(6) << A.determinant() << std::endl; }
                catch (const std::exception& e) { std::cout << "  Ошибка: " << e.what() << std::endl; }
            }
            else std::cout << "  Сначала создайте матрицу A!\n";
            break;

        case 17:
            if (hasA) {
                printHeader("ОБРАТНАЯ МАТРИЦА A⁻¹");
                try { std::cout << A.inverse() << std::endl; }
                catch (const std::exception& e) { std::cout << "  Ошибка: " << e.what() << std::endl; }
            }
            else std::cout << "  Сначала создайте матрицу A!\n";
            break;

        case 18:
            if (hasA) {
                printHeader("ВОЗВЕДЕНИЕ A В СТЕПЕНЬ");
                std::cout << "  Введите степень: ";
                std::cin >> exponent;
                try { std::cout << (A.power(exponent)) << std::endl; }
                catch (const std::exception& e) { std::cout << "  Ошибка: " << e.what() << std::endl; }
            }
            else std::cout << "  Сначала создайте матрицу A!\n";
            break;

        case 19:
            if (hasA) {
                printHeader("НОРМА МАТРИЦЫ A");
                std::cout << "  ||A|| = " << std::setprecision(6) << A.norm() << std::endl;
            }
            else std::cout << "  Сначала создайте матрицу A!\n";
            break;

        case 20:
            if (hasA) printTypeCheck(A, "A");
            else std::cout << "  Сначала создайте матрицу A!\n";
            break;

        case 21:
            if (hasA) {
                printHeader("МАТРИЦА A");
                std::cout << A;
            }
            else std::cout << "  Матрица A не создана!\n";
            break;

        case 22:
            if (hasB) {
                printHeader("МАТРИЦА B");
                std::cout << B;
            }
            else std::cout << "  Матрица B не создана!\n";
            break;

        case 23:
            if (hasA) {
                int r, c;
                std::cout << "  Введите размеры матрицы A (строки столбцы): ";
                std::cin >> r >> c;
                A = Matrix(r, c);
                std::cout << "  Ввод матрицы A:\n";
                A.input();
                std::cout << "  Матрица A обновлена:\n" << A;
            }
            else std::cout << "  Сначала создайте матрицу A!\n";
            break;

        case 24:
            if (hasB) {
                int r, c;
                std::cout << "  Введите размеры матрицы B (строки столбцы): ";
                std::cin >> r >> c;
                B = Matrix(r, c);
                std::cout << "  Ввод матрицы B:\n";
                B.input();
                std::cout << "  Матрица B обновлена:\n" << B;
            }
            else std::cout << "  Сначала создайте матрицу B!\n";
            break;

        case 0:
            std::cout << "\n  +--------------------------------------------------------------+\n";
            std::cout << "  |                    Работа завершена. До свидания!            |\n";
            std::cout << "  +--------------------------------------------------------------+\n";
            break;

        default:
            std::cout << "  Неверный выбор! Попробуйте снова.\n";
        }
    } while (choice != 0);

    return 0;
}