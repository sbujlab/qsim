#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TLatex.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>

#define __IO_MAXHIT 10000

using namespace std;

TH1* hist_pool[100];
int n_hist = 0;

void draw(TCanvas* c, TH1* hist, const char* filename, const Option_t* option="", bool normalize=false) {
    c->cd();
    if (normalize) hist->Scale(1.0/hist->Integral());
    hist->Draw(option);
    c->SaveAs(filename);
}

void draw(TCanvas* c, TH1* hist_1, TH1* hist_2, const char* filename, const Option_t* option_1="", const Option_t* option_2="", bool normalize=false) {
    if (normalize) {
	hist_1->Scale(1.0/hist_1->Integral());
	hist_2->Scale(1.0/hist_2->Integral());
    }
    c->cd(1);
    hist_1->Draw(option_1);
    c->cd(2);
    hist_2->Draw(option_2);
    c->SaveAs(filename);
}

void draw(TCanvas* c, TH1* hist_1, TH1* hist_2, TH1* hist_3, const char* filename, const Option_t* option_1="", const Option_t* option_2="", const Option_t* option_3="", bool normalize=false) {
    if (normalize) {
	hist_1->Scale(1.0/hist_1->Integral());
	hist_2->Scale(1.0/hist_2->Integral());
	hist_3->Scale(1.0/hist_3->Integral());
    }
    c->cd(1);
    hist_1->Draw(option_1);
    c->cd(2);
    hist_2->Draw(option_2);
    c->cd(3);
    hist_3->Draw(option_3);
    c->SaveAs(filename);
}

void register_hist(TH1* hist){
    hist_pool[n_hist++] = hist;
}

TH1D* TH1D_creation(const char* name, const char* title, int n, double x_low, double x_up, const char* x_title="", const char* y_title=""){
    TH1D* hist = new TH1D(name, title, n, x_low, x_up);
    hist->GetXaxis()->SetTitle(x_title);
    hist->GetYaxis()->SetTitle(y_title);
    register_hist(hist);
    return hist;
}

TH1F* TH1F_creation(const char* name, const char* title, int n, double x_low, double x_up, const char* x_title="", const char* y_title=""){
    TH1F* hist = new TH1F(name, title, n, x_low, x_up);
    hist->GetXaxis()->SetTitle(x_title);
    hist->GetYaxis()->SetTitle(y_title);
    register_hist(hist);
    return hist;
}

TH2F* TH2F_creation(const char* name, const char* title, int nx, double x_low, double x_up, int ny, double y_low, double y_up, const char* x_title="", const char* y_title=""){
    TH2F* hist = new TH2F(name, title, nx, x_low, x_up, ny, y_low, y_up);
    hist->GetXaxis()->SetTitle(x_title);
    hist->GetYaxis()->SetTitle(y_title);
    register_hist(hist);
    return hist;
}

