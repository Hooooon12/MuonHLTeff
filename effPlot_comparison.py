import os, sys, array
from ROOT import *

gROOT.SetBatch(kTRUE)

file_dict = {
             "DY" : {
                     #'Winter20_113X' : TFile.Open("/data6/Users/jihkim/MuonHLT/logs/20210719__180358/Run3_Winter20_113X_DY_WP00/Run3_Winter20_113X_DY_WP00.root"),
                     #'Winter21' : TFile.Open("/data6/Users/jihkim/MuonHLT/logs/20210719__180358/Run3_Winter21_subset_DY_WP00/Run3_Winter21_subset_DY_WP00.root"),
                     'Winter21' : TFile.Open("/data6/Users/jihkim/MuonHLT/logs/20210806__023659/Run3_Winter21_DY_WP00/Run3_Winter21_DY_WP00.root"),
                    },
             "DY4" : {
                      'Winter21' : TFile.Open("/data6/Users/jihkim/MuonHLT/logs/20210806__023659/Run3_Winter21_DY4_WP00/Run3_Winter21_DY4_WP00.root")
                     },
             "TTbar" : {
                       },
             "Jpsi" : {
                       'Winter21' : TFile.Open("/data6/Users/jihkim/MuonHLT/logs/20210806__023659/Run3_Winter21_Jpsi_WP00/Run3_Winter21_Jpsi_WP00.root")
                      },
             "Bs" : {
                     #'Winter21' : TFile.Open("/data6/Users/jihkim/MuonHLT/logs/20210806__023659/Run3_Winter21_Bs_WP00/Run3_Winter21_Bs_WP00.root")
                     'Winter21' : TFile.Open("/data6/Users/jihkim/MuonHLT/logs/20210812__034800/Run3_Winter21_subset_Bs_WP00/Run3_Winter21_subset_Bs_WP00.root")
                    },
            }

bin_dict = {
            "pt" : array.array('d', [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 15, 20, 25, 30, 40, 50, 60, 120, 200]),
            "eta" : array.array('d', [-2.4, -2.1, -1.6, -1.2, -0.9, -0.3, -0.2, 0.2, 0.3, 0.9, 1.2, 1.6, 2.1, 2.4]),
            "phi" : array.array('d', [-3.15, -2.85, -2.55, -2.25, -1.95, -1.65, -1.35, -1.05, -0.75, -0.45, -0.15, 0.15, 0.45, 0.75, 1.05, 1.35, 1.65, 1.95, 2.25, 2.55, 2.85, 3.15]),
            "truePU" : array.array('d', [30,32,34,36,38,40,42,44,46,48,50,52,54,56,58,60,62,64,66,68,70,72,74,76,78,80]),
           }

Xrange = {
          "DY" : {
                  "pt" : [20, 200],
                  "eta" : [-2.4, 2.4],
                  "phi" : [-3.15, 3.15],
                  "truePU" : [30, 80]
                 },
          "DY4" : {
                   "pt" : [0, 60],
                   "eta" : [-2.4, 2.4],
                   "phi" : [-3.15, 3.15],
                   "truePU" : [30, 80]
                  },
          "Jpsi" : {
                    "pt" : [0, 200],
                    "eta" : [-2.4, 2.4],
                    "phi" : [-3.15, 3.15],
                    "truePU" : [30, 80]
                   },
          "Bs" : {
                  "pt" : [0, 200],
                  "eta" : [-2.4, 2.4],
                  "phi" : [-3.15, 3.15],
                  "truePU" : [30, 80]
                 },
         }

Xtitle = {
          "pt" : "p_{T}(#mu) [GeV]",
          "eta" : "#eta(#mu)",
          "phi" : "#varphi(#mu)",
          "truePU" : "# of PU",
         }

color_list = { 1:[kRed], 2:[kRed, kBlue], 3:[kRed, kOrange, kBlue], 4:[kRed, kOrange, kGreen+1, kBlue] }

lineStyle_dict = {"Winter21" : 1, "Winter20_113X" : 2}

# -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

