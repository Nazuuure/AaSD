#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

int countLetters(const std::string& str) {
	int count = 0;
	for (int i = 0; str[i] != '\0'; i++) {
		++count;
	}
	return count;
}

std::string typeOfLetter(char ch) {
	int chCode = (unsigned char)ch;
	if ((chCode >= 0xC0 && chCode <= 0xDF) || (chCode >= 0xE0 && chCode <= 0xFF) || (chCode == 0x2D) || (chCode == 0x96) || (chCode == 0x97) || (chCode == 0xA8) || (chCode == 0xB8)) {
		switch (ch) {
		case '-': case '–': case '—':
			return "split";
		case 'а': case 'А':
		case 'е': case 'Е':
		case 'ё': case 'Ё':
		case 'и': case 'И':
		case 'о': case 'О':
		case 'у': case 'У':
		case 'ы': case 'Ы':
		case 'э': case 'Э':
		case 'ю': case 'Ю':
		case 'я': case 'Я':
			return "vowel";
		case 'ъ': case 'Ъ':
		case 'ь': case 'Ь':
			return "special";
		default:
			return "consonant";
		}
	}
	else {
		return "other";
	}
}

std::string formatWord(std::string inStr) {
	int strLen = countLetters(inStr);
	std::string cleanStr;
	std::string formattedStr;

	for (char ch : inStr) {
		if (typeOfLetter(ch) != "other") {
			cleanStr += ch;
		}
	}

	int cleanStrLen = countLetters(cleanStr);
	int lastLetterIndex = cleanStrLen - 1;
	while (lastLetterIndex > 0 && typeOfLetter(cleanStr[lastLetterIndex]) == "split") {
		lastLetterIndex--;
	}

	std::string strForFormating;
	for (int i = 0; i <= lastLetterIndex; i++) {
		strForFormating += cleanStr[i];
	}

	if (cleanStr == "") {
		return "";
	}

	bool lastWasSplit = false;
	bool hasLetter = false;

	for (char ch : strForFormating) {
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

int findStartIndex(std::string inWord) {
	int strLength = countLetters(inWord);
	for (int i = 0; i < strLength; i++) {
		if (typeOfLetter(inWord[i]) == "vowel") {
			return i;
		}
	}
	return 0;
}

std::string addHyphens(const std::string& word) {
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
	std::string resultStr;

	for (int i = 0; i < startIndex; i++) {
		resultStr += word[i];
	}

	char nextCh1 = '#';
	char nextCh2 = '#';

	for (int i = startIndex; i < strLength - 2; i++) {
		resultStr += word[i];
		nextCh1 = word[i + 1];
		nextCh2 = word[i + 2];

		if (countLetters(resultStr) > 1 && canSplit(nextCh1, nextCh2)) {
			resultStr += '-';
		}
	}
	resultStr += word[strLength - 2];
	resultStr += word[strLength - 1];
	return resultStr;
}

std::string splitWord(const std::string& word) {
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
		std::string tempWord1;
		std::string tempWord2;
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

	std::string operatingWord;
	std::string inFileSrc;
	std::string outFileSrc;

	std::cout << "Введите путь к input-файлу: ";
	std::cin >> inFileSrc;
	std::cout << "Введите путь к output-файлу: ";
	std::cin >> outFileSrc;

	std::ifstream inFile(inFileSrc);
	std::ofstream outFile(outFileSrc, std::ios_base::out);

	if (!inFile.is_open()) {
		std::cout << "Ошибка: не удалось input-открыть файл " << inFileSrc << std::endl;
		return 1;
	}

	if (!outFile.is_open()) {
		std::cout << "Ошибка: не удалось output-открыть файл " << outFileSrc << std::endl;
		return 1;
	}

	while (inFile >> operatingWord) {
		std::string validWord = formatWord(operatingWord);
		if (validWord == "") {
			continue;
		}
		std::string resultWord = splitWord(validWord);
		outFile << resultWord << std::endl;
		std::cout << resultWord << std::endl;
	}

	inFile.close();
	outFile.close();
	return 0;
}