#include "TM1640.h"
#include "stm32f0xx_hal.h"
#include <stdio.h>
#include <string.h>
#include "main.h"

// 输入一个或两个
// A.
// A
// 2
// 2.
// 这样的字符串，转换成可以显示在数码管上编码
unsigned char GetSmgOneData(unsigned char ucsmg)
{
    unsigned char i;
    SmgType smg;

    switch (ucsmg)
    {
    case ' ':
        smg.bits.a = 0;  //    ----a----
        smg.bits.b = 0;  //   |         |
        smg.bits.c = 0;  //   f         b
        smg.bits.d = 0;  //   |         |
        smg.bits.e = 0;  //    ----g----
        smg.bits.f = 0;  //   |         |
        smg.bits.g = 0;  //   e         c
        smg.bits.dp = 0; //   |         |
        break;           //----d----  dp
    case '0':
        smg.bits.a = 1;  //    ----a----
        smg.bits.b = 1;  //   |         |
        smg.bits.c = 1;  //   f         b
        smg.bits.d = 1;  //   |         |
        smg.bits.e = 1;  //    ----g----
        smg.bits.f = 1;  //   |         |
        smg.bits.g = 0;  //   e         c
        smg.bits.dp = 0; //   |         |
        break;           //----d----  dp

    case '1':
        smg.bits.a = 0;
        smg.bits.b = 1;
        smg.bits.c = 1;
        smg.bits.d = 0;
        smg.bits.e = 0;
        smg.bits.f = 0;
        smg.bits.g = 0;
        smg.bits.dp = 0;
        break;
    case '2':
        smg.bits.a = 1;
        smg.bits.b = 1;
        smg.bits.c = 0;
        smg.bits.d = 1;
        smg.bits.e = 1;
        smg.bits.f = 0;
        smg.bits.g = 1;
        smg.bits.dp = 0;
        break;
    case '3':
        smg.bits.a = 1;
        smg.bits.b = 1;
        smg.bits.c = 1;
        smg.bits.d = 1;
        smg.bits.e = 0;
        smg.bits.f = 0;
        smg.bits.g = 1;
        smg.bits.dp = 0;
        break;
    case '4':
        smg.bits.a = 0;
        smg.bits.b = 1;
        smg.bits.c = 1;
        smg.bits.d = 0;
        smg.bits.e = 0;
        smg.bits.f = 1;
        smg.bits.g = 1;
        smg.bits.dp = 0;
        break;
    case '5':
        smg.bits.a = 1;
        smg.bits.b = 0;
        smg.bits.c = 1;
        smg.bits.d = 1;
        smg.bits.e = 0;
        smg.bits.f = 1;
        smg.bits.g = 1;
        smg.bits.dp = 0;
        break;
    case '6':
        smg.bits.a = 1;
        smg.bits.b = 0;
        smg.bits.c = 1;
        smg.bits.d = 1;
        smg.bits.e = 1;
        smg.bits.f = 1;
        smg.bits.g = 1;
        smg.bits.dp = 0;
        break;
    case '7':
        smg.bits.a = 1;
        smg.bits.b = 1;
        smg.bits.c = 1;
        smg.bits.d = 0;
        smg.bits.e = 0;
        smg.bits.f = 0;
        smg.bits.g = 0;
        smg.bits.dp = 0;
        break;
    case '8':
        smg.bits.a = 1;
        smg.bits.b = 1;
        smg.bits.c = 1;
        smg.bits.d = 1;
        smg.bits.e = 1;
        smg.bits.f = 1;
        smg.bits.g = 1;
        smg.bits.dp = 0;
        break;
    case '9':
        smg.bits.a = 1;
        smg.bits.b = 1;
        smg.bits.c = 1;
        smg.bits.d = 1;
        smg.bits.e = 0;
        smg.bits.f = 1;
        smg.bits.g = 1;
        smg.bits.dp = 0;
        break; // g=Q=9
    case 'A':
        smg.bits.a = 1;
        smg.bits.b = 1;
        smg.bits.c = 1;
        smg.bits.d = 0;
        smg.bits.e = 1;
        smg.bits.f = 1;
        smg.bits.g = 1;
        smg.bits.dp = 0;
        break;
    case 'B':
        smg.bits.a = 0;
        smg.bits.b = 0;
        smg.bits.c = 1;
        smg.bits.d = 1;
        smg.bits.e = 1;
        smg.bits.f = 1;
        smg.bits.g = 1;
        smg.bits.dp = 0;
        break;
    case 'C':
        smg.bits.a = 1;
        smg.bits.b = 0;
        smg.bits.c = 0;
        smg.bits.d = 1;
        smg.bits.e = 1;
        smg.bits.f = 1;
        smg.bits.g = 0;
        smg.bits.dp = 0;
        break;
    case 'D':
        smg.bits.a = 0;
        smg.bits.b = 1;
        smg.bits.c = 1;
        smg.bits.d = 1;
        smg.bits.e = 1;
        smg.bits.f = 0;
        smg.bits.g = 1;
        smg.bits.dp = 0;
        break;
    case 'E':
        smg.bits.a = 1;
        smg.bits.b = 0;
        smg.bits.c = 0;
        smg.bits.d = 1;
        smg.bits.e = 1;
        smg.bits.f = 1;
        smg.bits.g = 1;
        smg.bits.dp = 0;
        break;
    case 'F':
        smg.bits.a = 1;
        smg.bits.b = 0;
        smg.bits.c = 0;
        smg.bits.d = 0;
        smg.bits.e = 1;
        smg.bits.f = 1;
        smg.bits.g = 1;
        smg.bits.dp = 0;
        break;
    case 'G':
        smg.bits.a = 1;
        smg.bits.b = 0;
        smg.bits.c = 0;
        smg.bits.d = 1;
        smg.bits.e = 1;
        smg.bits.f = 1;
        smg.bits.g = 1;
        smg.bits.dp = 0;
        break;
    case 'H':
        smg.bits.a = 0;  //     ---a---
        smg.bits.b = 1;  //    |       |
        smg.bits.c = 1;  //    f       b
        smg.bits.d = 0;  //    |       |
        smg.bits.e = 1;  //     ---g---
        smg.bits.f = 1;  //    |       |
        smg.bits.g = 1;  //    e       c
        smg.bits.dp = 0; //    |       |
        break;           //         ---d---  dp
    case 'I':
        smg.bits.a = 0;
        smg.bits.b = 1;
        smg.bits.c = 1;
        smg.bits.d = 0;
        smg.bits.e = 0;
        smg.bits.f = 0;
        smg.bits.g = 0;
        smg.bits.dp = 0;
        break;
    case 'J':
        smg.bits.a = 0;
        smg.bits.b = 1;
        smg.bits.c = 1;
        smg.bits.d = 1;
        smg.bits.e = 1;
        smg.bits.f = 0;
        smg.bits.g = 0;
        smg.bits.dp = 0;
        break;
    case 'K':
        smg.bits.a = 0;
        smg.bits.b = 1;
        smg.bits.c = 1;
        smg.bits.d = 0;
        smg.bits.e = 1;
        smg.bits.f = 1;
        smg.bits.g = 1;
        smg.bits.dp = 0;
        break;
    case 'L':
        smg.bits.a = 0;
        smg.bits.b = 0;
        smg.bits.c = 0;
        smg.bits.d = 1;
        smg.bits.e = 1;
        smg.bits.f = 1;
        smg.bits.g = 0;
        smg.bits.dp = 0;
        break;
    case 'M':
        smg.bits.a = 0;
        smg.bits.b = 1;
        smg.bits.c = 1;
        smg.bits.d = 0;
        smg.bits.e = 1;
        smg.bits.f = 1;
        smg.bits.g = 1;
        smg.bits.dp = 0;
        break;
    case 'N':
        smg.bits.a = 0;
        smg.bits.b = 1;
        smg.bits.c = 1;
        smg.bits.d = 0;
        smg.bits.e = 1;
        smg.bits.f = 1;
        smg.bits.g = 1;
        smg.bits.dp = 0;
        break;
    case 'O':
        smg.bits.a = 1;
        smg.bits.b = 1;
        smg.bits.c = 1;
        smg.bits.d = 1;
        smg.bits.e = 1;
        smg.bits.f = 1;
        smg.bits.g = 0;
        smg.bits.dp = 0;
        break;
    case 'P':
        smg.bits.a = 1;
        smg.bits.b = 1;
        smg.bits.c = 0;
        smg.bits.d = 0;
        smg.bits.e = 1;
        smg.bits.f = 1;
        smg.bits.g = 1;
        smg.bits.dp = 0;
        break;
    case 'Q':
        smg.bits.a = 1;
        smg.bits.b = 1;
        smg.bits.c = 1;
        smg.bits.d = 1;
        smg.bits.e = 0;
        smg.bits.f = 1;
        smg.bits.g = 1;
        smg.bits.dp = 0;
        break; // Q=9
    case 'R':
        smg.bits.a = 0;
        smg.bits.b = 1;
        smg.bits.c = 1;
        smg.bits.d = 0;
        smg.bits.e = 1;
        smg.bits.f = 1;
        smg.bits.g = 1;
        smg.bits.dp = 0;
        break;
    case 'S':
        smg.bits.a = 1;
        smg.bits.b = 0;
        smg.bits.c = 0;
        smg.bits.d = 1;
        smg.bits.e = 1;
        smg.bits.f = 1;
        smg.bits.g = 1;
        smg.bits.dp = 0;
        break;
    case 'T':
        smg.bits.a = 0;
        smg.bits.b = 0;
        smg.bits.c = 0;
        smg.bits.d = 1;
        smg.bits.e = 1;
        smg.bits.f = 1;
        smg.bits.g = 1;
        smg.bits.dp = 0;
        break;
    case 'U':
        smg.bits.a = 0;
        smg.bits.b = 1;
        smg.bits.c = 1;
        smg.bits.d = 1;
        smg.bits.e = 1;
        smg.bits.f = 1;
        smg.bits.g = 0;
        smg.bits.dp = 0;
        break;
    case 'V':
        smg.bits.a = 0;
        smg.bits.b = 1;
        smg.bits.c = 1;
        smg.bits.d = 1;
        smg.bits.e = 1;
        smg.bits.f = 1;
        smg.bits.g = 0;
        smg.bits.dp = 0;
        break;
    case 'W':
        smg.bits.a = 0;
        smg.bits.b = 1;
        smg.bits.c = 1;
        smg.bits.d = 1;
        smg.bits.e = 1;
        smg.bits.f = 1;
        smg.bits.g = 0;
        smg.bits.dp = 0;
        break;
    case 'X':
        smg.bits.a = 0;
        smg.bits.b = 1;
        smg.bits.c = 1;
        smg.bits.d = 0;
        smg.bits.e = 1;
        smg.bits.f = 1;
        smg.bits.g = 1;
        smg.bits.dp = 0;
        break;
    case 'Y':
        smg.bits.a = 0;
        smg.bits.b = 1;
        smg.bits.c = 1;
        smg.bits.d = 1;
        smg.bits.e = 0;
        smg.bits.f = 1;
        smg.bits.g = 1;
        smg.bits.dp = 0;
        break;
    case 'Z':
        smg.bits.a = 1;
        smg.bits.b = 1;
        smg.bits.c = 0;
        smg.bits.d = 1;
        smg.bits.e = 1;
        smg.bits.f = 0;
        smg.bits.g = 1;
        smg.bits.dp = 0;
        break; // z=2
    case 'a':
        smg.bits.a = 1;
        smg.bits.b = 1;
        smg.bits.c = 1;
        smg.bits.d = 0;
        smg.bits.e = 1;
        smg.bits.f = 1;
        smg.bits.g = 1;
        smg.bits.dp = 0;
        break;
    case 'b':
        smg.bits.a = 0;
        smg.bits.b = 0;
        smg.bits.c = 1;
        smg.bits.d = 1;
        smg.bits.e = 1;
        smg.bits.f = 1;
        smg.bits.g = 1;
        smg.bits.dp = 0;
        break;
    case 'c':
        smg.bits.a = 0;
        smg.bits.b = 0;
        smg.bits.c = 0;
        smg.bits.d = 1;
        smg.bits.e = 1;
        smg.bits.f = 0;
        smg.bits.g = 1;
        smg.bits.dp = 0;
        break;
    case 'd':
        smg.bits.a = 0;
        smg.bits.b = 1;
        smg.bits.c = 1;
        smg.bits.d = 1;
        smg.bits.e = 1;
        smg.bits.f = 0;
        smg.bits.g = 1;
        smg.bits.dp = 0;
        break;
    case 'e':
        smg.bits.a = 1;
        smg.bits.b = 1;
        smg.bits.c = 0;
        smg.bits.d = 1;
        smg.bits.e = 1;
        smg.bits.f = 1;
        smg.bits.g = 1;
        smg.bits.dp = 0;
        break;
    case 'f':
        smg.bits.a = 1;
        smg.bits.b = 0;
        smg.bits.c = 0;
        smg.bits.d = 0;
        smg.bits.e = 1;
        smg.bits.f = 1;
        smg.bits.g = 1;
        smg.bits.dp = 0;
        break;
    case 'g':
        smg.bits.a = 1;
        smg.bits.b = 1;
        smg.bits.c = 1;
        smg.bits.d = 1;
        smg.bits.e = 0;
        smg.bits.f = 1;
        smg.bits.g = 1;
        smg.bits.dp = 0;
        break; // q=g=9
    case 'h':
        smg.bits.a = 0;
        smg.bits.b = 0;
        smg.bits.c = 1;
        smg.bits.d = 0;
        smg.bits.e = 1;
        smg.bits.f = 1;
        smg.bits.g = 1;
        smg.bits.dp = 0;
        break;
    case 'i':
        smg.bits.a = 0;
        smg.bits.b = 0;
        smg.bits.c = 0;
        smg.bits.d = 0;
        smg.bits.e = 1;
        smg.bits.f = 0;
        smg.bits.g = 0;
        smg.bits.dp = 0;
        break;
    case 'j':
        smg.bits.a = 0;
        smg.bits.b = 1;
        smg.bits.c = 1;
        smg.bits.d = 1;
        smg.bits.e = 1;
        smg.bits.f = 0;
        smg.bits.g = 0;
        smg.bits.dp = 0;
        break;
    case 'k':
        smg.bits.a = 0;
        smg.bits.b = 0;
        smg.bits.c = 1;
        smg.bits.d = 0;
        smg.bits.e = 1;
        smg.bits.f = 1;
        smg.bits.g = 1;
        smg.bits.dp = 0;
        break;
    case 'l':
        smg.bits.a = 0;
        smg.bits.b = 0;
        smg.bits.c = 0;
        smg.bits.d = 1;
        smg.bits.e = 1;
        smg.bits.f = 0;
        smg.bits.g = 0;
        smg.bits.dp = 0;
        break;
    case 'm':
        smg.bits.a = 0;
        smg.bits.b = 0;
        smg.bits.c = 1;
        smg.bits.d = 0;
        smg.bits.e = 1;
        smg.bits.f = 1;
        smg.bits.g = 1;
        smg.bits.dp = 0;
        break;
    case 'n':
        smg.bits.a = 0;
        smg.bits.b = 0;
        smg.bits.c = 1;
        smg.bits.d = 0;
        smg.bits.e = 1;
        smg.bits.f = 0;
        smg.bits.g = 1;
        smg.bits.dp = 0;
        break;
    case 'o':
        smg.bits.a = 0;
        smg.bits.b = 0;
        smg.bits.c = 1;
        smg.bits.d = 1;
        smg.bits.e = 1;
        smg.bits.f = 0;
        smg.bits.g = 1;
        smg.bits.dp = 0;
        break;
    case 'p':
        smg.bits.a = 1;
        smg.bits.b = 1;
        smg.bits.c = 0;
        smg.bits.d = 0;
        smg.bits.e = 1;
        smg.bits.f = 1;
        smg.bits.g = 1;
        smg.bits.dp = 0;
        break;
    case 'q':
        smg.bits.a = 1;
        smg.bits.b = 1;
        smg.bits.c = 1;
        smg.bits.d = 0;
        smg.bits.e = 0;
        smg.bits.f = 1;
        smg.bits.g = 1;
        smg.bits.dp = 0;
        break;
    case 'r':
        smg.bits.a = 0;
        smg.bits.b = 0;
        smg.bits.c = 0;
        smg.bits.d = 0;
        smg.bits.e = 1;
        smg.bits.f = 0;
        smg.bits.g = 1;
        smg.bits.dp = 0;
        break;
    case 's':
        smg.bits.a = 1;
        smg.bits.b = 0;
        smg.bits.c = 1;
        smg.bits.d = 1;
        smg.bits.e = 0;
        smg.bits.f = 1;
        smg.bits.g = 1;
        smg.bits.dp = 0;
        break;
    case 't':
        smg.bits.a = 0;
        smg.bits.b = 0;
        smg.bits.c = 0;
        smg.bits.d = 1;
        smg.bits.e = 1;
        smg.bits.f = 1;
        smg.bits.g = 1;
        smg.bits.dp = 0;
        break;
    case 'u':
        smg.bits.a = 0;
        smg.bits.b = 0;
        smg.bits.c = 1;
        smg.bits.d = 1;
        smg.bits.e = 1;
        smg.bits.f = 0;
        smg.bits.g = 0;
        smg.bits.dp = 0;
        break;
    case 'v':
        smg.bits.a = 0;
        smg.bits.b = 0;
        smg.bits.c = 1;
        smg.bits.d = 1;
        smg.bits.e = 1;
        smg.bits.f = 0;
        smg.bits.g = 0;
        smg.bits.dp = 0;
        break;
    case 'w':
        smg.bits.a = 0;
        smg.bits.b = 1;
        smg.bits.c = 1;
        smg.bits.d = 0;
        smg.bits.e = 0;
        smg.bits.f = 1;
        smg.bits.g = 0;
        smg.bits.dp = 0;
        break;
    case 'x':
        smg.bits.a = 0;
        smg.bits.b = 1;
        smg.bits.c = 1;
        smg.bits.d = 0;
        smg.bits.e = 1;
        smg.bits.f = 1;
        smg.bits.g = 1;
        smg.bits.dp = 0;
        break;
    case 'y':
        smg.bits.a = 0;
        smg.bits.b = 1;
        smg.bits.c = 1;
        smg.bits.d = 1;
        smg.bits.e = 0;
        smg.bits.f = 1;
        smg.bits.g = 1;
        smg.bits.dp = 0;
        break;
    case 'z':
        smg.bits.a = 1;
        smg.bits.b = 1;
        smg.bits.c = 0;
        smg.bits.d = 0;
        smg.bits.e = 1;
        smg.bits.f = 0;
        smg.bits.g = 1;
        smg.bits.dp = 0;
        break;
    case '-':
        smg.bits.a = 0;
        smg.bits.b = 0;
        smg.bits.c = 0;
        smg.bits.d = 0;
        smg.bits.e = 0;
        smg.bits.f = 0;
        smg.bits.g = 1;
        smg.bits.dp = 0;
        break;
    case '.':
        smg.bits.a = 0;
        smg.bits.b = 0;
        smg.bits.c = 0;
        smg.bits.d = 0;
        smg.bits.e = 0;
        smg.bits.f = 0;
        smg.bits.g = 0;
        smg.bits.dp = 1;
        break;
    }

    return smg.byte;
}

