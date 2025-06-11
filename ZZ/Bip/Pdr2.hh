//_________________________________________________________________________________________________
//|                                                                                      -- INFO --
//| Name        : Pdr2.hh
//| Author(s)   : Niklas Een
//| Module      : Bip
//| Description : PDR with learned cubes over arbitrary internal variables.
//| 
//| (C) Copyright 2010-2014, The Regents of the University of California
//|________________________________________________________________________________________________
//|                                                                                  -- COMMENTS --
//| 
//|________________________________________________________________________________________________

#ifndef ZZ__Bip__Pdr2_hh
#define ZZ__Bip__Pdr2_hh

#include "ZZ_CmdLine.hh"
#include "ZZ_Netlist.hh"
#include "ZZ_Bip.Common.hh"

namespace ZZ {
using namespace std;


//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm


struct Params_Pdr2 {
    enum Recycling { NONE, SOLVER, VARS };
    enum PobCone   { JUST, PSEUDO_JUST, COI };
    enum JustStrat { FIRST, LOWEST_LEVEL, ACTIVITY };

    Recycling   recycling;
    PobCone     pob_cone;
    bool        pob_internals;
    bool        pob_weaken;
    bool        pob_rotate;
    JustStrat   just_strat;
    bool        use_activity;
    bool        randomize;
    bool        restarts;
    uint        gen_orbits;
    bool        tweak_cut;
    SolverType  sat_solver;
    bool        prop_init;
    bool        check_klive;
    bool        par_send_result;        // -- place holder; Pdr2 doesn't support PAR yet
    uint        rlive_limit;            // -- max recursion depth for rlive DFS

    Params_Pdr2() :
        recycling    (SOLVER),
        pob_cone     (JUST),
        pob_internals(false),
        pob_weaken   (true),
        pob_rotate   (false),
        just_strat   (FIRST),
        use_activity (true),
        randomize    (false),
        restarts     (false),
        gen_orbits   (2),
        tweak_cut    (false),
        sat_solver   (sat_Abc),
        prop_init    (false),
        check_klive  (false),
        par_send_result(true),
        rlive_limit(3)        // default recursion limit for rlive
    {}
};


void addCli_Pdr2(CLI& cli);
void setParams(const CLI& cli, Params_Pdr2& P);


bool pdr2(NetlistRef N, const Vec<Wire>& props, const Params_Pdr2& P, Cex* cex, NetlistRef N_invar);
bool pdr2Constrained(NetlistRef N, const Vec<Wire>& props, const Params_Pdr2& P,
                     const Vec<Cube>& blocks, Cex* cex, NetlistRef N_invar);

// Convenience wrapper that initializes a Pdr2 engine and queries the over-approximate
// predecessor core for cube 'b' relative to state cube 's'.
Cube approxPreRlive(NetlistRef N, const Vec<Wire>& props, const Params_Pdr2& P,
                    const Vec<Cube>& blocks,
                    const Cube& s, const Cube& b, Cube* succ = NULL);

// Convenience wrapper that detects if state 's' is dead (has no successors)
// using the SAT solver. If unsat, a subset of 's' is returned and stored in
// frame 0 as a blocking cube.
Cube pruneDeadRlive(NetlistRef N, const Vec<Wire>& props, const Params_Pdr2& P,
                    const Vec<Cube>& blocks, const Cube& s);

// Bounded DFS exploration using 'approxPre' and 'pruneDead'. Returns TRUE if a
// cycle reachable within 'P.rlive_limit' is detected starting from 's'.
bool dfsExploreRlive(NetlistRef N, const Vec<Wire>& props, const Params_Pdr2& P,
                     const Vec<Cube>& blocks, const Cube& s);

// Full rlive implementation based on Algorithm 2. Returns TRUE if the property
// holds, FALSE if a lasso-shaped counterexample is found.
bool rlive(NetlistRef N, const Vec<Wire>& props, const Params_Pdr2& P);

// Convenience wrapper that initializes a Pdr2 engine and queries the over-approximate
// predecessor core for cube 'b' relative to state cube 's'.
Cube approxPreRlive(NetlistRef N, const Vec<Wire>& props, const Params_Pdr2& P,
                    const Cube& s, const Cube& b, Cube* succ = NULL);


//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
}
#endif
