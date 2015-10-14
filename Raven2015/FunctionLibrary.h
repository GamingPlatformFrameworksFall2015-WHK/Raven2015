#pragma once

#include "Common.h"
#include "SFML/System.hpp"

namespace Raven {

    // A publicly accessible static struct maintaining a definition of function pointers and their signatures
    // Includes some sample signatures to indicate the format
    typedef struct FunctionTemplateLibrary {

        struct SigTypes {

            // A set of type definitions for the various function signatures allowed
            // Declarations must follow the given structure:
            //      typedef void(*[function name])([parameters]);
            // You can assume from this that only void-returning functions are permitted
            // Names should follow the following convention:
            //      1. Use lowerCamelCase.
            //      2. "_"-delimited words
            //      3. The name entails a list of the parameters
            //         a. Prefix the name with "s_" to indicate it is a signature by name
            //         a. Replace "&" with "Ref".
            //         b. Replace "*" with "Ptr".
            //         c. Replace "::" with "$".
            //         d. Replace "<" with "$o" for "open" | ">" with "$c" for "close"
            //         e. Use "const" normally.
            //         f. Suffix the name with "_t" to indicate it is a unique typename.
            typedef int(*s_std$Vector$oInt$c_IntPtr_t)(std::vector<int> a, int* b);
            typedef void(*s_IntRef_constChar_t)(int& a, const char b);
            typedef void(*s_ex$Entity_sf$Vector2f_sf$Vector2f_t)(ex::Entity, sf::Vector2f, sf::Vector2f);

        };

        // Enumerated type with 3-fold purpose:
        // 1. Represents a function signature
        // 2. Allows us to switch to the appropriate casting operation for the FunctionLibrary's void* function pointer storage
        // 3. Allows us to switch to the appropriate string representation of the function signature
        enum Signature {
            s_std$Vector$oInt$c_IntPtr,
            s_IntRef_constChar,
            s_ex$Entity_sf$Vector2f_sf$Vector2f
        };

        // The string representations of the various signatures, for human readability
        static std::map<Signature, std::string> signatureStrings;
    } FTL;

    struct Function {

        Function(FTL::Signature s, void* ptr = nullptr) : sig(s), fptr(ptr) {}

        void* fptr;
        FTL::Signature sig;

        template <typename sig_t>
        sig_t get() { return (sig_t)fptr; }

    };

    typedef struct FunctionLibrary
    {
        static std::map<std::string, Function> functions;

        template <typename sig>
        sig cast(Function f) {
            switch (f.sig) {
            case FTL::int_ret_std$Vector$oInt$c_IntPtr:

                break;
            }
        }
    } FL;
}

