#include "cpp_utilities/Units_Interface.hh"


namespace Units {
using namespace std;


string UnitSuffix(int power, string abbrev)
{
    string suf;
    if (power)
    {
        suf = abbrev;
        if (power != 1)
            suf += "^" + to_string(power);
    }
    return suf;
}


}