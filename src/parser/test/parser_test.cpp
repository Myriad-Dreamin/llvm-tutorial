
#include <gtest/gtest.h>
#include <kaleidoscope/istream.h>
#include <kaleidoscope/lexer.h>
#include <kaleidoscope/parser.h>

struct ParserParseTestCase {
    const char *input;
    ASTNode *expected;

};

class GoodParserParseTest : public testing::TestWithParam<ParserParseTestCase> {

    void TearDown() override {
        if (GetParam().expected != nullptr) delete GetParam().expected;
    };
};

TEST_P(GoodParserParseTest, WillNotThrowException) /* NOLINT */
{
    auto &&param = GetParam();
    std::stringstream in(param.input);
    auto input = IStream(in);

    using IStreamLexer = Lexer<IStream>;

    IStreamLexer lexer(input);
    Parser<IStreamLexer> parser(lexer);

    auto _ = parser.parse();
//    ASSERT_EQ(tokens.size(), param.expected->size());
//    for (size_t i = 0; i < tokens.size(); i++) {
//        ASSERT_TOKEN_EQUAL(tokens[i], (*param.expected)[i]);
//    }
}

INSTANTIATE_TEST_SUITE_P(Simple, GoodParserParseTest, testing::Values( /* NOLINT */
        ParserParseTestCase{
                "1", new ExpNumber(new Number(1))},
        ParserParseTestCase{
                "1+1", new BiExp(
                        new ExpNumber(new Number(1)), new Marker(MarkerType::Add),
                        new ExpNumber(new Number(1)))},
        ParserParseTestCase{
                "1+1+1",
                new BiExp(
                        new BiExp(
                                new ExpNumber(new Number(1)),
                                new Marker(MarkerType::Add),
                                new ExpNumber(new Number(1))
                        ),
                        new Marker(MarkerType::Add),
                        new ExpNumber(new Number(1))
                )
        },
        ParserParseTestCase{
                "1+1*1",
                new BiExp(
                        new ExpNumber(new Number(1)),
                        new Marker(MarkerType::Add),
                        new BiExp(
                                new ExpNumber(new Number(1)),
                                new Marker(MarkerType::Mul),
                                new ExpNumber(new Number(1))
                        )
                )
        },
        ParserParseTestCase{
                "1+1*1+1",
                new BiExp(
                        new BiExp(
                                new ExpNumber(new Number(1)),
                                new Marker(MarkerType::Add),
                                new BiExp(
                                        new ExpNumber(new Number(1)),
                                        new Marker(MarkerType::Mul),
                                        new ExpNumber(new Number(1))
                                )
                        ),
                        new Marker(MarkerType::Add),
                        new ExpNumber(new Number(1))
                )
        },
        ParserParseTestCase{
                "(1)", new ExpNumber(new Number(1))},
        ParserParseTestCase{
                "(1+1)", new BiExp(
                        new ExpNumber(new Number(1)), new Marker(MarkerType::Add),
                        new ExpNumber(new Number(1)))},
        ParserParseTestCase{
                "1+(1+1)",
                new BiExp(
                        new ExpNumber(new Number(1)),
                        new Marker(MarkerType::Add),
                        new BiExp(
                                new ExpNumber(new Number(1)),
                                new Marker(MarkerType::Add),
                                new ExpNumber(new Number(1))
                        )
                )
        },
        ParserParseTestCase{
                "1*(1+1)",
                new BiExp(
                        new ExpNumber(new Number(1)),
                        new Marker(MarkerType::Mul),
                        new BiExp(
                                new ExpNumber(new Number(1)),
                                new Marker(MarkerType::Add),
                                new ExpNumber(new Number(1))
                        )
                )
        },
        ParserParseTestCase{
                "a", new ExpIdent(new Identifier("a"))},
        ParserParseTestCase{
                "a+b", new BiExp(
                        new ExpIdent(new Identifier("a")), new Marker(MarkerType::Add),
                        new ExpIdent(new Identifier("b")))},
        ParserParseTestCase{
                "a()+b", new BiExp(
                        new ExpFuncCall(
                                new Identifier("a"), new ParamList()), new Marker(MarkerType::Add),
                        new ExpIdent(new Identifier("b")))},
        ParserParseTestCase{
                "a(1)+b", new BiExp(
                        new ExpFuncCall(
                                new Identifier("a"), new ParamList(
                                        {new ExpNumber(new Number(1))}
                                )), new Marker(MarkerType::Add),
                        new ExpIdent(new Identifier("b")))},
        ParserParseTestCase{
                "a(1,a)+b", new BiExp(
                        new ExpFuncCall(
                                new Identifier("a"), new ParamList(
                                        {new ExpNumber(new Number(1)), new ExpIdent(new Identifier("a"))}
                                )), new Marker(MarkerType::Add),
                        new ExpIdent(new Identifier("b")))},
        ParserParseTestCase{
                "a(1,a())+b", new BiExp(
                        new ExpFuncCall(
                                new Identifier("a"), new ParamList(
                                        {new ExpNumber(new Number(1)),
                                         new ExpFuncCall(
                                                 new Identifier("a"), new ParamList())}
                                )), new Marker(MarkerType::Add),
                        new ExpIdent(new Identifier("b")))}
));


