/*
Homework #8
name: Guanming Lin
sbuid: 109299260
*/
#include <stdio.h>
#include "hw8.h"

size_t hw_strlen(const char *str) {
	size_t length = 0;
	if (str == NULL) {
		return 0;
	}
	while (*(str++) != '\0') {
		length++;
	}
	return length;
}

int hw_strcmp(const char *str1, const char *str2) {
	int i;
	if (str1 == NULL || str2 == NULL) {
		return 0;
	}
	if (hw_strlen(str1) != hw_strlen(str2)) {
		return 0;
	}
	for (i = 0; i < hw_strlen(str1); i++) {
		if (*(str1++) != *(str2++)) {
			return 0;
		}
	}
	return 1;
}

char* hw_strncpy(char *dst, const char *src, size_t n) {
	int i;
	if (dst == NULL || src == NULL) {
		return NULL;
	}
	if (n >= hw_strlen(src)) {
		for (i = 0; i < hw_strlen(src); i++) {
			*(dst + i) = *(src + i);
		}
		*(dst + i) = '\0';
	}
	else {
		for (i = 0; i < n; i++) {
			*(dst + i) = *(src + i);
		}
		*(dst + i) = '\0';
	}
	return dst;
}

int hw_indexof(const char *str, char c) {
	int i;
	if (str == NULL) {
		return -1;
	}
	for (i = 0; i < hw_strlen(str); i++) {
		if (*(str + i) == c) {
			return i;
		}
	}
	return -1;
}

void hw_reversestr(char *str) {
	int i;
	int j;
	char c;
	if (str == NULL) {
		return;
	}
	i = 0;
	j = hw_strlen(str) - 1;
	while (i < j) {
		c = *(str + i);
		*(str + i) = *(str + j);
		*(str + j) = c;
		i++;
		j--;
	}
}

int hw_equalsIgnoreCase(const char *str1, const char *str2) {
	int i;
	if (str1 == NULL || str2 == NULL) {
		return 0;
	}
	if (hw_strlen(str1) != hw_strlen(str2)) {
		return 0;
	}
	for (i = 0; i < hw_strlen(str1); i++) {
		if (*(str1 + i) >= 'a' && *(str1 + i) <= 'z') {
			if (*(str1 + i) != *(str2 + i) && *(str2 + i) - *(str1 + i) != 'A' - 'a') {
				return 0;
			}
		}
		else if (*(str1 + i) >= 'A' && *(str1 + i) <= 'Z') {
			if (*(str1 + i) != *(str2 + i) && *(str1 + i) - *(str2 + i) != 'A' - 'a') {
				return 0;
			}
		}
		else {
			if (*(str1 + i) != *(str2 + i)) {
				return 0;
			}
		}
	}
	return 1;
}

void hw_replaceall(char *str, const char *pattern, char replacement) {
	int i;
	if (str == NULL || pattern == NULL) {
		return;
	}
	for (i = 0; i < hw_strlen(str); i++) {
		if (hw_indexof(pattern, *(str + i)) != -1) {
			*(str + i) = replacement;
		}
	}
}

char* hw_expandtabs(const char *str, size_t tabsize) {
	int i;
	int tab_count = 0;
	char *expanded_str;
	int position = 0;
	int j;
	if (str == NULL) {
		return 0;
	}
	for (i = 0; i < hw_strlen(str); i++) {
		if (*(str + i) == '\t') {
			tab_count++;
		}
	}
	expanded_str = malloc(hw_strlen(str) - tab_count + tab_count * (int)tabsize + 1);
	if (expanded_str == NULL) {
		return NULL;
	}
	for (i = 0; i < hw_strlen(str); i++) {
		if (*(str + i) == '\t') {
			for (j = 0; j < (int)tabsize; j++) {
				printf("j: %d\n", j);
				*(expanded_str + position + j) = ' ';
			}
			position += (int)tabsize;
		}
		else {
			*(expanded_str + position) = *(str + i);
			position++;
		}
	}
	*(expanded_str + position) = '\0';
	return expanded_str;
}

char** hw_split(const char *str, char c) {
	char *splitted_str;
	int i;
	int count = 0;
	char **tokens;
	int position = 0;
	if (str == NULL) {
		return NULL;
	}
	splitted_str = malloc(hw_strlen(str) + 1);
	if (splitted_str == NULL) {
		return NULL;
	}
	for (i = 0; i < hw_strlen(str); i++) {
		if (*(str + i) == c) {
			*(splitted_str + i) = '\0';
		}
		else {
			*(splitted_str + i) = *(str + i);
		}
	}
	*(splitted_str + i) = '\0';
	for (i = 0; i < hw_strlen(str) - 1; i++) {
		if (i == 0 && *splitted_str != '\0') {
			count++;
		}
		else if (*(splitted_str + i) == '\0' && *(splitted_str + i + 1) != '\0') {
			count++;
		}
	}
	tokens = (char **)malloc(count * (sizeof(char *) + 1));
	if (tokens == NULL) {
		free(splitted_str);
		splitted_str = NULL;
	}
	for (i = 0; i < hw_strlen(str) - 1; i++) {
		if (position == count) {
			break;
		}
		if (i == 0 && *splitted_str != '\0') {
			*tokens = splitted_str;
			position++;
		}
		else if (*(splitted_str + i) == '\0' && *(splitted_str + i + 1) != '\0') {
			*(tokens + position) = splitted_str + i + 1;
			position++;
		}
	}
	*(tokens + position) = NULL;
	return tokens;
}

