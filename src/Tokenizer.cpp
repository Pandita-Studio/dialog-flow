#include "Tokenizer.h"

using namespace df;
using namespace std;

static const char *token_names[] = {
    "T_EMPTY",
    "T_NEWLINE",
    "T_EQUAL",
    "T_POINT",
    "T_DOUBLE_POINT",
    "T_COMMA",
    "T_OPEN_PARENTHESIS",
    "T_CLOSE_PARENTHESIS",
    "T_OPEN_BRACKET",
    "T_CLOSE_BRACKET",
    "T_OPEN_KEY",
    "T_CLOSE_KEY",
    "T_DOLLAR",
    "T_INTEGER",
    "T_FLOAT",
    "T_STRING",
    "T_IDENTIFIER",
    "T_LABEL",
    "T_AT",
    "T_WITH",
    "T_DEFINE",
    "T_EOF"
};

static_assert(sizeof(token_names) / sizeof(token_names[0]) == Tokenizer::Token::TK_MAX, "Amount of token names don't match the amount of token types.");

Tokenizer::Tokenizer() {
    done = false;
    cursor = 0;
    row = 1;
    col = 1;
}

Tokenizer::~Tokenizer() {

}

void Tokenizer::_advance() {
    col++;
    cursor++;

    if (cursor == script->get_source_size())
        done = true;
}

char Tokenizer::_peek(int p_position) {
    if (p_position == -1 && cursor == 0)
        return 2; // ascii START_OF_TEXT
    
    else if (p_position == 1 && (cursor + 1) == script->get_source_size())
        return 3; // ascii END_OF_TEXT
    
    else
        return script->get_source_code()[cursor + p_position];
}

bool Tokenizer::_is_valid_digit_char(char symbol){
    uint32_t ascii = symbol;

    if (ascii >= 48 && ascii <= 57)
        return true;
    
    return false;
}

bool Tokenizer::_is_valid_identifier_name_char(char symbol) {
    uint32_t ascii = symbol;

    if (symbol == '_')
        return true;

    else if (ascii >= 48 && ascii <= 57)
        return true;

    else if ((ascii >= 65 && ascii <= 90) || (ascii >= 97 && ascii <= 122))
        return true;   

    return false; 
}

Tokenizer::Token Tokenizer::_parse_keywords(Token p_token) {
    if (p_token.literal == "label")
        p_token.type = Token::LABEL;
    
    else if (p_token.literal == "at")
        p_token.type = Token::AT;
    
    else if (p_token.literal == "with")
        p_token.type = Token::WITH;
    
    else if (p_token.literal == "define")
        p_token.type = Token::DEFINE;
    
    return p_token;
}

void Tokenizer::set_script(ScriptLoader *p_script)
{
    script = p_script;
    tokens.clear();
}

void Tokenizer::tokenize() {
    while (!done) {
        if (_peek(0) == '\r') {
            col = 0;
            _advance();
        }

        else if (_peek(0) == '\n') {            
            // skiping multiple newlines
            if (!tokens.empty()) {
                if (tokens.at(tokens.size() - 1).type != Token::NEWLINE)
                    tokens.push_back(Token(Token::NEWLINE, col, row));
            }

            _advance();
            row++;
        }

        else if (_peek(0) == ' ') {
            // ignoring blank spaces
            _advance();
        }

        else if (_peek(0) == '#') {
            // consuming and ignoring comments

            _advance();

            while (_peek(0) != '\n') {
                _advance();
            }
        }

        else if (_peek(0) == '=') {
            tokens.push_back(Token(Token::EQUAL, col, row));
            _advance();
        }

        else if (_peek(0) == '.') {
            tokens.push_back(Token(Token::POINT, col, row));
            _advance();
        }

        else if (_peek(0) == ':') {
            tokens.push_back(Token(Token::DOUBLE_POINT, col, row));
            _advance();
        }

        else if (_peek(0) == ',') {
            tokens.push_back(Token(Token::DOUBLE_POINT, col, row));
            _advance();
        }

        else if (_peek(0) == '(') {
            tokens.push_back(Token(Token::OPEN_PARENTHESIS, col, row));
            _advance();
        }

        else if (_peek(0) == ')') {
            tokens.push_back(Token(Token::CLOSE_PARENTHESIS, col, row));
            _advance();
        }

        else if (_peek(0) == '[') {
            tokens.push_back(Token(Token::OPEN_BRACKET, col, row));
            _advance();
        }

        else if (_peek(0) == ']') {
            tokens.push_back(Token(Token::CLOSE_BRACKET, col, row));
            _advance();
        }

        else if (_peek(0) == '{') {
            tokens.push_back(Token(Token::OPEN_KEY, col, row));
            _advance();
        }

        else if (_peek(0) == '}') {
            tokens.push_back(Token(Token::CLOSE_KEY, col, row));
            _advance();
        }

        else if (_peek(0) == '$') {
            tokens.push_back(Token(Token::DOLLAR, col, row));
            _advance();
        }

        else if (_is_valid_digit_char(_peek(0))) {
            string literal;

            while (_is_valid_digit_char(_peek(0))) {
                literal += _peek(0);
                _advance();
            }

            // Is a float number?
            if (_peek(0) == '.' && _is_valid_digit_char(_peek(1))) {
                literal += _peek(0); // consuming point
                _advance();

                while (_is_valid_digit_char(_peek(0))) {
                    literal += _peek(0);
                    _advance();
                }

                tokens.push_back(Token(Token::FLOAT, col, row, literal));
            }

            else {
                tokens.push_back(Token(Token::INTEGER, col, row, literal));
            }
        }

        else if (_peek(0) == '"') {
            _advance();

            string literal;

            while (_peek(0) != '"' && _peek(0) != '\n') {
                literal += _peek(0);
                _advance();
            }

            if (_peek(0) == '"') {
                tokens.push_back(Token(Token::STRING, col, row, literal));
                _advance();
            }

            else {
                cout << "Unclosed string at line: " << row << endl;
                done = true;
            }
        }

        else if (_is_valid_identifier_name_char(_peek(0))) {
            string literal;

            while (_is_valid_identifier_name_char(_peek(0))) {
                literal += _peek(0);
                _advance();
            }

            tokens.push_back(_parse_keywords(Token(Token::IDENTIFIER, col, row, literal)));
        }

        else {
            cout << "Unexpected symbol: '" << _peek(0) << "'";
            cout << " at line " << row << " col " << col << endl;

            done = true;
        }
    }
}

void Tokenizer::debug_parsed_tokens() {
    for (size_t i = 0; i < tokens.size(); i++)
    {
        Token token = tokens.at(i);

        cout << token_names[token.type] << "(" << token.literal << ")";
        cout << " at Line " << token.row << " Col " << token.col << endl;
    }

    cout << endl;
}