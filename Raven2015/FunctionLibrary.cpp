#include "FunctionLibrary.h"


namespace Raven {

    std::map<FTL::Signature, std::string> FTL::signatureStrings{
        std::make_pair(Signature::s_std$Vector$oInt$c_IntPtr, "(std:vector<int>, int*) -> int"),
        std::make_pair(Signature::s_IntRef_constChar, "(int&, const char) -> void"),
        std::make_pair(Signature::s_ex$Entity_sf$Vector2f_sf$Vector2f, "(ex::Entity, sf::Vector2f, sf::Vector2f) -> void")
    };



}