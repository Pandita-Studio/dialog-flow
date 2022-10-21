#include <iostream>
#include <cassert>
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

    char *test_script =
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

    char *test_script =
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

    char *test_script =
        "label start:\r\n"
        "    $ player_bag.item_phone = item_phone\r\n"
        "    scene black with dissolve\r\n"
        "    $ renpy.pause(2, hard=True)\r\n"
        "    stop music fadeout 5.0\r\n"
        "    if not persistent.script_a:\r\n"
        "        jump script_a_chapter_1\r\n"
        "    else:\r\n"
        "        jump script_b_chapter_1\r\n"
        "# ------------";

    script_loader.set_source_code(test_script);
    
    Tokenizer tokenizer;

    tokenizer.set_script(&script_loader);
    tokenizer.tokenize();
    
    for(;;) {
        Tokenizer::Token token = tokenizer.scan(0);

        tokenizer.consume();

        //cout << token.to_string() << endl;

        if (token.type == Tokenizer::Token::TK_EOF)
            break;
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
