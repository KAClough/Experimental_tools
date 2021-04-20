/* GRChombo
 * Copyright 2012 The GRChombo collaboration.
 * Please refer to LICENSE in GRChombo's root directory.
 */

#ifdef CH_LANG_CC
/*
 *      _______              __
 *     / ___/ /  ___  __ _  / /  ___
 *    / /__/ _ \/ _ \/  V \/ _ \/ _ \
 *    \___/_//_/\___/_/_/_/_.__/\___/
 *    Please refer to Copyright.txt, in Chombo's root directory.
 */
#endif

#ifndef _READINPUT_H_
#define _READINPUT_H_

#include "AMRIO.H"
#include "BRMeshRefine.H"
#include "BiCGStabSolver.H"
#include "CH_HDF5.H"
#include "DebugDump.H"
#include "FABView.H"
#include "FArrayBox.H"
#include "ReadinUserVariables.hpp"
#include "LevelData.H"
#include "LoadBalance.H"
#include "MultigridUserVariables.hpp"
#include "MultilevelLinearOp.H"
#include "ParmParse.H"
#include "PoissonParameters.H"
#include "SetLevelData.H"
#include <iostream>

#ifdef CH_Linux
// Should be undefined by default
//#define TRAP_FPE
#undef TRAP_FPE
#endif

#ifdef TRAP_FPE
static void enableFpExceptions();
#endif

using std::cerr;

/// This function outputs the variable data during the solver updates
/// which helps to check convergence and debug
void read_source_data(Vector<LevelData<FArrayBox> *> &a_multigrid_vars,
                       const Vector<DisjointBoxLayout> &a_grids,
                       const PoissonParameters &a_params)
{
#ifdef CH_USE_HDF5

#if CH_SPACEDIM == 2
    string fileName("InputSource.2d.hdf5");
#else
    string fileName("InputSource.3d.hdf5");
#endif

    // set names
    Vector<string> variable_names; //(NUM_READIN_VARS);
//    for (int icomp = 0; icomp < NUM_READIN_VARS; icomp++)
//    {
//        variable_names[icomp] =
//            ReadinUserVariables::variable_names[icomp];
//    }

    // set up a temp data structure for the source data
    int max_level = a_multigrid_vars.size();
    Vector<LevelData<FArrayBox> *> temp_data; //(max_level, NULL);
//    for (int level = 0; level < max_level; level++)
//    {
//        temp_data[level] = new LevelData<FArrayBox>(a_grids[level], NUM_READIN_VARS);
//    }
    int temp_num_levels = 0;
    Real dx, dt, time;
    Vector<DisjointBoxLayout> temp_grids;
    Box temp_domain_box;
    Vector<int> temp_ref_ratio;
    ReadAMRHierarchyHDF5(fileName, temp_grids, temp_data, variable_names,
                          temp_domain_box, dx, dt, time,
                          temp_ref_ratio, temp_num_levels);

    // check the read in data matches what we expect
    CH_assert(temp_num_levels == max_level);

    // assign values to the source vars in multigrid_vars
    for (int level = 0; level < max_level; level++)
    {
        temp_data[level] = new LevelData<FArrayBox>(a_grids[level], NUM_READIN_VARS);
        Interval sourceComps(c_phi, c_Pi);
        temp_data[level]->copyTo(temp_data[level]->interval(), *a_multigrid_vars[level],
                              sourceComps);
    }

    // clean up temporary storage
    for (int level = 0; level < max_level; level++)
    {
        delete temp_data[level];
        temp_data[level] = NULL;
    }
#endif
}

#endif
