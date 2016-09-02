#!/bin/sh
#$ -S /bin/bash 
#$ -N CHttbarMS  
#$ -wd /data4/LQ_SKTreeOutput/gbyu/SKttbarMS_pf_716210_cmscluster.snu.ac.kr/Job_2092/ 
#$ -o /data4/LQ_SKTreeOutput/gbyu/SKttbarMS_pf_716210_cmscluster.snu.ac.kr/Job_2092/ 
#$ -e /data4/LQ_SKTreeOutput/gbyu/SKttbarMS_pf_716210_cmscluster.snu.ac.kr/Job_2092/ 
echo "Job started at " `date` 
cd /share/apps/root_v5_34_32/root/ 
. bin/thisroot.sh 
cd /data4/LQAnalyzerCode/gbyu/CHs_v2_sub/LQanalyzer/
source setup.sh 
echo "PWD= "$PWD 
cd /data4/LQ_SKTreeOutput/gbyu/SKttbarMS_pf_716210_cmscluster.snu.ac.kr/Job_2092/ 
root -l -q -b /data4/LQ_SKTreeOutput/gbyu/SKttbarMS_pf_716210_cmscluster.snu.ac.kr/Job_2092/runJob_2092_16.C 
echo "Ran macro 2" 
