#include <iostream>
#include <string>
#include <cassert>
#include <sstream>
#include <vector>
#include <numeric>
#include <algorithm>
#include "../shared/BitStream.h"

using namespace std;

const string PKTTYPE[] = {"SUM", "PROD", "MIN", "MAX", "LIT", "GT", "LT", "EQ"};
enum class PktType {SUM, PROD, MIN, MAX, LIT, GT, LT, EQ};

struct PacketVal {
    size_t consumed;
    size_t value;
};

PacketVal DecodePacket(BitStream& bs, size_t& ver_sum)
{
    size_t consumed = 0;
    size_t value = 0;

    auto ver = bs.getbits(3);
    ver_sum += ver;
    cout << 'v' << ver << ':';

    auto t = bs.getbits(3);
    auto ty = static_cast<PktType>(t);
    consumed += 6;
    cout << PKTTYPE[t] << ':';

    if (ty == PktType::LIT) {

        // Literal
        bool cont = true;
        size_t val = 0;
        while (cont) {
            cont = bs.getbits(1);
            auto four = bs.getbits(4);
            val = (val << 4) | four;
            consumed += 5;
        }

        // TODO: Something with val
        cout << "lit" << val << ':';
        value = val;
    }
    else {
        vector<size_t> subvals;

        auto ltype = bs.getbits(1);
        consumed += 1;

        if (ltype == 0) {
            auto len = bs.getbits(15);
            consumed += 15;

            cout << "subl" << len << ":[";
            
            size_t read = 0;
            while (read < len) {
                auto pktval = DecodePacket(bs, ver_sum);
                read += pktval.consumed;
                subvals.push_back(pktval.value);
            }

            consumed += read;
            cout << "]";
        }
        else {
            auto num = bs.getbits(11);
            consumed += 11;

            cout << "subn" << num << ":[";

            size_t read = 0;
            while (read < num) {
                auto pktval = DecodePacket(bs, ver_sum);
                consumed += pktval.consumed;
                subvals.push_back(pktval.value);
                ++read;
            }
            cout << "]";
        }

        switch (ty) {
        case PktType::SUM:
            value = accumulate(subvals.begin(), subvals.end(), 0i64);
            break;

        case PktType::PROD:
            value = accumulate(subvals.begin(), subvals.end(), 1i64, [](auto x, auto y) { return x * y; });
            break;

        case PktType::MIN:
            value = *min_element(subvals.begin(), subvals.end());
            break;

        case PktType::MAX:
            value = *max_element(subvals.begin(), subvals.end());
            break;

        case PktType::GT:
            assert(subvals.size() == 2);
            value = subvals[0] > subvals[1] ? 1 : 0;
            break;

        case PktType::LT:
            assert(subvals.size() == 2);
            value = subvals[0] < subvals[1] ? 1 : 0;
            break;

        case PktType::EQ:
            assert(subvals.size() == 2);
            value = subvals[0] == subvals[1] ? 1 : 0;
            break;
        }
    }

    return { consumed, value };
}

int main()
{
    string s;
    while (getline(cin, s)) {
        cout << "Decode " << s << endl << "  ";
        stringstream ss(s);
        BitStream bs(ss);
        size_t ver_sum = 0;
        auto pktval = DecodePacket(bs, ver_sum);
        cout << endl;
        cout << "Version Sum = " << ver_sum << endl;
        cout << "Value = " << pktval.value << endl;
    }
}