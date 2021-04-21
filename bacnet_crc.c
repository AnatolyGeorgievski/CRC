/*! \brief Вычисление контрольной суммы заголовков и данных в BACnet MS/TP */
/*

Description Value CRC Register After Octet is Processed
preamble 1, not included in CRC X'55'
preamble 2, not included in CRC X'FF'
X'FF' (initial value)
frame type = TOKEN X'00' X'55'
destination address X'10' X'C2'
source address X'05' X'BC'
data length MSB = 0 X'00' X'95'
data length LSB = 0 X'00' X'73' ones complement is X'8C'
Header CRC X'8C' X'55' final result at receiver
00 10 05 00 00 8C
01 23 07 00 00 3a


Description Value CRC Accumulator After Octet is Processed
X'FFFF' (initial value)
first data octet X'01' X'1E0E'
second data octet X'22' X'EB70'
third data octet X'30' X'42EF' ones complement is X'BD10'
CRC1 (least significant octet) X'10' X'0F3A'
CRC2 (most significant octet) X'BD' X'F0B8' final result at receiver

Header CRC-8/BACnet: X8 + X7 + 1
	width=8 poly=0x81 init=0xff refin=true refout=true xorout=0xff check=0x89 name="??"

Data CRC-16/X-25: X16 + X12 + X5 + 1
	width=16 poly=0x1021 init=0xffff refin=true refout=true xorout=0xffff check=0x906e name="X-25"
*/
#include <stdint.h>



#define POLY16	0x1021
#define POLY16B	0x1081
#define CRC16B_INIT 0xFFFF;
#define CRC16B_FINI 0xFFFF;
#define CRC16B_TEST 0xFFFF;


#ifndef __PCLMUL__
/* этот вариант не использует таблицу, использует умножение вместо carry-less умножения, потому что полином позволяет это сделать.
uint16_t	bacnet_crc16_update(uint16_t crc, unsigned char val){
	crc = ((crc >> 4) ^ (POLY16B* ((crc ^ (val     )) & 0xF))) & CRC16_MASK;
	crc = ((crc >> 4) ^ (POLY16B* ((crc ^ (val >> 4)) & 0xF))) & CRC16_MASK;
	return crc;
}*/
/*! таблица для расчета CRC-8/BAC
Таблица умножения расcчитана в уме, методом сдвига полинома вправо 0x81 и редуцирования методом (хоr 0x81)
По сути это операция умножения галуа, с обратной последовательностью бит (отражением) бит на входе и выходе.
0*0x81 1*0x81 2*0x81 ... 0xF*0x81
затем элементы массива переставлены так чтобы в индексе была обратная последовательность бит
0 8 4 C ... F
 */
static uint8_t crc8_table[] =  {
 0x00, 0xF1, 0xE1, 0x10,
 0xC1, 0x30, 0x20, 0xD1,
 0x81, 0x70, 0x60, 0x91,
 0x40, 0xB1, 0xA1, 0x50
};
// 0x00 0xF1 0xE1 0x10 0xC1 0x30 0x20 0xD1 0x81 0x70 0x60 0x91 0x40 0xB1 0xA1 0x50
static uint8_t	bacnet_crc8_update(uint8_t crc, unsigned char val)
{
	crc = (crc>>4) ^ crc8_table[(crc ^ (val   )) & 0xF];
	crc = (crc>>4) ^ crc8_table[(crc ^ (val>>4)) & 0xF];
	return crc;
}
uint8_t	bacnet_crc8(const uint8_t * buffer){
	uint8_t crc8= 0xFF;
	int i;
	for(i=0;i<5;i++)
		crc8 = bacnet_crc8_update (crc8, buffer[i]);
	return crc8 ^ 0xFF;
}
static uint16_t crc16b_table[16]={
0x0000, 0x1081, 0x2102, 0x3183, 0x4204, 0x5285, 0x6306, 0x7387,
0x8408, 0x9489, 0xA50A, 0xB58B, 0xC60C, 0xD68D, 0xE70E, 0xF78F
};
static uint16_t	bacnet_crc16_update(uint16_t crc, unsigned char val)
{
	crc = (crc>>4) ^ crc16_table[(crc ^ (val   )) & 0xF];
	crc = (crc>>4) ^ crc16_table[(crc ^ (val>>4)) & 0xF];
	return crc;
}
// вычисление контрольной суммы от данных
uint16_t bacnet_crc16 (const uint8_t * buffer, int data_len)
{
	uint16_t crc = 0xFFFF;
	int i;
	for(i=0;i<data_len;i++)
		crc = bacnet_crc16_update (crc, buffer[i]);
	return crc ^ 0xFFFF;
}
static const uint32_t crc32k_table[] =  {
0x00000000, 0x83CF0F3C, 0xD1FDAE25, 0x5232A119,
0x7598EC17, 0xF657E32B, 0xA4654232, 0x27AA4D0E,
0xEB31D82E, 0x68FED712, 0x3ACC760B, 0xB9037937,
0x9EA93439, 0x1D663B05, 0x4F549A1C, 0xCC9B9520,
};

