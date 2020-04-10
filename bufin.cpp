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
    if(*preg<0x80&&byte<0x80){
        if(*preg+byte>=0x80){
            OF=true;
        } else
            OF=0;
    }
    *preg += byte;
    if(*preg>0xff){
        *preg=*preg%0x100;
        CF=true;
    }
    else{
        CF=false;
    }
    if(*preg>=0x80) {
        SF=1;
    }
    else{
        SF=0;
    }
    if(*preg==0){
        ZF=true;
    }
    else{
        ZF=false;
    }
}
template <typename regtype>
void add_reg_word(regtype *preg, unsigned short word) {
    SF=0;
    if(*preg<0x8000&&word<0x8000){
        if(*preg+word>=0x8000){
            OF=true;
        } else
            OF=0;
    }
    unsigned int temp=*preg + word;
    if(temp>0xffff){
        *preg=temp%0x10000;
        CF=true;
    }
    else{
        CF=false;
    }
    if(*preg>=0x8000) {
        SF=1;
    }
    else{
        SF=0;
    }
    if(*preg==0){
        ZF=true;
    }
    else{
        ZF=false;
    }}
void add_mem_byte(unsigned short memLocation, unsigned char byte) {
    SF = 0;
    if (memory[memLocation] < 0x80 && byte < 0x80) {
        if (memory[memLocation] + byte >= 0x80) {
            OF = true;
        } else
            OF = 0;
    }
    memory[memLocation] += byte;
    if (memory[memLocation] > 0xff) {
        memory[memLocation] = memory[memLocation] % 0x100;
        CF = true;
    } else {
        CF = false;
    }
        if (memory[memLocation] >= 0x80) {
            SF = 1;
        } else {
            SF = 0;
        }
        if (memory[memLocation] == 0) {
            ZF = true;
        } else {
            ZF = false;
        }
    }

void add_mem_word(unsigned short memLocation, unsigned short word) {
    unsigned short memWord = byte_to_word(memory[memLocation+1], memory[memLocation]);
    if(memWord<0x8000&&word<0x8000){
        if(memWord+word>=0x8000){
            OF=true;
        } else
            OF=0;
    }
    unsigned int temp=memWord + word;
    if(temp>0xffff){
        memWord=temp%0x10000;
        CF=true;
    }
    else{
        CF=false;
    }
    if(memWord>=0x8000) {
        SF=1;
    }
    else{
        SF=0;
    }
    if(memWord==0){
        ZF=true;
    }
    else{
        ZF=false;
    }
    unsigned char memWordLow = memWord & 0xff;
    unsigned char memWordHigh = (memWord >> 8);
    memory[memLocation+1] = memWordHigh;
    memory[memLocation] = memWordLow;
}template <typename regtype>
void add_mem_byte_register(unsigned short memLocation, regtype *preg) {
    SF = 0;
    if (memory[memLocation] < 0x80 && *preg < 0x80) {
        if (memory[memLocation] + *preg >= 0x80) {
            OF = true;
        } else
            OF = 0;
    }
    memory[memLocation] += *preg;
    if (memory[memLocation] > 0xff) {
        memory[memLocation] = memory[memLocation] % 0x100;
        CF = true;
    } else {
        CF = false;
    }
    if (memory[memLocation] >= 0x80) {
        SF = 1;
    } else {
        SF = 0;
    }
    if (memory[memLocation] == 0) {
        ZF = true;
    } else {
        ZF = false;
    }
}
template <typename regtype>
void add_mem_word_register(unsigned short memLocation, regtype *preg) {
    unsigned short memWord = byte_to_word(memory[memLocation+1], memory[memLocation]);
    if(memWord<0x8000&&*preg<0x8000){
        if(memWord+*preg>=0x8000){
            OF=true;
        } else
            OF=0;
    }
    unsigned int temp=*preg + memWord;
    if(temp>0xffff){
        memWord=temp%0x10000;
        CF=true;
    }
    else{
        CF=false;
    }
    if(memWord>=0x8000) {
        SF=1;
    }
    else{
        SF=0;
    }
    if(memWord==0){
        ZF=true;
    }
    else{
        ZF=false;
    }    unsigned char memWordLow = memWord & 0xff;
    unsigned char memWordHigh = (memWord >> 8);
    memory[memLocation+1] = memWordHigh;
    memory[memLocation] = memWordLow;
}
template <typename regtype>
void add_register_byte_mem(regtype *preg, unsigned short memLocation) {
    SF=0;
    if(*preg<0x80&&memory[memLocation]<0x80){
        if(*preg+memory[memLocation]>=0x80){
            OF=true;
        } else
            OF=0;
    }
    *preg += memory[memLocation];
    if(*preg>0xff){
        *preg=*preg%0x100;
        CF=true;
    }
    else{
        CF=false;
    }
    if (*preg >= 0x80) {
        SF = 1;
    } else {
        SF = 0;
    }
    if(*preg==0){
        ZF=true;
    }
    else{
        ZF=false;
    }
}
template <typename regtype>
void add_register_word_mem(regtype *preg, unsigned short memLocation) {
    unsigned short word= byte_to_word(memory[memLocation+1],memory[memLocation]);
    SF=0;
    if(*preg<0x8000&&word<0x8000){
        if(*preg+word>=0x8000){
            OF=true;
        } else
            OF=0;
    }
    unsigned int temp=*preg + word;
    if(temp>0xffff){
        *preg=temp%0x10000;
        CF=true;
    }
    else{
        CF=false;
    }
    if (*preg >= 0x8000) {
        SF = 1;
    } else {
        SF = 0;
    }
    if(*preg==0){
        ZF=true;
    }
    else{
        ZF=false;
    }
}

template <typename regtype>
void add_bregister_bregister(regtype *preg1, regtype *preg2) {
    SF=0;
    if(*preg1<0x80&&*preg2<0x80){
        if(*preg1+*preg2>=0x80){
            OF=true;
        } else
            OF=0;
    }
    *preg1 += *preg2;
    if(*preg1>0xff){
        *preg1=*preg1%0x100;
        CF=true;
    }
    else{
        CF=false;
    }
    if (*preg1 >= 0x80) {
        SF = 1;
    } else {
        SF = 0;
    }
    if(*preg1==0){
        ZF=true;
    }
    else{
        ZF=false;
    }
}
template <typename regtype>
void add_wregister_wregister(regtype *preg1, regtype *preg2) {
    SF=0;
    if(*preg1<0x8000&&*preg2<0x8000){
        if(*preg1+*preg2>=0x8000){
            OF=true;
        } else
            OF=0;
    }
    unsigned int temp=*preg1 + *preg2;
    if(temp>0xffff){
        *preg1=temp%0x10000;
        CF=true;
    }
    else{
        CF=false;
    }
    if (*preg1 >= 0x8000) {
        SF = 1;
    } else {
        SF = 0;
    }
    if(*preg1==0){
        ZF=true;
    }
    else{
        ZF=false;
    }
}

//AND methods by operand types
template <typename regtype>
void and_bregister_bregister(regtype *preg1, regtype *preg2) {
    OF=0;
    CF=0;
    *preg1 = *preg1 & *preg2;
    if(*preg1==0){
        ZF=1;
    }
    else{
        ZF=0;
    }
    if(*preg1>=0x80){
        SF=1;
    }
    else{
        SF=0;
    }
}
template <typename regtype>
void and_wregister_wregister(regtype *preg1, regtype *preg2) {
    OF=0;
    CF=0;
    *preg1 = *preg1 & *preg2;
    if(*preg1==0){
        ZF=1;
    }
    else{
        ZF=0;
    }
    if(*preg1>=0x8000){
        SF=1;
    }
    else{
        SF=0;
    }
}
template <typename regtype>
void and_register_byte_memory(regtype *preg, unsigned short memLocation) {
    OF=0;
    CF=0;
    *preg = *preg & memory[memLocation];
    if(*preg==0){
        ZF=1;
    }
    else{
        ZF=0;
    }
    if(*preg>=0x80){
        SF=1;
    }
    else{
        SF=0;
    }
}
template <typename regtype>
void and_register_word_memory(regtype *preg, unsigned short memLocation) {
    unsigned short memWord = byte_to_word(memory[memLocation+1], memory[memLocation]);
    OF=0;
    CF=0;
    *preg = *preg & memWord;
    if(*preg==0){
        ZF=1;
    }
    else{
        ZF=0;
    }
    if(*preg>=0x8000){
        SF=1;
    }
    else{
        SF=0;
    }
}
template <typename regtype>
void and_memory_byte_register(regtype *preg, unsigned short memLocation) {
    OF=0;
    CF=0;
    memory[memLocation] = *preg & memory[memLocation];
    if(memory[memLocation]==0){
        ZF=1;
    }
    else{
        ZF=0;
    }
    if(memory[memLocation]>=0x80){
        SF=1;
    }
    else{
        SF=0;
    }}
