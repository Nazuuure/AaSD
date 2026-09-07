#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

using namespace std;

int countLetters(const string& str) {
	int count = 0;
	for (int i = 0; str[i] != '\0'; i++) {
		++count;
	}
	return count;
}

string typeOfLetter(char ch) {
	int chCode = (unsigned char)ch;
	if ((chCode >= 0xC0 && chCode <= 0xDF) || (chCode >= 0xE0 && chCode <= 0xFF) || (chCode == 0x2D) || (chCode == 0x96) || (chCode == 0x97)) {
		switch (chCode) {
		case 0x2D: case 0x96: // разные тире, дефисы и тп
		case 0x97:
			return "split";
			break;
		case 0xE0: case 0xC0: // а А
		case 0xE5: case 0xC5: // е Е
		case 0xB8: case 0xA8: // ё Ё
		case 0xE8: case 0xC8: // и И
		case 0xEE: case 0xCE: // о О
		case 0xF3: case 0xD3: // у У
		case 0xFB: case 0xDB: // ы Ы
		case 0xFD: case 0xDD: // э Э
		case 0xFE: case 0xDE: // ю Ю
		case 0xFF: case 0xDF: // я Я
			return "vowel";
			break;
		case 0xFA: case 0xCA: // ъ Ъ
		case 0xFC: case 0xCC: // ь Ь
			return "special";
			break;
		default:
			return "consonant";
		}
	}
	else {
		return "other";
	}
}

string formatter(string inStr) {
	int strLen = countLetters(inStr);
	string cleanStr;
	string formattedStr;

	for (char ch : inStr) {
		if (typeOfLetter(ch) != "other") {
			cleanStr += ch;
		}
	}

	if (cleanStr == "") {
		return "";
	}

	bool lastWasSplit = false;
	bool hasLetter = false;

	for (char ch : cleanStr) {
		if (typeOfLetter(ch) == "split") {
			if (!lastWasSplit && hasLetter) {
				formattedStr += '-';
			}
			lastWasSplit = true;
		}
		else {
			formattedStr += ch;
			hasLetter = true;
			lastWasSplit = false;
		}
	}

	return formattedStr;
}

bool canSplit(char nextCh1, char nextCh2) {
	if ((typeOfLetter(nextCh1) == "consonant") && (typeOfLetter(nextCh2) == "consonant")) {
		return false;
	}
	else if ((typeOfLetter(nextCh1) == "special") || (typeOfLetter(nextCh2) == "special")) {
		return false;
	}
	else {
		return true;
	}
}

int findStartIndex(string inWord) {
	int strLength = countLetters(inWord);
	for (int i = 0; i < strLength; i++) {
		if (typeOfLetter(inWord[i]) == "vowel") {
			return i;
		}
	}
	return 0;
}

string addHyphens(const string& word) {
	int strLength = countLetters(word);

	if (strLength < 4) {
		return word;
	}

	int vowCount = 0;
	for (short i = 0; i < strLength; i++) {
		if (typeOfLetter(word[i]) == "vowel") {
			vowCount++;
		}
	}

	if (vowCount <= 1) {
		return word;
	}

	int startIndex = findStartIndex(word);
	string resultStr;

	for (int i = 0; i < startIndex; i++) {
		resultStr += word[i];
	}

	char nextCh1 = '#';
	char nextCh2 = '#';

	for (int i = startIndex; i < strLength - 2; i++) {
		resultStr += word[i];
		nextCh1 = word[i + 1];
		nextCh2 = word[i + 2];

		if (canSplit(nextCh1, nextCh2)) {
			resultStr += '-';
		}
	}
	resultStr += word[strLength - 2];
	resultStr += word[strLength - 1];
	return resultStr;
}

string splitWord(const string& word) {
	int hyphIndex = 0;
	for (char ch : word) {
		if (typeOfLetter(ch) == "split") {
			break;
		}
		hyphIndex += 1;
	}
	if (hyphIndex == countLetters(word)) {
		return addHyphens(word);
	}
	else {
		string tempWord1;
		string tempWord2;
		for (int i = 0; i < hyphIndex; i++) {
			tempWord1 += word[i];
		}
		for (int i = hyphIndex + 1; i < countLetters(word); i++) {
			tempWord2 += word[i];
		}
		return addHyphens(tempWord1) + "-" + splitWord(tempWord2);
	}	
}

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	string operatingWord;
	string inFileSrc;
	string outFileSrc;

	cout << "Введите путь к input-файлу: ";
	cin >> inFileSrc;
	cout << "Введите путь к output-файлу: ";
	cin >> outFileSrc;

	ifstream inFile(inFileSrc);
	ofstream outFile(outFileSrc, ios_base::out);

	if (!inFile.is_open()) {
		cout << "Ошибка: не удалось открыть файл " << inFileSrc << endl;
		return 1;
	}

	if (!outFile.is_open()) {
		cout << "Ошибка: не удалось открыть файл " << outFileSrc << endl;
		return 1;
	}

	while (inFile >> operatingWord) {
		string validWord = formatter(operatingWord);
		if (validWord == "") {
			continue;
		}
		string resultWord = splitWord(validWord);
		outFile << resultWord << endl;
		cout << resultWord << endl;
	}

	inFile.close();
	outFile.close();
	return 0;
}