import os, sys
import commands as cmd
import argparse
from datetime import datetime

def set_input(sampleName, fileList, nTotFiles, nJobs):
  # -- split the input files -- #
  logPath = pwd+"/logs/"+timestamp+"/"+sampleName
  os.system('mkdir -p '+logPath)
  os.system("cp -n run_calculateEff_dist.sh "+logPath)
  os.system("cp -n calculateEff_dist.C "+logPath)
  nTotFilesPerJobs = int(nTotFiles/nJobs) # q for aq+b
  Remainder = nTotFiles - (nJobs)*(nTotFilesPerJobs) # b = nTotFiles - aq
  if Remainder >= nJobs:
    print "Remainder : "+str(Remainder)+" >= Divisor : "+str(nJobs)+"; Exit."
    sys.exit()

  Ranges = []
  EndOfHalfRanges = 0
  checksum = 0
  for b in range(0, Remainder): # Firstly, run loop (q+1) for b times; aq+b = b(q+1) + (a-b)q.
    Ranges.append(range( b*(nTotFilesPerJobs+1) , (b+1)*(nTotFilesPerJobs+1) ))
    EndOfHalfRanges = (b+1)*(nTotFilesPerJobs+1)
    checksum += len(range( b*(nTotFilesPerJobs+1) , (b+1)*(nTotFilesPerJobs+1) ))

  for aMb in range(0, nJobs-Remainder): # Secondly, run loop q for a-b times.
    Ranges.append(range( EndOfHalfRanges + aMb*(nTotFilesPerJobs) , EndOfHalfRanges + (aMb+1)*(nTotFilesPerJobs) ))
    checksum += len(range( EndOfHalfRanges + aMb*(nTotFilesPerJobs) , EndOfHalfRanges + (aMb+1)*(nTotFilesPerJobs) ))

  if not checksum == nTotFiles:
    print "checksum : "+str(checksum)+" != nTotFiles : "+str(nTotFiles)+"; Exit."
    sys.exit()

  for it_job in range(0, len(Ranges)):
    out = open(logPath+'/input_'+str(it_job)+'.txt', 'w')
    for it_file in Ranges[it_job]:
      out.write(fileList[it_file]+'\n')
    out.close()

  for nIter in range(0, nJobs):
    os.chdir(logPath)
    os.system('./run_calculateEff_dist.sh input_'+str(nIter)+'.txt '+sampleName+'_'+str(nIter)+'.root '+sampleName+' '+str(nIter))
    #os.system('pwd')
    #os.system('echo ./run_calculateEff_dist.sh input_'+str(nIter)+'.txt '+sampleName+'_'+str(nIter)+'.root '+sampleName+' '+str(nIter)) #TEST

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

if __name__ == "__main__":

  today = datetime.today()
  timestamp = today.strftime("%Y%m%d")+"__"+today.strftime("%H%M%S")
  
  parser = argparse.ArgumentParser()
  parser.add_argument('-n', '--nJobs', type=int, default=10, help='the number of jobs to run')
  args = parser.parse_args()
  
  nJobs = args.nJobs # a for nTotFiles = aq+b

  pwd = os.getcwd()
  
  #campaigns = ["Winter20","Winter21"]
  campaigns = ["Winter20_113X"]
  #campaigns = ["Winter21_subset"]
  #processes = ["DY","TT","Jpsi","QCD","Zprime6000"]
  processes = ["DY"]
  #WPs = ["WP00","WP02","WP04","WP10","N50","N10","N5","N0"]
  WPs = ["WP00"]
  
  samples = {'Run3_Winter21_Zprime6000_WP00' : "/data9/Users/wonjun/public/crab_Zprime_M6000_113X_hlt_muon_Run3_mc_20210703/210703_041027/0000/*.root",
             'Run3_Winter21_DY_WP00' : "/data9/Users/wonjun/public/crab_DYToLL_M50_113X_hlt_muon_Run3_mc_20210703/210703_050244/*/*.root",
             'Run3_Winter21_subset_DY_WP00' : "/data9/Users/wonjun/public/crab_DYToLL_M50_113X_hlt_muon_Run3_mc_20210703/210703_050244/0001/*.root",
             'Run3_Winter20_DY_WP00' : "/data9/Users/wonjun/public/crab_DYToLL_M50_110X_hlt_muon_Run3_mc_wp00_20210513/210513_155853/0000/*.root",
             'Run3_Winter20_113X_DY_WP00' : "/data9/Users/wonjun/public/crab_DYToLL_M50_Winter20_113X_hlt_muon_Run3_mc_20210714/210714_122920/0000/*.root",
             #'Run3_Winter20_113X_DY_WP00' : "/data9/Users/wonjun/public/crab_DYToLL_M50_Winter20_113X_hlt_muon_Run3_mc_20210714/210714_122920/0000/ntuple_1.root",
  }
  
  for campaign in campaigns:
    for process in processes:
      for WP in WPs:
        sampleName = "Run3_"+campaign+"_"+process+"_"+WP
        fileList = cmd.getoutput("ls "+samples[sampleName]).split('\n')
        nTotFiles = len(fileList)
        if nJobs > nTotFiles or nJobs == 0:
          nJobs = nTotFiles
        set_input(sampleName, fileList, nTotFiles, nJobs)
