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

    // 打开文件
    std::ifstream  fin(argv[1]);
    if (fin.fail()) {
        std::cerr << "Kale-LLVM open input file "  << argv[1] << " failed: " << strerror(errno);
        return errno;
    }

    using IStreamLexer = Lexer<IStream>;

    // 构建模块
    IStream input(fin);
    IStreamLexer lexer(input);
    Parser<IStreamLexer> parser(lexer);
    LLVMBuilder builder;

    // 解析文件
    auto ast = parser.parse();
    auto value = builder.code_gen(ast);

    // 输出中间代码，待生成目标平台代码
    std::string buf;
    llvm::raw_string_ostream os(buf);
    value->print(os, true);
    std::cout << buf;

    // 关闭
    fin.close();

    // 清理堆内存
    delete ast;

    return 0;
}