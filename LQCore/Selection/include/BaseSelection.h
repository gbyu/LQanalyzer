#ifndef BaseSelection_h
#define BaseSelection_h

#include <iostream>
using namespace std;

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include "TLorentzVector.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "AnalysisBase.h"
#include "LQEvent.h"

class BaseSelection {

 public:


  enum ID {
    EGAMMA_VETO              = 0,
    EGAMMA_LOOSE             = 1,
    EGAMMA_MEDIUM            = 2,
    EGAMMA_TIGHT             = 3,
    EGAMMA_TRIGTIGHT         = 4,
    EGAMMA_TRIGWP70          = 5,
    EGAMMA_EOP               = 6,
    MVATrig                  = 7,
    MVANonTrig               = 8,
    ECAL_FIDUCIAL            = 9,
    MUON_LOOSE               = 10,
    MUON_TIGHT               = 11,
    PFJET_LOOSE              = 12,
    PFJET_MEDIUM             = 13,
    PFJET_TIGHT              = 14,

    EGAMMA_FAKELOOSE         = 15
    
  };

  Int_t ifid;
  
  //// Selection cuts
  Bool_t etaPt,RelIsod0Chi2,DepositVeto,individual,RelIsod0;//fiducial;//muonid,pTcut,isIso;

  //// variables to make cuts
  Double_t LeptonRelIso,dxy,dz,D0,D0Error,D0Significance,Vxy, Vz;
  Double_t LeptonchiNdof;
  Int_t numlep;
  Bool_t jetIsOK;

  /// variables to set cuts
  Double_t pt_cut_min, pt_cut_max, eta_cut_min, eta_cut, 
           jpt_cut_min, jpt_cut_max, jeta_cut_min, jeta_cut,
           relIso_cut, relIsoMIN_cut, chiNdof_cut, chiNdofMIN_cut, 
           dxy_cut, dxyMIN_cut, dz_cut;

  Int_t casediscriminator,simpleselection;

  /// LQEvent class object. Stores all vectors
  LQEvent k_lqevent;


  BaseSelection();
  BaseSelection& operator= (const BaseSelection& obj);
  BaseSelection(const BaseSelection& bs);

  ~BaseSelection();

  /// bools to tell selector to apply cuts
  Bool_t  apply_ptcut,apply_etacut, apply_jptcut,apply_jetacut, apply_relisocut, apply_chi2cut, apply_dxycut, apply_dzcut, apply_general, apply_deposit;
  Bool_t apply_ID, apply_convcut, apply_chargeconst, applypileuptool;
  
  ID k_id;
  
  /// SetCut functions
  void reset();
  void SetPt(Double_t minPt, Double_t maxPt);
  void SetPt(Double_t minPt);
  void SetJetPt(Double_t minPt);
  void SetEta(Double_t Eta);
  void SetUseJetPileUp(bool use);
  void SetJetEta(Double_t Eta);
  void SetEta(Double_t minEta, Double_t Eta);
  void SetRelIso(Double_t RelIso);
  void SetRelIso(Double_t RelIsoMIN, Double_t RelIso);
  void SetChiNdof(Double_t ChiNdof);
  void SetChiNdof(Double_t ChiNdofMIN, Double_t ChiNdof);
  void SetBSdxy(Double_t dxy);
  void SetBSdxy(Double_t dxyMIN, Double_t dxy);
  void SetBSdz(Double_t dz);
  void SetID(ID  id);
  void SetCheckCharge(bool check);
  void SetApplyConvVeto(bool apply);
};

#endif
