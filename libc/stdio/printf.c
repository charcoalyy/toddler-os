#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

// accessible only in current file (static)
static bool print(const char* data, size_t length) {
	const unsigned char* bytes = (const unsigned char*) data;
	for (size_t i = 0; i < length; i++)
		// this implementation writes to terminal
		if (putchar(bytes[i]) == EOF)
			return false;
	return true;
}

// "format" = pointer to start of string, ... = variadic arguments
int printf(const char* restrict format, ...) {
	va_list parameters; // list of extra variadic arguments
	va_start(parameters, format);

	int written = 0;

	// loop through chars of string until pointing to null
	while (*format != '\0') {
		// check how many chars can still be written
		size_t maxrem = INT_MAX - written;

		// if current char is literal
		if (format[0] != '%' || format[1] == '%') {
			if (format[0] == '%')
				format++; // handles the case of '%%'
			size_t amount = 1;
			while (format[amount] && format[amount] != '%')
				amount++; // calculates num of consecutive literal chars
			if (maxrem < amount) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, amount))
				return -1;
			// continue with next unwritten chars
			format += amount;
			written += amount;
			continue;
		}

		// saves position of placeholder start %, then moves forward to next char
		const char* format_begun_at = format++;

		if (*format == 'c') {
			format++;
			// fetches next char of the variadic arguments
			char c = (char) va_arg(parameters, int /* char promotes to int */);
			if (!maxrem) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(&c, sizeof(c)))
				return -1;
			written++;
		} else if (*format == 's') {
			format++;
			// fetches next string of the variadic arguments
			const char* str = va_arg(parameters, const char*);
			size_t len = strlen(str);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(str, len))
				return -1;
			written += len;
		} else if (*format == 'd') {
			format++;

			int num = va_arg(parameters, int);

			char str[12];
			inttostr(num, str);

			size_t len = strlen(str);
			if (maxrem < len) {
				return -1;
			}
			if (!print(str, len)) {
				return -1;
			}

			written += len;
		} else {
			// return to % and treat as plain text if unsupported
			format = format_begun_at;
			size_t len = strlen(format);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, len))
				return -1;
			written += len;
			format += len;
		}
	}

	va_end(parameters);
	return written;
}
