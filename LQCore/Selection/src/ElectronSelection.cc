#include "ElectronSelection.h"

using namespace snu;

ElectronSelection::ElectronSelection(LQEvent ev) : BaseSelection() {
  k_lqevent = ev;
};

ElectronSelection::~ElectronSelection() {};


void ElectronSelection::BasicSelection(std::vector<KElectron>& leptonColl) {
  
  /// For filling SKTrees
  std::vector<KElectron> allelectrons = k_lqevent.GetElectrons();
  double rho = k_lqevent.GetEvent().JetRho();
  

  for (std::vector<KElectron>::iterator el = allelectrons.begin(); el!=allelectrons.end(); el++){
    
    if ( fabs(el->Eta())>1.4442 && fabs(el->Eta())<1.566 ) continue;
    if ( el->CaloEnergy()==0 ) continue;
    if ( !PassUserID(EGAMMA_MEDIUM, *el, false, rho) ) continue;
    
    
    if ( fabs(el->SCEta()) < eta_cut && el->Pt() >= pt_cut_min && el->Pt() < pt_cut_max){
      leptonColl.push_back(*el);
    }
  }
  
}

void ElectronSelection::SkimSelection(std::vector<KElectron>& leptonColl) {

  
  std::vector<KElectron> allelectrons = k_lqevent.GetElectrons();
  double rho = k_lqevent.GetEvent().JetRho();
  
  for (std::vector<KElectron>::iterator el = allelectrons.begin(); el!=allelectrons.end(); el++){
    if ( fabs(el->Eta())>1.4442 && fabs(el->Eta())<1.566 ) continue;
    if ( el->CaloEnergy()==0 ) continue;
    if ( !PassUserID(EGAMMA_MEDIUM, *el, false, rho) ) continue;
    
    if ( fabs(el->SCEta()) < eta_cut && el->Pt() >= pt_cut_min && el->Pt() < pt_cut_max){
      leptonColl.push_back(*el);
    }
  }
 
}

void ElectronSelection::Selection(std::vector<KElectron>& leptonColl, bool recalculate_id) {
  
  std::vector<KElectron> allelectrons = k_lqevent.GetElectrons();
  double rho = k_lqevent.GetEvent().JetRho(); 
  

  for (std::vector<KElectron>::iterator el = allelectrons.begin(); el!=allelectrons.end(); el++){
    
    //// DEFAULT cuts
    //// Require it is not in crack
    if ( fabs(el->Eta())>1.4442 && fabs(el->Eta())<1.566 ) continue;
    if ( el->CaloEnergy()==0 ) continue;
    
    bool pass_selection = true;
    
    ////  ID cut : need to optimise cuts
    /// Default is medium
    if(apply_ID){
      ElectronID = PassUserID(k_id, *el, true, rho);
      if(!ElectronID) pass_selection = false;
    }
    /// extra cut to reduce conversions
    /// https://twiki.cern.ch/twiki/bin/view/CMS/ConversionTools
    if(apply_convcut && (el->HasMatchedConvPhot() ||  (el->MissingHits() != 0)) ) {
      pass_selection = false; 
    }

    /// PF ISOLATION DR 03 is default https://twiki.cern.ch/twiki/bin/viewauth/CMS/EgammaCutBasedIdentification  with 0.12 cut
    
    Double_t PHONH_04[7]          = {0.208, 0.209, 0.115, 0.143, 0.183, 0.194, 0.261};
    Double_t PHONH_03[7]          = {0.13, 0.14, 0.07, 0.09, 0.11, 0.11, 0.14};

    //// ISOLATION (AREA CORERECTION)    
    //https://twiki.cern.ch/twiki/bin/viewauth/CMS/EgammaEARhoCorrection

    if (fabs(el->SCEta()) < 1.0) ifid = 0;
    else if (fabs(el->SCEta()) < 1.479) ifid = 1;
    else if (fabs(el->SCEta()) < 2.0) ifid = 2;
    else if (fabs(el->SCEta()) < 2.2) ifid = 3;
    else if (fabs(el->SCEta()) < 2.3) ifid = 4;
    else if (fabs(el->SCEta()) < 2.4) ifid = 5;
    else ifid = 6;
    float LeptonRelIsoDR03(0.), LeptonRelIsoDR04(0.);
    float ElectronIsoDR03 =  el->PFChargedHadronIso03() + max( el->PFNeutralHadronIso03() + el->PFPhotonIso03() - rho * PHONH_03[ifid], 0.);
    float ElectronIsoDR04 =  el->PFChargedHadronIso04() + max( el->PFNeutralHadronIso04() + el->PFPhotonIso04() - rho * PHONH_04[ifid], 0.)  ;
    if(el->Pt() > 0.){
      LeptonRelIsoDR03 = ElectronIsoDR03/  el->Pt();
      LeptonRelIsoDR03 = ElectronIsoDR03/  el->Pt();
    }else
      {
	LeptonRelIsoDR03 = -999.;
	LeptonRelIsoDR04 = -999.;
      }

    if(apply_relisocut && !(LeptonRelIsoDR03 < relIso_cut && LeptonRelIsoDR03 >= relIsoMIN_cut)){
      pass_selection = false;
    }

    //// Check charge consistancy between different detectors
    if(apply_chargeconst && !el->GsfCtfScPixChargeConsistency()) {
      pass_selection = false;
    }
    
    if(apply_etacut && !(fabs(el->Eta()) < eta_cut)) {
      pass_selection = false;
    }
    if(apply_ptcut && ! (el->Pt() >= pt_cut_min && el->Pt() < pt_cut_max)) {
      pass_selection = false; 
    }
    /// impact parameter cuts
    if(apply_dzcut && !(el->dz()<  dz_cut )) {
      pass_selection = false;
    }
    if(apply_dxycut && !(el->dz()< dxy_cut )) {
      pass_selection = false;
    }
    
    if(pass_selection){
      leptonColl.push_back(*el);
    }
    
  }// end of el loop
  
  return;
}


