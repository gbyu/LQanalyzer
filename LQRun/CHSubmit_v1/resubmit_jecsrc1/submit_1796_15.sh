#!/bin/sh
#$ -S /bin/bash 
#$ -N CHttbarMS  
#$ -wd /data4/LQ_SKTreeOutput/gbyu/SKttbarMS_pf_716210_cmscluster.snu.ac.kr/Job_1796/ 
#$ -o /data4/LQ_SKTreeOutput/gbyu/SKttbarMS_pf_716210_cmscluster.snu.ac.kr/Job_1796/ 
#$ -e /data4/LQ_SKTreeOutput/gbyu/SKttbarMS_pf_716210_cmscluster.snu.ac.kr/Job_1796/ 
echo "Job started at " `date` 
cd /share/apps/root_v5_34_32/root/ 
. bin/thisroot.sh 
cd /data4/LQAnalyzerCode/gbyu/CHs_v2_sub/LQanalyzer/
source setup.sh 
echo "PWD= "$PWD 
cd /data4/LQ_SKTreeOutput/gbyu/SKttbarMS_pf_716210_cmscluster.snu.ac.kr/Job_1796/ 
root -l -q -b /data4/LQ_SKTreeOutput/gbyu/SKttbarMS_pf_716210_cmscluster.snu.ac.kr/Job_1796/runJob_1796_15.C 
echo "Ran macro 2" 
