/*
 * 값이 1인 비트의 개수 세기(population count).
 * 
 * reference: 
 *     - https://namocom.tistory.com/377
 *     - https://en.wikipedia.org/wiki/Hamming_weight
 *	   - http://codeseekah.com/2012/02/27/bitfun-popcount-sideways-sum-hamming-weight/
 * 
 */

#include <stdint.h>
#include <stdio.h>
#include "print_util.h"

typedef uint32_t u32;

int count_bit0(u32 a) {
	int cnt = 0;
	while (a) {
		cnt += a & 1;
		a >>= 1;
	}
	return cnt;
}

/*
1을 빼면 a에서 값이 1인 비트중에 LSB에 가장 가까운 비트는 0이 된다.
따라서 1을 빼고 and를 하면 최하위 비트에 가장 가까운 비트 하나가 clear된다.
*/
int count_bit1(u32 a) {
	int cnt = 0;
	while (a) {
		a &= a - 1;
		cnt++;
	}
	return cnt;
}

/* hamming weight 
reference: https://en.wikipedia.org/wiki/Hamming_weight#Efficient_implementation
비트 하나하나를 칸으로 보자. 그럼 32bit니까 칸은 32개 있을 것이다.
각 칸은 각 칸의 영역에서 1의 개수를 담고 있다고 생각할 수 있다.
이 알고리즘은 32개의 칸을 각각 옆에 있는 것과 합쳐 16개로 만들고,
이 16개의 칸을 다시 8개로 만든다. 이것은 칸이 1개가 남을 때까지
계속된다. 일종의 분할 정복 알고리즘이라 볼 수 있다.
다른 형태의 구현들도 있으니 위의 reference를 살펴보기 바란다.
*/
int count_bit2(u32 x) {
	static const u32 mask[] = {
	  0x55555555,
	  0x33333333,
	  0x0F0F0F0F,
	  0x00FF00FF,
	  0x0000FFFF,
	};

	for (size_t i = 0; i < 5; i++)
		x = ((x >> (1 << i)) & mask[i]) + (x & mask[i]);

	return x;
}


int main() {
	printf("%d\n", count_bit0(123456789));
	printf("%d\n", count_bit1(123456789));
	printf("%d\n", count_bit2(123456789));
}