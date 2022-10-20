#include "ErrorStack.h"

using namespace df;
using namespace std;

ErrorStack::ErrorStack(string p_stack_name) {
    _stack_name = p_stack_name;
}

ErrorStack::~ErrorStack() {

}

void ErrorStack::push_error(Error p_error) {
    _stack.push_back(p_error);
}

const void ErrorStack::debug_trace_back() {
    cout << _stack_name << " error trace back: " << endl;

    for (Error const &error : _stack)
    {
        cout << error.type << ": ";
        cout << error.message;

        if (error.col > 0 && error.row > 0)
            cout << " at Line " << error.col << " Col " << error.row;
        
        cout << endl;
    }    
}