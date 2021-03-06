#include <string.h>

#include "TChain.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"
#include "TKey.h"
#include <iostream>
#include <TStyle.h>
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"
#include <map>

//#include "Macro.h"
void setTDRStyle();



void GetNMCEvents_isochec_dec(){
  
  
  setTDRStyle();
  //gStyle->SetPalette(1);
  TString path ="/home/jalmond/Analysis/LQanalyzer/data/output/SSElectron/HNDiElectron_mc_5_3_14.root";
  TFile * file = new TFile(path);
  
  TString fakepath ="/home/jalmond/Analysis/LQanalyzer/data/output/SSElectron/HNDiElectron_SKnonprompt_dilep_5_3_14.root";
  TFile * filefake = new TFile(fakepath);
  
  TString cfpath ="/home/jalmond/Analysis/LQanalyzer/data/output/SSElectron/HNDiElectron_SKchargeflip_dilep_5_3_14.root";
  TFile * filecf = new TFile(cfpath);
  cout << file << " " << filefake << " " << filecf << endl;
  vector<TString> masses;
  masses.push_back("40");
  masses.push_back("50");
  masses.push_back("60");
  masses.push_back("70");
  masses.push_back("80");
  /*
    masses.push_back("90");
  masses.push_back("100");
  masses.push_back("125");
  masses.push_back("150");
  masses.push_back("175");
  masses.push_back("200");
  masses.push_back("225");
  masses.push_back("250");
  masses.push_back("275");
  masses.push_back("300");
  masses.push_back("325");
  masses.push_back("350");
  masses.push_back("375");
  masses.push_back("400");
  masses.push_back("500");*/
  
  vector<TString> type;
  type.push_back("");
  //type.push_back("_fg");
  
    std::map<TString, int> mapcut;
    std::map<TString, int>::iterator mit; 
    
  mapcut["_default"] = 1;
  /*mapcut["_lowmass"] = 2;
  mapcut["_noMe2jj"] = 3;
  mapcut["_noeeupper"] = 4;
  mapcut["_noeejjupper"] = 5;
  mapcut["_nopt10"] = 6;
  */
  
  for(mit = mapcut.begin(); mit!= mapcut.end() ; mit++){
    for(unsigned int j=0; j < type.size(); j++){
    
      //if(type.at(j).Contains("fg") && !mit->first.Contains("default")) continue;
      TString outfile = "Limit_file_isocheck_" + mit->first + type.at(j) + ".root";
      TFile* fout = new TFile(outfile.Data(),"RECREATE");
      fout->cd();
      
      TH1F* h_sigeff = new TH1F("h_sigeff","h_sigeff", 20,0.,20.);
      TH1F* h_sigefferr = new TH1F("h_sigefferr","h_sigefferr", 20,0.,20.);
      TH1F* h_nbkg = new TH1F("h_nbkg","h_nbkg", 20,0.,20.);
      TH1F* h_nfake =  new TH1F("h_nfake","h_nfake", 20,0.,20.);
      TH1F* h_staterr  =  new TH1F("h_staterr","h_staterr", 20,0.,20.);
      TH1F* h_bkgerr  =  new TH1F("h_bkgerr","h_bkgerr", 20,0.,20.);
      TH1F* h_xsec  =  new TH1F("h_xsec","h_xsec", 20,0.,20.);
      TH1F* h_obs  =  new TH1F("h_obs","h_obs", 20,0.,20.);
      TH1F* h_mass  =  new TH1F("h_mass","h_mass", 20,0.,20.);
      Double_t eff_alp[20];
      
      
      Int_t result []     = {7   , 7 , 7   , 7, 7,13,12,17,12,9,9,4,4,3,2,2,2,2,2,2};
      Double_t mass[]     = {40 , 50, 60, 70, 80   , 90    , 100   , 125   , 150   , 175    , 200  , 225  , 250   , 275,  300    , 325   , 350   ,  375  , 400     , 500 };
      Double_t k_factor=1.;
      Double_t xsec_alp[] = {1516, 1071.1, 607.7, 211.96, 19.07, 7.1047, 3.5618, 1.0767, 0.4594, 0.23266, 0.13127, 0.079661,  0.050928, 0.033857, 0.023214,0.016317,  0.011705,0.0085448, 0.006332, 0.002154};
      Double_t efferr_alp[20];
      
      
      for(unsigned int i=0; i < masses.size(); i++){
	h_xsec->SetBinContent(i+1,xsec_alp[i]);
	h_obs->SetBinContent(i+1,result[i]);
	h_mass->SetBinContent(i+1,mass[i]);
	
	TString tag = "limithist/" +masses.at(i)+ type.at(j)+  mit->first;
	cout << tag << endl;
	TH1* hnmc =   (TH1F*)file->Get((tag + "MassRegion_limithist").Data());
	cout << hnmc << endl;
	TH1* hnnp =   (TH1F*)filefake->Get((tag + "MassRegion_limithist").Data());
	cout << hnnp << endl;
	TH1* hncf =   (TH1F*)filecf->Get((tag + "MassRegion_limithist").Data());
	cout << hncf << endl;


	cout << "hnmc->GetBinError(2) = " << hnmc->GetBinError(2) << " hnnp->GetBinError(2) = " << hnnp->GetBinError(2) << " hncf->GetBinError(2) = " << hncf->GetBinError(2) <<  endl;
	cout << "hnmc->GetBinContent(2) = " << hnmc->GetBinContent(2) << " hnnp->GetBinContent(2) = " << hnnp->GetBinContent(2) << " hncf->GetBinContent(2) = " << hncf->GetBinContent(2) <<  endl;
	float staterr = sqrt( hnmc->GetBinError(2)*hnmc->GetBinError(2) +  hnnp->GetBinError(2)*hnnp->GetBinError(2) + hncf->GetBinError(2)*hncf->GetBinError(2));
	
	TString sigpath ="/home/jalmond/Analysis/LQanalyzer/data/output/SSElectron/HNDiElectron_SKHNee" + masses.at(i) + "_nocut_5_3_14.root";
	TFile * file_sig = new TFile(sigpath);
	TH1* hn_sig_mc  = (TH1F*)file_sig->Get((tag + "MassRegion_limithist").Data());
	TH1F* h_ref= (TH1F*)file_sig->Get(("Efficiency/eff_electronRef"));
	
	float sig_nom = hn_sig_mc->GetBinContent(2);
	
	
	float mc_nom = hnmc->GetBinContent(2);
	float mc_nom_no_weight = hnmc->GetBinContent(1);
	
	float np_nom = hnnp->GetBinContent(2);
	float cf_nom = hncf->GetBinContent(2);
	
	float total_bkg = mc_nom + np_nom + cf_nom;
	
	
	cout << "Mass point : " << masses.at(i)+ type.at(j) << endl;
	cout << "Nsig = " << sig_nom/h_ref->Integral() << endl;
	cout << "Total bkg = " << total_bkg << endl;
	cout << "Total fakes = " << np_nom << endl;
	cout << "Stat err = " << staterr << endl;
	
	cout << "TOTAL number of CF = " << cf_nom << endl;
	
      cout <<"Number of MC events (weighted) passing mass region: " <<tag  <<  " = " << mc_nom << endl;
      cout <<"Number of MC events passing mass region: " <<tag  <<  " = " << mc_nom_no_weight << endl;
      
      h_sigeff->SetBinContent(i+1, float(sig_nom/h_ref->Integral()));    
      eff_alp[i] = float(sig_nom/h_ref->Integral());
      h_nbkg->SetBinContent(i+1, total_bkg);
      h_nfake->SetBinContent(i+1,np_nom);
      h_staterr->SetBinContent(i+1,staterr);
      
      float nperrup  = fabs(hnnp->GetBinContent(2) - hnnp->GetBinContent(13) )/hnnp->GetBinContent(2);  
      float nperrdown = fabs(hnnp->GetBinContent(2) - hnnp->GetBinContent(14) )/hnnp->GetBinContent(2);
      
      float nperr = nperrup;
      if(nperrup < nperrdown) nperr= nperrdown;
      cout << "NP err = " << nperr << endl;
      nperr = 0.35;
      
      float bkgerr = sqrt( pow(staterr,2) + pow(nperr*np_nom,2) + pow(.19* (mc_nom),2)  +  pow(.20*(cf_nom),2) );
      cout << pow(staterr,2) << endl;
      cout <<  pow(nperr*np_nom,2) << endl;
      cout <<  pow(.19* (mc_nom),2) << endl;
      cout << pow(.20*(cf_nom),2) << endl;
      h_bkgerr->SetBinContent(i+1,bkgerr);
      
      cout << "Bkgerr = " << bkgerr << endl;

      float err_sig_up = sqrt( pow((hn_sig_mc->GetBinContent(3) - sig_nom),2) 
			       + pow((hn_sig_mc->GetBinContent(5) - sig_nom),2) 
			       + pow((hn_sig_mc->GetBinContent(7) - sig_nom),2) 
		     + pow((hn_sig_mc->GetBinContent(8) - sig_nom),2) 
			       + pow((hn_sig_mc->GetBinContent(9) - sig_nom),2)
			       + pow((hn_sig_mc->GetBinContent(10) - sig_nom),2)
			       + pow((hn_sig_mc->GetBinContent(11) - sig_nom),2)
			       + pow((hn_sig_mc->GetBinContent(12) - sig_nom),2)
			       + pow((hn_sig_mc->GetBinContent(15) - sig_nom),2)
			       + pow((hn_sig_mc->GetBinContent(16) - sig_nom),2));
      float err_sig_down = sqrt( pow((hn_sig_mc->GetBinContent(4) - sig_nom),2)
                               + pow((hn_sig_mc->GetBinContent(6) - sig_nom),2)
                               + pow((hn_sig_mc->GetBinContent(7) - sig_nom),2)
                               + pow((hn_sig_mc->GetBinContent(8) - sig_nom),2)
			       + pow((hn_sig_mc->GetBinContent(9) - sig_nom),2)
			       + pow((hn_sig_mc->GetBinContent(10) - sig_nom),2)
			       + pow((hn_sig_mc->GetBinContent(11) - sig_nom),2)
                               + pow((hn_sig_mc->GetBinContent(12) - sig_nom),2)
                               + pow((hn_sig_mc->GetBinContent(15) - sig_nom),2)
                               + pow((hn_sig_mc->GetBinContent(16) - sig_nom),2));
      
      float err_sig = err_sig_up; 
      if(err_sig_up < err_sig_down) err_sig = err_sig_down;
      
      cout << "sig err = " << err_sig << endl;
      err_sig = err_sig/ sig_nom;
      efferr_alp[i]  = eff_alp[i]   *sqrt(1./hn_sig_mc->GetBinContent(1)  + 2*pow(.02,2) + pow(err_sig,2) );//40
      cout << "sig err = " << err_sig << endl;
      cout << "sig err = " << eff_alp[i]   *sqrt(1./hn_sig_mc->GetBinContent(1)  + 2*pow(.02,2) + pow(err_sig,2) ) << endl;
    }

    for(unsigned int i=0; i < masses.size(); i++){
      h_sigefferr->SetBinContent(i+1,efferr_alp[i]);
    }
  
    fout->cd();

    h_sigeff->Write();
    h_sigefferr->Write();
    h_nbkg->Write();
    h_nfake->Write();
    h_staterr->Write();
    h_bkgerr->Write();
    h_xsec->Write();
    h_obs->Write();
    h_mass->Write();
  }
  }
}


