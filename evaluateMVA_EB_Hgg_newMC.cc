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

#include "TMVA/Reader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <string>
#include <algorithm>

void evaluateMVA(){

  string treeFileName = "output_SinglePhoton_Train.root";

  TFile *treeFile = new TFile(treeFileName.c_str());

  TTree *t_sig = (TTree*)treeFile->Get("promptPhotons");
  TTree *t_bkg = (TTree*)treeFile->Get("fakePhotons");

  Long64_t nEntries_sig = t_sig->GetEntries();
  Long64_t nEntries_bkg = t_bkg->GetEntries();

  string outputFileName = "mvares_Hgg_phoId_94X_EB_Isocorr.root"; // output from the programme

  // output file create
  TFile *outputFile = new TFile (outputFileName.c_str(),"recreate");
  TTree *outputTree_s = new TTree("promptPhotons","promptPhotons");
  TTree *outputTree_b = new TTree("fakePhotons","fakePhotons");

  // output vars:

  // sig:
  float out_SCRawE_s = 999.;
  float out_full_r9_s = 999.;
  float out_sigmaIetaIeta_s = 999.;
  float out_etaWidth_s = 999.;
  float out_phiWidth_s = 999.;
  float out_covIEtaIPhi_s = 999.;
  float out_s4_s = 999.;
  float out_isoPhotons_s = -999.;
  float out_isoChargedHad_s = -999.;
  float out_chgIsoWrtWorstVtx_s = -999.;
  float out_scEta_s = -999.;
  float out_rho_s = -999.;
  float out_esEffSigmaRR_s = -999.;

  float out_mvares_s = 999.;
  float out_weight_s = 999.;

  float out_E1x3_s = 999.;
  float out_E5x5_s = 999.;
  float out_E2x5_s = 999.;
  float out_EsEnergy_ov_scRawEnergy_s = 999.;
  float out_Pt_s = 999.;
  float out_Phi_s = 999.;
  float out_Eta_s = 999.;

  // bkg:
  float out_SCRawE_b = 999.;
  float out_full_r9_b = 999.;
  float out_sigmaIetaIeta_b = 999.;
  float out_etaWidth_b = 999.;
  float out_phiWidth_b = 999.;
  float out_covIEtaIPhi_b = 999.;
  float out_s4_b = 999.;
  float out_isoPhotons_b = -999.;
  float out_isoChargedHad_b = -999.;
  float out_chgIsoWrtWorstVtx_b = -999.;
  float out_scEta_b = -999.;
  float out_rho_b = -999.;
  float out_esEffSigmaRR_b = -999.;

  float out_mvares_b = 999.;
  float out_weight_b = 999.;

  float out_E1x3_b = 999.;
  float out_E5x5_b = 999.;
  float out_E2x5_b = 999.;
  float out_EsEnergy_ov_scRawEnergy_b = 999.;
  float out_Pt_b = 999.;
  float out_Phi_b = 999.;
  float out_Eta_b = 999.;

  float out_PtvsEtaWeight_s = 999.;
  float out_PtvsEtaWeight_b = 999.;

  float out_RhoRew_s = 999.;
  float out_RhoRew_b = 999.;

  int out_nvtx_s = 999.;
  int out_nvtx_b = 999.;

  float out_npu_s = 999.;
  float out_npu_b = 999.;

  float out_phoIsoCorr_s = 999.;
  float out_phoIsoCorr_b = 999.;

  // output file branches
  outputTree_s->Branch("SCRawE",&out_SCRawE_s);
  outputTree_s->Branch("r9",&out_full_r9_s);
  outputTree_s->Branch("sigmaIetaIeta",&out_sigmaIetaIeta_s);
  outputTree_s->Branch("etaWidth",&out_etaWidth_s);
  outputTree_s->Branch("phiWidth",&out_phiWidth_s);
  outputTree_s->Branch("covIEtaIPhi",&out_covIEtaIPhi_s);
  outputTree_s->Branch("s4",&out_s4_s);
  outputTree_s->Branch("phoIso03",&out_isoPhotons_s);
  //outputTree_s->Branch("phoIsoCorr", &out_phoIsoCorr_s );
  outputTree_s->Branch("chgIsoWrtChosenVtx",&out_isoChargedHad_s);
  outputTree_s->Branch("chgIsoWrtWorstVtx",&out_chgIsoWrtWorstVtx_s);
  outputTree_s->Branch("scEta",&out_scEta_s);
  outputTree_s->Branch("rho",&out_rho_s);
  outputTree_s->Branch("mvares",&out_mvares_s);
  outputTree_s->Branch("weight",&out_weight_s);
  outputTree_s->Branch("pt", &out_Pt_s );
  outputTree_s->Branch("phi", &out_Phi_s );
  outputTree_s->Branch("eta", &out_Eta_s );

  outputTree_s->Branch("PtvsEtaWeight", &out_PtvsEtaWeight_s );
  // outputTree_s->Branch("rhoRew", &out_RhoRew_s );
  outputTree_s->Branch("nvtx", &out_nvtx_s );
  outputTree_s->Branch("npu", &out_npu_s );


  outputTree_b->Branch("SCRawE",&out_SCRawE_b);
  outputTree_b->Branch("r9",&out_full_r9_b);
  outputTree_b->Branch("sigmaIetaIeta",&out_sigmaIetaIeta_b);
  outputTree_b->Branch("etaWidth",&out_etaWidth_b);
  outputTree_b->Branch("phiWidth",&out_phiWidth_b);
  outputTree_b->Branch("covIEtaIPhi",&out_covIEtaIPhi_b);
  outputTree_b->Branch("s4",&out_s4_b);
  outputTree_b->Branch("phoIso03",&out_isoPhotons_b);
  //outputTree_b->Branch("phoIsoCorr", &out_phoIsoCorr_b );
  outputTree_b->Branch("chgIsoWrtChosenVtx",&out_isoChargedHad_b);
  outputTree_b->Branch("chgIsoWrtWorstVtx",&out_chgIsoWrtWorstVtx_b);
  outputTree_b->Branch("scEta",&out_scEta_b);
  outputTree_b->Branch("rho",&out_rho_b);
  outputTree_b->Branch("mvares",&out_mvares_b);
  outputTree_b->Branch("weight",&out_weight_b);
  outputTree_b->Branch("pt", &out_Pt_b );
  outputTree_b->Branch("phi", &out_Phi_b );
  outputTree_b->Branch("eta", &out_Eta_b );

  outputTree_b->Branch("PtvsEtaWeight", &out_PtvsEtaWeight_b );
  // outputTree_b->Branch("rhoRew", &out_RhoRew_b );
  outputTree_b->Branch("nvtx", &out_nvtx_b );
  outputTree_b->Branch("npu", &out_npu_b );

  TMVA::Reader *photonIdMva_sig_ = new TMVA::Reader("!Color");
  TMVA::Reader *photonIdMva_bkg_ = new TMVA::Reader("!Color");

  photonIdMva_sig_->AddVariable("SCRawE", &out_SCRawE_s );
  photonIdMva_sig_->AddVariable("r9",&out_full_r9_s);
  photonIdMva_sig_->AddVariable("sigmaIetaIeta",&out_sigmaIetaIeta_s);
  photonIdMva_sig_->AddVariable("etaWidth",&out_etaWidth_s);
  photonIdMva_sig_->AddVariable("phiWidth",&out_phiWidth_s);
  photonIdMva_sig_->AddVariable("covIEtaIPhi",&out_covIEtaIPhi_s);
  photonIdMva_sig_->AddVariable("s4",&out_s4_s);
  photonIdMva_sig_->AddVariable("phoIso03",&out_isoPhotons_s);
  //photonIdMva_sig_->AddVariable("phoIsoCorr",&out_phoIsoCorr_s);
  photonIdMva_sig_->AddVariable("chgIsoWrtChosenVtx",&out_isoChargedHad_s);
  photonIdMva_sig_->AddVariable("chgIsoWrtWorstVtx",&out_chgIsoWrtWorstVtx_s);
  photonIdMva_sig_->AddVariable("scEta",&out_scEta_s);
  photonIdMva_sig_->AddVariable("rho",&out_rho_s);



  t_sig->SetBranchAddress("SCRawE",&out_SCRawE_s);
  t_sig->SetBranchAddress("r9",&out_full_r9_s);
  t_sig->SetBranchAddress("sigmaIetaIeta",&out_sigmaIetaIeta_s);
  t_sig->SetBranchAddress("etaWidth",&out_etaWidth_s);
  t_sig->SetBranchAddress("phiWidth",&out_phiWidth_s);
  t_sig->SetBranchAddress("covIEtaIPhi",&out_covIEtaIPhi_s);
  t_sig->SetBranchAddress("s4",&out_s4_s);
  t_sig->SetBranchAddress("phoIso03",&out_isoPhotons_s);
  //t_sig->SetBranchAddress("phoIsoCorr",&out_phoIsoCorr_s);
  t_sig->SetBranchAddress("chgIsoWrtChosenVtx",&out_isoChargedHad_s);
  t_sig->SetBranchAddress("chgIsoWrtWorstVtx",&out_chgIsoWrtWorstVtx_s);
  t_sig->SetBranchAddress("scEta",&out_scEta_s);
  t_sig->SetBranchAddress("rho",&out_rho_s);
  t_sig->SetBranchAddress("weight",&out_weight_s);
  t_sig->SetBranchAddress("pt", &out_Pt_s );
  t_sig->SetBranchAddress("phi", &out_Phi_s );
  t_sig->SetBranchAddress("eta", &out_Eta_s );
  t_sig->SetBranchAddress("PtvsEtaWeight", &out_PtvsEtaWeight_s );
  // t_sig->SetBranchAddress("rhoRew", &out_RhoRew_s );
  t_sig->SetBranchAddress("nvtx", &out_nvtx_s );
  t_sig->SetBranchAddress("npu", &out_npu_s );


  photonIdMva_bkg_->AddVariable("SCRawE", &out_SCRawE_b );
  photonIdMva_bkg_->AddVariable("r9",&out_full_r9_b);
  photonIdMva_bkg_->AddVariable("sigmaIetaIeta",&out_sigmaIetaIeta_b);
  photonIdMva_bkg_->AddVariable("etaWidth",&out_etaWidth_b);
  photonIdMva_bkg_->AddVariable("phiWidth",&out_phiWidth_b);
  photonIdMva_bkg_->AddVariable("covIEtaIPhi",&out_covIEtaIPhi_b);
  photonIdMva_bkg_->AddVariable("s4",&out_s4_b);
  photonIdMva_bkg_->AddVariable("phoIso03",&out_isoPhotons_b);
  //photonIdMva_bkg_->AddVariable("phoIsoCorr",&out_phoIsoCorr_b);
  photonIdMva_bkg_->AddVariable("chgIsoWrtChosenVtx",&out_isoChargedHad_b);
  photonIdMva_bkg_->AddVariable("chgIsoWrtWorstVtx",&out_chgIsoWrtWorstVtx_b);
  photonIdMva_bkg_->AddVariable("scEta",&out_scEta_b);
  photonIdMva_bkg_->AddVariable("rho",&out_rho_b);



  t_bkg->SetBranchAddress("SCRawE",&out_SCRawE_b);
  t_bkg->SetBranchAddress("r9",&out_full_r9_b);
  t_bkg->SetBranchAddress("sigmaIetaIeta",&out_sigmaIetaIeta_b);
  t_bkg->SetBranchAddress("etaWidth",&out_etaWidth_b);
  t_bkg->SetBranchAddress("phiWidth",&out_phiWidth_b);
  t_bkg->SetBranchAddress("covIEtaIPhi",&out_covIEtaIPhi_b);
  t_bkg->SetBranchAddress("s4",&out_s4_b);
  t_bkg->SetBranchAddress("phoIso03",&out_isoPhotons_b);
  //t_bkg->SetBranchAddress("phoIsoCorr",&out_phoIsoCorr_b);
  t_bkg->SetBranchAddress("chgIsoWrtChosenVtx",&out_isoChargedHad_b);
  t_bkg->SetBranchAddress("chgIsoWrtWorstVtx",&out_chgIsoWrtWorstVtx_b);
  t_bkg->SetBranchAddress("scEta",&out_scEta_b);
  t_bkg->SetBranchAddress("rho",&out_rho_b);
  t_bkg->SetBranchAddress("weight",&out_weight_b);
  t_bkg->SetBranchAddress("pt", &out_Pt_b );
  t_bkg->SetBranchAddress("phi", &out_Phi_b );
  t_bkg->SetBranchAddress("eta", &out_Eta_b );
  // t_bkg->SetBranchAddress("PtvsEtaWeight", &out_PtvsEtaWeight_b );
  // t_bkg->SetBranchAddress("rhoRew", &out_RhoRew_b );

  t_bkg->SetBranchAddress("nvtx", &out_nvtx_b );
  t_bkg->SetBranchAddress("npu", &out_npu_b );


  photonIdMva_sig_->BookMVA( "BDT", "dataset/weights/HggPhoId_94X_barrel_BDT.weights.xml");
  photonIdMva_bkg_->BookMVA( "BDT", "dataset/weights/HggPhoId_94X_barrel_BDT.weights.xml");

  for(int i = 0; i < nEntries_sig; i++){
    t_sig->GetEntry(i);
    out_mvares_s = photonIdMva_sig_->EvaluateMVA( "BDT" );
    if(abs(out_scEta_s)<1.5) outputTree_s->Fill();
  }

  cout <<"signal evaluated" << endl;

  for(int i = 0; i < nEntries_bkg; i++){
    t_bkg->GetEntry(i);
    out_mvares_b = photonIdMva_bkg_->EvaluateMVA( "BDT" );
    if(abs(out_scEta_b)<1.5) outputTree_b->Fill();
  }

  outputFile->Write();

}

