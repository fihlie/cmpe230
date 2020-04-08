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


unsigned short byte_to_word(unsigned byteHigh, unsigned byteLow) {
    unsigned short word = ( ( byteHigh & 0xFF ) << 8 ) | ( byteLow & 0xFF );
    return word;
}


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
void mov_reg_mem_word (regtype *preg, unsigned short memLocation) {
    unsigned short word = byte_to_word(memory[memLocation+1], memory[memLocation]);
    *preg = word;
}
template <typename regtype>
void mov_reg_mem_byte (regtype *preg, unsigned short memLocation) {
    *preg = memory[memLocation];
}
void mov_mem_byte (unsigned short memLocation, unsigned char byte) {
    memory[memLocation] = byte;
}
void mov_mem_word (unsigned short memLocation, unsigned short word) {
    unsigned char wordLow = word & 0xff;
    unsigned char wordHigh = (word >> 8);
    memory[memLocation+1] = wordHigh;
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
    memory[memLocation+1] = pregHigh;
    memory[memLocation] = pregLow;
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
    unsigned short memWord = byte_to_word(memory[memLocation+1], memory[memLocation]);
    memWord += word;
    unsigned char memWordLow = memWord & 0xff;
    unsigned char memWordHigh = (memWord >> 8);
    memory[memLocation+1] = memWordHigh;
    memory[memLocation] = memWordLow;
}
template <typename regtype>
void add_mem_byte_register(unsigned short memLocation, regtype *preg) {
    memory[memLocation] += *preg;
}
template <typename regtype>
void add_mem_word_register(unsigned short memLocation, regtype *preg) {
    unsigned short memWord = byte_to_word(memory[memLocation+1], memory[memLocation]);
    memWord += *preg;
    unsigned char memWordLow = memWord & 0xff;
    unsigned char memWordHigh = (memWord >> 8);
    memory[memLocation+1] = memWordHigh;
    memory[memLocation] = memWordLow;
}
template <typename regtype>
void add_register_byte_mem(regtype *preg, unsigned short memLocation) {
    *preg += memory[memLocation];
}
template <typename regtype>
void add_register_word_mem(regtype *preg, unsigned short memLocation) {
    *preg += memory[memLocation];
}

template <typename regtype>
void add_register_register(regtype *preg1, regtype *preg2) {
    *preg1 += *preg2;
}

//AND methods by operand types
template <typename regtype>
void and_register_register(regtype *preg1, regtype *preg2) {
    *preg1 = *preg1 & *preg2;
}
template <typename regtype>
void and_register_byte_memory(regtype *preg, unsigned short memLocation) {
    *preg = *preg & memory[memLocation];
}
template <typename regtype>
void and_register_word_memory(regtype *preg, unsigned short memLocation) {
    unsigned short memWord = byte_to_word(memory[memLocation+1], memory[memLocation]);
    *preg = *preg & memWord;
}
template <typename regtype>
void and_memory_byte_register(regtype *preg, unsigned short memLocation) {
    memory[memLocation] = memory[memLocation] & *preg;
}
template <typename regtype>
void and_memory_word_register(regtype *preg, unsigned short memLocation) {
    memory[memLocation] = memory[memLocation] & *preg;
    unsigned short memWord = byte_to_word(memory[memLocation+1], memory[memLocation]);
    memWord = memWord & *preg;
    unsigned char memWordLow = memWord & 0xff;
    unsigned char memWordHigh = (memWord >> 8);
    memory[memLocation] = memWordLow;
    memory[memLocation+1] = memWordHigh;
}
template <typename regtype>
void and_register_byte(regtype *preg, unsigned char byte) {
    *preg = *preg & byte;
}
template <typename regtype>
void and_register_word(regtype *preg, unsigned short word) {
    *preg = *preg & word;
}
void and_memory_byte(unsigned short memLocation, unsigned char byte) {
    memory[memLocation] = memory[memLocation] & byte;
}
void and_memory_word(unsigned short memLocation, unsigned short word) {
    unsigned short memWord = byte_to_word(memory[memLocation+1], memory[memLocation]);
    memWord = memWord & word;
    unsigned char memWordLow = memWord & 0xff;
    unsigned char memWordHigh = (memWord >> 8);
    memory[memLocation+1] = memWordHigh;
    memory[memLocation] = memWordLow;
}