// 显示字符串
// smgblock:
// #define smg1 0xC0
// #define smg2 0xC4
// #define smg3 0xC8
void getdisplaycode(char smgblock, char *pstr)
{
    int i = 0;
    int j = 0;
    SmgType smgcode;
    smgcode.byte = 0;
    TM1640Start();
    TM1640WriteByte(0x44);
    TM1640Stop();
    for (i = 0; pstr[i] != '\0'; i++, j++)
    {
        if (j >= 4) // 一片4个数码管的数码管块只能显示4个字符
        {
            break;
        }
        TM1640Start();
        TM1640WriteByte(smgblock + j);
        smgcode.byte = GetSmgOneData(pstr[i]);
        if (pstr[i + 1] == '.') // 下一个是点的话
        {
            i++;                 // 跳过
            smgcode.bits.dp = 1; // 当前补上小数点
        }
        TM1640WriteByte(smgcode.byte);
        TM1640Stop();
    }

    TM1640Start();
    TM1640WriteByte(0x8b);
    TM1640Stop();
}

// 在倒数第二个字符中插入小数点
void setShowsetDot(char *pstr, signed int sT)
{

    sprintf(pstr, "%4d", sT); // 右对齐，不足4位前面补空格
    if (sT > -10 && sT < 10)  // 补上0.
    {
        pstr[4] = pstr[3];
        pstr[3] = '.';
        pstr[1] = pstr[2];
        pstr[2] = '0';
        pstr[5] = 0;
    }
    else if (sT > -999 && sT < 9999) // 插入小数点
    {
        pstr[4] = pstr[3];
        pstr[3] = '.';
        pstr[5] = 0;
    }
    else //
    {
        pstr[0] = '9';
        pstr[1] = '9';
        pstr[2] = '9';
        pstr[3] = '9';
        pstr[4] = '.';
        pstr[5] = 0;
    }
}