#include "to32converter.h"

void print_error(CODE_32 code) {
    switch (code) {
        case WRONG_BYTE_IN_UTF_8 :
            cerr << "Got a wrong byte in UTF-8 sequence\n";
            break;
        case INVALID_NUMBER_OF_BYTES :
            cerr << "Got invalid number of bytes in UTF-8 sequence\n";
            break;
        case BAD_BYTE_VECTOR :
            cerr << "Got bad byte vector\n";
            break;
        case UNKNOWN_ERROR :
            cerr << "Got some unknown error\n";
            break;
        case SURROGATE_HALF :
            cerr << "Got number from the surrogate half \n";
            break;
    }
}

std::pair<CODE_32, vector<uint32_t>> to_utf32(const vector<uint8_t> &x) {
    vector<uint32_t> result;
    uint32_t res;
    StrUtf8Iter it(x);

    while (it.hasNext()) {
        res = it.getNext();
        if (it.getStatus() == SUCCESS) {
            result.push_back(res);
        } else {
            break;
        }
    }
    return std::make_pair(it.getStatus(), result);
}

StrUtf8Iter::StrUtf8Iter(const std::vector<uint8_t> &str_utf8) : utf8_array(str_utf8) {
    curr_pointer = str_utf8.begin();
    status = SUCCESS;
    curr_length = 0;
}

bool StrUtf8Iter::hasNext() const {
    return !(curr_pointer == utf8_array.end()) && (status == SUCCESS);
}

CODE_32 StrUtf8Iter::getStatus() {
    return status;
}

void StrUtf8Iter::estimate_size() {
    status = UNKNOWN_ERROR;
    uint8_t firstbit = *curr_pointer;
    //firstbit = 0b10101010; //to trigger error WRONG_BYTE_IN_UTF_8
    if (firstbit < 0x80) {
        curr_length = 1;
    } else if ((firstbit & 0xe0) == 0xc0) {
        curr_length = 2;
    } else if ((firstbit & 0xf0) == 0xe0) {
        curr_length = 3;
    } else if ((firstbit & 0xf8) == 0xf0) {
        curr_length = 4;
    } else if ((firstbit & 0xfc) == 0xf8) {
        curr_length = 5;
    } else if ((firstbit & 0xfe) == 0xfc) {
        curr_length = 6;
    } else {
        status = WRONG_BYTE_IN_UTF_8;
        return;
    }
    status = SUCCESS;
}

void StrUtf8Iter::convert_to_32(const vector<uint8_t> &utf8_input, uint32_t &c) {
    status = UNKNOWN_ERROR;
    if (utf8_input.size() < curr_length) {
        status = BAD_BYTE_VECTOR;
        return;
    }
    switch (curr_length) {
        case 1 : {
            c = utf8_input[0];
            break;
        }
        case 2 : {
            c = ((uint32_t) (utf8_input[0] & 0x1f) << 6) |
                ((uint32_t) (utf8_input[1] & 0x3f) << 0);
            break;
        }
        case 3 : {
            c = ((uint32_t) (utf8_input[0] & 0x0f) << 12) |
                ((uint32_t) (utf8_input[1] & 0x3f) << 6) |
                ((uint32_t) (utf8_input[2] & 0x3f) << 0);
            break;
        }
        case 4 : {
            c = ((uint32_t) (utf8_input[0] & 0x07) << 18) |
                ((uint32_t) (utf8_input[1] & 0x3f) << 12) |
                ((uint32_t) (utf8_input[2] & 0x3f) << 6) |
                ((uint32_t) (utf8_input[3] & 0x3f) << 0);
            break;
        }
        case 5 : {
            c = ((uint32_t) (utf8_input[0] & 0x07) << 24) |
                ((uint32_t) (utf8_input[1] & 0x3f) << 18) |
                ((uint32_t) (utf8_input[2] & 0x3f) << 12) |
                ((uint32_t) (utf8_input[3] & 0x3f) << 6) |
                ((uint32_t) (utf8_input[4] & 0x3f) << 0);
            break;
        }
        case 6 : {
            c = ((uint32_t) (utf8_input[0] & 0x07) << 30) |
                ((uint32_t) (utf8_input[1] & 0x3f) << 24) |
                ((uint32_t) (utf8_input[2] & 0x3f) << 18) |
                ((uint32_t) (utf8_input[3] & 0x3f) << 12) |
                ((uint32_t) (utf8_input[4] & 0x3f) << 6) |
                ((uint32_t) (utf8_input[4] & 0x3f) << 0);
            break;
        }
        default: {
            status = INVALID_NUMBER_OF_BYTES;
        }
    }
    if (c >= 0xd800 && c <= 0xdfff) {
        c = -1; // surrogate half
        status = SURROGATE_HALF;
        return;
    }
    status = SUCCESS;
}

uint32_t StrUtf8Iter::getNext() {
    estimate_size();
    if (status != SUCCESS) {
        return 0;
    }
    vector<uint8_t> current_vector;
    for (int i = 0; i < curr_length; i++) {
        current_vector.push_back(*curr_pointer);
        curr_pointer++;
        if (curr_pointer == utf8_array.end()) {
            if (i != curr_length - 1) {
                status = BAD_BYTE_VECTOR;
                return 0;
            }
        }
    }
    if (status != SUCCESS) {
        return 0;
    }
    uint32_t res;
    convert_to_32(current_vector, res);
    if (status != SUCCESS) {
        return 0;
    }
    return res;
}




