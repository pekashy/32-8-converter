#pragma once
#include <cstdint>
#include <vector>
#include <iostream>
#include <algorithm>

using std::vector;
using std::cerr;

enum CODE_32{
    SUCCESS,
    WRONG_BYTE_IN_UTF_8,
    SURROGATE_HALF,
    INVALID_NUMBER_OF_BYTES,
    BAD_BYTE_VECTOR,
    UNKNOWN_ERROR
};

vector<uint32_t> to_utf32(const vector<uint8_t> &x);

class StrUtf8Iter {
public:
    StrUtf8Iter(const std::vector<uint8_t>& str_utf8);
    bool hasNext() const;
    uint32_t getNext();
private:
    const vector<uint8_t>& utf8_array;
    vector<uint8_t>::const_iterator curr_pointer;
    int curr_length;
    CODE_32 status;
    void estimate_size();
    void convert_to_32(const vector<uint8_t>& utf8_input, uint32_t& c);
    vector<uint32_t> to_utf32(const vector<uint8_t> &x);
};