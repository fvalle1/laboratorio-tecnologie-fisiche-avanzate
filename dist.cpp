#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "TH1F.h"
#include "TAxis.h"

Double_t lsb = 0.4901;
Double_t lsboff = -11;

int dist(){
  const Int_t NSeries = 3;
  auto cx = new TCanvas("cx","Spettri",0,0,800,500);
  TH1F* histos[NSeries];
  std::vector<std::string> channels = {"0","1","2"};
  auto leg = new TLegend(.45,.55,.9,.9);


  Int_t iCh=0;
  for(auto ch :channels){
    histos[iCh] = new TH1F(Form("%s%s","hist",ch.c_str()), Form("%s%s","dist",ch.c_str()), 30, 0, 4196);
    std::fstream distfile(Form("dist%s.%s",ch.c_str(),"csv"), std::ios::in);
    std::string fileline;
    Int_t nGraph = 0;
    while(getline(distfile, fileline)){
      double data=atof(fileline.c_str());
      histos[iCh]->Fill((Double_t)data);
    }

    cx->cd();
    histos[iCh]->SetLineWidth(2);
    if(iCh==0){
      histos[iCh]->SetStats(kFALSE);
      histos[iCh]->SetTitle("Spettri");
      histos[iCh]->Draw();
      histos[iCh]->GetXaxis()->SetTitle("channel");
    }else{
      histos[iCh]->Draw("same");
    }

    histos[iCh]->GetYaxis()->SetRangeUser(0,6900);
    leg->AddEntry(histos[iCh],Form("%s %s","Canale",ch.c_str()));

    distfile.close();
    printf("channel%s:\tmean: %.0f\tstd: %.0f\t\ttensione: %.0fmV\n",ch.c_str(), histos[iCh]->GetMean(), histos[iCh]->GetStdDev(), histos[iCh]->GetMean()*lsb + lsboff);
    iCh++;
  }

  histos[0]->SetLineColor(kBlue);
  histos[1]->SetLineColor(kGreen);
  histos[2]->SetLineColor(kRed);

  iCh=0;
  TF1* poissons[NSeries];

  for(auto ch :channels){
    poissons[iCh] = new TF1(Form("%s%s","pois",ch.c_str()), "[0]*TMath::Poisson([3]*x+[2],[1])");
    poissons[iCh]->SetParameter(0,5000);
    poissons[iCh]->SetParameter(1,1000);
    poissons[iCh]->SetParameter(2,-800);
    poissons[iCh]->SetParameter(3,1./4196);

    switch(iCh){
      case 0:
      poissons[iCh]->SetLineColor(kBlue+1);
      break;
      case 1:
      poissons[iCh]->SetLineColor(kGreen+1);
      break;
      default:
      poissons[iCh]->SetLineColor(kRed +1);
    }

    histos[iCh]->Fit(poissons[iCh],"MQ+");
    poissons[iCh]->Draw("same");

    leg->AddEntry(poissons[iCh],Form("%s %s","Fit canale",ch.c_str()));

    Double_t mean = (poissons[iCh]->GetParameter(1) - poissons[iCh]->GetParameter(2))/poissons[iCh]->GetParameter(3);

    printf("channel%s:\textimated mean: %.0f\textemated voltage: %.0fmV\n",ch.c_str(), mean, mean*lsb+lsboff);

    iCh++;
  }

  leg->Draw("same");


  return 0;
}
