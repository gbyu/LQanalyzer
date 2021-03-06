################################################################### 
### configure Job
#################################################################### 
timeWait=1#

###################################################
### Make Input File
###################################################
import os, getpass, sys
from functions import *

from optparse import OptionParser

#Import parser to get options
parser = OptionParser()
parser.add_option("-p", "--period", dest="period", default="A",help="which data period or mc sample")
parser.add_option("-s", "--stream", dest="stream", default="", help="Which data channel- ee,or mumu?")
parser.add_option("-j", "--jobs", dest="jobs", default=1, help="Name of Job")
parser.add_option("-c", "--cycle", dest="cycle", default="Analyzer", help="which cycle")
parser.add_option("-t", "--tree", dest="tree", default="rootTupleTree/tree", help="What is input tree name?")
parser.add_option("-o", "--logstep", dest="logstep", default=-1, help="How many events betwene log messages")
parser.add_option("-d", "--data_lumi", dest="data_lumi", default="A", help="How much data are you running on/ needed to weight mc?")
parser.add_option("-l", "--loglevel", dest="loglevel", default="INFO", help="Set Log output level")
parser.add_option("-n", "--nevents", dest="nevents", default=-1, help="Set number of events to process")
parser.add_option("-k", "--skip", dest="skip", default=-1, help="Set number of events to skip")
parser.add_option("-a", "--datatype", dest="datatype", default="", help="Is data or mc?")
parser.add_option("-e", "--totalev", dest="totalev", default=-1, help="How many events in sample?")
parser.add_option("-x", "--xsec", dest="xsec", default=-1., help="How many events in sample?")
parser.add_option("-T", "--targetlumi", dest="targetlumi", default=-1., help="How many events in sample?")
parser.add_option("-E", "--efflumi", dest="efflumi", default=-1., help="How many events in sample?")
parser.add_option("-O", "--outputdir", dest="outputdir", default="${LQANALYZER_DIR}/data/output/", help="Where do you like output to go?")
parser.add_option("-w", "--remove", dest="remove", default=True, help="Remove the work space?")
parser.add_option("-S", "--skinput", dest="skinput", default=True, help="Use SKTree as input?")
parser.add_option("-R", "--runevent", dest="runevent", default=True, help="Run Specific Event?")
parser.add_option("-N", "--use5312ntuples", dest="use5312ntuples", default=True, help="use5312ntuples? add use5312ntuples='True' to run on these samples")
parser.add_option("-M", "--use538ntuples", dest="use538ntuples", default=True, help="use538ntuples? add use5384ntuples='True' to run on these samples")
parser.add_option("-L", "--LibList", dest="LibList", default="", help="Add extra lib files to load")
parser.add_option("-D", "--debug", dest="debug", default=False, help="Run submit script in debug mode?")
parser.add_option("-m", "--useskim", dest="useskim", default="Lepton", help="Run submit script in debug mode?")
parser.add_option("-P", "--runnp", dest="runnp", default="runnp", help="Run fake mode for np bkg?")
parser.add_option("-Q", "--runcf", dest="runcf", default="runcf", help="Run fake mode for np bkg?")


###################################################
#set the local variables using options
###################################################
(options, args) = parser.parse_args()
number_of_cores = int(options.jobs)
sample = options.period
channel = options.stream
cycle = options.cycle
logstep = int(options.logstep)
loglevel = options.loglevel
runnp = options.runnp
runcf = options.runcf
### THESE ARE OPTIONS THAT CAN BE INCLUDED but not in example
tree = options.tree
number_of_events_per_job= int(options.nevents)
skipev = int(options.skip)
dataType = options.datatype
totalev = int(options.totalev)
xsec = float(options.xsec)
tar_lumi = float(options.targetlumi)
eff_lumi = float(options.efflumi)
data_lumi = options.data_lumi
Finaloutputdir = options.outputdir
remove_workspace=options.remove
useskinput=options.skinput
runevent= options.runevent
use5312ntuples = options.use5312ntuples
use538ntuples = options.use538ntuples
tmplist_of_extra_lib=options.LibList
DEBUG = options.debug
useskim = options.useskim

original_channel = channel


list_of_extra_lib=[]
libname=''
for lib in tmplist_of_extra_lib:
    if '"' in libname:
        libname=""
    if ',' in libname:
        libname=""
        
    libname+=lib
    if ".so" in libname:
        list_of_extra_lib.append(libname)
        libname=""
    
if libname:
    if len(list_of_extra_lib) ==0:
        print "Name of library has to contain .so."
   
