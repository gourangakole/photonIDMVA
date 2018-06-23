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

void plotMVARes(){

  string fileNames[10];

  //fileNames[0] = "/eos/user/k/kmondal/public/FLASHgg/PhotonIDMVA/RunIIFall17/MCv2_February2018/February03/Mass95/mvares_Hgg_phoId_94X_EB_woIsocorr.root";
  fileNames[0] = "mvares_Hgg_phoId_94X_EB_Isocorr.root";
  //  fileNames[1] = "/eos/user/k/kmondal/public/FLASHgg/PhotonIDMVA/RunIIFall17/MCv2_February2018/February03/Mass95/mvares_Hgg_phoId_92X_EB_woIsocorr.root";
  //  fileNames[2] = "/eos/user/k/kmondal/public/FLASHgg/PhotonIDMVA/RunIIFall17/MCv2_February2018/February03/Mass95/mvares_Hgg_phoId_80X_EB_Moriond17.root";

  TCanvas * can = new TCanvas("can_mvares","can_mvares",600,600);
  string label_mvares = "mva output";

  TLegend *legend = new TLegend(0.35,0.55,0.8,0.85,"","brNDC");
  legend->SetHeader("PhoId, EB :");
  legend->SetBorderSize(0);
  legend->SetFillStyle(0);
  legend->SetTextFont(42);

  string labelLeg_s[10];

  labelLeg_s[0] = "Sig: 9_4_X";
  labelLeg_s[1] = "Sig: 9_2_X";
  labelLeg_s[2] = "Sig: 8_0_X";

  string labelLeg_b[10];

  labelLeg_b[0] = "Bkg: 9_4_X";
  labelLeg_b[1] = "Bkg: 9_2_X";
  labelLeg_b[2] = "Bkg: 8_0_X";


  TCanvas * can_RoC = new TCanvas ("can_RoC","can_RoC",600,600);

  TLegend *legend_RoC = new TLegend(0.2,0.5,0.6,0.90,"","brNDC");
  legend_RoC->SetHeader("PhoId, EB :");
  legend_RoC->SetBorderSize(0);
  legend_RoC->SetFillStyle(0);
  legend_RoC->SetTextFont(42);

  string labelLeg_RoC[10];  
  labelLeg_RoC[0] = "9_4_X";
  labelLeg_RoC[1] = "9_2_X";
  labelLeg_RoC[2] = "8_0_X";


  for(int i = 0; i < 1; i++){

    cout << "file # " << i << endl;

    TFile *mvaResFile = new TFile(fileNames[i].c_str());

    TTree *t_output_s = (TTree*)mvaResFile->Get("promptPhotons");
    TTree *t_output_b = (TTree*)mvaResFile->Get("fakePhotons");

    TH1F * histo_s = new TH1F ("histo_s","histo_s",100,-1,1);
    TH1F * histo_b = new TH1F ("histo_b","histo_b",100,-1,1);

    TString tmp_s = "";
    TString tmp_b  = "";

    tmp_s = "mvares";
    tmp_s+=">>histo_s";
    
    tmp_b = "mvares";
    tmp_b+=">>histo_b";

    t_output_s->Draw(tmp_s,"(abs(scEta)<1.5)*weight","goff");
    t_output_b->Draw(tmp_b,"(abs(scEta)<1.5)*weight","goff");

    
    float Nsig[320], Nbkg[320];
    float sigEff[320], bkgEff[320];
    float cutsVal[320];
    float mvaResCutVal = -1.00625;

    int nCuts = 320; // 0.00625*320 = 2 (so it is covered -1 to +1)

    int mvaSMaxBin = histo_s->GetXaxis()->FindBin(1);
    int mvaBMaxBin = histo_b->GetXaxis()->FindBin(1);

    for(int k = 0; k < nCuts; k++){

      mvaResCutVal+= 0.00625;
      cutsVal[k] = mvaResCutVal;

      int mvaBin = histo_s->GetXaxis()->FindBin(mvaResCutVal);
      Nsig[k] = histo_s->Integral(mvaBin,mvaSMaxBin);

      int mvaBin_b = histo_b->GetXaxis()->FindBin(mvaResCutVal);
      Nbkg[k] = histo_b->Integral(mvaBin_b,mvaBMaxBin);
      sigEff[k] = Nsig[k]/Nsig[0];
      bkgEff[k] = Nbkg[k]/Nbkg[0];

      // be keep in mind that the way efficiency is canculated
      // when mva value increases the efficiency decreases (because it is always devided by the integral)
      // if(sigEff[k] > 0.98) cout << " sigEff[k] = " << sigEff[k] <<  " bkgEff[k] = " << bkgEff[k] << " with a cut at " << mvaResCutVal << endl;
      if(sigEff[k] > 0.895 && sigEff[k] < 0.905) cout << " sigEff[k] = " << sigEff[k] <<  " bkgEff[k] = " << bkgEff[k] << " with a cut at " << mvaResCutVal << endl;

    }

    TGraph * sigEff_vs_cut = new TGraph (nCuts, cutsVal, sigEff);
    TGraph * bkgEff_vs_cut = new TGraph (nCuts, cutsVal, bkgEff);
    TGraph * sigEff_vs_bkgEff = new TGraph (nCuts, sigEff, bkgEff);

    // draw mvares
    can->cd();
    can->SetLogy();

    histo_s->SetTitle("");
    histo_s->SetStats(0);
    histo_s->GetXaxis()->SetTitle(label_mvares.c_str());
    histo_s->SetMaximum(histo_s->GetBinContent(histo_s->GetMaximumBin())*1000);
       
    // histo_s->SetMarkerColor(i+2);
    // histo_s->SetLineColor(i+2);
    histo_s->SetLineWidth(2);

    histo_b->SetLineStyle(2);
    // histo_b->SetMarkerColor(i+2);
    // histo_b->SetLineColor(i+2);
    histo_b->SetLineWidth(2);

    if(i == 0) {
      histo_s->SetLineColor(kRed);
      histo_s->SetMarkerColor(kRed);
      histo_b->SetLineColor(kBlue);
      histo_b->SetMarkerColor(kBlue);
      
    }
    else if(i == 2){
      histo_s->SetLineColor(kBlue);
      histo_s->SetMarkerColor(kBlue);
      histo_b->SetLineColor(kBlue);
      histo_b->SetMarkerColor(kBlue);
    }
    else if(i == 1){
      histo_s->SetLineColor(kGreen+2);
      histo_s->SetMarkerColor(kGreen+2);
      histo_b->SetLineColor(kGreen+2);
      histo_b->SetMarkerColor(kGreen+2);
    }
    else{
      histo_s->SetLineColor(i+2);
      histo_s->SetMarkerColor(i+2);
      histo_b->SetLineColor(i+2);
      histo_b->SetMarkerColor(i+2);
    
    }

    legend->AddEntry(histo_s,labelLeg_s[i].c_str(),"lem");
    legend->AddEntry(histo_b,labelLeg_b[i].c_str(),"lem");

        
    if(i == 0){
      histo_s->Draw();
    }
    else histo_s->Draw("same");
    legend->Draw("same");
    histo_b->Draw("same");

    can->Update();
    can->Modified();

    // draw RoC curves 

    can_RoC->cd();

    if(i == 0){
      sigEff_vs_bkgEff->SetTitle("");
      sigEff_vs_bkgEff->GetYaxis()->SetTitleOffset(1.5);
      sigEff_vs_bkgEff->GetYaxis()->SetTitleSize(0.03);
      sigEff_vs_bkgEff->GetYaxis()->SetLabelSize(0.02);
      sigEff_vs_bkgEff->GetXaxis()->SetTitleOffset(1.5);
      sigEff_vs_bkgEff->GetXaxis()->SetTitleSize(0.03);
      sigEff_vs_bkgEff->GetXaxis()->SetLabelSize(0.02);

      
      sigEff_vs_bkgEff->GetXaxis()->SetRangeUser(0.0,1.);
      sigEff_vs_bkgEff->GetYaxis()->SetRangeUser(0.,1.0);
      

      sigEff_vs_bkgEff->GetXaxis()->SetTitle("signal eff");
      sigEff_vs_bkgEff->GetYaxis()->SetTitle("bkg eff");

    }

    
    sigEff_vs_bkgEff->SetLineWidth(2);

    if(i == 0) {
      sigEff_vs_bkgEff->SetLineColor(kRed);
      sigEff_vs_bkgEff->SetMarkerColor(kRed);
    }
    if(i == 2){
      sigEff_vs_bkgEff->SetLineColor(kBlue);
      sigEff_vs_bkgEff->SetMarkerColor(kBlue);
    }
    if(i == 1){
      sigEff_vs_bkgEff->SetLineColor(kGreen+2);
      sigEff_vs_bkgEff->SetMarkerColor(kGreen+2);
    }

    else{
      sigEff_vs_bkgEff->SetLineColor(i+2);
      sigEff_vs_bkgEff->SetMarkerColor(i+2);
    }

    can_RoC->SetGrid();

    if(i == 0)  sigEff_vs_bkgEff->Draw("AC");
    else sigEff_vs_bkgEff->Draw("sameC");

    legend_RoC->AddEntry(sigEff_vs_bkgEff,labelLeg_RoC[i].c_str(),"pl");
    legend_RoC->Draw("same");

    can_RoC->Update();
    can_RoC->Modified();


  }

  string mvaRes = "";
  mvaRes = "mvares_EB_phoId_Hgg";

  can->SaveAs((mvaRes+".pdf").c_str()); 
  can->SaveAs((mvaRes+".png").c_str()); 
  can->SaveAs((mvaRes+".root").c_str()); 

  string canName_RoC = "";

  canName_RoC = "RoC_EB_phoId_Hgg";

  can_RoC->SaveAs((canName_RoC+".pdf").c_str()); 
  can_RoC->SaveAs((canName_RoC+".png").c_str()); 
  can_RoC->SaveAs((canName_RoC+".root").c_str()); 
}
