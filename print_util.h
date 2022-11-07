#ifndef PRINT_LABEL_H
#define PRINT_LABEL_H

#include <stdio.h>
#include <stdint.h>

static inline 
void print_label(const char* label, unsigned int size) {
	unsigned int psize = printf("%s", label);

	while (psize++ < size) {
		putchar('=');
	}

	putchar('\n');
}

static inline
void print_bin(uint32_t data, uint32_t size) {
	while (size--) {
		putchar(((data >> size) & 1) + '0');
	}
	putchar('\n');
}

#endif /* PRINT_LABEL_H */