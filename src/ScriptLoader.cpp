#include "ScriptLoader.h"

using namespace df;
using namespace std;

ScriptLoader::ScriptLoader(char p_file_path[]) {
    errors = new ErrorStack("Script loader");

    _source_file = fopen(p_file_path, "rb");

    if (_source_file == NULL) {
        errors->push_error(ErrorStack::Error("Unable to open script: " + string(p_file_path)));
        return;
    }

    fseek(_source_file, 0, SEEK_END);

    _source_size = ftell(_source_file);

    fseek(_source_file, 0, SEEK_SET);

    _source_code = (char *)malloc(_source_size + 1);

    fread(_source_code, _source_size, 1, _source_file);
    fclose(_source_file);

    _source_code[_source_size] = 0;
}

ScriptLoader::~ScriptLoader() {
    if (errors->empty()) {
        delete _source_code;
        delete _source_file;
    }

    delete errors;
}