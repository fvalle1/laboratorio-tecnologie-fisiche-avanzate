#include <iostream>
#include <fstream>
#include "TGraph.h"
#include "TAxis.h"

int esf(){
  auto g = new TGraph();
  std::fstream errorfile("esf.csv", std::ios::in);
  std::string errorline;
  Int_t nGraph = 0;
  while(getline(errorfile, errorline)){
    double data=atof(errorline.c_str());
    g->SetPoint(nGraph,nGraph, data);
    nGraph++;
  }

  g->SetMarkerStyle(20);
  g->SetMarkerColor(kCyan-3);
  g->SetTitle("Edge spread function");

  g->Draw("APL");
  g->GetXaxis()->SetTitle("X [pixel]");
  g->GetYaxis()->SetTitle("Densita' ottica");

  return 0;
}
