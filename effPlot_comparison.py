import os, array
from ROOT import *

gROOT.SetBatch(kTRUE)
os.system("mkdir -p comp")

file_dict = {
             #'Winter20_113X' : TFile.Open("/data6/Users/jihkim/MuonHLT/logs/20210717__065923/Run3_Winter20_113X_DY_WP00/Run3_Winter20_113X_DY_WP00.root"),
             #'Winter21' : TFile.Open("/data6/Users/jihkim/MuonHLT/logs/20210717__050554/Run3_Winter21_subset_DY_WP00/Run3_Winter21_subset_DY_WP00.root"),
             'Winter20_113X' : TFile.Open("/data6/Users/jihkim/MuonHLT/logs/20210719__180358/Run3_Winter20_113X_DY_WP00/Run3_Winter20_113X_DY_WP00.root"),
             'Winter21' : TFile.Open("/data6/Users/jihkim/MuonHLT/logs/20210719__180358/Run3_Winter21_subset_DY_WP00/Run3_Winter21_subset_DY_WP00.root"),
            }

var_dict = {
            "pt" : array.array('d', [5, 10, 15, 20, 25, 30, 40, 50, 60, 120, 200]),
            "eta" : array.array('d', [-2.4, -2.1, -1.6, -1.2, -0.9, -0.3, -0.2, 0.2, 0.3, 0.9, 1.2, 1.6, 2.1, 2.4]),
            "phi" : array.array('d', [-3.15, -2.85, -2.55, -2.25, -1.95, -1.65, -1.35, -1.05, -0.75, -0.45, -0.15, 0.15, 0.45, 0.75, 1.05, 1.35, 1.65, 1.95, 2.25, 2.55, 2.85, 3.15]),
            "truePU" : array.array('d', [30,32,34,36,38,40,42,44,46,48,50,52,54,56,58,60,62,64,66,68,70,72,74,76,78,80]),
           }

Xrange = {
          "pt" : [20, 200],
          "eta" : [-2.4, 2.4],
          "phi" : [-3.15, 3.15],
          "truePU" : [30, 80]
         }

color_list = { 1:[kRed], 2:[kRed, kBlue], 3:[kRed, kOrange, kBlue], }

lineStyle_dict = {"Winter21" : 1, "Winter20_113X" : 2}

def DrawHist(var): # For each title : multiple { legend : histo }s to draw in the same canvas (in order).
  num_dict = [
               {'L3NoID Efficiency' : [ {'L3/hardP_L1 dR matching' : "hardP_L1_"+var+"_eff_L3NoID"},
                                        {'L3/hardP dR matching' : "hardP_"+var+"_eff_L3NoID"},
                                        {'L3/TP hit association' : "TP_"+var+"_eff_TPtoL3NoIDTrack"}, ]
               },
               {'IOFromL1 Efficiency' : [ {'L3/hardP_L1 dR matching' : "hardP_L1_"+var+"_eff_IOFromL1"},
                                          {'L3/hardP dR matching' : "hardP_"+var+"_eff_IOFromL1"},
                                          {'L3/TP hit association' : "TP_"+var+"_eff_TPtoIOFromL1"}, ]
               },
               {'IOFromL2 Efficiency' : [ {'L3/TP hit association' : "TP_"+var+"_eff_TPtoIOFromL2"}, ]
               },
               {'OI Efficiency' : [ {'L3/hardP_L1 dR matching' : "hardP_L1_"+var+"_eff_OI"},
                                    {'L3/hardP dR matching' : "hardP_"+var+"_eff_OI"},
                                    {'L3/TP hit association' : "TP_"+var+"_eff_TPtoOI"}, ]
               },
               {'L3NoID Purity' : [ {'dR matching' : "L3NoIDTrack_"+var+"_purity"},
                                    {'hit association' : "L3NoIDTrack_"+var+"_Asso"}, ]
               },
             ]
  den_dict = [
               {'L3NoID Efficiency' : [ {'L3/hardP_L1 dR matching' : "hardP_L1_"+var},
                                        {'L3/hardP dR matching' : "hardP_"+var},
                                        {'L3/TP hit association' : "TP_"+var}, ]
               },
               {'IOFromL1 Efficiency' : [ {'L3/hardP_L1 dR matching' : "hardP_L1_"+var},
                                          {'L3/hardP dR matching' : "hardP_"+var},
                                          {'L3/TP hit association' : "TP_"+var}, ]
               },
               {'IOFromL2 Efficiency' : [ {'L3/TP hit association' : "TP_"+var}, ]
               },
               {'OI Efficiency' : [ {'L3/hardP_L1 dR matching' : "hardP_L1_"+var},
                                    {'L3/hardP dR matching' : "hardP_"+var},
                                    {'L3/TP hit association' : "TP_"+var}, ]
               },
               {'L3NoID Purity' : [ {'dR matching' : "L3NoIDTrack_"+var},
                                    {'hit association' : "L3NoIDTrack_"+var}, ]
               },
             ]
  
  for i in range(len(num_dict)):
    c1 = TCanvas("c1","c1",200,200,900,800)
    c1.cd()
    #p1 = TPad("p1","p1",0,0.3,1,1)
    #p1.SetBottomMargin(0.01)
    #p1.Draw()
    #p1.cd()
    #lg = TLegend(0.7,0.75,0.9,0.9)
    lg = TLegend(0.6,0.15,0.9,0.35)
    grs = []
    for j in range(len(num_dict[i].values()[0])):
      for camp in file_dict.keys():
        #print "num = "+(num_dict[i].values()[0])[j].values()[0]
        #print "den = "+(den_dict[i].values()[0])[j].values()[0]
        #print "SetTitle = "+num_dict[i].keys()[0]
        #print "SetRangeUser = ",Xrange[var][0],Xrange[var][1]
        #print "lg.AddEntry = "+(num_dict[i].values()[0])[j].keys()[0], camp
        num = file_dict[camp].Get((num_dict[i].values()[0])[j].values()[0])
        den = file_dict[camp].Get((den_dict[i].values()[0])[j].values()[0])
        num_rebin = num.Rebin(len(var_dict[var])-1,'',var_dict[var])
        den_rebin = den.Rebin(len(var_dict[var])-1,'',var_dict[var])
        gr = TGraphAsymmErrors(len(var_dict[var])-1)
        gr.Divide(num_rebin,den_rebin)
        gr.SetTitle(num_dict[i].keys()[0])
        gr.GetXaxis().SetTitle(var)
        gr.SetMarkerStyle(8)
        gr.SetMarkerColor(color_list[len(num_dict[i].values()[0])][j])
        gr.SetLineStyle(lineStyle_dict[camp])
        gr.SetLineColor(color_list[len(num_dict[i].values()[0])][j])
        gr.SetLineWidth(3)
        gr.GetXaxis().SetRangeUser(Xrange[var][0],Xrange[var][1])
        if 'IOFromL2' in num_dict[i].keys()[0]:
          gr.GetYaxis().SetRangeUser(0., 1.1)
        else:
          gr.GetYaxis().SetRangeUser(0.5, 1.1)
        if j == 0:
          gr.Draw("AP")
        else:
          gr.Draw("P same")
        grs = [gr] + grs
        lg.AddEntry(gr,(num_dict[i].values()[0])[j].keys()[0]+" ("+camp+")")
    lg.Draw()
    c1.SaveAs("comp/"+num_dict[i].keys()[0].replace(' ','_')+"_"+var+".png")

DrawHist("pt")
DrawHist("eta")
DrawHist("phi")
DrawHist("truePU")
