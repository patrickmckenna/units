// catch unit conversion errors at compile time (zero run-time overhead)
// facilitate this with simple notation, e.g. 789_nm, 2.5_MeV -- see UnitSuffixes.hh for interfaces to use
// to do:   more units & conversions between them, e.g. nm, MeV, &c.; implementation options:
//              parameterize Units by order of mag.
//              convert everything to meters, eV, seconds, e.g. constexpr Quantity<Meter> operator""_nm(long double val) { return 1e-9 * val; }
//              create 1+ more templates, the analogue of Unit<>, and add template conversions
//          output units as well as values with output operator, e.g. print "480nm" to screen instead of just "480"
//          allow arithmetic operations btwn 2 Quantities w/ same units but diff NumTypes, e.g. Q<M,int> + Q<M,double>
//              will need to check that any cast is valid 
//          enable use of literal operators to create of Quantity types w/ 2nd template arg that's not double?
// 
// technical notes: ISO reserves for the standard library all literals that do not begin w/ an underscore, i.e. cannot have notation like 789m
//                  the only built-in numeric types that ISO allows user-defined literals to accept are long double and unsigned long long int


#include <iostream>
#include <ratio>


namespace Units 
{
    
using namespace std;


template<typename M = ratio<1,1>, typename EV = ratio<1,1>, typename S = ratio<1,1>>
struct UnitRatio {
    using Meter = M;
    using eV = EV;
    using Second = S;
};




// dimensions of meters, electron volts, seconds
// template args represent powers
template<int M, int EV, int S, typename UR = UnitRatio<>> 
struct Unit {
    enum {m = M, eV = EV, s = S};
    using Ratio = UR;
};


// type aliases for different Units
using Meter = Unit<1,0,0>;
using eV = Unit<0,1,0>;
using Second = Unit<0,0,1>;




// allow addition, subtraction of Units w/ type functions, i.e. permits creation of new types (and thus unit checks on those new types) at compile time
// needed when multiplying, dividing two unit types
template<typename U1,typename U2>
struct AddUs {
    using type = Unit<U1::m + U2::m, U1::eV + U2::eV, U1::s + U2::s>;
};


template<typename U1,typename U2>
struct SubUs {
    using type = Unit<U1::m - U2::m, U1::eV - U2::eV, U1::s - U2::s>;
};


// get shorter names for Unit type functions w/ template aliases
template<typename U1,typename U2>
using AddUnits = typename AddUs<U1,U2>::type;


template<typename U1,typename U2>
using SubtractUnits = typename SubUs<U1,U2>::type;




// Quantity: a Unit plus a value -- this is the type that users will actually use (though rarely construct explicitly)
// Rep for representation
template<typename Unit,typename Rep = double>
struct Quantity {
    explicit constexpr Quantity(Rep v) : Val{v} {}    // disallow implicit conversion of built-in types to a Quantity -- must know units
    Rep Val;
};            


// define the usual arithmetic operations
// allow only computations that make sense, e.g. can only add 2 Quantities w/ the same Unit (U) type and NumType (NT)
template<typename U,typename NT = double>
Quantity<U,NT> operator+(const Quantity<U,NT> a, const Quantity<U,NT> b)
{
    return Quantity<U,NT>{a.Val + b.Val};
}

template<typename U,typename NT = double>
Quantity<U,NT> operator-(const Quantity<U,NT> a, const Quantity<U,NT> b)
{
    return Quantity<U,NT>{a.Val - b.Val};
}

template<typename U1,typename U2,typename NT>
Quantity<AddUnits<U1,U2>,NT> operator*(const Quantity<U1,NT> a, const Quantity<U2,NT> b)
{
    return Quantity<AddUnits<U1,U2>,NT>{a.Val * b.Val};
}

template<typename U1,typename U2,typename NT>
Quantity<SubtractUnits<U1,U2>,NT> operator/(const Quantity<U1,NT> a, const Quantity<U2,NT> b)
{
    return Quantity<SubtractUnits<U1,U2>,NT>{a.Val / b.Val};
}

// allow (commutative) scalar multiplication, too
template<typename U,typename NT1 = double,typename NT2 = double>
Quantity<U,NT1> operator*(const Quantity<U,NT1> q, const NT2 scaleFactor)
{
    return Quantity<U,NT1>{q.Val * scaleFactor};
}

template<typename U,typename NT1 = double,typename NT2 = double>
Quantity<U,NT1> operator*(NT2 scaleFactor, Quantity<U,NT1> q)
{
    return Quantity<U,NT1>{q * scaleFactor};
}

template<typename U,typename NT>
constexpr bool operator==(Quantity<U,NT> q1, Quantity<U,NT> q2)
{
    return q1.Val == q2.Val;
}

template<typename U,typename NT>
constexpr bool operator!=(Quantity<U,NT> q1, Quantity<U,NT> q2)
{
    return !(q1 == q2);
}

template<typename U,typename NT>
constexpr bool operator<(Quantity<U,NT> q1, Quantity<U,NT> q2)
{
    return q1.Val < q2.Val;
}

template<typename U,typename NT>
constexpr bool operator>(Quantity<U,NT> q1, Quantity<U,NT> q2)
{
    return !(q1 < q2);
}

template<typename U,typename NT>
constexpr bool operator<=(Quantity<U,NT> q1, Quantity<U,NT> q2)
{
    return q1.Val <= q2.Val;
}

template<typename U,typename NT>
constexpr bool operator>=(Quantity<U,NT> q1, Quantity<U,NT> q2)
{
    return !(q1 <= q2);
}



string UnitSuffix(int power, string abbrev);


template<typename U,typename NT>
std::ostream& operator<<(std::ostream& os, Quantity<U,NT> q)
{
    return os << q.Val << UnitSuffix(U::m, "m") << UnitSuffix(U::eV, "eV") << UnitSuffix(U::s, "s");
}


}   // Units



