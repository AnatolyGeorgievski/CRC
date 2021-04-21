#include "crc.h"
/*! \defgroup _crc_16 Алгоритмы расчета контрольных сумм FCS и CRC

    \see IUT-T V.42 : Error-correcting procedures for DCEs using asynchronous-to-synchronous conversion <http://www.itu.int/rec/T-REC-V.42-198811-S>

    В данном разделе собраны функции расчета контрольных сумм и контроля целостности.
    Персональная коллекция включает алгоритмы CRC16, CRC16B, CRC32, CRC32B, ADLER32, и пр.
    Все алгоритмы ориентированы на минимальный размер памяти занимаемый таблицами.


    \see <http://www.ross.net/cRC/download/crc_v3.txt>
    \see Каталог контрольных сумм СRC    <http://reveng.sourceforge.net/crc-catalogue/>

    Все алгоритмы могуть быть получены путем отражения бит входных и выходных данных, и выбора полинома.
    Обобщенный алгоритм может использоваться для проверки параметров.
    Для проверки работы алгоритма может использоваться контрольная сумма (check) от ASCII string "123456789"

CRC-64
    width=64 poly=0x42f0e1eba9ea3693 init=0x0000000000000000 refin=false refout=false xorout=0x0000000000000000 check=0x6c40df5f0b497347 name="CRC-64"

    Used in DLT-1 tape cartridges.
    ECMA standard ECMA-182 (December 1992)

CRC-64/XZ
    width=64 poly=0x42f0e1eba9ea3693 init=0xffffffffffffffff refin=true refout=true xorout=0xffffffffffffffff check=0x995dc9bbdf1939fa name="CRC-64/XZ"

CRC-32 Alias: CRC-32/ADCCP, PKZIP
    width=32 poly=0x04c11db7 init=0xffffffff refin=true refout=true xorout=0xffffffff check=0xcbf43926 name="CRC-32"

    ITU-T Recommendation V.42 (March 2002)

CRC-32/BZIP2 Alias: CRC-32/AAL5, CRC-32/DECT-B, B-CRC-32
    width=32 poly=0x04c11db7 init=0xffffffff refin=false refout=false xorout=0xffffffff check=0xfc891918 name="CRC-32/BZIP2"

    Used in DECT B-fields.
    Black's example AAL5 cells, with bytes 00 00 00 28 inserted between the described data fields and their CRCs, equal the examples in I.363.5.
    ITU-T Recommendation I.363.5 (August 1996)

CRC-32C Alias: CRC-32/ISCSI, CRC-32/CASTAGNOLI
    width=32 poly=0x1edc6f41 init=0xffffffff refin=true refout=true xorout=0xffffffff check=0xe3069283 name="CRC-32C"

    IETF RFC 3720 (April 2004)
    IETF RFC 4960

CRC-32K (Koopman)
    width=32 poly=0x741B8CD7
CRC-32????
    width=32 poly=0xF4ACFB13
    width=32 poly=0x32583499
    width=32 poly=0x20044009
    width=32 poly=0xA833982B
    width=32 poly=0x00210801 (этот вариант позволяет заменить CLMUL на MUL)
    \see <http://www.ece.cmu.edu/~koopman/>

CRC-32K/BACnet (Koopman)
	width=32 poly= 0x741b8cd7 (0xEB31D82E) init=0xffffffff refin=true refout=true xorout=0xffffffff check=0x2D3DD0AE name="CRC-32K/BACnet"
    0x741b8cd7 (0xEB31D82E)

CRC-32D
    width=32 poly=0xa833982b init=0xffffffff refin=true refout=true xorout=0xffffffff check=0x87315576 name="CRC-32D"

CRC-32/MPEG-2
    width=32 poly=0x04c11db7 init=0xffffffff refin=false refout=false xorout=0x00000000 check=0x0376e6e7 name="CRC-32/MPEG-2"

    ISO/IEC 13818-1:2000 — ITU-T Recommendation H.222.0 (February 2000)

CRC-32/POSIX Alias: CKSUM
    width=32 poly=0x04c11db7 init=0x00000000 refin=false refout=false xorout=0xffffffff check=0x765e7680 name="CRC-32/POSIX"

CRC-32Q
    width=32 poly=0x814141ab init=0x00000000 refin=false refout=false xorout=0x00000000 check=0x3010bf7f name="CRC-32Q"

    Used for aeronautical data. Recognised by the ICAO.


CRC-24 Alias: CRC-24/OPENPGP
    width=24 poly=0x864cfb init=0xb704ce refin=false refout=false xorout=0x000000 check=0x21cf02 name="CRC-24"

    IETF RFC 4880 (November 2007)

CRC-16/CCITT-FALSE
    width=16 poly=0x1021 init=0xffff refin=false refout=false xorout=0x0000 check=0x29b1 name="CRC-16/CCITT-FALSE"

    An algorithm commonly misidentified as CRC-CCITT. For the true CCITT algorithm see KERMIT. For the later ITU-T algorithm see X.25.

CRC-16/DECT-R
	width=16 poly=0x0589 init=0x0000 refin=false refout=false xorout=0x0001 check=0x007e name="CRC-16/DECT-R"
CRC-16/DECT-X Alias: X-CRC-16
    width=16 poly=0x0589 init=0x0000 refin=false refout=false xorout=0x0000 check=0x007f name="CRC-16/DECT-X"

CRC-16/GENIBUS Alias: CRC-16/EPC, CRC-16/I-CODE, CRC-16/DARC
    width=16 poly=0x1021 init=0xffff refin=false refout=false xorout=0xffff check=0xd64e name="CRC-16/GENIBUS"

    Used in standardised RFID tags. Presented high byte first.
    EPCglobal Inc™ (23 October 2008), UHF Class 1 Gen 2: Air Interface Protocol Standard version 1.2.0
    Philips Semiconductors (30 January 2004), SL2 ICS11 Product Specification, revision 3.0 (courtesy of NXP)
    ETSI EN 300 751 version 1.2.1 (January 2003)

CRC-16/MAXIM
    width=16 poly=0x8005 init=0x0000 refin=true refout=true xorout=0xffff check=0x44c2 name="CRC-16/MAXIM"

    Maxim Integrated (8 August 2012), DS1921G Datasheet

CRC-16/MCRF4XX
    width=16 poly=0x1021 init=0xffff refin=true refout=true xorout=0x0000 check=0x6f91 name="CRC-16/MCRF4XX"

    Youbok Lee, PhD, Microchip Technology Inc. (16 July 2001), "CRC Algorithm for MCRF45X Read/Write Device"

CRC-16/PROFIBUS
	width=16 poly=0x1dcf init=0xffff refin=false refout=false xorout=0xffff check=0xa819 residue=0xe394 name="CRC-16/PROFIBUS"
	
CRC-16/T10-DIF
    width=16 poly=0x8bb7 init=0x0000 refin=false refout=false xorout=0x0000 check=0xd0db name="CRC-16/T10-DIF"

    Used in the SCSI Data Integrity Field. XorOut = 0xBADB is proposed to mark known bad blocks.

KERMIT Alias: CRC-16/CCITT, CRC-16/CCITT-TRUE, CRC-CCITT
    width=16 poly=0x1021 init=0x0000 refin=true refout=true xorout=0x0000 check=0x2189 name="KERMIT"

    Press et al. identify the CCITT algorithm with the one implemented in Kermit. V.41 is endianness-agnostic, referring only to bit sequences, but the CRC appears reflected when used with LSB-first modems. Ironically, the unreflected form is used in XMODEM.
    For the algorithm often misidentified as CCITT, see CCITT-FALSE. For the later ITU-T algorithm see X.25.
    ITU-T Recommendation V.41 (November 1988)

MODBUS
    width=16 poly=0x8005 init=0xffff refin=true refout=true xorout=0x0000 check=0x4b37 name="MODBUS"

    CRC presented low byte first.
    MODICON Inc. (June 1996), Modbus Protocol Reference Guide, Rev. J

X-25 Alias: CRC-16/IBM-SDLC, CRC-16/ISO-HDLC, CRC-B, CRC-16/BACnet
    width=16 poly=0x1021 init=0xffff refin=true refout=true xorout=0xffff check=0x906e name="X-25"

    HDLC is defined in ISO/IEC 13239. CRC_B is defined in ISO/IEC 14443-3.
    ITU-T Recommendation T.30 (September 2005) Full mathematical description (Section 5.3.7, p.78)
    ITU-T Recommendation V.42 (March 2002) Full mathematical description (Section 8.1.1.6.1, p.17)
    ITU-T Recommendation X.25 (October 1996) Full mathematical description (Section 2.2.7.4, p.9)
    IETF RFC 1171 (July 1990)

XMODEM Alias: ZMODEM, CRC-16/ACORN
    width=16 poly=0x1021 init=0x0000 refin=false refout=false xorout=0x0000 check=0x31c3 name="XMODEM"

    The MSB-first form of the V.41 algorithm. For the LSB-first form see KERMIT. CRC presented high byte first.
    Used in the MultiMediaCard interface. In XMODEM and Acorn MOS the message bits are processed out of transmission order, compromising the guarantees on burst error detection.
    ITU-T Recommendation V.41 (November 1988)

CRC-16/CDMA2000
	width=16 poly=0xc867 init=0xffff refin=false refout=false xorout=0x0000 check=0x4c06 name="CRC-16/CDMA2000"

CRC-16/DNP
	width=16 poly=0x3d65 init=0x0000 refin=true refout=true xorout=0xffff check=0xea82 name="CRC-16/DNP"

CRC-16/EN-13757
	width=16 poly=0x3d65 init=0x0000 refin=false refout=false xorout=0xffff check=0xc2b7 name="CRC-16/EN-13757"

    Used in the Wireless M-Bus protocol for remote meter reading.
	0x3D65 / 0xA6BC / 0x9EB2
	
CRC-15 Alias: CAN
    width=15 poly=0x4599 init=0x0000 refin=false refout=false xorout=0x0000 check=0x059e name="CRC-15"

    Robert Bosch GmbH (September 1991), CAN 2.0 Specification

CRC-5/EPC
    width=5 poly=0x09 init=0x09 refin=false refout=false xorout=0x00 check=0x00 name="CRC-5/EPC"

    Used in standardised RFID tags.
    EPCglobal Inc™ (23 October 2008), UHF Class 1 Gen 2: Air Interface Protocol Standard version 1.2.0

CRC-5/ITU
    width=5 poly=0x15 init=0x00 refin=true refout=true xorout=0x00 check=0x07 name="CRC-5/ITU"

    ITU-T Recommendation G.704 (October 1998)

CRC-5/USB
    width=5 poly=0x05 init=0x1f refin=true refout=true xorout=0x1f check=0x19 name="CRC-5/USB"

CRC-6/CDMA2000-A
    width=6 poly=0x27 init=0x3f refin=false refout=false xorout=0x00 check=0x0d name="CRC-6/CDMA2000-A"

    3rd Generation Partnership Project 2 (3GPP2) (October 2005), Physical layer standard for cdma2000 spread spectrum systems, revision D, version 2.0

CRC-6/CDMA2000-B
    width=6 poly=0x07 init=0x3f refin=false refout=false xorout=0x00 check=0x3b name="CRC-6/CDMA2000-B"

CRC-6/ITU
    width=6 poly=0x03 init=0x00 refin=true refout=true xorout=0x00 check=0x06 name="CRC-6/ITU"

    ITU-T Recommendation G.704 (October 1998)

CRC-7
    width=7 poly=0x09 init=0x00 refin=false refout=false xorout=0x00 check=0x75 name="CRC-7"

    Used in the MultiMediaCard interface.
    JEDEC Standard JESD84-A441 (March 2010) (registration required)

CRC-8
    width=8 poly=0x07 init=0x00 refin=false refout=false xorout=0x00 check=0xf4 name="CRC-8"

    The System Management Interface Forum, Inc. (3 August 2000), System Management Bus (SMBus) Specification, version 2.0

CRC-8/CDMA2000
    width=8 poly=0x9b init=0xff refin=false refout=false xorout=0x00 check=0xda name="CRC-8/CDMA2000"

CRC-8/I-CODE
    width=8 poly=0x1d init=0xfd refin=false refout=false xorout=0x00 check=0x7e name="CRC-8/I-CODE"

    Philips Semiconductors (30 January 2004), SL2 ICS11 Product Specification, revision 3.0 (courtesy of NXP)

CRC-8/MAXIM    Alias: DOW-CRC
    width=8 poly=0x31 init=0x00 refin=true refout=true xorout=0x00 check=0xa1 name="CRC-8/MAXIM"

    Used in Maxim 1-Wire® device registration numbers.
    Maxim Integrated (8 August 2012), DS1921G Datasheet

CRC-8/BAC 

	width=8 poly=0x81 init=0xff refin=true refout=true xorout=0xff check=0x89 name="CRC-8/BAC"
	BACnet-MS/TP Header: X8 + X7 + 1

    \{
*/