for lib in list_of_extra_lib:
    print "Adding " + lib + " to list of Libraies to Load"


if DEBUG == "True":
    print "In debug mode"

print "Running : " + cycle

if useskinput == "True": 
    print "Using SKTrees as input."
elif useskinput == "true":
    print "Using SKTrees as input."
else:
    print "Using LQntuples as input"    

########  Sample specific configuration ###############
## set the job conguration set for a specific sample###
#######################################################
sample = sample.replace(":", " ")
datatype=""
splitsample  = sample.split()
if not len(splitsample)==1:
    sample = splitsample[0]
    for conf in range(1,len(splitsample)-1):
        if "nevents" in splitsample[conf]:
            conf+=1
            number_of_events_per_job = splitsample[conf]
        if "remove" in splitsample[conf]:    
            conf+=1
            remove_workspace  = splitsample[conf]
        if "loglevel" in splitsample[conf]:
            conf+=1
            loglevel = splitsample[conf]
        if "cycle"  in splitsample[conf]:
            conf+=1
            cycle = splitsample[conf]
        if "njobs" in splitsample[conf]:
            conf+=1
            number_of_cores = splitsample[conf]
        if "skipevent" in splitsample[conf]:
            conf+=1
            skipev = splitsample[conf]
        if "skinput" in splitsample[conf]:
            conf+=1
            useskinput = splitsample[conf]
        if "runevent" in splitsample[conf]:
            conf+=1
            runevent = splitsample[conf]
        if "use5312ntuples" in splitsample[conf]:
            conf+=1
            use5312ntuples = splitsample[conf]
        if "use538ntuples" in splitsample[conf]:
            conf+=1
            use584ntuples = splitsample[conf]

            
####################
####
####################

if not cycle == "SKTreeMaker":
    if not cycle == "SKTreeMakerNoCut":
        if not cycle == "SKTreeMakerDiLep":
            if not useskinput == "True":
                if not useskinput == "true":
                    update = raw_input("You are running on LQntuples. This will be cpu extensive. This is only advisable if you are testing some new branches NOT in SKTrees. Will change settings to run on SKTrees: Type 'N' if you wish to stick to LQntuples.")
                    if not  update == "N":
                        useskinput="True"

##########################################################
### Make tmp directory for job
############################################################

if not os.path.exists("job_output"):
    os.system("mkdir job_output/")

local_sub_dir= "job_output/" + sample + '_' + channel + '_' + now()
    
if not os.path.exists(local_sub_dir):
    os.system("mkdir " + local_sub_dir)
        
##################################################################################################################
#### HARD CODE THE MAXIMUM number of subjobs
##################################################################################################################

import platform
if platform.system() == "Linux":
    os.system("top -n 1 -b | grep 'root.exe' &> " + local_sub_dir + "/toplog")
    filename = local_sub_dir +'/toplog'

    n_previous_jobs=0
    for line in open(filename, 'r'):
        n_previous_jobs+=1

    if n_previous_jobs > 40:
        number_of_cores = 1
        print "Number of subjobs is reduced to 1, since there are over 40 subjobs running on this machine."

        for line in open(filename, 'r'):
            print line

    os.system("rm " + filename)

if number_of_cores > 1:
    if useskinput == "True":
        if (40 - n_previous_jobs) < number_of_cores:
            number_of_cores = 40 - n_previous_jobs
        if number_of_cores > 30:
            number_of_cores = 30
            print "Number of sub jobs is set to high. Reset to default of 30."
    elif useskinput == "true":
        if (40 - n_previous_jobs) < number_of_cores:
            number_of_cores = 40 - n_previous_jobs
        if number_of_cores > 30:
            number_of_cores= 30
            print "Number of sub jobs is set to high. Reset to default of 30."
    else:
        if number_of_cores > 5:
            if not cycle == "SKTreeMaker":
                if not cycle == "SKTreeMakerNoCut":
                    if not cycle == "SKTreeMakerDiLep":
                        number_of_cores = 5
                        print "Number of sub jobs is set to high. Reset to default of 5."

if number_of_cores < 0:
    number_of_cores=1
##################################################################################################################            
##### FINISHED CONFIGURATION
##################################################################################################################
singlejob = number_of_cores==1            

#### determine if input is data/mc
mc = len(sample)>1
if mc:
    datatype="mc"
else:
    datatype="data"

if sample == "AtoD":
    datatype="data"

if datatype == "mc":
    timeWait=10

