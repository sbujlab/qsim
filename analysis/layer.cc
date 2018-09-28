#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TLegend.h"
#include <iostream>
#include <fstream>

using namespace std;

#define __IO_MAXHIT 10000

TH1* hist_pool[300];
int n_hist = 0;

void draw(TCanvas* c, TH1* hist, const char* filename, const Option_t* option="") {
    c->cd();
    hist->Draw(option);
    c->SaveAs(filename);
}

void draw(TCanvas* c, TH1* hist_1, TH1* hist_2, const char* filename, const Option_t* option_1="", const Option_t* option_2="") {
    c->cd(1);
    hist_1->Draw(option_1);
    c->cd(2);
    hist_2->Draw(option_2);
    c->SaveAs(filename);
}

void draw(TCanvas* c, TH1* hist_1, TH1* hist_2, TH1* hist_3, const char* filename, const Option_t* option_1="", const Option_t* option_2="", const Option_t* option_3="") {
    c->cd(1);
    hist_1->Draw(option_1);
    c->cd(2);
    hist_2->Draw(option_2);
    c->cd(3);
    hist_3->Draw(option_3);
    c->SaveAs(filename);
}

void draw(TCanvas* c, TH1* hist_1, TH1* hist_2, TH1* hist_3, TH1 * hist_4, const char* filename, const Option_t* option_1="", const Option_t* option_2="", const Option_t* option_3="", const Option_t* option_4="") {
    c->cd(1);
    hist_1->Draw(option_1);
    c->cd(2);
    hist_2->Draw(option_2);
    c->cd(3);
    hist_3->Draw(option_3);
    c->cd(4);
    hist_4->Draw(option_4);
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


void layer(const char * qsim) {   // convert layer tree into hit tree

    TH2F * hist_l1_for_xy = TH2F_creation("hist_l1_for_xy", "xy distribution of forward particles in layer1", 52, -0.13, 0.13, 52, -0.13, 0.13, "x [m]", "y [m]");
    TH2F * hist_l1_back_xy = TH2F_creation("hist_l1_back_xy", "xy distribution of backward particles in layer1", 52, -0.13, 0.13, 52, -0.13, 0.13, "x [m]", "y [m]");
    TH2F * hist_l2_for_xy = TH2F_creation("hist_l2_for_xy", "xy distribution of forward particles in layer2", 52, -0.13, 0.13, 52, -0.13, 0.13, "x [m]", "y [m]");
    TH2F * hist_l2_back_xy = TH2F_creation("hist_l2_back_xy", "xy distribution of backward particles in layer2", 52, -0.13, 0.13, 52, -0.13, 0.13, "x [m]", "y [m]");
    TH2F * hist_l3_for_xy = TH2F_creation("hist_l3_for_xy", "xy distribution of forward particles in layer3", 52, -0.13, 0.13, 52, -0.13, 0.13, "x [m]", "y [m]");
    TH2F * hist_l3_back_xy = TH2F_creation("hist_l3_back_xy", "xy distribution of backward particles in layer3", 52, -0.13, 0.13, 52, -0.13, 0.13, "x [m]", "y [m]");
    TH2F * hist_l4_for_xy = TH2F_creation("hist_l4_for_xy", "xy distribution of forward particles in layer4", 52, -0.13, 0.13, 52, -0.13, 0.13, "x [m]", "y [m]");
    TH2F * hist_l4_back_xy = TH2F_creation("hist_l4_back_xy", "xy distribution of backward particles in layer4", 52, -0.13, 0.13, 52, -0.13, 0.13, "x [m]", "y [m]");
    TH2F * hist_l5_for_xy = TH2F_creation("hist_l5_for_xy", "xy distribution of forward particles in layer5", 52, -0.13, 0.13, 52, -0.13, 0.13, "x [m]", "y [m]");
    TH2F * hist_l5_back_xy = TH2F_creation("hist_l5_back_xy", "xy distribution of backward particles in layer5", 52, -0.13, 0.13, 52, -0.13, 0.13, "x [m]", "y [m]");
    TH2F * hist_l6_for_xy = TH2F_creation("hist_l6_for_xy", "xy distribution of forward particles in layer6", 52, -0.13, 0.13, 52, -0.13, 0.13, "x [m]", "y [m]");
    TH2F * hist_l6_back_xy = TH2F_creation("hist_l6_back_xy", "xy distribution of backward particles in layer6", 52, -0.13, 0.13, 52, -0.13, 0.13, "x [m]", "y [m]");
    TH2F * hist_l7_for_xy = TH2F_creation("hist_l7_for_xy", "xy distribution of forward particles in layer7", 52, -0.13, 0.13, 52, -0.13, 0.13, "x [m]", "y [m]");
    TH2F * hist_l7_back_xy = TH2F_creation("hist_l7_back_xy", "xy distribution of backward particles in layer7", 52, -0.13, 0.13, 52, -0.13, 0.13, "x [m]", "y [m]");
    TH2F * hist_l8_for_xy = TH2F_creation("hist_l8_for_xy", "xy distribution of forward particles in layer8", 52, -0.13, 0.13, 52, -0.13, 0.13, "x [m]", "y [m]");
    TH2F * hist_l8_back_xy = TH2F_creation("hist_l8_back_xy", "xy distribution of backward particles in layer8", 52, -0.13, 0.13, 52, -0.13, 0.13, "x [m]", "y [m]");
    TH2F * hist_l9_for_xy = TH2F_creation("hist_l9_for_xy", "xy distribution of forward particles in layer9", 52, -0.13, 0.13, 52, -0.13, 0.13, "x [m]", "y [m]");
    TH2F * hist_l9_back_xy = TH2F_creation("hist_l9_back_xy", "xy distribution of backward particles in layer9", 52, -0.13, 0.13, 52, -0.13, 0.13, "x [m]", "y [m]");


    // energy and momentum dist.
    /// layer1
    TH1F * hist_l1_for_g_P = TH1F_creation("hist_l1_for_g_P", "momentrum distribution of forward photon in layer1", 25, 0, 50, "P [MeV]", "Event");
    TH1F * hist_l1_back_g_P = TH1F_creation("hist_l1_back_g_P", "momentrum distribution of backward photon in layer1", 25, 0, 50, "P [MeV]", "Event");
    TH1F * hist_l1_for_e_P = TH1F_creation("hist_l1_for_e_P", "momentrum distribution of forward electron in layer1", 35, 0, 35, "P [MeV]", "Event");
    TH1F * hist_l1_back_e_P = TH1F_creation("hist_l1_back_e_P", "momentrum distribution of backward electron in layer1", 35, 0, 35, "P [MeV]", "Event");
    TH1F * hist_l1_for_n_P = TH1F_creation("hist_l1_for_n_P", "momentrum distribution of forward neutron in layer1", 40, 0, 200, "P [MeV]", "Event");
    TH1F * hist_l1_back_n_P = TH1F_creation("hist_l1_back_n_P", "momentrum distribution of backward neutron in layer1", 40, 0, 200, "P [MeV]", "Event");
    TH1F * hist_l1_for_n_KE = TH1F_creation("hist_l1_for_n_KE", "kinetic energy distribution of forward neutron in layer1", 20, 0, 20, "P [MeV]", "Event");
    TH1F * hist_l1_back_n_KE = TH1F_creation("hist_l1_back_n_KE", "kinetic energy distribution of backward neutron in layer1", 20, 0, 20, "P [MeV]", "Event");
    TH1F * hist_l1_for_eplus_P = TH1F_creation("hist_l1_for_eplus_P", "momentrum distribution of forward eplus in layer1", 30, 0, 30, "P [MeV]", "Event");
    TH1F * hist_l1_back_eplus_P = TH1F_creation("hist_l1_back_eplus_P", "momentrum distribution of backward eplus in layer1", 30, 0, 30, "P [MeV]", "Event");

    /// layer2
    TH1F * hist_l2_for_g_P = TH1F_creation("hist_l2_for_g_P", "momentrum distribution of forward photon in layer2", 25, 0, 50, "P [MeV]", "Event");
    TH1F * hist_l2_back_g_P = TH1F_creation("hist_l2_back_g_P", "momentrum distribution of backward photon in layer2", 25, 0, 50, "P [MeV]", "Event");
    TH1F * hist_l2_for_e_P = TH1F_creation("hist_l2_for_e_P", "momentrum distribution of forward electron in layer2", 35, 0, 35, "P [MeV]", "Event");
    TH1F * hist_l2_back_e_P = TH1F_creation("hist_l2_back_e_P", "momentrum distribution of backward electron in layer2", 35, 0, 35, "P [MeV]", "Event");
    TH1F * hist_l2_for_n_P = TH1F_creation("hist_l2_for_n_P", "momentrum distribution of forward neutron in layer2", 40, 0, 200, "P [MeV]", "Event");
    TH1F * hist_l2_back_n_P = TH1F_creation("hist_l2_back_n_P", "momentrum distribution of backward neutron in layer2", 40, 0, 200, "P [MeV]", "Event");
    TH1F * hist_l2_for_n_KE = TH1F_creation("hist_l2_for_n_KE", "kinetic energy distribution of forward neutron in layer2", 20, 0, 20, "P [MeV]", "Event");
    TH1F * hist_l2_back_n_KE = TH1F_creation("hist_l2_back_n_KE", "kinetic energy distribution of backward neutron in layer2", 20, 0, 20, "P [MeV]", "Event");
    TH1F * hist_l2_for_eplus_P = TH1F_creation("hist_l2_for_eplus_P", "momentrum distribution of forward eplus in layer2", 30, 0, 30, "P [MeV]", "Event");
    TH1F * hist_l2_back_eplus_P = TH1F_creation("hist_l2_back_eplus_P", "momentrum distribution of backward eplus in layer2", 30, 0, 30, "P [MeV]", "Event");

    /// layer3
    TH1F * hist_l3_for_g_P = TH1F_creation("hist_l3_for_g_P", "momentrum distribution of forward photon in layer3", 25, 0, 50, "P [MeV]", "Event");
    TH1F * hist_l3_back_g_P = TH1F_creation("hist_l3_back_g_P", "momentrum distribution of backward photon in layer3", 25, 0, 50, "P [MeV]", "Event");
    TH1F * hist_l3_for_e_P = TH1F_creation("hist_l3_for_e_P", "momentrum distribution of forward electron in layer3", 35, 0, 35, "P [MeV]", "Event");
    TH1F * hist_l3_back_e_P = TH1F_creation("hist_l3_back_e_P", "momentrum distribution of backward electron in layer3", 35, 0, 35, "P [MeV]", "Event");
    TH1F * hist_l3_for_n_P = TH1F_creation("hist_l3_for_n_P", "momentrum distribution of forward neutron in layer3", 40, 0, 200, "P [MeV]", "Event");
    TH1F * hist_l3_back_n_P = TH1F_creation("hist_l3_back_n_P", "momentrum distribution of backward neutron in layer3", 40, 0, 200, "P [MeV]", "Event");
    TH1F * hist_l3_for_n_KE = TH1F_creation("hist_l3_for_n_KE", "kinetic energy distribution of forward neutron in layer3", 20, 0, 20, "P [MeV]", "Event");
    TH1F * hist_l3_back_n_KE = TH1F_creation("hist_l3_back_n_KE", "kinetic energy distribution of backward neutron in layer3", 20, 0, 20, "P [MeV]", "Event");
    TH1F * hist_l3_for_eplus_P = TH1F_creation("hist_l3_for_eplus_P", "momentrum distribution of forward eplus in layer3", 30, 0, 30, "P [MeV]", "Event");
    TH1F * hist_l3_back_eplus_P = TH1F_creation("hist_l3_back_eplus_P", "momentrum distribution of backward eplus in layer3", 30, 0, 30, "P [MeV]", "Event");

    /// layer4
    TH1F * hist_l4_for_g_P = TH1F_creation("hist_l4_for_g_P", "momentrum distribution of forward photon in layer4", 25, 0, 50, "P [MeV]", "Event");
    TH1F * hist_l4_back_g_P = TH1F_creation("hist_l4_back_g_P", "momentrum distribution of backward photon in layer4", 25, 0, 50, "P [MeV]", "Event");
    TH1F * hist_l4_for_e_P = TH1F_creation("hist_l4_for_e_P", "momentrum distribution of forward electron in layer4", 35, 0, 35, "P [MeV]", "Event");
    TH1F * hist_l4_back_e_P = TH1F_creation("hist_l4_back_e_P", "momentrum distribution of backward electron in layer4", 35, 0, 35, "P [MeV]", "Event");
    TH1F * hist_l4_for_n_P = TH1F_creation("hist_l4_for_n_P", "momentrum distribution of forward neutron in layer4", 40, 0, 200, "P [MeV]", "Event");
    TH1F * hist_l4_back_n_P = TH1F_creation("hist_l4_back_n_P", "momentrum distribution of backward neutron in layer4", 40, 0, 200, "P [MeV]", "Event");
    TH1F * hist_l4_for_n_KE = TH1F_creation("hist_l4_for_n_KE", "kinetic energy distribution of forward neutron in layer4", 20, 0, 20, "P [MeV]", "Event");
    TH1F * hist_l4_back_n_KE = TH1F_creation("hist_l4_back_n_KE", "kinetic energy distribution of backward neutron in layer4", 20, 0, 20, "P [MeV]", "Event");
    TH1F * hist_l4_for_eplus_P = TH1F_creation("hist_l4_for_eplus_P", "momentrum distribution of forward eplus in layer4", 30, 0, 30, "P [MeV]", "Event");
    TH1F * hist_l4_back_eplus_P = TH1F_creation("hist_l4_back_eplus_P", "momentrum distribution of backward eplus in layer4", 30, 0, 30, "P [MeV]", "Event");

    /// layer5
    TH1F * hist_l5_for_g_P = TH1F_creation("hist_l5_for_g_P", "momentrum distribution of forward photon in layer5", 25, 0, 50, "P [MeV]", "Event");
    TH1F * hist_l5_back_g_P = TH1F_creation("hist_l5_back_g_P", "momentrum distribution of backward photon in layer5", 25, 0, 50, "P [MeV]", "Event");
    TH1F * hist_l5_for_e_P = TH1F_creation("hist_l5_for_e_P", "momentrum distribution of forward electron in layer5", 35, 0, 35, "P [MeV]", "Event");
    TH1F * hist_l5_back_e_P = TH1F_creation("hist_l5_back_e_P", "momentrum distribution of backward electron in layer5", 35, 0, 35, "P [MeV]", "Event");
    TH1F * hist_l5_for_n_P = TH1F_creation("hist_l5_for_n_P", "momentrum distribution of forward neutron in layer5", 40, 0, 200, "P [MeV]", "Event");
    TH1F * hist_l5_back_n_P = TH1F_creation("hist_l5_back_n_P", "momentrum distribution of backward neutron in layer5", 40, 0, 200, "P [MeV]", "Event");
    TH1F * hist_l5_for_n_KE = TH1F_creation("hist_l5_for_n_KE", "kinetic energy distribution of forward neutron in layer5", 20, 0, 20, "P [MeV]", "Event");
    TH1F * hist_l5_back_n_KE = TH1F_creation("hist_l5_back_n_KE", "kinetic energy distribution of backward neutron in layer5", 20, 0, 20, "P [MeV]", "Event");
    TH1F * hist_l5_for_eplus_P = TH1F_creation("hist_l5_for_eplus_P", "momentrum distribution of forward eplus in layer5", 30, 0, 30, "P [MeV]", "Event");
    TH1F * hist_l5_back_eplus_P = TH1F_creation("hist_l5_back_eplus_P", "momentrum distribution of backward eplus in layer5", 30, 0, 30, "P [MeV]", "Event");

    /// layer6
    TH1F * hist_l6_for_g_P = TH1F_creation("hist_l6_for_g_P", "momentrum distribution of forward photon in layer6", 25, 0, 50, "P [MeV]", "Event");
    TH1F * hist_l6_back_g_P = TH1F_creation("hist_l6_back_g_P", "momentrum distribution of backward photon in layer6", 25, 0, 50, "P [MeV]", "Event");
    TH1F * hist_l6_for_e_P = TH1F_creation("hist_l6_for_e_P", "momentrum distribution of forward electron in layer6", 35, 0, 35, "P [MeV]", "Event");
    TH1F * hist_l6_back_e_P = TH1F_creation("hist_l6_back_e_P", "momentrum distribution of backward electron in layer6", 35, 0, 35, "P [MeV]", "Event");
    TH1F * hist_l6_for_n_P = TH1F_creation("hist_l6_for_n_P", "momentrum distribution of forward neutron in layer6", 40, 0, 200, "P [MeV]", "Event");
    TH1F * hist_l6_back_n_P = TH1F_creation("hist_l6_back_n_P", "momentrum distribution of backward neutron in layer6", 40, 0, 200, "P [MeV]", "Event");
    TH1F * hist_l6_for_n_KE = TH1F_creation("hist_l6_for_n_KE", "kinetic energy distribution of forward neutron in layer6", 20, 0, 20, "P [MeV]", "Event");
    TH1F * hist_l6_back_n_KE = TH1F_creation("hist_l6_back_n_KE", "kinetic energy distribution of backward neutron in layer6", 20, 0, 20, "P [MeV]", "Event");
    TH1F * hist_l6_for_eplus_P = TH1F_creation("hist_l6_for_eplus_P", "momentrum distribution of forward eplus in layer6", 30, 0, 30, "P [MeV]", "Event");
    TH1F * hist_l6_back_eplus_P = TH1F_creation("hist_l6_back_eplus_P", "momentrum distribution of backward eplus in layer6", 30, 0, 30, "P [MeV]", "Event");

    /// layer7
    TH1F * hist_l7_for_g_P = TH1F_creation("hist_l7_for_g_P", "momentrum distribution of forward photon in layer7", 25, 0, 50, "P [MeV]", "Event");
    TH1F * hist_l7_back_g_P = TH1F_creation("hist_l7_back_g_P", "momentrum distribution of backward photon in layer7", 25, 0, 50, "P [MeV]", "Event");
    TH1F * hist_l7_for_e_P = TH1F_creation("hist_l7_for_e_P", "momentrum distribution of forward electron in layer7", 35, 0, 35, "P [MeV]", "Event");
    TH1F * hist_l7_back_e_P = TH1F_creation("hist_l7_back_e_P", "momentrum distribution of backward electron in layer7", 35, 0, 35, "P [MeV]", "Event");
    TH1F * hist_l7_for_n_P = TH1F_creation("hist_l7_for_n_P", "momentrum distribution of forward neutron in layer7", 40, 0, 200, "P [MeV]", "Event");
    TH1F * hist_l7_back_n_P = TH1F_creation("hist_l7_back_n_P", "momentrum distribution of backward neutron in layer7", 40, 0, 200, "P [MeV]", "Event");
    TH1F * hist_l7_for_n_KE = TH1F_creation("hist_l7_for_n_KE", "kinetic energy distribution of forward neutron in layer7", 20, 0, 20, "P [MeV]", "Event");
    TH1F * hist_l7_back_n_KE = TH1F_creation("hist_l7_back_n_KE", "kinetic energy distribution of backward neutron in layer7", 20, 0, 20, "P [MeV]", "Event");
    TH1F * hist_l7_for_eplus_P = TH1F_creation("hist_l7_for_eplus_P", "momentrum distribution of forward eplus in layer7", 30, 0, 30, "P [MeV]", "Event");
    TH1F * hist_l7_back_eplus_P = TH1F_creation("hist_l7_back_eplus_P", "momentrum distribution of backward eplus in layer7", 30, 0, 30, "P [MeV]", "Event");

    /// layer8
    TH1F * hist_l8_for_g_P = TH1F_creation("hist_l8_for_g_P", "momentrum distribution of forward photon in layer8", 25, 0, 50, "P [MeV]", "Event");
    TH1F * hist_l8_back_g_P = TH1F_creation("hist_l8_back_g_P", "momentrum distribution of backward photon in layer8", 25, 0, 50, "P [MeV]", "Event");
    TH1F * hist_l8_for_e_P = TH1F_creation("hist_l8_for_e_P", "momentrum distribution of forward electron in layer8", 35, 0, 35, "P [MeV]", "Event");
    TH1F * hist_l8_back_e_P = TH1F_creation("hist_l8_back_e_P", "momentrum distribution of backward electron in layer8", 35, 0, 35, "P [MeV]", "Event");
    TH1F * hist_l8_for_n_P = TH1F_creation("hist_l8_for_n_P", "momentrum distribution of forward neutron in layer8", 40, 0, 200, "P [MeV]", "Event");
    TH1F * hist_l8_back_n_P = TH1F_creation("hist_l8_back_n_P", "momentrum distribution of backward neutron in layer8", 40, 0, 200, "P [MeV]", "Event");
    TH1F * hist_l8_for_n_KE = TH1F_creation("hist_l8_for_n_KE", "kinetic energy distribution of forward neutron in layer8", 20, 0, 20, "P [MeV]", "Event");
    TH1F * hist_l8_back_n_KE = TH1F_creation("hist_l8_back_n_KE", "kinetic energy distribution of backward neutron in layer8", 20, 0, 20, "P [MeV]", "Event");
    TH1F * hist_l8_for_eplus_P = TH1F_creation("hist_l8_for_eplus_P", "momentrum distribution of forward eplus in layer8", 30, 0, 30, "P [MeV]", "Event");
    TH1F * hist_l8_back_eplus_P = TH1F_creation("hist_l8_back_eplus_P", "momentrum distribution of backward eplus in layer8", 30, 0, 30, "P [MeV]", "Event");

    /// layer9
    TH1F * hist_l9_for_g_P = TH1F_creation("hist_l9_for_g_P", "momentrum distribution of forward photon in layer9", 25, 0, 50, "P [MeV]", "Event");
    TH1F * hist_l9_back_g_P = TH1F_creation("hist_l9_back_g_P", "momentrum distribution of backward photon in layer9", 25, 0, 50, "P [MeV]", "Event");
    TH1F * hist_l9_for_e_P = TH1F_creation("hist_l9_for_e_P", "momentrum distribution of forward electron in layer9", 35, 0, 35, "P [MeV]", "Event");
    TH1F * hist_l9_back_e_P = TH1F_creation("hist_l9_back_e_P", "momentrum distribution of backward electron in layer9", 35, 0, 35, "P [MeV]", "Event");
    TH1F * hist_l9_for_n_P = TH1F_creation("hist_l9_for_n_P", "momentrum distribution of forward neutron in layer9", 40, 0, 200, "P [MeV]", "Event");
    TH1F * hist_l9_back_n_P = TH1F_creation("hist_l9_back_n_P", "momentrum distribution of backward neutron in layer9", 40, 0, 200, "P [MeV]", "Event");
    TH1F * hist_l9_for_n_KE = TH1F_creation("hist_l9_for_n_KE", "kinetic energy distribution of forward neutron in layer9", 20, 0, 20, "P [MeV]", "Event");
    TH1F * hist_l9_back_n_KE = TH1F_creation("hist_l9_back_n_KE", "kinetic energy distribution of backward neutron in layer9", 20, 0, 20, "P [MeV]", "Event");
    TH1F * hist_l9_for_eplus_P = TH1F_creation("hist_l9_for_eplus_P", "momentrum distribution of forward eplus in layer9", 30, 0, 30, "P [MeV]", "Event");
    TH1F * hist_l9_back_eplus_P = TH1F_creation("hist_l9_back_eplus_P", "momentrum distribution of backward eplus in layer9", 30, 0, 30, "P [MeV]", "Event");

    TH1D * hist_l_for = TH1D_creation("hist_l_for", "layer statistics", 9, 0, 9, "layer", "count");
    TH1D * hist_l_back = TH1D_creation("hist_l_back", "layer statistics", 9, 0, 9, "layer", "count");

    hist_l_for->SetFillColor(4);
    hist_l_for->SetBarWidth(0.4);
    hist_l_for->SetBarOffset(0.1);
    hist_l_for->SetStats(0);
    hist_l_back->SetFillColor(2);
    hist_l_back->SetBarWidth(0.4);
    hist_l_back->SetBarOffset(0.5);
    hist_l_back->SetStats(0);

    hist_l_back->GetXaxis()->SetBinLabel(1, "l1");
    hist_l_back->GetXaxis()->SetBinLabel(2, "l2");
    hist_l_back->GetXaxis()->SetBinLabel(3, "l3");
    hist_l_back->GetXaxis()->SetBinLabel(4, "l4");
    hist_l_back->GetXaxis()->SetBinLabel(5, "l5");
    hist_l_back->GetXaxis()->SetBinLabel(6, "l6");
    hist_l_back->GetXaxis()->SetBinLabel(7, "l7");
    hist_l_back->GetXaxis()->SetBinLabel(8, "l8");
    hist_l_back->GetXaxis()->SetBinLabel(9, "l9");

    TFile f(qsim, "read");
    TTree * t = (TTree*)f.Get("T_Layer"); // input tree
    int pid, trid;
    double x, y, z, vx, vy, vz, theta, cos, phi;
    double p, e, m, px, py, pz;
    t->SetBranchAddress("layer.pid",  &pid);
    t->SetBranchAddress("layer.trid", &trid);
    t->SetBranchAddress("layer.x",    &x);
    t->SetBranchAddress("layer.y",    &y);
    t->SetBranchAddress("layer.z",    &z);
    t->SetBranchAddress("layer.vx",   &vx);
    t->SetBranchAddress("layer.vy",   &vy);
    t->SetBranchAddress("layer.vz",   &vz);
    t->SetBranchAddress("layer.px",   &px);
    t->SetBranchAddress("layer.py",   &py);
    t->SetBranchAddress("layer.pz",   &pz);
    t->SetBranchAddress("layer.p",    &p);
    t->SetBranchAddress("layer.e",    &e);
    t->SetBranchAddress("layer.m",    &m);

    int n = t->GetEntries();
    for (int i=0; i<n; i++){
	t->GetEntry(i);

	if (pz > 0){	// forward particles
	    if (z > 0){	// layer 9
		hist_l9_for_xy->Fill(x, y);
		if (pid == 22){	// photon
		    hist_l9_for_g_P->Fill(p);
		}
		else if (pid == 11){	// electron
		    hist_l9_for_e_P->Fill(p);
		}
		else if (pid == 2112){	// neutron
		    hist_l9_for_n_P->Fill(p);
		    hist_l9_for_n_KE->Fill(e-m);
		}
		else if (pid == -11) {	// positron
		    hist_l9_for_eplus_P->Fill(p);
		}
	    }
	    else if (z > -1*0.009){  // layer 8
		hist_l8_for_xy->Fill(x, y);
		if (pid == 22){	// photon
		    hist_l8_for_g_P->Fill(p);
		}
		else if (pid == 11){	// electron
		    hist_l8_for_e_P->Fill(p);
		}
		else if (pid == 2112){	// neutron
		    hist_l8_for_n_P->Fill(p);
		    hist_l8_for_n_KE->Fill(e-m);
		}
		else if (pid == -11) {	// positron 
		    hist_l8_for_eplus_P->Fill(p);
		}
	    }
	    else if (z > -2*0.009){  // layer 7
		hist_l7_for_xy->Fill(x, y);
		if (pid == 22){	// photon
		    hist_l7_for_g_P->Fill(p);
		}
		else if (pid == 11){	// electron
		    hist_l7_for_e_P->Fill(p);
		}
		else if (pid == 2112){	// neutron
		    hist_l7_for_n_P->Fill(p);
		    hist_l7_for_n_KE->Fill(e-m);
		}
		else if (pid == -11) {	// positron
		    hist_l7_for_eplus_P->Fill(p);
		}
	    }
	    else if (z > -3*0.009){  // layer 6
		hist_l6_for_xy->Fill(x, y);
		if (pid == 22){	// photon
		    hist_l6_for_g_P->Fill(p);
		}
		else if (pid == 11){	// electron
		    hist_l6_for_e_P->Fill(p);
		}
		else if (pid == 2112){	// neutron
		    hist_l6_for_n_P->Fill(p);
		    hist_l6_for_n_KE->Fill(e-m);
		}
		else if (pid == -11) {	// positron
		    hist_l6_for_eplus_P->Fill(p);
		}
	    }
	    else if (z > -4*0.009){  // layer 5
		hist_l5_for_xy->Fill(x, y);
		if (pid == 22){	// photon
		    hist_l5_for_g_P->Fill(p);
		}
		else if (pid == 11){	// electron
		    hist_l5_for_e_P->Fill(p);
		}
		else if (pid == 2112){	// neutron
		    hist_l5_for_n_P->Fill(p);
		    hist_l5_for_n_KE->Fill(e-m);
		}
		else if (pid == -11) {	// positron
		    hist_l5_for_eplus_P->Fill(p);
		}
	    }
	    else if (z > -5*0.009){  // layer 4
		hist_l4_for_xy->Fill(x, y);
		if (pid == 22){	// photon
		    hist_l4_for_g_P->Fill(p);
		}
		else if (pid == 11){	// electron
		    hist_l4_for_e_P->Fill(p);
		}
		else if (pid == 2112){	// neutron
		    hist_l4_for_n_P->Fill(p);
		    hist_l4_for_n_KE->Fill(e-m);
		}
		else if (pid == -11) {	// positron
		    hist_l4_for_eplus_P->Fill(p);
		}
	    }
	    else if (z > -6*0.009){  // layer 3
		hist_l3_for_xy->Fill(x, y);
		if (pid == 22){	// photon
		    hist_l3_for_g_P->Fill(p);
		}
		else if (pid == 11){	// electron
		    hist_l3_for_e_P->Fill(p);
		}
		else if (pid == 2112){	// neutron
		    hist_l3_for_n_P->Fill(p);
		    hist_l3_for_n_KE->Fill(e-m);
		}
		else if (pid == -11) {	// positron
		    hist_l3_for_eplus_P->Fill(p);
		}
	    }
	    else if (z > -7*0.009){  // layer 2
		hist_l2_for_xy->Fill(x, y);
		if (pid == 22){	// photon
		    hist_l2_for_g_P->Fill(p);
		}
		else if (pid == 11){	// electron
		    hist_l2_for_e_P->Fill(p);
		}
		else if (pid == 2112){	// neutron
		    hist_l2_for_n_P->Fill(p);
		    hist_l2_for_n_KE->Fill(e-m);
		}
		else if (pid == -11) {	// positron
		    hist_l2_for_eplus_P->Fill(p);
		}
	    }
	    else if (z > -8*0.009){  // layer 1
		hist_l1_for_xy->Fill(x, y);
		if (pid == 22){	// photon
		    hist_l1_for_g_P->Fill(p);
		}
		else if (pid == 11){	// electron
		    hist_l1_for_e_P->Fill(p);
		}
		else if (pid == 2112){	// neutron
		    hist_l1_for_n_P->Fill(p);
		    hist_l1_for_n_KE->Fill(e-m);
		}
		else if (pid == -11) {	// positron
		    hist_l1_for_eplus_P->Fill(p);
		}
	    }
	}
	else { // backward
	    if (z > 0){	// layer 9
		hist_l9_back_xy->Fill(x, y);
		if (pid == 22){	// photon
		    hist_l9_back_g_P->Fill(p);
		}
		else if (pid == 11){	// electron
		    hist_l9_back_e_P->Fill(p);
		}
		else if (pid == 2112){	// neutron
		    hist_l9_back_n_P->Fill(p);
		    hist_l9_back_n_KE->Fill(e-m);
		}
		else if (pid == -11) {	// positron
		    hist_l9_back_eplus_P->Fill(p);
		}
	    }
	    else if (z > -1*0.009){  // layer 8
		hist_l8_back_xy->Fill(x, y);
		if (pid == 22){	// photon
		    hist_l8_back_g_P->Fill(p);
		}
		else if (pid == 11){	// electron
		    hist_l8_back_e_P->Fill(p);
		}
		else if (pid == 2112){	// neutron
		    hist_l8_back_n_P->Fill(p);
		    hist_l8_back_n_KE->Fill(e-m);
		}
		else if (pid == -11) {	// positron 
		    hist_l8_back_eplus_P->Fill(p);
		}
	    }
	    else if (z > -2*0.009){  // layer 7
		hist_l7_back_xy->Fill(x, y);
		if (pid == 22){	// photon
		    hist_l7_back_g_P->Fill(p);
		}
		else if (pid == 11){	// electron
		    hist_l7_back_e_P->Fill(p);
		}
		else if (pid == 2112){	// neutron
		    hist_l7_back_n_P->Fill(p);
		    hist_l7_back_n_KE->Fill(e-m);
		}
		else if (pid == -11) {	// positron
		    hist_l7_back_eplus_P->Fill(p);
		}
	    }
	    else if (z > -3*0.009){  // layer 6
		hist_l6_back_xy->Fill(x, y);
		if (pid == 22){	// photon
		    hist_l6_back_g_P->Fill(p);
		}
		else if (pid == 11){	// electron
		    hist_l6_back_e_P->Fill(p);
		}
		else if (pid == 2112){	// neutron
		    hist_l6_back_n_P->Fill(p);
		    hist_l6_back_n_KE->Fill(e-m);
		}
		else if (pid == -11) {	// positron
		    hist_l6_back_eplus_P->Fill(p);
		}
	    }
	    else if (z > -4*0.009){  // layer 5
		hist_l5_back_xy->Fill(x, y);
		if (pid == 22){	// photon
		    hist_l5_back_g_P->Fill(p);
		}
		else if (pid == 11){	// electron
		    hist_l5_back_e_P->Fill(p);
		}
		else if (pid == 2112){	// neutron
		    hist_l5_back_n_P->Fill(p);
		    hist_l5_back_n_KE->Fill(e-m);
		}
		else if (pid == -11) {	// positron
		    hist_l5_back_eplus_P->Fill(p);
		}
	    }
	    else if (z > -5*0.009){  // layer 4
		hist_l4_back_xy->Fill(x, y);
		if (pid == 22){	// photon
		    hist_l4_back_g_P->Fill(p);
		}
		else if (pid == 11){	// electron
		    hist_l4_back_e_P->Fill(p);
		}
		else if (pid == 2112){	// neutron
		    hist_l4_back_n_P->Fill(p);
		    hist_l4_back_n_KE->Fill(e-m);
		}
		else if (pid == -11) {	// positron
		    hist_l4_back_eplus_P->Fill(p);
		}
	    }
	    else if (z > -6*0.009){  // layer 3
		hist_l3_back_xy->Fill(x, y);
		if (pid == 22){	// photon
		    hist_l3_back_g_P->Fill(p);
		}
		else if (pid == 11){	// electron
		    hist_l3_back_e_P->Fill(p);
		}
		else if (pid == 2112){	// neutron
		    hist_l3_back_n_P->Fill(p);
		    hist_l3_back_n_KE->Fill(e-m);
		}
		else if (pid == -11) {	// positron
		    hist_l3_back_eplus_P->Fill(p);
		}
	    }
	    else if (z > -7*0.009){  // layer 2
		hist_l2_back_xy->Fill(x, y);
		if (pid == 22){	// photon
		    hist_l2_back_g_P->Fill(p);
		}
		else if (pid == 11){	// electron
		    hist_l2_back_e_P->Fill(p);
		}
		else if (pid == 2112){	// neutron
		    hist_l2_back_n_P->Fill(p);
		    hist_l2_back_n_KE->Fill(e-m);
		}
		else if (pid == -11) {	// positron
		    hist_l2_back_eplus_P->Fill(p);
		}
	    }
	    else if (z > -8*0.009){  // layer 1
		hist_l1_back_xy->Fill(x, y);
		if (pid == 22){	// photon
		    hist_l1_back_g_P->Fill(p);
		}
		else if (pid == 11){	// electron
		    hist_l1_back_e_P->Fill(p);
		}
		else if (pid == 2112){	// neutron
		    hist_l1_back_n_P->Fill(p);
		    hist_l1_back_n_KE->Fill(e-m);
		}
		else if (pid == -11) {	// positron
		    hist_l1_back_eplus_P->Fill(p);
		}
	    }
	}
    }

    hist_l_for->SetBinContent(1, hist_l1_for_xy->GetEntries());
    hist_l_back->SetBinContent(1, hist_l1_back_xy->GetEntries());
    hist_l_for->SetBinContent(2, hist_l2_for_xy->GetEntries());
    hist_l_back->SetBinContent(2, hist_l2_back_xy->GetEntries());
    hist_l_for->SetBinContent(3, hist_l3_for_xy->GetEntries());
    hist_l_back->SetBinContent(3, hist_l3_back_xy->GetEntries());
    hist_l_for->SetBinContent(4, hist_l4_for_xy->GetEntries());
    hist_l_back->SetBinContent(4, hist_l4_back_xy->GetEntries());
    hist_l_for->SetBinContent(5, hist_l5_for_xy->GetEntries());
    hist_l_back->SetBinContent(5, hist_l5_back_xy->GetEntries());
    hist_l_for->SetBinContent(6, hist_l6_for_xy->GetEntries());
    hist_l_back->SetBinContent(6, hist_l6_back_xy->GetEntries());
    hist_l_for->SetBinContent(7, hist_l7_for_xy->GetEntries());
    hist_l_back->SetBinContent(7, hist_l7_back_xy->GetEntries());
    hist_l_for->SetBinContent(8, hist_l8_for_xy->GetEntries());
    hist_l_back->SetBinContent(8, hist_l8_back_xy->GetEntries());
    hist_l_for->SetBinContent(9, hist_l9_for_xy->GetEntries());
    hist_l_back->SetBinContent(9, hist_l9_back_xy->GetEntries());

    // output
    ofstream fout("statistics.txt", std::ifstream::app);
    if (! fout.good() )
    {
	cout << "Fail to open file 'statistics.txt'" << endl;
	exit(10);
    }
    fout << "Total hits:\t" << n << "\n"
	 << "layer1\t\tforward:\t" << hist_l1_for_xy->GetEntries() << "\tbackward:\t" << hist_l1_back_xy->GetEntries() << "\n"
	 << "layer2\t\tforward:\t" << hist_l2_for_xy->GetEntries() << "\tbackward:\t" << hist_l2_back_xy->GetEntries() << "\n"
	 << "layer3\t\tforward:\t" << hist_l3_for_xy->GetEntries() << "\tbackward:\t" << hist_l3_back_xy->GetEntries() << "\n"
	 << "layer4\t\tforward:\t" << hist_l4_for_xy->GetEntries() << "\tbackward:\t" << hist_l4_back_xy->GetEntries() << "\n"
	 << "layer5\t\tforward:\t" << hist_l5_for_xy->GetEntries() << "\tbackward:\t" << hist_l5_back_xy->GetEntries() << "\n"
	 << "layer6\t\tforward:\t" << hist_l6_for_xy->GetEntries() << "\tbackward:\t" << hist_l6_back_xy->GetEntries() << "\n"
	 << "layer7\t\tforward:\t" << hist_l7_for_xy->GetEntries() << "\tbackward:\t" << hist_l7_back_xy->GetEntries() << "\n"
	 << "layer8\t\tforward:\t" << hist_l8_for_xy->GetEntries() << "\tbackward:\t" << hist_l8_back_xy->GetEntries() << "\n"
	 << "layer9\t\tforward:\t" << hist_l9_for_xy->GetEntries() << "\tbackward:\t" << hist_l9_back_xy->GetEntries() << endl << endl;
    fout.close();


    TCanvas * c1 = new TCanvas("c1", "c1", 600, 450);
    TCanvas * c2 = new TCanvas("c2", "c2", 600, 900);
    c2->Divide(1,2);
    TCanvas * c4 = new TCanvas("c4", "c4", 1200, 1000);
    c4->Divide(2,2);

    draw(c2, hist_l1_for_xy, hist_l1_back_xy, "l1_xy.png", "COLZ", "COLZ");
    draw(c2, hist_l2_for_xy, hist_l2_back_xy, "l2_xy.png", "COLZ", "COLZ");
    draw(c2, hist_l3_for_xy, hist_l3_back_xy, "l3_xy.png", "COLZ", "COLZ");
    draw(c2, hist_l4_for_xy, hist_l4_back_xy, "l4_xy.png", "COLZ", "COLZ");
    draw(c2, hist_l5_for_xy, hist_l5_back_xy, "l5_xy.png", "COLZ", "COLZ");
    draw(c2, hist_l6_for_xy, hist_l6_back_xy, "l6_xy.png", "COLZ", "COLZ");
    draw(c2, hist_l7_for_xy, hist_l7_back_xy, "l7_xy.png", "COLZ", "COLZ");
    draw(c2, hist_l8_for_xy, hist_l8_back_xy, "l8_xy.png", "COLZ", "COLZ");
    draw(c2, hist_l9_for_xy, hist_l9_back_xy, "l9_xy.png", "COLZ", "COLZ");

    c4->GetPad(1)->SetLogy();
    draw(c4, hist_l1_for_g_P, hist_l1_for_e_P, hist_l1_for_n_P, hist_l1_for_eplus_P, "l1_P_for.png", "COLZ", "COLZ", "COLZ", "COLZ");
    draw(c4, hist_l1_back_g_P, hist_l1_back_e_P, hist_l1_back_n_P, hist_l1_back_eplus_P, "l1_P_back.png", "COLZ", "COLZ", "COLZ", "COLZ");
    draw(c4, hist_l2_for_g_P, hist_l2_for_e_P, hist_l2_for_n_P, hist_l2_for_eplus_P, "l2_P_for.png", "COLZ", "COLZ", "COLZ", "COLZ");
    draw(c4, hist_l2_back_g_P, hist_l2_back_e_P, hist_l2_back_n_P, hist_l2_back_eplus_P, "l2_P_back.png", "COLZ", "COLZ", "COLZ", "COLZ");
    draw(c4, hist_l3_for_g_P, hist_l3_for_e_P, hist_l3_for_n_P, hist_l3_for_eplus_P, "l3_P_for.png", "COLZ", "COLZ", "COLZ", "COLZ");
    draw(c4, hist_l3_back_g_P, hist_l3_back_e_P, hist_l3_back_n_P, hist_l3_back_eplus_P, "l3_P_back.png", "COLZ", "COLZ", "COLZ", "COLZ");
    draw(c4, hist_l4_for_g_P, hist_l4_for_e_P, hist_l4_for_n_P, hist_l4_for_eplus_P, "l4_P_for.png", "COLZ", "COLZ", "COLZ", "COLZ");
    draw(c4, hist_l4_back_g_P, hist_l4_back_e_P, hist_l4_back_n_P, hist_l4_back_eplus_P, "l4_P_back.png", "COLZ", "COLZ", "COLZ", "COLZ");
    draw(c4, hist_l5_for_g_P, hist_l5_for_e_P, hist_l5_for_n_P, hist_l5_for_eplus_P, "l5_P_for.png", "COLZ", "COLZ", "COLZ", "COLZ");
    draw(c4, hist_l5_back_g_P, hist_l5_back_e_P, hist_l5_back_n_P, hist_l5_back_eplus_P, "l5_P_back.png", "COLZ", "COLZ", "COLZ", "COLZ");
    draw(c4, hist_l6_for_g_P, hist_l6_for_e_P, hist_l6_for_n_P, hist_l6_for_eplus_P, "l6_P_for.png", "COLZ", "COLZ", "COLZ", "COLZ");
    draw(c4, hist_l6_back_g_P, hist_l6_back_e_P, hist_l6_back_n_P, hist_l6_back_eplus_P, "l6_P_back.png", "COLZ", "COLZ", "COLZ", "COLZ");
    draw(c4, hist_l7_for_g_P, hist_l7_for_e_P, hist_l7_for_n_P, hist_l7_for_eplus_P, "l7_P_for.png", "COLZ", "COLZ", "COLZ", "COLZ");
    draw(c4, hist_l7_back_g_P, hist_l7_back_e_P, hist_l7_back_n_P, hist_l7_back_eplus_P, "l7_P_back.png", "COLZ", "COLZ", "COLZ", "COLZ");
    draw(c4, hist_l8_for_g_P, hist_l8_for_e_P, hist_l8_for_n_P, hist_l8_for_eplus_P, "l8_P_for.png", "COLZ", "COLZ", "COLZ", "COLZ");
    draw(c4, hist_l8_back_g_P, hist_l8_back_e_P, hist_l8_back_n_P, hist_l8_back_eplus_P, "l8_P_back.png", "COLZ", "COLZ", "COLZ", "COLZ");
    draw(c4, hist_l8_for_g_P, hist_l9_for_e_P, hist_l9_for_n_P, hist_l9_for_eplus_P, "l9_P_for.png", "COLZ", "COLZ", "COLZ", "COLZ");
    draw(c4, hist_l8_back_g_P, hist_l9_back_e_P, hist_l9_back_n_P, hist_l9_back_eplus_P, "l9_P_back.png", "COLZ", "COLZ", "COLZ", "COLZ");

    c1->cd();
    c1->SetLogy();
    TLegend * leg = new TLegend(0.8, 0.8, 0.9, 0.9);
    leg->AddEntry(hist_l_for, "forward", "f");
    leg->AddEntry(hist_l_back, "backward", "f");
    hist_l_back->Draw("b");
    hist_l_for->Draw("b same");
    leg->Draw("same");
    c1->SaveAs("layer_for_back.png");

    for (int i=0; i<n_hist; ++i){
	hist_pool[i]->Delete();
    }

    c1->Close();
    c2->Close();
    c4->Close();
    f.Close();
}


#ifndef __CINT__
int main(int argc, char** argv) 
{
    layer(argv[1]);
    return 0;
}
#endif
