#include "to8converter.h"
#include "to32converter.h"

bool test_32_8(std::pair<vector<uint8_t>, vector<uint32_t>> &input) {
    vector<uint8_t> res = to_utf8(input.second);
    if (input.first != res) {
        cerr << "Conversion mistake\n";
        return false;
    }
    cout << "Test 8-32 passed\n";
    return true;
}

bool test_8_32(std::pair<vector<uint8_t>, vector<uint32_t>> &input) {
    std::pair<CODE_32, std::vector<uint32_t>> v32 = to_utf32(input.first);
    if (v32.first == SUCCESS) {
        if (v32.second != input.second) {
            cerr << "Conversion mistake\n";
            return false;
        }
    } else {
        print_error(v32.first);
        return false;
    }
    cout << "Test 8-32 passed\n";
    return true;

}


bool cross_test_32_8(vector<uint32_t> v32) {
    vector<uint8_t> res = to_utf8(v32);
    std::pair<CODE_32, std::vector<uint32_t>> back_v32 = to_utf32(res);
    if (back_v32.first == SUCCESS) {
        if (v32 != back_v32.second) {
            cerr << "Conversion mistake\n";
            return false;
        }
    } else {
        print_error(back_v32.first);
        return false;
    }
    cout << "Cross test 32-8 passed\n";
    return true;
}

bool cross_test_8_32(const vector<uint8_t> &v8) {
    std::pair<CODE_32, std::vector<uint32_t>> v32 = to_utf32(v8);
    if (v32.first == SUCCESS) {
        vector<uint8_t> res = to_utf8(v32.second);
        if (res != v8) {
            cerr << "Conversion mistake\n";
            return false;
        }
    } else {
        print_error(v32.first);
        return false;
    }
    cout << "Cross test 8-32 passed\n";
    return true;
}


int main() {
    auto teststr1 = std::make_pair(std::vector<uint8_t>({0xe1, 0x82, 0xa0}), std::vector<uint32_t>({0x10A0}));
    auto teststr2 = std::make_pair(std::vector<uint8_t>({0xe1, 0x85, 0x99}), std::vector<uint32_t>({0x1159}));
    std::vector<uint8_t> invalid_str1({0b11111111, 0x85, 0x99});
    std::vector<uint8_t> invalid_str2({0xe1, 0b11000000, 0x99});

    bool passed = true;
    passed &= cross_test_32_8(teststr1.second);
    passed &= cross_test_32_8(teststr2.second);
    passed &= cross_test_8_32(teststr1.first);
    passed &= cross_test_8_32(teststr2.first);
    passed &= test_8_32(teststr1);
    passed &= test_8_32(teststr2);
    passed &= test_32_8(teststr2);
    passed &= test_32_8(teststr2);
    passed &= !cross_test_8_32(invalid_str1);
    passed &= !cross_test_8_32(invalid_str1);

    if (passed) {
        cout << "Success!\n";
    }
    return 0;
}