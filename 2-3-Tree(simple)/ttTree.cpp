#include "ttTree.h"
#include <iomanip>
#include <sstream>

ttTree::ttTree() {
	root = NULL;
}

void ttTree::printTree(ostream& out) {
	out << "2-3 дерево индексы:\n-------------------------\n";
	printTreeHelper(root, out);
}

void ttTree::contains() {
	vector<int> lines;
	string input;
	cout << "Поиск строки: ";
	cin >> input;
	bool found = searchTreeHelper(root, input, lines);
	if (found) {
		cout << "Номер линии:";
		cout << " " << lines[0];
		for (int i = 1; i < lines.size(); i++)
			cout << ", " << lines[i];
	}
	else {
		cout << "\"" << input << "\" " << "нет в файле";
	}
	cout << endl;
}

void ttTree::buildTree(ifstream& input) {
	cout << "-----------------------------------------------------" << endl;
	int line = 1, numWords = 0, distWords = 0, treeHeight = 0;
	string tempLine, tempWord;

	while (!input.eof()) {

		getline(input, tempLine);
		for (int i = 0; i < tempLine.length(); i++) {
			while (tempLine[i] != ' ' && tempLine[i] != '\n' && i < tempLine.length()) {
				tempWord.insert(tempWord.end(), tempLine[i]);
				i++;
			}

			while (tempWord.length() > 0 && !isalnum(tempWord[tempWord.length() - 1]))
				tempWord.resize(tempWord.size() - 1);

			if (tempWord.length() > 0)
			{
				words.push_back(tempWord);
				root = insert(tempWord, line, root, distWords);
				numWords++;
				tempWord.clear();
			}
		}
		line++;
	}

	treeHeight = findHeight(root);

	cout << setw(40) << std::left;
	cout << "Всего строк: " << numWords << endl;

	cout << setw(40) << std::left
		<< "Общее количество отдельных слов: " << distWords << endl;

	cout << setw(40) << std::left
		<< "Высота 2-3 дерева: " << treeHeight << endl;
}

ttTree::node* ttTree::createNode(const string x, node* left, node* middle, node* right) {
	node* t = new node();
	t->leftKey = x;
	t->left = left;
	t->middle = middle;
	t->right = right;
	return t;
}

bool ttTree::isLeafNode(node* x) {
	if (x->left == NULL && x->middle == NULL && x->right == NULL) {
		return true;
	}
	else {
		return false;
	}
}

ttTree::node* ttTree::add(node* x, node* n) {
	if (x->rightKey.empty()) {
		if (x->leftKey < n->leftKey) {
			x->rightKey = n->leftKey; x->rightLines = n->leftLines;
			x->middle = n->left;
			x->right = n->middle;
		}
		else { //SWAP
			x->rightKey = x->leftKey; x->rightLines = x->leftLines;
			x->leftKey = n->leftKey; x->leftLines = n->leftLines;
			x->right = x->middle;
			x->middle = n->middle;
			x->left = n->left;
		}
		return x;
	}
	// ADD TO LEFT
	else if (x->leftKey >= n->leftKey) {
		node* newNode = createNode(x->leftKey, n, x, NULL);
		newNode->leftLines = x->leftLines;
		x->left = x->middle; x->middle = x->right; x->right = NULL;
		x->leftKey = x->rightKey; x->rightKey.clear();
		x->leftLines = x->rightLines; x->rightLines.clear();

		return newNode;
	}
	// ADD TO CENTER
	else if (x->rightKey >= n->leftKey) {
		node* newNode = createNode(x->rightKey, n->middle, x->right, NULL);
		newNode->leftLines = x->rightLines;
		x->middle = n->left;
		n->middle = newNode; n->left = x;
		x->rightKey.clear(); x->right = NULL; x->rightLines.clear();

		return n;
	}
	// ADD TO RIGHT
	else {
		node* newNode = createNode(x->rightKey, x, n, NULL);
		newNode->leftLines = x->rightLines;
		x->right = NULL; x->rightKey.clear(); x->rightLines.clear();

		return newNode;
	}
}

