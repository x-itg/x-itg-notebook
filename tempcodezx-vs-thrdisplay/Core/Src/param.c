#include "24C02.h"
#include "string.h"
#include "stdio.h"
#include "param.h"

// 读取参数
void LoadPara(void)
{
    readAT24C16(addrFenH, (unsigned char *)&FenH, 4);
    if (FenH > 24)
        FenH = 0;
    readAT24C16(addrFenM, (unsigned char *)&FenM, 4);
    if (FenM > 60)
        FenM = 0;
    readAT24C16(addrTcala1, (unsigned char *)&Tcala1, 4);
    if (Tcala1 < -1000 || Tcala1 > 1000)
        Tcala1 = 0;
    readAT24C16(addrTcala2, (unsigned char *)&Tcala2, 4);
    if (Tcala2 < -1000 || Tcala2 > 1000)
        Tcala2 = 0;
    readAT24C16(addrTcala3, (unsigned char *)&Tcala3, 4);
    if (Tcala3 < -1000 || Tcala3 > 1000)
        Tcala3 = 0;
    readAT24C16(addrTcala4, (unsigned char *)&Tcala4, 4);
    if (Tcala4 < -1000 || Tcala4 > 1000)
        Tcala4 = 0;
    readAT24C16(addrTcala5, (unsigned char *)&Tcala5, 4);
    if (Tcala5 < -1000 || Tcala5 > 1000)
        Tcala5 = 0;

    readAT24C16(addrk1, (unsigned char *)&k1, 4);
    readAT24C16(addrb1, (unsigned char *)&b1, 4);
    readAT24C16(addrk2, (unsigned char *)&k2, 4);
    readAT24C16(addrb2, (unsigned char *)&b2, 4);
    readAT24C16(addrk3, (unsigned char *)&k3, 4);
    readAT24C16(addrb3, (unsigned char *)&b3, 4);

    readAT24C16(addrTset1, (unsigned char *)&Tset1, 4);
    if (Tset1 < TsetValLow || Tset1 > TsetValHig)
        Tset1 = 1000;
    readAT24C16(addrTset2, (unsigned char *)&Tset2, 4);
    if (Tset2 < TsetValLow || Tset2 > TsetValHig)
        Tset2 = 800;
    readAT24C16(addrTset3, (unsigned char *)&Tset3, 4);
    if (Tset3 < TsetValLow || Tset3 > TsetValHig)
        Tset3 = 1300;
    readAT24C16(addrTset4, (unsigned char *)&Tset4, 4);
    if (Tset4 < TsetValLow || Tset4 > TsetValHig)
        Tset4 = 1500;
    readAT24C16(addrTset5, (unsigned char *)&Tset5, 4);
    if (Tset5 < TsetValLow || Tset5 > TsetValHig)
        Tset5 = 600;
    readAT24C16(addrTset6, (unsigned char *)&Tset6, 4);
    if (Tset6 < TsetValLow || Tset6 > TsetValHig)
        Tset6 = 600;
    readAT24C16(addrTset7, (unsigned char *)&Tset7, 4);
    if (Tset7 < TsetValLow || Tset7 > TsetValHig)
        Tset7 = 600;

    readAT24C16(addrcshuiCha, (unsigned char *)&cshuiCha, 4);
    if (cshuiCha < HCLow || cshuiCha > HCHig)
        cshuiCha = 4;
    readAT24C16(addrcwhuiCha, (unsigned char *)&cwhuiCha, 4);
    if (cwhuiCha < HCLow || cwhuiCha > HCHig)
        cwhuiCha = 4;
    readAT24C16(addrtzhuiCha, (unsigned char *)&tzhuiCha, 4);
    if (tzhuiCha < HCLow || tzhuiCha > HCHig)
        tzhuiCha = 4;

    readAT24C16(addraddr, (unsigned char *)&addr, 4);
    if (addr > 254)
        addr = 1;
    readAT24C16(addrjojy, (unsigned char *)&jojy, 4);
    if (jojy > 2)
        jojy = 0;
    readAT24C16(addrbtlv, (unsigned char *)&btlv, 4);
    if (btlv > 4)
        btlv = 3;
}
