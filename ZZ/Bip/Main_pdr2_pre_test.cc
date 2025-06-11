#include "Prelude.hh"
#include "Pdr2.hh"
#include "ZZ_Netlist.hh"

using namespace ZZ;

int main(int argc, char** argv)
{
    ZZ_Init;

    // Build an 8-bit counter "cnt" starting at 0 and incrementing by one.
    Netlist N;
    Add_Pob(N, flop_init);
    Add_Pob(N, properties);

    Wire cnt[8];
    for (uint i = 0; i < 8; i++){
        cnt[i] = N.add(Flop_(i));
        flop_init(cnt[i]) = l_False; // counter starts at 0
    }

    Wire carry = N.True();
    for (uint i = 0; i < 8; i++){
        Wire next = cnt[i] ^ carry;
        carry = N.add(And_(), cnt[i], carry);
        cnt[i].set(0, next);        // cnt[i]' = next
    }

    // Property is ~cnt[7]; it is initially true and violated when MSB becomes 1.
    Wire bad = N.add(PO_(0), cnt[7]);
    properties.push(~bad);

    Params_Pdr2 P;
    Vec<Wire> props(1, bad);

    // Build cube for state cnt == 0.
    Vec<GLit> s_vec;
    for (uint i = 0; i < 8; i++)
        s_vec.push(~cnt[i]);
    Cube s(s_vec);

    // Ask for predecessor core of cube {cnt[0]'} wrt state s.
    Cube b(cnt[0]);
    Cube succ;
    Cube core = approxPreRlive(N, props, P, s, b, &succ);

    WriteLn "core size: %_", (uint) (core ? core.size() : 0);
    if (succ)
        WriteLn "succ size: %_", (uint) succ.size();
    return 0;
}
