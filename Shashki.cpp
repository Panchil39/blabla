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

    // Метод для отображения доски в консоли
    void printBoard() const {
        // Вывод номеров столбцов
        cout << "  ";
        for (int col = 0; col < SIZE; ++col) {
            cout << col << " ";
        }
        cout << endl;
        
        // Вывод каждой строки доски
        for (int row = 0; row < SIZE; ++row) {
            cout << row << " ";  // Номер строки
            for (int col = 0; col < SIZE; ++col) {
                // Вывод символа для белой шашки
                if (board[row][col].color == PieceColor::WHITE) {
                    cout << (board[row][col].is_king ? "W " : "w ");
                } 
                // Вывод символа для черной шашки
                else if (board[row][col].color == PieceColor::BLACK) {
                    cout << (board[row][col].is_king ? "B " : "b ");
                } 
                // Вывод символа для пустой клетки
                else {
                    cout << ". ";
                }
            }
            cout << endl;
        }
    }
    
    // Метод для проверки, находится ли клетка в пределах доски
    bool isOnBoard(int row, int col) const {
        return row >= 0 && row < SIZE && col >= 0 && col < SIZE;
    }
    
    // Метод для проверки валидности хода
    bool isValidMove(int from_row, int from_col, int to_row, int to_col) {
        // Проверка, что начальная и конечная позиции на доске
        if (!isOnBoard(from_row, from_col) || !isOnBoard(to_row, to_col)) {
            return false;
        }
        
        // Проверка, что в начальной позиции стоит шашка текущего игрока
        if (board[from_row][from_col].color != current_player) {
            return false;
        }
        
        // Проверка, что конечная позиция пуста
        if (board[to_row][to_col].color != PieceColor::NONE) {
            return false;
        }
        
        // Шашки ходят только по черным клеткам (где сумма координат нечетная)
        if ((to_row + to_col) % 2 != 1) {
            return false;
        }
        
        // Вычисление разницы между начальной и конечной позицией
        int row_diff = to_row - from_row;
        int col_diff = to_col - from_col;
        
        // Проверка хода для обычной шашки (не дамки)
        if (!board[from_row][from_col].is_king) {
            // Проверка направления хода (белые ходят вверх, черные - вниз)
            if (current_player == PieceColor::WHITE) {
                // Белые могут ходить только на -1 или -2 по строке
                if (row_diff != -1 && row_diff != -2) {
                    return false;
                }
            } else {
                // Черные могут ходить только на +1 или +2 по строке
                if (row_diff != 1 && row_diff != 2) {
                    return false;
                }
            }
            
            // Ход должен быть строго по диагонали
            if (abs(col_diff) != abs(row_diff)) {
                return false;
            }
            
            // Простой ход (без взятия)
            if (abs(row_diff) == 1) {
                return true;
            }
            // Ход с взятием (через одну клетку)
            else if (abs(row_diff) == 2) {
                // Позиция шашки противника, которую бьем
                int mid_row = (from_row + to_row) / 2;
                int mid_col = (from_col + to_col) / 2;
                
                // Проверка, что между начальной и конечной позицией стоит шашка противника
                if (board[mid_row][mid_col].color != PieceColor::NONE && 
                    board[mid_row][mid_col].color != current_player) {
                    return true;
                }
            }
            return false;
        }
        // Проверка хода для дамки
        else {
            // Дамка ходит по диагонали на любое расстояние
            if (abs(row_diff) != abs(col_diff)) {
                return false;
            }
            
            // Определение направления движения
            int row_step = (row_diff > 0) ? 1 : -1;
            int col_step = (col_diff > 0) ? 1 : -1;
            int pieces_count = 0;  // Счетчик встреченных шашек на пути
            
            // Проверка всех клеток между начальной и конечной позицией
            for (int r = from_row + row_step, c = from_col + col_step; 
                 r != to_row; 
                 r += row_step, c += col_step) {
                // Если встретили шашку
                if (board[r][c].color != PieceColor::NONE) {
                    // Нельзя перепрыгивать через свои шашки
                    if (board[r][c].color == current_player) {
                        return false;
                    }
                    // Нельзя перепрыгивать более одной шашки противника
                    pieces_count++;
                    if (pieces_count > 1) {
                        return false;
                    }
                }
            }
            
            return true;
        }
    }
    
    // Метод для выполнения хода
    bool makeMove(int from_row, int from_col, int to_row, int to_col) {
        // Проверка валидности хода
        if (!isValidMove(from_row, from_col, to_row, to_col)) {
            return false;
        }
        
        // Вычисление разницы между начальной и конечной позицией
        int row_diff = to_row - from_row;
        int col_diff = to_col - from_col;
        
        // Перемещение шашки на новую позицию
        board[to_row][to_col] = board[from_row][from_col];
        // Очистка старой позиции
        board[from_row][from_col] = Piece(PieceColor::NONE);
        
        // Проверка на превращение в дамку
        if (!board[to_row][to_col].is_king) {
            // Белая шашка становится дамкой на первой строке (0)
            // Черная шашка становится дамкой на последней строке (SIZE-1)
            if ((current_player == PieceColor::WHITE && to_row == 0) || 
                (current_player == PieceColor::BLACK && to_row == SIZE - 1)) {
                board[to_row][to_col].is_king = true;
                cout << "Шашка превратилась в дамку!" << endl;
            }
        }
        
        // Обработка взятия шашки противника
        if (abs(row_diff) == 2 || abs(row_diff) > 2) {
            // Определение начальной и конечной позиции для поиска шашки противника
            int start_row = (row_diff > 0) ? from_row + 1 : from_row - 1;
            int end_row = (row_diff > 0) ? to_row - 1 : to_row + 1;
            int start_col = (col_diff > 0) ? from_col + 1 : from_col - 1;
            int end_col = (col_diff > 0) ? to_col - 1 : to_col + 1;
            
            // Поиск и удаление шашки противника между начальной и конечной позицией
            for (int r = start_row, c = start_col; 
                 (row_diff > 0) ? (r <= end_row) : (r >= end_row); 
                 (row_diff > 0) ? r++ : r--) {
                if (board[r][c].color != PieceColor::NONE && board[r][c].color != current_player) {
                    // Удаление шашки противника
                    board[r][c] = Piece(PieceColor::NONE);
                    // Уменьшение счетчика шашек
                    if (current_player == PieceColor::WHITE) {
                        black_pieces--;
                    } else {
                        white_pieces--;
                    }
                    break;
                }
                // Движение по столбцу в зависимости от направления
                (col_diff > 0) ? c++ : c--;
            }
        }
        
        // Проверка окончания игры (когда у одного из игроков не осталось шашек)
        if (white_pieces == 0 || black_pieces == 0) {
            game_over = true;
            cout << "Игра окончена! Победил " << 
                (current_player == PieceColor::WHITE ? "белый" : "черный") << " игрок!" << endl;
        }
        
        // Смена текущего игрока
        current_player = (current_player == PieceColor::WHITE) ? PieceColor::BLACK : PieceColor::WHITE;
        return true;
    }
    
    // Метод для выполнения хода компьютером
    void makeComputerMove() {
        vector<vector<int>> possible_moves;  // Вектор для хранения всех возможных ходов
        
        // Сначала ищем все возможные взятия
        for (int from_row = 0; from_row < SIZE; ++from_row) {
            for (int from_col = 0; from_col < SIZE; ++from_col) {
                // Если нашли шашку текущего игрока
                if (board[from_row][from_col].color == current_player) {
                    // Проверяем все возможные конечные позиции
                    for (int to_row = 0; to_row < SIZE; ++to_row) {
                        for (int to_col = 0; to_col < SIZE; ++to_col) {
                            // Если ход валиден и это взятие (ход через клетку)
                            if (isValidMove(from_row, from_col, to_row, to_col) && 
                                (abs(to_row - from_row) == 2 || abs(to_row - from_row) > 2)) {
                                // Добавляем ход в список возможных
                                possible_moves.push_back({from_row, from_col, to_row, to_col});
                            }
                        }
                    }
                }
            }
        }
        
        // Если не нашли взятий, ищем обычные ходы
        if (possible_moves.empty()) {
            for (int from_row = 0; from_row < SIZE; ++from_row) {
                for (int from_col = 0; from_col < SIZE; ++from_col) {
                    if (board[from_row][from_col].color == current_player) {
                        for (int to_row = 0; to_row < SIZE; ++to_row) {
                            for (int to_col = 0; to_col < SIZE; ++to_col) {
                                if (isValidMove(from_row, from_col, to_row, to_col)) {
                                    possible_moves.push_back({from_row, from_col, to_row, to_col});
                                }
                            }
                        }
                    }
                }
            }
        }
        
        // Если нашли возможные ходы
        if (!possible_moves.empty()) {
            // Выбираем случайный ход из возможных
            int random_index = rand() % possible_moves.size();
            vector<int> move = possible_moves[random_index];
            // Выполняем выбранный ход
            makeMove(move[0], move[1], move[2], move[3]);
            cout << "Компьютер сделал ход: " << move[0] << " " << move[1] << " " 
                 << move[2] << " " << move[3] << endl;
        } else {
            // Если нет возможных ходов, игра заканчивается
            game_over = true;
            cout << "Нет возможных ходов. Игра окончена!" << endl;
        }
    }
    
    // Геттер для текущего игрока
    PieceColor getCurrentPlayer() const {
        return current_player;
    }
    
    // Геттер для типа текущего игрока (человек/компьютер)
    PlayerType getCurrentPlayerType() const {
        return (current_player == PieceColor::WHITE) ? white_player : black_player;
    }
    
    // Геттер для проверки окончания игры
    bool isGameOver() const {
        return game_over;
    }
};