void setTDRStyle() {
  TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");

  // For the canvas:
  tdrStyle->SetCanvasBorderMode(0);
  tdrStyle->SetCanvasColor(kWhite);
  tdrStyle->SetCanvasDefH(600); //Height of canvas
  tdrStyle->SetCanvasDefW(600); //Width of canvas
  tdrStyle->SetCanvasDefX(0);   //POsition on screen
  tdrStyle->SetCanvasDefY(0);

  // For the Pad:
  tdrStyle->SetPadBorderMode(0);
  // tdrStyle->SetPadBorderSize(Width_t size = 1);
  tdrStyle->SetPadColor(kWhite);
  tdrStyle->SetPadGridX(false);
  tdrStyle->SetPadGridY(false);
  tdrStyle->SetGridColor(0);
  tdrStyle->SetGridStyle(3);
  tdrStyle->SetGridWidth(1);


  // For the frame:
  tdrStyle->SetFrameBorderMode(0);
  tdrStyle->SetFrameBorderSize(1);
  tdrStyle->SetFrameFillColor(0);
  tdrStyle->SetFrameFillStyle(0);
  tdrStyle->SetFrameLineColor(1);
  tdrStyle->SetFrameLineStyle(1);
  tdrStyle->SetFrameLineWidth(1);


  // For the histo:
  // tdrStyle->SetHistFillColor(1);
  // tdrStyle->SetHistFillStyle(0);
  tdrStyle->SetHistLineColor(1);
  tdrStyle->SetHistLineStyle(0);
  tdrStyle->SetHistLineWidth(1);
  // tdrStyle->SetLegoInnerR(Float_t rad = 0.5);
  // tdrStyle->SetNumberContours(Int_t number = 20);
  tdrStyle->SetEndErrorSize(2);
  //  tdrStyle->SetErrorMarker(20);
  //  tdrStyle->SetErrorX(0.);

  tdrStyle->SetMarkerStyle(20);

  //For the fit/function:
  tdrStyle->SetOptFit(1);
  tdrStyle->SetFitFormat("5.4g");
  tdrStyle->SetFuncColor(2);
  tdrStyle->SetFuncStyle(1);
  tdrStyle->SetFuncWidth(1);

  //For the date:
  tdrStyle->SetOptDate(0);
  // tdrStyle->SetDateX(Float_t x = 0.01);

  // tdrStyle->SetDateY(Float_t y = 0.01);

  // For the statistics box:
  tdrStyle->SetOptFile(0);
  tdrStyle->SetOptStat(0); // To display the mean and RMS:   SetOptStat("mr");
  tdrStyle->SetStatColor(kWhite);
  tdrStyle->SetStatFont(42);
  tdrStyle->SetStatFontSize(0.025);
  tdrStyle->SetStatTextColor(1);
  tdrStyle->SetStatFormat("6.4g");
  tdrStyle->SetStatBorderSize(1);
  tdrStyle->SetStatH(0.1);
  tdrStyle->SetStatW(0.15);
  // tdrStyle->SetStatStyle(Style_t style = 1001);
  // tdrStyle->SetStatX(Float_t x = 0);
  // tdrStyle->SetStatY(Float_t y = 0);

  // Margins:
  tdrStyle->SetPadTopMargin(0.05);
  tdrStyle->SetPadBottomMargin(0.13);
  tdrStyle->SetPadLeftMargin(0.16);
  tdrStyle->SetPadRightMargin(0.02);

  // For the Global title:

  tdrStyle->SetOptTitle(0);
  tdrStyle->SetTitleFont(42);
  tdrStyle->SetTitleColor(1);

  tdrStyle->SetTitleTextColor(1);
  tdrStyle->SetTitleFillColor(10);
  tdrStyle->SetTitleFontSize(0.05);
  // tdrStyle->SetTitleH(0); // Set the height of the title box
  // tdrStyle->SetTitleW(0); // Set the width of the title box
  // tdrStyle->SetTitleX(0); // Set the position of the title box
  // tdrStyle->SetTitleY(0.985); // Set the position of the title box
  // tdrStyle->SetTitleStyle(Style_t style = 1001);
  // tdrStyle->SetTitleBorderSize(2);

  // For the axis titles:

  tdrStyle->SetTitleColor(1, "XYZ");
  tdrStyle->SetTitleFont(42, "XYZ");
  tdrStyle->SetTitleSize(0.06, "XYZ");
  // tdrStyle->SetTitleXSize(Float_t size = 0.02); // Another way to set the size?
  // tdrStyle->SetTitleYSize(Float_t size = 0.02);
  tdrStyle->SetTitleXOffset(0.9);
  tdrStyle->SetTitleYOffset(1.4);
  // tdrStyle->SetTitleOffset(1.1, "Y"); // Another way to set the Offset



  // For the axis labels:

  tdrStyle->SetLabelColor(1, "XYZ");
  tdrStyle->SetLabelFont(42, "XYZ");
  tdrStyle->SetTitleSize(0.06, "XYZ");
  // tdrStyle->SetTitleXSize(Float_t size = 0.02); // Another way to set the size?
  // tdrStyle->SetTitleYSize(Float_t size = 0.02);
  tdrStyle->SetTitleXOffset(0.9);
  tdrStyle->SetTitleYOffset(1.4);
  // tdrStyle->SetTitleOffset(1.1, "Y"); // Another way to set the Offset

  // For the axis labels:

  tdrStyle->SetLabelColor(1, "XYZ");
  tdrStyle->SetLabelFont(42, "XYZ");
  tdrStyle->SetLabelOffset(0.007, "XYZ");
  tdrStyle->SetLabelSize(0.05, "XYZ");

  // For the axis:

  tdrStyle->SetAxisColor(1, "XYZ");
  tdrStyle->SetStripDecimals(kTRUE);
  tdrStyle->SetTickLength(0.03, "XYZ");
  tdrStyle->SetNdivisions(510, "XYZ");
  tdrStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
  tdrStyle->SetPadTickY(1);

  // Change for log plots:
  tdrStyle->SetOptLogx(0);
  tdrStyle->SetOptLogy(0);
  tdrStyle->SetOptLogz(0);

  // Postscript options:
  tdrStyle->SetPaperSize(20.,20.);
  // tdrStyle->SetLineScalePS(Float_t scale = 3);
  // tdrStyle->SetLineStyleString(Int_t i, const char* text);
  // tdrStyle->SetHeaderPS(const char* header);
  // tdrStyle->SetTitlePS(const char* pstitle);

  // tdrStyle->SetBarOffset(Float_t baroff = 0.5);
  // tdrStyle->SetBarWidth(Float_t barwidth = 0.5);
  // tdrStyle->SetPaintTextFormat(const char* format = "g");
  // tdrStyle->SetPalette(Int_t ncolors = 0, Int_t* colors = 0);
  // tdrStyle->SetTimeOffset(Double_t toffset);
  // tdrStyle->SetHistMinimumZero(kTRUE);

  tdrStyle->cd();

}




