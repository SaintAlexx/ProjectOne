#include <iostream>
#include <string>
#include <ctime>
using namespace std;

int game_field[100][100];
// "-1" - мина
// "0-8" - любая другая клетка, где число означает количество мин вокруг
int player_field[100][100];
// "-2" - неизвестная игроку клетка
// "-1" - флаг, который игрок поставил
// "0-8" - открытая любая другая клетка, где число означает количество мин вокруг
const int ROWS = 9, COLUMNS = 9, MINES = 10;


void fill_space_around_mine(int i, int j) { // функция для заполнения поля вокруг мины.

	for (int k = i - 1; k <= i + 1; k++) {
		for (int m = j - 1; m <= j + 1; m++) {
			if (k >= 0 && k < ROWS && m >= 0 && m <= COLUMNS && game_field[k][m] != -1)
				game_field[k][m]++;
		}
	}
}

bool out_of_bounds(int x, int y, bool condition) {
	return x < 0 || x >= ROWS || y < 0 || y >= COLUMNS || condition;
}

void print_field() {
	system("cls"); // очистка экрана
	cout << "  ";
	for (int i = 1; i <= COLUMNS; i++) { //заполнить поле для игры
		cout << " " << i << " ";
	}
	cout << endl;
	for (int i = 0; i < ROWS; i++) {	//вывод значений (флагов/мин/не открытых клеток) в игровое поле
		cout << i + 1 << "  ";
		for (int j = 0; j < COLUMNS; j++) {
			switch (player_field[i][j]) {
			case -1: cout << "f  "; break;
			case -2: cout << "_  "; break;
			case -3: cout << "*  "; break;
			default: cout << player_field[i][j] << "  "; break;
			}
		}
		cout << endl;
	}
	cout << endl << endl << endl;
}

void depth_first_search(int x, int y) { //Функция вывода похожих значений в поле игры
	if (out_of_bounds(x, y, player_field[x][y] > -2)) return; //Если клетка неизвестна игроку, то ничего не выводить
	player_field[x][y] = game_field[x][y];
	if (game_field[x][y] > 0) return;
	for (int i = x - 1; i <= x + 1; ++i) {
		for (int j = y - 1; j <= y + 1; ++j) {
			if (i != x || j != y)
				depth_first_search(i, j);
		}
	}
}

bool open_cell(int x, int y) {	//открыть клетку по координатам x; y
	if (game_field[x][y] == -1) return false; //Если игрок открыл клетку с миной, то возвращать false
	if (game_field[x][y] > 0) { //Если игрок открыл клетку без мины, то вернуть true
		player_field[x][y] = game_field[x][y];
		return true;
	}
	depth_first_search(x, y);
}

bool is_win() { //функция выйгрыша
	int opened = 0;
	for (int i = 0; i < ROWS; i++) { //открыть всё 
		for (int j = 0; j < COLUMNS; j++) {
			if (player_field[i][j] >= 0) opened++;
		}
	}
	return (ROWS * COLUMNS - MINES == opened);
}

void end_game(bool is_win) { //функция для завершения игры, если игрок попал на мину.
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLUMNS; j++) {
			if (player_field[i][j] == -1) player_field[i][j] = -2;
			if (game_field[i][j] == -1) player_field[i][j] = -3;
		}
	}
	print_field();
	cout << "You " << (is_win ? "WIN" : "LOSE") << "!\nEnter any key to restart:\n";
}

int main()
{
	string command = "\\n";
	while (command != "\\q") {
		srand(time(0));
		for (int i = 0; i < ROWS; i++) {
			for (int j = 0; j < COLUMNS; j++) {
				game_field[i][j] = 0;
				player_field[i][j] = -2;
			}
		}
		int i = 0;
		while (i < MINES) { //Рандомная расстановка мин
			int x = rand() % ROWS;
			int y = rand() % COLUMNS;
			if (game_field[x][y] != -1) {
				game_field[x][y] = -1;
				i++;
			}
		}

		for (int i = 0; i < ROWS; i++) { //Заполнение полей вокруг мин
			for (int j = 0; j < COLUMNS; j++) {
				if (game_field[i][j] == -1) {
					fill_space_around_mine(i, j);
				}
			}
		}
		bool flag = true;
		while (flag) {
			print_field();
			cout << "Enter the command:";
			// открыть клетку "\o x y"
			// поставить флаг "\f x y"
			// новая игра "\n"
			// выйти из игры "\q"

			cin >> command;

			if (command == "\\o") { //Если команда равна "\\o" то ввести координаты точки, 
				int x, y;
				cin >> x >> y;
				--x; --y;
				if (out_of_bounds(x, y, player_field[x][y] >= 0)) continue; //продолжить, если игрок не попал на мину
				if (!open_cell(x, y)) { //открыть указанную игроком клетку
					end_game(false);
					cin >> command;
					flag = false;
				}
				if (is_win()) { //завершить игру, если игрок выйграл
					end_game(true);
					cin >> command;
					flag = false;
				}
			}
			else if (command == "\\f") { //иначе если команда равна "\\f", то ввести координаты точки
				int x, y;
				cin >> x >> y;
				--x; --y;
				if (out_of_bounds(x, y, player_field[x][y] >= 0)) continue; //продолжить, если точка >=0
				if (player_field[x][y] == -1) player_field[x][y] = -2; //Если точка указанная игроком -1, то поставить на игровое поле флаг
				else player_field[x][y] = -1;
			}
			else if (command == "\\n") {
				flag = false;
			}
			else if (command == "\\q") {
				flag = false;
			}
		}
	}
	return 0;
}


