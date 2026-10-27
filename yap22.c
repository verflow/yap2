#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>
#include <time.h>
// C:\Users\User\Desktop\test.txt
#define NOT_USED -2
#define USED -1
#define FIRST -3

struct letter {
	char character;
	short value;
};

struct letter letters_arr[26];
char words[20][10];
int used[10];//использованные цифры
char need[10];//буквы, которые надо заменить
int K, L; //  - число слов. L - максимальна€ длина.

int letter_value(char a) {
	return letters_arr[(int)a - 65].value;
}

void printer(char* str) {
	int i = 0;
	while (str[i] != 0) {
		if (isalpha(str[i]))
			printf("%d", letter_value(str[i]));
		else printf("%c", str[i]);
		i++;
	}
}


int numbers_need(int num) {
	for (int i = 0; i < K; i++) {
		char c = words[i][9 - num];
		if (c != 0 && (letter_value(c) == USED || letter_value(c) == FIRST)) return 1;
	}
	return 0;
}

int znak(char* word, int num) {
	char c = word[9 - num];
	if (c == 0) return 0;
	return letter_value(c);
}

int parser(char* str) {
	int i = 0; char w[10] = { 0 }; int j = 0; int k = 0;
	do {
		if (isalpha(str[i])) {
			if (j == 0) letters_arr[str[i] - 65].value = -5;
			w[j++] = str[i];
		}
		if (!isalpha(str[i + 1]) && j > 0) {
			for (int count = 0; count < j; count++) {
				words[k][10 - j + count] = w[count];
				w[count] = 0;
			}
			L = j;
			j = 0;
			k++;
		}
		i++;
	} while (str[i] != '\0');
	K = k;
	k = 0;
	for (i = 9; i >= 0; i--)//потому что нужны в правильном пор€дке.
		for (j = 0; j < K; j++) {
			if (words[j][i] == 0)continue;
			if (letter_value(words[j][i]) == NOT_USED || letter_value(words[j][i]) == -5) {
				need[k] = words[j][i];
				if (letter_value(words[j][i]) == NOT_USED) letters_arr[need[k] - 65].value = USED;
				if (letter_value(words[j][i]) == -5) letters_arr[need[k] - 65].value = FIRST;
				k++;
			}
		}
	return k;
}
int solve(int column, int done, int trans) {
	while (column < L && !numbers_need(column)) {
		int sum = trans;
		for (int i = 0; i < (K - 1); i++)
			sum += znak(words[i], column);
		if ((sum % 10) != znak(words[K - 1], column)) return 0;
		trans = sum / 10;
		column++;
	}
	if (L == column) return trans == 0;
	for (int i = 0; i < 10; i++) {
		if (used[i]) continue;
		int old = letters_arr[need[done] - 65].value;
		if (old == FIRST && i == 0)continue;
		letters_arr[need[done] - 65].value = i; //забавный момент
		used[i] = 1;
		if (solve(column, done + 1, trans)) return 1;
		letters_arr[need[done] - 65].value = old;
		used[i] = 0;
	}return 0;
}
void init_all_by_zero(void) {
for (int i = 0; i < 26; i++) {
	if (i < 20) {
		for (int j = 0; j < 10; j++)
			words[i][j] = 0;
	}
	if(i < 10) {
	used[i] = 0;
	need[i] = 0;
	}
letters_arr[i].character = (char)(i + 65);
letters_arr[i].value = NOT_USED;
}
	K = 0; L = 0;
}
int main() {
	char path[64];
	printf("get path inside\n");
	scanf("%63s", path);
	FILE* f = fopen(path, "r");
	if (f == NULL) return 1;
	char str[64] = { 0 };

	clock_t total = 0;
	clock_t t0 = clock();
	while (fgets(str, 64, f)) {
		init_all_by_zero();
		parser(str);
		if (solve(0, 0, 0))
			printer(str);
		else printf("no solution");
	}
	fclose(f);
	total += clock() - t0;
	printf("\nsolve: %.3f ms\n", 1000.0 * total / CLOCKS_PER_SEC);
}