/*
unsigned short CRC16_Lookup[16] = {
0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF
};

Poly16B = 0x8408;
CRC16B_Lookup4[16]={
0x0000, 0x1081, 0x2102, 0x3183, 0x4204, 0x5285, 0x6306, 0x7387,
0x8408, 0x9489, 0xA50A, 0xB58B, 0xC60C, 0xD68D, 0xE70E, 0xF78F
};

CRC16	CRC16_updateT(CRC16 crc, unsigned char val){
	crc = ((crc << 4) ^ CRC16_Lookup[((crc >> 12) ^ (val >> 4)) & 0xF]) & CRC16_MASK;
	crc = ((crc << 4) ^ CRC16_Lookup[((crc >> 12) ^ (val     )) & 0xF]) & CRC16_MASK;
	return crc;
}
*/

#ifdef CRC16_ENABLE
/*! обновление контрольной суммы CRC16

Оптимизация:
    В операцию CRC можно разложить на CMUL - умножение без переноса и редуцирование,
    поскольку операция проивзодится с 4 биными числами и растояния между битами в
    полиноме превышают 4 бита, можно заменить операцию умоножения без переноса на обычное
    целочисленное умножение.
 */
CRC16	CRC16_update(CRC16 crc, unsigned char val){
	crc = ((crc << 4) ^ (POLY16 * (((crc >> 12) ^ (val >> 4)) & 0xF))) & CRC16_MASK;
	crc = ((crc << 4) ^ (POLY16 * (((crc >> 12) ^ (val     )) & 0xF))) & CRC16_MASK;
	return crc;
}
#endif

