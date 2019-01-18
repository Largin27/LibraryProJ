#include <string>
#include <fstream>
#include <windows.h>
#include <iostream>
#include <conio.h>
#include <filesystem>
using namespace std;
namespace fs = std::filesystem;

#define maxline 30
#define maxcolumn 110

BOOL WINAPI SetConsoleCursorPosition(
	_In_ HANDLE hConsoleOutput, // descriptor
	_In_ COORD  dwCursorPosition // define pos
); // cursor pos

class Interface {
public: 
	
	Interface() {
		for (int i = 0; i < maxline; i++) {
			for (int j = 0; j < maxcolumn; j++) {
				matrix[i] += " ";
			}
		}
		Border();

		Print();
	}

	void Run() {
		Auth();
		if (auth == 1) {
			while (true) {
				Menu();
			}
		}
		else {
			return;
		}
	}


private:
	string matrix[maxline];
	string history[30];
	string key;
	string buffer;
	string buffer2;
	string check1;
	int count = 0;
	int flag = 0;
	char buff[128]; // validation
	string buff0; // crypt
	COORD scrn;
	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	string src;
	const char *src_db;
	const char *direct;
	int auth; // flg auth
	int cc = 2; // switcher menu
	int cl = 0; // menu flg
	int CurrentId; // userid

	void Print() {
		system("CLS");
		for (int i = 0; i < maxline; i++) {
			cout << matrix[i] << endl;
		}
	}

	void Auth() {
		ClearM();
		buffer = matrix[maxline / 2];
		matrix[maxline / 2 - 2].replace(maxcolumn / 3,24, "Введите данные для входа");
		matrix[maxline / 2].replace(maxcolumn / 3,6, "Логин:");
		scrn.X =43;
		scrn.Y =15;
		Print();
		SetConsoleCursorPosition(hOutput, scrn);
		cin >> buffer;
		
		for (int i = 1; i <= 5; i++) {
			src = "db/" + to_string(i) + ".vsc";
			src_db = src.c_str();
			if (Validation(src_db, 0) != "invalid") {
				CurrentId = i;
				ClearM();
				matrix[maxline / 2 - 2].replace(maxcolumn / 3, 24, "Введите данные для входа");
				matrix[maxline / 2].replace(maxcolumn / 3, 7, "Пароль:");

				Print();
				scrn.X = 44;
				SetConsoleCursorPosition(hOutput, scrn);
				cin >> buffer;
				if (Validation(src_db, 1) != "invalid") {
					    auth = 1;
						matrix[maxline / 2].replace(maxcolumn / 3, 10, "Все верно!");
						Print();
						Sleep(1000);
				}
				else {
					matrix[maxline / 2].replace(maxcolumn / 3, 16, "Неверный пароль!");
					Print();
					Sleep(2000);
					Auth();
					return;
				}
				return;
			}
			else {
				if (i == 5) {
					matrix[maxline / 2].replace(maxcolumn / 3, 50, "Извините, такого пользователя нет в нашей базе. :(");
					Print();
					Sleep(2000);
					Auth();
					return;
				}
				continue;
			}
		}
	}

	void Menu() {
		ClearM();
		matrix[maxline / 2 - 2].replace(maxcolumn / 3, 14, "Недавние книги");
		matrix[maxline / 2 - 1].replace(maxcolumn / 3, 27, "Посмотреть свой список книг");
		matrix[maxline / 2].replace(maxcolumn / 3, 34, "Добавить новую книгу из библиотеки");
		matrix[maxline / 2 - cc].replace(maxcolumn / 3 - 2, 1, "*");
		Print();
		while (true) {
			_getch();
			int keyp = _getch();
			if (keyp == 80) { // down
				if (cc == 0) {
					cc = 3;
				}
				cc--;
				ClearM();
				matrix[maxline / 2 - 2].replace(maxcolumn / 3, 14, "Недавние книги");
				matrix[maxline / 2 - 1].replace(maxcolumn / 3, 27, "Посмотреть свой список книг");
				matrix[maxline / 2].replace(maxcolumn / 3, 34, "Добавить новую книгу из библиотеки");
				matrix[maxline / 2 - cc].replace(maxcolumn / 3 - 2, 1, "*");

				Print();
			}
			else if (keyp == 72) { // up
				if (cc == 2) {
					cc = -1;
				}
				cc++;
				ClearM();
				matrix[maxline / 2 - 2].replace(maxcolumn / 3, 14, "Недавние книги");
				matrix[maxline / 2 - 1].replace(maxcolumn / 3, 27, "Посмотреть свой список книг");
				matrix[maxline / 2].replace(maxcolumn / 3, 34, "Добавить новую книгу из библиотеки");
				matrix[maxline / 2 - cc].replace(maxcolumn / 3 - 2, 1, "*");

				Print();
			}
			else if (keyp == 77) {
				switch(cc) {
				case 0:
					AddBook();
					break;
				case 1:
					ViewMy();
					break;
				case 2:
					History();
					break;
				}
				
				return;
			}
		}
		Sleep(1000);
	}
	
