
#include <gtest/gtest.h>
#include <kaleidoscope/istream.h>
#include <kaleidoscope/lexer.h>

struct LexerGetAllTokensTestCase {
    const char *input;
    std::vector<Token *> *expected;
};

class GoodLexerGetAllTokensTest : public testing::TestWithParam<LexerGetAllTokensTestCase> {

    void TearDown() override {
        auto &tokens = GetParam().expected;
        for (auto tok: *tokens) {
            delete tok;
        }
        tokens->clear();
        delete tokens;
    };
};

TEST_P(GoodLexerGetAllTokensTest, WillNotThrowException) /* NOLINT */
{
    auto &&param = GetParam();
    std::stringstream in(param.input);
    auto input = IStream(in);
    Lexer<IStream> lexer(input);
    auto tokens = lexer.get_all_tokens();
    ASSERT_EQ(tokens.size(), param.expected->size());
//    for (size_t i = 0; i < tokens.size(); i++) {
//        ASSERT_TOKEN_EQUAL(tokens[i], (*param.expected)[i]);
//    }
}

INSTANTIATE_TEST_SUITE_P(Simple, GoodLexerGetAllTokensTest, testing::Values( /* NOLINT */
        LexerGetAllTokensTestCase{
                "", new std::vector<Token *>()},
        LexerGetAllTokensTestCase{
                "a",
                new std::vector<Token *>({new Identifier("a")})},
        LexerGetAllTokensTestCase{
                "a 1",
                new std::vector<Token *>({new Identifier("a"), new Number(1)})},
        LexerGetAllTokensTestCase{
                "a a1 1",
                new std::vector<Token *>({
                                                 new Identifier("a"), new Identifier("a1"),
                                                 new Number(1)})},
        LexerGetAllTokensTestCase{
                "1+1",
                new std::vector<Token *>({
                                                 new Number(1), new Marker(MarkerType::Add), new Number(1)})}
));