# This returns three lists which contain num, den hist and legend; nums should be in the suffix dictionary. (can be added)
# "gen" in num is for purity plots.
# den must be "gen" or "gen_L1".
def GetHistList(num, den, L1_den, var, turn_on):
  suffix = {
            "L3NoID" : ["L3NoID", "TPtoL3NoIDTrack"],
            "IOFromL1" : ["IOFromL1", "TPtoIOFromL1"],
            "OI" : ["OI", "TPtoOI"],
            "L1Raw0" : ["L1Raw0", "L1Raw0_AtVtx", "L1Matched_Raw0", "L1Matched_l1drByQ_Raw0"],
            "L1DQ0" : ["L1DQ0", "L1DQ0_AtVtx", "L1Matched_DQ0", "L1Matched_l1drByQ_DQ0"],
            "L1SQ0" : ["L1SQ0", "L1SQ0_AtVtx", "L1Matched_SQ0", "L1Matched_l1drByQ_SQ0"],
            "L1Raw8" : ["L1Raw8", "L1Raw8_AtVtx", "L1Matched_Raw8", "L1Matched_l1drByQ_Raw8"],
            "L1DQ8" : ["L1DQ8", "L1DQ8_AtVtx", "L1Matched_DQ8", "L1Matched_l1drByQ_DQ8"],
            "L1SQ8" : ["L1SQ8", "L1SQ8_AtVtx", "L1Matched_SQ8", "L1Matched_l1drByQ_SQ8"],
            "L1Raw22" : ["L1Raw22", "L1Raw22_AtVtx", "L1Matched_Raw22", "L1Matched_l1drByQ_Raw22"],
            "L1DQ22" : ["L1DQ22", "L1DQ22_AtVtx", "L1Matched_DQ22", "L1Matched_l1drByQ_DQ22"],
            "L1SQ22" : ["L1SQ22", "L1SQ22_AtVtx", "L1Matched_SQ22", "L1Matched_l1drByQ_SQ22"],
            "gen" : ["_purity", "_Asso"], # for purity
           }

  legend = {
            "L3NoID" : ["L3 muon before ID dR matching", "L3 muon before ID hit association"],
            "IOFromL1" : ["IO from L1 dR matching", "IO from L1 hit association"],
            "OI" : ["OI dR matching", "OI hit association"],
            "L1Raw0" : ["L1Raw0 position dR matching", "L1Raw0 position_at_vtx dR matching", "L1 Matcher matched L1Raw0 dR matching", "L1 Matcher matched L1Raw0"],
            "L1DQ0" : ["L1DQ0 position dR matching", "L1DQ0 position_at_vtx dR matching", "L1 Matcher matched L1DQ0 dR matching", "L1 Matcher matched L1DQ0"],
            "L1SQ0" : ["L1SQ0 position dR matching", "L1SQ0 position_at_vtx dR matching", "L1 Matcher matched L1SQ0 dR matching", "L1 Matcher matched L1SQ0"],
            "L1Raw8" : ["L1Raw8 position dR matching", "L1Raw8 position_at_vtx dR matching", "L1 Matcher matched L1Raw8 dR matching", "L1 Matcher matched L1Raw8"],
            "L1DQ8" : ["L1DQ8 position dR matching", "L1DQ8 position_at_vtx dR matching", "L1 Matcher matched L1DQ8 dR matching", "L1 Matcher matched L1DQ8"],
            "L1SQ8" : ["L1SQ8 position dR matching", "L1SQ8 position_at_vtx dR matching", "L1 Matcher matched L1SQ8 dR matching", "L1 Matcher matched L1SQ8"],
            "L1Raw22" : ["L1Raw22 position dR matching", "L1Raw22 position_at_vtx dR matching", "L1 Matcher matched L1Raw22 dR matching", "L1 Matcher matched L1Raw22"],
            "L1DQ22" : ["L1DQ22 position dR matching", "L1DQ22 position_at_vtx dR matching", "L1 Matcher matched L1DQ22 dR matching", "L1 Matcher matched L1DQ22"],
            "L1SQ22" : ["L1SQ22 position dR matching", "L1SQ22 position_at_vtx dR matching", "L1 Matcher matched L1SQ22 dR matching", "L1 Matcher matched L1SQ22"],
            "gen" : [den+"Track"+" dR matching", den+"Track"+" hit association"], # for purity
           }

  isL1Gen = ""
  isMed = ""
  if "gen" in den: # efficiency
    measure = "_eff_"

    if "L1" in den: # "gen_L1" in den
      if not L1_den in ["L1SQ22", "L1DQ8"]:
        print "gen dR matched with", L1_den, "is not supported.. sorry!"
        print "Exiting..."
        sys.exit()

      isL1Gen += "_L1"
      L1_den = "_"+L1_den

    if var != "pt":
      if turn_on == "double":
        isMed += "_med"

    if "L1" in num and not "From" in num: # L1 efficiency
      prefix = ["hardP"+isMed+"_"+var, "hardP"+isMed+"_"+var, "hardP"+isMed+"_"+var, "hardP"+isMed+"_"+var]
    else: # other efficiencies
      prefix = ["hardP"+isL1Gen+isMed+"_"+var, "TP"+L1_den+isMed+"_"+var]

  else: # purity
    measure = ""
    prefix = [den+"Track"+"_"+var, den+"Track"+"_"+var]

  numList = []
  for i in range(len(prefix)):
    numList+=[prefix[i]+measure+suffix[num][i]]

  denList = []
  for i in range(len(prefix)):
    denList+=[prefix[i]]

  lgdList = []
  for i in range(len(prefix)):
    lgdList+=[legend[num][i]]

  return numList, denList, lgdList

