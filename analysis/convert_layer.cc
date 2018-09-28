#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

#define __IO_MAXHIT 10000
void convert(const char * qsim) {   // convert events tree into monitor tree and layer tree (slim layer tree by trackID)

    TFile f(qsim, "read");
    TFile * f_mon = new TFile("mon.root", "recreate");
    TFile * f_layer = new TFile("layer.root", "recreate");
    TTree * t = (TTree*)f.Get("T"); // input tree
    TTree * t_mon = new TTree("T_Mon", "Tree of Monitor Hits");   // monitor tree
    t_mon->SetDirectory(f_mon);
    TTree * t_layer = new TTree("T_Layer", "Tree of Layer Hits");   // layer tree
    int mon_pid[__IO_MAXHIT], mon_trid[__IO_MAXHIT], mon_n;
    double mon_x[__IO_MAXHIT], mon_y[__IO_MAXHIT], mon_z[__IO_MAXHIT], mon_vx[__IO_MAXHIT], mon_vy[__IO_MAXHIT], mon_vz[__IO_MAXHIT];
    double mon_p[__IO_MAXHIT], mon_e[__IO_MAXHIT], mon_m[__IO_MAXHIT], mon_px[__IO_MAXHIT], mon_py[__IO_MAXHIT], mon_pz[__IO_MAXHIT];
    int layer_pid[__IO_MAXHIT], layer_trid[__IO_MAXHIT], layer_n;
    double layer_x[__IO_MAXHIT], layer_y[__IO_MAXHIT], layer_z[__IO_MAXHIT], layer_vx[__IO_MAXHIT], layer_vy[__IO_MAXHIT], layer_vz[__IO_MAXHIT];
    double layer_p[__IO_MAXHIT], layer_e[__IO_MAXHIT], layer_m[__IO_MAXHIT], layer_px[__IO_MAXHIT], layer_py[__IO_MAXHIT], layer_pz[__IO_MAXHIT];
    int pid, tid;
    vector<int> trid;	// track id of monitor hits
    double x, y, z, vx, vy, vz, theta, cos, phi;
    double p, e, m, px, py, pz;

    t->SetBranchAddress("mon.n",    &mon_n);
    t->SetBranchAddress("mon.pid",  mon_pid);
    t->SetBranchAddress("mon.trid", mon_trid);
    t->SetBranchAddress("mon.x",    mon_x);
    t->SetBranchAddress("mon.y",    mon_y);
    t->SetBranchAddress("mon.z",    mon_z);
    t->SetBranchAddress("mon.vx",   mon_vx);
    t->SetBranchAddress("mon.vy",   mon_vy);
    t->SetBranchAddress("mon.vz",   mon_vz);
    t->SetBranchAddress("mon.p",    mon_p);
    t->SetBranchAddress("mon.e",    mon_e);
    t->SetBranchAddress("mon.m",    mon_m);
    t->SetBranchAddress("mon.px",   mon_px);
    t->SetBranchAddress("mon.py",   mon_py);
    t->SetBranchAddress("mon.pz",   mon_pz);
    t->SetBranchAddress("layer.n",	&layer_n);
    t->SetBranchAddress("layer.pid",	layer_pid);
    t->SetBranchAddress("layer.trid",	layer_trid);
    t->SetBranchAddress("layer.x",	layer_x);
    t->SetBranchAddress("layer.y",	layer_y);
    t->SetBranchAddress("layer.z",	layer_z);
    t->SetBranchAddress("layer.vx",	layer_vx);
    t->SetBranchAddress("layer.vy",	layer_vy);
    t->SetBranchAddress("layer.vz",	layer_vz);
    t->SetBranchAddress("layer.p",	layer_p);
    t->SetBranchAddress("layer.e",	layer_e);
    t->SetBranchAddress("layer.m",	layer_m);
    t->SetBranchAddress("layer.px",	layer_px);
    t->SetBranchAddress("layer.py",	layer_py);
    t->SetBranchAddress("layer.pz",	layer_pz);

    // monitor output tree
    t_mon->Branch("mon.pid",	&pid,	"mon.pid/I");
    t_mon->Branch("mon.trid",	&tid,	"mon.trid/I");
    t_mon->Branch("mon.x",	&x,	"mon.x/D");
    t_mon->Branch("mon.y",	&y,	"mon.y/D");
    t_mon->Branch("mon.z",	&z,	"mon.z/D");
    t_mon->Branch("mon.cos",	&cos,	"mon.cos/D");	// cos(theta) = pz/p;
    t_mon->Branch("mon.theta",	&theta,	"mon.theta/D");
    t_mon->Branch("mon.phi",	&phi,	"mon.phi/D");
    t_mon->Branch("mon.vx",	&vx,	"mon.vx/D");
    t_mon->Branch("mon.vy", 	&vy,	"mon.vy/D");
    t_mon->Branch("mon.vz", 	&vz,	"mon.vz/D");
    t_mon->Branch("mon.p",	&p,	"mon.p/D");
    t_mon->Branch("mon.e", 	&e,	"mon.e/D");
    t_mon->Branch("mon.m", 	&m,	"mon.m/D");
    t_mon->Branch("mon.px",	&px,	"mon.px/D");
    t_mon->Branch("mon.py", 	&py,	"mon.py/D");
    t_mon->Branch("mon.pz", 	&pz,	"mon.pz/D");

    // layer output tree
    t_layer->Branch("layer.pid",    &pid,	"layer.pid/I");
    t_layer->Branch("layer.trid",   &tid,  	"layer.trid/I");
    t_layer->Branch("layer.x",	    &x,	    	"layer.x/D");
    t_layer->Branch("layer.y",	    &y,	    	"layer.y/D");
    t_layer->Branch("layer.z",	    &z,	    	"layer.z/D");
    t_layer->Branch("layer.vx",	    &vx,    	"layer.vx/D");
    t_layer->Branch("layer.vy",     &vy,    	"layer.vy/D");
    t_layer->Branch("layer.vz",     &vz,    	"layer.vz/D");
    t_layer->Branch("layer.p",	    &p,		"layer.p/D");
    t_layer->Branch("layer.e",	    &e,		"layer.e/D");
    t_layer->Branch("layer.m",	    &m,		"layer.m/D");
    t_layer->Branch("layer.px",	    &px,    	"layer.px/D");
    t_layer->Branch("layer.py",     &py,    	"layer.py/D");
    t_layer->Branch("layer.pz",     &pz,    	"layer.pz/D");

    int n = t->GetEntries();
    int n0 = 0;	// events without bounding back
    for (int i=0; i<n; ++i){
	t->GetEntry(i);

	if (mon_n == 0) {
	    n0++;
	    continue;
	}

	trid.clear();
	for (int j=0; j<mon_n; ++j) {
	    pid = mon_pid[j];
	    tid = mon_trid[j];
	    trid.push_back(mon_trid[j]);
	    x	= mon_x[j];
	    y	= mon_y[j];
	    z	= mon_z[j];
	    vx	= mon_vx[j];
	    vy	= mon_vy[j];
	    vz	= mon_vz[j];
	    p	= mon_p[j];
	    e	= mon_e[j];
	    m	= mon_m[j];
	    px	= mon_px[j];
	    py	= mon_py[j];
	    pz	= mon_pz[j];
	    cos = pz/p;
	    theta = TMath::ACos(pz/p)*180.0/TMath::Pi();
	    phi = TMath::ATan(py/px)*180.0/TMath::Pi();
	    if (px<0){
		if(py>0) phi += 180;
		else phi -= 180;
	    }
	    t_mon->Fill();
	}

	for (int j=0; j<layer_n; ++j) {
	    auto it = std::find(trid.begin(), trid.end(), layer_trid[j]);
	    if (it != trid.end()){
		pid = layer_pid[j];
		tid = layer_trid[j];
		x   = layer_x[j];
		y   = layer_y[j];
		z   = layer_z[j];
		vx  = layer_vx[j];
		vy  = layer_vy[j];
		vz  = layer_vz[j];
		px  = layer_px[j];
		py  = layer_py[j];
		pz  = layer_pz[j];
		p   = layer_p[j];
		e   = layer_e[j];
		t_layer->Fill();
	    }
	}
    }

    // output
    ofstream fout("statistics.txt", std::ifstream::app);
    if (! fout.good() )
    {
	cout << "Fail to open file 'statistics.txt'" << endl;
	exit(10);
    }
    fout << "Total events:\t" << n << "\n"
	 << "Number of event without bounding back:\t" << n0 << "\t" << 100.0*n0/n << "%\n"
         << "Monitor hits:\t"  << t_mon->GetEntries() << "\t" << 1.0*t_mon->GetEntries()/n << "\n" 
	 << "Layer hits:\t" << t_layer->GetEntries() << endl << endl;
    fout.close();

    f_mon->cd();
    t_mon->Write();
    f_mon->Close();
    f_layer->cd();
    t_layer->Write();
    f_layer->Close();
    f.Close();
}


#ifndef __CINT__
int main(int argc, char** argv) 
{
    convert(argv[1]);
    return 0;
}
#endif
