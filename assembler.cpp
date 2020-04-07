#include <stdio.h>

unsigned char memory[2<<15];

unsigned short ax = 0, bx = 0, cx = 0, dx = 0, di = 0, si = 0, bp = 0;
unsigned short sp = (2<<15)-2;

unsigned short *pax = &ax;
unsigned short *pbx = &bx;
unsigned short *pcx = &cx;
unsigned short *pdx = &dx;
unsigned short *pdi = &di;
unsigned short *psi = &si;
unsigned short *pbp = &bp;
unsigned short *psp = &sp;

unsigned char *pah = (unsigned char *) ( ( (unsigned char *) &ax) + 1);
unsigned char *pal = (unsigned char *) &ax;
unsigned char *pbh = (unsigned char *) ( ( (unsigned char *) &bx) + 1);
unsigned char *pbl = (unsigned char *) &bx;
unsigned char *pch = (unsigned char *) ( ( (unsigned char *) &cx) + 1);
unsigned char *pcl = (unsigned char *) &cx;
unsigned char *pdh = (unsigned char *) ( ( (unsigned char *) &dx) + 1);
unsigned char *pdl = (unsigned char *) &dx;

bool ZF, CF, AF, SF, OF;

//MOV methods by operand types
template <typename regtype>
void mov_reg_word (regtype *preg, unsigned short word) {
    *preg = word;
}
template <typename regtype>
void mov_reg_byte (regtype *preg, unsigned char byte) {
    *preg = byte;
}
template <typename regtype>
void mov_reg_reg (regtype *preg1, regtype *preg2) {
    *preg1 = *preg2;
}
template <typename regtype>
void mov_reg_mem_word (regtype *preg, unsigned short wordLocation) {
    unsigned short word = ( ( memory[wordLocation] & 0xFF ) << 8 ) | ( memory[wordLocation+1] & 0xFF );
    *preg = word;
}
template <typename regtype>
void mov_reg_mem_byte (regtype *preg, unsigned short byteLocation) {
    *preg = memory[byteLocation];
}
void mov_mem_byte (unsigned short memLocation, unsigned char byte) {
    memory[memLocation] = byte;
}
void mov_mem_word (unsigned short memLocation, unsigned short word) {
    unsigned char wordLow = word & 0xff;
    unsigned char wordHigh = (word >> 8);
    memory[memLocation] = wordHigh;
    memory[memLocation] = wordLow;
}
template <typename regtype>
void mov_mem_byte_reg (regtype *preg, unsigned short memLocation) {
    memory[memLocation] = *preg;
}
template <typename regtype>
void mov_mem_word_reg (regtype *preg, unsigned short memLocation) {
    unsigned char pregLow = *preg & 0xff;
    unsigned char pregHigh = (*preg >> 8);
    memory[memLocation] = pregHigh;
    memory[memLocation+1] = pregLow;
}

//ADD methods by operand types
template <typename regtype>
void add_reg_byte(regtype *preg, unsigned char byte) {
    *preg += byte;
}
template <typename regtype>
void add_reg_word(regtype *preg, unsigned short word) {
    *preg += word;
}
void add_mem_byte(unsigned short memLocation, unsigned char byte) {
    memory[memLocation] += byte;
}
void add_mem_word(unsigned short memLocation, unsigned short word) {
    unsigned short memWord = ( ( memory[memLocation] & 0xFF ) << 8 ) | ( memory[memLocation+1] & 0xFF );
    memWord += word;
    unsigned char memWordLow = memWord & 0xff;
    unsigned char memWordHigh = (memWord >> 8);
    memory[memLocation] = memWordHigh;
    memory[memLocation+1] = memWordLow;
}
template <typename regtype>
void add_mem_byte_register(unsigned short memLocation, regtype *preg) {
    memory[memLocation] += *preg;
}
template <typename regtype>
void add_mem_word_register(unsigned short memLocation, regtype *preg) {
    unsigned short memWord = ( ( memory[memLocation] & 0xFF ) << 8 ) | ( memory[memLocation+1] & 0xFF );
    memWord += *preg;
    unsigned char memWordLow = memWord & 0xff;
    unsigned char memWordHigh = (memWord >> 8);
    memory[memLocation] = memWordHigh;
    memory[memLocation+1] = memWordLow;
}