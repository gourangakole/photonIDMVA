#include "TROOT.h"
#include "TKey.h"
#include "TFile.h"
#include "TSystem.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TObjArray.h"
#include "THStack.h"
#include "TLegend.h"
#include "TEfficiency.h"
#include "TGraphAsymmErrors.h"
#include "TF1.h"
#include "TMath.h"
#include "TCut.h"
#include "TPaletteAxis.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <string>
#include <algorithm>


void makeTestTree(){

  string FileName = "/eos/user/k/kmondal/public/FLASHgg/PhotonIDMVA/PhaseISummer17/CMSSW_9_2_3_patch2/July24_2017/output_GJet_mass_80_Inf.root";
  string FileNameTest = "/eos/user/k/kmondal/public/FLASHgg/PhotonIDMVA/PhaseISummer17/CMSSW_9_2_3_patch2/July24_2017/output_SinglePhoton_Test.root";

  TFile fileIn("Weights_PtVSeta_Hgg_Gjets_all.root");

  TH2F *hWeightEB = (TH2F*)fileIn.Get("hWeight_bar");
  TH2F *hWeightEE = (TH2F*)fileIn.Get("hWeight_end");

  //TFile fileInrho("Weights_rho_Moriond2017_vs_ICHEP2016.root");
  //TH1F *hrho_weight = (TH1F*)fileInrho.Get("hrho_Moriond_vs_ICHEP");


  TFile *bigFile = new TFile(FileName.c_str());

  TDirectory *dir_Photon = (TDirectory*)bigFile->Get("photonViewDumper/trees");

  TTree *t_PromtPhotons = (TTree*)dir_Photon->Get("promptPhotons");
  TTree *t_FakePhotons = (TTree*)dir_Photon->Get("fakePhotons");

  float s4_bkg = 999.;
  float s4_sig = 999.;

  float rho_bkg = 999.;
  float rho_sig = 999.;

  float weight_sig = 999.;
  float weight_bkg = 999.;

  t_PromtPhotons->SetBranchAddress("s4",&s4_sig);
  t_FakePhotons->SetBranchAddress("s4",&s4_bkg);

  t_PromtPhotons->SetBranchAddress("rho",&rho_sig);
  t_FakePhotons->SetBranchAddress("rho",&rho_bkg);

  t_PromtPhotons->SetBranchAddress("weight",&weight_sig);
  t_FakePhotons->SetBranchAddress("weight",&weight_bkg);


  int sigNEvs = t_PromtPhotons->GetEntries();
  int bkgNEvs = t_FakePhotons->GetEntries();

  TFile *FileToTest = new TFile(FileNameTest.c_str(),"recreate");
  TTree *t_sig_test = t_PromtPhotons->CloneTree(0);
  TTree *t_bkg_test = t_FakePhotons->CloneTree(0);

  float PtvsEtaWeight_sig_train;
  float rhoRew_sig;
  float rhoRew_bkg;

  t_sig_test->Branch("PtvsEtaWeight",&PtvsEtaWeight_sig_train, "PtvsEtaWeight/F");
  t_sig_test->Branch("rhoRew",&rhoRew_sig, "rhoRew/F");
  t_bkg_test->Branch("rhoRew",&rhoRew_bkg, "rhoRew/F");

  //signal

  float pt;
  float eta;

  t_PromtPhotons->SetBranchAddress("pt",&pt);
  t_PromtPhotons->SetBranchAddress("scEta",&eta);

  for (Long64_t i=0;i<sigNEvs; i++) {
    t_PromtPhotons->GetEntry(i);

    double weightPtEta = 0;

    if( fabs(eta) < 1.4442 ){
      int i = hWeightEB->GetXaxis()->FindBin(pt);
      int j = hWeightEB->GetYaxis()->FindBin(fabs(eta));
      weightPtEta = hWeightEB->GetBinContent(i,j);
    }

    if( fabs(eta) > 1.566 ){
      int i = hWeightEE->GetXaxis()->FindBin(pt);
      int j = hWeightEE->GetYaxis()->FindBin(fabs(eta));
      weightPtEta = hWeightEE->GetBinContent(i,j);
    }

    //rho
    /*if(weight_sig < 0.02){
      int bin = hrho_weight->GetXaxis()->FindBin(rho_sig);
      float newRhoW = hrho_weight->GetBinContent(bin);
      //      rhoRew_sig = rho_sig*newRhoW;
      rhoRew_sig = newRhoW;
    }
    //    else rhoRew_sig = rho_sig;
    else rhoRew_sig = 1.;
    */
    //rhoRew_sig = 1.;
    PtvsEtaWeight_sig_train = weightPtEta;
    if(s4_sig > -2 && s4_sig < 2 && i%2 == 0)      t_sig_test->Fill();

  }

  t_sig_test->AutoSave();

  cout << "signal saved" << endl;

  for (Long64_t k=0;k<bkgNEvs; k++) {
    t_FakePhotons->GetEntry(k);

    /*if(weight_bkg < 0.02){
      int bin = hrho_weight->GetXaxis()->FindBin(rho_bkg);
      float newRhoW = hrho_weight->GetBinContent(bin);
      //      rhoRew_bkg = rho_bkg*newRhoW;
      rhoRew_bkg = newRhoW;
    }
    //    else rhoRew_bkg = rho_bkg;
    else rhoRew_bkg = 1;
    */
    //rhoRew_bkg = 1.;
    if(s4_bkg > -2 && s4_bkg < 2 && k%2 == 0)  t_bkg_test->Fill();

  }

  t_bkg_test->AutoSave();

  cout << "background saved" << endl;

}

