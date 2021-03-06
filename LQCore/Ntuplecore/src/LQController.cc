// Local includes
#include "LQController.h"
#include "ILQCycleBase.h"
#include "LQLogWriter.h"
#include "LQConstants.h"


// System include(s):                                                           
#include <cxxabi.h>
#include <cstdlib>

// STL include(s):
#include <stdio.h>  
#include <stdlib.h>     /* getenv */
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <fstream>
#include <iomanip>

// ROOT include(s): 
#include <TROOT.h>
#include <TFile.h>
#include <TClass.h>
#include <TStopwatch.h>
#include <TClass.h>
#include <TSystem.h>
#include <TChain.h>

LQController::LQController():inputType(NOTSET), outputLevelString("INFO"), CycleName("Analyzer"), jobName("Test"), treeName("rootTupleTree/tree"),filelist(""), fullfilelist(""), completename(""),runnp(false), runcf(false), m_logger( "LQCycleController") , target_luminosity(1.),  sample_crosssection(-999.), effective_luminosity(1.), n_total_event(-1.),  nevents_to_process(-1), m_isInitialized( kFALSE ), n_ev_to_skip(0), v_libnames(0), list_to_run(0),single_ev(0), run_single_event(false), total_events_beforeskim(0), total_events_afterskim(0),output_step(10000), channel(""), k_period("NOTSET"), kLQInput(true){
  
  chain = NULL;
  h_timing_hist = new TH1F ("CycleTiming","Timing", 7,0.,7.);
  h_timing_hist->GetYaxis()->SetTitle("Time (s)");
  h_timing_hist->GetXaxis()->SetBinLabel(1,"Initialisation");
  h_timing_hist->GetXaxis()->SetBinLabel(2,"BeginCycle");
  h_timing_hist->GetXaxis()->SetBinLabel(3,"QuarterExecute");
  h_timing_hist->GetXaxis()->SetBinLabel(4,"HalfExecute");
  h_timing_hist->GetXaxis()->SetBinLabel(5,"ThreeQuarterExecute");
  h_timing_hist->GetXaxis()->SetBinLabel(6,"FullExecute");
  h_timing_hist->GetXaxis()->SetBinLabel(7,"EndCycle");
  h_virtmemory_hist = new TH1F ("CycleVirtualMemoryUsage","Memory", 8,0.,8.);
  h_virtmemory_hist->GetYaxis()->SetTitle("Usage (kB)");
  h_virtmemory_hist->GetXaxis()->SetBinLabel(1, "LoadLibraries");
  h_virtmemory_hist->GetXaxis()->SetBinLabel(2, "EndInitialization");
  h_virtmemory_hist->GetXaxis()->SetBinLabel(3, "BeginCycle");
  h_virtmemory_hist->GetXaxis()->SetBinLabel(4, "QuarterExecute");
  h_virtmemory_hist->GetXaxis()->SetBinLabel(5, "HalfExecute");
  h_virtmemory_hist->GetXaxis()->SetBinLabel(6, "ThreeQuarterExecute");
  h_virtmemory_hist->GetXaxis()->SetBinLabel(7, "FullExecute");
  h_virtmemory_hist->GetXaxis()->SetBinLabel(8, "EndCycle");
  h_physicalmemory_hist = new TH1F ("CyclePhysicalMemoryUsage","Memory", 8,0.,8.);
  h_physicalmemory_hist->GetYaxis()->SetTitle("Usage (kB)");
  h_physicalmemory_hist->GetXaxis()->SetBinLabel(1, "LoadLibraries");
  h_physicalmemory_hist->GetXaxis()->SetBinLabel(2, "EndInitialization");
  h_physicalmemory_hist->GetXaxis()->SetBinLabel(3, "BeginCycle");
  h_physicalmemory_hist->GetXaxis()->SetBinLabel(4, "QuarterExecute");
  h_physicalmemory_hist->GetXaxis()->SetBinLabel(5, "HalfExecute");
  h_physicalmemory_hist->GetXaxis()->SetBinLabel(6, "ThreeQuarterExecute");
  h_physicalmemory_hist->GetXaxis()->SetBinLabel(7, "FullExecute");
  h_physicalmemory_hist->GetXaxis()->SetBinLabel(8, "EndCycle");
}

