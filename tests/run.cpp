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

void TEST_Tokenizer_Tokenization()
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

    cout << script_loader.get_source_code() << endl;
    
    Tokenizer tokenizer;

    tokenizer.set_script(&script_loader);
    tokenizer.tokenize();
    tokenizer.debug_parsed_tokens();
}

int main()
{
    cout << "TEST_ScriptLoader_LoadInvalidScript" << endl;
    TEST_ScriptLoader_LoadInvalidScript();
    cout << "TEST_ScriptLoader_SourceCodeSetByMethod" << endl;
    TEST_ScriptLoader_SourceCodeSetByMethod();
    cout << "TEST_Tokenizer_Tokenization" << endl;
    TEST_Tokenizer_Tokenization();
    return 0;
}
