#ifndef DF_SCRIPTLOADER_H
#define DF_SCRIPTLOADER_H

#include <string>
#include <iostream>
#include "ErrorStack.h"

namespace df{
    class ScriptLoader {
        public:
            ErrorStack *errors;

        private:
            char *_source_code;
            uint32_t _source_size;
            std::FILE *_source_file;
        
        public:
            inline const char* get_source_code() const { return _source_code; }
            inline const uint32_t get_source_size() const { return _source_size; }

            void set_source_code(char *p_source_code);

            ScriptLoader();
            ScriptLoader(char p_file_path[]);
            ~ScriptLoader();
    };
}

#endif // DF_SCRIPTLOADER_H