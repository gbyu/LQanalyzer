#!/bin/sh

rundata=true
runmc=true
runmc_cl=false
runsig=true
runfakes=true
runflips=true

if [[ $1  == "NP" ]];
    then
    runfakes=true
    rundata=false
    runmc=false
    runsig=false
    runflips=false
fi

if [[ $1  == "CF" ]];
    then
    runfakes=false
    rundata=false
    runmc=false
    runsig=false
    runflips=true
fi

if [[ $1  == "DATA" ]];
    then
    runfakes=false
    rundata=true
    runmc=false
    runsig=false
    runflips=false
fi

if [[ $1  == "MC" ]];
    then
    runfakes=false
    rundata=false
    runmc=true
    runsig=false
    runflips=false
fi

if [[ $1  == "CL" ]];
    then
    runfakes=false
    rundata=false
    runmc=false
    runmc_cl=true
    runsig=false
    runflips=false
fi



if [[ $1  == "SIG" ]];
    then
    runfakes=false
    rundata=false
    runmc=false
    runsig=true
    runflips=false
fi

if [[ $1  == "testMC" ]];
    then
    
    rundata=false
    runmc=false
    runsig=false
    runfakes=false
    runflips=false
    source functions.sh
    
    cycle="HNDiElectron"
    skinput="True"
    useskim="DiLep"
    loglevel="INFO"

    njobs=1
    data_lumi="AtoD"
    nevents=100000000
    
    declare -a input_samples=("DY50plus")

    stream="egamma"
    outputdir="./"
    
    
    ### submit this configured job (uses bin/submit.sh)
    source submit.sh $1
fi



if [[ $1  == "testData" ]];
then
    source functions.sh
    rundata=false
    runmc=false
    runsig=false
    runfakes=false
    runflips=false
    
    cycle="HNDiElectron_basic"
    skinput="True"
    useskim="DiLep"
    njobs=1
    data_lumi="AtoD"
    
    nevents=1000000
    loglevel="INFO"
    declare -a input_samples=("A")
    
    stream="egamma"
    outputdir=$LQANALYZER_DIR"/data/output/SSElectron/"
    
    ### submit this configured job (uses bin/submit.sh)
    source submit.sh $1
    
fi

if [[ $1  == "testNP" ]];
then
    source functions.sh

    rundata=false
    runmc=false
    runsig=false
    runfakes=false
    runflips=false
    
    cycle="HNDiElectron"
    skinput="True"
    useskim="DiLep"

    njobs=30
    data_lumi="AtoD"

    nevents=10000
    loglevel="INFO"
    runnp="True"
    declare -a input_samples=("C")

    stream="egamma"
    outputdir=$LQANALYZER_DIR"/data/output/SSElectron/"

    ### submit this configured job (uses bin/submit.sh)
    source submit.sh $1

fi


if [[ $runsig  == "true" ]];
then
    #signal
    source functions.sh
    
    cycle="HNDiElectron_basic"
    skinput="True"
    useskim="NoCut"

    njobs=30
    data_lumi="AtoD"
    loglevel="INFO"

    logstep=1000
    
    declare -a input_samples=("HNee40" "HNee50" "HNee60" "HNee70" "HNee80" "HNee90" "HNee100" "HNee125" "HNee150" "HNee175" "HNee200" "HNee225" "HNee250" "HNee275" "HNee300" "HNee325" "HNee350" "HNee375" "HNee400" "HNee500" "HNee600" "HNee700")
    
    outputdir=$LQANALYZER_DIR"/data/output/SSElectron_ntup/"
    ### submit this configured job (uses bin/submit.sh)
    source submit.sh $1
fi



if [[ $runmc  == "true" ]];

