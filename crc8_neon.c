// crc8
#ifdef __ARM_NEON
#include <arm_neon.h>

// BACnet CRC-8 poly=81 
static const poly8x8_t K1 = {
//  7     15    23    31    39    47    55    63 -- сдвиги
	0x01, 0xFE, 0xAB, 0x98, 0x89, 0x86, 0x83, 0x80};
static const poly8x8_t Ur = {
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
static const poly8x8_t Pr = {
	0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03};


uint32_t    CRC8_update_64(uint32_t crc, uint8_t *data, int len) {
	uint64x1_t c = vld1_u8(data);
	// сдвиги
	uint16x8_t q;
	q = vmull_p8(c, K1);
	// Редуцирование 16 бит в 8 бит
	uint16x8_t t;
//	t = vmull_p8(vmovn_u16(q), Ur);
//	c = vmovn_u16(t);
	c = vmul_p8 (c, Ur);
	q^= vmull_p8(c, Pr);// старшая часть часть
// Горизонтальное суммирование компонент вектора
	c^= vshrnq_n_u16(q,8);//64
	crc = c>>32   ^ c;
	crc = crc>>16 ^ crc;
	crc = crc>>8  ^ crc;
	return crc & 0xFF;
}
/*	
static const poly8x8_t KF = {
//  64    56    48    40    96    88    80    72 -- сдвиги
	0x13, 0x79, 0x68, 0x1F, 0x5D, 0x94, 0xE5, 0xB5};
static const poly8x8_t K2 = {
//  8     16    24    32    40    48    56    64 -- сдвиги
	0x07, 0x15, 0x6B, 0x16, 0x62, 0x29, 0xDF, 0x13};
uint32_t    CRC8_update_128(uint32_t crc, uint8_t *data, int len) {
	uint64x1_t c = vld1_u64(data);
	c[0] ^= (uint64_t)crc<<56;
	if (len & 7){
		data+=(len & 7);
		c>>= 64 - ((len&7)<<3);
	} else 
		data+=8;
	int blocks = (len+7 >> 3);
	while (--blocks>0) {
		q0= vmull_p8(c,KF);
		c0 = vget_low_u16(q0) ^ vget_high_u16(q0);
		c^= vld1_u8(data); data+=8;
	}
	q = vmull_p8(c, K2);
// Редуцирование 16 бит в 8 бит
	t  = vmull_p8(vshrnq_n_u16(q,8), Ux) ^ q;
	q ^= vmull_p8(vshrnq_n_u16(t,8), Px);// младшая часть
// Горизонтальное суммирование компонент вектора 64 32 16 8 (vzup+veor)
	c = vuzp_u8(vmovn_u16(q),Z);//32
	c = vuzp_u8(c->val[0]^c->val[1],Z);//16
	c = vuzp_u8(c->val[0]^c->val[1],Z);
	c0= c->val[0]^c->val[1];
	return c0[0] & 0xFF;
}*/
// можно ли совместить операцию умножения и Барретта?
// Есть операция folding она представлена сдвигом(умножением) 
// Есть оперция умножение старшей части на баррета
#endif