#ifdef CRC16B_ENABLE
/*! обновление контрольной суммы CRC16B */
CRC16	CRC16B_update(CRC16 crc, unsigned char val){
	crc = ((crc >> 4) ^ (POLY16B* ((crc ^ (val     )) & 0xF)))  & CRC16_MASK;
	crc = ((crc >> 4) ^ (POLY16B* ((crc ^ (val >> 4)) & 0xF)))  & CRC16_MASK;
	return crc;
}
#endif

#define ADLER_POLY 65521
#define ADLER_MASK 0xFFFFUL;
#if def ADLER32_ENABLE
/*! обновление контрольной суммы ADLER32. Начальное значение должно быть 0x1.
    Алгоритм используется в графическом формате PNG.
    */
HASH32 ADLER32_update(HASH32 adler, void *p, int len){
	const unsigned int poly = ADLER_POLY;
	unsigned long s1 = (adler      ) & ADLER_MASK;
	unsigned long s2 = (adler >> 16) & ADLER_MASK;
	unsigned char * buf = p; --buf;
	if (len) do{
		s1 = (s1 + *++buf); // префиксная команда ++ экономит одну инструкцию в цикле
		if (s1 >= poly) s1 -= poly;
		s2 += s1;
		if (s2 >= poly) s2 -= poly;
	} while (--len);
	return (s2<<16) + s1;
}
#endif
#if 0
/*! Евгенская версия алгоритма, не уверен что считает с тем же результатом
     в цикле операций меньше. Для маленьких блоков может не понадобится
     считать остаток от деления вообще.

*/
HASH32 ADLER32_update2(unsigned char *data, int len)
{
    const unsigned int poly = ADLER_POLY;
    unsigned long sum1 = 1, sum2 = 0;
    while (len)
    {
        unsigned int tlen = len > 5552 ? 5552 : len;
        len -= tlen;

        do {
            sum1 += *(data++);
            sum2 += sum1;
        }while (--tlen); // tlen всегда больше 1

        if (sum1 >= poly) sum1 %= poly;
        if (sum2 >= poly) sum2 %= poly;
//        sum2 %= ADLER_POLY;
    }

    return (sum2 << 16) | sum1;
}
#endif
#ifdef CRC32_ENABLE
#define POLY32 0x04C11DB7
/** Способ получения таблиы из большой таблицы:
    CRC32C_Lookup4[i] = CRC32C_Lookup8[i]
 */
