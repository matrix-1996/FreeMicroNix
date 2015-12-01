#include <i386/global.h>
#include <i386/kprintf.h>
#include <kernel/tty.h>
#include <i386/mm.h>

int kvsnprintf(char* str, size_t size, const char* fmt, va_list va) {
	char tmp[22];	// Enough to hold a 64-bit unsigned integer.
	char ch;
	char* buf = str;
	char* sztmp;
	int i;

	while ((ch = *(fmt++))) {
		if ((size_t)(buf - str) == size-1)
			ch = 0;
		if (ch != '%') {
			*buf++ = ch;
		} else if (ch) {
			int w = 0;
			bool pf = false;
			int length = 0;
		nastygoto:
			ch = *(fmt++);
			if (ch == '#') {
				pf = true;
				ch = *(fmt++);
			}
			if (ch >= '0' && ch <= '9') {
				w = w * 10 + (ch - '0');
				goto nastygoto;
			}
			switch (ch) {
				case 'l':
					length++;
					goto nastygoto;
				case 'u':
					i = 0;
					if (length < 2)
						uitoa(tmp, va_arg(va, unsigned int), 10, w);
					else
						u64toa(tmp, va_arg(va, uint64_t), 10, w);
					while (tmp[i])
						*buf++ = tmp[i++];
					break;
				case 'd':
				case 'i':
					i = 0;
					if (length < 2)
						itoa(tmp, va_arg(va, int), 10, w);
					else
						i64toa(tmp, va_arg(va, int64_t), 10, w);
					while (tmp[i])
						*buf++ = tmp[i++];
					break;
				case 'x':
				case 'X':
					i = 0;
					if (length < 2)
						uitoa(tmp, va_arg(va, unsigned int), 16, w);
					else
						u64toa(tmp, va_arg(va, uint64_t), 16, w);
					if (pf) {
						*buf++ = '0';
						*buf++ = (ch == 'x' ? 'x' : 'X');
					}
					while (tmp[i]) {
						*buf++ = (ch == 'x' && tmp[i] <= 'F' && tmp[i] >= 'A') ? tmp[i] + 32 : tmp[i];
						i++;
					}
					break;
				case 'p':
					i = 0;
					uitoa(tmp, va_arg(va, unsigned int), 16, w);
					*buf++ = '0';
					*buf++ = 'x';
					while (tmp[i])
						*buf++ = tmp[i++];
					break;
				case 'c':
					*buf++ = (char)(va_arg(va, int));
					break;
				case 's':
					i = 0;
					sztmp = va_arg(va, char*);
					while (sztmp[i])
						*buf++ = sztmp[i++];
					break;
				case '%':
					*buf++ = ch;
				default:
					break;
			}
		} else {
			*buf++ = 0;
		}
	}
	return buf - str;
}

int kprintf(const char *fmt, ...) {
	//char buf[2048];
	char* buf = (char*)malloc(2048);
	int ret;
	memset(buf, 0, 2048);
	va_list va;
	va_start(va, fmt);
	ret = kvsnprintf(buf, 2048, fmt, va);
	va_end(va);
	
	terminal_writestring(buf);
	free(buf);
	return ret;
}

void crash(char* file, int line, const char* fmt, ...) {
	char buf[2048], bc[12];
	memset(buf, 0, 2048);
	memset(bc, 0, 12);
	asm volatile("cli");
	va_list args;
	va_start(args, fmt);

	kvsnprintf(buf, 2048, fmt, args);

	va_end(args);

	itoa(bc, line, 10, 1);
	terminal_writestring("\nRSoD: crash() called at ");
	terminal_writestring(file);
	terminal_writestring(":");
	terminal_writestring(bc);
	terminal_writestring(": ");

	terminal_writestring(buf);

	_crash();
}
