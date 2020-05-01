//
// Created by kamiyoru on 2020/5/1.
//

#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <kaleidoscope/istream.h>
#include <kaleidoscope/lexer.h>
#include <kaleidoscope/parser.h>
#include <kaleidoscope/llvm-codegen.h>

#define VERSION "0.1.0"


int32_t main(int argc, char **argv) {
    if (argc <= 1) {
        printf("kaleidoscope-llvm version: " VERSION);
    }

    std::ifstream  fin(argv[1]);

    if (fin.fail()) {
        std::cerr << "Kale-LLVM Open Input File "  << argv[1] << " Failed: " << strerror(errno);
        return errno;
    }

    auto input = IStream(fin);

    using IStreamLexer = Lexer<IStream>;

    IStreamLexer lexer(input);
    Parser<IStreamLexer> parser(lexer);

    auto builder = LLVMBuilder();
    auto value = builder.code_gen(parser.parse());
    std::string buf;
    llvm::raw_string_ostream os(buf);

    value->print(os, true);
    std::cout << buf;

    fin.close();

    return 0;
}