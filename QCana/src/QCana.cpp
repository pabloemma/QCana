//============================================================================
// Name        : QCana.cpp
// Author      : andi klein
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================


#define QCana_cxx



#include <unistd.h>  // to get gnu getoption
#include <ctime> // to manipulate time
#include <stdlib.h>
#include  <vector>
#include <string>

#include <iostream>
#include "QCana.h"
#include "QCana_main.h"



#include <TStyle.h>

#include <TObjArray.h>
#include <TApplication.h>

#ifdef __APPLE__
 std::string OS = "OSX";
#elif __linux__
 std::string OS = "OSX";
 #elif _WIN64
 std::string OS = "WIN";
 // add windows includes
#elif _WIN32
 std::string OS = "WIN";
 // add windows includes
#endif


QCana QA;  // initialze class
using namespace std;



#ifdef __APPLE__
int main(Int_t argc,char **argv) {
#endif

#ifdef __linux__
int main(Int_t argc,char **argv) {
#endif

#ifdef __WIN64
int main(Int_t argc,char *argv[],char *envp[] ) {
#endif

#ifdef __WIN32
int main(Int_t argc,char *argv[],char *envp[] ) {
#endif






	TApplication *theApp = new TApplication("theApp",&argc,argv);  // problem with the two commandline args getting in conflict
	theApp->GetOptions(&argc,argv);

	for (Int_t k=0;k < theApp->Argc();k++){  // find the filenames
	TString temp = std::string(theApp->Argv(k));  //Argv with index is char*

	if(temp.Contains(".root"))
	{
			InputSignalFile.push_back(InputRootDirectory+temp);
	}
	}

	if(InputSignalFile.size()>1){
		QA.OpenChain(InputSignalFile);  // we will create a TChain
	}
	else {
		QA.OpenFile(InputSignalFile[0]); // just one spectrum
	}

    QA.CreateCanvas();
    QA.CreateHistos();
    QA.Loop();
    QA.DrawHistos();

    theApp->Run();

    theApp->Terminate();


return 0;
} //end main

