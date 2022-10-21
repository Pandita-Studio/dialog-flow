#include "Tokenizer.h"

using namespace df;
using namespace std;

static_assert(sizeof(token_names) / sizeof(token_names[0]) == Tokenizer::Token::TK_MAX, "Amount of token names don't match the amount of token types.");

Tokenizer::Tokenizer() {
    errors = new ErrorStack("DialogFlow Tokenizer");
    cursor = 0;
    row = 1;
    col = 1;
}

Tokenizer::~Tokenizer() {
    delete errors;
}

void Tokenizer::_advance() {
    col++;
    cursor++;
}

const char Tokenizer::_peek(int p_position) const {
    if (p_position == -1 && cursor == 0)
        return START_OF_TEXT;
    
    else if (
            (p_position == 1 && (cursor + 1) == script->get_source_size())
            || (p_position == 0 && cursor == script->get_source_size()))
        return END_OF_TEXT;
    
    else
        return script->get_source_code()[cursor + p_position];
}

const bool Tokenizer::_is_valid_digit_char(char symbol) const {
    uint32_t ascii = symbol;

    if (ascii >= 48 && ascii <= 57)
        return true;
    
    return false;
}

const bool Tokenizer::_is_valid_identifier_name_char(char symbol) const {
    uint32_t ascii = symbol;

    if (symbol == '_')
        return true;

    else if (ascii >= 48 && ascii <= 57)
        return true;

    else if ((ascii >= 65 && ascii <= 90) || (ascii >= 97 && ascii <= 122))
        return true;   

    return false; 
}

const Tokenizer::Token Tokenizer::_parse_keywords(Token p_token) const {
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
    while (true) {
        if (_peek(0) == END_OF_TEXT) {
            tokens.push_back(Token(Token::TK_EOF, col, row));
            break;
        }

        else if (_peek(0) == '\r') {
            // ignoring cret
            _advance();
            col--;
        }

        else if (_peek(0) == '\n') {            
            // skiping multiple newlines
            if (!tokens.empty()) {
                if (tokens.at(tokens.size() - 1).type != Token::NEWLINE)
                    tokens.push_back(Token(Token::NEWLINE, col, row));
            }

            _advance();
            col = 0;
            row++;
        }

        else if (_peek(0) == ' ') {
            if (_peek(-1) == '\n') {
                // consuming space tabulations
                uint32_t space_counter = 0;
                Token token(Token::TAB, col, row);

                while (_peek(0) == ' ')
                {
                    _advance();
                    space_counter++;
                }

                token.literal = to_string(space_counter); // ToDo: Optimize this
                tokens.push_back(token);
            }

            else {
                _advance();
            }
        }

        else if (_peek(0) == '#') {
            // consuming and ignoring comments

            _advance();

            while (_peek(0) != '\n' && _peek(0) != END_OF_TEXT) {
                _advance();
                col--;
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
            Token token(Token::INTEGER, col, row);

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

                token.type = Token::FLOAT;
                token.literal = literal;
                tokens.push_back(token);
            }

            else {
                token.literal = literal;
                tokens.push_back(token);
            }
        }

        else if (_peek(0) == '"') {
            _advance();

            string literal;
            Token token(Token::STRING, col, row);

            while (_peek(0) != '"' && _peek(0) != '\n' && _peek(0) != END_OF_TEXT) {
                literal += _peek(0);
                _advance();
            }

            if (_peek(0) == '"') {
                token.literal = literal;
                tokens.push_back(token);
                _advance();
            }

            else {
                errors->push_error(ErrorStack::Error("Unclosed string", col, row));
                break;
            }
        }

        else if (_is_valid_identifier_name_char(_peek(0))) {
            string literal;
            Token token(Token::IDENTIFIER, col, row);

            while (_is_valid_identifier_name_char(_peek(0))) {
                literal += _peek(0);
                _advance();
            }

            token.literal = literal;
            tokens.push_back(_parse_keywords(token));
        }

        else {
            errors->push_error(ErrorStack::Error("Unexpected symbol: '" + to_string(_peek(0)) + "'", col, row));
            break;
        }
    }

    cursor = 0;
}

const Tokenizer::Token Tokenizer::scan(int p_position) const {
    if (tokens.empty()) {
        errors->push_error(ErrorStack::Error("Scaning from an empty tokens stack"));
        return Token();
    }

    if (p_position == -1 && cursor == 0)
        return Token(Token::TK_SOF);

    else if (p_position == 1 && (cursor + 1) == tokens.size()
            || p_position == 0 && cursor == tokens.size())
        return Token(Token::TK_EOF);
    
    else
        return tokens.at(cursor + p_position);
}

void Tokenizer::consume() {
    cursor++;
}