# -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

def GetYtitle(num, den, L1_den, var, turn_on):
  if "gen" in den:
    suffix = "efficiency"
    if "L1" in den:
      prefix = "L3/L1"
    else:
      if "L1" in num and not "From" in num:
        prefix = "L1"
      else:
        prefix = "L3"
  else:
    prefix = "L3"
    suffix = "purity"

  return prefix+" "+suffix

# -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

def GetL1qual(num, den, L1_den, var, turn_on):

  if ("L1" in num and not "From" in num) or ("L1" in L1_den):
    isValid = True
  else:
    isValid = False

  if "Raw" in num or "Raw" in L1_den:
    prefix = "no "
    suffix = " cut"
  elif "DQ" in num or "DQ" in L1_den:
    prefix = ""
    suffix = " > 7"
  elif "SQ" in num or "SQ" in L1_den:
    prefix = ""
    suffix = " > 11"

  if isValid:
    return prefix+"L1 qual"+suffix
  else:
    return ""
 
# -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

def GetL1pt(num, den, L1_den, var, turn_on):

  if ("L1" in num and not "From" in num) or ("L1" in L1_den):
    isValid = True
  else:
    isValid = False

  if "0" in num or "0" in L1_den:
    prefix = "no "
    suffix = " cut"
  elif "8" in num or "8" in L1_den:
    prefix = ""
    suffix = " > 8 GeV"
  elif "22" in num or "22" in L1_den:
    prefix = ""
    suffix = " > 22 GeV"

  if isValid:
    return prefix+"p_{T}^{L1}"+suffix
  else:
    return ""

# -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

def GetGenPt(num, den, L1_den, var, turn_on):

  if num != "gen":
    if var in ["eta", "phi", "truePU"]:
      isValid = True
    elif var == "pt":
      isValid = False
  else:
    isValid = False

  if isValid:
    if turn_on == "single":
      return "p_{T}^{gen} > 26 GeV"
    elif turn_on == "double":
      return "p_{T}^{gen} > 10 GeV"
  else:
    return ""

# -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

class plot: # each instance of this class corresponds to one canvas
  def __init__(self, num, den, L1_den, var, turn_on):
    self.nums, self.dens, self.lgds = GetHistList(num, den, L1_den, var, turn_on)
    self.Xtitle = Xtitle[var]
    self.Ytitle = GetYtitle(num, den, L1_den, var, turn_on)
    self.L1qual = GetL1qual(num, den, L1_den, var, turn_on)
    self.L1pt = GetL1pt(num, den, L1_den, var, turn_on)
    self.genPt = GetGenPt(num, den, L1_den, var, turn_on)
    self.var = var
    if "TP" in self.nums[-1]:
      self.name = self.nums[-1].replace("TPto","").replace("TP","hardP") # TP_L1SQ22_eta_eff_TPtoIOFromL1 --> hardP_L1SQ22_eta_eff_IOFromL1
    elif "l1drByQ" in self.nums[-1]:
      self.name = self.nums[-1].replace("Matched_l1drByQ_","") # hardP_med_eta_eff_L1Matched_l1drByQ_DQ0 --> hardP_med_eta_eff_L1DQ0
    elif "Asso" in self.nums[-1]:
      self.name = self.nums[-1].replace("Asso","purity") # IOFromL1Track_eta_Asso --> IOFromL1Track_eta_purity

#####How it works#####
#c1 = plot("L3NoID","gen_L1","L1SQ22","eta","single")
#
#c1.Yaxis = "L3 efficiency"
#c1.L1qual = "L1 qual > 11"
#c1.L1pt = "p_{T}^{L1} > 22 GeV"
#c1.genPt = "p_{T}^{gen} > 26 GeV"
######################

