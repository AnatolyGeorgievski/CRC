#include <stdint.h>
#include <stddef.h>
#ifdef __ARM_NEON
#include <arm_neon.h>
static inline
poly64x2_t CL_MUL128(poly64x2_t a, poly64x2_t b, const int c)
{
	poly64_t __t1 = (poly64_t)vgetq_lane_p64(a, c & 1);
	poly64_t __t2 = (poly64_t)vgetq_lane_p64(b,(c>>4) & 1);

	return (poly64x2_t) vmull_p64 ( __t1,  __t2);
}
static inline uint8x16_t LOAD128U(const uint8_t* p) {
	return vld1q_u8(p);
}
#elif defined(__PCLMUL__)// оптимизация под инструкции x86 PCLMUL
#include <x86intrin.h>
typedef uint64_t poly64x2_t __attribute__((__vector_size__(16)));
typedef uint8_t  uint8x16_t __attribute__((__vector_size__(16)));
typedef  int64_t v2di __attribute__((__vector_size__(16)));
#define CL_MUL128(a,b,c) ((poly64x2_t)__builtin_ia32_pclmulqdq128 ((v2di)(a),(v2di)(b),c))
//#define CL_MUL128(a,b,c) ((poly64x2_t)_mm_clmulepi64_si128((__m128i)(a),(__m128i)(b),c))
#define LOAD128U(p) ((poly64x2_t)_mm_loadu_si128((__m128i_u const *)p))
static inline __m128i LOAD128U_len(const uint8_t *mem, size_t len){
#if defined(__AVX512VL__) && defined(__AVX512BW__)
    return _mm_maskz_loadu_epi8 (((__mmask16)0xFFFF)>>(16-len), mem); // vmovdqu8
#else
    __v16qi v= (__v16qi){0};
    __builtin_memcpy(&v, mem, len);
    return (__m128i)v;
}
#endif // __AVX512VL__
#endif
//static const uint8x16_t BSWAP_MASK = {15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0};
static inline uint8x16_t REVERSE(uint8x16_t x) {
    return __builtin_shufflevector (x,x, 15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0);
}
// Структура коэффициентов
struct _CRC_ctx {
	poly64x2_t K34[16];
	poly64x2_t K12;//!< fold by 1 (128 bits)
	poly64x2_t KBP;//!< final reduction: Barrett's constant and Polynom
	poly64x2_t KF2;//!< fold by 2
	poly64x2_t KF3;//!< fold by 3
	poly64x2_t KF4;//!< fold by 4
};

