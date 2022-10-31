/*
 * reference: https://homepage.divms.uiowa.edu/~jones/bcd/bcd.html
 */

#include <stdio.h>
#include <stdint.h>

#include "print_label.h"

//#ifndef ENALBE_BCD4
//    #define ENALBE_BCD4
//#endif
//#ifndef ENALBE_BCD6
//    #define ENALBE_BCD6
//#endif
//#ifndef ENALBE_BCD_ASCII
//    #define ENALBE_BCD_ASCII
//#endif

typedef uint32_t u32;

/* Packed BCD =============================================================== */

/*
    B31                            B15                             B0
    |_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _|_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _|
    |_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|
    |   D7  |   D6  |   D5  |   D4  |   D3  |   D2  |   D1  |   D0  |

    + ���:
        + (BCD) digit:
          BCD���� �ϳ��� decimal digit�� ��Ÿ���� bit field(ex: D0 ~ D7)
          �Ʒ� comment�鿡���� BCD digit���� BCD�� �����Ͽ� ����ϴ� ��찡 ��ټ��̴�.
         

    + NOTE:
        + �Ϲ������� ���� BCD digit(D7)�� 7�ڸ�(D0 ~ D6) BCD �ǿ������� carry�� �����ϴ� ���� ����ؾ� �Ѵ�.
          �̷��� ������� ����� ���ѵǸ� ���ڴ� �׻� ����̸�, 2�� ���� overflow�� �������� �ʴ´�.
*/

#ifdef ENALBE_BCD4

u32 valid(u32 a) {
	u32 t1 = a + 0x06666666;    // 9���� ū ���� ���� ��� (��ȿ���� ����) digit�� ���� higher digit���� carry�� �����Ѵ�.
    u32 t2 = a ^ 0x06666666;    // xor�� carry�� ���ٴ� ���� ���� add�� �����ϴ�.
    u32 t3 = t1 ^ t2;           // ���� t3���� carry in�� �ִ� ��Ʈ�� 1�� �ȴ�.
    u32 t4 = t3 & 0x11111110;    // t4�� digit�� carry�� �ִ� ������ 1�� �����Ѵ�.

    return t4 == 0;             // t4�� 0�� �ƴ϶�� a�� BCD�ν� ��ȿ���� �ʴ�.   
}

u32 valid2(u32 a) {
    u32 t1 = a + 0x06666666;
    u32 t2 = t1 ^ a;            // valid1������ t2 ������ digit�� LSB�� �ٲ��� �����Ƿ� ���� �����ϴ�.
    u32 t3 = t2 & 0x11111110;

    return t3 == 0;
}

u32 valid3(u32 a) {
    u32 t1 = a & 0x08888888;    // 8���� ū digit�� 1000�� �ȴ�.
    u32 t2 = t1 >> 3;            // digit�� 0001�� �ȴ�.
    u32 t3 = t1 - t2;           // a���� 8���� ū digit�� 0111�� �ȴ�.
    u32 t4 = t1 | t3;           // 0111 digit�� 1111�� �����
    u32 t5 = a & t4;            // 8���� ���� digit�� masking�Ѵ�.
    u32 t6 = t5 & 0x06666666;   // ��ȿ�� digit�̶�� 0000, 1000 �Ǵ� 1001�̹Ƿ� 0110(0x6)�� �ɸ��� �ʴ´�.

    return t6 == 0;
}

u32 add(u32 a, u32 b) {
    u32 t1 = a + 0x06666666;        // carry ������ ���� digit�� 6�� �߰��Ѵ�.
    u32 t2 = t1 + b;                // carry ����.
    u32 t3 = t1 ^ b;
    u32 t4 = t2 ^ t3;               // t2�� t3�� carry�� �ִ� ������ �ٸ���.
    u32 t5 = ~t4 & 0x11111110;      // digit ĳ���� ���� ������ 1
    u32 t6 = (t5 >> 2) | (t5 >> 3); // carry�� ������ digit�� 0, carry�� �������� ���� digit�� 6

    return t2 - t6;                 // carry�� ���� t1������ 6 digit ���ϱ⸦ ����
}

u32 tencomp(u32 a) { // 10's complement
    u32 t1 = 0xF9999999 - a;        // 9's complement
    return add(t1, 0x00000001);
}

u32 tencomp2(u32 a) {
    a = 0xF9999999 - a;

    /* add(a, 0x00000001) */
    u32 t1 = a + 0x06666666;
    u32 t2 = t1 + 0x00000001;
    u32 t3 = t1 ^ 0x00000001;
    u32 t4 = t2 ^ t3;
    u32 t5 = ~t4 & 0x11111110;
    u32 t6 = (t5 >> 2) | (t5 >> 3); 

    return t2 - t6;
}

u32 tencomp3(u32 a) { // ����ȭ
    u32 t1 = 0xFFFFFFFF - a;        // 0xFFFFFFFF = 0xF9999999 + 0x06666666
    u32 t2 = 0 - a;                 // -a = t1 + 0x00000001 (MSVC���� "-a"�� error�� �����ϱ� ������ "0 - a"�� ���)
    u32 t3 = t1 ^ 0x00000001;
    u32 t4 = t2 ^ t3;
    u32 t5 = ~t4 & 0x11111110;
    u32 t6 = (t5 >> 2) | (t5 >> 3);

    return t2 - t6;
}

