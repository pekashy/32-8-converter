#pragma once
#include "to8converter.h"

using std::vector;

vector<uint8_t> to_utf8(const vector<uint32_t> &x){
    vector<uint32_t>::const_iterator it = x.begin();
    vector<uint8_t> result;
    for(; it != x.end(); it++){
        uint32_t symb32= *it;
        char bitpart[6];
        size_t bitsize = convert_to_8(bitpart, symb32);
        for(int i = 0; i < bitsize; i++){
            result.push_back(bitpart[i]);
        }
    }
    return result;
}


size_t convert_to_8(char* bitpart, uint32_t symb32){
    if (symb32 <= 0x7F) {
        bitpart[0] = symb32;
        return 1;
    }
    if (symb32 <= 0x7FF) {
        bitpart[0] = 0xC0 | (symb32 >> 6);
        bitpart[1] = 0x80 | (symb32 & 0x3F);
        return 2;
    }
    if (symb32 <= 0xFFFF) {
        bitpart[0] = 0xE0 | (symb32 >> 12);
        bitpart[1] = 0x80 | ((symb32 >> 6) & 0x3F);
        bitpart[2] = 0x80 | (symb32 & 0x3F);
        return 3;
    }
    if (symb32 <= 0x1FFFFF) {
        bitpart[0] = 0xF0 | (symb32 >> 18);
        bitpart[1] = 0x80 | ((symb32 >> 12) & 0x3F);
        bitpart[2] = 0x80 | ((symb32 >> 6) & 0x3F);
        bitpart[3] = 0x80 | (symb32 & 0x3F);
        return 4;
    }
    if (symb32 <= 0x3FFFFFF) {
        bitpart[0] = 0xF0 | (symb32 >> 24);
        bitpart[1] = 0x80 | ((symb32 >> 18) & 0x3F);
        bitpart[2] = 0x80 | ((symb32 >> 12) & 0x3F);
        bitpart[3] = 0x80 | ((symb32 >> 6) & 0x3F);
        bitpart[4] = 0x80 | (symb32 & 0x3F);
        return 5;
    }
    if (symb32 <= 0x7FFFFFFF) {
        bitpart[0] = 0xF0 | (symb32 >> 30);
        bitpart[1] = 0x80 | ((symb32 >> 24) & 0x3F);
        bitpart[2] = 0x80 | ((symb32 >> 18) & 0x3F);
        bitpart[3] = 0x80 | ((symb32 >> 12) & 0x3F);
        bitpart[4] = 0x80 | ((symb32 >> 6) & 0x3F);
        bitpart[5] = 0x80 | (symb32 & 0x3F);
        return 6;
    }
    return 0;
}

