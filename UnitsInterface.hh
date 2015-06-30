#if !defined __Units_Interface_hh__
#define __Units_Interface_hh__


// incorporate Units by using the suffixes defined below
// to do so, a user should put this using directive:
// using namespace Units;
// in any scope where they want use Units -- that will allow statements like
// auto length = 200_m;
// omitting the using directive would require a user to use fully qualified names, and thus ugly notation, e.g.
// auto length = Units::operator""_m(200);


#include <exception>

#include "cpp_utilities/Units.hh"
#include "cpp_utilities/Numerics.hh"


namespace Units
{

using namespace std;
using ULLI = unsigned long long int;
using LD = long double;
using Numerics::IsFeasibleCast;


// append one of these suffixes to a number in order to assign units
constexpr Quantity<Meter> operator"" _m(LD value);
constexpr Quantity<Meter> operator""_m(ULLI value);
constexpr Quantity<eV> operator"" _eV(LD value);
constexpr Quantity<eV> operator"" _eV(ULLI value);
constexpr Quantity<Second> operator"" _s(LD value);
constexpr Quantity<Second> operator"" _s(ULLI value);




// definitions 
constexpr Quantity<Meter> operator"" _m(LD value)
{   
    return IsFeasibleCast<LD,double>(value)?
           Quantity<Meter>{static_cast<double>(value)} : throw logic_error("!");
}

constexpr Quantity<Meter> operator""_m(ULLI value)
{
    return IsFeasibleCast<ULLI,double>(value)?
           Quantity<Meter>{static_cast<double>(value)} : throw logic_error("!");
}

constexpr Quantity<eV> operator"" _eV(LD value) 
{
    return IsFeasibleCast<LD,double>(value)?
           Quantity<eV>{static_cast<double>(value)} : throw logic_error("!");
}

constexpr Quantity<eV> operator"" _eV(ULLI value) 
{
    return IsFeasibleCast<ULLI,double>(value)?
           Quantity<eV>{static_cast<double>(value)} : throw logic_error("!");
}

constexpr Quantity<Second> operator"" _s(LD value) 
{
    return IsFeasibleCast<LD,double>(value)?
           Quantity<Second>{static_cast<double>(value)} : throw logic_error("!");
}

constexpr Quantity<Second> operator"" _s(ULLI value) 
{
    return IsFeasibleCast<ULLI,double>(value)?
           Quantity<Second>{static_cast<double>(value)} : throw logic_error("!");
}


}   // namespace Units


#endif 
// end header guard