LQController::~LQController(){
  
  delete h_timing_hist;
  delete h_virtmemory_hist;
  delete h_physicalmemory_hist;
}

void LQController::AddLibraries(TString libname){
  v_libnames.push_back(libname);
  
}
void LQController::RunNtupleEvent(Long64_t pick_event){
  list_to_run.push_back(pick_event);
}

void LQController::RunEvent(Long64_t ev){

  if(ev==-1) return;
  run_single_event=true;
  single_ev=ev;
}

void LQController::SetOutPutStep(int step){
  output_step = step;
}

void LQController::SetName(TString name, Int_t version, TString dir) {
  
  string out_dir = getenv("LQANALYZER_OUTPUT_PATH");
  if(!dir.Contains("NULL")) out_dir = dir;
  
  completename = TString(out_dir) + name + "_";
  completename += version;
  completename += ".root";

  return;
}

void LQController::SkipEvents(int toskip){
  n_ev_to_skip = toskip;
}


void LQController::SetInputChain(TChain* ch){
  chain = ch;
}


void LQController::SetDataType(TString settype){
  
  if     ( settype == "Data" )    inputType = data;
  else if( settype == "data" )    inputType = data;
  else if( settype == "DATA" )    inputType = data;
  else if( settype == "MC" )      inputType = mc;
  else if( settype == "mc" )      inputType = mc;
  else                            inputType = NOTSET;
  
}

void LQController::RunNonPrompt(TString np){

  if(np.Contains("True")) runnp = true;
  else runnp = false;
  if(np.Contains("True"))m_logger << INFO << "Running Non-Prompt background estimate" << LQLogger::endmsg; 
}


void LQController::RunChargeFlip(TString cf){

  if(cf.Contains("True")) runcf = true;
  else runcf = false;
  if(cf.Contains("True"))m_logger << INFO << "Running ChargeFlip background estimate" << LQLogger::endmsg;
}


std::pair<Double_t, Double_t>  LQController::GetTotalEvents() throw (LQError){
  
  if(fullfilelist.Contains("NULL")){
    return std::make_pair(0.,0.);
  }
  std::ifstream fin(fullfilelist.Data());
  std::string word;
  int ifile(0);

  total_events_beforeskim = 0.;
  total_events_afterskim = 0.;

  if(fin.is_open()){
    while(getline (fin,word)){

      TFile * file = TFile::Open(word.c_str());
      TH1I*  EventCounter = (TH1I*) (file ->Get("LJFilter/EventCount/EventCounter"));
      if(!EventCounter) throw LQError( "LJFilter/EventCount/EventCount NOT found!!!",   LQError::StopExecution );

      total_events_beforeskim += EventCounter->GetBinContent(1);
      total_events_afterskim += EventCounter->GetBinContent(2);
      file->Close();
      delete file;
      ifile++;
    }
    fin.close();
  }
  
  return std::make_pair(total_events_beforeskim ,total_events_afterskim);
}


void LQController::SetChannel(TString ch){
  
  if     (ch == "muon")  channel = "Muon";
  else if     (ch == "egamma")  channel = "Electron";
  else if     (ch == "emu")  channel = "EMu";
  else if     (ch == "singlemuon")      channel = "singleMuon";
  else if     (ch == "singleelectron")  channel = "singleElectron";
  
}
void LQController::SetDataPeriod(TString period){
  
  if( period == "A") k_period = "A";
  else if( period == "B") k_period = "B";
  else if( period == "C") k_period = "C";
  else if( period == "D") k_period = "D";
  else if( period == "AtoB") k_period = "AtoB";
  else if( period == "AtoC") k_period = "AtoC";
  else if( period == "AtoD") k_period = "AtoD";
  else {
    m_logger << ERROR << "Failed to correctly set data period" << LQLogger::endmsg;
    throw LQError( "Data Period not correctly set!!!",
		   LQError::StopExecution );
  }
  
  
  if( period == "A") target_luminosity = 887.501;
  else if( period == "B") target_luminosity = 4443.;
  else if( period == "C") target_luminosity = 7114.;
  else if( period == "D") target_luminosity = 7318.;
  else if( period == "AtoB") target_luminosity = (887.501 + 4443.);
  else if( period == "AtoC") target_luminosity = (887.501 + 4443. + 7114.);
  else target_luminosity = (887.501 + 4443. + 7114. + 7318.);

  
}