static const CRC32 CRC32_Lookup4[16] = {
0x00000000, 0x04C11DB7, 0x09823B6E, 0x0D4326D9, 
0x130476DC, 0x17C56B6B, 0x1A864DB2, 0x1E475005, 
0x2608EDB8, 0x22C9F00F, 0x2F8AD6D6, 0x2B4BCB61, 
0x350C9B64, 0x31CD86D3, 0x3C8EA00A, 0x384FBDBD
};
CRC32 CRC32_update(CRC32 CRC, unsigned char val){
	CRC = (CRC << 4) ^ CRC32_Lookup4[((CRC >> 28) ^ (val>>4)) & 0xF ];
	CRC = (CRC << 4) ^ CRC32_Lookup4[((CRC >> 28) ^ (val   )) & 0xF ];
	return CRC;
}
#endif

#ifdef CRC32B_ENABLE
#define POLY32B 0xEDB88320 // - инвертированный POLY32
static const CRC32 CRC32B_Lookup4[16]={
0x00000000, 0x1DB71064, 0x3B6E20C8, 0x26D930AC, 
0x76DC4190, 0x6B6B51F4, 0x4DB26158, 0x5005713C, 
0xEDB88320, 0xF00F9344, 0xD6D6A3E8, 0xCB61B38C, 
0x9B64C2B0, 0x86D3D2D4, 0xA00AE278, 0xBDBDF21C
};

