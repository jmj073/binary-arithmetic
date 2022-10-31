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

    + 용어:
        + (BCD) digit:
          BCD에서 하나의 decimal digit을 나타내는 bit field(ex: D0 ~ D7)
          아래 comment들에서는 BCD digit에서 BCD를 생략하여 언급하는 경우가 대다수이다.
         

    + NOTE:
        + 일반적으로 상위 BCD digit(D7)은 7자리(D0 ~ D6) BCD 피연산자의 carry를 누적하는 데만 사용해야 한다.
          이러한 방식으로 사용이 제한되면 숫자는 항상 양수이며, 2의 보수 overflow를 생성하지 않는다.
*/

#ifdef ENALBE_BCD4

u32 valid(u32 a) {
	u32 t1 = a + 0x06666666;    // 9보다 큰 값을 갖는 모든 (유효하지 않은) digit은 다음 higher digit으로 carry를 생성한다.
    u32 t2 = a ^ 0x06666666;    // xor은 carry가 없다는 점만 빼면 add와 유사하다.
    u32 t3 = t1 ^ t2;           // 따라서 t3에서 carry in이 있는 비트는 1이 된다.
    u32 t4 = t3 & 0x11111110;    // t4는 digit에 carry가 있는 곳마다 1을 보유한다.

    return t4 == 0;             // t4가 0이 아니라면 a는 BCD로써 유효하지 않다.   
}

u32 valid2(u32 a) {
    u32 t1 = a + 0x06666666;
    u32 t2 = t1 ^ a;            // valid1에서의 t2 연산은 digit의 LSB를 바꾸지 않으므로 생략 가능하다.
    u32 t3 = t2 & 0x11111110;

    return t3 == 0;
}

u32 valid3(u32 a) {
    u32 t1 = a & 0x08888888;    // 8보다 큰 digit은 1000이 된다.
    u32 t2 = t1 >> 3;            // digit이 0001이 된다.
    u32 t3 = t1 - t2;           // a에서 8보다 큰 digit은 0111이 된다.
    u32 t4 = t1 | t3;           // 0111 digit을 1111로 만든다
    u32 t5 = a & t4;            // 8보다 작은 digit을 masking한다.
    u32 t6 = t5 & 0x06666666;   // 유효한 digit이라면 0000, 1000 또는 1001이므로 0110(0x6)에 걸리지 않는다.

    return t6 == 0;
}

u32 add(u32 a, u32 b) {
    u32 t1 = a + 0x06666666;        // carry 생성을 위해 digit에 6을 추가한다.
    u32 t2 = t1 + b;                // carry 생성.
    u32 t3 = t1 ^ b;
    u32 t4 = t2 ^ t3;               // t2와 t3는 carry가 있는 곳마다 다르다.
    u32 t5 = ~t4 & 0x11111110;      // digit 캐리가 없는 곳마다 1
    u32 t6 = (t5 >> 2) | (t5 >> 3); // carry를 생성한 digit은 0, carry를 생성하지 않은 digit은 6

    return t2 - t6;                 // carry를 위한 t1에서의 6 digit 더하기를 제거
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

u32 tencomp3(u32 a) { // 간소화
    u32 t1 = 0xFFFFFFFF - a;        // 0xFFFFFFFF = 0xF9999999 + 0x06666666
    u32 t2 = 0 - a;                 // -a = t1 + 0x00000001 (MSVC에서 "-a"가 error를 생성하기 때문에 "0 - a"를 사용)
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
//    u32 t1 = a +  006666666666; // 각 digit에 066을 더하면 9보다 큰 값을 가진 digit은 carry를 생성한다.
//    u32 t2 = t1 & 006060606060;
//
//    return t2 ==  006060606060;
//}

u32 valid2(u32 a) {
    u32 t1 = a +  060606060606; // 각 digit에 066을 더하면 9보다 큰 값을 가진 digit은 carry를 생성한다.
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
    u32 t1 = a + b + 0x96969696;            // 더하기와 carry.
    u32 t2 = t1 & 0x30303030;               // carry 없는 놈만 뽑아냄.
    u32 t3 = t1 - (t2 >> 3);                // carry를 일으키지 않은 digit은 6을 뺀다.

    return (t3 & 0x0F0F0F0F) | 0x30303030;  // 0011xxxx 형식을 맞춘다.
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
