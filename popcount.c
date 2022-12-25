/*
 * ���� 1�� ��Ʈ�� ���� ����(population count).
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
1�� ���� a���� ���� 1�� ��Ʈ�߿� LSB�� ���� ����� ��Ʈ�� 0�� �ȴ�.
���� 1�� ���� and�� �ϸ� ������ ��Ʈ�� ���� ����� ��Ʈ �ϳ��� clear�ȴ�.
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
��Ʈ �ϳ��ϳ��� ĭ���� ����. �׷� 32bit�ϱ� ĭ�� 32�� ���� ���̴�.
�� ĭ�� �� ĭ�� �������� 1�� ������ ��� �ִٰ� ������ �� �ִ�.
�� �˰����� 32���� ĭ�� ���� ���� �ִ� �Ͱ� ���� 16���� �����,
�� 16���� ĭ�� �ٽ� 8���� �����. �̰��� ĭ�� 1���� ���� ������
��ӵȴ�. ������ ���� ���� �˰����̶� �� �� �ִ�.
�ٸ� ������ �����鵵 ������ ���� reference�� ���캸�� �ٶ���.
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