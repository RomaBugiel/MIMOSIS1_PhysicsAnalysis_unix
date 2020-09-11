/*! @file globals.h
 *  @brief globals.h -- includes libraries, defines verbosity level of output stream.
 *	
 * 	In the documentation, only the ROOT libraries are put in hierarchy graphs.
 * 	After changing VERBOSITY LEVEL software needs to be recompiled. 
 * 
 */

#ifndef _GLOBALS_H_      
#define _GLOBALS_H_   


//cpp
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <map>
#include <memory>
#include <ostream>

//ROOT 
#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include "TSystem.h"
#include "TROOT.h"
#include "TEnv.h"
#include "TH2D.h"
#include "TMath.h"
#include "TBenchmark.h"
#include "TF1.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include <Math/MinimizerOptions.h>



 /**
 * \var VERBOSITY_LEVEL
 * \brief Verbosity level of output messages
 * 
 * <TABLE>
 * <TR><TD>Value</TD><TD>Verbosity level</TD>
 * <TR><TD>ERR</TD><TD>0</TD>
 * <TR><TD>WARN</TD><TD>1</TD>
 * <TR><TD>INFO</TD><TD>2</TD>
 * <TR><TD>CNTR</TD><TD>3</TD>
 * <TR><TD>DEB</TD><TD>4</TD>
 * </TABLE>
 */
 
const int VERBOSITY_LEVEL = 3;

#endif
