#include "to8converter.h"
#include "to32converter.h"



int main() {
    vector<uint32_t> v32 = {0x1BC03, 0x16A41, 0x1BC1B}; //0xd800 to trigger SURROGATE_HALF
    vector<uint8_t> res = to_utf8(v32);
    vector<uint32_t> back_v32 = to_utf32(res);
    for(int i = 0; i < v32.size(); i++){
        assert(v32[i] == back_v32[i]);
    }
    printf("Success!");
    return 0;
}