bool ElectronSelection::PassUserID(ID id, snu::KElectron el, bool recalculate, double jetrho){

  if(recalculate){
    if ( id == EGAMMA_TIGHT   ) return PassUserID_EGamma2012     ( EGAMMA_TIGHT, el, jetrho);
    else if ( id == EGAMMA_MEDIUM  ) return PassUserID_EGamma2012     (EGAMMA_MEDIUM,el, jetrho);
    else if ( id == EGAMMA_LOOSE   ) return PassUserID_EGamma2012     (EGAMMA_LOOSE,el, jetrho);
    else if ( id == EGAMMA_VETO    ) return PassUserID_EGamma2012     (EGAMMA_VETO,el, jetrho);
    else if ( id == EGAMMA_TRIGTIGHT ) return PassUserID_EGamma2012     (EGAMMA_TRIGTIGHT,el, jetrho);
    else if ( id == EGAMMA_TRIGWP70 ) return PassUserID_EGamma2012     (EGAMMA_TRIGWP70,el, jetrho);
    else if ( id == EGAMMA_EOP      ) return PassUserID_EGamma2012     (EGAMMA_EOP,el, jetrho);
    else if ( id == MVATrig            ) return PassUserID_MVA     (el, true);
    else if ( id == MVANonTrig            ) return PassUserID_MVA     (el, false);
    else if ( id == ECAL_FIDUCIAL  ) return PassUserID_ECALFiducial     (el);
    else return false;
  }
  else{
    if ( id == EGAMMA_TIGHT   )  return el.PassEGammaIDTight();
    else if ( id == EGAMMA_MEDIUM  ) return el.PassEGammaIDMedium();
    else if ( id == EGAMMA_LOOSE   ) return el.PassEGammaIDLoose();
    else if ( id == EGAMMA_VETO    ) return el.PassEGammaIDVeto();
    else if ( id == EGAMMA_TRIGTIGHT ) return el.PassEGammaIDTrigTight();
    else if ( id == EGAMMA_TRIGWP70 ) return el.PassEGammaIDTrigWP70();
    else if ( id == EGAMMA_EOP      ) return el.PassEGammaIDEoP(); 
    else if ( id == MVATrig            )  return PassUserID_MVA     (el, true);
    else if ( id == MVANonTrig            )  return PassUserID_MVA     (el, false);
    else return false;
    
    
  }
  
  
}

bool ElectronSelection::PassUserID_ECALFiducial (snu::KElectron el){
  if ( el.IsEBFiducial() || el.IsEEFiducial() ) return true;
  else return false;
}