//OR
template <typename regtype>
void or_register_register(regtype *preg1, regtype *preg2) {
    *preg1 = *preg1 | *preg2;
}
template <typename regtype>
void or_register_byte_memory(regtype *preg, unsigned short memLocation) {
    *preg = *preg | memory[memLocation];
}
template <typename regtype>
void or_register_word_memory(regtype *preg, unsigned short memLocation) {
    unsigned short memWord = byte_to_word(memory[memLocation+1], memory[memLocation]);
    *preg = *preg | memWord;
}
template <typename regtype>
void or_memory_byte_register(regtype *preg, unsigned short memLocation) {
    memory[memLocation] = memory[memLocation] | *preg;
}
template <typename regtype>
void or_memory_word_register(regtype *preg, unsigned short memLocation) {
    memory[memLocation] = memory[memLocation] & *preg;
    unsigned short memWord = byte_to_word(memory[memLocation+1], memory[memLocation]);
    memWord = memWord | *preg;
    unsigned char memWordLow = memWord & 0xff;
    unsigned char memWordHigh = (memWord >> 8);
    memory[memLocation] = memWordLow;
    memory[memLocation+1] = memWordHigh;
}
template <typename regtype>
void or_register_byte(regtype *preg, unsigned char byte) {
    *preg = *preg | byte;
}
template <typename regtype>
void or_register_word(regtype *preg, unsigned short word) {
    *preg = *preg | word;
}
void or_memory_byte(unsigned short memLocation, unsigned char byte) {
    memory[memLocation] = memory[memLocation] | byte;
}
void or_memory_word(unsigned short memLocation, unsigned short word) {
    unsigned short memWord = byte_to_word(memory[memLocation+1], memory[memLocation]);
    memWord = memWord | word;
    unsigned char memWordLow = memWord & 0xff;
    unsigned char memWordHigh = (memWord >> 8);
    memory[memLocation+1] = memWordHigh;
    memory[memLocation] = memWordLow;
}