def DrawHist_New(proc, var):

  hist_setup = [
                ["L3NoID","gen_L1","L1SQ22",var,"single"],
                #["IOFromL1","gen","",var,"double"], # I didn't make "double" turn-on (i.e. med) for gen efficiency with dR matching, so this cannot be used.
                ["IOFromL1","gen_L1","L1DQ8",var,"double"],
                ["L1SQ22","gen","",var,"single"],
                #["L1SQ22","gen","",var,"double"],
                ["L1DQ8","gen","",var,"double"],
                ["gen","L3NoID","",var,""],
		  		     ]

  for i in range(len(hist_setup)):
    h1 = plot(hist_setup[i][0],hist_setup[i][1],hist_setup[i][2],hist_setup[i][3],hist_setup[i][4])
    c1 = TCanvas("c1","c1",200,200,900,800)
    c1.cd()
    lg_L1 = TPaveText(0.13,0.82,0.65,0.85,"NDC NB")
    lg_L1.SetShadowColor(0)
    lg_L1.SetFillColor(0)
    lg_L1.SetTextFont(42)
    lg_L1.SetTextAlign(12)
    lg_L1.SetTextSize(0.04)
    lg_gen = TPaveText(0.65,0.82,0.7,0.85,"NDC NB")
    lg_gen.SetShadowColor(0)
    lg_gen.SetFillColor(0)
    lg_gen.SetTextFont(42)
    lg_gen.SetTextAlign(12)
    lg_gen.SetTextSize(0.04)
    lg_gr = TLegend(0.13,0.7,0.85,0.8)
    lg_gr.SetBorderSize(0)
    grs = []
    for j in range(len(h1.nums)):
      for camp in file_dict[proc].keys():
        ############################ Debug ############################
        print "num = "+h1.nums[j]
        print "den = "+h1.dens[j]
        print "SetRangeUser =",Xrange[proc][h1.var][0],Xrange[proc][h1.var][1]
        print "lg_gr.AddEntry = "+h1.lgds[j]+" ("+camp+" "+proc+")"
        ###############################################################
        num = file_dict[proc][camp].Get(h1.nums[j])
        den = file_dict[proc][camp].Get(h1.dens[j])
        #print "file :", file_dict[proc][camp]
        #print "num :", num
        #print "den :", den
        num_rebin = num.Rebin(len(bin_dict[h1.var])-1,'a',bin_dict[h1.var])
        den_rebin = den.Rebin(len(bin_dict[h1.var])-1,'b',bin_dict[h1.var])
        ############################ Debug ############################
        #print "num.GetBinContent(40)", num.GetBinContent(40)
        #print "den.GetBinContent(40)", den.GetBinContent(40)
        #print "num_rebin.GetBinContent(6)", num_rebin.GetBinContent(6)
        #print "den_rebin.GetBinContent(6)", den_rebin.GetBinContent(6)
        ###############################################################
        gr = TGraphAsymmErrors(len(bin_dict[h1.var])-1)
        gr.Divide(num_rebin,den_rebin)
        gr.SetTitle("")
        gr.GetXaxis().SetTitle(h1.Xtitle)
        gr.GetYaxis().SetTitle(h1.Ytitle)
        gr.SetMarkerStyle(8)
        gr.SetMarkerColor(color_list[len(h1.nums)][j])
        gr.SetLineStyle(lineStyle_dict[camp])
        gr.SetLineColor(color_list[len(h1.nums)][j])
        gr.SetLineWidth(3)
        gr.GetXaxis().SetRangeUser(Xrange[proc][h1.var][0],Xrange[proc][h1.var][1])
        if j == 0:
          #yran1 = TMath.MinElement(len(gr.GetY()),gr.GetY())*0.8
          yran1 = 0.
          yran2 = TMath.MaxElement(len(gr.GetY()),gr.GetY())*1.6
          gr.GetYaxis().SetRangeUser(yran1, yran2)
          gr.Draw("AP")
        else:
          gr.Draw("P same")
        grs = [gr] + grs
        if j == len(h1.nums)-1:
          if h1.L1qual != "" : lg_L1.AddText(h1.L1qual+", "+h1.L1pt)
          if h1.genPt  != "" : lg_gen.AddText(h1.genPt)
        lg_gr.AddEntry(gr,h1.lgds[j]+" ("+camp+" "+proc+")")
    lg_L1.Draw()
    lg_gen.Draw()
    lg_gr.Draw()
    c1.SaveAs("comp/"+proc+"/"+h1.name+".png")


def DrawHist(proc, var): # For each title : multiple { legend : histo }s to draw in the same canvas (in order).
 
