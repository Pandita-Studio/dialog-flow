#ifndef DF_TOKENIZER_H
#define DF_TOKENIZER_H

#include <string>
#include <vector>
#include "ErrorStack.h"
#include "ScriptLoader.h"

namespace df {
    static const char *token_names[] = {
        "T_EMPTY",
        "T_NEWLINE",
        "T_EQUAL",
        "T_POINT",
        "T_COLON",
        "T_COMMA",
        "T_OPEN_PARENTHESIS",
        "T_CLOSE_PARENTHESIS",
        "T_OPEN_BRACKET",
        "T_CLOSE_BRACKET",
        "T_OPEN_KEY",
        "T_CLOSE_KEY",
        "T_DOLLAR",
        "T_TAB",
        "T_INTEGER",
        "T_FLOAT",
        "T_STRING",
        "T_IDENTIFIER",
        "T_LABEL",
        "T_AT",
        "T_WITH",
        "T_DEFINE",
        "T_SOF",
        "T_EOF"
    };

    class Tokenizer {
    public:
        const char END_OF_TEXT = 0x03;
        const char START_OF_TEXT = 0x02;

        struct Token {
            const enum Type {
                EMPTY,
                // Simbols
                NEWLINE,
                EQUAL,
                POINT,
                COLON,
                COMMA,
                OPEN_PARENTHESIS,
                CLOSE_PARENTHESIS,
                OPEN_BRACKET,
                CLOSE_BRACKET,
                OPEN_KEY,
                CLOSE_KEY,
                DOLLAR,
                // Basic
                TAB,
                INTEGER,
                FLOAT,
                STRING,
                IDENTIFIER,
                // Keywords
                LABEL,
                AT,
                WITH,
                DEFINE,
                TK_SOF,
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

            Token(Type p_type) {
                type = p_type;
                literal = "?";
            }

            Token() {
                literal = "_";
            }

            const std::string to_string() const {
                std::string token_string(token_names[type]);

                token_string += "(" + literal + ")";
                token_string += " at Line " + std::to_string(row) + " Col " + std::to_string(col);

                return token_string;
            }
        };

        ErrorStack *errors;

    private:

        uint32_t col;
        uint32_t row;
        uint32_t cursor;

        ScriptLoader *script;

        std::vector<Token> tokens;

        void _advance();
        const char _peek(int p_position) const;

        const bool _is_valid_digit_char(char symbol) const;
        const bool _is_valid_identifier_name_char(char symbol) const;

        const Token _parse_keywords(Token p_token) const;
        
    public:

        inline const uint32_t get_cursor_position() const { return cursor; }
        inline const std::vector<Token> get_tokens() const { return tokens; }

        void set_script(ScriptLoader *p_script);
        void tokenize();
        
        const Token scan(int p_position) const;
        void consume();

        Tokenizer();
        virtual ~Tokenizer();
    };
}

#endif //DF_TOKENIZER_H