u32 sub(u32 a, u32 b) {
    return add(a, tencomp(b));
}

int main() {
    print_label("valid", 50);
    printf("%lu\n", valid(0x12345678));

    print_label("add", 50);
    printf("%08lx\n", add(0x01234567, 0x07654321));
    printf("%08lx\n", add(0x00000015, 0x00000017));
    printf("%08lx\n", add(0x09999999, 0x00000001));

    print_label("10's complement", 50);
    printf("%08lx\n", tencomp(0x01234567));

    print_label("sub", 50);
    printf("%08lx\n", sub(0x00000017, 0x00000015));
    printf("%08lx\n", tencomp(sub(0x00000015, 0x00000017)));
}

#endif /* ENALBE_BCD4 */

/* 6-bit BCD ================================================================ */

/*
    B31                            B15                             B0
    |_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _|_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _|
    |_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|
    | D5|0 0|   D4  |0 0|   D3  |0 0|   D2  |0 0|   D1  |0 0|   D0  |
*/

#ifdef ENALBE_BCD6

//u32 valid(u32 a) { //?
//    u32 t1 = a +  006666666666; // �� digit�� 066�� ���ϸ� 9���� ū ���� ���� digit�� carry�� �����Ѵ�.
//    u32 t2 = t1 & 006060606060;
//
//    return t2 ==  006060606060;
//}

u32 valid2(u32 a) {
    u32 t1 = a +  060606060606; // �� digit�� 066�� ���ϸ� 9���� ū ���� ���� digit�� carry�� �����Ѵ�.
    return (006060606060 & (a | t1)) == 0;
}

u32 add(u32 a, u32 b) {
    u32 t1 = a + b + 006666666666;
    u32 t2 = t1 &    006060606060;

    return (t1 - (t2 >> 3)) & ~006060606060;
}

u32 tencomp(u32 a) {
    u32 t1 = 001111111111 - a;
    return add(t1, 000000000001);
}

u32 tencomp2(u32 a) {
    u32 t1 = 010000000000 - a;
    u32 t2 = t1 & 006060606060;
    return (t1 - (t2 >> 3)) & ~006060606060;
}

u32 sub(u32 a, u32 b) {
    return add(a, tencomp(b));
}

int main() {

    print_label("valid", 50);
    //printf("%lu\n", valid(000203040506));
    //printf("%lu\n", valid(000203040576));
    //printf("%lu\n", valid(000203041112));
    //printf("%lu\n", valid(000203041171));

    print_label("valid2", 50);
    printf("%lu\n", valid2(010203040506));
    printf("%lu\n", valid2(010203040576));
    printf("%lu\n", valid2(010203041112));

    print_label("add", 50);
    printf("%012lo\n", add(000102030405, 000504030201));
    printf("%012lo\n", add(000102030411, 000000000001));

    print_label("10's complement", 50);
    printf("%012lo\n", tencomp(000102030405));
    printf("%012lo\n", tencomp2(000102030405));

    print_label("sub", 50);
    printf("%012lo\n", sub(000102030411, 000000000001));
    printf("%012lo\n", sub(000102030400, 000000000011));
}

#endif /* ENALBE_BCD6 */

/* ASCII ==================================================================== */

/*
    B31                            B15                             B0
    |_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _|_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _|
    |_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|
    |0 0 1 1|   D3  |0 0 1 1|   D2  |0 0 1 1|   D1  |0 0 1 1|   D0  |
*/

#ifdef ENALBE_BCD_ASCII

u32 valid(u32 a) {
    u32 t1 = a + 0xC6C6C6C6; // 0xC6C6C6C6 = ~'9999'
    u32 t2 = t1 & 0xF0F0F0F0;

    return t2 == 0xF0F0F0F0;
}

u32 add(u32 a, u32 b) {
    u32 t1 = a + b + 0x96969696;            // ���ϱ�� carry.
    u32 t2 = t1 & 0x30303030;               // carry ���� �� �̾Ƴ�.
    u32 t3 = t1 - (t2 >> 3);                // carry�� ����Ű�� ���� digit�� 6�� ����.

    return (t3 & 0x0F0F0F0F) | 0x30303030;  // 0011xxxx ������ �����.
}

u32 tencomp(u32 a) {
    u32 t1 = 0x69696969 - a;
    return add(t1, '0001');
}

u32 tencomp2(u32 a) {
    u32 t1 = 0x30303030 - a;
    u32 t2 = t1 & 0x30303030;
    u32 t3 = t1 - (t2 >> 3);
    return (t3 & 0x0F0F0F0F) | 0x30303030;
}

u32 sub(u32 a, u32 b) {
    return add(a, tencomp(b));
}

int main() {
    print_label("valid", 50);
    printf("%lu\n", valid('1234'));
    printf("%lu\n", valid('1234' | 0xFF));

    print_label("add", 50);
    printf("%08lx\n", add('1234', '4321'));
    printf("%08lx\n", add('1234', '1234'));

    print_label("10's complement", 50);
    printf("%08lx\n", tencomp('1234'));
    printf("%08lx\n", tencomp2('1234'));

    print_label("sub", 50);
    printf("%08lx\n", sub('4321', '1234'));
}

#endif /* ENALBE_BCD_ASCII */