int hw_findAndReplace(char **dst, const char *str, const char *find_str,
const char* replace_str) {
	int i;
	int count = 0;
	int j;
	int position = 0;
	if (str == NULL || find_str == NULL || replace_str == NULL) {
		return 0;
	}
	for (i = 0; i < hw_strlen(str) - hw_strlen(find_str) + 1; i++) {
		for (j = 0; j < hw_strlen(find_str); j++) {
			if (*(str + i + j) != *(find_str + j)) {
				break;
			}
			if (j == hw_strlen(find_str) - 1) {
				count++;
				i += hw_strlen(find_str) - 1;
				break;
			}
		}
	}
	*dst = malloc(hw_strlen(str) + count * (hw_strlen(replace_str) - hw_strlen(find_str)) + 1);
	if (*dst == NULL) {
		return 0;
	}
	for (i = 0; i < hw_strlen(str); i++) {
		if (i < hw_strlen(str) - hw_strlen(find_str) + 1) {
			for (j = 0; j < hw_strlen(find_str); j++) {
				if (*(str + i + j) != *(find_str + j)) {
					*(*dst + position) = *(str + i);
					position++;
					break;
				}
				if (j == hw_strlen(find_str) - 1) {
					hw_strncpy(*dst + position, replace_str, hw_strlen(replace_str));
					i += hw_strlen(find_str) - 1;
					position += hw_strlen(replace_str);
					break;
				}
			}
		}
		else {
			*(*dst + position) = *(str + i);
			position++;
		}
	}
	*(*dst + position) = '\0';
	return count;
}

void hw_swapTokens(char *str, size_t i, size_t j, const char
*delimiters) {
	char *buffer;
	int k;
	char **tokens;
	int num_tokens = 0;
	char *temp;
	int position = 0;
	int tokens_placed = 0;
	if (str == NULL ||  delimiters == NULL || i < 0 || j < 0) {
		return;
	}
	buffer = malloc(hw_strlen(str) + 1);
	if (buffer == NULL) {
		return;
	}
	for (k = 0; k < hw_strlen(str); k++) {
		if (hw_indexof(delimiters, *(str + k)) != -1) {

			*(buffer + k) = *delimiters;
		}
		else {
			*(buffer + k) = *(str + k);
		}
	}
	*(buffer + k) = '\0';
	tokens = hw_split(buffer, *delimiters);
	while (*(tokens + num_tokens) != '\0') {
		num_tokens++;
	}
	if ((int)i >= num_tokens || (int)j >= num_tokens) {
		free(buffer);
		buffer = NULL;
		free(tokens);
		tokens = NULL;
		return;
	}
	else {
		temp = tokens[(int)i];
		tokens[(int)i] = tokens[(int)j];
		tokens[(int)j] = temp;
		for (k = 0; k < hw_strlen(str); k++) {
			if (k == 0 && hw_indexof(delimiters, *str) == -1) {
				if (tokens_placed < num_tokens) {
					hw_strncpy(buffer + position, tokens[tokens_placed], hw_strlen(tokens[tokens_placed]));
					position += hw_strlen(tokens[tokens_placed]);
					tokens_placed++;
				}
			}
			else if (hw_indexof(delimiters, *(str + k)) != -1) {
				*(buffer + position) = *(str + k);
				position++;
				if (k < hw_strlen(str) - 1 && hw_indexof(delimiters, *(str + k + 1)) == -1) {
					if (tokens_placed < num_tokens) {
						hw_strncpy(buffer + position, tokens[tokens_placed], hw_strlen(tokens[tokens_placed]));
						position += hw_strlen(tokens[tokens_placed]);
						tokens_placed++;
					}
				}
			}
		}
		*(buffer + position) = '\0';
		hw_strncpy(str, buffer, hw_strlen(str));
		free(buffer);
		buffer = NULL;
		free(tokens);
		tokens = NULL;
	}
}

int ec_findAndReplace(char **dst, const char *str, const char *find_str,
const char* replace_str) {
	int i;
	int count = 0;
	int j;
	int position = 0;
	if (str == NULL || find_str == NULL || replace_str == NULL) {
		return 0;
	}
	for (i = 0; i < hw_strlen(str) - hw_strlen(find_str) + 1; i++) {
		for (j = 0; j < hw_strlen(find_str); j++) {
			if (*(str + i + j) != *(find_str + j) && !(*(find_str + j) == '*' && *(str + i + j) != ' ')) {
				break;
			}
			if (j == hw_strlen(find_str) - 1) {
				count++;
				i += hw_strlen(find_str) - 1;
				break;
			}
		}
	}
	*dst = malloc(hw_strlen(str) + count * (hw_strlen(replace_str) - hw_strlen(find_str)) + 1);
	if (*dst == NULL) {
		return 0;
	}
	for (i = 0; i < hw_strlen(str); i++) {
		if (i < hw_strlen(str) - hw_strlen(find_str) + 1) {
			for (j = 0; j < hw_strlen(find_str); j++) {
				if (*(str + i + j) != *(find_str + j) && !(*(find_str + j) == '*' && *(str + i + j) != ' ')) {
					*(*dst + position) = *(str + i);
					position++;
					break;
				}
				if (j == hw_strlen(find_str) - 1) {
					hw_strncpy(*dst + position, replace_str, hw_strlen(replace_str));
					i += hw_strlen(find_str) - 1;
					position += hw_strlen(replace_str);
					break;
				}
			}
		}
		else {
			*(*dst + position) = *(str + i);
			position++;
		}
	}
	*(*dst + position) = '\0';
	return count;
}