template <typename regtype>
void and_memory_word_register(regtype *preg, unsigned short memLocation) {
    OF=0;
    CF=0;
    unsigned short memWord = byte_to_word(memory[memLocation+1], memory[memLocation]);
    memWord = memWord & *preg;
    if(memWord==0){
        ZF=1;
    }
    else{
        ZF=0;
    }
    if(memWord>=0x8000){
        SF=1;
    }
    else{
        SF=0;
    }
    unsigned char memWordLow = memWord & 0xff;
    unsigned char memWordHigh = (memWord >> 8);
    memory[memLocation] = memWordLow;
    memory[memLocation+1] = memWordHigh;
}
template <typename regtype>
void and_register_byte(regtype *preg, unsigned char byte) {
    OF=0;
    CF=0;
    *preg = *preg & byte;
    if(*preg==0){
        ZF=1;
    }
    else{
        ZF=0;
    }
    if(*preg>=0x80){
        SF=1;
    }
    else{
        SF=0;
    }}
template <typename regtype>
void and_register_word(regtype *preg, unsigned short word) {
    OF=0;
    CF=0;
    *preg = *preg & word;
    if(*preg==0){
        ZF=1;
    }
    else{
        ZF=0;
    }
    if(*preg>=0x8000){
        SF=1;
    }
    else{
        SF=0;
    }}
void and_memory_byte(unsigned short memLocation, unsigned char byte) {
    OF=0;
    CF=0;
    memory[memLocation] = byte & memory[memLocation];
    if(memory[memLocation]==0){
        ZF=1;
    }
    else{
        ZF=0;
    }
    if(memory[memLocation]>=0x80){
        SF=1;
    }
    else{
        SF=0;
    }
}
void and_memory_word(unsigned short memLocation, unsigned short word) {
    OF=0;
    CF=0;
    unsigned short memWord = byte_to_word(memory[memLocation+1], memory[memLocation]);
    memWord = memWord & word;
    if(memWord==0){
        ZF=1;
    }
    else{
        ZF=0;
    }
    if(memWord>=0x8000){
        SF=1;
    }
    else{
        SF=0;
    }
    unsigned char memWordLow = memWord & 0xff;
    unsigned char memWordHigh = (memWord >> 8);
    memory[memLocation+1] = memWordHigh;
    memory[memLocation] = memWordLow;
}

//OR
template <typename regtype>
void or_bregister_bregister(regtype *preg1, regtype *preg2) {
    OF=0;
    CF=0;
    *preg1 = *preg1 | *preg2;
    if(*preg1==0){
        ZF=1;
    }
    else{
        ZF=0;
    }
    if(*preg1>=0x80){
        SF=1;
    }
    else{
        SF=0;
    }
}
template <typename regtype>
void or_wregister_wregister(regtype *preg1, regtype *preg2) {
    OF=0;
    CF=0;
    *preg1 = *preg1 | *preg2;
    if(*preg1==0){
        ZF=1;
    }
    else{
        ZF=0;
    }
    if(*preg1>=0x8000){
        SF=1;
    }
    else{
        SF=0;
    }
}
template <typename regtype>
void or_register_byte_memory(regtype *preg, unsigned short memLocation) {
    OF=0;
    CF=0;
    *preg = *preg | memory[memLocation];
    if(*preg==0){
        ZF=1;
    }
    else{
        ZF=0;
    }
    if(*preg>=0x80){
        SF=1;
    }
    else{
        SF=0;
    }
}
template <typename regtype>
void or_register_word_memory(regtype *preg, unsigned short memLocation) {
    unsigned short memWord = byte_to_word(memory[memLocation+1], memory[memLocation]);
    OF=0;
    CF=0;
    *preg = *preg | memWord;
    if(*preg==0){
        ZF=1;
    }
    else{
        ZF=0;
    }
    if(*preg>=0x8000){
        SF=1;
    }
    else{
        SF=0;
    }
}
template <typename regtype>
void or_memory_byte_register(regtype *preg, unsigned short memLocation) {
    OF=0;
    CF=0;
    memory[memLocation] = *preg | memory[memLocation];
    if(memory[memLocation]==0){
        ZF=1;
    }
    else{
        ZF=0;
    }
    if(memory[memLocation]>=0x80){
        SF=1;
    }
    else{
        SF=0;
    }}
template <typename regtype>
void or_memory_word_register(regtype *preg, unsigned short memLocation) {
    OF=0;
    CF=0;
    unsigned short memWord = byte_to_word(memory[memLocation+1], memory[memLocation]);
    memWord = memWord | *preg;
    if(memWord==0){
        ZF=1;
    }
    else{
        ZF=0;
    }
    if(memWord>=0x8000){
        SF=1;
    }
    else{
        SF=0;
    }
    unsigned char memWordLow = memWord & 0xff;
    unsigned char memWordHigh = (memWord >> 8);
    memory[memLocation] = memWordLow;
    memory[memLocation+1] = memWordHigh;
}
template <typename regtype>
void or_register_byte(regtype *preg, unsigned char byte) {
    OF=0;
    CF=0;
    *preg = *preg | byte;
    if(*preg==0){
        ZF=1;
    }
    else{
        ZF=0;
    }
    if(*preg>=0x80){
        SF=1;
    }
    else{
        SF=0;
    }}
template <typename regtype>
void or_register_word(regtype *preg, unsigned short word) {
    OF=0;
    CF=0;
    *preg = *preg | word;
    if(*preg==0){
        ZF=1;
    }
    else{
        ZF=0;
    }
    if(*preg>=0x8000){
        SF=1;
    }
    else{
        SF=0;
    }}
void or_memory_byte(unsigned short memLocation, unsigned char byte) {
    OF=0;
    CF=0;
    memory[memLocation] = byte | memory[memLocation];
    if(memory[memLocation]==0){
        ZF=1;
    }
    else{
        ZF=0;
    }
    if(memory[memLocation]>=0x80){
        SF=1;
    }
    else{
        SF=0;
    }
}
void or_memory_word(unsigned short memLocation, unsigned short word) {
    OF=0;
    CF=0;
    unsigned short memWord = byte_to_word(memory[memLocation+1], memory[memLocation]);
    memWord = memWord | word;
    if(memWord==0){
        ZF=1;
    }
    else{
        ZF=0;
    }
    if(memWord>=0x8000){
        SF=1;
    }
    else{
        SF=0;
    }
    unsigned char memWordLow = memWord & 0xff;
    unsigned char memWordHigh = (memWord >> 8);
    memory[memLocation+1] = memWordHigh;
    memory[memLocation] = memWordLow;
}

//XOR
template <typename regtype>
void xor_bregister_bregister(regtype *preg1, regtype *preg2) {
    OF=0;
    CF=0;
    *preg1 = *preg1 ^ *preg2;
    if(*preg1==0){
        ZF=1;
    }
    else{
        ZF=0;
    }
    if(*preg1>=0x80){
        SF=1;
    }
    else{
        SF=0;
    }
}
template <typename regtype>
void xor_wregister_wregister(regtype *preg1, regtype *preg2) {
    OF=0;
    CF=0;
    *preg1 = *preg1 ^ *preg2;
    if(*preg1==0){
        ZF=1;
    }
    else{
        ZF=0;
    }
    if(*preg1>=0x8000){
        SF=1;
    }
    else{
        SF=0;
    }
}
template <typename regtype>
void xor_register_byte_memory(regtype *preg, unsigned short memLocation) {
    OF=0;
    CF=0;
    *preg = *preg ^ memory[memLocation];
    if(*preg==0){
        ZF=1;
    }
    else{
        ZF=0;
    }
    if(*preg>=0x80){
        SF=1;
    }
    else{
        SF=0;
    }
}
template <typename regtype>
void xor_register_word_memory(regtype *preg, unsigned short memLocation) {
    unsigned short memWord = byte_to_word(memory[memLocation+1], memory[memLocation]);
    OF=0;
    CF=0;
    *preg = *preg ^ memWord;
    if(*preg==0){
        ZF=1;
    }
    else{
        ZF=0;
    }
    if(*preg>=0x8000){
        SF=1;
    }
    else{
        SF=0;
    }
}
template <typename regtype>
void xor_memory_byte_register(regtype *preg, unsigned short memLocation) {
    OF=0;
    CF=0;
    memory[memLocation] = *preg ^ memory[memLocation];
    if(memory[memLocation]==0){
        ZF=1;
    }
    else{
        ZF=0;
    }
    if(memory[memLocation]>=0x80){
        SF=1;
    }
    else{
        SF=0;
    }}
template <typename regtype>
void xor_memory_word_register(regtype *preg, unsigned short memLocation) {
    OF=0;
    CF=0;
    unsigned short memWord = byte_to_word(memory[memLocation+1], memory[memLocation]);
    memWord = memWord ^ *preg;
    if(memWord==0){
        ZF=1;
    }
    else{
        ZF=0;
    }
    if(memWord>=0x8000){
        SF=1;
    }
    else{
        SF=0;
    }
    unsigned char memWordLow = memWord & 0xff;
    unsigned char memWordHigh = (memWord >> 8);
    memory[memLocation] = memWordLow;
    memory[memLocation+1] = memWordHigh;
}
template <typename regtype>
void xor_register_byte(regtype *preg, unsigned char byte) {
    OF=0;
    CF=0;
    *preg = *preg ^ byte;
    if(*preg==0){
        ZF=1;
    }
    else{
        ZF=0;
    }
    if(*preg>=0x80){
        SF=1;
    }
    else{
        SF=0;
    }}
template <typename regtype>
void xor_register_word(regtype *preg, unsigned short word) {
    OF=0;
    CF=0;
    *preg = *preg ^ word;
    if(*preg==0){
        ZF=1;
    }
    else{
        ZF=0;
    }
    if(*preg>=0x8000){
        SF=1;
    }
    else{
        SF=0;
    }}
