#include <iostream>
#include <cassert>
#include "ScriptLoader.h"
#include "Tokenizer.h"

using namespace df;
using namespace std;

int main()
{
    ScriptLoader script_loader("TestDialog.df");

    if (!script_loader.errors->empty())
        script_loader.errors->debug_trace_back();

    assert(script_loader.errors->empty());

    cout << script_loader.get_source_code() << endl << endl;

    Tokenizer tokenizer;

    tokenizer.set_script(&script_loader);
    tokenizer.tokenize();
    tokenizer.debug_parsed_tokens();

    return 0;
}
