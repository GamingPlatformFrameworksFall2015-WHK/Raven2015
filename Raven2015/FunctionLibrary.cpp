#include "FunctionLibrary.h"


namespace Raven {

    // A mapping between function signatures and their string representations
    // The strings are what would show up in the ComboBox (Drop-down menu) when selecting which signature you wished to use
    std::map<FTL::Signature, std::string> FTL::signatureStrings{
        std::make_pair(Signature::s_std$Vector$oInt$c_IntPtr, "(std:vector<int>, int*)"),
        std::make_pair(Signature::s_IntRef_constChar, "(int&, const char)"),
        std::make_pair(Signature::s_ex$Entity_sf$Vector2f_sf$Vector2f, "(ex::Entity, sf::Vector2f, sf::Vector2f)")
    };

    // A mapping between function names and the wrapped functions
    std::map<std::string, Function> FL::functions{
        std::make_pair("OscillateEntity", Function(FTL::Signature::s_ex$Entity_sf$Vector2f_sf$Vector2f, 
                &[](ex::Entity entity, sf::Vector2f position1, sf::Vector2f position2) {
            nullptr; //insert code here
        })),
        std::make_pair("ResetBoxColliderDimensionsAndOrigin", Function(FTL::Signature::s_ex$Entity_sf$Vector2f_sf$Vector2f, 
                &[](ex::Entity entity, sf::Vector2f newDimensions, sf::Vector2f newOriginOffset) {
            nullptr; //insert code here
        }))
    };

}