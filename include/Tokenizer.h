#ifndef DF_TOKENIZER_H
#define DF_TOKENIZER_H

#include <string>
#include <vector>
#include "ScriptLoader.h"

namespace df {
    class Tokenizer {
    public:
        struct Token {
            enum Type {
                EMPTY,
                // Simbols
                NEWLINE,
                EQUAL,
                POINT,
                DOUBLE_POINT,
                COMMA,
                OPEN_PARENTHESIS,
                CLOSE_PARENTHESIS,
                OPEN_BRACKET,
                CLOSE_BRACKET,
                OPEN_KEY,
                CLOSE_KEY,
                DOLLAR,
                // Basic
                INTEGER,
                FLOAT,
                STRING,
                IDENTIFIER,
                // Keywords
                LABEL,
                AT,
                WITH,
                DEFINE,
                TK_EOF,
                TK_MAX
            };

            Type type = EMPTY;
            std::string literal;
            uint32_t col = 0, row = 0;

            Token(Type p_type, uint32_t p_col, uint32_t p_row, std::string p_literal) {
                type = p_type;
                col = p_col;
                row = p_row;
                literal = p_literal;
            }

            Token(Type p_type, uint32_t p_col, uint32_t p_row) {
                type = p_type;
                col = p_col;
                row = p_row;
                literal = "?";
            }

            Token() {

            }
        };

    private:

        bool done;
        uint32_t col;
        uint32_t row;
        uint32_t cursor;

        ScriptLoader *script;

        std::vector<Token> tokens;

        void _advance();
        char _peek(int p_position);

        bool _is_valid_digit_char(char symbol);
        bool _is_valid_identifier_name_char(char symbol);

        Token _parse_keywords(Token p_token);
        
    public:

        inline const std::vector<Token> get_tokens() const { return tokens; }

        void set_script(ScriptLoader *p_script);
        void tokenize();
        void debug_parsed_tokens();

        Tokenizer();
        ~Tokenizer();
    };
}

#endif //DF_TOKENIZER_H