then

    source functions.sh
    
    cycle="HNDiElectron_basic"
    skinput="True"
    useskim="DiLep"
    
    njobs=1
    data_lumi="AtoD"
    loglevel="INFO"

    loglevel="INFO"
    logstep=1000
    declare -a input_samples=("SSWmWm" "SSWpWp" "WW_dp" "ttW" "ttZ" "WWW" "TTWW" "TTG" "ZZZ" "WZZ" "WWZ" "WWG" "WW_py" "WZ_py" "ZZ_py" "HtoWW" "ttbar" "DY10to50" "DY50plus" "WW_py" "WW_mg" "HtoTauTau" "ggHtoZZ")

    declare -a input_samples=("HtoWW")
    outputdir=$LQANALYZER_DIR"/data/output/SSElectron_ntup/MC/"
    ### submit this configured job (uses bin/submit.sh)
    source submit.sh $1
    source hadd.sh ${LQANALYZER_DIR}/data/output/SSElectron/MC/  HNDiElectron_basic_mc_5_3_14.root HNDiElectron_basic_*

fi

if [[ $runmc_cl  == "true" ]];

then

    source functions.sh

    cycle="HNDiElectron"
    skinput="True"
    useskim="DiLep"
    
    njobs=30
    data_lumi="AtoD"
    loglevel="INFO"

    loglevel="INFO"
    logstep=1000
    
    declare -a input_samples=("Wjets" "ttbar" )
    #"QCD_30-40_EM2" "QCD_40_EM2" "QCD_20_30_EM" "QCD_20_30_BCtoE" "QCD_30_80_EM" "QCD_30_80_BCtoE" "QCD_80_170_EM" "QCD_80_170_BCtoE" "QCD_170_250_EM" "QCD_170_250_BCtoE" "QCD_250_350_EM" "QCD_250_350_BCtoE" "QCD_350_EM" "QCD_350_BCtoE" )
    
    runnp="True"

    outputdir=$LQANALYZER_DIR"/data/output/SSElectron/MC/"
    ### submit this configured job (uses bin/submit.sh)
    source submit.sh $1
fi





if [[ $rundata  == "true" ]];
then

    source functions.sh

    cycle="HNDiElectron_basic"
    skinput="True"
    useskim="DiLep"

    njobs=1
    data_lumi="AtoD"
    loglevel="INFO"

    loglevel="INFO"
    logstep=1000

    declare -a input_samples=( "A" "B" "C" "D")
     declare -a input_samples=( "A" )
    stream="egamma"

    outputdir=$LQANALYZER_DIR"/data/output/SSElectron_ntup/"
    ### submit this configured job (uses bin/submit.sh)
    source submit.sh $1
    source hadd.sh ${LQANALYZER_DIR}/data/output/SSElectron/  HNDiElectron_basic_data_5_3_14.root HNDiElectron_basic_period*
fi


if [[ $runfakes  == "true" ]];
    then
    source functions.sh
    
    ### FAKE
    cycle="HNDiElectron_basic"
    skinput="True"
    useskim="DiLep"
    loglevel="INFO"

    njobs=30
    data_lumi="AtoD"
    
    loglevel="INFO"
    logstep=1000
    
    runnp="True"
    declare -a input_samples=("A" "B" "C" "D") 
    
    stream="egamma"
    outputdir=$LQANALYZER_DIR"/data/output/SSElectron_ntup/"


    ### submit this configured job (uses bin/submit.sh)
    source submit.sh $1
    source hadd.sh ${LQANALYZER_DIR}/data/output/SSElectron_ntup/  HNDiElectron_basic_SKnonprompt_dilep_5_3_14.root HNDiElectron_basic_nonprompt_period*
fi

if [[ $runflips  == "true" ]];
    then
    source functions.sh
    
    cycle="HNDiElectron_basic"
    skinput="True"
    useskim="DiLep"
    loglevel="INFO"
    
    njobs=30
    data_lumi="AtoD"

    loglevel="INFO"
    logstep=1000

    runcf="True"
    declare -a input_samples=("A" "B" "C" "D" )
        
    stream="egamma"
    outputdir=$LQANALYZER_DIR"/data/output/SSElectron/"
    
    
    ### submit this configured job (uses bin/submit.sh)
    source submit.sh $1
    source hadd.sh ${LQANALYZER_DIR}/data/output/SSElectron/  HNDiElectron_basic_SKchargeflip_dilep_5_3_14.root HNDiElectron_basic_chargeflip_*
fi



echo ""
echo "End of example_submit.sh script."