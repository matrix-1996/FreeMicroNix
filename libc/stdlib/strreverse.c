void strreverse(char* begin, char* end) {
	char aux;
	while (end > begin)
		aux = *end, *end-- = *begin, *begin++ = aux;
}