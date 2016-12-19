/*
 * QCana_main.h
 *
 *  Created on: Dec 9, 2016
 *      Author: klein
 */

/*
 * QCana_main.h
 *
 *  Created on: May 10, 2016
 *      Author: klein
 */

#ifndef QCANA_MAIN_H_
#define QCANA_MAIN_H_

TString InputRootDirectory ; // the directory where the root input file is located
TString root_infile; // the current root file
//TStringQCfilename ; // gives the QCurve filename

std::vector<TString> InputSignalFile ; // if there is a list of input files it will put them into vector
std::string QCfilename ; // gives the QCurve filename

std::string  NMR_ROOT ; // top directory of NMR system, needs to be defined thorugh enviro variable $NMR_ROOT

std::string QC_pr = "QC_ana> ";












#endif /* QCANA_MAIN_H_ */
