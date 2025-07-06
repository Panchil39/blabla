#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cstdlib>

using namespace std;

// ========== КОНСТАНТЫ И ПЕРЕЧИСЛЕНИЯ ==========
enum class PieceColor { WHITE, BLACK, NONE };
enum class PlayerType { HUMAN, COMPUTER };
const int BOARD_SIZE = 8;

// ========== СТРУКТУРЫ ДАННЫХ ==========
struct Piece {
    PieceColor color;
    bool is_king;

    Piece(PieceColor c = PieceColor::NONE, bool king = false)
        : color(c), is_king(king) {
    }
};

// ========== КЛАСС ИГРОВОЙ ДОСКИ ==========
class CheckersBoard {
private:
static const int SIZE = 8;  // Размер доски (8x8)
     vector<vector<Piece>> board;  // Двумерный вектор для хранения состояния доски
    PieceColor current_player;    // Текущий игрок (чей ход)
    bool game_over;              // Флаг окончания игры
    int white_pieces;           // Количество белых шашек
    int black_pieces;           // Количество черных шашек
    PlayerType white_player;     // Тип игрока для белых
    PlayerType black_player;     // Тип игрока для черных

public:
    // Конструктор
    CheckersBoard(PlayerType white = PlayerType::HUMAN,
        PlayerType black = PlayerType::COMPUTER);
: current_player(PieceColor::WHITE), // Белые ходят первыми
          game_over(false),                 // Игра только начинается
          white_pieces(12),                 // Начальное количество белых шашек
          black_pieces(12),                 // Начальное количество черных шашек
          white_player(white),              // Установка типа игрока для белых
          black_player(black) {             // Установка типа игрока для черных
        board.resize(SIZE, vector<Piece>(SIZE));  // Инициализация доски
        setupBoard();                            // Начальная расстановка шашек
        srand(time(0));                          // Инициализация генератора случайных чисел
    }

    // Настройка начальной позиции
    void setupBoard() {
        // Проходим по всем клеткам доски
        for (int row = 0; row < SIZE; ++row) {
            for (int col = 0; col < SIZE; ++col) {
                // Шашки располагаются только на черных клетках (где сумма row+col нечетная)
                if ((row + col) % 2 == 1) {
                    // Первые 3 ряда - черные шашки
                    if (row < 3) {
                        board[row][col] = Piece(PieceColor::BLACK);
                    } 
                    // Последние 3 ряда - белые шашки
                    else if (row > 4) {
                        board[row][col] = Piece(PieceColor::WHITE);
                    } 
                    // Средние 2 ряда - пустые клетки
                    else {
                        board[row][col] = Piece(PieceColor::NONE);
                    }
                } 
                // Белые клетки всегда пустые
                else {
                    board[row][col] = Piece(PieceColor::NONE);
                }
            }
        }
    }

    // Отображение доски
    void printBoard() const;

    // Проверка позиции на доске
    bool isOnBoard(int row, int col) const;

    // Проверка валидности хода
    bool isValidMove(int from_row, int from_col, int to_row, int to_col);

    // Выполнение хода
    bool makeMove(int from_row, int from_col, int to_row, int to_col);

    // Ход компьютера
    void makeComputerMove();

    // Геттеры
    PieceColor getCurrentPlayer() const;
    PlayerType getCurrentPlayerType() const;
    bool isGameOver() const;
};

// ========== РЕАЛИЗАЦИЯ МЕТОДОВ КЛАССА ==========

// Конструктор
CheckersBoard::CheckersBoard(PlayerType white, PlayerType black)
    : current_player(PieceColor::WHITE), game_over(false),
    white_pieces(12), black_pieces(12),
    white_player(white), black_player(black) {
    board.resize(BOARD_SIZE, vector<Piece>(BOARD_SIZE));
    setupBoard();
    srand(time(0));
}

// Настройка начальной позиции
void CheckersBoard::setupBoard() {
    // Реализация...
}

// Отображение доски
void CheckersBoard::printBoard() const {
    // Реализация...
}

// Проверка позиции на доске
bool CheckersBoard::isOnBoard(int row, int col) const {
    // Реализация...
}

// Проверка валидности хода
bool CheckersBoard::isValidMove(int from_row, int from_col,
    int to_row, int to_col) {
    // Реализация...
}

// Выполнение хода
bool CheckersBoard::makeMove(int from_row, int from_col,
    int to_row, int to_col) {
    // Реализация...
}

// Ход компьютера
void CheckersBoard::makeComputerMove() {
    // Реализация...
}

// Геттеры
PieceColor CheckersBoard::getCurrentPlayer() const {
    // Реализация...
}

PlayerType CheckersBoard::getCurrentPlayerType() const {
    // Реализация...
}

bool CheckersBoard::isGameOver() const {
    // Реализация...
}

// ========== ФУНКЦИИ ИГРОВОГО ПРОЦЕССА ==========
void showMenu() {
    cout << "=== МЕНЮ ===" << endl;
    cout << "1. Игрок vs Компьютер" << endl;
    cout << "2. Игрок vs Игрок" << endl;
    cout << "Выберите режим игры: ";
}

void playGame(PlayerType white, PlayerType black) {
    CheckersBoard game(white, black);

    while (!game.isGameOver()) {
        game.printBoard();

        if (game.getCurrentPlayerType() == PlayerType::HUMAN) {
            // Обработка хода человека
        }
        else {
            // Обработка хода компьютера
        }
    }
}

// ========== ОСНОВНАЯ ФУНКЦИЯ ==========
int main() {
    setlocale(LC_ALL, "");

    // Вывод приветствия
    cout << "Добро пожаловать в игру Шашки!" << endl;

    // Отображение меню
    showMenu();

    // Выбор режима игры
    int choice;
    cin >> choice;

    // Запуск игры в выбранном режиме
    switch (choice) {
    case 1:
        playGame(PlayerType::HUMAN, PlayerType::COMPUTER);
        break;
    case 2:
        playGame(PlayerType::HUMAN, PlayerType::HUMAN);
        break;
    default:
        cout << "Неверный выбор. Запуск по умолчанию: Игрок vs Компьютер" << endl;
        playGame(PlayerType::HUMAN, PlayerType::COMPUTER);
    }

    return 0;
}