	void AddBook() {
		ClearM();
		int l = 0;
		for (auto& dir : fs::directory_iterator("book_base")) {
			matrix[maxline/2 - 10 + l].replace(maxcolumn / 6, dir.path().filename().generic_string().size(), dir.path().filename().generic_string());
			l++;
		}
		matrix[maxline / 2 - 10 + cl].replace(maxcolumn / 6 - 3, 2, "->");
		Print();
			while (true) {
				_getch();
				int keyp = _getch();
				if (keyp == 80) { // down
					if (cl == l-1) {
						cl = -1;
					}
					cl++;
					l = 0;
					ClearM();
					for (auto& dir : fs::directory_iterator("book_base")) {
						matrix[maxline / 2 - 10 + l].replace(maxcolumn / 6, dir.path().filename().generic_string().size(), dir.path().filename().generic_string());
						l++;
					}
					matrix[maxline / 2 - 10 + cl].replace(maxcolumn / 6 - 3, 2, "->");
					Print();
				}
				else if (keyp == 72) { // up
					if (cl == 0) {
						cl = l;
					}
					cl--;
					l = 0;
					ClearM();
					for (auto& dir : fs::directory_iterator("book_base")) {
						matrix[maxline / 2 - 10 + l].replace(maxcolumn / 6, dir.path().filename().generic_string().size(), dir.path().filename().generic_string());
						l++;
					}
					matrix[maxline / 2 - 10 + cl].replace(maxcolumn / 6 - 3, 2, "->");
					Print();
				}
				else if (keyp == 77) { // right
					l = 0;
					buffer2 = "C:/Users/dah41/source/repos/Library/Library/db/" + to_string(CurrentId) + "/";
					for (auto& dir : fs::directory_iterator("C:/Users/dah41/source/repos/Library/Library/book_base/")) {
						
						l++;
						if (cl == l - 1) {
							buffer = dir.path().generic_string();
							flag = 0;
							check1 = dir.path().filename().generic_string();
							for (auto& dir1 : fs::directory_iterator(buffer2)) {
									if (dir1.path().filename().generic_string() == check1) {
										flag = 1;
									}
							}
							if (flag == 1) {
								ClearM();
								matrix[maxline / 2 - 1].replace(maxcolumn / 5, 25, "У вас уже есть эта книга!");
								Print();
								Sleep(2000);
								return;
							}
							else {
								fs::copy(buffer, buffer2);
								return;
							}
							flag = 0;
						}
					}
				}
				else if (keyp == 75) {
					return;
				}
			}
	}