CRC32 CRC32B_update(CRC32 CRC, unsigned char val){
	CRC = (CRC>>4) ^ CRC32B_Lookup4[(CRC ^ (val   )) & 0xF];
	CRC = (CRC>>4) ^ CRC32B_Lookup4[(CRC ^ (val>>4)) & 0xF];
	return CRC;
}

/*! \brief расчет циклической контрольной суммы от блока данных

    Алгоритм CRC-32B может быть получен из CRC-32-ССITT путем отражения бит на входе и на выходе
    do {
        crc = CRC32_update(crc, rev8(val) );
    } while ...
    return rev32(crc);
 */
CRC32 crc_from_block(CRC32 crc, unsigned char *buffer, int size)
{
	crc = CRC32_init(crc);
	int count = size;
	CRC32 const * table = CRC32B_Lookup4;
	do{
//		crc = CRC32B_update(crc, *buffer++);
		unsigned int val = *buffer++;
		crc = (crc>>4) ^ table[(crc ^ (val   )) & 0xF];
		crc = (crc>>4) ^ table[(crc ^ (val>>4)) & 0xF];
	} while (--count);
	return CRC32_finalize(crc);
}
#endif

#ifdef CRC32C_ENABLE
#define POLY32C 0x1EDC6F41
/*! Способ получения таблицы из большой таблицы:
    CRC32C_Lookup4[i] = CRC32C_Lookup8[i<<4]
    Способ получения таблицы может быть с использованием Carry-less multiplication:
    CRC32C_Lookup4[i] = CLMUL(0x105EC76F, i)
*/
static const CRC32 CRC32C_Lookup4[16] = {
0x00000000L, 0x105EC76FL, 0x20BD8EDEL, 0x30E349B1L,
0x417B1DBCL, 0x5125DAD3L, 0x61C69362L, 0x7198540DL,
0x82F63B78L, 0x92A8FC17L, 0xA24BB5A6L, 0xB21572C9L,
0xC38D26C4L, 0xD3D3E1ABL, 0xE330A81AL, 0xF36E6F75L,
};

