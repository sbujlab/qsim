#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TLatex.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void z(const char * qsim) {

    TFile f(qsim, "read");
    TTree * t = (TTree*)f.Get("T_Hits");
    double p, e;
    double x, y, z;
    int pid;
//    t->SetBranchAddress("mon.x", &x);
//    t->SetBranchAddress("mon.y", &y);
    t->SetBranchAddress("mon.z", &z);

    // pid
    TH1F *hist_z = new TH1F ("hist_z", "z of hit", 40, -0.552, -0.548);
    hist_z->GetXaxis()->SetTitle("z [m]");
    hist_z->GetYaxis()->SetTitle("Event");

    int n = t->GetEntries();
    for (int i=0; i<n; i++){
	t->GetEntry(i);
	hist_z->Fill(z);
    }

    TCanvas * c = new TCanvas("c3", "c3", 1200, 450);
    c->Divide(2,1);

    c->cd(1);
    hist_z->Draw();
    c->cd(2);
    c->GetPad(2)->SetLogy();
    hist_z->Draw();
    c->SaveAs("z.png");


    hist_z->Delete();

    c->Close();
    f.Close();
}



#ifndef __CINT__
int main(int argc, char** argv) 
{
    z(argv[1]);
    return 0;
}
#endif
