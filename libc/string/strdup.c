#include <string.h>

char* strdup(const char* str) {
	char* ret, *tmp;
	size_t len;
 
	if (!str)
		return 0;
       
	len = 0;
	tmp = (char*)str;
	while(*(tmp++))
		len++;
	ret = (char*) malloc(len + 1);
	tmp = ret;
	while(len--)
		*(tmp++) = *(str++);
	*tmp = '\0';
 
	return ret;
}