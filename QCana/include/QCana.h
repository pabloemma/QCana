/*
 * QCana.h
 *
 *  Created on: Dec 9, 2016
 *      Author: klein
 */

//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Dec  9 19:39:16 2016 by ROOT version 6.06/08
// from TTree NMRtree/NMR
// found on file: QCV3564066681.root
//////////////////////////////////////////////////////////

#ifndef QCana_h
#define QCana_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TDatime.h>
#include <TStyle.h>  // so we can use gStyle
#include <TTimeStamp.h>
#include <TString.h>
#include <TF1.h>
#include <TMath.h>
#include <Math/MinimizerOptions.h>


// Header file for the classes stored in the TTree if any.
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <math.h>
#include <map>


using namespace std;


class QCana {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Double_t        FreqCenter;
   Double_t        FreqStep;
   Double_t        Temperature;
   Double_t        ScanPoints;
   Double_t        ScanNumber;
   Double_t        TuneV;
   Double_t        Offset;
   Double_t        ControllerV;
   Double_t        Phase_Voltage;
   Double_t        Peak_Area;
   Double_t        QcurveAmp;
   Double_t        Pol_Calib_Const;
   Double_t        Gain;
   Double_t        Pol_Sign;
   Double_t        Log_Channel;
   Double_t        Peak_Amp;
   Double_t        NMRchan;
   Double_t        PeakCenter;
   Double_t        BeamOn;
   Double_t        RFlevel;
   Double_t        IFatt;
   Double_t        HeT;
   Double_t        HeP;
   vector<double>  *array;
   Long64_t        timel;
   Int_t           IntScanPoints;

   // List of branches
   TBranch        *b_FreqCenter;   //!
   TBranch        *b_FreqStep;   //!
   TBranch        *b_Temperature;   //!
   TBranch        *b_ScanPoints;   //!
   TBranch        *b_ScanNumber;   //!
   TBranch        *b_TuneV;   //!
   TBranch        *b_Offset;   //!
   TBranch        *b_ControllerV;   //!
   TBranch        *b_Phase_Voltage;   //!
   TBranch        *b_Peak_Area;   //!
   TBranch        *b_QcurveAmp;   //!
   TBranch        *b_Pol_Calib_Const;   //!
   TBranch        *b_Gain;   //!
   TBranch        *b_Pol_Sign;   //!
   TBranch        *b_Log_Channel;   //!
   TBranch        *b_Peak_Amp;   //!
   TBranch        *b_NMRchan;   //!
   TBranch        *b_PeakCenter;   //!
   TBranch        *b_BeamOn;   //!
   TBranch        *b_RFlevel;   //!
   TBranch        *b_IFatt;   //!
   TBranch        *b_HeT;   //!
   TBranch        *b_HeP;   //!
   TBranch        *b_array;   //!
   TBranch        *b_timel;   //!
   TBranch        *b_IntScanPoints;   //!

   // ak root definitions
   TFile *f;
   TChain *QAchain;
   TTree *tree;
   TH1D *histo1;
   TCanvas *can1;
   TF1 *FitH; // Fit function
   TF1 *FitH1; // Fit function with sinh
   TString rootfilename;




   QCana();
   virtual ~QCana();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
   virtual int      OpenChain(std::vector<TString> );
   virtual int 		CreateCanvas();
   virtual int 		CreateHistos();
   virtual int		DrawHistos();
   Int_t 	OpenFile(TString);
   static Double_t FitFcn(Double_t * , Double_t *);
   static Double_t FitFcn1(Double_t * , Double_t *);  //poly and sinH
   virtual int  Fitak();


};

#endif

#ifdef QCana_cxx



QCana::QCana(){

	cout<< "Initializing Qanalysis"<<endl;



}

QCana::~QCana()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t QCana::CreateCanvas(){
	// Creates any Canvas needed
	can1 = new TCanvas("can1","Qcurve analysis",100,100,400,400);

	return 0;
}

Int_t QCana::CreateHistos(){
	// Sets up the histograms
	// very simple at this point all fixed values
	histo1 = new TH1D("histo1",rootfilename, ScanPoints,FreqCenter-((ScanPoints-1.)/2.*FreqStep),FreqCenter+((ScanPoints-1.)/2.*FreqStep));
	histo1->Sumw2();
	return 0;
}

Int_t QCana::DrawHistos(){
	// draw histograms
	can1->cd();
	//histo1->Draw("HIST P");
	gStyle->SetOptFit(1111);
	histo1->Draw();
	Fitak();
	can1->Update();
	return 0;
}
Int_t QCana::Fitak(){
	// this is a convolution fit as from example on root tutorials
	// it is a convolution of a 3deg pol and a sinh(x) function
	// set fit environment
	ROOT::Math::MinimizerOptions::SetDefaultMinimizer("Minuit","Minimize");
	ROOT::Math::MinimizerOptions::SetDefaultStrategy(2);
	ROOT::Math::MinimizerOptions::SetDefaultTolerance(.1);
	ROOT::Math::MinimizerOptions::SetDefaultPrecision(1.e-08);
	ROOT::Math::MinimizerOptions::SetDefaultMaxIterations(100000);
	ROOT::Math::MinimizerOptions::SetDefaultMaxFunctionCalls(100000);

	Int_t Npar = 3;
	FitH= new TF1("FitH",FitFcn,FreqCenter-((ScanPoints-1.)/2.*FreqStep),FreqCenter+((ScanPoints-1.)/2.*FreqStep),Npar);
	// get first estimae from root poly fit
	histo1->Fit("pol2");
	TF1 *fitroot = histo1->GetFunction("pol2");
	FitH->SetParameters(fitroot->GetParameter(0),fitroot->GetParameter(1),fitroot->GetParameter(2));
	histo1->Fit(FitH,"MR");

/*	FitH1= new TF1("FitH1",FitFcn1,FreqCenter-((ScanPoints-1.)/2.*FreqStep),FreqCenter+((ScanPoints-1.)/2.*FreqStep),Npar+2);
	FitH1->SetParameters(fitroot->GetParameter(0),fitroot->GetParameter(1),fitroot->GetParameter(2),1.,1.);
	histo1->Fit(FitH1,"MR");
	FitH1->SetLineColor(1);
    //FitH1->Draw("SAME");

     */


	// Now subtract the function from the histo






	return 0;
}

