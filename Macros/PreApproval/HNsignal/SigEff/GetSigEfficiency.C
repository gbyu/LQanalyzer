#include "Macro.h"
#include "CMS_lumi.h"
#include "TGraphAsymmErrors.h"



TH1* makehist(TString label, vector<float> val, vector<TString> masses);


void GetSigEfficiency(){
  
  TString path = "/home/jalmond/Analysis/LQanalyzer/data/output/SSElectron/HNDiElectron_SKHNee";
  vector<TString> masses;
  masses.push_back("40");
  masses.push_back("50");
  masses.push_back("60");
  masses.push_back("70");
  masses.push_back("80");
  masses.push_back("90");
  masses.push_back("100");
  masses.push_back("125");
  masses.push_back("150");
  masses.push_back("175");
  masses.push_back("200");
  masses.push_back("250");
  masses.push_back("300");
  masses.push_back("350");
  masses.push_back("400");
  masses.push_back("500");
  
  setTDRStyle();
  //gStyle->SetPalette(1);

  
  vector<float>  isoid,id , elid, jet, total, noZ;
  
  for(int i=0; i < masses.size(); i++){
    TFile * file = new TFile(path + masses[i] + "_nocut_5_3_14.root");
    
    
    TH1F* h_ref= (TH1F*)file->Get(("Efficiency/eff_electronRef"));
    TH1F* h_pt15 =(TH1F*)file->Get(("Efficiency/eff_electronPt15CutColl"));
    TH1F* h_eta25 =(TH1F*)file->Get(("Efficiency/eff_electronEta25CutColl"));
    TH1F* h_tight =(TH1F*)file->Get(("Efficiency/eff_electron_NoIsoCutColl"));
    TH1F* h_iso =(TH1F*)file->Get(("Efficiency/eff_electron_IsoCutColl"));
    TH1F* h_npfiso =(TH1F*)file->Get(("Efficiency/eff_electron_NPFIsoCutColl"));
    TH1F* h_analysis =(TH1F*)file->Get(("Efficiency/eff_electron_analysisCutColll"));
    TH1F* h_analysisnpf =(TH1F*)file->Get(("Efficiency/eff_electron_analysisNPFCutColll"));
    
    TH1F* h_DeltaEta =(TH1F*)file->Get(("Efficiency/eff_electron_DeltaEta"));
    TH1F* h_DeltaPhi =(TH1F*)file->Get(("Efficiency/eff_electron_DeltaPhi"));
    TH1F* h_SigmaIEta =(TH1F*)file->Get(("Efficiency/eff_electron_SigmaIEta"));
    TH1F* h_DZ =(TH1F*)file->Get(("Efficiency/eff_electron_DZ"));
    TH1F* h_HoverE =(TH1F*)file->Get(("Efficiency/eff_electron_HoverE"));
    TH1F* h_convfit =(TH1F*)file->Get(("Efficiency/eff_electron_convfit"));
    TH1F* h_ep =(TH1F*)file->Get(("Efficiency/eff_electron_ep"));
    TH1F* h_missinghit =(TH1F*)file->Get(("Efficiency/eff_electron_missinghit"));
    TH1F* h_convphoton =(TH1F*)file->Get(("Efficiency/eff_electron_convphoton"));
    TH1F* h_DXY =(TH1F*)file->Get(("Efficiency/eff_electron_DXY"));
    TH1F* h_chargeconst =(TH1F*)file->Get(("Efficiency/eff_electron_chargeconst"));
    TH1F* h_refd0 = (TH1F*)file->Get(("tight/eff_electronTight_tightref"));
    
    TH1F* h_jetsel =(TH1F*)file->Get(("_JetSelection"));
    TH1F* h_presel =(TH1F*)file->Get(("PreSelection_veto"));
    TH1F* h_presel_dijet =(TH1F*)file->Get(("PreSelection"));
    TH1F* h_presel_noZ =(TH1F*)file->Get(("PreSelection_noZ"));
    
    double tight_err ;
    h_tight->IntegralAndError(0, h_tight->GetNbinsX()+1, tight_err, ""); 
    double eta25_err ;
    h_eta25->IntegralAndError(0, h_eta25->GetNbinsX()+1, eta25_err, "");
    double iso_err ;
    h_iso->IntegralAndError(0, h_iso->GetNbinsX()+1, iso_err, "");
    double el_err ;
    h_analysis->IntegralAndError(0, h_analysis->GetNbinsX()+1, el_err, "");
    
    double jet_err ;
    h_jetsel->IntegralAndError(0, h_jetsel->GetNbinsX()+1, jet_err, "");

    double presel_err;
    h_presel->IntegralAndError(0, h_presel->GetNbinsX()+1, presel_err, "");


    cout << "\n ------- "  << endl;
    cout << "Mass = " << masses.at(i) << endl;
    cout << "Pt 15 cut efficiency [%] = " <<100* h_pt15->Integral() / h_ref->Integral()<< endl;;
    
    cout << "Eta 25 cut efficiency [%] = " <<100* h_eta25->Integral() / h_ref->Integral()<< " \pm " << 100.* eta25_err/ h_ref->Integral() << endl;;
    
    cout << "ID cut efficiency [%] = " << 100* h_tight->Integral() / h_ref->Integral()<< " \pm " << 100.* tight_err / h_ref->Integral()<<  endl;;
    cout << "DeltaEta cut efficiency [%] = " << 100* h_DeltaEta->Integral() / h_refd0->Integral()<< endl;;
    cout << "DeltaPhi cut efficiency [%] = " << 100* h_DeltaPhi->Integral() / h_refd0->Integral()<< endl;;
    cout << "SigmaIEta cut efficiency [%] = " << 100* h_SigmaIEta->Integral() / h_refd0->Integral()<< endl;;
    cout << "DZ  cut efficiency [%] = " <<100* h_DZ->Integral() / h_refd0->Integral()<< endl;;
    cout << "HoverE cut efficiency [%] = " << 100* h_HoverE->Integral() / h_refd0->Integral()<< endl;;
    cout << "convfit cut efficiency [%] = " <<  100* h_convfit->Integral() / h_refd0->Integral()<< endl;;
    cout << "ep cut efficiency [%] = " <<  100* h_ep->Integral() / h_refd0->Integral()<< endl;;
    cout << "missinghit cut efficiency [%] = " << 100* h_missinghit->Integral() / h_refd0->Integral()<< endl;;
    cout << "convphoton  cut efficiency [%] = " <<  100* h_convphoton->Integral() / h_refd0->Integral()<< endl;;
    cout << "DXY cut efficiency [%] = " << 100* h_DXY->Integral() / h_pt15->Integral()<< endl;;
    cout << "chargeconst cut efficiency [%] = " << 100*  h_chargeconst->Integral() / h_refd0->Integral()<< endl;;
    cout << "\n ------------ " << endl;
    
    
    cout << "Isolation cut efficiency [%] = " << 100* h_iso->Integral() / h_ref->Integral()<< " \pm " <<   100.*iso_err/ h_ref->Integral() << endl;;
    cout << "Electron efficiency [%] = " << 100* h_analysis->Integral() / h_ref->Integral()<<  " \pm " <<  100.*el_err/ h_ref->Integral() << endl;;
    
    cout << "\n ------------ " << endl;
    cout << "Jet Selection  efficiency [%] " << 100* h_jetsel->Integral() / h_ref->Integral() << " \pm " << 100.* jet_err / h_ref->Integral()   << endl;;
    cout << "\n ------------ " <<endl;
    
    cout << "Total efficiency (SSee + 2 jets) " << 100* h_presel->Integral() / h_ref->Integral() << " \pm   " <<  100* presel_err /  h_ref->Integral() << endl;;
    cout << "\n ------- "  << endl;
    

    isoid.push_back(  h_iso->Integral() / h_ref->Integral());
    noZ.push_back(  h_presel_noZ->Integral() / h_presel_dijet->Integral());
    id.push_back(  h_tight->Integral() / h_ref->Integral());
    elid.push_back(  h_analysis->Integral() / h_ref->Integral());
    jet.push_back( h_jetsel->Integral() / h_ref->Integral());
    total.push_back( h_presel->Integral() / h_ref->Integral());
    
  }
  
  TH1* total_hist = makehist("total", total, masses); 
  TH1* jet_hist = makehist("jet", jet, masses); 
  TH1* el_hist = makehist("elid", elid, masses); 
  TH1* noZ_hist = makehist("noZ", noZ, masses); 
  
  cout << total_hist  << endl;
  total_hist->GetXaxis()->SetTitle("m_{N} GeV");
  total_hist->GetYaxis()->SetTitle("Efficiency");

  TLegend* legendH = new TLegend(0.2, 0.7, 0.7, 0.9);
  legendH->SetFillColor(kWhite);
  legendH->SetTextSize(0.03);
  
  total_hist->SetMarkerColor(kRed);
  total_hist->SetMarkerStyle(20.);

  el_hist->SetMarkerColor(kBlue);
  el_hist->SetMarkerStyle(21.);
  jet_hist->SetMarkerColor(kGreen);
  jet_hist->SetMarkerStyle(22.);
  noZ_hist->SetMarkerColor(kCyan);
  noZ_hist->SetMarkerStyle(22.);




  legendH->AddEntry(el_hist, "Electron ID", "p");
  legendH->AddEntry(noZ_hist, "|m(ee) - m(Z) | > 10 GeV ", "p");
  legendH->AddEntry(jet_hist, "Jet ID", "p");
  legendH->AddEntry(total_hist, "Preselection", "p");

  total_hist->GetYaxis()->SetRangeUser(0.,1.6);
  total_hist->Draw("p");
  el_hist->Draw("psame");
  jet_hist->Draw("psame");
  noZ_hist->Draw("psame");
  
  legendH->Draw();


  
  CMS_lumi( c1, 2, 11 );
  c1->Update();
  c1->RedrawAxis();



  c1->SaveAs(("/home/jalmond/WebPlots/PreApproval/SignalPlots/SignalEff.pdf" ));
}

  