if not dataType =="":
    datatype=dataType

##################################################################################################################
### Make a list of input samples: at the moment this is useless. Will add code to include * options in input
##################################################################################################################
list = []
import re
if ("*" in sample) and mc:
    print "ADD code"
else:
    list.append(sample)
#list has only size ==1 currently

##################################################################################################################
##### Specify if the job is running on SKTrees or LQNtuples
##################################################################################################################
original_sample = sample
if useskinput == "true":
    if not mc:
        if useskim == "Lepton":
            channel="SK" + channel
        else:
            if useskim == "NoCut":
                channel="SK" + channel + "_nocut"
            else:
                if useskim == "DiLep":
                    channel="SK" + channel + "_dilep"

    else:
        if useskim == "Lepton":
            sample="SK" + sample
        else:
            if useskim == "NoCut":
                sample="SK" + sample + "_nocut"
            else:
                if useskim == "DiLep":
                    sample="SK" + sample + "_dilep"
elif useskinput == "True":

    if not mc:
        if useskim == "Lepton":
            channel="SK" + channel
        else:
            if useskim == "NoCut":
                channel="SK" + channel + "_nocut"
            else:
                if useskim == "DiLep":
                    channel="SK" + channel + "_dilep"
    else:
        if useskim == "Lepton":
            sample="SK" + sample
        else:
            if useskim == "NoCut":
                sample="SK" + sample + "_nocut"
            else:
                if useskim == "DiLep":
                    sample="SK" + sample + "_dilep"
                
print "Input sample = " + sample
if not mc:
    print "Input channel = " + channel

##############################################################################################
#### Check if sktrees are located on current machines  (not used when running on cmsX at snu)                        
#############################################################################################

isfile = os.path.isfile
join = os.path.join
if platform.system() != "Linux":

    localDir = os.getenv("LQANALYZER_DIR")+ "/data/input/" 
    if not mc:        
        localDir = os.getenv("LQANALYZER_DIR")+ "/data/input/data/" + channel  + sample
    else:
        localDir = os.getenv("LQANALYZER_DIR")+ "/data/input/mc/"  + sample
    
    if not os.path.exists(localDir):
        print "No files in current location: Will copy them over"
        CopySKTrees(channel,sample,mc,"True")
    elif  sum(1 for item in os.listdir(localDir) if isfile(join(localDir, item))) == 0:
        print "No files are located locally: Will copy from cms21 machine"
        CopySKTrees(channel,sample,mc,"True")
    else:
        update = raw_input("Files already located on current machine. Do you want these updating? Yes/No")
        if update == "Yes":
            print "Updating local sktree"
            CopySKTrees(channel,sample,mc,"True")
        elif update == "yes":
            print "Updating local sktree"
            CopySKTrees(channel,sample,mc,"True")
        else:
            CheckPathInFile(channel,sample,mc)
            
##################################################################################################################
#Find the DS name (and lumi if MC) from txt/datasets.txt
##################################################################################################################
inDS = ""
mcLumi = 1.0
filechannel=""

if platform.system() == "Linux":
    version="_5_3_14"
    if  use5312ntuples == "True":
        version = "_5_3_12"
        print "Using Version 5312 ntuples"
    elif  use538ntuples == "True":
        version = "_5_3_8"
        print "Using Version 538 ntuples"
    else:
        print "Using Version 5314 ntuples"
    filename = os.getenv("LQANALYZER_RUN_PATH") + '/txt/datasets_snu' + version +  '.txt'
    
else:
    filename = os.getenv("LQANALYZER_RUN_PATH") + 'txt/datasets_mac.txt'

if not mc:
    for line in open(filename, 'r'):
        if not line.startswith("#"):
            entries = line.split()
            if len(entries)==3:
                if channel ==entries[0] and sample == entries[1]:
                    inDS = entries[2]
    sample = "period"+sample
    eff_lumi=1.
    tar_lumi=1.
    filechannel = channel+"_"
else:
    for line in open(filename, 'r'):
        if not line.startswith("#"):
            entries = line.split()
            if len(entries)==3:
                if sample == entries[0]:
                    eff_lumi = entries[1]
                    inDS = entries[2]
InputDir = inDS    

##################################################################################################################
print "Input directory= " + inDS    ## now have defined what dur contains input files
##################################################################################################################                    

############################################################
############################################################
###### RUN JOB
############################################################
############################################################

os.system("ls " + InputDir + "/*.root > " + local_sub_dir + "/inputlist.txt")