ttTree::node* ttTree::insert(const string& key, int line, node* root, int& distWord) {
	if (root == NULL) {
		node* newNode = createNode(key, NULL, NULL, NULL);
		newNode->leftLines.push_back(line);
		distWord++;
		return newNode;
	}
	if (key == root->leftKey || key == root->rightKey) { // Remove's duplicates
		if (key == root->leftKey)
			root->leftLines.push_back(line);
		else
			root->rightLines.push_back(line);
		return root;
	}
	if (isLeafNode(root)) {
		node* newNode = createNode(key, NULL, NULL, NULL);
		newNode->leftLines.push_back(line);
		node* finalNode = add(root, newNode);
		distWord++;
		return finalNode;
	}
	// LEFT
	if (key < root->leftKey) {
		node* newNode = insert(key, line, root->left, distWord);
		if (newNode == root->left)
			return root;
		else
			return add(root, newNode);
	}
	// MIDDLE
	else if (root->rightKey.empty() || key < root->rightKey) {
		node* newNode = insert(key, line, root->middle, distWord);
		if (newNode == root->middle)
			return root;
		else
			return add(root, newNode);
	}
	// RIGHT
	else {
		node* newNode = insert(key, line, root->right, distWord);
		if (newNode == root->right)
			return root;
		else
			return add(root, newNode);
	}
}

bool ttTree::searchTreeHelper(node* x, string value, vector<int>& lines) {
	if (root == NULL) {
		return false;
	}
	if (value == x->leftKey) {
		lines = x->leftLines;
		return true;
	}
	if (value == x->rightKey) {
		lines = x->rightLines;
		return true;
	}
	if (isLeafNode(x))
		return false;
	else if (!x->rightKey.empty()) {
		if (value < x->leftKey) {
			return searchTreeHelper(x->left, value, lines);
		}
		else if (value > x->leftKey&& value < x->rightKey) {
			return searchTreeHelper(x->middle, value, lines);
		}
		else {
			return searchTreeHelper(x->right, value, lines);
		}
	}
	else {
		if (value < x->leftKey)
			return searchTreeHelper(x->left, value, lines);
		else
			return searchTreeHelper(x->middle, value, lines);
	}
}

void ttTree::printLines(node* x, bool isLeft, ostream& out) {
	if (isLeft) {
		if (!x->leftLines.empty()) {
			out << " " << x->leftLines[0];
			for (int i = 1; i < x->leftLines.size(); i++)
				out << ", " << x->leftLines[i];
		}
	}
	else {
		if (!x->rightLines.empty()) {
			out << " " << x->rightLines[0];
			for (int i = 1; i < x->rightLines.size(); i++)
				out << ", " << x->rightLines[i];
		}
	}
}

void ttTree::print(node* x, ostream& out) {
	if (x != NULL) {
		if (x->rightKey.empty()) {
			out << setw(30) << std::left;
			out << x->leftKey; printLines(x, true, out);
			out << endl;
		}
		else {
			out << setw(30) << std::left;
			out << x->leftKey; printLines(x, true, out);
			out << endl;
			out << setw(30) << std::left;
			out << x->rightKey; printLines(x, false, out);
			out << endl;
		}
	}
}

void ttTree::printTreeHelper(node* x, ostream& out) { //Inorder
	if (x != NULL) {
		if (isLeafNode(x)) {
			print(x, out);
		}
		else {
			printTreeHelper(x->left, out);
			out << setw(30) << std::left;
			out << x->leftKey; printLines(x, true, out);
			out << endl;
			if (x->middle) {
				printTreeHelper(x->middle, out);
				if (!x->rightKey.empty()) {
					out << setw(30) << std::left;
					out << x->rightKey; printLines(x, false, out);
					out << endl;
				}
			}
			printTreeHelper(x->right, out);
		}
	}
}

int ttTree::findHeight(node* x) {
	if (x == NULL)
		return 0;
	else {
		int leftHeight = findHeight(x->left), rightHeight = findHeight(x->right);
		int middleHeight = findHeight(x->middle);
		if (leftHeight > rightHeight&& leftHeight > middleHeight)
			return(leftHeight + 1);
		else if (rightHeight > leftHeight&& rightHeight > middleHeight)
			return (rightHeight + 1);
		else
			return(middleHeight + 1);
	}
}