	void ViewMy() {
		ClearM();
		int l = 0;
		buffer = "db/" + to_string(CurrentId);
		for (auto& dir : fs::directory_iterator(buffer)) {
			matrix[maxline / 2 - 10 + l].replace(maxcolumn / 6, dir.path().filename().generic_string().size(), dir.path().filename().generic_string());
			l++;
		}
		if (l == 0) {
			ClearM();
			matrix[maxline / 2 - 1].replace(maxcolumn / 5, 53, "У вас пока что нет книг, добавьте их из библиотеки :)");
			Print();
			Sleep(4000);
		}
		else {
			cl = 0;
			matrix[maxline / 2 - 10 + cl].replace(maxcolumn / 6 - 3, 2, "->");
			Print();
			while (true) {
				_getch();
				int keyp = _getch();
				if (keyp == 80) { // down
					if (cl == l - 1) {
						cl = -1;
					}
					cl++;
					l = 0;
					ClearM();
					for (auto& dir : fs::directory_iterator(buffer)) {
						matrix[maxline / 2 - 10 + l].replace(maxcolumn / 6, dir.path().filename().generic_string().size(), dir.path().filename().generic_string());
						l++;
					}
					matrix[maxline / 2 - 10 + cl].replace(maxcolumn / 6 - 3, 2, "->");
					Print();
				}
				else if (keyp == 72) { // up
					if (cl == 0) {
						cl = l;
					}
					cl--;
					l = 0;
					ClearM();
					for (auto& dir : fs::directory_iterator(buffer)) {
						matrix[maxline / 2 - 10 + l].replace(maxcolumn / 6, dir.path().filename().generic_string().size(), dir.path().filename().generic_string());
						l++;
					}
					matrix[maxline / 2 - 10 + cl].replace(maxcolumn / 6 - 3, 2, "->");
					Print();
				}
				else if (keyp == 77) { // right
					l = 0;
					buffer2 = "C:/Users/dah41/source/repos/Library/Library/db/" + to_string(CurrentId);
					for (auto& dir : fs::directory_iterator(buffer2)) {
						l++;
						if (cl == l - 1) {
							/*buffer = "notepad.exe ";*/
							buffer = dir.path().generic_string();
							direct = buffer.c_str();
							system(direct);
							history[count] = dir.path().filename().generic_string();
							count++;
							if (count > 30) {
								count = 0;
							}
							return;
						}
					}
				}
				else if (keyp == 75) { //left
					return;
				}
				else if (keyp == 83) { //del
					l = 0;
					buffer2 = "C:/Users/dah41/source/repos/Library/Library/db/" + to_string(CurrentId);
					for (auto& dir : fs::directory_iterator(buffer2)) {
						l++;
						if (cl == l - 1) {
							fs::remove(dir.path());
							ClearM();
							for (auto& dir : fs::directory_iterator(buffer)) {
								matrix[maxline / 2 - 10 + l].replace(maxcolumn / 6, dir.path().filename().generic_string().size(), dir.path().filename().generic_string());
								l++;
							}
							matrix[maxline / 2 - 10 + cl].replace(maxcolumn / 6 - 3, 2, "->");
							Print();
							return;
						}
					}
				}
			}
		}
	}

	void Border() {
		for (int i = 0; i < maxline; i++) {
			for (int j = 0; j < maxcolumn; j++) {
				matrix[0][j] = '#';
				matrix[i][0] = '#';
				matrix[maxline - 1][j] = '#';
				matrix[i][maxcolumn - 1] = '#';
			}
		}
	}

	void ClearM() {
		for (int i = 0; i < maxline; i++) {
			matrix[i].clear();
		}
		for (int i = 0; i < maxline; i++) {
			for (int j = 0; j < maxcolumn; j++) {
				matrix[i] += " ";
			}
		}
		matrix[maxline - 2].replace(maxcolumn / 9 - 5, 93, "стрелки вверх/вниз - навигация, влево/вправо - переходы, del - удалить книгу из своей галереи");
		matrix[maxline - 4].replace(maxcolumn / 9, 81, "'Недавние книги' хранятся только в течении данной сессии (beta)");
		Border();
	}

	void History() {
		ClearM();
		for (int i = 0; i < count; i++) {
			matrix[maxline / 2 - 10 + i].replace(maxcolumn / 5 - 3, 2, to_string(i + 1) + ':');
			matrix[maxline / 2 - 10 + i].replace(maxcolumn / 5, history[i].size(), history[i]);
		}
		Print();
		Sleep(3000);
	}

	string Crypt(string buff) {
		key = "2015";
		buff0 = buff;
		if (key.length() < buff0.length()) {
		    for (int i = 0; i < buff0.length()-key.length()+1; i++) {
				key += key[i];
			}
		}
		char *b = new char[key.length()];
		for (int i = 0; i < buff0.length(); i++) {
			b[i] = key[i];
			buff0[i] = char(toascii(buff0[i]) + atoi(&b[i]));
		}
		delete b;
		return buff0;
	}

	string Validation(const char src[], int type) {
		if (type == 0) { //login
			fstream fin(src, ios_base::in);
			fin.getline(buff, 128, '\n');
			if (buff == Crypt(buffer)) {
				return buff;
				fin.close();
			}
			else {
				fin.close();
				return "invalid";
			}
		}
		else if (type == 1) { //pass
			fstream fin(src, ios_base::in);
			fin.getline(buff, 128, '\n');
			fin.getline(buff, 128, '\n');
			if (buff == Crypt(buffer)) {
				return buff;
				fin.close();
			} else {
				fin.close();
				return "invalid";
			}
		}
		else {
			return "invalid";
		}	
	}
};