############################################################
## Get number of files in Input directory
############################################################
isfile = os.path.isfile
join = os.path.join
number_of_files = sum(1 for item in os.listdir(InputDir) if isfile(join(InputDir, item)))

if DEBUG == "True":
    print "Job has " + str(number_of_files) + " files to process:"

############################################################
### Correct user if ncores is > nfiles
############################################################
if number_of_cores > number_of_files:
    number_of_cores = number_of_files
print "Splitting job into " + str(number_of_cores) + " subjobs"
############################################################
### set number of files per job
############################################################
nfilesperjobs= 0
for i in range(1,number_of_files):
    if not i%number_of_cores:
        nfilesperjobs+=1

if number_of_cores == 1:
    nfilesperjobs = number_of_files

if nfilesperjobs == 0:
    nfilesperjobs=1
    
files_torun = (nfilesperjobs*number_of_cores)
remainder = number_of_files - (nfilesperjobs*number_of_cores)

if DEBUG == "True":
    print "Each job will process  " + str(nfilesperjobs) + "/" + str(nfilesperjobs+1) + " files"


###################################################
## counters
###################################################
nfiles=0
count=1
total_nsamples=0
filesprocessed=0
nfiles_file=0
n_remainder_files=0
check_array = []

###################################################
# Setup work area on var tmp
###################################################

tmpwork = "/data1/"+ getpass.getuser()
if not (os.path.exists(tmpwork)):
    os.system("mkdir " + tmpwork)
    
workspace = "/data1/"+ getpass.getuser() + "/LQ_SKTreeOutput/"
if not (os.path.exists(workspace)):
        os.system("mkdir " + workspace)
out_end=sample
output=workspace + sample + "_" + now() + "/"
outputdir= output+ "output/"
outputdir_tmp= output+ "output_tmp/"
if not (os.path.exists(output)):
    os.system("mkdir " + output)
    print "Making tmp working directory to run Job  : " + output

if(os.path.exists(outputdir)):
    number_of_outputfiles = sum(1 for item in os.listdir(outputdir) if isfile(join(outputdir, item)))
    if  not number_of_outputfiles ==0:
       os.system("rm " + outputdir + "/*.root")
       print "Emptying output directory as this should be empty for new job"
              
if not (os.path.exists(outputdir)):
    os.system("mkdir " + outputdir)
    os.system("mkdir " + outputdir_tmp)

###################################################
## Make subjob directories
###################################################
printedworkdir =  output + "Job_[" + str(1) + "-" + str(number_of_cores) + "]/"
for i in range(1,number_of_cores+1):
    workdir =  output + "Job_" + str(i) + "/"    
    if not (os.path.exists(workdir)):
            os.system("mkdir " + workdir)
            if i==1:
                if DEBUG == "True":
                    print "making sub work directories " + printedworkdir


####################################################
## Creat separate input lists/macros for each subjob
####################################################

### read inputlist.txt which contains all input files
fr = open(local_sub_dir + '/inputlist.txt', 'r')

outsamplename = sample
if runnp == "True":
    outsamplename = "nonprompt_" + outsamplename
    print "sample --> " + outsamplename
if runcf == "True":
    outsamplename = "chargeflip_" + outsamplename
    print "sample --> " + outsamplename
if not mc:
    outsamplename = outsamplename +  "_" + channel
    if use538ntuples == "True":
        outsamplename = outsamplename + "_5_3_8"
    elif use5312ntuples == "True":
        outsamplename = outsamplename + "_5_3_12"
    else:
        outsamplename = outsamplename + "_5_3_14"
else:
    if use538ntuples == "True":
        outsamplename = outsamplename + "_5_3_8"
    elif use5312ntuples == "True":
        outsamplename = outsamplename + "_5_3_12"
    else:
        outsamplename = outsamplename + "_5_3_14"
        
### specify the location of the macro for the subjob     
printedrunscript = output+ "Job_[1-" + str(number_of_cores)  + "]/runJob_[1-" + str(number_of_cores)  + "].C"