bool ElectronSelection::PassUserID_EGamma2012 ( ID id, snu::KElectron el, double jetrho ){

  //----------------------------------------------------------------------
  // Barrel electron cut values
  //----------------------------------------------------------------------

  double l_b_dEtaIn  [4] = { 0.007 , 0.007, 0.004, 0.004 };
  double l_b_dPhiIn  [4] = { 0.8   , 0.15 , 0.06 , 0.03 };
  double l_b_sieie   [4] = { 0.01  , 0.01 , 0.01 , 0.01 };
  double l_b_hoe     [4] = { 0.15  , 0.12 , 0.12 , 0.12 };
  double l_b_d0      [4] = { 0.04  , 0.02 , 0.02 , 0.02 };
  double l_b_dZ      [4] = { 0.2   , 0.2  , 0.1  ,  0.1 };
  double l_b_ep      [4] = { 999.  , 0.05 , 0.05 , 0.05 };
  double l_b_pfRelIso[4] = { 0.15  , 0.15 , 0.15 , 0.10 };
  double l_b_vtxProb [4] = { 999.  , 1e-6 , 1e-6 , 1e-6 };
  int    l_b_missHits[4] = { 999   , 1    , 1    , 0 }; 

  //----------------------------------------------------------------------
  // Endcap electron cut values
  //----------------------------------------------------------------------
  
  double l_e_dEtaIn  [4] = { 0.01  , 0.009, 0.007, 0.005 };
  double l_e_dPhiIn  [4] = { 0.7   , 0.10 , 0.03 , 0.02 };
  double l_e_sieie   [4] = { 0.03  , 0.03 , 0.03 , 0.03 };
  double l_e_hoe     [4] = { 999.  , 0.10 , 0.10 , 0.10 };
  double l_e_d0      [4] = { 0.04  , 0.02 , 0.02 , 0.02 };
  double l_e_dZ      [4] = { 0.2   , 0.2  , 0.1  , 0.1 };
  double l_e_ep      [4] = { 999.  , 0.05 , 0.05 , 0.05 };
  double l_e_pfRelIso[4] = { 0.15  , 0.15 , 0.15 , 0.10 };
  double l_e_vtxProb [4] = { 999.  , 1e-6 , 1e-6 , 1e-6 };
  int    l_e_missHits[4] = { 999   , 1    , 1    , 0 };
  
  //----------------------------------------------------------------------
  // Bools that depend on barrel vs. endcap
  //----------------------------------------------------------------------

  bool   pass_deltaEta      = false;
  bool   pass_deltaPhi      = false;
  bool   pass_sigmaIEtaIEta = false;
  bool   pass_hoe           = false;
  bool   pass_vtxDistXY     = false;
  bool   pass_vtxDistZ      = false;
  bool   pass_ep            = false;
  bool   pass_pfIsolation   = false;
  bool   pass_convFitProb   = false;
  bool   pass_missingHits   = false;

  //----------------------------------------------------------------------
  // Define EGamma ep parameter
  //----------------------------------------------------------------------

  double egamma_e  = el.CaloEnergy();
  double egamma_p  = el.CaloEnergy() / el.ESuperClusterOverP();
  double egamma_ep = fabs ( ( 1.0 / egamma_e ) - ( 1.0 / egamma_p ) );

  //----------------------------------------------------------------------
  // Define PF Isolation
  //----------------------------------------------------------------------

  double effective_area_eta_minimums    [7] = { 0.000, 1.000, 1.479, 2.000, 2.200, 2.300, 2.400 };
  double effective_area_eta_maximums    [7] = { 1.000, 1.479, 2.000, 2.200, 2.300, 2.400, 999.0 };
  double effective_areas_04             [7] = { 0.190, 0.250, 0.120, 0.210, 0.270, 0.440, 0.520 };
  double effective_areas_03             [7] = { 0.100, 0.120, 0.085, 0.110, 0.120, 0.120, 0.130 };
  double effective_area_03  = 0.0;
  double effective_area_04  = 0.0;
  
  for (int i = 0; i < 7; ++i ){ 
    double bin_minimum = effective_area_eta_minimums[i];
    double bin_maximum = effective_area_eta_maximums[i];
    if ( fabs(el.Eta()) >= bin_minimum && fabs(el.Eta()) < bin_maximum ) {
      effective_area_03 = effective_areas_03 [i];
      effective_area_04 = effective_areas_04 [i];
    }
  }
  
  double egamma_pfiso_03 = el.PFChargedHadronIso03() + std::max ( el.PFPhotonIso03() + el.PFNeutralHadronIso03() - ( jetrho * effective_area_03 ), 0.0 );
  double egamma_pfiso_04 = el.PFChargedHadronIso04() + std::max ( el.PFPhotonIso04() + el.PFNeutralHadronIso04() - ( jetrho * effective_area_04 ), 0.0 );

  egamma_pfiso_03 /= el.Pt();
  egamma_pfiso_04 /= el.Pt();
  
  //----------------------------------------------------------------------
  // Barrel electron test
  //----------------------------------------------------------------------

  if ( fabs(el.SCEta()) < 1.479 ){

    pass_deltaEta      = bool ( fabs(el.DeltaEta())   <= l_b_dEtaIn  [ id ] ) ;
    pass_deltaPhi      = bool ( fabs(el.DeltaPhi())   <= l_b_dPhiIn  [ id ] ) ;
    pass_sigmaIEtaIEta = bool ( el.SigmaIEtaIEta()    <= l_b_sieie   [ id ] ) ;
    pass_hoe           = bool ( el.HoE            ()  <= l_b_hoe     [ id ] ) ;
    pass_vtxDistXY     = bool ( fabs(el.VtxDistXY())  <= l_b_d0      [ id ] ) ;
    pass_vtxDistZ      = bool ( fabs(el.VtxDistZ ())  <= l_b_dZ      [ id ] ) ;
    pass_ep            = bool ( egamma_ep          <= l_b_ep      [ id ] ) ;
    pass_pfIsolation   = bool ( egamma_pfiso_03    <= l_b_pfRelIso[ id ] ) ;
    pass_convFitProb   = bool ( el.ConvFitProb  ()    <= l_b_vtxProb [ id ] ) ;
    pass_missingHits   = bool ( el.MissingHits()    <= l_b_missHits[ id ] ) ;
    
  } 

  //----------------------------------------------------------------------
  // Endcap electron test
  //----------------------------------------------------------------------

  else if ( fabs(el.SCEta()) > 1.479 && fabs(el.SCEta()) < 2.5 ){ 

    pass_deltaEta      = bool ( fabs(el.DeltaEta())   <= l_e_dEtaIn  [ id ] ) ;
    pass_deltaPhi      = bool ( fabs(el.DeltaPhi())   <= l_e_dPhiIn  [ id ] ) ;
    pass_sigmaIEtaIEta = bool ( el.SigmaIEtaIEta()    <= l_e_sieie   [ id ] ) ;
    pass_hoe           = bool ( el.HoE          ()    <= l_e_hoe     [ id ] ) ;
    pass_vtxDistXY     = bool ( fabs(el.VtxDistXY())  <= l_e_d0      [ id ] ) ;
    pass_vtxDistZ      = bool ( fabs(el.VtxDistZ ())  <= l_e_dZ      [ id ] ) ;
    pass_ep            = bool ( egamma_ep          <= l_e_ep      [ id ] ) ;
    pass_pfIsolation   = bool ( egamma_pfiso_03    <= l_e_pfRelIso[ id ] ) ;
    pass_convFitProb   = bool ( el.ConvFitProb  ()    <= l_e_vtxProb [ id ] ) ;
    pass_missingHits   = bool ( el.MissingHits()    <= l_e_missHits[ id ] ) ;
  }

  bool decision = ( 
		      pass_deltaEta      && 
		         pass_deltaPhi      && 
		         pass_sigmaIEtaIEta && 
		         pass_hoe           && 
		         pass_vtxDistXY     && 
		         pass_vtxDistZ      && 
		         pass_ep            && 
		         pass_pfIsolation   && 
		         pass_convFitProb   && 
		      pass_missingHits   ) ;
  
  return decision;
  
}

bool ElectronSelection::PassUserID_MVA (snu::KElectron el, bool triggermva){
  
  int id = el.PassID();
  
  int cut (0);
  if(triggermva) cut = 32;
  else cut =64;
  
  return (id >=  cut);
}


ElectronSelection& ElectronSelection::operator= (const ElectronSelection& ms) {
  if(this != &ms){    
    BaseSelection::operator = (ms); 
    k_lqevent = ms.k_lqevent;  
  }
  return *this;
};

ElectronSelection::ElectronSelection(const ElectronSelection& ms): 
  BaseSelection(ms)
{
  k_lqevent = ms.k_lqevent;

};

