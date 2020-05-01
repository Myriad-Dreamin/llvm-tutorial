//
// Created by kamiyoru on 2020/5/2.
//

#ifndef KALEIDOSCOPE_ISTREAM_H
#define KALEIDOSCOPE_ISTREAM_H
#include <istream>


struct IStream /* implement Stream */ {

    explicit IStream(std::istream &in) : in(in) {}

    [[maybe_unused]] char next_char() {
        return in.get();
    }

protected:
    std::istream &in;
};

#endif //KALEIDOSCOPE_ISTREAM_H