for line in fr:
    if nfiles < files_torun:
        if nfiles == 0 :
            runscript = output+ "Job_" + str(count) + "/runJob_" + str(count) + ".C"
            filelist = output+ "Job_" + str(count) + "/" + sample + "_%s" % (count) + ".txt"
            fwrite = open(filelist, 'w')
            configfile=open(runscript,'w')
            configfile.write(makeConfigFile(loglevel, outsamplename, filelist, tree, cycle, count, outputdir_tmp, outputdir, number_of_events_per_job, logstep, skipev, datatype, channel, data_lumi, totalev, xsec, tar_lumi, eff_lumi, useskinput, runevent, list_of_extra_lib, runnp,runcf)) #job, input, sample, ver, output
            configfile.close()
            if DEBUG == "True":
                print "Making file : " + printedrunscript
            fwrite.write(line)
            filesprocessed+=1
            nfiles_file+=1            
            nfiles+=1
            if files_torun == 1:
                fwrite.close()
            continue

        #End of file
        if not nfiles % nfilesperjobs:
            if not nfiles == number_of_files :
                # set counters
                nfiles_file=0
                count+=1        
                # close files
                fwrite.close()
                ### Make next set of scripts
                runscript = output+ "Job_" + str(count) + "/runJob_" + str(count) + ".C"
                filelist = output+ "Job_" + str(count) + "/" + sample + "_%s" % (count) + ".txt"
                fwrite = open(filelist, 'w')
                configfile=open(runscript,'w')
                configfile.write(makeConfigFile(loglevel,outsamplename, filelist, tree, cycle, count, outputdir_tmp,outputdir, number_of_events_per_job, logstep, skipev, datatype , channel, data_lumi, totalev, xsec, tar_lumi, eff_lumi, useskinput, runevent,list_of_extra_lib, runnp, runcf))
                configfile.close()
                fwrite.write(line)
                filesprocessed+=1
                nfiles_file+=1
            else:
                fwrite.write(line)
                filesprocessed+=1
                nfiles_file+=1
                if DEBUG == "True":
                    print "File " + filelist + " contains " + str(nfiles_file) + " files"            
                
        else:
            fwrite.write(line)
            filesprocessed+=1
            nfiles_file+=1

        if nfiles == number_of_files-1 :
            fwrite.close()

    else:
        n_remainder_files+=1
        filelist = output+ "Job_" + str(n_remainder_files) + "/" + sample + "_%s" % (n_remainder_files) + ".txt"
        #runscript = output+ "Job_" + str(count) + "/runJob_" + str(count) + ".C"
        fwrite = open(filelist, 'a')
        fwrite.write(line)
        #configfile=open(runscript,'w')
        #configfile.write(makeConfigFile(loglevel,sample, filelist, tree, cycle, count, outputdir_tmp,outputdir, number_of_events_per_job, logstep, skipev, datatype , channel, data_lumi, totalev, xsec, tar_lumi, eff_lumi, useskinput, runevent,list_of_extra_lib, runnp, runcf))
        #configfile.close()
        filesprocessed+=1
        fwrite.close()        
    nfiles+=1        
fr.close()

#################################################################### 
### Check Final input files have no duplicates
#################################################################### 
no_duplicate=False
for check in range(1, number_of_cores+1):
    filelist = output+ "Job_" + str(check) + "/" + sample + "_%s" % (check) + ".txt"
    fcheck = open(filelist, 'r')
    nsamples=0
    for line in fcheck:
        nsamples+=1
        total_nsamples+=1
        no_duplicate= True
        for s in check_array:
            if s == line :
                print "DUPLICATE file : " + s
                no_duplicate=False
                sys.exit()
        check_array.append(line)
    if DEBUG == "True":
        print  "File " + filelist + " contains " + str(nsamples) + " files"
    fcheck.close()
if DEBUG == "True":
    print "Total Number of input files = " + str(total_nsamples)     

if no_duplicate:
    #print "Checking for duplicates: "
    #print "Checking for duplicates:...... "
    if DEBUG == "True":
        print "Checking for duplicates: NONE found"
else:
     #print "Checking for duplicates: "
     #print "Checking for duplicates:...... "
     print "Checking for duplicates: Duplicate files found. Check script "
            
if DEBUG == "True":
    print "Total number of files processed = " + str(filesprocessed)

###################################################
### Run each .C file in background
###################################################
import thread,time
start_time = time.time()

wait_sub = 1
if number_of_cores < 10:
    wait_sub = 5

if DEBUG == "True":
    print "Running LQAnalyzer jobs for: " + getpass.getuser()
for i in range(1,number_of_cores+1):
    script = output+ "Job_" + str(i) + "/runJob_" + str(i) + ".C"
    log = output+ "Job_" + str(i) + "/runJob_" + str(i) +".log"
    runcommand = "nohup root.exe -l -q -b " +  script + "&>" + log + "&"
    if singlejob:
        print "Running single job " + script 
        runcommand = "root.exe -l -q -b " +  script 
        os.system(runcommand)
    else:
        if i==1:
            print "Running " + script + " . Log file --->  " + log 
        elif i== number_of_cores:
            print "Running " + script + " . Log file --->  " + log
        elif i==2:
            print "......"
        os.system(runcommand)

