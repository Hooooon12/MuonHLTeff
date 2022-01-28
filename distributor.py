#############################################################################################
#
# python distributor.py -n 100
#
#############################################################################################

import os, sys
import commands as cmd
import argparse
from datetime import datetime

def set_input(sampleName, fileList, nTotFiles, nJobs, IsKNU):
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
      if IsKNU:
        out.write("dcap://cluster142.knu.ac.kr/"+fileList[it_file]+'\n') #JH : use dcap when using condor in KNU. see http://t2-cms.knu.ac.kr/wiki/index.php/Dcap
      else:
        out.write(fileList[it_file]+'\n')
    out.close()

  for nIter in range(0, nJobs):
    os.chdir(logPath)
    os.system('./run_calculateEff_dist.sh input_'+str(nIter)+'.txt '+sampleName+'_'+str(nIter)+'.root '+sampleName+' '+str(nIter))
    #os.system('pwd')
    #os.system('echo ./run_calculateEff_dist.sh input_'+str(nIter)+'.txt '+sampleName+'_'+str(nIter)+'.root '+sampleName+' '+str(nIter)) #TEST

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

if __name__ == "__main__":

  HOSTNAME = os.environ['HOSTNAME']
  IsKNU = ("knu" in HOSTNAME)

  today = datetime.today()
  timestamp = today.strftime("%Y%m%d")+"__"+today.strftime("%H%M%S")
  
  parser = argparse.ArgumentParser()
  parser.add_argument('-n', '--nJobs', type=int, default=10, help='the number of jobs to run')
  args = parser.parse_args()
  
  pwd = os.getcwd()
  
  #campaigns = ["Winter20","Winter21"]
  campaigns = ["Summer21"]
  #processes = ["DY","TT","Jpsi","QCD","Zprime6000"]
  #processes = ["DY","TT","Jpsi","Bs","DY4"]
  #processes = ["DY","Jpsi","Wprime","Zprime"]
  processes = ["subset_Jpsi","subset_Wprime","subset_Zprime"]
  #WPs = ["WP00","WP02","WP04","WP10","N50","N10","N5","N0"]
  #WPs = ["Run2","Baseline","Upgrade","Upgrade_L1METTkMuTri","Upgrade_ROI1p2","Upgrade_ROI1p5"]
  WPs = ["Run2","Baseline","Upgrade_L1METTkMuTri","Upgrade_ROI1p2","Upgrade_ROI1p5"]
  #WPs = ["Upgrade"]
  #WPs = ["Baseline"]
 
  #samples = {
  #           'Run3_Winter21_Zprime6000_WP00' : "/data9/Users/wonjun/public/crab_Zprime_M6000_113X_hlt_muon_Run3_mc_20210703/210703_041027/0000/*.root",
  #           'Run3_Winter21_subset_DY_WP00' : "/data9/Users/wonjun/public/crab_DYToLL_M50_113X_hlt_muon_Run3_mc_wp00_20210924/210924_055042/0000/ntuple_39.root",
  #           'Run3_Winter21_DY_WP00' : "/data9/Users/wonjun/public/crab_DYToLL_M50_113X_hlt_muon_Run3_mc_wp00_20210924/*/*/*.root",
  #           'Run3_Winter21_DY_WP02' : "/data9/Users/wonjun/public/crab_DYToLL_M50_113X_hlt_muon_Run3_mc_wp02_20210830/*/*/*.root",
  #           'Run3_Winter21_DY_WP04' : "/data9/Users/wonjun/public/crab_DYToLL_M50_113X_hlt_muon_Run3_mc_wp04_20210830/*/*/*.root",
  #           'Run3_Winter21_DY_WP10' : "/data9/Users/wonjun/public/crab_DYToLL_M50_113X_hlt_muon_Run3_mc_wp10_20210830/*/*/*.root",
  #           'Run3_Winter21_TT_WP00' : "/data9/Users/wonjun/public/crab_TTbar_113X_hlt_muon_Run3_mc_wp00_NoDY_20210906/*/*/*.root",
  #           'Run3_Winter21_TT_WP02' : "/data9/Users/wonjun/public/crab_TTbar_113X_hlt_muon_Run3_mc_wp02_NoDY_20210906/*/*/*.root",
  #           'Run3_Winter21_TT_WP04' : "/data9/Users/wonjun/public/crab_TTbar_113X_hlt_muon_Run3_mc_wp04_NoDY_20210906/*/*/*.root",
  #           'Run3_Winter21_TT_WP10' : "/data9/Users/wonjun/public/crab_TTbar_113X_hlt_muon_Run3_mc_wp10_NoDY_20210906/*/*/*.root",
  #           'Run3_Winter21_DY4_WP00' : "/data9/Users/wonjun/public/crab_DYToLL_M4_113X_hlt_muon_Run3_mc_wp00_NoDY_20210924/*/*/*.root",
  #           'Run3_Winter21_DY4_WP02' : "/data9/Users/wonjun/public/crab_DYToLL_M4_113X_hlt_muon_Run3_mc_wp02_NoDY_20210830/*/*/*.root",
  #           'Run3_Winter21_DY4_WP04' : "/data9/Users/wonjun/public/crab_DYToLL_M4_113X_hlt_muon_Run3_mc_wp04_NoDY_20210830/*/*/*.root",
  #           'Run3_Winter21_DY4_WP10' : "/data9/Users/wonjun/public/crab_DYToLL_M4_113X_hlt_muon_Run3_mc_wp10_NoDY_20210830/*/*/*.root",
  #           'Run3_Winter21_Jpsi_WP00' : "/data9/Users/wonjun/public/crab_Jpsi_113X_hlt_muon_Run3_mc_wp00_NoDY_20210924/*/*/*.root",
  #           'Run3_Winter21_Jpsi_WP02' : "/data9/Users/wonjun/public/crab_Jpsi_113X_hlt_muon_Run3_mc_wp02_NoDY_20210830/*/*/*.root",
  #           'Run3_Winter21_Jpsi_WP04' : "/data9/Users/wonjun/public/crab_Jpsi_113X_hlt_muon_Run3_mc_wp04_NoDY_20210830/*/*/*.root",
  #           'Run3_Winter21_Jpsi_WP10' : "/data9/Users/wonjun/public/crab_Jpsi_113X_hlt_muon_Run3_mc_wp10_NoDY_20210830/*/*/*.root",
  #           'Run3_Winter21_Bs_WP00' : "/data9/Users/wonjun/public/crab_BsMuMu_113X_hlt_muon_Run3_mc_wp00_NoDY_20210924/*/*/*.root",
  #           'Run3_Winter21_Bs_WP02' : "/data9/Users/wonjun/public/crab_BsMuMu_113X_hlt_muon_Run3_mc_wp02_NoDY_20210830/*/*/*.root",
  #           'Run3_Winter21_Bs_WP04' : "/data9/Users/wonjun/public/crab_BsMuMu_113X_hlt_muon_Run3_mc_wp04_NoDY_20210830/*/*/*.root",
  #           'Run3_Winter21_Bs_WP10' : "/data9/Users/wonjun/public/crab_BsMuMu_113X_hlt_muon_Run3_mc_wp10_NoDY_20210830/*/*/*.root",
  #           'Run3_Winter21_Gun_WP00' : "/data9/Users/wonjun/public/crab_MuGunPU_113X_hlt_muon_Run3_mc_wp00_NoDY_20210910/*/*/*.root",
  #           'Run3_Winter21_Gun_WP02' : "/data9/Users/wonjun/public/crab_MuGunPU_113X_hlt_muon_Run3_mc_wp02_NoDY_20210910/*/*/*.root",
  #           'Run3_Winter21_Gun_WP04' : "/data9/Users/wonjun/public/crab_MuGunPU_113X_hlt_muon_Run3_mc_wp04_NoDY_20210910/*/*/*.root",
  #           'Run3_Winter21_Gun_WP10' : "/data9/Users/wonjun/public/crab_MuGunPU_113X_hlt_muon_Run3_mc_wp10_NoDY_20210910/*/*/*.root",
  #           'Run3_Winter21_Zprime_WP00' : "/data9/Users/wonjun/public/crab_Zprime_M6000_113X_hlt_muon_Run3_mc_wp00_NoDY_20210923/*/*/*.root",
  #           'Run3_Winter21_Zprime_WP02' : "/data9/Users/wonjun/public/crab_Zprime_M6000_113X_hlt_muon_Run3_mc_wp02_NoDY_20210923/*/*/*.root",
  #           'Run3_Winter21_Zprime_WP04' : "/data9/Users/wonjun/public/crab_Zprime_M6000_113X_hlt_muon_Run3_mc_wp04_NoDY_20210923/*/*/*.root",
  #           'Run3_Winter21_Zprime_WP10' : "/data9/Users/wonjun/public/crab_Zprime_M6000_113X_hlt_muon_Run3_mc_wp10_NoDY_20210923/*/*/*.root",
  #           #'Run3_Winter21_subset_DY4_WP00' : "/data9/Users/wonjun/public/crab_DYToLL_M4_113X_hlt_muon_Run3_mc_wp00_NoDY_20210805/*/*/*.root",
  #           #'Run3_Winter21_subset_Jpsi_WP00' : "/data9/Users/wonjun/public/crab_Jpsi_113X_hlt_muon_Run3_mc_wp00_NoDY_20210805/*/0000/*.root",
  #           #'Run3_Winter21_subset_Bs_WP00' : "/data9/Users/wonjun/public/crab_BsMuMu_113X_hlt_muon_Run3_mc_wp00_NoDY_20210805/*/*/*.root",
  #           #'Run3_Winter21_subset_Bs_WP00' : "/data9/Users/wonjun/public/crab_BsMuMu_113X_hlt_muon_Run3_mc_wp00_NoDY_20210802/210802_112820/0000/ntuple_9.root",
  #           'Run3_Winter20_DY_WP00' : "/data9/Users/wonjun/public/crab_DYToLL_M50_110X_hlt_muon_Run3_mc_wp00_20210513/210513_155853/0000/*.root",
  #           'Run3_Winter20_113X_DY_WP00' : "/data9/Users/wonjun/public/crab_DYToLL_M50_Winter20_113X_hlt_muon_Run3_mc_20210714/210714_122920/0000/*.root",
  #          } #JH : keep this for record purposes

  samples = {
             'Run3_Summer21_subset_DY_Run2'                     : "/pnfs/knu.ac.kr/data/cms/store/user/wjun/MuonHLTRun3_cmssw1220/20220124/DYToLL_M-50_TuneCP5_14TeV-pythia8/*run2*/*/*/*11*.root",
             'Run3_Summer21_subset_DY_Baseline'                 : "/pnfs/knu.ac.kr/data/cms/store/user/wjun/MuonHLTRun3_cmssw1220/20220124/DYToLL_M-50_TuneCP5_14TeV-pythia8/*baseline*/*/*/*11*.root",
             'Run3_Summer21_subset_DY_Upgrade'                  : "/pnfs/knu.ac.kr/data/cms/store/user/wjun/MuonHLTRun3_cmssw1220/20220124/DYToLL_M-50_TuneCP5_14TeV-pythia8/*upgrade_20220124/*/*/*11*.root",
             'Run3_Summer21_subset_DY_Upgrade_L1METTkMuTri'     : "/pnfs/knu.ac.kr/data/cms/store/user/wjun/MuonHLTRun3_cmssw1220/20220124/DYToLL_M-50_TuneCP5_14TeV-pythia8/*L1METTkMuTri*/*/*/*11*.root",
             'Run3_Summer21_subset_DY_Upgrade_ROI1p2'           : "/pnfs/knu.ac.kr/data/cms/store/user/wjun/MuonHLTRun3_cmssw1220/20220124/DYToLL_M-50_TuneCP5_14TeV-pythia8/*ROI1p2*/*/*/*11*.root",
             'Run3_Summer21_subset_DY_Upgrade_ROI1p5'           : "/pnfs/knu.ac.kr/data/cms/store/user/wjun/MuonHLTRun3_cmssw1220/20220124/DYToLL_M-50_TuneCP5_14TeV-pythia8/*ROI1p5*/*/*/*11*.root",
             'Run3_Summer21_subset_Jpsi_Run2'                   : "/pnfs/knu.ac.kr/data/cms/store/user/wjun/MuonHLTRun3_cmssw1220/20220124/JPsiToMuMu_Pt-0To100-pythia8-gun/*run2*/*/*/*11*.root",
             'Run3_Summer21_subset_Jpsi_Baseline'               : "/pnfs/knu.ac.kr/data/cms/store/user/wjun/MuonHLTRun3_cmssw1220/20220124/JPsiToMuMu_Pt-0To100-pythia8-gun/*baseline*/*/*/*11*.root",
             'Run3_Summer21_subset_Jpsi_Upgrade'                : "/pnfs/knu.ac.kr/data/cms/store/user/wjun/MuonHLTRun3_cmssw1220/20220124/JPsiToMuMu_Pt-0To100-pythia8-gun/*upgrade_20220124/*/*/*11*.root",
             'Run3_Summer21_subset_Jpsi_Upgrade_L1METTkMuTri'   : "/pnfs/knu.ac.kr/data/cms/store/user/wjun/MuonHLTRun3_cmssw1220/20220124/JPsiToMuMu_Pt-0To100-pythia8-gun/*L1METTkMuTri*/*/*/*11*.root",
             'Run3_Summer21_subset_Jpsi_Upgrade_ROI1p2'         : "/pnfs/knu.ac.kr/data/cms/store/user/wjun/MuonHLTRun3_cmssw1220/20220124/JPsiToMuMu_Pt-0To100-pythia8-gun/*ROI1p2*/*/*/*11*.root",
             'Run3_Summer21_subset_Jpsi_Upgrade_ROI1p5'         : "/pnfs/knu.ac.kr/data/cms/store/user/wjun/MuonHLTRun3_cmssw1220/20220124/JPsiToMuMu_Pt-0To100-pythia8-gun/*ROI1p5*/*/*/*11*.root",
             'Run3_Summer21_subset_Wprime_Run2'                 : "/pnfs/knu.ac.kr/data/cms/store/user/wjun/MuonHLTRun3_cmssw1220/20220124/WprimeToMuNu_M-5000_TuneCP5_14TeV-pythia8/*run2*/*/*/*11*.root",
             'Run3_Summer21_subset_Wprime_Baseline'             : "/pnfs/knu.ac.kr/data/cms/store/user/wjun/MuonHLTRun3_cmssw1220/20220124/WprimeToMuNu_M-5000_TuneCP5_14TeV-pythia8/*baseline*/*/*/*11*.root",
             'Run3_Summer21_subset_Wprime_Upgrade'              : "/pnfs/knu.ac.kr/data/cms/store/user/wjun/MuonHLTRun3_cmssw1220/20220124/WprimeToMuNu_M-5000_TuneCP5_14TeV-pythia8/*upgrade_20220124/*/*/*11*.root",
             'Run3_Summer21_subset_Wprime_Upgrade_L1METTkMuTri' : "/pnfs/knu.ac.kr/data/cms/store/user/wjun/MuonHLTRun3_cmssw1220/20220124/WprimeToMuNu_M-5000_TuneCP5_14TeV-pythia8/*L1METTkMuTri*/*/*/*11*.root",
             'Run3_Summer21_subset_Wprime_Upgrade_ROI1p2'       : "/pnfs/knu.ac.kr/data/cms/store/user/wjun/MuonHLTRun3_cmssw1220/20220124/WprimeToMuNu_M-5000_TuneCP5_14TeV-pythia8/*ROI1p2*/*/*/*11*.root",
             'Run3_Summer21_subset_Wprime_Upgrade_ROI1p5'       : "/pnfs/knu.ac.kr/data/cms/store/user/wjun/MuonHLTRun3_cmssw1220/20220124/WprimeToMuNu_M-5000_TuneCP5_14TeV-pythia8/*ROI1p5*/*/*/*11*.root",
             'Run3_Summer21_subset_Zprime_Run2'                 : "/pnfs/knu.ac.kr/data/cms/store/user/wjun/MuonHLTRun3_cmssw1220/20220124/ZprimeToMuMu_M-6000_TuneCP5_14TeV-pythia8/*run2*/*/*/*11*.root",
             'Run3_Summer21_subset_Zprime_Baseline'             : "/pnfs/knu.ac.kr/data/cms/store/user/wjun/MuonHLTRun3_cmssw1220/20220124/ZprimeToMuMu_M-6000_TuneCP5_14TeV-pythia8/*baseline*/*/*/*11*.root",
             'Run3_Summer21_subset_Zprime_Upgrade'              : "/pnfs/knu.ac.kr/data/cms/store/user/wjun/MuonHLTRun3_cmssw1220/20220124/ZprimeToMuMu_M-6000_TuneCP5_14TeV-pythia8/*upgrade_20220124/*/*/*11*.root",
             'Run3_Summer21_subset_Zprime_Upgrade_L1METTkMuTri' : "/pnfs/knu.ac.kr/data/cms/store/user/wjun/MuonHLTRun3_cmssw1220/20220124/ZprimeToMuMu_M-6000_TuneCP5_14TeV-pythia8/*L1METTkMuTri*/*/*/*11*.root",
             'Run3_Summer21_subset_Zprime_Upgrade_ROI1p2'       : "/pnfs/knu.ac.kr/data/cms/store/user/wjun/MuonHLTRun3_cmssw1220/20220124/ZprimeToMuMu_M-6000_TuneCP5_14TeV-pythia8/*ROI1p2*/*/*/*11*.root",
             'Run3_Summer21_subset_Zprime_Upgrade_ROI1p5'       : "/pnfs/knu.ac.kr/data/cms/store/user/wjun/MuonHLTRun3_cmssw1220/20220124/ZprimeToMuMu_M-6000_TuneCP5_14TeV-pythia8/*ROI1p5*/*/*/*11*.root",
             'Run3_Summer21_DY_Run2'                     : "/pnfs/knu.ac.kr/data/cms/store/user/wjun/MuonHLTRun3_cmssw1220/20220124/DYToLL_M-50_TuneCP5_14TeV-pythia8/*run2*/*/*/*.root",
             'Run3_Summer21_DY_Baseline'                 : "/pnfs/knu.ac.kr/data/cms/store/user/wjun/MuonHLTRun3_cmssw1220/20220124/DYToLL_M-50_TuneCP5_14TeV-pythia8/*baseline*/*/*/*.root",
             'Run3_Summer21_DY_Upgrade'                  : "/pnfs/knu.ac.kr/data/cms/store/user/wjun/MuonHLTRun3_cmssw1220/20220124/DYToLL_M-50_TuneCP5_14TeV-pythia8/*upgrade_20220124/*/*/*.root",
             'Run3_Summer21_DY_Upgrade_L1METTkMuTri'     : "/pnfs/knu.ac.kr/data/cms/store/user/wjun/MuonHLTRun3_cmssw1220/20220124/DYToLL_M-50_TuneCP5_14TeV-pythia8/*L1METTkMuTri*/*/*/*.root",
             'Run3_Summer21_DY_Upgrade_ROI1p2'           : "/pnfs/knu.ac.kr/data/cms/store/user/wjun/MuonHLTRun3_cmssw1220/20220124/DYToLL_M-50_TuneCP5_14TeV-pythia8/*ROI1p2*/*/*/*.root",
             'Run3_Summer21_DY_Upgrade_ROI1p5'           : "/pnfs/knu.ac.kr/data/cms/store/user/wjun/MuonHLTRun3_cmssw1220/20220124/DYToLL_M-50_TuneCP5_14TeV-pythia8/*ROI1p5*/*/*/*.root",
             'Run3_Summer21_Jpsi_Run2'                   : "/pnfs/knu.ac.kr/data/cms/store/user/wjun/MuonHLTRun3_cmssw1220/20220124/JPsiToMuMu_Pt-0To100-pythia8-gun/*run2*/*/*/*.root",
             'Run3_Summer21_Jpsi_Baseline'               : "/pnfs/knu.ac.kr/data/cms/store/user/wjun/MuonHLTRun3_cmssw1220/20220124/JPsiToMuMu_Pt-0To100-pythia8-gun/*baseline*/*/*/*.root",
             'Run3_Summer21_Jpsi_Upgrade'                : "/pnfs/knu.ac.kr/data/cms/store/user/wjun/MuonHLTRun3_cmssw1220/20220124/JPsiToMuMu_Pt-0To100-pythia8-gun/*upgrade_20220124/*/*/*.root",
             'Run3_Summer21_Jpsi_Upgrade_L1METTkMuTri'   : "/pnfs/knu.ac.kr/data/cms/store/user/wjun/MuonHLTRun3_cmssw1220/20220124/JPsiToMuMu_Pt-0To100-pythia8-gun/*L1METTkMuTri*/*/*/*.root",
             'Run3_Summer21_Jpsi_Upgrade_ROI1p2'         : "/pnfs/knu.ac.kr/data/cms/store/user/wjun/MuonHLTRun3_cmssw1220/20220124/JPsiToMuMu_Pt-0To100-pythia8-gun/*ROI1p2*/*/*/*.root",
             'Run3_Summer21_Jpsi_Upgrade_ROI1p5'         : "/pnfs/knu.ac.kr/data/cms/store/user/wjun/MuonHLTRun3_cmssw1220/20220124/JPsiToMuMu_Pt-0To100-pythia8-gun/*ROI1p5*/*/*/*.root",
             'Run3_Summer21_Wprime_Run2'                 : "/pnfs/knu.ac.kr/data/cms/store/user/wjun/MuonHLTRun3_cmssw1220/20220124/WprimeToMuNu_M-5000_TuneCP5_14TeV-pythia8/*run2*/*/*/*.root",
             'Run3_Summer21_Wprime_Baseline'             : "/pnfs/knu.ac.kr/data/cms/store/user/wjun/MuonHLTRun3_cmssw1220/20220124/WprimeToMuNu_M-5000_TuneCP5_14TeV-pythia8/*baseline*/*/*/*.root",
             'Run3_Summer21_Wprime_Upgrade'              : "/pnfs/knu.ac.kr/data/cms/store/user/wjun/MuonHLTRun3_cmssw1220/20220124/WprimeToMuNu_M-5000_TuneCP5_14TeV-pythia8/*upgrade_20220124/*/*/*.root",
             'Run3_Summer21_Wprime_Upgrade_L1METTkMuTri' : "/pnfs/knu.ac.kr/data/cms/store/user/wjun/MuonHLTRun3_cmssw1220/20220124/WprimeToMuNu_M-5000_TuneCP5_14TeV-pythia8/*L1METTkMuTri*/*/*/*.root",
             'Run3_Summer21_Wprime_Upgrade_ROI1p2'       : "/pnfs/knu.ac.kr/data/cms/store/user/wjun/MuonHLTRun3_cmssw1220/20220124/WprimeToMuNu_M-5000_TuneCP5_14TeV-pythia8/*ROI1p2*/*/*/*.root",
             'Run3_Summer21_Wprime_Upgrade_ROI1p5'       : "/pnfs/knu.ac.kr/data/cms/store/user/wjun/MuonHLTRun3_cmssw1220/20220124/WprimeToMuNu_M-5000_TuneCP5_14TeV-pythia8/*ROI1p5*/*/*/*.root",
             'Run3_Summer21_Zprime_Run2'                 : "/pnfs/knu.ac.kr/data/cms/store/user/wjun/MuonHLTRun3_cmssw1220/20220124/ZprimeToMuMu_M-6000_TuneCP5_14TeV-pythia8/*run2*/*/*/*.root",
             'Run3_Summer21_Zprime_Baseline'             : "/pnfs/knu.ac.kr/data/cms/store/user/wjun/MuonHLTRun3_cmssw1220/20220124/ZprimeToMuMu_M-6000_TuneCP5_14TeV-pythia8/*baseline*/*/*/*.root",
             'Run3_Summer21_Zprime_Upgrade'              : "/pnfs/knu.ac.kr/data/cms/store/user/wjun/MuonHLTRun3_cmssw1220/20220124/ZprimeToMuMu_M-6000_TuneCP5_14TeV-pythia8/*upgrade_20220124/*/*/*.root",
             'Run3_Summer21_Zprime_Upgrade_L1METTkMuTri' : "/pnfs/knu.ac.kr/data/cms/store/user/wjun/MuonHLTRun3_cmssw1220/20220124/ZprimeToMuMu_M-6000_TuneCP5_14TeV-pythia8/*L1METTkMuTri*/*/*/*.root",
             'Run3_Summer21_Zprime_Upgrade_ROI1p2'       : "/pnfs/knu.ac.kr/data/cms/store/user/wjun/MuonHLTRun3_cmssw1220/20220124/ZprimeToMuMu_M-6000_TuneCP5_14TeV-pythia8/*ROI1p2*/*/*/*.root",
             'Run3_Summer21_Zprime_Upgrade_ROI1p5'       : "/pnfs/knu.ac.kr/data/cms/store/user/wjun/MuonHLTRun3_cmssw1220/20220124/ZprimeToMuMu_M-6000_TuneCP5_14TeV-pythia8/*ROI1p5*/*/*/*.root",
            }
  
  for campaign in campaigns:
    for process in processes:
      for WP in WPs:
        nJobs = args.nJobs # a for nTotFiles = aq+b
        sampleName = "Run3_"+campaign+"_"+process+"_"+WP
        fileList = cmd.getoutput("ls "+samples[sampleName]).split('\n')
        nTotFiles = len(fileList)
        if nJobs > nTotFiles or nJobs == 0:
          nJobs = nTotFiles
        set_input(sampleName, fileList, nTotFiles, nJobs, IsKNU)
