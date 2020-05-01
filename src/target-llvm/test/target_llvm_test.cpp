
#include <gtest/gtest.h>
#include <mutex>
#include <kaleidoscope/lexer.h>
#include <kaleidoscope/parser.h>
#include <kaleidoscope/llvm-codegen.h>
#include <iostream>

struct GenerateLLVMIRTestCase {
    const char *input;
};

class GoodGenerateLLVMIRTest : public testing::TestWithParam<GenerateLLVMIRTestCase> {

    void TearDown() override {

    };
};

TEST_P(GoodGenerateLLVMIRTest, WillNotThrowException) /* NOLINT */
{
    auto &&param = GetParam();
    std::stringstream in(param.input);
    auto input = IStream(in);

    using IStreamLexer = Lexer<IStream>;

    IStreamLexer lexer(input);
    Parser<IStreamLexer> parser(lexer);

    auto builder = LLVMBuilder();
    auto value = builder.code_gen(parser.parse());
    std::string buf;
    llvm::raw_string_ostream os(buf);

    value->print(os, true);
    std::cout << buf;
}

INSTANTIATE_TEST_SUITE_P(Simple, GoodGenerateLLVMIRTest, testing::Values( /* NOLINT */
        GenerateLLVMIRTestCase{"1"},
        GenerateLLVMIRTestCase{"1+1"},
        GenerateLLVMIRTestCase{"1+(2*3)"}
));


