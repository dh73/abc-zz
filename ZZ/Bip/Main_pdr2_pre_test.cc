#include "Prelude.hh"
#include "Pdr2.hh"
#include "ZZ_Netlist.hh"

using namespace ZZ;

int main(int argc, char** argv)
{
    ZZ_Init;

    // Build simple netlist with one flop x holding constant 0
    Netlist N;
    Add_Pob(N, flop_init);
    Add_Pob(N, properties);

    Wire x = N.add(Flop_(0));
    flop_init(x) = l_False;  // x starts at 0
    x.set(0, x);             // x' = x

    Wire bad = N.add(PO_(0), x); // property is ~x
    properties.push(~bad);

    Params_Pdr2 P;
    Vec<Wire> props(1, bad);

    Cube s(x);
    Cube b(~x);
    Cube succ;
    Cube core = approxPreRlive(N, props, P, s, b, &succ);

    WriteLn "core size: %_", (uint) (core ? core.size() : 0);
    if (succ)
        WriteLn "succ size: %_", (uint) succ.size();
    return 0;
}