void LQController::SetTotalMCEvents(int ev){
  n_total_event = ev;
}
  
void LQController::SetMCCrossSection(float xsec){
  m_logger << INFO << "Set cross section: This is be used to calculate event weights. WARNING be careful." << LQLogger::endmsg;
  m_logger << INFO << "Will use weight = (lumi *  cs(pb) * gen filter efficiency) / MCevents "  << LQLogger::endmsg;
  sample_crosssection = xsec;
}


void LQController::SetJobName(TString name){
  jobName = name;
}
  


void LQController::SetInputList(TString list){
  filelist = list;
}

void LQController::SetFullInputList(TString list){
  fullfilelist = list;
}

void LQController::SetTreeName(TString treename){
  treeName = treename;
}

void LQController::SetCycleName(TString cyclename){
  CycleName= cyclename;

}

void LQController::SetLogLevel(TString  level){
  outputLevelString = level;
}



void LQController::SetNEventsToProcess(int nevents){

  nevents_to_process = nevents;
}

void LQController::SetTargetLuminosity(float target_lumi){

  target_luminosity= target_lumi;
}
  

void LQController::SetEffectiveLuminosity(float eff_lumi){

  effective_luminosity= eff_lumi;
}


void LQController::Initialize() throw( LQError ){
  
  m_logger << DEBUG << "Initializing" << LQLogger::endmsg;
  
  GetMemoryConsumption("Start of Initialize()");

  // Just for kicks, lets measure the time it needs to initialise the                                                      
  // analysis:    
  TStopwatch timer;
  timer.Start();

  
  ///// List of things that can be set to congigure cycle:
  /////
  /*
    1) CycleName (i.e HNCycle)
    2) OUT put log level (i.e, INFO)
    3) libraires to load
    4) Set jobName
  */
  
  LQMsgType type = INFO;
  if     ( outputLevelString == "VERBOSE" ) type = VERBOSE;
  else if( outputLevelString == "DEBUG" )   type = DEBUG;
  else if( outputLevelString == "INFO" )    type = INFO;
  else if( outputLevelString == "WARNING" ) type = WARNING;
  else if( outputLevelString == "ERROR" )   type = ERROR;
  else if( outputLevelString == "FATAL" )   type = FATAL;
  else if( outputLevelString == "ALWAYS" )  type = ALWAYS;
  else {
    m_logger << WARNING << "Message output level ("
	     << outputLevelString << ") not recognized"
	     << LQLogger::endmsg;
  }
  
  LQLogWriter::Instance()->SetMinType( type );
  
  
  ////// Either  use default OR use class specified by user
  try {
    for(std::vector<TString>::iterator lit = v_libnames.begin(); lit != v_libnames.end(); lit++){
      TString libraryName = *lit;
      REPORT_VERBOSE( "Trying to load library \"" << libraryName << "\"" );
      int ret = 0;
      if( ( ret = gSystem->Load( libraryName.Data() ) ) >= 0 ) {
	m_logger << INFO << "Library loaded: \"" << libraryName << "\""
		 << LQLogger::endmsg;
      } else {
	LQError error( LQError::StopExecution );
	error << "Library failed to load: \"" << libraryName
	<< "\"\nRet. Val.: " << ret;
	throw error;
      }
      
      GetMemoryConsumption("Added library " + libraryName);
    }// lib loop
    
    FillMemoryHists("LoadLibraries");
  }
  catch( const LQError& error ) {
    //                                                                                                              
    // This is where I catch "cycle level" problems:                                                                
    //                                                                                                              
    if( error.request() <= LQError::SkipCycle ) {
    //// If just this cycle has to be skipped:                                                                     
      REPORT_ERROR( "Message: " << error.what() );
      REPORT_ERROR( "--> Skipping cycle!" );
      
      return;
    } else {
      m_logger << INFO << "FAILED" << LQLogger::endmsg;
     // If this is more serious:                                                                                  
      throw;
    }
  }
  
  m_logger << INFO << "Job '" << jobName << "' configured" << LQLogger::endmsg;
  
  timer.Stop();
  m_logger << INFO << "Time needed for initialisation: " << std::setw( 6 )
	   << std::setprecision( 2 ) << timer.RealTime() << " s"
	   << LQLogger::endmsg;
  h_timing_hist->Fill("Initialisation", timer.RealTime());
  // Print memory consumption after initialising the analysis:                                                             
  ProcInfo_t procinfo;
  gSystem->GetProcInfo( &procinfo );
  m_logger << DEBUG << "Memory consumption after initialisation:" << LQLogger::endmsg;
  m_logger.setf( std::ios::fixed );
  m_logger << DEBUG << "  Resident mem.: " << std::setw( 7 ) << procinfo.fMemResident
	   << " kB; Virtual mem.: " << std::setw( 7 ) << procinfo.fMemVirtual
	   << " kB" << LQLogger::endmsg;
  
  FillMemoryHists("EndInitialization");
  // set object status to be ready                                                                                         
  m_isInitialized = kTRUE;
}