static uint32_t bacnet_crc32k_update(uint32_t crc, uint8_t val){
	crc = (crc>>4) ^ crc32k_table[(crc ^ (val   )) & 0xF];
	crc = (crc>>4) ^ crc32k_table[(crc ^ (val>>4)) & 0xF];
	return crc;
}
/*! \brief Добавить контрольную сумму в конец кадра
	расчет контрольной суммы кадра производится методом CRC32 (Koopman)
	\see ...

 */
uint32_t bacnet_crc32k(const uint8_t * data, size_t length)
{
	uint32_t crc32k = ~0UL;
	int i;
	for (i=0; i<length; i++)
		crc32k = bacnet_crc32k_update(crc32k, data[i]);
    return crc32k;
}
#else // Присутствует инструкция PCLMUL
#include <x86intrin.h>
typedef uint64_t poly64x2_t __attribute__((__vector_size__(16)));
typedef  int64_t v2di __attribute__((__vector_size__(16)));
#define CL_MUL128(a,b,c) ((poly64x2_t)__builtin_ia32_pclmulqdq128 ((v2di)(a),(v2di)(b),c))
//#define CL_MUL128(a,b,c) ((poly64x2_t)_mm_clmulepi64_si128((a),(b),c))
#define LOAD128U(p) ((poly64x2_t)_mm_loadu_si128((char*)p))
static inline __m128i LOAD128U_len(const uint8_t *mem, size_t len){
#if defined(__AVX512VL__) && defined(__AVX512BW__)
    return _mm_maskz_loadu_epi8 (((__mmask16)0xFFFF)>>(16-len), mem); // vmovdqu8
#else
    __v16qi v= (__v16qi){0};
    __builtin_memcpy(&v, mem, len);
    return (__m128i)v;
#endif // __AVX512VL__
}
// Структура коэффициентов
struct _CRC_ctx {
	poly64x2_t K34[16];
	poly64x2_t K12;
	poly64x2_t KBP;
};

