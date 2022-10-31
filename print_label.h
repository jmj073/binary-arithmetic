#ifndef PRINT_LABEL_H
#define PRINT_LABEL_H

#include <stdio.h>

static inline void print_label(const char* label, unsigned int size) {
	unsigned int psize = printf("%s", label);

	while (psize++ < size) {
		putchar('=');
	}

	putchar('\n');
}

#endif /* PRINT_LABEL_H */