TH1* makehist(TString label, vector<float> val, vector<TString> masses){
  int n = masses.size();
  TH1F* heff = new TH1F(label,label, n, 0., float(n));
  for(int i = 0 ; i < n ; i++){
    heff->SetBinContent(i+1, val[i]); 
    heff->GetXaxis()->SetBinLabel(i+1,masses[i].Data());
  }

  return heff;
}


CMS_lumi( TPad* pad, int iPeriod, int iPosX )
{
  bool outOfFrame    = false;
  if( iPosX/10==0 )
    {
      outOfFrame = true;
    }
  int alignY_=3;
  int alignX_=2;
  if( iPosX/10==0 ) alignX_=1;
  if( iPosX==0    ) alignY_=1;
  if( iPosX/10==1 ) alignX_=1;
  if( iPosX/10==2 ) alignX_=2;
  if( iPosX/10==3 ) alignX_=3;
  int align_ = 10*alignX_ + alignY_;

  float H = pad->GetWh();
  float W = pad->GetWw();
  float l = pad->GetLeftMargin();
  float t = pad->GetTopMargin();
  float r = pad->GetRightMargin();
  float b = pad->GetBottomMargin();
  float e = 0.025;

  pad->cd();

  TString lumiText;
  if( iPeriod==1 )
    {
      lumiText += lumi_7TeV;
      lumiText += " (7 TeV)";
    }
  else if ( iPeriod==2 )
    {
      lumiText += " (8 TeV)";
    }
  else if( iPeriod==3 )
    {
      lumiText = lumi_8TeV;
      lumiText += " (8 TeV)";
      lumiText += " + ";
      lumiText += lumi_7TeV;
      lumiText += " (7 TeV)";
    }
  else if ( iPeriod==4 )
    {
      lumiText += lumi_13TeV;
      lumiText += " (13 TeV)";
    }
  else if ( iPeriod==7 )
    {
      if( outOfFrame ) lumiText += "#scale[0.85]{";
      lumiText += lumi_13TeV;
      lumiText += " (13 TeV)";
      lumiText += " + ";
      lumiText += lumi_8TeV;
      lumiText += " (8 TeV)";
      lumiText += " + ";
      lumiText += lumi_7TeV;
      lumiText += " (7 TeV)";
      if( outOfFrame) lumiText += "}";
    }
  else if ( iPeriod==12 )
    {
      lumiText += "8 TeV";
    }

  cout << lumiText << endl;

  TLatex latex;
  latex.SetNDC();
  latex.SetTextAngle(0);
  latex.SetTextColor(kBlack);

  float extraTextSize = extraOverCmsTextSize*cmsTextSize;

  latex.SetTextFont(42);
  latex.SetTextAlign(31);
  latex.SetTextSize(lumiTextSize*t);
  latex.DrawLatex(1-r,1-t+lumiTextOffset*t,lumiText);

  if( outOfFrame )
    {
      latex.SetTextFont(cmsTextFont);
      latex.SetTextAlign(11);
      latex.SetTextSize(cmsTextSize*t);
      latex.DrawLatex(l,1-t+lumiTextOffset*t,cmsText);
    }

  pad->cd();

  float posX_;
  if( iPosX%10<=1 )
    {
      posX_ =   l + relPosX*(1-l-r);
    }
  else if( iPosX%10==2 )
    {
      posX_ =  l + 0.5*(1-l-r);
    }
  else if( iPosX%10==3 )
    {
      posX_ =  1-r - relPosX*(1-l-r);
    }
  float posY_ = 1-t+lumiTextOffset*t;
  if( !outOfFrame )
    {
      if( drawLogo )
        {
          posX_ =   l + 0.045*(1-l-r)*W/H;
          posY_ = t;
          float xl_0 = posX_;
          float yl_0 = posY_ ;
          float xl_1 = posX_ + 0.15*H/W;
          float yl_1 = posY_;
          /*TASImage* CMS_logo = new TASImage("CMS-BW-label.png");
          TPad* pad_logo = new TPad("logo","logo", xl_0, yl_0, xl_1, yl_1 );
          pad_logo->Draw();
          pad_logo->cd();
          CMS_logo->Draw("X");
          pad_logo->Modified();
          pad->cd();*/
        }
      else
        {
          latex.SetTextFont(cmsTextFont);
          latex.SetTextSize(cmsTextSize*t);
          latex.SetTextAlign(align_);
          latex.DrawLatex(posX_, 0.985  , cmsText);
          if( writeExtraText )
            {
              latex.SetTextFont(extraTextFont);
              latex.SetTextAlign(align_);
              latex.SetTextSize(extraTextSize*t);
              latex.DrawLatex(posX_, posY_- relExtraDY*cmsTextSize*t, extraText);
            }
        }
    }
  else if( writeExtraText )
    {
      if( iPosX==0)
        {
          posX_ =   l +  relPosX*(1-l-r);
          posY_ =   1-t+lumiTextOffset*t;
        }
      latex.SetTextFont(extraTextFont);
      latex.SetTextSize(extraTextSize*t);
      latex.SetTextAlign(align_);
      latex.DrawLatex(posX_, posY_, extraText);
    }
  return;
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