##########################################################
## wait and do merging (also remove old log file/rootfiles
##########################################################
check_log= os.getenv("LQANALYZER_LOG_PATH") + "/" + outsamplename + "/runJob_1.log"
if (os.path.exists(check_log)):
    os.system("rm " + os.getenv("LQANALYZER_LOG_PATH") + "/" + outsamplename + "/*.log")

if DEBUG == "True":
    print "Waiting for all jobs to finish before Merging."

ncomplete_files=0
JobSuccess=False
JobOutput=True
CompletedJobs=[]
doMerge=False

print "Checking Job status:"

if singlejob:    
    check_outfile = outputdir + outsamplename +  "_1.root"
    if (os.path.exists(check_outfile)):
        JobSuccess=True;
else:        
    sys.stdout.write('\r'+ '0% of events processed...' )
    sys.stdout.flush()
low_cpu=0
ncycle=0
file_iterator=0
files_done= []
clear_line='                                                                          '
while not JobSuccess:
    os.system("ps ux &> " + local_sub_dir + "/log")
    filename = local_sub_dir +'/log'
    running = False
    
    for line in open(filename, 'r'):
        if "root.exe" in line:
            running = True            
            splitline  = line.split()
            if splitline[2] < 0.1:
                low_cpu+=1
    if low_cpu > 3:
        running = False

    if not running:
        check_outfile = outputdir + outsamplename +  "_1.root"
        if not (os.path.exists(check_outfile)):
            JobSuccess=True
            JobOutput=False
            
    os.system("rm  " + local_sub_dir + "/log")
            
    for i in range(1,number_of_cores+1):
        skipcheck=False
        for check in CompletedJobs:
            if i== check: skipcheck=True
        while not skipcheck:
            skipcheck=True
            check_outfile = outputdir + outsamplename +  "_" +  str(i) + ".root"   
            if (os.path.exists(check_outfile)):
                CompletedJobs.append(i)
                ncomplete_files+=1
                files_done.append("Job [" + str(i) + "] completed. Output ="  + check_outfile)
            
    if ncomplete_files== number_of_cores :
        sys.stdout.write('\r' + clear_line)
        sys.stdout.flush()
        sys.stdout.write('\r'+ '100% of events processed. \n' )
        sys.stdout.flush()
        print "Job finished"
        doMerge=True
        if ncycle == 0:
            print "Job ran in less than 10 seconds. Assumed bug:"
            if number_of_cores == 1:
                JobOutput=True
            else:
                JobOutput=False
        JobSuccess=True
                                    
    else:
        nevents_total=0.
        nevent_processed=0.
        for i in range(1,number_of_cores+1):
            check_outfile = output + "/Job" +  "_" +  str(i) + "/runJob_"+ str(i) +".log"
            os.system('tail -100 ' + check_outfile + ' > ' + local_sub_dir + '/outlog.txt')
            nevent_processed_i=0.
            nevents_total_i=0.
            for line in open(local_sub_dir + '/outlog.txt', 'r'):
                if "Processing entry" in line:
                    if "LQCycleController" not in line:
                        entries = line.split()
                        if len(entries)> 6:                        
                            num = entries[7]
                            s = num.replace("/", " ")
                            event_split = s.split()
                            nevent_processed_i = float(event_split[0])
                            nevents_total_i= float(event_split[1])
            nevent_processed+=nevent_processed_i                
            nevents_total+=nevents_total_i

        ## calculate time left to run jobs    
        end_time = time.time()
        remaining = (end_time - start_time)
        if nevent_processed !=0:
            remaining*= (1./nevent_processed)
            remaining*= ( nevents_total  - nevent_processed)
            b = str(round(float(100.*nevent_processed/nevents_total), 2)) + "% of events processed. Estimated cpu time remaining =  " + str(round(remaining,2)) + "[s]" 
            sys.stdout.write('\r' + clear_line)
            sys.stdout.flush()
            sys.stdout.write('\r'+b)            
            sys.stdout.flush()
        else:
            mess="Job Initialising 0% of events processed."
            sys.stdout.write('\r' + clear_line)
            sys.stdout.flush()
            sys.stdout.write('\r'+mess)
            sys.stdout.flush()
            time.sleep(2.)
        if ncomplete_files > file_iterator:
            #print str(ncomplete_files) + "/" + str(number_of_cores) + " jobs completed.  " #Wait " + str(timeWait) + " second..."
            #print ""
            file_iterator=ncomplete_files
        time.sleep(5.)
        ncycle+=1
     