#def DrawHist(proc, var, compare):
#TODO : if compare == "matching", this_num(den)_dict should be varieties of histograms in one process(same root file)
#       elif compare == "process", this_num(den)_dict should be processes(files) with the same histogram

  this_num_dict = {
                   "proc" : [
                              {'L3 efficiency' : [
                                                       {'L3 muon before ID dR matching' : "hardP_"+var+"_eff_L3NoID"},
                                                       {'L3 muon before ID hit association' : "TP_"+var+"_eff_TPtoL3NoIDTrack"},
                                                     ]
                              },
                              {'L3/L1 efficiency' : [
                                                       {'L3 muon before ID dR matching' : "hardP_L1_"+var+"_eff_L3NoID"},
                                                       {'L3 muon before ID hit association' : "TP_L1SQ22_"+var+"_eff_TPtoL3NoIDTrack"},
                                                     ]
                              },
                              #{'L3/L1 efficiency' : [
                              #                         {'L3 muon before ID dR matching' : "hardP_L1_med_"+var+"_eff_L3NoID"},
                              #                         {'L3 muon before ID hit association' : "TP_L1DQ8_med_"+var+"_eff_TPtoL3NoIDTrack"},
                              #                       ]
                              #},
                              {'L3 efficiency' : [
                                                         {'IO from L1 dR matching' : "hardP_"+var+"_eff_IOFromL1"},
                                                         {'IO from L1 hit association' : "TP_"+var+"_eff_TPtoIOFromL1"},
                                                       ]
                              },
                              {'L3/L1 efficiency' : [
                                                         {'IO from L1 dR matching' : "hardP_L1_"+var+"_eff_IOFromL1"},
                                                         {'IO from L1 hit association' : "TP_L1SQ22_"+var+"_eff_TPtoIOFromL1"},
                                                       ]
                              },
                              #{'L3/L1 efficiency' : [
                              #                           {'IO from L1 dR matching' : "hardP_L1_med_"+var+"_eff_IOFromL1"},
                              #                           {'IO from L1 hit association' : "TP_L1DQ8_med_"+var+"_eff_TPtoIOFromL1"},
                              #                         ]
                              #},
                              {'L3 efficiency' : [
                                                   {'OI dR matching' : "hardP_"+var+"_eff_OI"},
                                                   {'OI hit association' : "TP_"+var+"_eff_TPtoOI"},
                                                 ]
                              },
                              {'L3/L1 efficiency' : [
                                                   {'OI dR matching' : "hardP_L1_"+var+"_eff_OI"},
                                                   {'OI hit association' : "TP_L1SQ22_"+var+"_eff_TPtoOI"},
                                                 ]
                              },
                              #{'L3/L1 efficiency' : [
                              #                     {'OI dR matching' : "hardP_L1_med_"+var+"_eff_OI"},
                              #                     {'OI hit association' : "TP_L1DQ8_med_"+var+"_eff_TPtoOI"},
                              #                   ]
                              #}, # comment out 'med's when running with pt
                              {'L1 efficiency' : [
                                                   {'L1 position dR matching' : "hardP_"+var+"_eff_L1Raw0"},
                                                   {'L1 position_at_vtx dR matching' : "hardP_"+var+"_eff_L1Raw0_AtVtx"},
                                                   {'L1 Matcher dR' : "hardP_"+var+"_eff_L1Matched_Raw0"},
                                                   {'L1 Matcher dR at MS' : "hardP_"+var+"_eff_L1Matched_l1drByQ_Raw0"},
                                                 ]
                              },
                              {'L1 efficiency' : [
                                                   {'L1 position dR matching' : "hardP_"+var+"_eff_L1SQ0"},
                                                   {'L1 position_at_vtx dR matching' : "hardP_"+var+"_eff_L1SQ0_AtVtx"},
                                                   {'L1 Matcher dR' : "hardP_"+var+"_eff_L1Matched_SQ0"},
                                                   {'L1 Matcher dR at MS' : "hardP_"+var+"_eff_L1Matched_l1drByQ_SQ0"},
                                                 ]
                              },
                              {'L3 purity' : [
                                                   {'dR matching' : "L3NoIDTrack_"+var+"_purity"},
                                                   {'hit association' : "L3NoIDTrack_"+var+"_Asso"},
                                                 ]
                              },
                             ],
                  }
  
  this_den_dict = {
                   "proc" : [
                              {'L3 efficiency' : [
                                                       {'L3/hardP dR matching' : "hardP_"+var},
                                                       {'L3/TP hit association' : "TP_"+var},
                                                     ]
                              },
                              {'L3/L1 eEfficiency' : [
                                                       {'L3/hardP_L1 dR matching' : "hardP_L1_"+var},
                                                       {'L3/TP_L1 hit association' : "TP_L1SQ22_"+var},
                                                     ]
                              },
                              #{'L3/L1 efficiency' : [
                              #                         {'L3/hardP_L1_med dR matching' : "hardP_L1_med_"+var},
                              #                         {'L3/TP_L1_med hit association' : "TP_L1DQ8_med_"+var},
                              #                       ]
                              #},
                              {'L3 efficiency' : [
                                                         {'L3/hardP dR matching' : "hardP_"+var},
                                                         {'L3/TP hit association' : "TP_"+var},
                                                       ]
                              },
                              {'L3/L1 efficiency' : [
                                                         {'L3/hardP_L1 dR matching' : "hardP_L1_"+var},
                                                         {'L3/TP_L1 hit association' : "TP_L1SQ22_"+var},
                                                       ]
                              },
                              #{'L3/L1 efficiency' : [
                              #                           {'L3/hardP_L1_med dR matching' : "hardP_L1_med_"+var},
                              #                           {'L3/TP_L1_med hit association' : "TP_L1DQ8_med_"+var},
                              #                         ]
                              #},
                              {'L3 efficiency' : [
                                                   {'dR matching' : "hardP_"+var},
                                                   {'hit association' : "TP_"+var},
                                                 ]
                              },
                              {'L3/L1 efficiency' : [
                                                   {'dR matching' : "hardP_L1_"+var},
                                                   {'hit association' : "TP_L1SQ22_"+var},
                                                 ]
                              },
                              #{'L3/L1 efficiency' : [
                              #                     {'dR matching' : "hardP_L1_med_"+var},
                              #                     {'hit association' : "TP_L1DQ8_med_"+var},
                              #                   ]
                              #},
                              {'L1 efficiency' : [
                                                   {'L1/hardP dR matching' : "hardP_"+var},
                                                   {'L1_AtVtx/hardP dR matching' : "hardP_"+var},
                                                   {'L1/hardP L1Matcher dR' : "hardP_"+var},
                                                   {'L1/hardP L1Matcher dR_At_MS' : "hardP_"+var},
                                                 ]
                              },
                              {'L1 efficiency' : [
                                                   {'L1/hardP dR matching' : "hardP_"+var},
                                                   {'L1_AtVtx/hardP dR matching' : "hardP_"+var},
                                                   {'L1/hardP L1Matcher dR' : "hardP_"+var},
                                                   {'L1/hardP L1Matcher dR_At_MS' : "hardP_"+var},
                                                 ]
                              },
                              {'L3 purity' : [
                                                   {'dR matching' : "L3NoIDTrack_"+var},
                                                   {'hit association' : "L3NoIDTrack_"+var},
                                                 ]
                              },
                             ],
                  }

  if proc == "DY" or proc == "TTbar":
    num_dict = this_num_dict["proc"]
    den_dict = this_den_dict["proc"]
  elif proc == "DY4" or proc == "Jpsi" or proc == "Bs":
    num_dict = this_num_dict["proc"]
    den_dict = this_den_dict["proc"]

  for i in range(len(num_dict)):
    c1 = TCanvas("c1","c1",200,200,900,800)
    c1.cd()
    #########if you want to add ratio plot also...
    #p1 = TPad("p1","p1",0,0.3,1,1)
    #p1.SetBottomMargin(0.01)
    #p1.Draw()
    #p1.cd()
    ################################
    lg1 = TPaveText(0.13,0.82,0.85,0.85,"NDC NB")
    lg1.SetShadowColor(0)
    lg1.SetFillColor(0)
    lg1.SetTextFont(42)
    lg1.SetTextAlign(12)
    lg1.SetTextSize(0.04)
    #lg2 = TLegend(0.7,0.75,0.9,0.9)
    lg2 = TLegend(0.13,0.7,0.85,0.8)
    lg2.SetBorderSize(0)
    grs = []
    for j in range(len(num_dict[i].values()[0])):
      for camp in file_dict[proc].keys():
        ############################ Debug ############################
        #print "num = "+(num_dict[i].values()[0])[j].values()[0]
        #print "den = "+(den_dict[i].values()[0])[j].values()[0]
        #print "SetTitle = "+num_dict[i].keys()[0]
        #print "SetRangeUser =",Xrange[proc][var][0],Xrange[proc][var][1]
        #print "lg2.AddEntry = "+(num_dict[i].values()[0])[j].keys()[0], camp, proc
        ###############################################################
        #num = file_dict[proc][camp].Get((num_dict[i].values()[0])[j].values()[0])
        #den = file_dict[proc][camp].Get((den_dict[i].values()[0])[j].values()[0])
        num = file_dict[proc][camp].Get(num_dict[i].values()[0][j].values()[0])
        den = file_dict[proc][camp].Get(den_dict[i].values()[0][j].values()[0])
        #print "file :", file_dict[proc][camp]
        #print "num :", num_dict[i].values()[0][j].values()[0], num
        #print "den :", den_dict[i].values()[0][j].values()[0], den
        num_rebin = num.Rebin(len(bin_dict[var])-1,'a',bin_dict[var])
        den_rebin = den.Rebin(len(bin_dict[var])-1,'b',bin_dict[var])
        ############################ Debug ############################
        #print "num.GetBinContent(40)", num.GetBinContent(40)
        #print "den.GetBinContent(40)", den.GetBinContent(40)
        #print "num_rebin.GetBinContent(6)", num_rebin.GetBinContent(6)
        #print "den_rebin.GetBinContent(6)", den_rebin.GetBinContent(6)
        ###############################################################
        gr = TGraphAsymmErrors(len(bin_dict[var])-1)
        gr.Divide(num_rebin,den_rebin)
        gr.SetTitle("")
        gr.GetXaxis().SetTitle(var)
        gr.GetYaxis().SetTitle(num_dict[i].keys()[0])
        gr.SetMarkerStyle(8)
        gr.SetMarkerColor(color_list[len(num_dict[i].values()[0])][j])
        gr.SetLineStyle(lineStyle_dict[camp])
        gr.SetLineColor(color_list[len(num_dict[i].values()[0])][j])
        gr.SetLineWidth(3)
        gr.GetXaxis().SetRangeUser(Xrange[proc][var][0],Xrange[proc][var][1])
        #if 'IOFromL2' in num_dict[i].keys()[0]:
        #  gr.GetYaxis().SetRangeUser(0., 1.)
        #else:
        #  gr.GetYaxis().SetRangeUser(0.5, 1.1)
        if j == 0:
          #yran1 = TMath.MinElement(len(gr.GetY()),gr.GetY())*0.8
          yran1 = 0.
          yran2 = TMath.MaxElement(len(gr.GetY()),gr.GetY())*1.6
          gr.GetYaxis().SetRangeUser(yran1, yran2)
          gr.Draw("AP")
        else:
          gr.Draw("P same")
        grs = [gr] + grs
        if j == len(num_dict[i].values()[0])-1:
          if "SQ0" in num_dict[i].values()[0][j].values()[0]:
            lg1.AddText("L1 qual > 11, no p_{T}^{L1} cut")
          elif "SQ22" in num_dict[i].values()[0][j].values()[0]:
            lg1.AddText("L1 qual > 11, p_{T}^{L1} > 22 GeV")
          elif "DQ8" in num_dict[i].values()[0][j].values()[0]:
            lg1.AddText("L1 qual > 7, p_{T}^{L1} > 8 GeV")
          elif "Raw0" in num_dict[i].values()[0][j].values()[0]:
            lg1.AddText("no L1 qual cut, no p_{T}^{L1} cut")
        lg2.AddEntry(gr,(num_dict[i].values()[0])[j].keys()[0]+" ("+camp+" "+proc+")")
    lg1.Draw()
    lg2.Draw()
    #c1.SaveAs("comp/"+proc+"/"+num_dict[i].keys()[0].replace(' ','_')+"_"+var+".png")
    c1.SaveAs("comp/"+proc+"/"+num_dict[i].values()[0][-1].values()[0]+".png")