void xor_memory_byte(unsigned short memLocation, unsigned char byte) {
    OF=0;
    CF=0;
    memory[memLocation] = byte ^ memory[memLocation];
    if(memory[memLocation]==0){
        ZF=1;
    }
    else{
        ZF=0;
    }
    if(memory[memLocation]>=0x80){
        SF=1;
    }
    else{
        SF=0;
    }
}
void xor_memory_word(unsigned short memLocation, unsigned short word) {
    OF=0;
    CF=0;
    unsigned short memWord = byte_to_word(memory[memLocation+1], memory[memLocation]);
    memWord = memWord ^ word;
    if(memWord==0){
        ZF=1;
    }
    else{
        ZF=0;
    }
    if(memWord>=0x8000){
        SF=1;
    }
    else{
        SF=0;
    }
    unsigned char memWordLow = memWord & 0xff;
    unsigned char memWordHigh = (memWord >> 8);
    memory[memLocation+1] = memWordHigh;
    memory[memLocation] = memWordLow;
}
//SUB methods by operand types
template <typename regtype>
void sub_reg_byte(regtype *preg, unsigned char byte) {
    if(*preg>=0x80&&byte<0x80){
        *preg -= byte;
        if(*preg<0x80){
            SF=0;
            OF=1;
            if(*preg==0){
                ZF=1;
            }
            else{
                ZF=0;
            }
        }
        else{
            SF=1;
            OF=0;
            ZF=0;
        }
        CF=0;
    }
    else if(*preg<0x80&&byte>=0x80){
        *preg= 0xFF-(byte-*preg);
        if(*preg>=0x80){
            SF=1;
            OF=1;
            ZF=0;
        }
        else{
            SF=0;
            OF=0;
            if(*preg==0){
                ZF=1;
            }
            else{
                ZF=0;
            }
        }
        CF=1;
    }
    else{
        OF=0;
        if(*preg>=byte){
            *preg -= byte;
            CF=0;
        }
        else{
            *preg= 0xFF-(byte-*preg);
            CF=1;
        }
        if(*preg==0){
            ZF=1;
        }
        else{ZF=0;}
        if(*preg>=0x80){
            SF=1;
        }
        else{
            SF=0;
        }
    }
}
template <typename regtype>
void sub_reg_word(regtype *preg, unsigned short word) {
    if(*preg>=0x8000&&word<0x8000){
        *preg -= word;
        if(*preg<0x8000){
            SF=0;
            OF=1;
            if(*preg==0){
                ZF=1;
            }
            else{
                ZF=0;
            }
        }
        else{
            SF=1;
            OF=0;
            ZF=0;
        }
        CF=0;
    }
    else if(*preg<0x8000&&word>=0x8000){
        *preg= 0xffff-(word-*preg);
        if(*preg>=0x8000){
            SF=1;
            OF=1;
            ZF=0;
        }
        else{
            SF=0;
            OF=0;
            if(*preg==0){
                ZF=1;
            }
            else{
                ZF=0;
            }
        }
        CF=1;
    }
    else{
        OF=0;
        if(*preg>=word){
            *preg -= word;
            CF=0;
        }
        else{
            *preg= 0xffff-(word-*preg);
            CF=1;
        }
        if(*preg==0){
            ZF=1;
        }
        else{ZF=0;}
        if(*preg>=0x8000){
            SF=1;
        }
        else{
            SF=0;
        }
    }
}
void sub_mem_byte(unsigned short memLocation, unsigned char byte) {
    if(memory[memLocation]>=0x80&&byte<0x80){
        memory[memLocation] -= byte;
        if(memory[memLocation]<0x80){
            SF=0;
            OF=1;
            if(memory[memLocation]==0){
                ZF=1;
            }
            else{
                ZF=0;
            }
        }
        else{
            SF=1;
            OF=0;
            ZF=0;
        }
        CF=0;
    }
    else if(memory[memLocation]<0x80&&byte>=0x80){
        memory[memLocation]= 0xFF-(byte-memory[memLocation]);
        if(memory[memLocation]>=0x80){
            SF=1;
            OF=1;
            ZF=0;
        }
        else{
            SF=0;
            OF=0;
            if(memory[memLocation]==0){
                ZF=1;
            }
            else{
                ZF=0;
            }
        }
        CF=1;
    }
    else{
        OF=0;
        if(memory[memLocation]>=byte){
            memory[memLocation] -= byte;
            CF=0;
        }
        else{
            memory[memLocation]= 0xFF-(byte-memory[memLocation]);
            CF=1;
        }
        if(memory[memLocation]==0){
            ZF=1;
        }
        else{ZF=0;}
        if(memory[memLocation]>=0x80){
            SF=1;
        }
        else{
            SF=0;
        }
    }
}
void sub_mem_word(unsigned short memLocation, unsigned short word) {
    unsigned short memWord = byte_to_word(memory[memLocation+1], memory[memLocation]);
    if(memWord>=0x8000&&word<0x8000){
        memWord -= word;
        if(memWord<0x8000){
            SF=0;
            OF=1;
            if(memWord==0){
                ZF=1;
            }
            else{
                ZF=0;
            }
        }
        else{
            SF=1;
            OF=0;
            ZF=0;
        }
        CF=0;
    }
    else if(memWord<0x8000&&word>=0x8000){
        memWord= 0xffff-(word-memWord);
        if(memWord>=0x8000){
            SF=1;
            OF=1;
            ZF=0;
        }
        else{
            SF=0;
            OF=0;
            if(memWord==0){
                ZF=1;
            }
            else{
                ZF=0;
            }
        }
        CF=1;
    }
    else{
        OF=0;
        if(memWord>=word){
            memWord -= word;
            CF=0;
        }
        else{
            memWord= 0xffff-(word-memWord);
            CF=1;
        }
        if(memWord==0){
            ZF=1;
        }
        else{ZF=0;}
        if(memWord>=0x8000){
            SF=1;
        }
        else{
            SF=0;
        }
    }
    unsigned char memWordLow = memWord & 0xff;
    unsigned char memWordHigh = (memWord >> 8);
    memory[memLocation+1] = memWordHigh;
    memory[memLocation] = memWordLow;
}
template <typename regtype>
void sub_mem_byte_register(unsigned short memLocation, regtype *preg) {
    if(memory[memLocation]>=0x80&&*preg<0x80){
        memory[memLocation] -= *preg;
        if(memory[memLocation]<0x80){
            SF=0;
            OF=1;
            if(memory[memLocation]==0){
                ZF=1;
            }
            else{
                ZF=0;
            }
        }
        else{
            SF=1;
            OF=0;
            ZF=0;
        }
        CF=0;
    }
    else if(memory[memLocation]<0x80&&*preg>=0x80){
        memory[memLocation]= 0xFF-(*preg-memory[memLocation]);
        if(memory[memLocation]>=0x80){
            SF=1;
            OF=1;
            ZF=0;
        }
        else{
            SF=0;
            OF=0;
            if(memory[memLocation]==0){
                ZF=1;
            }
            else{
                ZF=0;
            }
        }
        CF=1;
    }
    else{
        OF=0;
        if(memory[memLocation]>=*preg){
            memory[memLocation] -= *preg;
            CF=0;
        }
        else{
            memory[memLocation]= 0xFF-(*preg-memory[memLocation]);
            CF=1;
        }
        if(memory[memLocation]==0){
            ZF=1;
        }
        else{ZF=0;}
        if(memory[memLocation]>=0x80){
            SF=1;
        }
        else{
            SF=0;
        }
    }
}
template <typename regtype>
void sub_mem_word_register(unsigned short memLocation, regtype *preg) {
    unsigned short memWord = byte_to_word(memory[memLocation+1], memory[memLocation]);
    if(memWord>=0x8000&&*preg<0x8000){
        memWord -= *preg;
        if(memWord<0x8000){
            SF=0;
            OF=1;
            if(memWord==0){
                ZF=1;
            }
            else{
                ZF=0;
            }
        }
        else{
            SF=1;
            OF=0;
            ZF=0;
        }
        CF=0;
    }
    else if(memWord<0x8000&&*preg>=0x8000){
        memWord= 0xffff-(*preg-memWord);
        if(memWord>=0x8000){
            SF=1;
            OF=1;
            ZF=0;
        }
        else{
            SF=0;
            OF=0;
            if(memWord==0){
                ZF=1;
            }
            else{
                ZF=0;
            }
        }
        CF=1;
    }
    else{
        OF=0;
        if(memWord>=*preg){
            memWord -= *preg;
            CF=0;
        }
        else{
            memWord= 0xffff-(*preg-memWord);
            CF=1;
        }
        if(memWord==0){
            ZF=1;
        }
        else{ZF=0;}
        if(memWord>=0x8000){
            SF=1;
        }
        else{
            SF=0;
        }
    }
    unsigned char memWordLow = memWord & 0xff;
    unsigned char memWordHigh = (memWord >> 8);
    memory[memLocation+1] = memWordHigh;
    memory[memLocation] = memWordLow;
}
template <typename regtype>
void sub_register_byte_mem(regtype *preg, unsigned short memLocation) {
    if(*preg>=0x80&&memory[memLocation]<0x80){
        *preg -= memory[memLocation];
        if(*preg<0x80){
            SF=0;
            OF=1;
            if(*preg==0){
                ZF=1;
            }
            else{
                ZF=0;
            }
        }
        else{
            SF=1;
            OF=0;
            ZF=0;
        }
        CF=0;
    }
    else if(*preg<0x80&&memory[memLocation]>=0x80){
        *preg= 0xFF-(memory[memLocation]-*preg);
        if(*preg>=0x80){
            SF=1;
            OF=1;
            ZF=0;
        }
        else{
            SF=0;
            OF=0;
            if(*preg==0){
                ZF=1;
            }
            else{
                ZF=0;
            }
        }
        CF=1;
    }
    else{
        OF=0;
        if(*preg>=memory[memLocation]){
            *preg -= memory[memLocation];
            CF=0;
        }
        else{
            *preg= 0xFF-(memory[memLocation]-*preg);
            CF=1;
        }
        if(*preg==0){
            ZF=1;
        }
        else{ZF=0;}
        if(*preg>=0x80){
            SF=1;
        }
        else{
            SF=0;
        }
    }
}
template <typename regtype>
void sub_register_word_mem(regtype *preg, unsigned short memLocation) {
    unsigned short word = byte_to_word(memory[memLocation+1], memory[memLocation]);
    if(*preg>=0x8000&&word<0x8000){
        *preg -= word;
        if(*preg<0x8000){
            SF=0;
            OF=1;
            if(*preg==0){
                ZF=1;
            }
            else{
                ZF=0;
            }
        }
        else{
            SF=1;
            OF=0;
            ZF=0;
        }
        CF=0;
    }
    else if(*preg<0x8000&&word>=0x8000){
        *preg= 0xffff-(word-*preg);
        if(*preg>=0x8000){
            SF=1;
            OF=1;
            ZF=0;
        }
        else{
            SF=0;
            OF=0;
            if(*preg==0){
                ZF=1;
            }
            else{
                ZF=0;
            }
        }
        CF=1;
    }
    else{
        OF=0;
        if(*preg>=word){
            *preg -= word;
            CF=0;
        }
        else{
            *preg= 0xffff-(word-*preg);
            CF=1;
        }
        if(*preg==0){
            ZF=1;
        }
        else{ZF=0;}
        if(*preg>=0x8000){
            SF=1;
        }
        else{
            SF=0;
        }
    }
}
template <typename regtype>
void sub_bregister_bregister(regtype *preg1, regtype *preg2) {
    if(*preg1>=0x80&&*preg2<0x80){
        *preg1 -= *preg2;
        if(*preg1<0x80){
            SF=0;
            OF=1;
            if(*preg1==0){
                ZF=1;
            }
            else{
                ZF=0;
            }
        }
        else{
            SF=1;
            OF=0;
            ZF=0;
        }
        CF=0;
    }
    else if(*preg1<0x80&&*preg2>=0x80){
        *preg1= 0xFF-(*preg2-*preg1);
        if(*preg1>=0x80){
            SF=1;
            OF=1;
            ZF=0;
        }
        else{
            SF=0;
            OF=0;
            if(*preg1==0){
                ZF=1;
            }
            else{
                ZF=0;
            }
        }
        CF=1;
    }
    else{
        OF=0;
        if(*preg1>=*preg2){
            *preg1 -= *preg2;
            CF=0;
        }
        else{
            *preg1= 0xFF-(*preg2-*preg1);
            CF=1;
        }
        if(*preg1==0){
            ZF=1;
        }
        else{ZF=0;}
        if(*preg1>=0x80){
            SF=1;
        }
        else{
            SF=0;
        }
    }
}
template <typename regtype>
void sub_wregister_wregister(regtype *preg1, regtype *preg2) {
    if(*preg1>=0x8000&&*preg2<0x8000){
        *preg1 -= *preg2;
        if(*preg1<0x8000){
            SF=0;
            OF=1;
            if(*preg1==0){
                ZF=1;
            }
            else{
                ZF=0;
            }
        }
        else{
            SF=1;
            OF=0;
            ZF=0;
        }
        CF=0;
    }
    else if(*preg1<0x8000&&*preg2>=0x8000){
        *preg1= 0xffff-(*preg2-*preg1);
        if(*preg1>=0x8000){
            SF=1;
            OF=1;
            ZF=0;
        }
        else{
            SF=0;
            OF=0;
            if(*preg1==0){
                ZF=1;
            }
            else{
                ZF=0;
            }
        }
        CF=1;
    }
    else{
        OF=0;
        if(*preg1>=*preg2){
            *preg1 -= *preg2;
            CF=0;
        }
        else{
            *preg1= 0xffff-(*preg2-*preg1);
            CF=1;
        }
        if(*preg1==0){
            ZF=1;
        }
        else{ZF=0;}
        if(*preg1>=0x8000){
            SF=1;
        }
        else{
            SF=0;
        }
    }
}


