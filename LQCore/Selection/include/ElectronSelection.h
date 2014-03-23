#ifndef ElectronSelection_h
#define ElectronSelection_h

#include "KElectron.h"
#include "LQEvent.h"
#include "BaseSelection.h" 

class ElectronSelection : public BaseSelection {

 public:
  
  Bool_t ElectronID;


  ElectronSelection(LQEvent ev);
  ~ElectronSelection();

  ElectronSelection& operator= (const ElectronSelection& obj);
  ElectronSelection(const ElectronSelection& bs);
  
  void Selection(std::vector<snu::KElectron>& leptonColl);		 
  void BasicSelection(std::vector<snu::KElectron>& leptonColl);
  void SkimSelection(std::vector<snu::KElectron>& leptonColl);
  
  //// HN analysis selection
  void HNTightElectronSelection(std::vector<snu::KElectron>& leptonColl) ;
  void HNLooseElectronSelection(std::vector<snu::KElectron>& leptonColl) ;
  void HNVetoElectronSelection(std::vector<snu::KElectron>& leptonColl) ;
  

  //// IDS
  bool PassUserID(ID id,snu::KElectron el,  double rho);
  bool PassUserID_EGamma2012 ( ID id, snu::KElectron el , double rho) ;
  bool PassUserID_ECALFiducial (snu::KElectron el);
  bool PassUserID_MVA (snu::KElectron el, bool trig);
  bool PassUserID_FakeLoose2012 (snu::KElectron el, double jetrho );

    
};
#endif
