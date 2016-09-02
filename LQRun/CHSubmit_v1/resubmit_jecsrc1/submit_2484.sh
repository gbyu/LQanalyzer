#!/bin/sh
#$ -S /bin/bash 
#$ -N CHttbarMS  
#$ -wd /data4/LQ_SKTreeOutput/gbyu/SKttbarMS_pf_711228_cmscluster.snu.ac.kr/Job_2484/ 
#$ -o /data4/LQ_SKTreeOutput/gbyu/SKttbarMS_pf_711228_cmscluster.snu.ac.kr/Job_2484/ 
#$ -e /data4/LQ_SKTreeOutput/gbyu/SKttbarMS_pf_711228_cmscluster.snu.ac.kr/Job_2484/ 
echo "Job started at " `date` 
cd /share/apps/root_v5_34_32/root/ 
. bin/thisroot.sh 
cd /data4/LQAnalyzerCode/gbyu/CHs_v2/LQanalyzer/
source setup.sh 
echo "PWD= "$PWD 
cd /data4/LQ_SKTreeOutput/gbyu/SKttbarMS_pf_711228_cmscluster.snu.ac.kr/Job_2484/ 
root -l -q -b /data4/LQ_SKTreeOutput/gbyu/SKttbarMS_pf_711228_cmscluster.snu.ac.kr/Job_2484/runJob_2484.C 
echo "Ran macro 2" 