//INC methods by operand types
template <typename regtype>
void inc_breg(regtype *preg){
    *preg++;
    *preg%0x100;
    if(*preg==0x80){
        OF=1;
    }
    else{
        OF=0;
    }
    if(*preg>=0x80){
        SF=1;
    }
    else{
        SF=0;
    }
    if(*preg==0){
        ZF=1;
    }
    else{
        ZF=0;
    }
}
template <typename regtype>
void inc_wreg(regtype *preg){
    unsigned int temp=*preg + 1;
    *preg=temp%0x10000;
    if(*preg==0x8000){
        OF=1;
    }
    else{
        OF=0;
    }
    if(*preg>=0x8000){
        SF=1;
    }
    else{
        SF=0;
    }
    if(*preg==0){
        ZF=1;
    }
    else{
        ZF=0;
    }
}
void inc_mem_byte(unsigned short memLocation){
    memory[memLocation]++;
    memory[memLocation]%0x100;
    if(memory[memLocation]==0x80){
        OF=1;
    }
    else{
        OF=0;
    }
    if(memory[memLocation]>=0x80){
        SF=1;
    }
    else{
        SF=0;
    }
    if(memory[memLocation]==0){
        ZF=1;
    }
    else{
        ZF=0;
    }
}
void inc_mem_word(unsigned short memLocation){
    unsigned word=byte_to_word(memory[memLocation+1],memory[memLocation]);
    unsigned int temp=word + 1;
    word=temp%0x10000;
    if(word==0x8000){
        OF=1;
    }
    else{
        OF=0;
    }
    if(word>=0x8000){
        SF=1;
    }
    else{
        SF=0;
    }
    if(word==0){
        ZF=1;
    }
    else{
        ZF=0;
    }
    unsigned char memWordLow = word & 0xff;
    unsigned char memWordHigh = (word >> 8);
    memory[memLocation+1] = memWordHigh;
    memory[memLocation] = memWordLow;

}

