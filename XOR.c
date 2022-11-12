/*
	
	exclusive or

	| A | B | A ^ B |
	|---|---|-------|
	| 0 | 0 |   0   |
	| 0 | 1 |   1   |
	| 1 | 0 |   1   |
	| 1 | 1 |   0   |

	�ٸ��� 1, ������ 0
*/

#include "print_util.h"

typedef uint32_t u32;



/* ������ NOT���� ==================================================

xor�� 1�� �κи� ������Ű�� ���̶�� ������ �� �ִ�.
������� c = a ^ 0b00001100 ����, c�� a�� 2��° ��Ʈ�� 3��° ��Ʈ�� ������ ���� �ȴ�.

	+ ���: ���� ���ϴ� �� 2���� ����� �� ����� �� �ִ�.
	+ swap: xor�� ���� swap�� �� �� �ִ�.
*/

#if 0

void swap(u32* restrict a, u32* restrict b) {
	*a ^= *b;
	*b ^= *a;
	*a ^= *b;

	// a = b ^ a ^ a
	// b = a ^ b ^ b
}

/* ���� swap �Լ��� ���� */
void swap2(u32* restrict a, u32* restrict b) {
	*a ^= *b ^= *a ^= *b;
}

int main() {
	u32 a = 0b11111101, b = 0b00000011;
	print_bin(a ^ b, 8);

	/* toggle */
	// 46464
	u32 t = 4;
	for (u32 i = 0; i < 5; i++) {
		putchar(t + '0');
		t ^= 2;
	}
	putchar('\n');

	/* swap */
	u32 c = 3, d = 4;
	swap(&c, &d);
	printf("%lu %lu\n", c, d);
}

#endif

/* carry���� ADD ===================================================

	+ reference: https://en.wikipedia.org/wiki/Binary_number#Addition
xor�� carry ���� ADD��� ������ �� �ִ�.

*/

#if 0

int main() {
	u32 a = 0b01010101;
	u32 b = 0b01010100;

	u32 _add = a + b;
	u32 _xor = a ^ b;

	print_bin(_add, 8);
	print_bin(_xor, 8);
	print_bin(_add ^ _xor, 8); // carry�� ���� ��Ʈ�� 1�� �ȴ�.
}

#endif