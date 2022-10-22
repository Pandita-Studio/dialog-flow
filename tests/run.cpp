#include <vector>
#include <cassert>
#include <iostream>
#include "ScriptLoader.h"
#include "Tokenizer.h"

using namespace df;
using namespace std;

void TEST_ScriptLoader_LoadInvalidScript()
{
    ScriptLoader script_loader("IDoNotExists.df");

    assert(!script_loader.errors->empty());
    //script_loader.errors->traceback();
}

void TEST_ScriptLoader_SourceCodeSetByMethod()
{
    ScriptLoader script_loader;

    const char *test_script =
        "# STARTLABEL\r\n"
        "# ------------\r\n"
        "label start:\r\n"
        "    # foo comment\r\n"
        "    $ player_bag.item_phone = item_phone\r\n"
        "\r\n"
        "    scene black with dissolve\r\n"
        "\r\n"
        "    $ renpy.pause(2, hard=True)\r\n"
        "\r\n"
        "    stop music fadeout 5.0\r\n"
        "\r\n"
        "    if not persistent.script_a:\r\n"
        "        jump script_a_chapter_1\r\n"
        "\r\n"
        "    else:\r\n"
        "        jump script_b_chapter_1\r\n"
        "# ------------";

    script_loader.set_source_code(test_script); 

    assert(strcmp(script_loader.get_source_code(), test_script) == 0);
    assert(script_loader.get_source_size() == 331);
}

void TEST_Tokenizer_ErrorHandling()
{
    ScriptLoader script_loader;

    const char *test_script =
        "label start:\r\n"
        "    ! player_bag.item_phone = item_phone\r\n"
        "# ------------";

    script_loader.set_source_code(test_script);
    
    Tokenizer tokenizer;

    tokenizer.set_script(&script_loader);
    tokenizer.tokenize();
    
    assert(!tokenizer.errors->empty());
    //tokenizer.errors->traceback();
}

void TEST_Tokenizer_Tokenization()
{
    ScriptLoader script_loader;

    const char *test_script =
        "label s:\n"
        "\n"
        "    $ foo[0].bar = {12, 13}\r\n"
        "    define f wait(0.5)\r\n"
        "    baz \"b'a'$%&az\" with f at c\r\n"
        "\n"
        "\n"
        "\n"
        "# ------------";

    script_loader.set_source_code(test_script);
    
    Tokenizer tokenizer;

    tokenizer.set_script(&script_loader);
    tokenizer.tokenize();

    const std::vector<Tokenizer::Token> expected_tokens = {
        Tokenizer::Token(Tokenizer::Token::LABEL),
        Tokenizer::Token(Tokenizer::Token::IDENTIFIER),
        Tokenizer::Token(Tokenizer::Token::COLON),
        Tokenizer::Token(Tokenizer::Token::NEWLINE),
        Tokenizer::Token(Tokenizer::Token::TAB),
        Tokenizer::Token(Tokenizer::Token::DOLLAR),
        Tokenizer::Token(Tokenizer::Token::IDENTIFIER),
        Tokenizer::Token(Tokenizer::Token::OPEN_BRACKET),
        Tokenizer::Token(Tokenizer::Token::INTEGER),
        Tokenizer::Token(Tokenizer::Token::CLOSE_BRACKET),
        Tokenizer::Token(Tokenizer::Token::POINT),
        Tokenizer::Token(Tokenizer::Token::IDENTIFIER),
        Tokenizer::Token(Tokenizer::Token::EQUAL),
        Tokenizer::Token(Tokenizer::Token::OPEN_KEY),
        Tokenizer::Token(Tokenizer::Token::INTEGER),
        Tokenizer::Token(Tokenizer::Token::COMMA),
        Tokenizer::Token(Tokenizer::Token::INTEGER),
        Tokenizer::Token(Tokenizer::Token::CLOSE_KEY),
        Tokenizer::Token(Tokenizer::Token::NEWLINE),
        Tokenizer::Token(Tokenizer::Token::TAB),
        Tokenizer::Token(Tokenizer::Token::DEFINE),
        Tokenizer::Token(Tokenizer::Token::IDENTIFIER),
        Tokenizer::Token(Tokenizer::Token::IDENTIFIER),
        Tokenizer::Token(Tokenizer::Token::OPEN_PARENTHESIS),
        Tokenizer::Token(Tokenizer::Token::FLOAT),
        Tokenizer::Token(Tokenizer::Token::CLOSE_PARENTHESIS),
        Tokenizer::Token(Tokenizer::Token::NEWLINE),
        Tokenizer::Token(Tokenizer::Token::TAB),
        Tokenizer::Token(Tokenizer::Token::IDENTIFIER),
        Tokenizer::Token(Tokenizer::Token::STRING),
        Tokenizer::Token(Tokenizer::Token::WITH),
        Tokenizer::Token(Tokenizer::Token::IDENTIFIER),
        Tokenizer::Token(Tokenizer::Token::AT),
        Tokenizer::Token(Tokenizer::Token::IDENTIFIER),
        Tokenizer::Token(Tokenizer::Token::NEWLINE),
        Tokenizer::Token(Tokenizer::Token::TK_EOF)
    };
    
    for(;;) {
        const Tokenizer::Token token = tokenizer.scan(0);

        assert(token.type == expected_tokens.at(tokenizer.get_cursor_position()).type);
        //cout << token.to_string() << endl;
        
        if (token.type == Tokenizer::Token::TK_EOF) break;
        tokenizer.consume(); // cursor step
    }
}

int main()
{
    cout << "TEST_ScriptLoader_LoadInvalidScript ... ";
    TEST_ScriptLoader_LoadInvalidScript();
    cout << "OK" << endl;

    cout << "TEST_ScriptLoader_SourceCodeSetByMethod ... ";
    TEST_ScriptLoader_SourceCodeSetByMethod();
    cout << "OK" << endl;

    cout << "TEST_Tokenizer_ErrorHandling ... ";
    TEST_Tokenizer_ErrorHandling();
    cout << "OK" << endl;

    cout << "TEST_Tokenizer_Tokenization ... ";
    TEST_Tokenizer_Tokenization();
    cout << "OK";
    
    return 0;
}