static const struct _CRC_ctx CRC32B_ctx= {
.KBP = {0xB4E5B025F7011641, 0x1DB710641},
.KF4 = {0x8F352D95, 0x1D9513D7},
.KF3 = {0x3DB1ECDC, 0xAF449247},
.KF2 = {0xF1DA05AA, 0x81256527},
.K12 = {0xAE689191, 0xCCAA009E},
.K34 = {
[ 1] = {0x3F036DC2, 0x40B3A940},// x^{-25}, x^{-89}
[ 2] = {0x7555A0F1, 0x769CF239},// x^{-17}, x^{-81}
[ 3] = {0xCACF972A, 0x5F7314FA},// x^{-9}, x^{-73}
[ 4] = {0xDB710641, 0x5D376816},// x^{-1}, x^{-65}
[ 5] = {0x01000000, 0xF4898239},// x^{7}, x^{-57}
[ 6] = {0x00010000, 0x5FF1018A},// x^{15}, x^{-49}
[ 7] = {0x00000100, 0x0D329B3F},// x^{23}, x^{-41}
[ 8] = {0x00000001, 0xB66B1FA6},// x^{31}, x^{-33}
[ 9] = {0x77073096, 0x3F036DC2},// x^{39}, x^{-25}
[10] = {0x191B3141, 0x7555A0F1},// x^{47}, x^{-17}
[11] = {0x01C26A37, 0xCACF972A},// x^{55}, x^{-9}
[12] = {0xB8BC6765, 0xDB710641},// x^{63}, x^{-1}
[13] = {0x3D6029B0, 0x01000000},// x^{71}, x^{7}
[14] = {0xCB5CD3A5, 0x00010000},// x^{79}, x^{15}
[15] = {0xA6770BB4, 0x00000100},// x^{87}, x^{23}
[ 0] = {0xCCAA009E, 0x00000001},// x^{95}, x^{31}
}};
#if 0
static
unsigned int 	CRC64B_update_N0(const struct _CRC_ctx * ctx,  unsigned int crc, const uint8_t *data, size_t len){
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
#endif

static const struct _CRC_ctx CRC16_ctx= {
.KBP = {0x11303471A041B343, 0x1021000000000000},
.K12 = {0xAEFC, 0x650B},// x^{128}, x^{192}
.K34 = {
[ 1] = {0xCBF3000000000000, 0x2AE4000000000000},// x^{-104}, x^{-40}
[ 2] = {0x9B27000000000000, 0x6128000000000000},// x^{-96}, x^{-32}
[ 3] = {0x15D2000000000000, 0x5487000000000000},// x^{-88}, x^{-24}
[ 4] = {0x9094000000000000, 0x9D71000000000000},// x^{-80}, x^{-16}
[ 5] = {0x17B9000000000000, 0x2314000000000000},// x^{-72}, x^{-8}
[ 6] = {0xDBD6000000000000, 0x0001000000000000},// x^{-64}, x^{ 0}
[ 7] = {0xAC16000000000000, 0x0100000000000000},// x^{-56}, x^{ 8}
[ 8] = {0x6266000000000000, 0x1021000000000000},// x^{-48}, x^{16}
[ 9] = {0x2AE4000000000000, 0x3331000000000000},// x^{-40}, x^{24}
[10] = {0x6128000000000000, 0x3730000000000000},// x^{-32}, x^{32}
[11] = {0x5487000000000000, 0x76B4000000000000},// x^{-24}, x^{40}
[12] = {0x9D71000000000000, 0xAA51000000000000},// x^{-16}, x^{48}
[13] = {0x2314000000000000, 0x45A0000000000000},// x^{-8}, x^{56}
[14] = {0x0001000000000000, 0xB861000000000000},// x^{ 0}, x^{64}
[15] = {0x0100000000000000, 0x47D3000000000000},// x^{ 8}, x^{72}
[ 0] = {0x1021000000000000, 0xEB23000000000000},// x^{16}, x^{80}
}};
/*! \brief Вычисление CRC8-CRC64
	\param crc Начальное значение суммы. При загрузке должно выполняться выравнивание по старшему биту (MSB).

*/
uint64_t 	CRC64_update_N(const struct _CRC_ctx * ctx,  uint64_t crc, const uint8_t *data, int len){
	poly64x2_t c = {0, crc};
	int blocks = (len+15) >> 4;
    if (0 && blocks>7) {// fold by 4x128 bits
        poly64x2_t c1 = {0}, c2 = {0}, c3 = {0};
__asm volatile("# LLVM-MCA-BEGIN CRC64_update_N_fold4");
        do {
			c ^= (poly64x2_t)REVERSE((uint8x16_t)LOAD128U((void*)(data   )));
			c1^= (poly64x2_t)REVERSE((uint8x16_t)LOAD128U((void*)(data+16)));
			c2^= (poly64x2_t)REVERSE((uint8x16_t)LOAD128U((void*)(data+32)));
			c3^= (poly64x2_t)REVERSE((uint8x16_t)LOAD128U((void*)(data+48)));
            c  = CL_MUL128(c , ctx->KF4, 0x00) ^ CL_MUL128(c , ctx->KF4, 0x11);
            c1 = CL_MUL128(c1, ctx->KF4, 0x00) ^ CL_MUL128(c1, ctx->KF4, 0x11);
            c2 = CL_MUL128(c2, ctx->KF4, 0x00) ^ CL_MUL128(c2, ctx->KF4, 0x11);
            c3 = CL_MUL128(c3, ctx->KF4, 0x00) ^ CL_MUL128(c3, ctx->KF4, 0x11);
            blocks-=4, data+=64;
        } while(blocks>7);
__asm volatile("# LLVM-MCA-END CRC64_update_N_fold4");
		c ^= (poly64x2_t)REVERSE((uint8x16_t)LOAD128U((void*)(data   )));
		c1^= (poly64x2_t)REVERSE((uint8x16_t)LOAD128U((void*)(data+16)));
		c2^= (poly64x2_t)REVERSE((uint8x16_t)LOAD128U((void*)(data+32)));
        c  = c3
		   ^ CL_MUL128(c , ctx->KF3, 0x00) ^ CL_MUL128(c , ctx->KF3, 0x11)
           ^ CL_MUL128(c1, ctx->KF2, 0x00) ^ CL_MUL128(c1, ctx->KF2, 0x11)
           ^ CL_MUL128(c2, ctx->K12, 0x00) ^ CL_MUL128(c2, ctx->K12, 0x11);
        blocks-=3, data+=48;
    }
__asm volatile("# LLVM-MCA-BEGIN CRC64_update_N");
    if (blocks>1) {// fold by 128 bits
        do {
			c^= (poly64x2_t)REVERSE((uint8x16_t)LOAD128U((void*)(data)));
            c = CL_MUL128(c, ctx->K12, 0x00) ^ CL_MUL128(c, ctx->K12, 0x11);
            blocks-=1, data+=16;
        } while(blocks>1);
    }
__asm volatile("# LLVM-MCA-END");
	poly64x2_t v;
	len &= 15;
	if (len){
		v = (poly64x2_t){0};
		__builtin_memcpy(&v, data, len);
	} else
		v = (poly64x2_t)LOAD128U(data);
	c^= (poly64x2_t)REVERSE((uint8x16_t)v);
	// final reduction 128 bit
	c = CL_MUL128(c, ctx->K34[len], 0x11) // 128-32
	  ^ CL_MUL128(c, ctx->K34[len], 0x00);// 64-32
	// Barrett's reduction
	poly64x2_t t;
	t  = CL_MUL128(c, ctx->KBP, 0x01)^c;//(uint64x2_t){0,c[1]};
	c ^= CL_MUL128(t, ctx->KBP, 0x11);//^(uint64x2_t){0,t[1]}; -- единица в старшем разряде Prime
//	printf("%016llx %016llx\n", c[0],c[1]);
	return c[0];
}
uint64_t 	CRC64B_update_N(const struct _CRC_ctx * ctx,  uint64_t crc, const uint8_t *data, int len){
	poly64x2_t c = {crc};
	int blocks = (len+15) >> 4;
    if (1 && blocks>7) {// fold by 4x128 bits
        poly64x2_t c1 = {0}, c2 = {0}, c3 = {0};
__asm volatile("# LLVM-MCA-BEGIN CRC64B_update_N_fold4");
        do {
			c ^= (poly64x2_t)LOAD128U((void*)(data   ));
			c1^= (poly64x2_t)LOAD128U((void*)(data+16));
			c2^= (poly64x2_t)LOAD128U((void*)(data+32));
			c3^= (poly64x2_t)LOAD128U((void*)(data+48));
            c  = CL_MUL128(c , ctx->KF4, 0x00) ^ CL_MUL128(c , ctx->KF4, 0x11);
            c1 = CL_MUL128(c1, ctx->KF4, 0x00) ^ CL_MUL128(c1, ctx->KF4, 0x11);
            c2 = CL_MUL128(c2, ctx->KF4, 0x00) ^ CL_MUL128(c2, ctx->KF4, 0x11);
            c3 = CL_MUL128(c3, ctx->KF4, 0x00) ^ CL_MUL128(c3, ctx->KF4, 0x11);
            blocks-=4, data+=64;
        } while(blocks>7);
__asm volatile("# LLVM-MCA-END CRC64B_update_N_fold4");
        c ^= (poly64x2_t)LOAD128U((void*)(data   ));
        c1^= (poly64x2_t)LOAD128U((void*)(data+16));
        c2^= (poly64x2_t)LOAD128U((void*)(data+32));
        c  = c3
		   ^ CL_MUL128(c , ctx->KF3, 0x00) ^ CL_MUL128(c , ctx->KF3, 0x11)
		   ^ CL_MUL128(c1, ctx->KF2, 0x00) ^ CL_MUL128(c1, ctx->KF2, 0x11)
           ^ CL_MUL128(c2, ctx->K12, 0x00) ^ CL_MUL128(c2, ctx->K12, 0x11);
        blocks-=3, data+=48;
    }
    if (1 && blocks>3) {// fold by 2x128 bits
        poly64x2_t c1 = {0};
__asm volatile("# LLVM-MCA-BEGIN CRC64B_update_N_fold2");
        do {
			c ^= (poly64x2_t)LOAD128U((void*)(data   ));
			c1^= (poly64x2_t)LOAD128U((void*)(data+16));
            //c  = CL_MUL128(c, ctx->K12, 0x00) ^ CL_MUL128(c, ctx->K12, 0x11);
            c  = CL_MUL128(c, ctx->KF2, 0x00) ^ CL_MUL128(c, ctx->KF2, 0x11);
            c1 = CL_MUL128(c1, ctx->KF2, 0x00) ^ CL_MUL128(c1, ctx->KF2, 0x11);
            blocks-=2, data+=32;
        } while(blocks>3);
__asm volatile("# LLVM-MCA-END CRC64B_update_N_fold2");
        c ^= (poly64x2_t)LOAD128U((void*)data);
        c  = c1 ^ CL_MUL128(c, ctx->K12, 0x00) ^ CL_MUL128(c, ctx->K12, 0x11);
        blocks-=1,  data+=16;
    }
__asm volatile("# LLVM-MCA-BEGIN CRC64B_update_N");
    if (blocks>1) {// fold by 128 bits
        do {
			poly64x2_t v = (poly64x2_t)LOAD128U((void*)data); data+=16;
			c^= v; 
            c = CL_MUL128(c, ctx->K12, 0x00) ^ CL_MUL128(c, ctx->K12, 0x11);
            blocks-=1;
        } while(blocks>1);
    }
__asm volatile("# LLVM-MCA-END CRC64B_update_N");
	len &= 15;
	if (len){
		poly64x2_t v={0};
		__builtin_memcpy(&v, data, len);
		c^= v;
	} else
		c^= (poly64x2_t)LOAD128U((void*)data);
	c = CL_MUL128(c, ctx->K34[len], 0x00) // 15+64
	  ^ CL_MUL128(c, ctx->K34[len], 0x11);// 15
	poly64x2_t t;
	t  = CL_MUL128(c, ctx->KBP, 0x00);
	c ^= CL_MUL128(t, ctx->KBP, 0x10);
	return c[1];
}
uint32_t update_crc32b(uint32_t crc, const uint8_t * data, size_t length){
    return CRC64B_update_N(&CRC32B_ctx, crc, data, length);
}
uint16_t update_crc16(uint16_t crc, const uint8_t * data, size_t length){
    return CRC64_update_N(&CRC16_ctx, (uint64_t)crc<<48, data, length)>>48;
}
#if 0 // defined(__PCLMUL)
// используется простая компактная реализация CRC32B. Переделать на быструю.
static const uint32_t CRC32B_Lookup4[16]={
0x00000000, 0x1DB71064, 0x3B6E20C8, 0x26D930AC,
0x76DC4190, 0x6B6B51F4, 0x4DB26158, 0x5005713C,
0xEDB88320, 0xF00F9344, 0xD6D6A3E8, 0xCB61B38C,
0x9B64C2B0, 0x86D3D2D4, 0xA00AE278, 0xBDBDF21C
};
uint32_t update_crc32b(uint32_t crc, const uint8_t *buffer, size_t size)
{
	if (size) do{
		crc^= *(uint8_t*)buffer++;
		crc = (crc>>4) ^ CRC32B_Lookup4[crc & 0xF];
		crc = (crc>>4) ^ CRC32B_Lookup4[crc & 0xF];
	} while (--size);
	return (crc);
}

#endif
#if defined(TEST_CRC32)
#define CRC32B_INIT 0xFFFFFFFF
#define CRC32B_XOUT 0xFFFFFFFF
#define CRC32B_CHEK 0xcbf43926

#define CRC16_INIT 0xFFFF
#define CRC16_XOUT 0x0000
#define CRC16_CHEK 0x29b1
/*
CRC-32 Alias: CRC-32/ADCCP, PKZIP
    width=32 poly=0x04c11db7 init=0xffffffff refin=true refout=true xorout=0xffffffff check=0xcbf43926 name="CRC-32"
CRC-16/CCITT-FALSE
    width=16 poly=0x1021 init=0xffff refin=false refout=false xorout=0x0000 check=0x29b1 name="CRC-16/CCITT-FALSE"
*/
#include <stdio.h>
int main(){
	uint8_t  test[]="123456789";
	uint32_t crc;
	crc = update_crc32b(CRC32B_INIT, test, 9)^CRC32B_XOUT;
	if (crc == CRC32B_CHEK) printf("CRC32 ok\n");
	crc = update_crc16(CRC16_INIT, test, 9)^CRC16_XOUT;
	if (crc == CRC16_CHEK) printf("CRC16/CCITT %04X ok\n", crc);
	return 0;
}
#endif 