/*!
    CRC-32C (Castagnoli) 	iSCSI, SCTP, G.hn payload, SSE4.2, Btrfs, ext4 	0x1EDC6F41 	инверсный полином 0x82F63B78
    \see [RFC 4960] Appendix B. CRC32c Checksum Calculation <http://tools.ietf.org/html/rfc4960#appendix-B>
*/
CRC32 CRC32C_update(CRC32 CRC, unsigned char val){
	CRC = (CRC>>4) ^ CRC32C_Lookup4[(CRC ^ (val   )) & 0xF];
	CRC = (CRC>>4) ^ CRC32C_Lookup4[(CRC ^ (val>>4)) & 0xF];
	return CRC;
}

/*
static const CRC32 CRC32Ci_Lookup4[16] = {
0x00000000, 0x1EDC6F41, 0x3DB8DE82, 0x2364B1C3,
0x7B71BD04, 0x65ADD245, 0x46C96386, 0x58150CC7,
0xF6E37A08, 0xE83F1549, 0xCB5BA48A, 0xD587CBCB,
0x8D92C70C, 0x934EA84D, 0xB02A198E, 0xAEF676CF
};

CRC32 CRC32Ci_update(CRC32 CRC, unsigned char val){
	CRC = (CRC << 4) ^ CRC32Ci_Lookup4[((CRC >> 28) ^ (val>>4)) & 0xF ];
	CRC = (CRC << 4) ^ CRC32Ci_Lookup4[((CRC >> 28) ^ (val   )) & 0xF ];
	return CRC;
}*/
#endif
#ifdef CRC32K_ENABLE
// CRC-32K/BACnet (Koopman) POLY=0xEB31D82E
static const CRC32 CRC32K_Lookup4[16] = {
0x00000000, 0x83CF0F3C, 0xD1FDAE25, 0x5232A119,
0x7598EC17, 0xF657E32B, 0xA4654232, 0x27AA4D0E,
0xEB31D82E, 0x68FED712, 0x3ACC760B, 0xB9037937,
0x9EA93439, 0x1D663B05, 0x4F549A1C, 0xCC9B9520,
};
CRC32 CRC32K_update   (CRC32 CRC, uint8_t val){
	CRC = (CRC >> 4) ^ CRC32K_Lookup4[(CRC ^ (val   )) & 0xF ];
	CRC = (CRC >> 4) ^ CRC32K_Lookup4[(CRC ^ (val>>4)) & 0xF ];
	return CRC;
}
#endif
#ifdef CRC64XZ_ENABLE
//CRC-64/XZ POLY=0xC96C5795D7870F42
static const CRC64 CRC64_Lookup4[16] = {
0x0000000000000000, 0x7D9BA13851336649, 0xFB374270A266CC92, 0x86ACE348F355AADB,
0x64B62BCAEBC387A1, 0x192D8AF2BAF0E1E8, 0x9F8169BA49A54B33, 0xE21AC88218962D7A,
0xC96C5795D7870F42, 0xB4F7F6AD86B4690B, 0x325B15E575E1C3D0, 0x4FC0B4DD24D2A599,
0xADDA7C5F3C4488E3, 0xD041DD676D77EEAA, 0x56ED3E2F9E224471, 0x2B769F17CF112238,
};
#define CRC64_CHECK 0x995dc9bbdf1939faULL
CRC64 CRC64_update   (CRC64 CRC, uint8_t val){
	CRC = (CRC >> 4) ^ CRC64_Lookup4[(CRC ^ (val   )) & 0xF ];
	CRC = (CRC >> 4) ^ CRC64_Lookup4[(CRC ^ (val>>4)) & 0xF ];
	return CRC;
}
#endif

#ifdef CRC16_MODBUS_ENABLE
#define MODBUS_POLY 0xA001
/*
unsigned short modbus_crc16_update(unsigned short crc, unsigned char data)
{
    crc ^= (unsigned short)data;
    int count = 8;
    do {
        if (crc & 0x01)
        {
            crc >>= 1;
            crc ^= MODBUS_POLY;
        } else {
            crc >>= 1;
        }
    } while (--count);
    return crc;
}
*/