// Функция для запуска игры
void playCheckers() {
    cout << "Выберите тип игры:" << endl;
    cout << "1. Игрок vs Компьютер" << endl;
    cout << "2. Игрок vs Игрок" << endl;
    
    int choice;
    cin >> choice;
    
    PlayerType white_player, black_player;
    
    // Установка типов игроков в зависимости от выбора пользователя
    switch(choice) {
        case 1:
            white_player = PlayerType::HUMAN;
            black_player = PlayerType::COMPUTER;
            break;
        case 2:
            white_player = PlayerType::HUMAN;
            black_player = PlayerType::HUMAN;
            break;
        default:
            cout << "Неверный выбор. По умолчанию: Игрок vs Компьютер" << endl;
            white_player = PlayerType::HUMAN;
            black_player = PlayerType::COMPUTER;
    }
    
    // Создание экземпляра игры
    CheckersBoard game(white_player, black_player);
    
    // Основной игровой цикл
    while (!game.isGameOver()) {
        // Отображение текущего состояния доски
        game.printBoard();
        
        // Если текущий игрок - человек
        if (game.getCurrentPlayerType() == PlayerType::HUMAN) {
            cout << "Ход " << (game.getCurrentPlayer() == PieceColor::WHITE ? "белых" : "черных") << endl;
            cout << "Введите ход (from_row from_col to_row to_col): ";
            
            int from_row, from_col, to_row, to_col;
            cin >> from_row >> from_col >> to_row >> to_col;
            
            // Попытка выполнить ход
            if (!game.makeMove(from_row, from_col, to_row, to_col)) {
                cout << "Неверный ход! Попробуйте снова." << endl;
            }
        } 
        // Если текущий игрок - компьютер
        else {
            cout << "Ход компьютера (" << 
                (game.getCurrentPlayer() == PieceColor::WHITE ? "белые" : "черные") << ")..." << endl;
            game.makeComputerMove();
        }
    }
}

// Точка входа в программу
int main() {
    setlocale(LC_ALL, "Russian");  // Установка русской локали
    cout << "Добро пожаловать в игру Шашки!" << endl;
    cout << "Вводите ходы в формате: from_row from_col to_row to_col" << endl;
    cout << "Например: 5 0 4 1 - ход из клетки (5,0) в (4,1)" << endl;
    
    playCheckers();  // Запуск игры
    
    return 0;
}
