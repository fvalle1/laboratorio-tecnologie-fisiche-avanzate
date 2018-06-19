#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "TH1F.h"
#include "TAxis.h"

Double_t lsb = 0.4901;
Double_t lsboff = -11;

int distpulse(){
  const Int_t NSeries = 3;
  auto cx = new TCanvas("cx","Spettro impulso",0,0,800,500);
  TH1F* h;
  auto leg = new TLegend(.75,.85,.9,.9);

  h = new TH1F("h", "distribuzione impulsatore", 50, 1400, 1700);
  std::fstream distfile("distpulse.csv", std::ios::in);
  std::string fileline;
  Int_t nGraph = 0;
  while(getline(distfile, fileline)){
    double data=atof(fileline.c_str());
    h->Fill((Double_t)data);
  }

  cx->cd();
  h->SetLineWidth(2);
  h->SetStats(kFALSE);
  h->Draw();

  leg->AddEntry(h,"dati");

  distfile.close();

  auto fit = new TF1("fit", "[0]*TMath::Gaus(x,[1],[2])");
  fit->SetParameter(0,35);
  fit->SetParameter(1,1550);
  fit->SetParameter(2,10);
  fit->SetParLimits(1,1530,1580);
  h->Fit(fit,"M+");
  printf("mean: %.0f\tstd: %.0f\t\ttensione: %.0fmV\n", h->GetMean(), h->GetStdDev(), h->GetMean()*lsb + lsboff);

  h->SetLineColor(kCyan +3);


  leg->Draw("same");


  return 0;
}
