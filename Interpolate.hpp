#include <sstream>
#include <iostream>
#include <stdexcept>

namespace cs540{

    class WrongNumberOfArgs : public std::length_error{
        public:
            WrongNumberOfArgs()
                : std::length_error{"Wrong Number of Args"}
            { }
    };

    std::string InterpolateHelper(const char *str, std::stringstream& ss){
        while (*str){
            if (*str == '%'){
                throw WrongNumberOfArgs();
            }
            if ((*str == '\\') && (*(str+1) == '%')){
                ++str;
            }
            ss << *str++;
        }
        return ss.str();
    }

    template <typename T, typename... Ts>
    std::string InterpolateHelper(const char *str, std::stringstream& ss, T &&val, 
                                            Ts&&... ts){
        while (*str){
            if (*str == '%'){
                ss << val;
                break;
            }
            if ((*str == '\\') && (*(str+1) == '%')){
                ++str;
            }
            ss << *str++;
        }
        if (*str == '%'){
            return InterpolateHelper(str + 1, ss, ts...);
        }
        else{
            throw WrongNumberOfArgs();
        }
    }

    template <typename ...Ts>
    std::string Interpolate(const char *str, Ts&&...args){
        std::stringstream ss;
        return InterpolateHelper(str, ss, args...);
    }
}

