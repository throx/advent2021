#pragma once
#include <istream>
#include <sstream>
#include <cassert>

class BitStream
{
    static const int SIZETBITS = (sizeof(size_t) * 8);
private:
    std::istream& m_is;
    size_t m_acc = 0;
    size_t m_bitsused = SIZETBITS;

    void Fill()
    {
        while (m_bitsused > 3) {
            char hexdigit = -1;
            m_is >> hexdigit;
            if (hexdigit == -1) {
                break;
            }
            hexdigit = tolower(hexdigit);
            size_t val = hexdigit > '9' ? hexdigit - 'a' + 10 : hexdigit - '0';
            m_bitsused -= 4;
            m_acc |= (val & 0xf) << m_bitsused;
        }
    }

public:
    BitStream(std::istream& is) :
        m_is(is)
    {
    }

    size_t getbits(size_t num)
    {
        if (num > SIZETBITS) {
            throw std::exception("Internal State Error: too many bits");
        }

        size_t ret = 0;
        if (num + m_bitsused > SIZETBITS) {
            ret = m_acc >> m_bitsused;
            num -= (SIZETBITS - m_bitsused);
            m_bitsused = SIZETBITS;
            ret = ret << num;
            m_acc = 0;
            Fill();
        }
        m_bitsused += num;
        if (m_bitsused > SIZETBITS) {
            throw std::exception("Read past end of BitStream");
        }
        ret = ret | (m_acc >> (SIZETBITS - num));
        m_acc = m_acc << num;

        return ret;
    }

    bool eof() const
    {
        return m_is.eof();
    }

};

#ifdef _DEBUG
namespace __BitStreamTest__ {
    class BitStreamTest
    {
    public:
        BitStreamTest()
        {
            std::stringstream ss("b6db6db6db6db6db6db6db6db6db6db6db6db6db6db6db6db6db6db6db6db6db6db6db6db6db6db6db6db6db6db6db6db6db6db6db6db6d");
            BitStream bs(ss);
            while (!bs.eof()) {
                size_t b = bs.getbits(3);
                assert(b == 5 || bs.eof());
            }
        }
    };
    static BitStreamTest _bst;
}
#endif