def Draw2DHist(proc, var): # For each title : multiple { legend : histo }s to draw in the same canvas (in order).

  os.system("mkdir -p comp/"+this_proc+"/dR")

  this_2D_dict = {
                  "2D" : [
                          {'dR(OI, gen) vs gen'+' '+var : [
                                                           {'' : "hardP_"+var+"_dR_OI_2D"},
                                                          ]
                          },
                          {'dR(Iter0FromL2, gen) vs gen'+' '+var : [
                                                                    {'' : "hardP_"+var+"_dR_Iter0FromL2_2D"},
                                                                   ]
                          },
                          {'dR(Iter0FromL1, gen) vs gen'+' '+var : [
                                                                    {'' : "hardP_"+var+"_dR_Iter0FromL1_2D"},
                                                                   ]
                          },
                          {'dR(Iter2FromL2, gen) vs gen'+' '+var : [
                                                                    {'' : "hardP_"+var+"_dR_Iter2FromL2_2D"},
                                                                   ]
                          },
                          {'dR(Iter2FromL1, gen) vs gen'+' '+var : [
                                                                    {'' : "hardP_"+var+"_dR_Iter2FromL1_2D"},
                                                                   ]
                          },
                          #{'dR(L1, gen) vs gen'+' '+var : [
                          #                                 {'' : "hardP_"+var+"_dR_L1Raw_2D"},
                          #                                ]
                          #},
                          #{'dR(L1_AtVtx, gen) vs gen'+' '+var : [
                          #                                       {'' : "hardP_"+var+"_dR_L1Raw_AtVtx_2D"},
                          #                                      ]
                          #},
                          #{'dR(L1_Matched, gen) vs gen'+' '+var : [
                          #                                         {'' : "hardP_"+var+"_dR_L1Matched_2D"},
                          #                                        ]
                          #},
                          #{'dR(L1_SQ, gen) vs gen'+' '+var : [
                          #                                    {'' : "hardP_"+var+"_dR_L1_2D"},
                          #                                   ]
                          #},
                          #{'dR(L1_SQ_AtVtx, gen) vs gen'+' '+var : [
                          #                                          {'' : "hardP_"+var+"_dR_L1_AtVtx_2D"},
                          #                                         ]
                          #},
                          #{'dR(L1_DQ, gen) vs gen'+' '+var : [
                          #                                    {'' : "hardP_"+var+"_dR_L1_med_2D"},
                          #                                   ]
                          #},
                          #{'dR(L1_DQ_AtVtx, gen) vs gen'+' '+var : [
                          #                                          {'' : "hardP_"+var+"_dR_L1_med_AtVtx_2D"},
                          #                                         ]
                          #},
                          {'dR(L1, gen) vs gen'+' '+var : [
                                                           {'' : "hardP_"+var+"_dR_L1Raw0_2D"},
                                                          ]
                          },
                          {'dR(L1_AtVtx, gen) vs gen'+' '+var : [
                                                                 {'' : "hardP_"+var+"_dR_L1Raw0_AtVtx_2D"},
                                                                ]
                          },
                          {'dR(L1_Matched, gen) vs gen'+' '+var : [
                                                                   {'' : "hardP_"+var+"_dR_L1Matched_l1drByQ_Raw0_2D"},
                                                                  ]
                          },
                          {'dR(L1_SQ, gen) vs gen'+' '+var : [
                                                              {'' : "hardP_"+var+"_dR_L1SQ22_2D"},
                                                             ]
                          },
                          {'dR(L1_SQ_AtVtx, gen) vs gen'+' '+var : [
                                                                    {'' : "hardP_"+var+"_dR_L1SQ22_AtVtx_2D"},
                                                                   ]
                          },
                          {'dR(L1_DQ, gen) vs gen'+' '+var : [
                                                              {'' : "hardP_"+var+"_dR_L1DQ8_2D"},
                                                             ]
                          },
                          {'dR(L1_DQ_AtVtx, gen) vs gen'+' '+var : [
                                                                    {'' : "hardP_"+var+"_dR_L1DQ8_AtVtx_2D"},
                                                                   ]
                          },
                          {'dR(L3NoID, gen) vs gen'+' '+var : [
                                                               {'' : "hardP_"+var+"_dR_L3NoID_2D"},
                                                              ]
                          },
                         ],
                 }


  dict_2D = this_2D_dict["2D"]

  for i in range(len(dict_2D)):
    for j in range(len(dict_2D[i].values()[0])):
      for camp in file_dict[proc].keys():
        c2 = TCanvas("c2","c2",200,200,900,800)
        c2.cd()
        hist_2D = file_dict[proc][camp].Get((dict_2D[i].values()[0])[j].values()[0])
        hist_2D.SetStats(0)
        hist_2D.SetTitle(dict_2D[i].keys()[0])
        hist_2D.GetXaxis().SetRangeUser(Xrange[proc]["pt"][0],Xrange[proc]["pt"][1])
        hist_2D.GetYaxis().SetRangeUser(0,5)
        hist_2D.Draw("COLZ")
        c2.SaveAs("comp/"+proc+"/dR/"+dict_2D[i].values()[0][0].values()[0]+".png")

  if proc == "Bs" or proc == "Jpsi":
    for camp in file_dict[proc].keys():
      c3 = TCanvas("c3","c3",200,200,900,800)
      c3.cd()
      hist_2D = file_dict[proc][camp].Get("hardP_pt1_dR_hardP_2D")
      hist_2D.SetStats(0)
      hist_2D.SetTitle("hardP_pt1_dR_hardP_2D")
      hist_2D.GetXaxis().SetRangeUser(Xrange[proc]["pt"][0],Xrange[proc]["pt"][1])
      hist_2D.GetYaxis().SetRangeUser(0,5)
      hist_2D.Draw("COLZ")
      c3.SaveAs("comp/"+proc+"/dR/hardP_pt1_dR_hardP_2D.png")
      hist_2D = file_dict[proc][camp].Get("hardP_pt2_dR_hardP_2D")
      hist_2D.SetStats(0)
      hist_2D.SetTitle("hardP_pt2_dR_hardP_2D")
      hist_2D.GetXaxis().SetRangeUser(Xrange[proc]["pt"][0],Xrange[proc]["pt"][1])
      hist_2D.GetYaxis().SetRangeUser(0,5)
      hist_2D.Draw("COLZ")
      c3.SaveAs("comp/"+proc+"/dR/hardP_pt2_dR_hardP_2D.png")

# -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- #

var_list = ["pt","eta","phi","truePU"]
#var_list = ["eta","phi","truePU"]
#var_list = ["pt"]
#var_2D_list = ["pt","ptres"]
#var_list = ["eta"]
#proc_list = ["DY","DY4","Bs","Jpsi"]
#proc_list = ["DY","Bs"]
#proc_list = ["DY4"]
#proc_list = ["Bs"]
#proc_list = ["Jpsi"]

for this_proc in proc_list:
  os.system("mkdir -p comp/"+this_proc)
  for this_var in var_list:
    DrawHist_New(this_proc, this_var)
  #for this_var in var_list:
  #  DrawHist(this_proc, this_var)
  #for this_2D_var in var_2D_list:
  #  Draw2DHist(this_proc, this_2D_var)