uint8_t bacnet_crc8(const uint8_t * buffer){
    const poly64x2_t KBP = {0x808182878899AAFFULL<<24, 0x103};
	uint64_t val = (buffer[0]^0xFF) | (uint64_t)(*(uint32_t*)&buffer[1])<<8;
	poly64x2_t c = {val};
	poly64x2_t t = CL_MUL128(c, KBP, 0x00);
	c = CL_MUL128(t, KBP, 0x10);
	return c[1] ^ 0xFF;// вынести CRC(0xFF) сюда
}
static const struct _CRC_ctx CRC32K_ctx= {
.KBP = {0xC25DD01C17D232CD, 0x1D663B05D},
.K12 = {0x7B4BC878, 0x9D65B2A5},// x^{159}, x^{95}
.K34 = {
[ 1] = {0x91F9A353, 0x13F534A1},// x^{-25}, x^{-89}
[ 2] = {0x9B1BE78B, 0xAC4A47F5},// x^{-17}, x^{-81}
[ 3] = {0xC790B954, 0x7A51D862},// x^{-9}, x^{-73}
[ 4] = {0xD663B05D, 0x5F572A23},// x^{-1}, x^{-65}
[ 5] = {0x01000000, 0xBC7F040C},// x^{7}, x^{-57}
[ 6] = {0x00010000, 0x61A83B55},// x^{15}, x^{-49}
[ 7] = {0x00000100, 0x40504C15},// x^{23}, x^{-41}
[ 8] = {0x00000001, 0x35E95875},// x^{31}, x^{-33}
[ 9] = {0x9695C4CA, 0x91F9A353},// x^{39}, x^{-25}
[10] = {0x24901FAA, 0x9B1BE78B},// x^{47}, x^{-17}
[11] = {0x80475843, 0xC790B954},// x^{55}, x^{-9}
[12] = {0x18C5564C, 0xD663B05D},// x^{63}, x^{-1}
[13] = {0x14946D10, 0x01000000},// x^{71}, x^{7}
[14] = {0x83DB9B51, 0x00010000},// x^{79}, x^{15}
[15] = {0x6041FC7A, 0x00000100},// x^{87}, x^{23}
[ 0] = {0x9D65B2A5, 0x00000001},// x^{95}, x^{31}
}};
static const struct _CRC_ctx CRC16M_ctx= {
.KBP = {0xF0FFEBFFCFFFBFFF, 0x14003},
.K12 = {0x90C1, 0xCCC1},// x^{143}, x^{79}
.K34 = {
[ 1] = {0x2666, 0x9BDB},// x^{-41}, x^{-105}
[ 2] = {0x2AA6, 0x5BDB},// x^{-33}, x^{-97}
[ 3] = {0x7AAA, 0x5B1B},// x^{-25}, x^{-89}
[ 4] = {0x7FFA, 0x0B1B},// x^{-17}, x^{-81}
[ 5] = {0x43FF, 0x0B4B},// x^{-9}, x^{-73}
[ 6] = {0x4003, 0x374B},// x^{-1}, x^{-65}
[ 7] = {0x0100, 0x3777},// x^{7}, x^{-57}
[ 8] = {0x0001, 0x2677},// x^{15}, x^{-49}
[ 9] = {0xC0C1, 0x2666},// x^{23}, x^{-41}
[10] = {0x9001, 0x2AA6},// x^{31}, x^{-33}
[11] = {0xC051, 0x7AAA},// x^{39}, x^{-25}
[12] = {0xFC01, 0x7FFA},// x^{47}, x^{-17}
[13] = {0xC03D, 0x43FF},// x^{55}, x^{-9}
[14] = {0xD101, 0x4003},// x^{63}, x^{-1}
[15] = {0xC010, 0x0100},// x^{71}, x^{7}
[ 0] = {0xCCC1, 0x0001},// x^{79}, x^{15}
}};
static const struct _CRC_ctx CRC16B_ctx= {
.KBP = {0x859B040B1C581911ULL, 0x10811ULL},
.K12 = {0x8E10, 0x81BF},// x^{143}, x^{79}
.K34 = {
[ 1] = {0x4EA8, 0x97B7},// x^{-41}, x^{-105}
[ 2] = {0x290C, 0xC1A3},// x^{-33}, x^{-97}
[ 3] = {0xCA45, 0x9750},// x^{-25}, x^{-89}
[ 4] = {0x1563, 0x5212},// x^{-17}, x^{-81}
[ 5] = {0x5188, 0x33C1},// x^{-9}, x^{-73}
[ 6] = {0x0811, 0xD7B6},// x^{-1}, x^{-65}
[ 7] = {0x0100, 0xD06A},// x^{ 7}, x^{-57}
[ 8] = {0x0001, 0xCC8C},// x^{15}, x^{-49}
[ 9] = {0x1189, 0x4EA8},// x^{23}, x^{-41}
[10] = {0x19D8, 0x290C},// x^{31}, x^{-33}
[11] = {0x5ADC, 0xCA45},// x^{39}, x^{-25}
[12] = {0x1CBB, 0x1563},// x^{47}, x^{-17}
[13] = {0x0B44, 0x5188},// x^{55}, x^{-9}
[14] = {0x042B, 0x0811},// x^{63}, x^{-1}
[15] = {0x9FD5, 0x0100},// x^{71}, x^{ 7}
[ 0] = {0x81BF, 0x0001},// x^{79}, x^{15}
}};
static
unsigned int 	CRC64B_update_N(const struct _CRC_ctx * ctx,  unsigned int crc, const uint8_t *data, size_t len){
	poly64x2_t c = {crc};
	int blocks = (len+15) >> 4;
	while (--blocks>0){
		c^= (poly64x2_t)LOAD128U(data); data+=16;
		c = CL_MUL128(c, ctx->K12, 0x00) // 128+15 143
		  ^ CL_MUL128(c, ctx->K12, 0x11);//  64+15  79
	}
	len &= 15;
	if (len){
		c^= (poly64x2_t)LOAD128U_len(data, len);
	} else
		c^= (poly64x2_t)LOAD128U(data);
	c = CL_MUL128(c, ctx->K34[len], 0x00) // 15+64
	  ^ CL_MUL128(c, ctx->K34[len], 0x11);// 15
	poly64x2_t t;
	t  = CL_MUL128(c, ctx->KBP, 0x00);
	c ^= CL_MUL128(t, ctx->KBP, 0x10);
	return c[1];
}
uint16_t bacnet_crc16(const uint8_t * data, int length){
    return 0xFFFF ^ CRC64B_update_N(&CRC16B_ctx, 0xFFFF, data, length);
}
uint32_t bacnet_crc32k(const uint8_t * data, size_t length){
    return CRC64B_update_N(&CRC32K_ctx, 0xFFFFFFFF, data, length);
}
uint16_t modbus_crc(const uint8_t *data, size_t length) {
    return CRC64B_update_N(&CRC16M_ctx, 0xFFFF, data, length);
}
#endif // __PCLMUL__

