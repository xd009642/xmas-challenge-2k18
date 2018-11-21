#ifndef SYMBOLS_BASE_SYMBOL_H
#define SYMBOLS_BASE_SYMBOL_H

namespace sym {
    
    //! Every symbol must expose a render function
    class base_symbol {
    public:
        //! Graphical symbols may want to be initialised at a set point
        //! mainly after GL context is bought up
        virtual void init() = 0;
        //! render function
        virtual void render() = 0;
    };
}


#endif
