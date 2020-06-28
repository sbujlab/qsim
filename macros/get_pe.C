#include<iostream>
#include<fstream>

using namespace std;

                   /*                                                                                           
const double w = 0.5473;                                                                                      
const double Q0 = 73.91;                                                                                      
const double sigma_0 = 0.5788;                                                                                
const double alpha = 0.06981;                                                                                 
const double mu = 5.871;
const double Q1 = 25.4;                                                                                       
const double sigma_1 = 12.15; 

//Sub functions
static const double degtorad = TMath::Pi()/180.;                                                              
                                                                                                              
static const double twopi = 2*TMath::Pi();                                                                    
                                                                                                              
Double_t fit_func_yxzhao_pe(Double_t *x, Double_t *par);                                                      
Double_t fit_func_yxzhao_bg(Double_t *x, Double_t *par);
*/

int det_id=2;  //1 for the upstream, 2 for the downstream

void get_pe(std::string fname = "qsim.root", double user_reflectivity = 0.9, double user_cerenkov = 1.0, double user_scintillation = 1.0, double user_z_pos = -11.0){
	
	TFile *root_file = new TFile(fname.c_str());
	TTree *T = (TTree*)root_file->Get("T");

	int hit_n=0;
	int hit_det[100000]={0};
	int hit_pid[100000]={0};
	double hit_x[100000] = {0};
	double hit_z[100000] = {0};
    double hit_e[100000] = {0};

	double hit_y[100000] = {0};


	T->SetBranchAddress("hit.n",&hit_n);
	T->SetBranchAddress("hit.det",hit_det);
	T->SetBranchAddress("hit.pid",hit_pid);
	T->SetBranchAddress("hit.x", hit_x);
	T->SetBranchAddress("hit.y", hit_y);
	T->SetBranchAddress("hit.z", hit_z);
    T->SetBranchAddress("hit.e", hit_e);

	long N_entries=T->GetEntries();

	TH1F *h_npe=new TH1F("npe","number of p.e",30,0,30);
	TH1F *h_npeN=new TH1F("npeN","Normalized number of p.e",30,0,30);
    TH1F *h = new TH1F("h", "Photon Energy", 10000, 0, 10);
	//#TH2F *h_hp = new TH2F("hp", "Hit position", 1000, -.1, .1, 1000, .2, .4);	
	//TH1F *h_QDC_sim=new TH1F("h_QDC_sim","smeared G4 simulation in QDC channel",4096,0,4096);

	int bins[100000] = {0};
	int sum = 0;

	for(long i=0; i<N_entries; i++){ //loop events
		T->GetEntry(i);
		//cout<<hit_n<<endl;
		
		double npe = 0;

		for(int j=0;j<hit_n;j++){ //loop hits
			if(hit_pid[j]==0 && hit_det[j]==det_id){  //if this hit is optical photon, and it's hit on the cathode
				//#!npe++;
				//change to fit the effective radius of the cathode
				//if(sqrt(pow(hit_x[j], 2)+pow((hit_z[j]-.3048), 2)) < .023){
			//	if(sqrt(pow(hit_x[j], 2)+pow(hit_y[j],2)) < .023){
					//#h_hp->Fill(hit_x[j], hit_z[j]);
					npe++;
					sum++;
                    //h->Fill(hit_e[j] / (10^-9));
			//	}
			}
		}
/*
		//  This is the smear
  		double parameters[8]={w,Q0, sigma_0,alpha,mu, Q1, sigma_1, npe  };                                    
                                                                                                              
        	TF1 *func_gen;                                                                                        
        	
		if(npe==0){                                                                                           
            		func_gen=new TF1("func_gen",fit_func_yxzhao_bg,0,4096,5);                                         
            		func_gen->SetParameters(parameters);                                                              
        	}else{                                                                                                
            		func_gen=new TF1("func_gen", fit_func_yxzhao_pe, 0, 4096, 8);                                     
            		func_gen->SetParameters(parameters);                                                              
        	}                                                                                                     
                                                                                                              
        	double QDC_chan_gen=func_gen->GetRandom();                                                            
  	        cout<<"#####################    "<<QDC_chan_gen<<endl;                                                
                                                                                                              
    	        //h_QDC_sim->Fill(QDC_chan_gen);                                                                        
                                                                                                              
	        delete func_gen; 
*/
		bins[i] = npe;
	}

	cout<<sum<<endl;
	for(int k = 0; k<N_entries; k++){
		h_npe->Fill(bins[k]);
		h_npeN->Fill(bins[k], 1.0/sum);
	}
	//h_QDC_sim->SetAxisRange(68, 600);

    TCanvas * c1 = new TCanvas();
    c1->Divide(2,1);
    c1->cd(1);
	h_npe->Draw();
    gPad->SetLogy();
    c1->cd(2);
    h_npeN->Draw();
    gPad->SetLogy();
    std::cout << "Mean PE/event = " << h_npe->GetMean() << ", RMS = " << h_npe->GetRMS() << ", and resolution = " << h_npe->GetRMS()/h_npe->GetMean() << " for " << N_entries << " entries" << std::endl;

    std::string fnameOutPDF = fname.substr(0,fname.find(".root")) + ".pdf";
    c1->SaveAs(fnameOutPDF.c_str());

    // Save data
    double ref_x_pos = atof((fname.substr(fname.find("ees_")+4,fname.find("_x")-fname.find("ees_")-4)).c_str());
    double ref_angle = atof((fname.substr(fname.find("out_")+4,fname.find("_degrees")-fname.find("out_")-4)).c_str());
    double ref_reflectivity = user_reflectivity;
    double ref_cerenkov = user_cerenkov;
    double ref_scintillation = user_scintillation;
    double ref_z_pos = user_z_pos;

    double oldx_pos = 0.0;
    double oldangle = 0.0;
    double oldavg     = 0.0;
    double oldavg_err = 0.0;
    double oldrms     = 0.0;
    double oldrms_err = 0.0;
    double oldres     = 0.0;
    double oldN_en    = 0.0;
    double oldreflectivity = 0.0;
    double oldcerenkov = 0.0;
    double oldscintillation = 0.0;
    double oldz_pos = 0.0;

    double x_pos = 0.0;
    double angle = 0.0;
    double avg     = 0.0;
    double avg_err = 0.0;
    double rms     = 0.0;
    double rms_err = 0.0;
    double res     = 0.0;
    double N_en    = 0.0;
    double reflectivity = 0.0;
    double cerenkov = 0.0;
    double scintillation = 0.0;
    double z_pos = 0.0;

    std::cout << "X = " << ref_x_pos << ", angle = " << ref_angle <<std::endl;

    TFile new_file(Form("localTmp.root"),"recreate");
    new_file.cd();
    TTree* newtree;

    TFile* old_file;
    TTree* oldtree;
    if (!gSystem->AccessPathName("scans.root")) {
        // Old file exists, read it and add new entries
        old_file = TFile::Open("scans.root");
        old_file->GetObject("scans", oldtree);
        new_file.cd();
        if (!oldtree) {
            std::cout << "ERROR: Dead scans tree" ;
            return;
        }
        newtree = oldtree->CloneTree(0);
        int nent = oldtree->GetEntries();

//        TLeaf* angleL = oldtree->GetLeaf("angle");
//        TLeaf* x_posL = oldtree->GetLeaf("x_pos");

        // Clear out prior instance if exists
        bool prior = true;
        if (oldtree->GetBranch("reflectivity")) {
            oldtree->SetBranchAddress("reflectivity",&oldreflectivity);
            newtree->SetBranchAddress("reflectivity",&reflectivity);
        }
        else {
            newtree->Branch("reflectivity",&reflectivity);
            prior = false;
        }
        if (oldtree->GetBranch("cerenkov")) {
            oldtree->SetBranchAddress("cerenkov",&oldcerenkov);
            newtree->SetBranchAddress("cerenkov",&cerenkov);
        }
        else {
            newtree->Branch("cerenkov",&cerenkov);
            prior = false;
        }
        if (oldtree->GetBranch("scintillation")) {
            oldtree->SetBranchAddress("scintillation",&oldscintillation);
            newtree->SetBranchAddress("scintillation",&scintillation);
        }
        else {
            newtree->Branch("scintillation",&scintillation);
            prior = false;
        }
        if (oldtree->GetBranch("z_pos")) {
            oldtree->SetBranchAddress("z_pos",&oldz_pos);
            newtree->SetBranchAddress("z_pos",&z_pos);
        }
        else {
            newtree->Branch("z_pos",&z_pos);
            prior = false;
        }
        oldtree->SetBranchAddress("angle",&oldangle);
        oldtree->SetBranchAddress("x_pos",&oldx_pos);
        oldtree->SetBranchAddress("avg_pes",&oldavg);
        oldtree->SetBranchAddress("avg_pes_err",&oldavg_err);
        oldtree->SetBranchAddress("rms_pes",&oldrms);
        oldtree->SetBranchAddress("rms_pes_err",&oldrms_err);
        oldtree->SetBranchAddress("res",&oldres);
        oldtree->SetBranchAddress("nentries",&oldN_en);
        newtree->SetBranchAddress("angle",&angle);
        newtree->SetBranchAddress("x_pos",&x_pos);
        newtree->SetBranchAddress("avg_pes",&avg);
        newtree->SetBranchAddress("avg_pes_err",&avg_err);
        newtree->SetBranchAddress("rms_pes",&rms);
        newtree->SetBranchAddress("rms_pes_err",&rms_err);
        newtree->SetBranchAddress("res",&res);
        newtree->SetBranchAddress("nentries",&N_en);
        for (auto j : ROOT::TSeqI(nent) ) {
//            x_posL->GetBranch()->GetEntry(j);
//            angleL->GetBranch()->GetEntry(j);
            oldtree->GetEntry(j);

            if (ref_x_pos == oldx_pos && ref_angle == oldangle && (!prior || (ref_reflectivity == oldreflectivity && ref_cerenkov == oldcerenkov && ref_scintillation == oldscintillation && ref_z_pos == oldz_pos))) {
            //if (ref_x_pos == x_posL->GetValue() && ref_angle == angleL->GetValue()) 
                std::cout << "TEST 1" << std::endl;
                continue;
            }
            x_pos   = oldx_pos;
            angle   = oldangle;
            avg     = oldavg;
            avg_err = oldavg_err;
            rms     = oldrms;
            rms_err = oldrms_err;
            res     = oldres;
            N_en    = oldN_en;
            reflectivity = oldreflectivity;
            cerenkov = oldcerenkov;
            scintillation = oldscintillation;
            z_pos = oldz_pos;
            if (!oldtree->GetBranch("reflectivity")) {
                reflectivity = 0.9;
            }
            if (!oldtree->GetBranch("cerenkov")) {
                cerenkov = 1.0;
            }
            if (!oldtree->GetBranch("scintillation")) {
                scintillation = 1.0;
            }
            if (!oldtree->GetBranch("z_pos")) {
                z_pos = -11.0;
            }
            newtree->Fill();
        }

        // Append current run to end
        old_file->Close();
        gSystem->Exec("rm scans.root");
        delete old_file;
    }
    else {
        // Old file doesn't exist, make a new one
        new_file.cd();
        newtree = new TTree("scans","scans");

        // Write new tree
        newtree->Branch("angle",&angle);
        newtree->Branch("x_pos",&x_pos);
        newtree->Branch("avg_pes",&avg);
        newtree->Branch("avg_pes_err",&avg_err);
        newtree->Branch("rms_pes",&rms);
        newtree->Branch("rms_pes_err",&rms_err);
        newtree->Branch("res",&res);
        newtree->Branch("nentries",&N_en);
        newtree->Branch("reflectivity",&reflectivity);
        newtree->Branch("cerenkov",&cerenkov);
        newtree->Branch("scintillation",&scintillation);
        newtree->Branch("z_pos",&z_pos);
    }

    newtree->SetBranchAddress("angle",&angle);
    newtree->SetBranchAddress("x_pos",&x_pos);
    newtree->SetBranchAddress("avg_pes",&avg);
    newtree->SetBranchAddress("avg_pes_err",&avg_err);
    newtree->SetBranchAddress("rms_pes",&rms);
    newtree->SetBranchAddress("rms_pes_err",&rms_err);
    newtree->SetBranchAddress("res",&res);
    newtree->SetBranchAddress("nentries",&N_en);
    newtree->SetBranchAddress("reflectivity",&reflectivity);
    newtree->SetBranchAddress("cerenkov",&cerenkov);
    newtree->SetBranchAddress("scintillation",&scintillation);
    newtree->SetBranchAddress("z_pos",&z_pos);

    x_pos   = atof((fname.substr(fname.find("ees_")+4,fname.find("_x")-fname.find("ees_")-4)).c_str());
    angle   = atof((fname.substr(fname.find("out_")+4,fname.find("_degrees")-fname.find("out_")-4)).c_str());
    avg     = h_npe->GetMean();
    avg_err = h_npe->GetMeanError();
    rms     = h_npe->GetRMS();
    rms_err = h_npe->GetRMSError();
    res     = h_npe->GetRMS()/h_npe->GetMean();
    N_en    = (double)N_entries;

    reflectivity = user_reflectivity;
    cerenkov = user_cerenkov;
    scintillation = user_scintillation;
    z_pos = user_z_pos;

    std::cout << "TEST 2 X = " << x_pos << ", angle = " << angle <<std::endl;
    newtree->Fill();
    newtree->Write("scans",TObject::kOverwrite);
    new_file.Close();

    gSystem->Exec("mv localTmp.root scans.root");

	//#h_hp->Draw();
}
/*
//Fitting functions

Double_t fit_func_yxzhao_pe(Double_t *x, Double_t *par){ 
	double  xx = x[0];                                                                                          
	double  s_real_sum=0;                                                                                       
	double term_1 = xx-par[1]-par[0]/par[3]-par[7]*par[5];                                                      
	s_real_sum=TMath::PoissonI(par[7],par[4]) *( 1.0/par[6]/sqrt(twopi)/sqrt(par[7]) * exp(-1.0*pow             
                                  (term_1, 2)/2.0/par[7]/pow(par[6],2)));                                     
	return s_real_sum;
}

Double_t fit_func_yxzhao_bg(Double_t *x, Double_t *par){ 
	double  xx = x[0];                                                                                          
	double theta_f=0;                                                                                           
	if(xx>=par[1]){                                                                                             
		theta_f=1.0;                                                                                            
	}else{                                                                                                      
		theta_f=0;                                                                                              
	}                                                                                                           
	double background=exp(-1.0*par[4]) * (  (1-par[0])/par[2]/sqrt(twopi)*exp(-1.0*pow(xx-par[1],2)/2.0/pow(par[2],2) ) + par[0]*theta_f*par[3]*exp(-1.0*par[3]*(xx-par[1]))  );
                                                                                                              
	return background;                                                                                          
}  */
