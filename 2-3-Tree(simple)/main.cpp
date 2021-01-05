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
		cout << "- ���� 2-3 ������" << endl;
		cout << "=====================================================" << endl;
		cout << "(1) �������� ������� " << endl;
		cout << "(2) ����� " << endl;
		cout << "(3) ��������� ������� " << endl;
		cout << "(4) ����� " << endl;
		while (1) {
			choice = 0;
			cout << "��������: ";
			cin >> choice;

			if (choice == 1)
				myTree.printTree(cout);

			else if (choice == 2)
				myTree.contains();

			else if (choice == 3) {
				string outputFile;
				cout << "������� ��� �����: ";
				cin >> outputFile;
				ofstream output(outputFile.c_str());
				myTree.printTree(output);
				output.close();
				cout << "��������\n";
			}
			else
				break;
		}
	}

	else {
		cout << "�������� ��� �����.\n";
		return 2;
	}

	cout << endl;
	return 0;
}