static const CRC16 CRC16M_Lookup4[16]={
    0x0000, 0xCC01, 0xD801, 0x1400, 0xF001, 0x3C00, 0x2800, 0xE401,
    0xA001, 0x6C00, 0x7800, 0xB401, 0x5000, 0x9C01, 0x8801, 0x4400
};

CRC16 modbus_crc16_update(CRC16 crc, unsigned char val)
{
	crc = (crc>>4) ^ CRC16M_Lookup4[(crc ^ (val   )) & 0xF];
	crc = (crc>>4) ^ CRC16M_Lookup4[(crc ^ (val>>4)) & 0xF];
	return crc;
}

#endif

/*! \brief Вычисление контрольной суммы пакета

    Функция используется при составлении пакетов TCP и UDP, для вычисления контрольных
    сумм шапок пакетов. Фактически функция вызываетя для всех пакетов и существенно
    замедляет процесс обработки пакетов, особенно UDP, потому что выполняется над буфером данных.

*/
unsigned short ip_standard_cksum(unsigned int acc, void *data, unsigned int len)
{
    unsigned int acc1/* acc=0, acc1=0 */,i;
//  unsigned short src;
    unsigned char  *octet = (unsigned char*) data - 1;
    acc1 = (acc     ) & 0xFF;
    acc  = (acc >> 8) & 0xFF;


    i = (len>>1);
    do
    {
        acc  +=  (*++octet);
        acc1 +=  (*++octet);
    }
    while (--i);

    if (len & 1UL)
    {
        // accumulate remaining octet
        acc += (*++octet);//<<8;
    }
    acc = (acc<<8)+acc1;

    /* add deferred carry bits */
    acc = (acc >> 16) + (acc & 0x0000ffffUL);
    if ((acc & 0xffff0000) != 0)
    {
        acc = (acc >> 16) + (acc & 0x0000ffffUL);
    }
    return (~acc);
}
//! \}
#ifdef CRC_GEN_TABLE
/*! \brief генерация инверсной таблицы 16 элементов

В основе лежит операция сдвига и редуцифрования в поле галуа.
Таблица представляет собой таблицу умоножения. 
Используется отраженный (обратный) порядок представления бит в индексе 
 */
void crc_gen_inv_table(uint32_t poly, int bits)
{
	uint32_t table[16] = {0};
	uint32_t p =poly;
	int i,j;
	table[0] = 0;
	table[1] = p;
	for (i=1;(1<<i)<16;i++)
	{
		if (p&1)
			p = (p>>1) ^ poly;
		else
			p = (p>>1);
		
		table[(1<<i)] = p;
		for(j=1; j<(1<<i); j++) {
			table[(1<<i)+j] = p ^ table[j];
		}
	}
	printf("POLY=0x%0*X\n", bits/4, poly);
	for(i=0;i<16;i++){
		int ri;// reverse index 0..F
		ri = ( i&0x3)<<2 | ( i&0xC)>>2;
		ri = (ri&0x5)<<1 | (ri&0xA)>>1;
		printf("0x%0*X, ", bits/4, table[ri]);
		if ((i&0x7)==0x7) printf("\n");
	}
	printf("\n");
}
/*! \brief генерация таблицы подстановки CRC
	\param poly полином
	\param bits число бит в полиноме
	\param size число элементов в таблице 16 или 256
 */

void crc_gen_table(uint32_t poly, int bits, int size)
{
	uint32_t table[size];// = {0};
	uint32_t p =poly;
	int i,j;
	table[0] = 0;
	table[1] = p;
	for (i=1;(1<<i)<size;i++)
	{
		if (p&(1<<(bits-1))) {
			p &= ~((~0)<<(bits-1));
			p = (p<<1) ^ poly;
		} else
			p = (p<<1);
		table[(1<<i)] = p;
		for(j=1; j<(1<<i); j++) {
			table[(1<<i)+j] = p ^ table[j];
		}
	}
	printf("POLY=0x%0*X\n", bits/4, poly);
	for(i=0;i<size;i++){
		printf("0x%0*X, ", bits/4, table[i]);
		if ((i&0x7)==0x7) printf("\n");
	}
	printf("\n");
}
#endif
