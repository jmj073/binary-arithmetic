/*
	
	exclusive or

	| A | B | A ^ B |
	|---|---|-------|
	| 0 | 0 |   0   |
	| 0 | 1 |   1   |
	| 1 | 0 |   1   |
	| 1 | 1 |   0   |

	다르면 1, 같으면 0
*/

#include "print_util.h"

typedef uint32_t u32;



/* 선택적 NOT연산 ==================================================

xor은 1인 부분만 반전시키는 것이라고 생각할 수 있다.
예를들어 c = a ^ 0b00001100 에서, c는 a의 2번째 비트와 3번째 비트를 반전한 값이 된다.

	+ 토글: 따라서 원하는 값 2개를 토글할 때 사용할 수 있다.
	+ swap: xor을 통해 swap을 할 수 있다.
*/

#if 0

void swap(u32* restrict a, u32* restrict b) {
	*a ^= *b;
	*b ^= *a;
	*a ^= *b;

	// a = b ^ a ^ a
	// b = a ^ b ^ b
}

/* 위의 swap 함수와 같다 */
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

/* carry없는 ADD ===================================================

	+ reference: https://en.wikipedia.org/wiki/Binary_number#Addition
xor은 carry 없는 ADD라고 생각할 수 있다.

*/

#if 0

int main() {
	u32 a = 0b01010101;
	u32 b = 0b01010100;

	u32 _add = a + b;
	u32 _xor = a ^ b;

	print_bin(_add, 8);
	print_bin(_xor, 8);
	print_bin(_add ^ _xor, 8); // carry를 받은 비트는 1이 된다.
}

#endif