//XOR
template <typename regtype>
void xor_register_register(regtype *preg1, regtype *preg2) {
    *preg1 = *preg1 ^ *preg2;
}
template <typename regtype>
void xor_register_byte_memory(regtype *preg, unsigned short memLocation) {
    *preg = *preg ^ memory[memLocation];
}
template <typename regtype>
void xor_register_word_memory(regtype *preg, unsigned short memLocation) {
    unsigned short memWord = byte_to_word(memory[memLocation+1], memory[memLocation]);
    *preg = *preg ^ memWord;
}
template <typename regtype>
void xor_memory_byte_register(regtype *preg, unsigned short memLocation) {
    memory[memLocation] = memory[memLocation] ^ *preg;
}
template <typename regtype>
void xor_memory_word_register(regtype *preg, unsigned short memLocation) {
    memory[memLocation] = memory[memLocation] & *preg;
    unsigned short memWord = byte_to_word(memory[memLocation+1], memory[memLocation]);
    memWord = memWord ^ *preg;
    unsigned char memWordLow = memWord & 0xff;
    unsigned char memWordHigh = (memWord >> 8);
    memory[memLocation] = memWordLow;
    memory[memLocation+1] = memWordHigh;
}
template <typename regtype>
void xor_register_byte(regtype *preg, unsigned char byte) {
    *preg = *preg ^ byte;
}
template <typename regtype>
void xor_register_word(regtype *preg, unsigned short word) {
    *preg = *preg ^ word;
}
void xor_memory_byte(unsigned short memLocation, unsigned char byte) {
    memory[memLocation] = memory[memLocation] ^ byte;
}
void xor_memory_word(unsigned short memLocation, unsigned short word) {
    unsigned short memWord = byte_to_word(memory[memLocation+1], memory[memLocation]);
    memWord = memWord ^ word;
    unsigned char memWordLow = memWord & 0xff;
    unsigned char memWordHigh = (memWord >> 8);
    memory[memLocation+1] = memWordHigh;
    memory[memLocation] = memWordLow;
}
//SUB methods by operand types
template <typename regtype>
void sub_reg_byte(regtype *preg, unsigned char byte) {
    if(*preg>byte){
        *preg -= byte;
    }
    else{
        *preg= 0xFF-(byte-*preg);
        SF=1;
    }
}
template <typename regtype>
void sub_reg_word(regtype *preg, unsigned short word) {
    if(*preg>=word){
        *preg -= word;
    }
    else{
        *preg= 0xFFFF-(word-*preg);
        SF=1;
    }
}
void sub_mem_byte(unsigned short memLocation, unsigned char byte) {
    if(memory[memLocation] >= byte)
        memory[memLocation] -= byte;
    else {
        memory[memLocation] = 0xff - (memory[memLocation] - byte);
        SF=1;
    }
}
void sub_mem_word(unsigned short memLocation, unsigned short word) {
    unsigned short memWord = byte_to_word(memory[memLocation+1], memory[memLocation]);
    if(memWord>=word){
        memWord -= word;
    }
    else{
        memWord= 0xffff-(word-memWord);
        SF=1;
    }
    unsigned char memWordLow = memWord & 0xff;
    unsigned char memWordHigh = (memWord >> 8);
    memory[memLocation+1] = memWordHigh;
    memory[memLocation] = memWordLow;
}
template <typename regtype>
void sub_mem_byte_register(unsigned short memLocation, regtype *preg) {
    if(memory[memLocation] >= *preg)
        memory[memLocation] -= *preg;
    else{
        memory[memLocation] = 0xff-(*preg-memory[memLocation]);
        SF=1;
    }
}
template <typename regtype>
void sub_mem_word_register(unsigned short memLocation, regtype *preg) {
    unsigned short memWord = byte_to_word(memory[memLocation+1], memory[memLocation]);
    if(memWord >= *preg)
    memWord -= *preg;
    else{
        memWord=0xffff-(*preg-memWord);
        SF=1;
    }
    unsigned char memWordLow = memWord & 0xff;
    unsigned char memWordHigh = (memWord >> 8);
    memory[memLocation+1] = memWordHigh;
    memory[memLocation] = memWordLow;
}
template <typename regtype>
void sub_register_byte_mem(regtype *preg, unsigned short memLocation) {
    if(*preg >= memory[memLocation])
        *preg -= memory[memLocation];
    else{
        *preg = 0xff-(memory[memLocation]-*preg);
        SF=1;
    }
}
template <typename regtype>
void sub_register_word_mem(regtype *preg, unsigned short memLocation) {
    if(*preg >= memory[memLocation])
        *preg -= memory[memLocation];
    else{
        *preg = 0xffff-(memory[memLocation]-*preg);
        SF=1;
    }
}
template <typename regtype>
void sub_bregister_bregister(regtype *preg1, regtype *preg2) {
    if(*preg1>=*preg2)
        *preg1 -= *preg2;
    else{
        *preg1 = 0xff-(*preg2-*preg1);
        SF=1;
    }
}
template <typename regtype>
void sub_wregister_wregister(regtype *preg1, regtype *preg2) {
    if(*preg1>=*preg2)
        *preg1 -= *preg2;
    else{
        *preg1 = 0xffff-(*preg2-*preg1);
        SF=1;
    }
}

//INC methods by operand types
template <typename regtype>
void inc_reg(regtype *preg){
    *preg++;
}
void inc_mem_byte(unsigned short memLocation){
    if(memory[memLocation]!=0xff)
        memory[memLocation]++;
    else{

    }
}
void inc_mem_word(unsigned short memLocation){
    if(memory[memLocation]!=0xff)
        memory[memLocation]++;
    else{
        if(memory[memLocation+1]!=0xff){
            memory[memLocation+1]++;
        }
        else{
        }
    }

}