void LQController::ExecuteCycle() throw( LQError ) {
    
  if( ! m_isInitialized ) {
    throw LQError( "LQCycleController is not initialized",
    LQError::StopExecution );
  }

  TString muonfitParametersFile = "";
  GetMemoryConsumption("Start of ExecuteCycle");
  m_logger << DEBUG << "Entering ExecuteCycles" << LQLogger::endmsg;

  //                                                                                                                       
  // Measure the total time needed for this cycle:                                                                         
  //                                                                                                                       
  TStopwatch timer;
  timer.Start();
  //                                                                                                                       
  // Access the current cycle:                                                                                             
  //
  ////// Either  use default OR use class specified by user                                                                                                       
  try {
    
    TClass* cycleClass  = gROOT->GetClass(CycleName.Data(), true);
    if( ! cycleClass){
      LQError error( LQError::SkipCycle );
      m_logger << INFO << "Failed to get class" <<  LQLogger::endmsg;
      throw error;
    }
    ILQCycleBase* cycle = reinterpret_cast<ILQCycleBase*> ( cycleClass->New());
    
    TString      cycleName = CycleName;
    m_logger << INFO << "Created cycle '" << cycleName << "'"
             << LQLogger::endmsg;
    
    GetMemoryConsumption("Initialised cycle class: " + cycleName );
    ///// This executes code:
    
    //                                                                                                                       
    // The begin cycle function has to be called here by hand:                                                               
    // This creates anyoutput files/Trees/Branches for analysis                


    /// Call BeginCycle by hand
    cycle->MakeOutPutFile(completename);
    cycle->SetDataChannel(channel);
    cycle->BeginCycle();
    cycle->ClearOutputVectors();

    GetMemoryConsumption("Ran Begin Cycle");

    if(!kLQInput){
      /// Use SKTree input

      chain = new TChain( "LQTree" );
      if(filelist.Contains("NULL")){
	throw LQError( "Filelist is null!!!",
		       LQError::StopExecution );
      }
      std::ifstream fin(filelist.Data());
      std::string word;
      
      if(!chain) {
	throw LQError( "Chain is null!!!",
		       LQError::StopExecution );
      }
      
      if(fin.is_open()){
	while(getline (fin,word)){
	  m_logger <<  INFO << "- " << word << LQLogger::endmsg;
	  chain->Add(word.c_str());
	}
	fin.close();
       
      }
      cycle->SetLQNtupleInputType(false);
    }
    else{
      cycle->SetLQNtupleInputType(true);
      if(!chain){
	m_logger << INFO << filelist << LQLogger::endmsg;
	if(filelist == "0") throw LQError( "No input filelist",
					   LQError::StopExecution );
	///  Get Tree Name / input filename
	chain = new TChain( treeName );
	if(filelist.Contains("NULL")){
	  throw LQError( "Filelist is null!!!",
			 LQError::StopExecution );
	}
	std::ifstream fin(filelist.Data());
	std::string word;
	if(!chain) {
	  throw LQError( "Chain is null!!!",
			 LQError::StopExecution );
	}
	
	if(fin.is_open()){
	  while(getline (fin,word)){      
	    m_logger <<  INFO << "- " << word << LQLogger::endmsg;
	    chain->Add(word.c_str());
	  }
	  fin.close();
	}
	GetMemoryConsumption("Created TChain");
      }
    }

    //// Connect chain to Data class                                                                                                                                        
    cycle->Init(chain);
    GetMemoryConsumption("Connected All Active Branches");
    
    /// We can now check 
    // a) is this Data?
    // b) how many events in sample

    /// tell base ntuple code if sample is data or MC 
    /// This can be set in configuration
    // if not set then we get the first entry of the input TTree and check ourselves
    // if this is set incorrectly then the code will exit in Analysis code
    if(inputType!=NOTSET) {
      // This is if set by user:
      if(inputType == data) cycle->SetDataType(true);
      else if(inputType == mc) cycle->SetDataType(false);
      else throw LQError( "InputType is wrongly configured",LQError::SkipCycle);
    }
    else{
      /// Get answer from input ntuple
      m_logger <<  INFO << chain <<  LQLogger::endmsg;
      cycle->GetInputTree()->GetEntry(1,0);/// Get first entry in ntuple
      bool alt_isdata =  cycle->isData;
      if(alt_isdata) inputType = data;
      else inputType = mc;
      cycle->SetDataType(alt_isdata);
      GetMemoryConsumption("Accessed branch to specify isData");
    }

    cycle->SetNPStatus(runnp);
    cycle->SetCFStatus(runcf);
    cycle->SetSampleName(jobName);
    
    Long64_t nentries = cycle->GetNEntries(); /// This is total number of events in Input list    
    if(n_ev_to_skip > nentries) n_ev_to_skip =0;
    
    if((k_period != "NOTSET") && (inputType == data)) m_logger << INFO << "Running on Data: Period " << k_period  << LQLogger::endmsg;
    if((k_period != "NOTSET") && (inputType == mc)) m_logger << INFO << "Running on MC: This will be weighted to represent period " << k_period << " of data" << LQLogger::endmsg;
    

    // calculate weight from input 
    float ev_weight =  CalculateWeight();
    
    GetMemoryConsumption("Calculated Weight");
    // Set number of event
    if(nevents_to_process > nentries || (nevents_to_process == -1) ) nevents_to_process = nentries;
    else if( nevents_to_process <=0) throw LQError( "nevents_to_process  is wrongly configured",LQError::SkipCycle);
    else {
      if(inputType == mc) ev_weight *= (nentries/nevents_to_process);
      if(inputType == mc) m_logger << INFO << "Weight is recalculated. Since user set a specific number of entries" << LQLogger::endmsg;
      if(inputType == data) m_logger << WARNING << "Weight is not recalculated (as it is data). Even though user set a number of entries to run on." << LQLogger::endmsg;
    }
    cycle->SetNEventsToProcess(nevents_to_process);
    cycle->SetOutPutStep(output_step);
    
    //// Help user understand  event readout
    if(nevents_to_process == nentries) m_logger << INFO <<  "Processing entry (#entry)/(#enties In Job)"<< LQLogger::endmsg;
    else  m_logger << INFO <<  "Processing entry (#entry)/(#enties to process) [(#Entries in input sample)]"<< LQLogger::endmsg;

    timer.Stop();
    h_timing_hist->Fill("BeginCycle", timer.RealTime());
    timer.Start();
    FillMemoryHists("BeginCycle");
    
   
    int m_nSkippedEvents(0);
    int m_nProcessedEvents(0);

    /// Get ints for 1/4 , 1/2 of the list to do timing checks
    //int entry_4 = int((nevents_to_process-n_ev_to_skip)/4.);
    //int entry_3_4 = 3*entry_4;
    //    int entry_2 = int((nevents_to_process-n_ev_to_skip)/2.);
    if(list_to_run.size()!=0){
      for(unsigned int list_entry = 0; list_entry < list_to_run.size(); list_entry++){
	Bool_t skipEvent = kFALSE;
	try {
	  cycle->GetEntry(list_entry);
	  cycle->SetUpEvent(list_entry,ev_weight);
	  cycle->ClearOutputVectors();
	  cycle->BeginEvent();
	  cycle->ExecuteEvents();
	  cycle->EndEvent();
	} catch( const LQError& error ) {
	  if( error.request() <= LQError::SkipEvent ) {
	    skipEvent = kTRUE;
	  } else {
	    throw;
	  }
	}
	if( ! skipEvent ) cycle->FillOutTree();	
      }// list loop
    }/// check size of list loop
    else if(run_single_event){
      for (Long64_t jentry = n_ev_to_skip; jentry < nevents_to_process; jentry++ ) {
	cycle->GetEntry(jentry);	
	if(!(jentry%50000)) m_logger << INFO << "Processing event " << jentry << " " << cycle->GetEventNumber() << LQLogger::endmsg;

	if(cycle->GetEventNumber() == single_ev){
	  cycle->SetUpEvent(jentry, ev_weight);
	  cycle->ClearOutputVectors();
	  cycle->BeginEvent();
	  cycle->ExecuteEvents();
	  cycle->EndEvent();
	  break;
	  }
	else continue;
      }
    }
    else{
      for (Long64_t jentry = n_ev_to_skip; jentry < nevents_to_process; jentry++ ) {            
	m_logger << DEBUG << "cycle->SetUpEvent " << LQLogger::endmsg;
	Bool_t skipEvent = kFALSE;
        try {
	  m_logger << DEBUG << "cycle->GetEvent " << LQLogger::endmsg;

	  cycle->GetEntry(jentry);
	  
	  m_logger << DEBUG << "cycle->SetUpEvent " << LQLogger::endmsg;
	  cycle->SetUpEvent(jentry, ev_weight);
	  
	  m_logger << DEBUG << "cycle->BeginEvent " << LQLogger::endmsg;
	  cycle->ClearOutputVectors();
	  cycle->BeginEvent();   
	  /// executes analysis code
	  m_logger << DEBUG << "cycle->ExecuteEvent " << LQLogger::endmsg;
	  cycle->ExecuteEvents();
	  // cleans up any pointers etc.
	  cycle->EndEvent();
	  m_logger << DEBUG << "cycle->ENDEvent " << LQLogger::endmsg;	  
	}
	catch( const LQError& error ) {
          if( error.request() <= LQError::SkipEvent ) {
            skipEvent = kTRUE;
	    cycle->EndEvent();
          } else {
	    throw;
	  }
	}

	if( ! skipEvent ) cycle->FillOutTree();
	else ++m_nSkippedEvents;
	
	++m_nProcessedEvents;
	
	/*if( jentry == entry_4) {
	  timer.Stop();
	  h_timing_hist->Fill("QuarterExecute", timer.RealTime());
	  timer.Start();
	  FillMemoryHists("QuarterExecute");
	}
	if( jentry == entry_2) {
	  timer.Stop();
	  h_timing_hist->Fill("HalfExecute", timer.RealTime());
	  timer.Start();
	  FillMemoryHists("HalfExecute");
	}
	if( jentry == entry_3_4) {
	  timer.Stop();
	  h_timing_hist->Fill("ThreeQuarterExecute", timer.RealTime());
	  timer.Start();
	  FillMemoryHists("ThreeQuarterExecute");
	  }*/
      }
    }     
    //    timer.Stop();
    h_timing_hist->Fill("FullExecute", timer.RealTime());
    //timer.Start();
    m_logger << INFO << "Execute time = " << timer.RealTime() << " s" << LQLogger::endmsg;
    FillMemoryHists("FullExecute");

    cycle->SaveOutputTrees(cycle->GetOutputFile());
    cycle->EndCycle();
    cycle->WriteHistograms();/// writes all histograms declared in the cycle to the output file

    //timer.Stop();
    h_timing_hist->Fill("EndCycle", timer.RealTime());
    FillMemoryHists("EndCycle");
    
    delete chain;
    cycle->WriteCycleHists(h_timing_hist,h_virtmemory_hist,h_physicalmemory_hist);
    cycle->CloseFiles();

    m_logger << INFO << "Number of event processed = " << m_nProcessedEvents << LQLogger::endmsg;
    m_logger << INFO << "Number of event skipped = " << m_nSkippedEvents << LQLogger::endmsg;
    
    GetMemoryConsumption("Finished Running Cycle");
  }
  
  catch( const LQError& error ) {
    // 
    // This is where I catch "cycle level" problems: 
    //                                                                                                                                                            
    if( error.request() <= LQError::SkipCycle ) {
      //If just this cycle has to be skipped:                                                                                                                    
      REPORT_ERROR( "Message: " << error.what() );
      REPORT_ERROR( "--> Skipping cycle!" );
      
      return;
    } else {
    //// If this is more serious:                                                                                                                                 
      throw;
    }
  }
  
}
 

