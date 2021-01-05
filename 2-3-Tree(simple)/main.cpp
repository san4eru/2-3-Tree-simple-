#include <iostream>
#include <fstream>
#include "ttTree.h"
using namespace std;

int main() {
	int choice;

	system("chcp 1251->null");

	ifstream input("char.txt");

	if (input.is_open()) {

		ttTree myTree;
		myTree.buildTree(input);
		input.close();
		
		cout << endl;
		cout << "=====================================================" << endl;
		cout << "- МЕНЮ 2-3 дерева" << endl;
		cout << "=====================================================" << endl;
		cout << "(1) показать индексы " << endl;
		cout << "(2) поиск " << endl;
		cout << "(3) сохранить индексы " << endl;
		cout << "(4) выйти " << endl;
		while (1) {
			choice = 0;
			cout << "Действие: ";
			cin >> choice;

			if (choice == 1)
				myTree.printTree(cout);

			else if (choice == 2)
				myTree.contains();

			else if (choice == 3) {
				string outputFile;
				cout << "Введите имя файла: ";
				cin >> outputFile;
				ofstream output(outputFile.c_str());
				myTree.printTree(output);
				output.close();
				cout << "Сохранен\n";
			}
			else
				break;
		}
	}

	else {
		cout << "Неверное имя файла.\n";
		return 2;
	}

	cout << endl;
	return 0;
}