//DEC methods by operand types
template <typename regtype>
void dec_breg(regtype *preg){
    if(*preg!=0)
        *preg--;
    else{
        *preg=0xfe;
    }
    if(*preg==(0x80-1)){
        OF=1;
    }
    else{
        OF=0;
    }
    if(*preg>=0x80){
        SF=1;
    }
    else{
        SF=0;
    }
    if(*preg==0){
        ZF=1;
    }
    else{
        ZF=0;
    }
}
template <typename regtype>
void dec_wreg(regtype *preg){
    if(*preg!=0)
        *preg--;
    else{
        *preg=0xfffe;
    }
    if(*preg==(0x8000-1)){
        OF=1;
    }
    else{
        OF=0;
    }
    if(*preg>=0x8000){
        SF=1;
    }
    else{
        SF=0;
    }
    if(*preg==0){
        ZF=1;
    }
    else{
        ZF=0;
    }
}
void dec_mem_byte(unsigned short memLocation){
    if(memory[memLocation]!=0){
        memory[memLocation]--;
    }
    else{
        memory[memLocation]=0xfe;
    }
    if(memory[memLocation]==(0x80-1)){
        OF=1;
    }
    else{
        OF=0;
    }
    if(memory[memLocation]>=0x80){
        SF=1;
    }
    else{
        SF=0;
    }
    if(memory[memLocation]==0){
        ZF=1;
    }
    else{
        ZF=0;
    }
}
void dec_mem_word(unsigned short memLocation){
    unsigned short word=byte_to_word(memory[memLocation+1],memory[memLocation]);
    if(word!=0)
        word--;
    else{
        word=0xfffe;
    }
    if(word==(0x8000-1)){
        OF=1;
    }
    else{
        OF=0;
    }
    if(word>=0x8000){
        SF=1;
    }
    else{
        SF=0;
    }
    if(word==0){
        ZF=1;
    }
    else{
        ZF=0;
    }
    unsigned char memWordLow = word & 0xff;
    unsigned char memWordHigh = (word >> 8);
    memory[memLocation+1] = memWordHigh;
    memory[memLocation] = memWordLow;

}
//MUL methods by operand types
template <typename regtype>
void mul_breg(regtype *preg){
    unsigned short temp = *pal* *preg;
    *pax=temp;
    if(*pal!=0){
        OF=1;
        CF=1;
    }
    else{
        CF=0;
        OF=0;
    }
}
template <typename regtype>
void mul_wreg(regtype *preg){
    unsigned int memWord=*pax * *preg;
    unsigned char memWordLow = memWord & 0xffff;
    unsigned char memWordHigh= (memWord>>8);
    *pdx=memWordHigh;
    *pax=memWordLow;
    if(*pdx!=0){
        OF=1;
        CF=1;
    }
    else{
        CF=0;
        OF=0;
    }
}
void mul_mem_byte(unsigned short memLocation){
    unsigned short temp = *pal* memory[memLocation];
    *pax=temp;
    if(*pal!=0){
        OF=1;
        CF=1;
    }
    else{
        CF=0;
        OF=0;
    }
}
void mul_mem_word(unsigned short memLocation){
    unsigned int memWord = byte_to_word(memory[memLocation+1], memory[memLocation]);
    unsigned int othermem=*pax * memWord;
    unsigned char memWordLow = othermem & 0xffff;
    unsigned char memWordHigh= (othermem>>8);
    *pdx=memWordHigh;
    *pax=memWordLow;
    if(*pdx!=0){
        OF=1;
        CF=1;
    }
    else{
        CF=0;
        OF=0;
    }
}
//DIV methods by operand types
template <typename regtype>
int div_breg(regtype *preg){
    if(*preg==0){
        return 0;
    }
    unsigned short high = *pax%*preg;
    unsigned short low = *pax/ *preg;
    *pah=high;
    *pal=low;
    return 1;
}
template <typename regtype>
int div_wreg(regtype *preg){
    if(*preg==0){
        return 0;
    }
    unsigned int word = ( ( *pdx & 0xFFFF ) << 16 ) | ( *pax & 0xFFFF );
        unsigned short high = word%*preg;
        unsigned short low = word/ *preg;
        *pdx=high;
        *pax=low;
        return 1;
}
int div_mem_byte(unsigned short memLocation){
    if(memory[memLocation]==0){
        return 0;
    }
    unsigned short high = *pax%memory[memLocation];
    unsigned short low = *pax/ memory[memLocation];
    *pah=high;
    *pal=low;
    return 1;
}
int div_mem_word(unsigned short memLocation){
    unsigned int memWord = byte_to_word(memory[memLocation+1], memory[memLocation]);
    if(memWord==0){
        return 0;
    }
    unsigned int word = ( ( *pdx & 0xFFFF ) << 16 ) | ( *pax & 0xFFFF );
    unsigned short high = word%memWord;
    unsigned short low = word/ memWord;
    *pdx=high;
    *pax=low;
    return 1;
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
//NOT
template <typename regtype>
void not_reg(regtype *preg) {
    *preg = ~*preg;
}
void not_mem_byte(unsigned short memLocation) {
    memory[memLocation] = ~memory[memLocation];
}
void not_mem_word(unsigned short memLocation) {
    memory[memLocation] = ~memory[memLocation];
    memory[memLocation+1] = ~memory[memLocation+1];
}
//CMP methods by operand types
template <typename regtype>
void cmp_reg_byte(regtype *preg, unsigned char byte) {
    if(*preg>=0x80&&byte<0x80){
        unsigned short temp=*preg - byte;
        if(temp<0x80){
            SF=0;
            OF=1;
            if(temp==0){
                ZF=1;
            }
            else{
                ZF=0;
            }
        }
        else{
            SF=1;
            OF=0;
            ZF=0;
        }
        CF=0;
    }
    else if(*preg<0x80&&byte>=0x80){
        unsigned short temp= 0xFF-(byte-*preg);
        if(temp>=0x80){
            SF=1;
            OF=1;
            ZF=0;
        }
        else{
            SF=0;
            OF=0;
            if(temp==0){
                ZF=1;
            }
            else{
                ZF=0;
            }
        }
        CF=1;
    }
    else{
        OF=0;
        unsigned short temp;
        if(*preg>=byte){
            temp=*preg - byte;
            CF=0;
        }
        else{
            temp= 0xFF-(byte-*preg);
            CF=1;
        }
        if(temp==0){
            ZF=1;
        }
        else{ZF=0;}
        if(temp>=0x80){
            SF=1;
        }
        else{
            SF=0;
        }
    }
}
template <typename regtype>
void cmp_reg_word(regtype *preg, unsigned short word) {
    unsigned short temp;
    if(*preg>=0x8000&&word<0x8000){
        temp=*preg - word;
        if(temp<0x8000){
            SF=0;
            OF=1;
            if(temp==0){
                ZF=1;
            }
            else{
                ZF=0;
            }
        }
        else{
            SF=1;
            OF=0;
            ZF=0;
        }
        CF=0;
    }
    else if(*preg<0x8000&&word>=0x8000){
        temp= 0xffff-(word-*preg);
        if(temp>=0x8000){
            SF=1;
            OF=1;
            ZF=0;
        }
        else{
            SF=0;
            OF=0;
            if(temp==0){
                ZF=1;
            }
            else{
                ZF=0;
            }
        }
        CF=1;
    }
    else{
        OF=0;
        if(temp>=word){
            *preg -= word;
            CF=0;
        }
        else{
            temp= 0xffff-(word-*preg);
            CF=1;
        }
        if(temp==0){
            ZF=1;
        }
        else{ZF=0;}
        if(temp>=0x8000){
            SF=1;
        }
        else{
            SF=0;
        }
    }
}
void cmp_mem_byte(unsigned short memLocation, unsigned char byte) {
    unsigned short temp;
    if(memory[memLocation]>=0x80&&byte<0x80){
        temp= memory[memLocation] - byte;
        if(temp<0x80){
            SF=0;
            OF=1;
            if(temp==0){
                ZF=1;
            }
            else{
                ZF=0;
            }
        }
        else{
            SF=1;
            OF=0;
            ZF=0;
        }
        CF=0;
    }
    else if(memory[memLocation]<0x80&&byte>=0x80){
        temp= 0xFF-(byte-memory[memLocation]);
        if(temp>=0x80){
            SF=1;
            OF=1;
            ZF=0;
        }
        else{
            SF=0;
            OF=0;
            if(temp==0){
                ZF=1;
            }
            else{
                ZF=0;
            }
        }
        CF=1;
    }
    else{
        OF=0;
        if(memory[memLocation]>=byte){
            temp=memory[memLocation] - byte;
            CF=0;
        }
        else{
            temp= 0xFF-(byte-memory[memLocation]);
            CF=1;
        }
        if(temp==0){
            ZF=1;
        }
        else{ZF=0;}
        if(temp>=0x80){
            SF=1;
        }
        else{
            SF=0;
        }
    }
}
void cmp_mem_word(unsigned short memLocation, unsigned short word) {
    unsigned short temp;
    unsigned short memWord = byte_to_word(memory[memLocation+1], memory[memLocation]);
    if(memWord>=0x8000&&word<0x8000){
        temp=memWord - word;
        if(temp<0x8000){
            SF=0;
            OF=1;
            if(temp==0){
                ZF=1;
            }
            else{
                ZF=0;
            }
        }
        else{
            SF=1;
            OF=0;
            ZF=0;
        }
        CF=0;
    }
    else if(memWord<0x8000&&word>=0x8000){
        temp= 0xffff-(word-memWord);
        if(temp>=0x8000){
            SF=1;
            OF=1;
            ZF=0;
        }
        else{
            SF=0;
            OF=0;
            if(temp==0){
                ZF=1;
            }
            else{
                ZF=0;
            }
        }
        CF=1;
    }
    else{
        OF=0;
        if(memWord>=word){
            temp= memWord - word;
            CF=0;
        }
        else{
            temp= 0xffff-(word-memWord);
            CF=1;
        }
        if(temp==0){
            ZF=1;
        }
        else{ZF=0;}
        if(temp>=0x8000){
            SF=1;
        }
        else{
            SF=0;
        }
    }
}
template <typename regtype>
void cmp_mem_byte_register(unsigned short memLocation, regtype *preg) {
    unsigned short temp;
    if(memory[memLocation]>=0x80&&*preg<0x80){
        temp=memory[memLocation] - *preg;
        if(temp<0x80){
            SF=0;
            OF=1;
            if(temp==0){
                ZF=1;
            }
            else{
                ZF=0;
            }
        }
        else{
            SF=1;
            OF=0;
            ZF=0;
        }
        CF=0;
    }
    else if(memory[memLocation]<0x80&&*preg>=0x80){
        temp= 0xFF-(*preg-memory[memLocation]);
        if(temp>=0x80){
            SF=1;
            OF=1;
            ZF=0;
        }
        else{
            SF=0;
            OF=0;
            if(temp==0){
                ZF=1;
            }
            else{
                ZF=0;
            }
        }
        CF=1;
    }
    else{
        OF=0;
        if(memory[memLocation]>=*preg){
            temp=memory[memLocation] - *preg;
            CF=0;
        }
        else{
            temp= 0xFF-(*preg-memory[memLocation]);
            CF=1;
        }
        if(temp==0){
            ZF=1;
        }
        else{ZF=0;}
        if(temp>=0x80){
            SF=1;
        }
        else{
            SF=0;
        }
    }
}
template <typename regtype>
void cmp_mem_word_register(unsigned short memLocation, regtype *preg) {
    unsigned short temp;
    unsigned short memWord = byte_to_word(memory[memLocation+1], memory[memLocation]);
    if(memWord>=0x8000&&*preg<0x8000){
        temp=memWord - *preg;
        if(temp<0x8000){
            SF=0;
            OF=1;
            if(temp==0){
                ZF=1;
            }
            else{
                ZF=0;
            }
        }
        else{
            SF=1;
            OF=0;
            ZF=0;
        }
        CF=0;
    }
    else if(memWord<0x8000&&*preg>=0x8000){
        temp= 0xffff-(*preg-memWord);
        if(temp>=0x8000){
            SF=1;
            OF=1;
            ZF=0;
        }
        else{
            SF=0;
            OF=0;
            if(temp==0){
                ZF=1;
            }
            else{
                ZF=0;
            }
        }
        CF=1;
    }
    else{
        OF=0;
        if(memWord>=*preg){
            temp=memWord - *preg;
            CF=0;
        }
        else{
            temp= 0xffff-(*preg-memWord);
            CF=1;
        }
        if(temp==0){
            ZF=1;
        }
        else{ZF=0;}
        if(temp>=0x8000){
            SF=1;
        }
        else{
            SF=0;
        }
    }
}
template <typename regtype>
void cmp_register_byte_mem(regtype *preg, unsigned short memLocation) {
    unsigned short temp;
    if(*preg>=0x80&&memory[memLocation]<0x80){
        temp=*preg - memory[memLocation];
        if(temp<0x80){
            SF=0;
            OF=1;
            if(temp==0){
                ZF=1;
            }
            else{
                ZF=0;
            }
        }
        else{
            SF=1;
            OF=0;
            ZF=0;
        }
        CF=0;
    }
    else if(*preg<0x80&&memory[memLocation]>=0x80){
        temp= 0xFF-(memory[memLocation]-*preg);
        if(temp>=0x80){
            SF=1;
            OF=1;
            ZF=0;
        }
        else{
            SF=0;
            OF=0;
            if(temp==0){
                ZF=1;
            }
            else{
                ZF=0;
            }
        }
        CF=1;
    }
    else{
        OF=0;
        if(*preg>=memory[memLocation]){
            temp=*preg - memory[memLocation];
            CF=0;
        }
        else{
            temp= 0xFF-(memory[memLocation]-*preg);
            CF=1;
        }
        if(temp==0){
            ZF=1;
        }
        else{ZF=0;}
        if(temp>=0x80){
            SF=1;
        }
        else{
            SF=0;
        }
    }
}
template <typename regtype>
void cmp_register_word_mem(regtype *preg, unsigned short memLocation) {
    unsigned short temp;
    unsigned short word = byte_to_word(memory[memLocation+1], memory[memLocation]);
    if(*preg>=0x8000&&word<0x8000){
        temp=*preg - word;
        if(temp<0x8000){
            SF=0;
            OF=1;
            if(temp==0){
                ZF=1;
            }
            else{
                ZF=0;
            }
        }
        else{
            SF=1;
            OF=0;
            ZF=0;
        }
        CF=0;
    }
    else if(*preg<0x8000&&word>=0x8000){
        temp= 0xffff-(word-*preg);
        if(temp>=0x8000){
            SF=1;
            OF=1;
            ZF=0;
        }
        else{
            SF=0;
            OF=0;
            if(temp==0){
                ZF=1;
            }
            else{
                ZF=0;
            }
        }
        CF=1;
    }
    else{
        OF=0;
        if(*preg>=word){
            temp=*preg - word;
            CF=0;
        }
        else{
            temp= 0xffff-(word-*preg);
            CF=1;
        }
        if(temp==0){
            ZF=1;
        }
        else{ZF=0;}
        if(temp>=0x8000){
            SF=1;
        }
        else{
            SF=0;
        }
    }
}
template <typename regtype>
void cmp_bregister_bregister(regtype *preg1, regtype *preg2) {
    unsigned short temp;
    if(*preg1>=0x80&&*preg2<0x80){
        temp=*preg1 - *preg2;
        if(temp<0x80){
            SF=0;
            OF=1;
            if(temp==0){
                ZF=1;
            }
            else{
                ZF=0;
            }
        }
        else{
            SF=1;
            OF=0;
            ZF=0;
        }
        CF=0;
    }
    else if(*preg1<0x80&&*preg2>=0x80){
        temp= 0xFF-(*preg2-*preg1);
        if(*preg1>=0x80){
            SF=1;
            OF=1;
            ZF=0;
        }
        else{
            SF=0;
            OF=0;
            if(temp==0){
                ZF=1;
            }
            else{
                ZF=0;
            }
        }
        CF=1;
    }
    else{
        OF=0;
        if(*preg1>=*preg2){
            temp=*preg1 - *preg2;
            CF=0;
        }
        else{
            temp= 0xFF-(*preg2-*preg1);
            CF=1;
        }
        if(temp==0){
            ZF=1;
        }
        else{ZF=0;}
        if(temp>=0x80){
            SF=1;
        }
        else{
            SF=0;
        }
    }
}
template <typename regtype>
void cmp_wregister_wregister(regtype *preg1, regtype *preg2) {
    unsigned short temp;
    if(*preg1>=0x8000&&*preg2<0x8000){
        temp=*preg1 - *preg2;
        if(temp<0x8000){
            SF=0;
            OF=1;
            if(temp==0){
                ZF=1;
            }
            else{
                ZF=0;
            }
        }
        else{
            SF=1;
            OF=0;
            ZF=0;
        }
        CF=0;
    }
    else if(*preg1<0x8000&&*preg2>=0x8000){
        temp= 0xffff-(*preg2-*preg1);
        if(temp>=0x8000){
            SF=1;
            OF=1;
            ZF=0;
        }
        else{
            SF=0;
            OF=0;
            if(temp==0){
                ZF=1;
            }
            else{
                ZF=0;
            }
        }
        CF=1;
    }
    else{
        OF=0;
        if(*preg1>=*preg2){
            temp= *preg1 -=*preg2;
            CF=0;
        }
        else{
            temp= 0xffff-(*preg2-*preg1);
            CF=1;
        }
        if(temp==0){
            ZF=1;
        }
        else{ZF=0;}
        if(temp>=0x8000){
            SF=1;
        }
        else{
            SF=0;
        }
    }
}

bool jz(){
    return ZF;
}
bool jnz(){
    return !ZF;
}
bool je(){
    return ZF;
}
bool jne(){
    return !ZF;
}
bool ja(){
    return !ZF&&!CF;
}
bool jae(){
    return !CF;
}
bool jb(){
    return CF;
}
bool jbe(){
    ZF&&CF;
}
bool jnae(){
    return CF;
}
bool jnb(){
    return !CF;
}
bool jnbe(){
    return !CF&&!ZF;
}
bool jc(){
    return CF;
}
bool jnc(){
    return !CF;
}
template<typename regtype>
void shr_byte_reg_var(regtype *preg, unsigned short var){
    if(var!=1){
        for(int i=0;i<var;i++){
            CF=*preg%2;
            *preg>>1;
        }
    }
    else{
        if(*preg>=0x80){
            OF=1;
        }
        else{
            OF=0;
        }
        CF=*preg%2;
        *preg>>1;
    }
    if(*preg==0){
        ZF=1;
    }
    else{
        ZF=0;
    }
    SF=0;

}

template<typename regtype>
void shr_word_reg_var(regtype *preg, unsigned short var){
    if(var!=1){
        for(int i=0;i<var;i++){
            CF=*preg%2;
            *preg>>1;
        }
    }
    else{
        if(*preg>=0x8000){
            OF=1;
        }
        else{
            OF=0;
        }
        CF=*preg%2;
        *preg>>1;
    }
    if(*preg==0){
        ZF=1;
    }
    else{
        ZF=0;
    }
    SF=0;

}
template<typename regtype>
void shr_word_reg_cl(regtype *preg){
    if(*pcl!=1){
        for(int i=0;i<*pcl;i++){
            CF=*preg%2;
            *preg>>1;
        }
    }
    else{
        if(*preg>=0x8000){
            OF=1;
        }
        else{
            OF=0;
        }
        CF=*preg%2;
        *preg>>1;
    }
    if(*preg==0){
        ZF=1;
    }
    else{
        ZF=0;
    }
    SF=0;

}

template<typename regtype>
void shr_byte_reg_cl(regtype *preg){
    if(*pcl!=1){
        for(int i=0;i<*pcl;i++){
            CF=*preg%2;
            *preg>>1;
        }
    }
    else{
        if(*preg>=0x80){
            OF=1;
        }
        else{
            OF=0;
        }
        CF=*preg%2;
        *preg>>1;
    }
    if(*preg==0){
        ZF=1;
    }
    else{
        ZF=0;
    }
    SF=0;
}
void shr_byte_mem_var(unsigned short memLocation, unsigned short var){
    if(var!=1){
        for(int i=0;i<var;i++){
            CF=memory[memLocation]%2;
            memory[memLocation]>>1;
        }
    }
    else{
        if(memory[memLocation]>=0x80){
            OF=1;
        }
        else{
            OF=0;
        }
        CF=memory[memLocation]%2;
        memory[memLocation]>>1;
    }
    if(memory[memLocation]==0){
        ZF=1;
    }
    else{
        ZF=0;
    }
    SF=0;

}

void shr_word_mem_var(unsigned short memLocation, unsigned short var){
    unsigned short memWord = byte_to_word(memory[memLocation+1],memory[memLocation]);
    if(var!=1){
        for(int i=0;i<var;i++){
            CF=memWord%2;
            memWord>>1;
        }
    }
    else{
        if(memWord>=0x8000){
            OF=1;
        }
        else{
            OF=0;
        }
        CF=memWord%2;
        memWord>>1;
    }
    if(memWord==0){
        ZF=1;
    }
    else{
        ZF=0;
    }
    SF=0;
    unsigned char memWordLow = memWord & 0xff;
    unsigned char memWordHigh = (memWord >> 8);
    memory[memLocation+1] = memWordHigh;
    memory[memLocation] = memWordLow;
}
void shr_word_mem_cl(unsigned short memLocation){
    unsigned short memWord = byte_to_word(memory[memLocation+1],memory[memLocation]);
    if(*pcl!=1){
        for(int i=0;i<*pcl;i++){
            CF=memWord%2;
            memWord>>1;
        }
    }
    else{
        if(memWord>=0x8000){
            OF=1;
        }
        else{
            OF=0;
        }
        CF=memWord%2;
        memWord>>1;
    }
    if(memWord==0){
        ZF=1;
    }
    else{
        ZF=0;
    }
    SF=0;
    unsigned char memWordLow = memWord & 0xff;
    unsigned char memWordHigh = (memWord >> 8);
    memory[memLocation+1] = memWordHigh;
    memory[memLocation] = memWordLow;
}

void shr_byte_mem_cl(unsigned short memLocation){
    if(*pcl!=1){
        for(int i=0;i<*pcl;i++){
            CF=memory[memLocation]%2;
            memory[memLocation]>>1;
        }
    }
    else{
        if(memory[memLocation]>=0x80){
            OF=1;
        }
        else{
            OF=0;
        }
        CF=memory[memLocation]%2;
        memory[memLocation]>>1;
    }
    if(memory[memLocation]==0){
        ZF=1;
    }
    else{
        ZF=0;
    }
    SF=0;
}

template<typename regtype>
void shl_byte_reg_var(regtype *preg, unsigned short var){
    if(var!=1){
        for(int i=0;i<var;i++){
            CF=*preg/0x80;
            *preg<<1;
            *preg=*preg%0x100;
        }
    }
    else{
        unsigned int temp=*preg;
        CF=*preg/0x80;
        *preg<<1;
        *preg=*preg%0x100;
        if((*preg/0x80-temp/0x80)!=0){
            OF=1;
        }
        else{OF=0;}
    }
    if(*preg==0){
        ZF=1;
    }
    else{
        ZF=0;
    }
    if(*preg/0x80==1){
        SF=1;
    }
    else{
        SF=0;
    }
}

template<typename regtype>
void shl_word_reg_var(regtype *preg, unsigned short var){
    if(var!=1){
        for(int i=0;i<var;i++){
            CF=*preg/0x8000;
            *preg<<1;
        }
    }
    else{
        unsigned int temp=*preg;
        CF=*preg/0x8000;
        *preg<<1;
        if((*preg/0x8000-temp/0x8000)!=0){
            OF=1;
        }
        else{OF=0;}
    }
    if(*preg==0){
        ZF=1;
    }
    else{
        ZF=0;
    }
    if(*preg/0x8000==1){
        SF=1;
    }
    else{
        SF=0;
    }

}
template<typename regtype>
void shl_word_reg_cl(regtype *preg){
    if(*pcl!=1){
        for(int i=0;i<*pcl;i++){
            CF=*preg/0x8000;
            *preg<<1;
        }
    }
    else{
        unsigned int temp=*preg;
        CF=*preg/0x8000;
        *preg<<1;
        if((*preg/0x8000-temp/0x8000)!=0){
            OF=1;
        }
        else{OF=0;}
    }
    if(*preg==0){
        ZF=1;
    }
    else{
        ZF=0;
    }
    if(*preg/0x8000==1){
        SF=1;
    }
    else{
        SF=0;
    }

}

template<typename regtype>
void shl_byte_reg_cl(regtype *preg){
    if(*pcl!=1){
        for(int i=0;i<*pcl;i++){
            CF=*preg/0x80;
            *preg<<1;
            *preg=*preg%0x100;
        }
    }
    else{
        unsigned int temp=*preg;
        CF=*preg/0x80;
        *preg<<1;
        *preg=*preg%0x100;
        if((*preg/0x80-temp/0x80)!=0){
            OF=1;
        }
        else{OF=0;}
    }
    if(*preg==0){
        ZF=1;
    }
    else{
        ZF=0;
    }
    if(*preg/0x80==1){
        SF=1;
    }
    else{
        SF=0;
    }
}
void shl_byte_mem_var(unsigned short memLocation, unsigned short var){
    if(var!=1){
        for(int i=0;i<var;i++){
            CF=memory[memLocation]/0x80;
            memory[memLocation]<<1;
            memory[memLocation]= memory[memLocation]%0x100;
        }
    }
    else{
        unsigned int temp=memory[memLocation];
        CF=memory[memLocation]/0x80;
        memory[memLocation]<<1;
        memory[memLocation]= memory[memLocation]%0x100;
        if((memory[memLocation]/0x80-temp/0x80)!=0){
            OF=1;
        }
        else{OF=0;}
    }
    if(memory[memLocation]==0){
        ZF=1;
    }
    else{
        ZF=0;
    }
    if(memory[memLocation]/0x80==1){
        SF=1;
    }
    else{
        SF=0;
    }

}

void shl_word_mem_var(unsigned short memLocation, unsigned short var){
    unsigned short memWord = byte_to_word(memory[memLocation+1],memory[memLocation]);
    if(var!=1){
        for(int i=0;i<var;i++){
            CF=memWord/0x8000;
            memWord<<1;
        }
    }
    else{
        unsigned int temp=memWord;
        CF=memWord/0x8000;
        memWord<<1;
        if((memWord/0x8000-temp/0x8000)!=0){
            OF=1;
        }
        else{OF=0;}
    }
    if(memWord==0){
        ZF=1;
    }
    else{
        ZF=0;
    }
    if(memWord/0x8000==1){
        SF=1;
    }
    else{
        SF=0;
    }
    unsigned char memWordLow = memWord & 0xff;
    unsigned char memWordHigh = (memWord >> 8);
    memory[memLocation+1] = memWordHigh;
    memory[memLocation] = memWordLow;
}
void shl_word_mem_cl(unsigned short memLocation){
    unsigned short memWord = byte_to_word(memory[memLocation+1],memory[memLocation]);
    if(*pcl!=1){
        for(int i=0;i<*pcl;i++){
            CF=memWord/0x8000;
            memWord<<1;
        }
    }
    else{
        unsigned int temp=memWord;
        CF=memWord/0x8000;
        memWord<<1;
        if((memWord/0x8000-temp/0x8000)!=0){
            OF=1;
        }
        else{OF=0;}
    }
    if(memWord==0){
        ZF=1;
    }
    else{
        ZF=0;
    }
    if(memWord/0x8000==1){
        SF=1;
    }
    else{
        SF=0;
    }     unsigned char memWordLow = memWord & 0xff;
    unsigned char memWordHigh = (memWord >> 8);
    memory[memLocation+1] = memWordHigh;
    memory[memLocation] = memWordLow;
}

void shl_byte_mem_cl(unsigned short memLocation){
    if(*pcl!=1){
        for(int i=0;i<*pcl;i++){
            CF=memory[memLocation]/0x80;
            memory[memLocation]<<1;
            memory[memLocation]= memory[memLocation]%0x100;
        }
    }
    else{
        unsigned int temp=memory[memLocation];
        CF=memory[memLocation]/0x80;
        memory[memLocation]<<1;
        memory[memLocation]= memory[memLocation]%0x100;
        if((memory[memLocation]/0x80-temp/0x80)!=0){
            OF=1;
        }
        else{OF=0;}
    }
    if(memory[memLocation]==0){
        ZF=1;
    }
    else{
        ZF=0;
    }
    if(memory[memLocation]/0x80==1){
        SF=1;
    }
    else{
        SF=0;
    }
}
template<typename regtype>
void rcr_byte_reg_var(regtype *preg, unsigned short var){
    if(var!=1){
        for(int i=0;i<var;i++){
            unsigned int tempCF=CF;
            CF=*preg%2;
            *preg>>1;
            *preg+=tempCF*0x80;
        }
    }
    else{
        if(*preg>=0x80){
            OF=1;
        }
        else{
            OF=0;
        }
        unsigned int tempCF=CF;
        CF=*preg%2;
        *preg>>1;
        *preg+=tempCF*0x80;
    }
}

template<typename regtype>
void rcr_word_reg_var(regtype *preg, unsigned short var){
    if(var!=1){
        for(int i=0;i<var;i++){
            unsigned int tempCF=CF;
            CF=*preg%2;
            *preg>>1;
            *preg+=tempCF*0x8000;
        }
    }
    else{
        if(*preg>=0x8000){
            OF=1;
        }
        else{
            OF=0;
        }
        unsigned int tempCF=CF;
        CF=*preg%2;
        *preg>>1;
        *preg+=tempCF*0x8000;
    }


}
template<typename regtype>
void rcr_word_reg_cl(regtype *preg){
    if(*pcl!=1){
        for(int i=0;i<*pcl;i++){
            unsigned int tempCF=CF;
            CF=*preg%2;
            *preg>>1;
            *preg+=tempCF*0x8000;
        }
    }
    else{
        if(*preg>=0x8000){
            OF=1;
        }
        else{
            OF=0;
        }
        unsigned int tempCF=CF;
        CF=*preg%2;
        *preg>>1;
        *preg+=tempCF*0x80;
    }
}

template<typename regtype>
void rcr_byte_reg_cl(regtype *preg){
    if(*pcl!=1){
        for(int i=0;i<*pcl;i++){
            unsigned int tempCF=CF;
            CF=*preg%2;
            *preg>>1;
            *preg+=tempCF*0x80;
        }
    }
    else{
        if(*preg>=0x80){
            OF=1;
        }
        else{
            OF=0;
        }
        unsigned int tempCF=CF;
        CF=*preg%2;
        *preg>>1;
        *preg+=tempCF*0x80;
    }
}
void rcr_byte_mem_var(unsigned short memLocation, unsigned short var){
    if(var!=1){
        for(int i=0;i<var;i++){
            unsigned int tempCF=CF;
            CF=memory[memLocation]%2;
            memory[memLocation]>>1;
            memory[memLocation]+=tempCF*0x80;
        }
    }
    else{
        if(memory[memLocation]>=0x80){
            OF=1;
        }
        else{
            OF=0;
        }
        unsigned int tempCF=CF;
        CF=memory[memLocation]%2;
        memory[memLocation]>>1;
        memory[memLocation]+=tempCF*0x80;
    }
}

void rcr_word_mem_var(unsigned short memLocation, unsigned short var){
    unsigned short memWord = byte_to_word(memory[memLocation+1],memory[memLocation]);
    if(var!=1){
        for(int i=0;i<var;i++){
            unsigned int tempCF=CF;
            CF=memWord%2;
            memWord>>1;
            memWord+=tempCF*0x8000;
        }
    }
    else{
        if(memWord>=0x8000){
            OF=1;
        }
        else{
            OF=0;
        }
        unsigned int tempCF=CF;
        CF=memWord%2;
        memWord>>1;
        memWord+=tempCF*0x8000;;
    }
    unsigned char memWordLow = memWord & 0xff;
    unsigned char memWordHigh = (memWord >> 8);
    memory[memLocation+1] = memWordHigh;
    memory[memLocation] = memWordLow;
}
void rcr_word_mem_cl(unsigned short memLocation){
    unsigned short memWord = byte_to_word(memory[memLocation+1],memory[memLocation]);
    if(*pcl!=1){
        for(int i=0;i<*pcl;i++){
            unsigned int tempCF=CF;
            CF=memWord%2;
            memWord>>1;
            memWord+=tempCF*0x8000;
        }
    }
    else{
        if(memWord>=0x8000){
            OF=1;
        }
        else{
            OF=0;
        }
        unsigned int tempCF=CF;
        CF=memWord%2;
        memWord>>1;
        memWord+=tempCF*0x8000;
    }
    unsigned char memWordLow = memWord & 0xff;
    unsigned char memWordHigh = (memWord >> 8);
    memory[memLocation+1] = memWordHigh;
    memory[memLocation] = memWordLow;
}

void rcr_byte_mem_cl(unsigned short memLocation){
    if(*pcl!=1){
        for(int i=0;i<*pcl;i++){
            unsigned int tempCF=CF;
            CF=memory[memLocation]%2;
            memory[memLocation]>>1;
            memory[memLocation]+=tempCF*0x80;
        }
    }
    else{
        if(memory[memLocation]>=0x80){
            OF=1;
        }
        else{
            OF=0;
        }
        unsigned int tempCF=CF;
        CF=memory[memLocation]%2;
        memory[memLocation]>>1;
        memory[memLocation]+=tempCF*0x80;
    }
}

template<typename regtype>
void rcl_byte_reg_var(regtype *preg, unsigned short var){
    if(var!=1){
        for(int i=0;i<var;i++){
            unsigned int tempCF=*preg%2;
            CF=*preg/0x80;
            *preg<<1;
            *preg=*preg%0x100;
            *preg+=tempCF;
        }
    }
    else{
        unsigned int temp=*preg;
        unsigned int tempCF=*preg%2;
        CF=*preg/0x80;
        *preg<<1;
        *preg=*preg%0x100;
        *preg+=tempCF;
        if((*preg/0x80-temp/0x80)!=0){
            OF=1;
        }
        else{OF=0;}
    }
}

template<typename regtype>
void rcl_word_reg_var(regtype *preg, unsigned short var){
    if(var!=1){
        for(int i=0;i<var;i++){
            unsigned int tempCF=*preg%2;
            CF=*preg/0x8000;
            *preg<<1;
            *preg+=tempCF;
        }
    }
    else{
        unsigned int temp=*preg;
        unsigned int tempCF=*preg%2;
        CF=*preg/0x8000;
        *preg<<1;
        *preg+=tempCF;
        if((*preg/0x8000-temp/0x8000)!=0){
            OF=1;
        }
        else{OF=0;}
    }
}
template<typename regtype>
void rcl_word_reg_cl(regtype *preg){
    if(*pcl!=1){
        for(int i=0;i<*pcl;i++){
            unsigned int tempCF=*preg%2;
            CF=*preg/0x8000;
            *preg<<1;
            *preg+=tempCF;
        }
    }
    else{
        unsigned int temp=*preg;
        unsigned int tempCF=*preg%2;
        CF=*preg/0x8000;
        *preg<<1;
        *preg+=tempCF;
        if((*preg/0x8000-temp/0x8000)!=0){
            OF=1;
        }
        else{OF=0;}
    }
}

template<typename regtype>
void rcl_byte_reg_cl(regtype *preg){
    if(*pcl!=1){
        for(int i=0;i<*pcl;i++){
            unsigned int tempCF=*preg%2;
            CF=*preg/0x80;
            *preg<<1;
            *preg=*preg%0x100;
            *preg+=tempCF;
        }
    }
    else{
        unsigned int temp=*preg;
        unsigned int tempCF=*preg%2;
        CF=*preg/0x80;
        *preg<<1;
        *preg=*preg%0x100;
        *preg+=tempCF;
        if((*preg/0x80-temp/0x80)!=0){
            OF=1;
        }
        else{OF=0;}
    }
}
void rcl_byte_mem_var(unsigned short memLocation, unsigned short var){
    if(var!=1){
        for(int i=0;i<var;i++){
            unsigned int tempCF=memory[memLocation]%2;
            CF=memory[memLocation]/0x80;
            memory[memLocation]<<1;
            memory[memLocation]= memory[memLocation]%0x100;
            memory[memLocation]+=tempCF;
        }
    }
    else{
        unsigned int temp=memory[memLocation];
        unsigned int tempCF=memory[memLocation]%2;
        CF=memory[memLocation]/0x80;
        memory[memLocation]<<1;
        memory[memLocation]= memory[memLocation]%0x100;
        memory[memLocation]+=tempCF;
        if((memory[memLocation]/0x80-temp/0x80)!=0){
            OF=1;
        }
        else{OF=0;}
    }
}

void rcl_word_mem_var(unsigned short memLocation, unsigned short var){
    unsigned short memWord = byte_to_word(memory[memLocation+1],memory[memLocation]);
    if(var!=1){
        for(int i=0;i<var;i++){
            unsigned int tempCF=memWord%2;
            CF=memWord/0x8000;
            memWord<<1;
            memWord+=tempCF;
        }
    }
    else{
        unsigned int temp=memWord;
        unsigned int tempCF=memWord%2;
        CF=memWord/0x8000;
        memWord<<1;
        memWord+=tempCF;
        if((memWord/0x8000-temp/0x8000)!=0){
            OF=1;
        }
        else{OF=0;}
    }
    unsigned char memWordLow = memWord & 0xff;
    unsigned char memWordHigh = (memWord >> 8);
    memory[memLocation+1] = memWordHigh;
    memory[memLocation] = memWordLow;
}
void rcl_word_mem_cl(unsigned short memLocation){
    unsigned short memWord = byte_to_word(memory[memLocation+1],memory[memLocation]);
    if(*pcl!=1){
        for(int i=0;i<*pcl;i++){
            unsigned int tempCF=memWord%2;
            CF=memWord/0x8000;
            memWord<<1;
            memWord+=tempCF;
        }
    }
    else{
        unsigned int temp=memWord;
        unsigned int tempCF=memWord%2;
        CF=memWord/0x8000;
        memWord<<1;
        memWord+=tempCF;
        if((memWord/0x8000-temp/0x8000)!=0){
            OF=1;
        }
        else{OF=0;}
    }
    if(memWord==0){
        ZF=1;
    }
    else{
        ZF=0;
    }
    if(memWord/0x8000==1){
        SF=1;
    }
    else{
        SF=0;
    }     unsigned char memWordLow = memWord & 0xff;
    unsigned char memWordHigh = (memWord >> 8);
    memory[memLocation+1] = memWordHigh;
    memory[memLocation] = memWordLow;
}

void rcl_byte_mem_cl(unsigned short memLocation){
    if(*pcl!=1){
        for(int i=0;i<*pcl;i++){
            unsigned int tempCF=memory[memLocation]%2;
            CF=memory[memLocation]/0x80;
            memory[memLocation]<<1;
            memory[memLocation]= memory[memLocation]%0x100;
            memory[memLocation]+=tempCF;
        }
    }
    else{
        unsigned int temp=memory[memLocation];
        unsigned int tempCF=memory[memLocation]%2;
        CF=memory[memLocation]/0x80;
        memory[memLocation]<<1;
        memory[memLocation]= memory[memLocation]%0x100;
        memory[memLocation]+=tempCF;
        if((memory[memLocation]/0x80-temp/0x80)!=0){
            OF=1;
        }
        else{OF=0;}
    }
}