if not JobOutput:
    print "Job Failed...."
    if not os.path.exists(os.getenv("LQANALYZER_LOG_PATH")):
        os.system("mkdir " + os.getenv("LQANALYZER_LOG_PATH"))
    if not os.path.exists(os.getenv("LQANALYZER_LOG_PATH")+ "/" + outsamplename):
        os.system("mkdir " + os.getenv("LQANALYZER_LOG_PATH")+ "/" + outsamplename)
        
    if not number_of_cores == 1:
        os.system("mv "+ output + "/*/*.log " + os.getenv("LQANALYZER_LOG_PATH") + "/" + outsamplename)
        os.system("mv "+ output + "/Job_1/runJob_1.C .")
    print "Check ./runJob_1.C or " + os.getenv("LQANALYZER_LOG_PATH") + "/" + outsamplename   +"/runJob_1.log file to debug"
    os.system("rm -r " + output)    
    os.system("rm -r " + local_sub_dir)    
    

    print "log files sent to " + os.getenv("LQANALYZER_LOG_PATH") + "/" + outsamplename
    
else:

    for line in files_done:
        if DEBUG == "True":
            print line


    SKTreeOutput = "/data1/LocalNtuples/Tag27_CMSSW_5_3_14/SKTrees/June14v2/"        
    #do not merge the output when using tree maker code
    if cycle == "SKTreeMaker":
        if not os.path.exists(SKTreeOutput):
            os.system("mkdir  " + SKTreeOutput)
        doMerge=False
        if not mc:
            Finaloutputdir = SKTreeOutput + "Data/"
            if not os.path.exists(Finaloutputdir):
                os.system("mkdir " + Finaloutputdir)
                
            if original_channel =="egamma":
                Finaloutputdir += "DoubleElectron/"
                if not os.path.exists(Finaloutputdir):
                    os.system("mkdir " + Finaloutputdir)
            if original_channel =="muon":
                Finaloutputdir += "DoubleMuon/"
                if not os.path.exists(Finaloutputdir):
                    os.system("mkdir " + Finaloutputdir)
            if original_channel =="emu":
                Finaloutputdir += "ElectronMuon/"
                if not os.path.exists(Finaloutputdir):
                    os.system("mkdir " + Finaloutputdir)
            if original_channel =="singlemuon":
                Finaloutputdir += "SingleMuon/"
                if not os.path.exists(Finaloutputdir):
                    os.system("mkdir " + Finaloutputdir)
            if original_channel =="singleelectron":
                Finaloutputdir += "SingleElectron/"
                if not os.path.exists(Finaloutputdir):
                    os.system("mkdir " + Finaloutputdir)
            Finaloutputdir += "period" + original_sample + "/"
            if not os.path.exists(Finaloutputdir):
                os.system("mkdir " + Finaloutputdir)
        else:
            Finaloutputdir = SKTreeOutput + "MC/"
            if not os.path.exists(Finaloutputdir):
                os.system("mkdir " + Finaloutputdir)
                                                                                            
            Finaloutputdir +=  original_sample + "/"
            if not os.path.exists(Finaloutputdir):
                os.system("mkdir " + Finaloutputdir)
                
    if cycle == "SKTreeMakerNoCut":
        doMerge=False
        if not os.path.exists(SKTreeOutput):
            os.system("mkdir " + SKTreeOutput)
        if not mc:
            Finaloutputdir = SKTreeOutput + "DataNoCut/"
            if not os.path.exists(Finaloutputdir):
                os.system("mkdir " + Finaloutputdir)
            if original_channel =="egamma":
                Finaloutputdir += "DoubleElectron/"
                if not os.path.exists(Finaloutputdir):
                    os.system("mkdir " + Finaloutputdir)
            if original_channel =="muon":
                Finaloutputdir += "DoubleMuon/"
                if not os.path.exists(Finaloutputdir):
                    os.system("mkdir " + Finaloutputdir)
            if original_channel =="emu":
                Finaloutputdir += "ElectronMuon/"
            if not os.path.exists(Finaloutputdir):
                os.system("mkdir " + Finaloutputdir)
            Finaloutputdir += "period" + original_sample + "/"
            if not os.path.exists(Finaloutputdir):
                os.system("mkdir " + Finaloutputdir)
        else:
            Finaloutputdir = SKTreeOutput + "MCNoCut/"
            if not os.path.exists(Finaloutputdir):
                os.system("mkdir " + Finaloutputdir)
            Finaloutputdir += original_sample + "/"
            if not os.path.exists(Finaloutputdir):
                os.system("mkdir " + Finaloutputdir)
    if cycle == "SKTreeMakerDiLep":
        doMerge=False
        if not os.path.exists(SKTreeOutput):
            os.system("mkdir " + SKTreeOutput)
        if not mc:
            Finaloutputdir = SKTreeOutput + "DataDiLep1510/"
            if not os.path.exists(Finaloutputdir):
                os.system("mkdir " + Finaloutputdir)
            if original_channel =="egamma":
                Finaloutputdir += "DoubleElectron/"
                if not os.path.exists(Finaloutputdir):
                    os.system("mkdir " + Finaloutputdir)
            if original_channel =="muon":
                Finaloutputdir += "DoubleMuon/"
                if not os.path.exists(Finaloutputdir):
                    os.system("mkdir " + Finaloutputdir)
            if original_channel =="emu":
                Finaloutputdir += "ElectronMuon/"
                if not os.path.exists(Finaloutputdir):
                    os.system("mkdir " + Finaloutputdir)
            Finaloutputdir += "period" + original_sample + "/"
            if not os.path.exists(Finaloutputdir):
                os.system("mkdir " + Finaloutputdir)
        else:
            Finaloutputdir = SKTreeOutput + "MCDiLep1510/"
            if not os.path.exists(Finaloutputdir):
                os.system("mkdir " + Finaloutputdir)
            Finaloutputdir +=  original_sample + "/"
            if not os.path.exists(Finaloutputdir):
                os.system("mkdir " + Finaloutputdir)
                
    if not os.path.exists(Finaloutputdir):
        os.system("mkdir " + Finaloutputdir)
    outfile = cycle + "_" + filechannel + outsamplename + ".root"
    if doMerge:
        if not mc:
            outfile = cycle + "_" + outsamplename + ".root"
        if os.path.exists(Finaloutputdir + outfile):
            os.system("rm  "  +  Finaloutputdir   + outfile)
        os.system("hadd " + Finaloutputdir +  outfile  + " "+ outputdir + "*.root")
        print "Merged output :" + Finaloutputdir + outfile
    else:
        if not mc:
            outfile = cycle + "_" + outsamplename + ".root"
        if number_of_cores == 1:
            os.system("mv " + outputdir + outsamplename + "_1.root " + Finaloutputdir + outfile )
        else:
            os.system("mv " + outputdir + "*.root " + Finaloutputdir )
        if DEBUG == "True":
            print "Non merged output :" +Finaloutputdir

    if remove_workspace == "True":
        if not os.path.exists(os.getenv("LQANALYZER_LOG_PATH")):
            os.system("mkdir " + os.getenv("LQANALYZER_LOG_PATH"))
            
        if not os.path.exists(os.getenv("LQANALYZER_LOG_PATH")+ "/" + outsamplename):
            os.system("mkdir " + os.getenv("LQANALYZER_LOG_PATH")+ "/" + outsamplename)
        
        if not number_of_cores == 1:    
            os.system("mv "+ output + "/*/*.log " + os.getenv("LQANALYZER_LOG_PATH") + "/" + outsamplename)
        os.system("rm -r " + output)
        os.system("rm -r " + local_sub_dir)
        print "Log files are sent to  --> "  + os.getenv("LQANALYZER_LOG_PATH")+ "/" + outsamplename
        if doMerge:
            print "All sampless finished: OutFile:"  + cycle + "_" + filechannel + outsamplename + ".root -->" + Finaloutputdir
        else:
            if number_of_cores == 1:
                print "All sampless finished: OutFiles "+ outsamplename + "_1.root -->" + Finaloutputdir + outfile
            else:
                print "All sampless finished: OutFiles "+ outsamplename + "*.root -->" + Finaloutputdir
            
    else:
        print "TMP directory " + output + "is not removed. "
        os.system("rm -r " + local_sub_dir)
        
if os.path.exists(local_sub_dir):
    os.system("rm -r " + local_sub_dir)

end_time = time.time()
total_time=end_time- start_time
print "Using " + str(number_of_cores) + " cores: Job time = " + str(total_time) +  " s"
print ""
