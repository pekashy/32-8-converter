#include <iostream>
#include <vector>
#include <bitset>
#include <cassert>
#include <string>
#include <codecvt>
#include <locale>

using std::vector;

size_t convert_to_8(char *bitpart, uint32_t symb32);

vector<uint8_t> to_utf8(const vector<uint32_t> &x);