#ifdef BACNET_TEST_CRC

#include <stdio.h>
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
		int ri;
		ri = ( i&0x3)<<2 | ( i&0xC)>>2;
		ri = (ri&0x5)<<1 | (ri&0xA)>>1;
		printf("0x%0*X, ", bits/4, table[ri]);
		if ((i&0x7)==0x7) printf("\n");
	}
	printf("\n");
}

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
int main()
{
	crc_gen_inv_table(0xEB31D82E,32);
	crc_gen_table(0x04C11DB7,32,16);
	crc_gen_table(0x04C11DB7,32,256);
	crc_gen_inv_table(0x8408,16);
	crc_gen_inv_table(0xA001,16); // - modbus
	crc_gen_inv_table(0x81,8);
	crc_gen_table(0x81,8,16); // прямая
	//uint8_t data[] = {0x01,0x22,0x30, 0x10,0xbd};
	uint8_t dat1[] ={
		0x05,0x07,0x00,0x00, 0x16,0x2b,0x01,0x0c,
		0x00,0x01,0x06,0xc0, 0xa8,0x00,0x69,0xba,
		0xc0,0x00,0x05,0x10, 0x0c,0x0c,0x02,0x00,
		0x00,0x07,0x19,0x4c, 0x83,0xf9};
	int i;
	uint16_t crc= 0xFFFF;
	for(i=6;i<28;i++)
		crc = bacnet_crc16_update (crc, dat1[i]);
	crc ^= 0xFFFF;
	printf("%04X CRC\n", crc);// lsb-msb

	uint8_t *data = "123456789";
	crc= 0xFFFF;
	for(i=0;i<9;i++)
		crc = bacnet_crc16_update (crc, data[i]);
	crc ^= 0xFFFF;
	printf("%04X CHK\n", crc);

	//uint8_t dat2[] = {0x00,0x10,0x05,0x00,0x00,0x8C};
	//uint8_t dat2[] = {0x01,0x23,0x07,0x00,0x00,0x3a};
	//uint8_t dat2[] = {0x06,0xff,0x07,0x00,0x14,0x25};
	//uint8_t dat2[] = {0x06,0x00,0x07,0x00,0x20,0xc7};
	uint8_t dat2[] = {0x00,0x00,0x7e,0x00,0x00,0xa7};
	uint8_t crc8= 0xFF;
	for(i=0;i<5;i++)
		crc8 = bacnet_crc8_update (crc8, dat2[i]);
	crc8 ^= 0xFF;
	printf("%02X CRC\n", crc8);
	crc8= 0xFF;
	for(i=0;i<9;i++)
		crc8 = bacnet_crc8_update (crc8, data[i]);
	crc8 ^= 0xFF;
	printf("%02X CHK\n", crc8);


	uint8_t crc81= 0xFF;
	uint8_t crc82= 0x00;
	for(i=0;i<9;i++) {
		crc81 = bacnet_crc8_update (crc81, data[i] & 0xF0);
		crc82 = bacnet_crc8_update (crc82, data[i] & 0x0F);
	}
	crc8 = crc81 ^ crc82 ^ 0xFF;
	printf("%02X CHK\n", crc8);


	printf("MUL %02X %02X  CHK\n", crc8_table[0x5] ^ crc8_table[0xA], crc8_table[0xF]);

	uint8_t dat3[] = {0x01,0x22,0x30, 0xBE,0xA5,0x22,0x7C};//7C22A5BE
	uint32_t crc32= 0xFFFFFFFF;
	for(i=0;i<3;i++)
		crc32 = bacnet_crc32k_update (crc32, dat3[i]);
	crc32 ^= 0xFFFFFFFF;
	printf("%08X CRC\n", crc32);
	crc32= 0xFFFFFFFF;
	for(i=0;i<9;i++)
		crc32 = bacnet_crc32k_update (crc32, data[i]);
	crc32 ^= 0xFFFFFFFF;
	printf("CRC32K: %08X CHK!!\n", crc32);


	uint32_t crc321= 0xFFFFFFFF;
	uint32_t crc322= 0;
	for(i=0;i<9;i++) {
		crc321 = bacnet_crc32k_update (crc321, data[i] & 0xF0);
		crc322 = bacnet_crc32k_update (crc322, data[i] & 0x0F);
	}
	crc32 = crc321 ^ crc322 ^ 0xFFFFFFFF;
	printf("%08X CHK\n", crc32);

	return 0;
}
#endif