void LQController::SetLQInput(bool lq){
  kLQInput=lq;
}


float LQController::CalculateWeight() throw(LQError) {
  
  // First check that sample is MC
  if(inputType == NOTSET){
    throw LQError( "inputType is failed to be set!!",   LQError::StopExecution );
    return 1.;
  }
  if(inputType == data) return 1.;
  
  //
  // Calculate weight for MC
  //
  
  m_logger <<  INFO << "Target lumi = " << target_luminosity << " " << effective_luminosity << LQLogger::endmsg;
  if(target_luminosity != 1. || effective_luminosity != 1.){
    m_logger <<  INFO << "Target lumi = " << target_luminosity <<LQLogger::endmsg;
    if(target_luminosity == 1. ) m_logger << WARNING << "Target_luminosity is set to 1. while effective_luminosity is not. Is this correct?" <<LQLogger::endmsg;
    if(effective_luminosity == 1. ) m_logger << WARNING <<"Effective_luminosity is set to 1. while target_luminosity is not. Is this correct?" <<LQLogger::endmsg;    
    return (target_luminosity/effective_luminosity);
  }
  
  
  if(n_total_event != 0){
    if(sample_crosssection  ==999.) m_logger << WARNING <<" SetTotalMCEvents was set but no xsec is given as input" <<LQLogger::endmsg;
  }
  else {
    m_logger << WARNING <<"Total number of sample events is being used to calculate MC event weight:"<<LQLogger::endmsg;
    m_logger << ERROR  <<"n_total_event is not configuured "<<LQLogger::endmsg;
    m_logger << WARNING <<"You can use SetFullInputFileList("") or SetTotalMCEvents(int) to configure this" <<LQLogger::endmsg;
  }
  
  if(sample_crosssection != 999.&& n_total_event != -1.){
    return (target_luminosity* sample_crosssection / n_total_event);
  }
  
  m_logger << INFO << "Weight is set to 1. by default. This is ONLY correct for data. " << LQLogger::endmsg;
  return 1.;
}


void LQController::FillMemoryHists(std::string binname){
  h_virtmemory_hist->Fill(binname.c_str(), GetVirtualMemoryConsumption());
  h_physicalmemory_hist->Fill(binname.c_str(), GetPhysicalMemoryConsumption());
}


void LQController::GetMemoryConsumption(TString label){

  ProcInfo_t procinfo;
  gSystem->GetProcInfo( &procinfo );
  m_logger << DEBUG << "Memory consumption at: " << label << LQLogger::endmsg;
  m_logger.setf( std::ios::fixed );
  m_logger << DEBUG << "  Resident mem.: " << std::setw( 7 ) << procinfo.fMemResident
	   << " kB; Virtual mem.: " << std::setw( 7 ) << procinfo.fMemVirtual
	   << " kB" << LQLogger::endmsg;

}
double LQController::GetPhysicalMemoryConsumption(){
  
  ProcInfo_t procinfo;
  gSystem->GetProcInfo( &procinfo );
  return procinfo.fMemResident;
}

double LQController::GetVirtualMemoryConsumption(){

  ProcInfo_t procinfo;
  gSystem->GetProcInfo( &procinfo );
  return procinfo.fMemVirtual;
}

