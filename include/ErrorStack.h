#ifndef DF_ERRORSTACK_H
#define DF_ERRORSTACK_H

#include <string>
#include <vector>
#include <iostream>

namespace df {
    class ErrorStack {
    public:
        struct Error {
            enum Type {
                EMPTY,
                IO_ERROR,
                PARSE_ERROR,
                ERROR_TYPE_MAX
            };

            Type type = EMPTY;
            std::string message;
            uint32_t col = 0, row = 0;

            Error(std::string p_message, uint32_t p_col, uint32_t p_row) {
                message = p_message;
                col = p_col;
                row = p_row;
            }

            Error(std::string p_message) {
                message = p_message;
            }

            Error() {
                message = "Unknown error";
            }
        };

    private:
        std::string _stack_name;
        std::vector<Error> _stack;
    
    public:
        inline const bool empty() const { return !(_stack.size() > 0); }
        inline const std::vector<Error> get_error_stack() const { return _stack; }

        void push_error(Error p_error);
        void debug_trace_back();

        ErrorStack(std::string p_stack_name);
        ~ErrorStack();
    };
}

#endif //DF_ERRORSTACK_H