Double_t QCana::FitFcn(Double_t *x , Double_t *par){
	// start with simple polynomial

	//Double_t result = par[0]+par[1]*x[0]+par[2]*pow(x[0],2.)+par[3]*pow(x[0],3.);
	Double_t result = par[0]+par[1]*x[0]+par[2]*pow(x[0],2.);

	return result;
}
Double_t QCana::FitFcn1(Double_t *x , Double_t *par){
	// start polynomial and sinh

	//Double_t result = par[0]+par[1]*x[0]+par[2]*pow(x[0],2.)+par[3]*pow(x[0],3.);
	Double_t result = par[0]+par[1]*x[0]+par[2]*pow(x[0],2.) +par[3]*TMath::SinH(((x[0]-213.)-par[4]));
	//cout<<x[0]-213<<"  "<<par[4]<<endl;

	return result;
}




Int_t QCana::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t QCana::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void QCana::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   array = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("FreqCenter", &FreqCenter, &b_FreqCenter);
   fChain->SetBranchAddress("FreqStep", &FreqStep, &b_FreqStep);
   fChain->SetBranchAddress("Temperature", &Temperature, &b_Temperature);
   fChain->SetBranchAddress("ScanPoints", &ScanPoints, &b_ScanPoints);
   fChain->SetBranchAddress("ScanNumber", &ScanNumber, &b_ScanNumber);
   fChain->SetBranchAddress("TuneV", &TuneV, &b_TuneV);
   fChain->SetBranchAddress("Offset", &Offset, &b_Offset);
   fChain->SetBranchAddress("ControllerV", &ControllerV, &b_ControllerV);
   fChain->SetBranchAddress("Phase_Voltage", &Phase_Voltage, &b_Phase_Voltage);
   fChain->SetBranchAddress("Peak_Area", &Peak_Area, &b_Peak_Area);
   fChain->SetBranchAddress("QcurveAmp", &QcurveAmp, &b_QcurveAmp);
   fChain->SetBranchAddress("Pol_Calib_Const", &Pol_Calib_Const, &b_Pol_Calib_Const);
   fChain->SetBranchAddress("Gain", &Gain, &b_Gain);
   fChain->SetBranchAddress("Pol_Sign", &Pol_Sign, &b_Pol_Sign);
   fChain->SetBranchAddress("Log_Channel", &Log_Channel, &b_Log_Channel);
   fChain->SetBranchAddress("Peak_Amp", &Peak_Amp, &b_Peak_Amp);
   fChain->SetBranchAddress("NMRchan", &NMRchan, &b_NMRchan);
   fChain->SetBranchAddress("PeakCenter", &PeakCenter, &b_PeakCenter);
   fChain->SetBranchAddress("BeamOn", &BeamOn, &b_BeamOn);
   fChain->SetBranchAddress("RFlevel", &RFlevel, &b_RFlevel);
   fChain->SetBranchAddress("IFatt", &IFatt, &b_IFatt);
   fChain->SetBranchAddress("HeT", &HeT, &b_HeT);
   fChain->SetBranchAddress("HeP", &HeP, &b_HeP);
   fChain->SetBranchAddress("array", &array, &b_array);
   fChain->SetBranchAddress("timel", &timel, &b_timel);
   fChain->SetBranchAddress("IntScanPoints", &IntScanPoints, &b_IntScanPoints);
   Notify();
}

Bool_t QCana::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void QCana::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t QCana::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}



int QCana::OpenFile(TString rootfile){

	// oepn file and initialize tree
     rootfilename = rootfile;
     f = new TFile(rootfile);

     f->GetObject("NMRtree",tree);
     Init(tree);

   return 0;

}
int QCana::OpenChain(std::vector<TString> RootFileArray){

	// This creates a chain fo trees instead of just one

	 QAchain = new TChain("NMRtree");
	 // Now loop over all the rootfiles we have
		for(Int_t pos = 0 ; pos < RootFileArray.size() ; pos++)
		{
			cout<<"QC chain "<<RootFileArray[pos]<<"   filename \n";
			QAchain->Add(RootFileArray[pos]);

		}

    Init(QAchain);

   return 0;

}
void QCana::Loop()
{
//   In a ROOT session, you can do:
//      root> .L QCana.C
//      root> QCana t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;

      Double_t freq_temp = FreqCenter-((ScanPoints-1.)/2.*FreqStep);

      for (UInt_t j = 0; j < array->size(); ++j) {
     	  histo1->Fill(freq_temp,array->at(j));
     	  freq_temp +=FreqStep;
      }


      // if (Cut(ientry) < 0) continue;
   }




}


#endif // #ifdef QCana_cxx