void hist_print(std::ofstream& out, const char* particle, TH1* hist, const int total){
    out << "Number of " << particle << ":\t" << hist->GetEntries() << "\t" << hist->GetEntries()*100.0/total << "%\n";
}
void plot(const char * qsim) {

//    gROOT->SetBatch();

    vector<int> v_pid = {-14, 14, -13, 13, 12, -11, 11, 22, 2112, 211, -211, 2212};
    map<int, string> map_pid = {
	{-14,   "#bar{#nu}_{#mu}"},
	{14,	"#nu_{#mu}"},
	{-13,	"#mu^{+}"},
	{13,	"#mu^{-}"},
	{12,	"#nu_{e}"},
	{-11,   "e^{+}"},
	{11,	"e^{-}"},
	{22,	"#gamma"},
	{2112,	"neutron"},
	{211,	"#pi^{+}"},
	{-211,  "#pi^{-}"},
	{2212,	"proton"},
    };

    TFile f(qsim, "read");
    TTree * t = (TTree*)f.Get("T_Mon");
    int pid, trid;
    double x, y, z, vx, vy, vz, theta, phi;
    double p, e, m, px, py, pz;
    t->SetBranchAddress("mon.pid",  &pid);
//    t->SetBranchAddress("mon.trid", &trid);
    t->SetBranchAddress("mon.x",    &x);
    t->SetBranchAddress("mon.y",    &y);
    t->SetBranchAddress("mon.z",    &z);
    t->SetBranchAddress("mon.theta",&theta);
    t->SetBranchAddress("mon.phi",  &phi);
    t->SetBranchAddress("mon.vx",   &vx);
    t->SetBranchAddress("mon.vy",   &vy);
    t->SetBranchAddress("mon.vz",   &vz);
    t->SetBranchAddress("mon.p",    &p);
    t->SetBranchAddress("mon.e",    &e);
    t->SetBranchAddress("mon.m",    &m);
    t->SetBranchAddress("mon.px",   &px);
    t->SetBranchAddress("mon.py",   &py);
    t->SetBranchAddress("mon.pz",   &pz);

    // pid
    TH1D *hist_pid = TH1D_creation("hist_pid", "particle distribution", 12, 0, 12, "", "Event");
    hist_pid->SetStats(kFALSE);

    // position and angular distribution
    TH1F * hist_x = TH1F_creation("hist_x", "x distribution", 48, -1.2, 1.2, "x [m]", "Event");
    TH1F * hist_y = TH1F_creation("hist_y", "y distribution", 48, -1.2, 1.2, "y [m]", "Event");
    TH2F * hist_xy = TH2F_creation("hist_xy", "xy distribution", 96, -1.2, 1.2, 96, -1.2, 1.2, "x [m]", "y [m]");
    TH1F * hist_theta = TH1F_creation("hist_theta", "#theta distribution", 90, 90, 180, "#theta [degree]", "Event");
    TH1F * hist_theta_PDF = TH1F_creation("hist_theta_PDF", "#theta PDF", 90, 90, 180, "#theta [degree]", "Event/sin#theta");
    TH1F * hist_phi = TH1F_creation("hist_phi", "#phi distribution", 36, -180, 180, "#phi [degree]", "Event");
    TH2F * hist_theta_phi = TH2F_creation("hist_theta_phi", "#theta and #phi distribution", 90, 90, 180, 36, -180, 180, "#theta [degree]", "#phi [degree]");

    /// electron
    TH1F * hist_e_x = TH1F_creation("hist_e_x", "x distribution of electron", 48, -1.2, 1.2, "x [m]", "Event");
    TH1F * hist_e_y = TH1F_creation("hist_e_y", "y distribution of electron", 48, -1.2, 1.2, "y [m]", "Event");
    TH2F * hist_e_xy = TH2F_creation("hist_e_xy", "xy distribution of electron", 48, -1.2, 1.2, 48, -1.2, 1.2, "x [m]", "y [m]");
    TH1F * hist_e_theta = TH1F_creation("hist_e_theta", "#theta distribution of electron", 90, 90, 180, "#theta [degree]", "Event");
    TH1F * hist_e_phi = TH1F_creation("hist_e_phi", "#phi distribution of electron", 36, -180, 180, "#phi [degree]", "Event");
    TH1F * hist_e_theta_PDF = TH1F_creation("hist_e_theta_PDF", "#theta PDF of electron", 90, 90, 180, "#theta [degree]", "Event/sin#theta");

    /// photon
    TH1F * hist_g_x = TH1F_creation("hist_g_x", "x distribution of photon", 48, -1.2, 1.2, "x [m]", "Event");
    TH1F * hist_g_y = TH1F_creation("hist_g_y", "y distribution of photon", 48, -1.2, 1.2, "y [m]", "Event");
    TH2F * hist_g_xy = TH2F_creation("hist_g_xy", "xy distribution of photon", 48, -1.2, 1.2, 48, -1.2, 1.2, "x [m]", "y [m]");
    TH1F * hist_g_theta = TH1F_creation("hist_g_theta", "#theta distribution of photon", 90, 90, 180, "#theta [degree]", "Event");
    TH1F * hist_g_phi = TH1F_creation("hist_g_phi", "#phi distribution of photon", 36, -180, 180, "#phi [degree]", "Event");
    TH1F * hist_g_theta_PDF = TH1F_creation("hist_g_theta_PDF", "#theta PDF of photon", 90, 90, 180, "#theta [degree]", "Event/sin#theta");

    /// neutron
    TH1F * hist_n_x = TH1F_creation("hist_n_x", "x distribution of neutron", 48, -1.2, 1.2, "x [m]", "Event");
    TH1F * hist_n_y = TH1F_creation("hist_n_y", "y distribution of neutron", 48, -1.2, 1.2, "y [m]", "Event");
    TH2F * hist_n_xy = TH2F_creation("hist_n_xy", "xy distribution of neutron", 48, -1.2, 1.2, 48, -1.2, 1.2, "x [m]", "y [m]");
    TH1F * hist_n_theta = TH1F_creation("hist_n_theta", "#theta distribution of neutron", 90, 90, 180, "#theta [degree]", "Event");
    TH1F * hist_n_phi = TH1F_creation("hist_n_phi", "#phi distribution of neutron", 36, -180, 180, "#phi [degree]", "Event");
    TH1F * hist_n_theta_PDF = TH1F_creation("hist_n_theta_PDF", "#theta PDF of neutron", 90, 90, 180, "#theta [degree]", "Event/sin#theta");
    
    // position of vertex
    TH1F * hist_vx = TH1F_creation("hist_vx", "Vertex x distribution", 70, -0.3, 0.4, "vx [m]", "Event");
    TH1F * hist_vy = TH1F_creation("hist_vy", "Vertex y distribution", 50, -0.25, 0.25, "vy [m]", "Event");
    TH1F * hist_vz = TH1F_creation("hist_vz", "Vertex z distribution", 30, -0.1, 0.05, "vz [m]", "Event");
    TH2F * hist_vxy = TH2F_creation("hist_vxy", "Vertex xy distribution", 100, -0.5, 0.5, 100, -0.5, 0.5, "vx [m]", "vy [m]");

    /// electron
    TH1F * hist_e_vx = TH1F_creation("hist_e_vx", "Vertex x distribution of electron", 70, -0.3, 0.4, "vx [m]", "Event");
    TH1F * hist_e_vy = TH1F_creation("hist_e_vy", "Vertex y distribution of electron", 50, -0.25, 0.25, "vy [m]", "Event");
    TH1F * hist_e_vz = TH1F_creation("hist_e_vz", "Vertex z distribution of electron", 30, -0.10, 0.05, "vz [m]", "Event");
    TH2F * hist_e_vxy = TH2F_creation("hist_e_vxy", "Vertex xy distribution of electron", 50, -0.5, 0.5, 50, -0.5, 0.5, "vx [m]", "vy [m]");

    /// photon
    TH1F * hist_g_vx = TH1F_creation("hist_g_vx", "Vertex x distribution of photon", 70, -0.3, 0.4, "vx [m]", "Event");
    TH1F * hist_g_vy = TH1F_creation("hist_g_vy", "Vertex y distribution of photon", 50, -0.25, 0.25, "vy [m]", "Event");
    TH1F * hist_g_vz = TH1F_creation("hist_g_vz", "Vertex z distribution of photon", 30, -0.10, 0.05, "vz [m]", "Event");
    TH2F * hist_g_vxy = TH2F_creation("hist_g_vxy", "Vertex xy distribution of photon", 50, -0.5, 0.5, 50, -0.5, 0.5, "vx [m]", "vy [m]");

    /// neutron
    TH1F * hist_n_vx = TH1F_creation("hist_n_vx", "Vertex x distribution of neutron", 70, -0.3, 0.4, "vx [m]", "Event");
    TH1F * hist_n_vy = TH1F_creation("hist_n_vy", "Vertex y distribution of neutron", 50, -0.25, 0.25, "vy [m]", "Event");
    TH1F * hist_n_vz = TH1F_creation("hist_n_vz", "Vertex z distribution of neutron", 30, -0.1, 0.05, "vz [m]", "Event");
    TH2F * hist_n_vxy = TH2F_creation("hist_n_vxy", "Vertex xy distribution of neutron", 50, -0.5, 0.5, 50, -0.5, 0.5, "vx [m]", "vy [m]");

    // Momentum
    /// electron
    TH1F *hist_e_p = TH1F_creation("hist_e_p", "momentum of electron", 40, 0, 40, "P [MeV]", "Event");
    TH1F *hist_e_p_1 = TH1F_creation("hist_e_p_1", "momentum of electron (P > 2 MeV)", 40, 2, 42, "P [MeV]", "Event");
    
    TH1F *hist_eplus_p = TH1F_creation("hist_eplus_p", "momentum of positron", 20, 0, 20, "P [MeV]", "Event");	/// positron

    // photon, gamma
    TH1F *hist_g_p = TH1F_creation("hist_g_p", "momentum of photon", 110, 0, 100, "P [MeV]", "Event");
    TH1F *hist_g_p_1 = TH1F_creation("hist_g_p_1", "momentum of photon (P > 1 MeV)", 99, 1, 110, "P [MeV]", "Event");
    TH1F *hist_g_p_2 = TH1F_creation("hist_g_p_2", "momentum of photon (P > 10 MeV)", 90, 10, 100, "P [MeV]", "Event");

    /// neutrino
    TH1F *hist_numuplus_p = TH1F_creation("hist_numuplus_p", "momentum of #bar{#nu}_{#mu}", 50, 0, 50, "P [MeV]", "Event");	/// anti-muon neutrino
    TH1F *hist_numu_p = TH1F_creation("hist_numu_p", "momentum of #nu_{#mu}", 50, 0, 50, "P [MeV]", "Event");   /// muon neutrino
    TH1F *hist_nue_p = TH1F_creation("hist_nue_p", "momentum of #nu_{e}", 50, 0, 50, "P [MeV]", "Event");   /// electron neutrino

    /// muon(mu plus)
    TH1F *hist_mu_p = TH1F_creation("hist_mu_p", "momentum of #mu", 10, 0, 100, "P [MeV]", "Event");
    TH1F *hist_mu_ke = TH1F_creation("hist_mu_ke", "kinetic energy of #mu", 20, 0, 200, "E [MeV]", "Event");
    TH1F *hist_muplus_p = TH1F_creation("hist_muplus_p", "momentum of #mu^{+}", 10, 0, 100, "P [MeV]", "Event");
    TH1F *hist_muplus_ke = TH1F_creation("hist_muplus_ke", "kinetic energy of #mu^{+}", 20, 0, 200, "E [MeV]", "Event");

    /// pi minus
    TH1F *hist_piminus_p = TH1F_creation("hist_piminus_p", "momentum of #pi^{-}", 70, 0, 350, "P [MeV]", "Event");
    TH1F *hist_piminus_ke = TH1F_creation("hist_piminus_ke", "kinetic energy of #pi^{-}", 70, 0, 350, "E [MeV]", "Event");

    /// pi plus
    TH1F *hist_piplus_p = TH1F_creation("hist_piplus_p", "momentum of #pi^{+}", 70, 0, 350, "P [MeV]", "Event");
    TH1F *hist_piplus_ke = TH1F_creation("hist_piplus_ke", "kinetic energy of #pi^{+}", 70, 0, 350, "E [MeV]", "Event");

    /// neutron
    TH1F *hist_n_p = TH1F_creation("hist_n_p", "momentum of neutron", 90, 0, 900, "P [MeV]", "Event");
    TH1F *hist_n_ke = TH1F_creation("hist_n_ke", "kinetic energy of neutron", 60, 0, 900, "E [MeV]", "Event");

    /// proton
    TH1F *hist_p_p = TH1F_creation("hist_p_p", "momentum of proton", 80, 200, 600, "P [MeV]", "Event");
    TH1F *hist_p_ke = TH1F_creation("hist_p_ke", "kinetic energy of proton", 50, 0, 1000, "E [MeV]", "Event");

    int n = t->GetEntries();
    int nx = 0; // count of unclassified particles
    for (int i=0; i<n; i++){
	t->GetEntry(i);

	hist_pid->Fill(std::find(v_pid.begin(), v_pid.end(), pid) - v_pid.begin());

	hist_x->Fill(x);
	hist_y->Fill(y);
	hist_xy->Fill(x, y);
	hist_theta->Fill(theta);
	hist_theta_PDF->Fill(theta, p/(px*px + py*py));
	if ((x*x+y*y) < 0.25)
	{
	    hist_phi->Fill(phi);
	}
	hist_theta_phi->Fill(theta, phi);
	hist_vx->Fill(vx);
	hist_vy->Fill(vy);
	hist_vz->Fill(vz);
	hist_vxy->Fill(vx,vy);

	if (pid == 22){	// gamma
	    hist_g_p->Fill(p);
	    hist_g_x->Fill(x);
	    hist_g_y->Fill(y);
	    hist_g_xy->Fill(x, y);
	    hist_g_theta->Fill(theta);
	    hist_g_theta_PDF->Fill(theta, p/(px*px + py*py));
	    hist_g_phi->Fill(phi);
	    hist_g_vx->Fill(vx);
	    hist_g_vy->Fill(vy);
	    hist_g_vz->Fill(vz);
	    hist_g_vxy->Fill(vx, vy);

	    if (p > 1) 
		hist_g_p_1->Fill(p);
	    if (p > 10) 
		hist_g_p_2->Fill(p);
	}
	else if (pid == 11)	{   // electron
	    hist_e_p->Fill(p);
	    hist_e_x->Fill(x);
	    hist_e_y->Fill(y);
	    hist_e_xy->Fill(x, y);
	    hist_e_theta->Fill(theta);
	    hist_e_theta_PDF->Fill(theta, p/(px*px + py*py));
	    hist_e_phi->Fill(phi);
	    hist_e_vx->Fill(vx);
	    hist_e_vy->Fill(vy);
	    hist_e_vz->Fill(vz);
	    hist_e_vxy->Fill(vx, vy);

	    if (p > 2)
		hist_e_p_1->Fill(p);
	}
	else if (pid == 2112){	// neutron
	    hist_n_p->Fill(p);
	    hist_n_ke->Fill(e-m);
	    hist_n_x->Fill(x);
	    hist_n_y->Fill(y);
	    hist_n_xy->Fill(x, y);
	    hist_n_theta->Fill(theta);
	    hist_n_theta_PDF->Fill(theta, p/(px*px + py*py));
	    hist_n_phi->Fill(phi);
	    hist_n_vx->Fill(vx);
	    hist_n_vy->Fill(vy);
	    hist_n_vz->Fill(vz);
	    hist_n_vxy->Fill(vx, vy);
	}
	else if (pid == -11){	// positron
	    hist_eplus_p->Fill(p);
	}
	else if (pid == 2212){	// proton
	    hist_p_p->Fill(p);
	    hist_p_ke->Fill(e-m);
	}
	else if (pid == 12){	// electron neutrino
	    hist_nue_p->Fill(p);
	}
	else if (pid == 14){	// muon neutrino
	    hist_numu_p->Fill(p);
	}
	else if (pid == -14){	// anti-muon neutrino
	    hist_numuplus_p->Fill(p);
	}
	else if (pid == -211){	// pion minus
	    hist_piminus_p->Fill(p);
	    hist_piminus_ke->Fill(e-m);
	}
	else if (pid == 211){	// pion plus
	    hist_piplus_p->Fill(p);
	    hist_piplus_ke->Fill(e-m);
	}
	else if (pid == 13) {     // muon
	    hist_mu_p->Fill(p);
	    hist_mu_ke->Fill(e-m);
	}
	else if (pid == -13) {     // mu plus
	    hist_muplus_p->Fill(p);
	    hist_muplus_ke->Fill(e-m);
	}
	else {	// unclassified
	    cout << pid << "\n" << endl;
	    nx++;
	}
    }

    // output
    ofstream fout("statistics.txt", std::ifstream::app);
    if (! fout.good() )
    {
	cout << "Fail to open file 'statistics.txt'" << endl;
	exit(10);
    }


    hist_print(fout, "photon", hist_g_p, n);
    hist_print(fout, "photon with P > 1", hist_g_p_1, n);
    hist_print(fout, "electron", hist_e_p, n);
    hist_print(fout, "electron with P > 2", hist_e_p_1, n);
    hist_print(fout, "neutron", hist_n_p, n);
    hist_print(fout, "positron", hist_eplus_p, n);
    hist_print(fout, "proton", hist_p_p, n);
    hist_print(fout, "electron neutrino", hist_nue_p, n);
    hist_print(fout, "muon neutrino", hist_numu_p, n);
    hist_print(fout, "anti-muon neutrino", hist_numuplus_p, n);
    hist_print(fout, "pion minus", hist_piminus_p, n);
    hist_print(fout, "pion plus", hist_piplus_p, n);
    hist_print(fout, "mu plus", hist_muplus_p, n);
    hist_print(fout, "muon", hist_mu_p, n);
    fout << "Number of particles:\t" << n << "\n"
         << "Number of unclassified:\t"  << nx << endl << endl << endl;

    TCanvas * c1 = new TCanvas("c1", "c1", 800, 600);
    TCanvas * c2 = new TCanvas("c2", "c2", 600, 900);
    c2->Divide(1,2);
    TCanvas * c3 = new TCanvas("c3", "c3", 1800, 450);
    c3->Divide(3,1);

    // position: overall
    draw(c2, hist_x, hist_y, "xy.png");
    draw(c1, hist_theta_phi, "theta_phi.png", "COLZ");
    draw(c1, hist_theta, "theta.png");
    draw(c1, hist_phi, "phi.png");

    /// position: electron
    draw(c2, hist_e_x, hist_e_y, "e_xy.png");
    draw(c1, hist_e_theta, "e_theta.png");
    draw(c1, hist_e_phi, "e_phi.png");

    /// position: photon
    draw(c2, hist_g_x, hist_g_y, "g_xy.png");
    draw(c1, hist_g_theta, "g_theta.png");
    draw(c1, hist_g_phi, "g_phi.png");

    /// position: neutron
    draw(c2, hist_n_x, hist_n_y, "n_xy.png");
    draw(c1, hist_n_theta, "n_theta.png");
    draw(c1, hist_n_phi, "n_phi.png");

    // vertex: overall
    draw(c3, hist_vx, hist_vy, hist_vz, "vxyz.png");
    draw(c1, hist_vz, "vz.png");

    /// vertex: electron
    draw(c3, hist_e_vx, hist_e_vy, hist_e_vz, "e_vxyz.png");
    draw(c1, hist_e_vz, "e_vz.png");
    
    /// vertex: photon
    draw(c3, hist_g_vx, hist_g_vy, hist_g_vz, "g_vxyz.png");
    draw(c1, hist_g_vz, "g_vz.png");
    
    /// vertex: neutron
    draw(c3, hist_n_vx, hist_n_vy, hist_n_vz, "n_vxyz.png");
    draw(c1, hist_n_vz, "n_vz.png");

    // momentum and energy
    draw(c1, hist_eplus_p, "positron.png"); /// positron
    draw(c1, hist_nue_p, "electron_neutrino.png");  /// electron_neutrino
    draw(c2, hist_numu_p, hist_numuplus_p, "muon_neutrino.png");    /// (anti-)muon neutrino
    draw(c2, hist_piminus_p, hist_piminus_ke, "pion_minus.png");	    /// pion-minus
    draw(c2, hist_piplus_p, hist_piplus_ke, "pion_plus.png");	    /// pion-plus
    draw(c2, hist_p_p, hist_p_ke, "proton.png");	    /// proton
    
    /// log scale ///
    
    c1->SetLogz();
    // position
    draw(c1, hist_xy, "x:y.png", "COLZ");
    draw(c1, hist_e_xy, "e_x:y.png", "COLZ");
    draw(c1, hist_g_xy, "g_x:y.png", "COLZ");
    draw(c1, hist_n_xy, "n_x:y.png", "COLZ");
    // vertex
    draw(c1, hist_vxy, "vx:vy.png", "COLZ");
    draw(c1, hist_e_vxy, "e_vx:vy.png", "COLZ");
    draw(c1, hist_g_vxy, "g_vx:vy.png", "COLZ");
    draw(c1, hist_n_vxy, "n_vx:vy.png", "COLZ");


    // pid
    c1->cd();
    gPad->SetLogy();
    hist_pid->Draw();
    for (int i=0; i<v_pid.size(); ++i){
	int ty = hist_pid->GetBinContent(i+1);
//	if (ty <= (n/10000.<100 ? 100 : n/10000.)) continue;
	if (ty == 0) continue;
	ty *= 1.3;
	TLatex * text = new TLatex(i+0.4, ty, map_pid.at(v_pid[i]).c_str());
	if (v_pid[i] == 2112 || v_pid[i] == 2212)
	    text->SetTextAngle(50);
	text->Draw("same");
    }
    c1->SaveAs("pid.png");

    // theta PDF
    draw(c1, hist_theta_PDF, "theta_PDF.png");
    draw(c1, hist_e_theta_PDF, "e_theta_PDF.png");
    draw(c1, hist_g_theta_PDF, "g_theta_PDF.png");
    draw(c1, hist_n_theta_PDF, "n_theta_PDF.png");

    c2->GetPad(1)->SetLogy();
    c2->GetPad(2)->SetLogy();
    c3->GetPad(1)->SetLogy();
    c3->GetPad(2)->SetLogy();
    c3->GetPad(3)->SetLogy();
    /// electron
    draw(c2, hist_e_p, hist_e_p_1, "electron.png");	/// electron
    draw(c2, hist_g_p, hist_g_p_1, "photon.png");	/// photon
    draw(c2, hist_n_p, hist_n_ke, "neutron.png");	/// neutron

    for (int i=0; i<n_hist; i++){
	hist_pool[i]->Delete();
    }

    c1->Close();
    c2->Close();
    c3->Close();
    f.Close();
    fout.close();
}

#ifndef __CINT__
int main(int argc, char** argv) 
{
    plot(argv[1]);
    return 0;
}
#endif