//DEC methods by operand types
template <typename regtype>
void dec_breg(regtype *preg){
    if(*preg!=0)
        *preg--;
    else{
        *preg=0xfe;
        SF=1;
    }
}
template <typename regtype>
void dec_wreg(regtype *preg){
    if(*preg!=0)
        *preg--;
    else{
        SF=1;
        *preg=0xfffe;
    }
}
void dec_mem_byte(unsigned short memLocation){
    if(memory[memLocation]!=0){
        memory[memLocation]--;
    }
    else{
        memory[memLocation]=0xfe;
        SF=1;
    }
}
void dec_mem_word(unsigned short memLocation){
    if(memory[memLocation]!=0){
        memory[memLocation]--;
    }
    else{
        if(memory[memLocation+1]!=0){
            memory[memLocation+1]--;
        }
        else{
            memory[memLocation]=0xfe;
            memory[memLocation+1]=0xff;
            SF=1;
        }
    }
}
//MUL methods by operand types
template <typename regtype>
void mul_breg(regtype *preg){
    if((*pal * *preg)<=0xff){
        *pax*=*preg;
    }
    else{

    }
}
template <typename regtype>
void mul_wreg(regtype *preg){
    if((*pax * *preg)<=0xffffffff){
        unsigned int memWord=*pax * *preg;
        unsigned char memWordLow = memWord & 0xffff;
        unsigned char memWordHigh= (memWord>>8);
        *pdx=memWordHigh;
        *pax=memWordLow;
    }
    else{

    }
}
void mul_mem_byte(unsigned short memLocation){
    if((*pal*memory[memLocation])<=0xffff){
        *pax*=memory[memLocation];
    }
    else{

    }
}
void mul_mem_word(unsigned short memLocation){
    unsigned int memWord = byte_to_word(memory[memLocation+1], memory[memLocation]);
    if((*pal*memWord)<=0xffffffff){
        unsigned int othermem=*pax * memWord;
        unsigned char memWordLow = othermem & 0xffff;
        unsigned char memWordHigh= (othermem>>8);
        *pdx=memWordHigh;
        *pax=memWordLow;
    }
    else{

    }
}
//DIV methods by operand types
template <typename regtype>
void div_breg(regtype *preg){
    unsigned short high = *pax%*preg;
    unsigned short low = *pax/ *preg;
    *pah=high;
    *pal=low;
}
template <typename regtype>
void div_wreg(regtype *preg){
    unsigned int word = ( ( *pdx & 0xFFFF ) << 16 ) | ( *pax & 0xFFFF );
        unsigned short high = word%*preg;
        unsigned short low = word/ *preg;
        *pdx=high;
        *pax=low;
}
void div_mem_byte(unsigned short memLocation){
    unsigned short high = *pax%memory[memLocation];
    unsigned short low = *pax/ memory[memLocation];
    *pah=high;
    *pal=low;
}
void div_mem_word(unsigned short memLocation){
    unsigned int memWord = byte_to_word(memory[memLocation+1], memory[memLocation]);
    unsigned int word = ( ( *pdx & 0xFFFF ) << 16 ) | ( *pax & 0xFFFF );
    unsigned short high = word%memWord;
    unsigned short low = word/ memWord;
    *pdx=high;
    *pax=low;
}
//PUSH methods by operand types
template <typename regtype>
void push_reg(regtype *preg){
    unsigned short first=*preg%0xff;
    unsigned short second=*preg/0xff;
    memory[sp]=first;
    memory[sp+1]=second;
    sp-=2;
}
void push_mem(unsigned short memWord){
    memory[sp]=memory[memWord];
    memory[sp+1]=memory[memWord+1];
    sp-=2;
}
void push_var(unsigned short var){
    unsigned short first=var%0xff;
    unsigned short second=var/0xff;
    memory[sp]=first;
    memory[sp+1]=second;
    sp-=2;
}
//POP methods by operand types
template <typename regtype>
void pop_reg(regtype *preg){
    unsigned short first=memory[sp];
    unsigned short second=memory[sp+1];
    unsigned short mem=byte_to_word(first,second);
    *preg=mem;
    memory[sp]=0;
    memory[sp+1]=0;
    sp+=2;
}
void pop_mem(unsigned short memWord){
    memory[memWord]=memory[sp];
    memory[memWord+1]=memory[sp+1];
    memory[sp]=0;
    memory[sp+1]=0;
    sp+=2;
}
