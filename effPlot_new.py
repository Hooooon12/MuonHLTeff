##########################################################################################################################################
#
#  Author : Jihun Kim
#  2021/03/02
#  Description : This makes and saves eff. or purity plots with various arguments.
#                If some arguments are not specified, then it iterates with all possible values in those arguments.
#                e.g. python effPlot.py will run all possible combinations.
#                Or you can run this with specified values like : python effPlot.py -p TT -f N -m purity -v phi -n L3 -t double
#                                                                 python effPlot.py -p TT DY -f N -m eff purity -v pt phi -n L3 -t double
#
##########################################################################################################################################

import os, array #JH : to use rebin, see https://root-forum.cern.ch/t/how-to-rebin-a-hisstogram-in-python/25482/3
import argparse
from ROOT import *

parser = argparse.ArgumentParser()
parser.add_argument('-p', '--process', nargs='*', dest="proc", default=["DY","TT","Jpsi","QCD"], help="physics process: DY, TT, Jpsi, QCD")
parser.add_argument('-f', '--filter', nargs='*', dest="fltr", default=["WP","N"], help="filtering method: WP, N")
parser.add_argument('-m', '--measure', nargs='*', dest="measure", default=["trackeff","eff","purity","profile","Ntrackfrac"], help="what to measure: trackeff, eff, purity, profile, Ntrackfrac")
parser.add_argument('-v', '--variable', nargs='*', dest="var", default=["pt","eta","phi","truePU"], help="measure as a function of: pt, eta, phi, truePU")
parser.add_argument('-d', '--denominator', nargs='*', dest="den", default=["hardP","hardP_L1"], help="denominator of efficiency: hardP, hardP_L1")
parser.add_argument('-n', '--numerator', nargs='*', dest="num", default=["L3NoID","L3"], help="numerator of efficiency or denominator of purity: L3NoID, L3")
parser.add_argument('-t', '--trigger', nargs='*', dest="trig", default=["single","double"], help="which trigger to check: single(IsoMu24), double(Mu17Mu8)")
args = parser.parse_args()

gROOT.SetBatch(kTRUE)
#gROOT.SetBatch(kFALSE) #JH
os.system('mkdir -p new_Run3_newplots/eff_den_valid')
os.system('mkdir -p new_Run3_newplots/eff_num_valid')

#Xaxis title
Xtitle = {'pt' : "#scale[1.8]{p_{T}(#mu) [GeV]}", 'eta' : "#scale[1.8]{#eta(#mu)}", 'phi' : "#scale[1.8]{#varphi(#mu)}", 'truePU' : "#scale[1.8]{# of truePU}"}

#Yaxis title #TODO

#pt Xaxis range
Xrange = {'single' : [20., 200.], 'double' : [5., 200.], 'Jpsi' : [0., 22.], 'QCD' : [0., 55.]}


#Graph colors
graphColors = [kRed+1, kRed-2, kAzure+2, kAzure-1, kViolet+5]


for this_proc, this_fltr in [(this_proc,this_fltr) for this_proc in args.proc for this_fltr in args.fltr]:

  #Call the input root files
  if this_fltr == "WP":
    #this_files = ["result_"+this_proc+"_WP00.root","result_"+this_proc+"_WP02.root","result_"+this_proc+"_WP04.root","result_"+this_proc+"_WP10.root"]
    this_files = ["Run3_result_"+this_proc+"_WP00.root","Run3_result_"+this_proc+"_WP02.root","Run3_result_"+this_proc+"_WP04.root","Run3_result_"+this_proc+"_WP10.root"]
    #this_files = ["fix_Run3_result_"+this_proc+"_WP00.root","fix_Run3_result_"+this_proc+"_WP02.root","fix_Run3_result_"+this_proc+"_WP04.root","fix_Run3_result_"+this_proc+"_WP10.root"]
  elif this_fltr == "N":
    #this_files = ["result_"+this_proc+"_WP00.root","result_"+this_proc+"_N50.root","result_"+this_proc+"_N10.root","result_"+this_proc+"_N5.root"]
    this_files = ["Run3_result_"+this_proc+"_WP00.root","Run3_result_"+this_proc+"_N50.root","Run3_result_"+this_proc+"_N10.root","Run3_result_"+this_proc+"_N5.root","Run3_result_"+this_proc+"_N0.root"]
    #this_files = ["fix_Run3_result_"+this_proc+"_WP00.root","fix_Run3_result_"+this_proc+"_N50.root","fix_Run3_result_"+this_proc+"_N10.root","fix_Run3_result_"+this_proc+"_N5.root","fix_Run3_result_"+this_proc+"_N0.root"]
  
  files = [TFile.Open(this_files[i]) for i in range(len(this_files))]
  
  for this_measure, this_var, this_den, this_num, this_trig in [(this_measure,this_var,this_den,this_num,this_trig) for this_measure in args.measure for this_var in args.var for this_den in args.den for this_num in args.num for this_trig in args.trig]:
 
    #Call the target histograms
    if this_trig == "single":
      this_trigLabel = ""
    elif this_trig == "double":
      this_trigLabel = "_med"
    
    if 'L1' in this_den:
      this_den_vars = [files[i].Get(this_den+this_trigLabel+"_"+this_var) for i in range(len(files))]
      this_den_var_eff_nums = [files[i].Get(this_den+this_trigLabel+"_"+this_var+"_eff_"+this_num) for i in range(len(files))]
    else:
      this_den_vars = [files[i].Get(this_den+"_"+this_var) for i in range(len(files))]
      #this_den_var_eff_nums = [files[i].Get(this_den+"_"+this_var+"_eff_"+this_num+this_trigLabel) for i in range(len(files))] #JH : TODO for later update
      this_den_var_eff_nums = [files[i].Get(this_den+"_"+this_var+"_eff_"+this_num) for i in range(len(files))]
    this_num_vars = [files[i].Get(this_num+"_"+this_var) for i in range(len(files))]
    this_num_var_puritys = [files[i].Get(this_num+"_"+this_var+"_purity") for i in range(len(files))]

    #Set the legend name
    if this_fltr == "WP":
      lg_names = ["WP 0","WP 0.02","WP 0.04","WP 0.10"]
    elif this_fltr == "N":
      #lg_names = ["Nocut","N50","N10","N5"]
      lg_names = ["Nocut","N50","N10","N5","N0"]
    
    #Other comments on the plot
    txt_CMS = TText(0.1,0.905,"CMS simulation")
    txt_CMS.SetNDC()
    
    txt_L1pt = TLatex()
    txt_mupt = TLatex()
    txt_14TeV = TLatex()
    txt_sample = TLatex()
    
    #Bins
    ptval = array.array('d', [5, 10, 15, 20, 25, 30, 40, 50, 60, 120, 200]) #JH : Avoid starting from 0. hardP_L1_pt histo starts with pt 8 --> ZeroDivisionError
    if this_measure == "purity":
      ptval = array.array('d', [20, 25, 30, 40, 50, 60, 120, 200]) #JH : To adjust y range (relatively low purity at low pt makes plot range inconsistent)
    if this_proc == "Jpsi":
      #ptval = array.array('d', [1, 1.5, 2, 2.5, 3, 3.5, 4, 4.5, 5, 5.5, 6, 6.5, 7, 7.5, 8, 8.5, 9, 9.5, 10, 10.5, 11, 11.5, 12, 12.5, 13, 13.5, 14, 14.5, 15, 15.5, 16, 16.5, 17, 17.5, 18, 18.5, 19, 19.5, 20]) #JH
      ptval = array.array('d', [2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20]) #JH
    if this_proc == "QCD":
      ptval = array.array('d', [5, 10, 15, 20, 25, 30, 35, 40, 45, 50]) #JH
    etaval = array.array('d', [-2.4, -2.1, -1.6, -1.2, -0.9, -0.3, -0.2, 0.2, 0.3, 0.9, 1.2, 1.6, 2.1, 2.4])
    phival = array.array('d', [-3.15, -2.85, -2.55, -2.25, -1.95, -1.65, -1.35, -1.05, -0.75, -0.45, -0.15, 0.15, 0.45, 0.75, 1.05, 1.35, 1.65, 1.95, 2.25, 2.55, 2.85, 3.15])
    truePUval = array.array('d', [30,32,34,36,38,40,42,44,46,48,50,52,54,56,58,60,62,64,66,68,70,72,74,76,78,80])
    vals = {'pt' : ptval, 'eta' : etaval, 'phi' : phival, 'truePU' : truePUval}

    #Xaxis title
    this_Xtitle = Xtitle[this_var]

    #Start to make the plots

#####Efficiency#####
    if this_measure == "eff":
      den_vars = [this_den_vars[i].Rebin(len(vals[this_var])-1,'',vals[this_var]) for i in range(len(this_den_vars))]
      den_var_eff_nums = [this_den_var_eff_nums[i].Rebin(len(vals[this_var])-1,'',vals[this_var]) for i in range(len(this_den_var_eff_nums))]
      #print "current pt binning :", ptval
      #for i in range(den_var_eff_nums[0].GetNbinsX()+2):
      #  print i,"th bin center :", den_var_eff_nums[0].GetBinCenter(i),", content :", den_var_eff_nums[0].GetBinContent(i)


      ####First, check the denominators are consistent each other among the WPs.
      c_den_var = TCanvas("c_den_var","c_den_var",200,200,900,800)
      c_den_var.cd()
      
      p_den_var = TPad("p_den_var","p_den_var",0,0.3,1,1)
      p_den_var.SetBottomMargin(0.01)
      p_den_var.Draw()
      p_den_var.cd()

      lg_den_var = TLegend(0.7,0.1,0.9,0.3)

      gr_den_vars = []

      for igr in reversed(range(len(den_vars))):
        option_gr = "P SAME"

        gr_den_var = TGraphErrors(len(vals[this_var])-1) #JH : TGraphErrors is better for the symmetric errors
        for i in range(den_vars[igr].GetNbinsX()):
          gr_den_var.SetPoint(i,den_vars[igr].GetBinCenter(i+1),den_vars[igr].GetBinContent(i+1))
          gr_den_var.SetPointError(i,den_vars[igr].GetBinWidth(i+1)/2.,den_vars[igr].GetBinError(i+1))
        gr_den_var.SetMarkerStyle(20)
        gr_den_var.SetMarkerColor(graphColors[igr])
        gr_den_var.SetLineColor(graphColors[igr])
        gr_den_var.SetLineWidth(3)

        if igr == len(den_vars)-1:
          option_gr = "AP"

          gr_den_var.SetTitle("")
          gr_den_var.GetXaxis().SetTickLength(0.025)
          gr_den_var.GetXaxis().SetLabelSize(0.)
          gr_den_var.GetYaxis().SetLabelSize(0.025)
          gr_den_var.GetYaxis().SetTitle("Denominators")
          if this_var == "pt": #JH : You need this line, because leaving other variable bins as default is clean.
            gr_den_var.GetXaxis().SetRangeUser(Xrange[this_trig][0],Xrange[this_trig][1]) #JH : Xrange['single'] or Xrange['double']
            if this_proc == "Jpsi" or this_proc == "QCD":
              gr_den_var.GetXaxis().SetRangeUser(Xrange[this_proc][0],Xrange[this_proc][1]) #JH
          yran1 = TMath.MinElement(len(gr_den_var.GetY()),gr_den_var.GetY())*0.9 #JH : You cannot use gr.GetMinimum() or gr.GetMaximum. see https://root-forum.cern.ch/t/tgraph-getmaximum-getminimum/8867/2
          yran2 = TMath.MaxElement(len(gr_den_var.GetY()),gr_den_var.GetY())*1.1
          gr_den_var.GetYaxis().SetRangeUser(yran1,yran2)
          
        gr_den_var.Draw(option_gr)
        gr_den_vars = [gr_den_var] + gr_den_vars

        lg_den_var.AddEntry(gr_den_var,lg_names[igr])

      lg_den_var.Draw()
      
      txt_CMS.Draw()
      if this_proc == "DY":
        txt_sample.DrawLatexNDC(0.46,0.91,"#scale[0.6]{DYToLL_M-50_TuneCP5_14TeV-pythia8_HCAL (14TeV)}")
      if this_proc == "TT":
        txt_sample.DrawLatexNDC(0.58,0.91,"#scale[0.6]{TTbar_14TeV_TuneCP5_Pythia8 (14TeV)}")
      if this_proc == "Jpsi":
        txt_sample.DrawLatexNDC(0.46,0.91,"#scale[0.6]{JpsiToMuMu_JpsiPt8_TuneCP5_14TeV-pythia8 (14TeV)}")
      if this_proc == "QCD":
        txt_sample.DrawLatexNDC(0.34,0.91,"#scale[0.6]{QCD_Pt_15to7000_TuneCP5_muEnriched_Flat_14TeV_pythia8 (14TeV)}")
      if this_proc == "DY" or this_proc == "TT":
        if this_den == "hardP_L1":
          if this_trig == "single":
            txt_L1pt.DrawLatexNDC(0.12,0.86,"#scale[0.8]{gen #mu matched with p_{T}(L1 #mu) > 22GeV}")
            if this_var != "pt":
              txt_mupt.DrawLatexNDC(0.12,0.82,"#scale[0.8]{p_{T}(#mu) > 26GeV}")
          if this_trig == "double":
            txt_L1pt.DrawLatexNDC(0.12,0.86,"#scale[0.8]{gen #mu matched with p_{T}(L1 #mu) > 5GeV}")
            if this_var != "pt":
              txt_mupt.DrawLatexNDC(0.12,0.82,"#scale[0.8]{p_{T}(#mu) > 10GeV}")
        if this_den == "hardP":
          if this_var != "pt":
            if this_trig == "single":
              txt_mupt.DrawLatexNDC(0.12,0.86,"#scale[0.8]{p_{T}(#mu) > 26GeV}")
            if this_trig == "double":
              txt_mupt.DrawLatexNDC(0.12,0.86,"#scale[0.8]{p_{T}(#mu) > 10GeV}") #JH : TODO how to improve this

      c_den_var.cd()
      
      p_ratio_den_var = TPad("p_ratio_den_var","p_ratio_den_var",0,0,1,0.3)
      p_ratio_den_var.SetTopMargin(0.03)
      p_ratio_den_var.SetBottomMargin(0.2)
      p_ratio_den_var.Draw()
      p_ratio_den_var.cd()
   
      gr_ratio_den_vars = []

      for irat in reversed(range(len(den_vars))):
        option_rat = "P SAME"

        gr_ratio_den_var = TGraphAsymmErrors(len(vals[this_var])-1)
        #gr_ratio_den_var.Divide(den_vars[irat],den_vars[0],"cl=0.683 b(1,1) mode") #JH : BayesDivide. see https://root.cern.ch/doc/master/classTGraphAsymmErrors.html#a78cd209f4da9a169848ab23f539e1c94
        gr_ratio_den_var.Divide(den_vars[irat],den_vars[0],"cp") #JH : Clopper-Pearson interval. see https://root.cern.ch/doc/master/classTEfficiency.html#ae80c3189bac22b7ad15f57a1476ef75b

        gr_ratio_den_var.SetMarkerStyle(20)
        gr_ratio_den_var.SetMarkerColor(graphColors[irat])
        gr_ratio_den_var.SetLineColor(graphColors[irat])
        gr_ratio_den_var.SetLineWidth(3)

        if irat == len(den_vars)-1:
          option_rat = "AP"
          gr_ratio_den_var.SetTitle("")
          gr_ratio_den_var.GetXaxis().SetTitle(this_Xtitle)
          gr_ratio_den_var.GetXaxis().SetTitleOffset(1.7)
          gr_ratio_den_var.GetXaxis().SetTickLength(0.05)
          gr_ratio_den_var.GetXaxis().SetLabelSize(0.06)
          gr_ratio_den_var.GetYaxis().SetLabelSize(0.05)
          gr_ratio_den_var.GetYaxis().SetTitle("#scale[1.8]{Ratio to Nocut}")
          gr_ratio_den_var.GetYaxis().SetTitleOffset(0.8)
          if this_var == "pt":
            gr_ratio_den_var.GetXaxis().SetRangeUser(Xrange[this_trig][0],Xrange[this_trig][1]) #JH : Xrange['single'] or Xrange['double']
            if this_proc == "Jpsi" or this_proc == "QCD":
              gr_ratio_den_var.GetXaxis().SetRangeUser(Xrange[this_proc][0],Xrange[this_proc][1]) #JH
          yran1 = TMath.MinElement(len(gr_ratio_den_var.GetY()),gr_ratio_den_var.GetY())*0.999 #JH
          yran2 = TMath.MaxElement(len(gr_ratio_den_var.GetY()),gr_ratio_den_var.GetY())*1.001 #JH
          gr_ratio_den_var.GetYaxis().SetRangeUser(yran1,yran2)

        gr_ratio_den_var.Draw(option_rat)
        gr_ratio_den_vars.append(gr_ratio_den_var) #JH : I really don't know why but this appending prevent the bug (???)

      c_den_var.SaveAs("new_Run3_newplots/eff_den_valid/"+this_proc+"_"+this_fltr+"_"+this_den+this_trigLabel+"_"+this_var+".png")










      ####Then, check whether the numerators also show the efficiency-increasing behavior.
      c_den_var_num = TCanvas("c_den_var_num","c_den_var_num",200,200,900,800)
      c_den_var_num.cd()
      
      p_den_var_num = TPad("p_den_var_num","p_den_var_num",0,0.3,1,1)
      p_den_var_num.SetBottomMargin(0.01)
      p_den_var_num.Draw()
      p_den_var_num.cd()

      lg_den_var_num = TLegend(0.7,0.1,0.9,0.3)

      gr_den_var_nums = []

      for igr in reversed(range(len(den_var_eff_nums))):
        option_gr = "P SAME"

        #gr_den_var_num = TGraphAsymmErrors(len(vals[this_var])-1)
        #gr_den_var_num.Divide(den_var_nums[igr],den_var_nums[igr],"n e0")
        gr_den_var_num = TGraphErrors(len(vals[this_var])-1)
        for i in range(den_var_eff_nums[igr].GetNbinsX()):
          gr_den_var_num.SetPoint(i,den_var_eff_nums[igr].GetBinCenter(i+1),den_var_eff_nums[igr].GetBinContent(i+1))
          gr_den_var_num.SetPointError(i,den_var_eff_nums[igr].GetBinWidth(i+1)/2.,den_var_eff_nums[igr].GetBinError(i+1))
        gr_den_var_num.SetMarkerStyle(20)
        gr_den_var_num.SetMarkerColor(graphColors[igr])
        gr_den_var_num.SetLineColor(graphColors[igr])
        gr_den_var_num.SetLineWidth(3)

        if igr == len(den_var_eff_nums)-1:
          option_gr = "AP"

          gr_den_var_num.SetTitle("")
          gr_den_var_num.GetXaxis().SetTickLength(0.025)
          gr_den_var_num.GetXaxis().SetLabelSize(0.)
          gr_den_var_num.GetYaxis().SetLabelSize(0.025)
          gr_den_var_num.GetYaxis().SetTitle("Numerators")
          if this_var == "pt": #JH : You need this line, because I want to customize ONLY pt bins, not other bins.
            gr_den_var_num.GetXaxis().SetRangeUser(Xrange[this_trig][0],Xrange[this_trig][1]) #JH : Xrange['single'] or Xrange['double']
            if this_proc == "Jpsi" or this_proc == "QCD":
              gr_den_var_num.GetXaxis().SetRangeUser(Xrange[this_proc][0],Xrange[this_proc][1]) #JH
          yran1 = TMath.MinElement(len(gr_den_var_num.GetY()),gr_den_var_num.GetY())*0.9 #JH : You cannot use gr.GetMinimum() or gr.GetMaximum. see https://root-forum.cern.ch/t/tgraph-getmaximum-getminimum/8867/2
          yran2 = TMath.MaxElement(len(gr_den_var_num.GetY()),gr_den_var_num.GetY())*1.1
          gr_den_var_num.GetYaxis().SetRangeUser(yran1,yran2) #JH
          
        gr_den_var_num.Draw(option_gr)
        gr_den_var_nums = [gr_den_var_num] + gr_den_var_nums

        lg_den_var_num.AddEntry(gr_den_var_num,lg_names[igr])

      lg_den_var_num.Draw()
      
      txt_CMS.Draw()
      if this_proc == "DY":
        txt_sample.DrawLatexNDC(0.46,0.91,"#scale[0.6]{DYToLL_M-50_TuneCP5_14TeV-pythia8_HCAL (14TeV)}")
      if this_proc == "TT":
        txt_sample.DrawLatexNDC(0.58,0.91,"#scale[0.6]{TTbar_14TeV_TuneCP5_Pythia8 (14TeV)}")
      if this_proc == "Jpsi":
        txt_sample.DrawLatexNDC(0.46,0.91,"#scale[0.6]{JpsiToMuMu_JpsiPt8_TuneCP5_14TeV-pythia8 (14TeV)}")
      if this_proc == "QCD":
        txt_sample.DrawLatexNDC(0.34,0.91,"#scale[0.6]{QCD_Pt_15to7000_TuneCP5_muEnriched_Flat_14TeV_pythia8 (14TeV)}")
      if this_proc == "DY" or this_proc == "TT":
        if this_den == "hardP_L1":
          if this_trig == "single":
            txt_L1pt.DrawLatexNDC(0.12,0.86,"#scale[0.8]{gen #mu matched with p_{T}(L1 #mu) > 22GeV}")
            if this_var != "pt":
              txt_mupt.DrawLatexNDC(0.12,0.82,"#scale[0.8]{p_{T}(#mu) > 26GeV}")
          if this_trig == "double":
            txt_L1pt.DrawLatexNDC(0.12,0.86,"#scale[0.8]{gen #mu matched with p_{T}(L1 #mu) > 5GeV}")
            if this_var != "pt":
              txt_mupt.DrawLatexNDC(0.12,0.82,"#scale[0.8]{p_{T}(#mu) > 10GeV}")
        if this_den == "hardP":
          if this_var != "pt":
            if this_trig == "single":
              txt_mupt.DrawLatexNDC(0.12,0.86,"#scale[0.8]{p_{T}(#mu) > 26GeV}")
            if this_trig == "double":
              txt_mupt.DrawLatexNDC(0.12,0.86,"#scale[0.8]{p_{T}(#mu) > 10GeV}") #JH : TODO how to improve this
      
      c_den_var_num.cd()
      
      p_ratio_den_var_num = TPad("p_ratio_den_var_num","p_ratio_den_var_num",0,0,1,0.3)
      p_ratio_den_var_num.SetTopMargin(0.03)
      p_ratio_den_var_num.SetBottomMargin(0.2)
      p_ratio_den_var_num.Draw()
      p_ratio_den_var_num.cd()
   
      gr_ratio_den_var_nums = []

      for irat in reversed(range(len(den_var_eff_nums))):
        option_rat = "P SAME"

        gr_ratio_den_var_num = TGraphAsymmErrors(len(vals[this_var])-1)
        #gr_ratio_den_var_num.Divide(den_var_eff_nums[irat],den_var_eff_nums[0],"cl=0.683 b(1,1) mode") #JH : BayesDivide. see https://root.cern.ch/doc/master/classTGraphAsymmErrors.html#a78cd209f4da9a169848ab23f539e1c94
        gr_ratio_den_var_num.Divide(den_var_eff_nums[irat],den_var_eff_nums[0],"pois") #JH : "pois" interpret two histos as independent, not the efficiency. (which will return error if >1. see https://root.cern.ch/doc/master/classTGraphAsymmErrors.html#a78cd209f4da9a169848ab23f539e1c94)
       
        gr_ratio_den_var_num.SetMarkerStyle(20)
        gr_ratio_den_var_num.SetMarkerColor(graphColors[irat])
        gr_ratio_den_var_num.SetLineColor(graphColors[irat])
        gr_ratio_den_var_num.SetLineWidth(3)

        if irat == len(den_var_eff_nums)-1:
          option_rat = "AP"
          gr_ratio_den_var_num.SetTitle("")
          gr_ratio_den_var_num.GetXaxis().SetTitle(this_Xtitle)
          gr_ratio_den_var_num.GetXaxis().SetTitleOffset(1.7)
          gr_ratio_den_var_num.GetXaxis().SetTickLength(0.05)
          gr_ratio_den_var_num.GetXaxis().SetLabelSize(0.06)
          gr_ratio_den_var_num.GetYaxis().SetLabelSize(0.05)
          gr_ratio_den_var_num.GetYaxis().SetTitle("#scale[1.8]{Ratio to Nocut}")
          gr_ratio_den_var_num.GetYaxis().SetTitleOffset(0.8)
          if this_var == "pt":
            gr_ratio_den_var_num.GetXaxis().SetRangeUser(Xrange[this_trig][0],Xrange[this_trig][1]) #JH : Xrange['single'] or Xrange['double']
            if this_proc == "Jpsi" or this_proc == "QCD":
              gr_ratio_den_var_num.GetXaxis().SetRangeUser(Xrange[this_proc][0],Xrange[this_proc][1]) #JH
          #yran1 = TMath.MinElement(len(gr_ratio_den_var_num.GetY()),gr_ratio_den_var_num.GetY())*0.98 #JH
          #yran2 = TMath.MaxElement(len(gr_ratio_den_var_num.GetY()),gr_ratio_den_var_num.GetY())*1.02 #JH
          #gr_ratio_den_var_num.GetYaxis().SetRangeUser(yran1,yran2) #JH
          gr_ratio_den_var_num.GetYaxis().SetRangeUser(0.999,1.001) #JH

        gr_ratio_den_var_num.Draw(option_rat)
        gr_ratio_den_var_nums.append(gr_ratio_den_var_num) #JH : I really don't know why but this appending prevent the bug (???)

      c_den_var_num.SaveAs("new_Run3_newplots/eff_num_valid/"+this_proc+"_"+this_fltr+"_"+this_den+this_trigLabel+"_"+this_var+"_"+this_num+".png")








      ####Now start to calculate efficiency

      #print "canvas name : " + this_proc+"_"+this_fltr+"_"+this_den+this_trigLabel+"_"+this_var+"_eff_"+this_num  
      c_den_var_eff_num = TCanvas("c_den_var_eff_num","c_den_var_eff_num",200,200,900,800)
      #c_den_var_eff_num = TCanvas(this_proc+"_"+this_fltr+"_"+this_den+this_trigLabel+"_"+this_var+"_eff_"+this_num,this_proc+"_"+this_fltr+"_"+this_den+this_trigLabel+"_"+this_var+"_eff_"+this_num,200,200,900,800) #FIXME segmentation violation. WHY????
      c_den_var_eff_num.cd()
      
      p_den_var_eff_num = TPad("p_den_var_eff_num","p_den_var_eff_num",0,0.3,1,1)
      p_den_var_eff_num.SetBottomMargin(0.01)
      p_den_var_eff_num.Draw()
      p_den_var_eff_num.cd()

      lg_den_var_eff_num = TLegend(0.7,0.1,0.9,0.3)

      gr_den_var_eff_nums = []

      for igr in reversed(range(len(this_den_vars))):
        option_gr = "P SAME"

        gr_den_var_eff_num = TGraphAsymmErrors(len(vals[this_var])-1)
        gr_den_var_eff_num.Divide(den_var_eff_nums[igr],den_vars[igr],"n e0")
        gr_den_var_eff_num.SetMarkerStyle(20)
        gr_den_var_eff_num.SetMarkerColor(graphColors[igr])
        gr_den_var_eff_num.SetLineColor(graphColors[igr])
        gr_den_var_eff_num.SetLineWidth(3)

        if igr == len(this_den_vars)-1:
          option_gr = "AP"

          gr_den_var_eff_num.SetTitle("")
          gr_den_var_eff_num.GetXaxis().SetTickLength(0.025)
          gr_den_var_eff_num.GetXaxis().SetLabelSize(0.)
          gr_den_var_eff_num.GetYaxis().SetLabelSize(0.025)
          gr_den_var_eff_num.GetYaxis().SetTitle("Efficiency")
          #if this_trig == "single":
          #  gr_den_var_eff_num.GetXaxis().SetRangeUser(20.,200.) #JH : TODO
          #if this_trig == "double":
          #  gr_den_var_eff_num.GetXaxis().SetRangeUser(5.,200.) #JH : TODO
          if this_var == "pt": #JH : You need this line, because I want to customize ONLY pt bins, not other bins.
            gr_den_var_eff_num.GetXaxis().SetRangeUser(Xrange[this_trig][0],Xrange[this_trig][1]) #JH : Xrange['single'] or Xrange['double']
            if this_proc == "Jpsi" or this_proc == "QCD":
              gr_den_var_eff_num.GetXaxis().SetRangeUser(Xrange[this_proc][0],Xrange[this_proc][1]) #JH
          yran1 = TMath.MinElement(len(gr_den_var_eff_num.GetY()),gr_den_var_eff_num.GetY())*0.98 #JH : You cannot use gr.GetMinimum() or gr.GetMaximum. see https://root-forum.cern.ch/t/tgraph-getmaximum-getminimum/8867/2
          yran2 = TMath.MaxElement(len(gr_den_var_eff_num.GetY()),gr_den_var_eff_num.GetY())*1.02
          if this_proc == "Jpsi":
            if this_fltr == "N":
              yran2 = TMath.MaxElement(len(gr_den_var_eff_num.GetY()),gr_den_var_eff_num.GetY())*1.05
          gr_den_var_eff_num.GetYaxis().SetRangeUser(yran1,yran2) #JH : TODO
          
        gr_den_var_eff_num.Draw(option_gr)
        gr_den_var_eff_nums = [gr_den_var_eff_num] + gr_den_var_eff_nums

        lg_den_var_eff_num.AddEntry(gr_den_var_eff_num,lg_names[igr])

      lg_den_var_eff_num.Draw()
      
      txt_CMS.Draw()
      if this_proc == "DY":
        txt_sample.DrawLatexNDC(0.46,0.91,"#scale[0.6]{DYToLL_M-50_TuneCP5_14TeV-pythia8_HCAL (14TeV)}")
      if this_proc == "TT":
        txt_sample.DrawLatexNDC(0.58,0.91,"#scale[0.6]{TTbar_14TeV_TuneCP5_Pythia8 (14TeV)}")
      if this_proc == "Jpsi":
        txt_sample.DrawLatexNDC(0.46,0.91,"#scale[0.6]{JpsiToMuMu_JpsiPt8_TuneCP5_14TeV-pythia8 (14TeV)}")
      if this_proc == "QCD":
        txt_sample.DrawLatexNDC(0.34,0.91,"#scale[0.6]{QCD_Pt_15to7000_TuneCP5_muEnriched_Flat_14TeV_pythia8 (14TeV)}")
      if this_proc == "DY" or this_proc == "TT":
        if this_den == "hardP_L1":
          if this_trig == "single":
            txt_L1pt.DrawLatexNDC(0.12,0.86,"#scale[0.8]{gen #mu matched with p_{T}(L1 #mu) > 22GeV}")
            if this_var != "pt":
              txt_mupt.DrawLatexNDC(0.12,0.82,"#scale[0.8]{p_{T}(#mu) > 26GeV}")
          if this_trig == "double":
            txt_L1pt.DrawLatexNDC(0.12,0.86,"#scale[0.8]{gen #mu matched with p_{T}(L1 #mu) > 5GeV}")
            if this_var != "pt":
              txt_mupt.DrawLatexNDC(0.12,0.82,"#scale[0.8]{p_{T}(#mu) > 10GeV}")
        if this_den == "hardP":
          if this_var != "pt":
            if this_trig == "single":
              txt_mupt.DrawLatexNDC(0.12,0.86,"#scale[0.8]{p_{T}(#mu) > 26GeV}")
            if this_trig == "double":
              txt_mupt.DrawLatexNDC(0.12,0.86,"#scale[0.8]{p_{T}(#mu) > 10GeV}") #JH : TODO how to improve this
      
      c_den_var_eff_num.cd()
      
      p_ratio_den_var_eff_num = TPad("p_ratio_den_var_eff_num","p_ratio_den_var_eff_num",0,0,1,0.3)
      p_ratio_den_var_eff_num.SetTopMargin(0.03)
      p_ratio_den_var_eff_num.SetBottomMargin(0.2)
      p_ratio_den_var_eff_num.Draw()
      p_ratio_den_var_eff_num.cd()
   
      gr_ratio_den_var_eff_nums = []

      for irat in reversed(range(len(this_den_vars))):
        option_rat = "P SAME"

        gr_ratio_den_var_eff_num = TGraphErrors(len(vals[this_var])-1)
        for i in range(gr_den_var_eff_nums[0].GetN()):
          #print i,"th x :",gr_den_var_eff_nums[0].GetX()[i],", y :",gr_den_var_eff_nums[0].GetY()[i] #JH
          gr_ratio_den_var_eff_num.SetPoint(i,gr_den_var_eff_nums[0].GetX()[i],gr_den_var_eff_nums[irat].GetY()[i]/gr_den_var_eff_nums[0].GetY()[i])
          gr_ratio_den_var_eff_num.SetPointError(i,(vals[this_var][i+1]-vals[this_var][i])/2,0)
        
        gr_ratio_den_var_eff_num.SetMarkerStyle(20)
        gr_ratio_den_var_eff_num.SetMarkerColor(graphColors[irat])
        gr_ratio_den_var_eff_num.SetLineColor(graphColors[irat])
        gr_ratio_den_var_eff_num.SetLineWidth(3)

        if irat == len(this_den_vars)-1:
          option_rat = "AP"
          gr_ratio_den_var_eff_num.SetTitle("")
          gr_ratio_den_var_eff_num.GetXaxis().SetTitle(this_Xtitle) #JH : TODO
          gr_ratio_den_var_eff_num.GetXaxis().SetTitleOffset(1.7)
          gr_ratio_den_var_eff_num.GetXaxis().SetTickLength(0.05)
          gr_ratio_den_var_eff_num.GetXaxis().SetLabelSize(0.06)
          gr_ratio_den_var_eff_num.GetYaxis().SetLabelSize(0.05)
          gr_ratio_den_var_eff_num.GetYaxis().SetTitle("#scale[1.8]{Ratio to Nocut}")
          gr_ratio_den_var_eff_num.GetYaxis().SetTitleOffset(0.8)
          #if this_trig == "single":
          #  gr_ratio_den_var_eff_num.GetXaxis().SetRangeUser(20,200) #JH : TODO
          #elif this_trig == "double":
          #  gr_ratio_den_var_eff_num.GetXaxis().SetRangeUser(5,200) #JH : TODO
          if this_var == "pt":
            gr_ratio_den_var_eff_num.GetXaxis().SetRangeUser(Xrange[this_trig][0],Xrange[this_trig][1]) #JH : Xrange['single'] or Xrange['double']
            if this_proc == "Jpsi" or this_proc == "QCD":
              gr_ratio_den_var_eff_num.GetXaxis().SetRangeUser(Xrange[this_proc][0],Xrange[this_proc][1]) #JH
          yran1 = TMath.MinElement(len(gr_ratio_den_var_eff_num.GetY()),gr_ratio_den_var_eff_num.GetY())*0.98 #JH : TODO
          yran2 = TMath.MaxElement(len(gr_ratio_den_var_eff_num.GetY()),gr_ratio_den_var_eff_num.GetY())*1.02 #JH : TODO
          if this_proc == "Jpsi":
            if this_fltr == "N":
              yran2 = TMath.MaxElement(len(gr_ratio_den_var_eff_num.GetY()),gr_ratio_den_var_eff_num.GetY())*1.05 #JH : TODO
          gr_ratio_den_var_eff_num.GetYaxis().SetRangeUser(yran1,yran2) #JH : TODO
          #gr_ratio_den_var_eff_num.GetYaxis().SetRangeUser(0.9,1.1) #JH : TODO

        gr_ratio_den_var_eff_num.Draw(option_rat)
        gr_ratio_den_var_eff_nums.append(gr_ratio_den_var_eff_num) #JH : I really don't know why but this appending prevent the bug (???)

      #c_den_var_eff_num.SaveAs("Run3_newplots/"+this_proc+"_"+this_fltr+"_"+this_den+this_trigLabel+"_"+this_var+"_eff_"+this_num+".png")
      #c_den_var_eff_num.SaveAs("fix_Run3_newplots/"+this_proc+"_"+this_fltr+"_"+this_den+this_trigLabel+"_"+this_var+"_eff_"+this_num+".png")
      c_den_var_eff_num.SaveAs("new_Run3_newplots/"+this_proc+"_"+this_fltr+"_"+this_den+this_trigLabel+"_"+this_var+"_eff_"+this_num+".png")

   
######purity#####       
    if this_measure == "purity":
      num_vars = [this_num_vars[i].Rebin(len(vals[this_var])-1,'',vals[this_var]) for i in range(len(this_num_vars))]
      num_var_puritys = [this_num_var_puritys[i].Rebin(len(vals[this_var])-1,'',vals[this_var]) for i in range(len(this_num_var_puritys))]
      
      c_num_var_purity = TCanvas("c_num_var_purity","c_num_var_purity",200,200,900,800)
      #c_num_var_purity = TCanvas(this_proc+"_"+this_fltr+"_"+this_num+"_"+this_var+"_purity",this_proc+"_"+this_fltr+"_"+this_num+"_"+this_var+"_purity",200,200,900,800) #FIXME segmentation violation. WHY?????
      c_num_var_purity.cd()
      
      p_num_var_purity = TPad("p_num_var_purity","p_num_var_purity",0,0.3,1,1)
      p_num_var_purity.SetBottomMargin(0.01)
      p_num_var_purity.Draw()
      p_num_var_purity.cd()
     
      if this_proc == "Jpsi" and this_var == "pt" and this_num == "L3":
        lg_num_var_purity = TLegend(0.7,0.15,0.9,0.4) #JH : TODO
      else:
        lg_num_var_purity = TLegend(0.7,0.7,0.9,0.9) #JH : TODO

      gr_num_var_puritys = []

      for igr in reversed(range(len(this_num_vars))):
      #for igr in [4,3,0]: #JH : to confirm N0 and N5 are really overlapped or just a bug
        option_gr = "P SAME"

        gr_num_var_purity = TGraphAsymmErrors(len(vals[this_var])-1)
        gr_num_var_purity.Divide(num_var_puritys[igr],num_vars[igr],"n e0")
        gr_num_var_purity.SetMarkerStyle(20)
        gr_num_var_purity.SetMarkerColor(graphColors[igr])
        gr_num_var_purity.SetLineColor(graphColors[igr])
        gr_num_var_purity.SetLineWidth(3)

        if igr == len(this_num_vars)-1:
          option_gr = "AP"

          gr_num_var_purity = TGraphAsymmErrors(len(vals[this_var])-1)
          gr_num_var_purity.Divide(num_var_puritys[3],num_vars[3],"n e0")
          gr_num_var_purity.SetMarkerStyle(20)
          gr_num_var_purity.SetMarkerColor(graphColors[igr])
          gr_num_var_purity.SetLineColor(graphColors[igr])
          gr_num_var_purity.SetLineWidth(3)
          gr_num_var_purity.SetTitle("")
          gr_num_var_purity.GetXaxis().SetTickLength(0.025)
          gr_num_var_purity.GetXaxis().SetLabelSize(0.)
          gr_num_var_purity.GetYaxis().SetLabelSize(0.025)
          gr_num_var_purity.GetYaxis().SetTitle("Purity")
          if this_var == "pt":
            if this_proc == "Jpsi" or this_proc == "QCD":
              gr_num_var_purity.GetXaxis().SetRangeUser(Xrange[this_proc][0],Xrange[this_proc][1]) #JH
          yran1 = TMath.MinElement(len(gr_num_var_purity.GetY()),gr_num_var_purity.GetY())*0.8 #JH : TODO
          if this_proc == "Jpsi":
            if this_var == "pt" or this_var == "phi":
              yran1 = TMath.MinElement(len(gr_num_var_purity.GetY()),gr_num_var_purity.GetY())*0.7 #JH : TODO
          yran2 = TMath.MaxElement(len(gr_num_var_purity.GetY()),gr_num_var_purity.GetY())*1.05 #JH : TODO
          if this_var == "pt" or this_var == "phi":
            yran1 = TMath.MinElement(len(gr_num_var_purity.GetY()),gr_num_var_purity.GetY())*0.9 #JH
          gr_num_var_purity.GetYaxis().SetRangeUser(yran1,yran2) #JH : TODO

        gr_num_var_purity.Draw(option_gr)
        gr_num_var_puritys = [gr_num_var_purity] + gr_num_var_puritys

        lg_num_var_purity.AddEntry(gr_num_var_purity,lg_names[igr])
      
      lg_num_var_purity.Draw()
      
      txt_CMS.Draw()
      if this_proc == "DY":
        txt_sample.DrawLatexNDC(0.46,0.91,"#scale[0.6]{DYToLL_M-50_TuneCP5_14TeV-pythia8_HCAL (14TeV)}")
      elif this_proc == "TT":
        txt_sample.DrawLatexNDC(0.58,0.91,"#scale[0.6]{TTbar_14TeV_TuneCP5_Pythia8 (14TeV)}")
      elif this_proc == "Jpsi":
        txt_sample.DrawLatexNDC(0.46,0.91,"#scale[0.6]{JpsiToMuMu_JpsiPt8_TuneCP5_14TeV-pythia8 (14TeV)}")
      elif this_proc == "QCD":
        txt_sample.DrawLatexNDC(0.34,0.91,"#scale[0.6]{QCD_Pt_15to7000_TuneCP5_muEnriched_Flat_14TeV_pythia8 (14TeV)}")
      
      c_num_var_purity.cd()
      
      p_ratio_num_var_purity = TPad("p_ratio_num_var_purity","p_ratio_num_var_purity",0,0,1,0.3)
      p_ratio_num_var_purity.SetTopMargin(0.03)
      p_ratio_num_var_purity.SetBottomMargin(0.2)
      p_ratio_num_var_purity.Draw()
      p_ratio_num_var_purity.cd()
     
      gr_ratio_num_var_puritys = []

      for irat in reversed(range(len(this_num_vars))):
      #for irat in [2,1,0]: #JH : to confirm N0 and N5 are really overlapped or just a bug
        option_rat = "P SAME"

        gr_ratio_num_var_purity = TGraphErrors(len(vals[this_var])-1)
        for i in range(gr_num_var_puritys[0].GetN()):
          gr_ratio_num_var_purity.SetPoint(i,gr_num_var_puritys[0].GetX()[i],gr_num_var_puritys[irat].GetY()[i]/gr_num_var_puritys[0].GetY()[i])
          gr_ratio_num_var_purity.SetPointError(i,(vals[this_var][i+1]-vals[this_var][i])/2,0)
        
        gr_ratio_num_var_purity.SetMarkerStyle(20)
        gr_ratio_num_var_purity.SetMarkerColor(graphColors[irat])
        gr_ratio_num_var_purity.SetLineColor(graphColors[irat])
        gr_ratio_num_var_purity.SetLineWidth(3)

        if irat == len(this_num_vars)-1:
        #if irat == 2: #JH : to confirm N0 and N5 are really overlapped or just a bug
          option_rat = "AP"

          gr_ratio_num_var_purity.SetTitle("")
          gr_ratio_num_var_purity.GetXaxis().SetTitle(this_Xtitle) #JH : TODO
          gr_ratio_num_var_purity.GetXaxis().SetTitleOffset(1.7)
          gr_ratio_num_var_purity.GetXaxis().SetTickLength(0.05)
          gr_ratio_num_var_purity.GetXaxis().SetLabelSize(0.06)
          gr_ratio_num_var_purity.GetYaxis().SetLabelSize(0.05)
          gr_ratio_num_var_purity.GetYaxis().SetTitle("#scale[1.8]{Ratio to Nocut}")
          gr_ratio_num_var_purity.GetYaxis().SetTitleOffset(0.8)
          if this_var == "pt":
            if this_proc == "Jpsi" or this_proc == "QCD":
              gr_ratio_num_var_purity.GetXaxis().SetRangeUser(Xrange[this_proc][0],Xrange[this_proc][1]) #JH
          yran1 = TMath.MinElement(len(gr_ratio_num_var_purity.GetY()),gr_ratio_num_var_purity.GetY())*0.95 #JH : TODO
          if this_proc == "Jpsi":
            if this_var == "pt" or this_var == "phi":
              yran1 = TMath.MinElement(len(gr_ratio_num_var_purity.GetY()),gr_ratio_num_var_purity.GetY())*0.85 #JH : TODO
          yran2 = TMath.MaxElement(len(gr_ratio_num_var_purity.GetY()),gr_ratio_num_var_purity.GetY())*1.05 #JH : TODO
          #if this_var == "phi":
          #  yran1 = TMath.MinElement(len(gr_num_var_purity_WP3.GetY()),gr_num_var_purity_WP3.GetY())*0.948 #JH
          gr_ratio_num_var_purity.GetYaxis().SetRangeUser(yran1,yran2) #JH : TODO
          
        gr_ratio_num_var_purity.Draw(option_rat)
        gr_ratio_num_var_puritys.append(gr_ratio_num_var_purity)

      #c_num_var_purity.SaveAs("Run3_newplots/"+this_proc+"_"+this_fltr+"_"+this_num+"_"+this_var+"_purity.png")
      #c_num_var_purity.SaveAs("fix_Run3_newplots/"+this_proc+"_"+this_fltr+"_"+this_num+"_"+this_var+"_purity.png")
      c_num_var_purity.SaveAs("new_Run3_newplots/"+this_proc+"_"+this_fltr+"_"+this_num+"_"+this_var+"_purity.png")


"""
this_file = TFile.Open("result_DY_WP0p00.root")
hardP_pt_WP0 = this_file.Get("hardP_pt")
hardP_pt_trackeff_step1_WP0 = this_file.Get("hardP_pt_trackeff_step1")

hardP_pt_WP0.Rebin(len(ptval)-1,"den_hardP_pt_WP0")
hardP_pt_trackeff_step1_WP0.Rebin(len(ptval)-1,"num_hardP_pt_trackeff_step1_WP0")

c_hardP_pt_trackeff_WP0 = TCanvas("hardP_pt_trackeff_WP0","hardP_pt_trackeff_WP0",200,200,900,800)

gr_hardP_pt_trackeff_step1_WP0 = TGraphAsymmErrors(len(ptval)-1)
gr_hardP_pt_trackeff_step1_WP0.Divide(num_hardP_pt_trackeff_step1_WP0,den_hardP_pt_WP0,"n e0")
gr_hardP_pt_trackeff_step1_WP0.SetMarkerStyle(20)
gr_hardP_pt_trackeff_step1_WP0.SetMarkerColor(kRed+1)
gr_hardP_pt_trackeff_step1_WP0.SetLineColor(kRed+1)
gr_hardP_pt_trackeff_step1_WP0.SetLineWidth(3)
gr_hardP_pt_trackeff_step1_WP0.SetTitle("")
gr_hardP_pt_trackeff_step1_WP0.Draw("AP")
gr_hardP_pt_trackeff_step1_WP0.GetXaxis().SetTitle("#scale[0.8]{p_{T} (GeV)}")
gr_hardP_pt_trackeff_step1_WP0.GetXaxis().SetTickLength(0.025)
gr_hardP_pt_trackeff_step1_WP0.GetXaxis().SetLabelSize(0.025)
gr_hardP_pt_trackeff_step1_WP0.GetYaxis().SetLabelSize(0.025)
gr_hardP_pt_trackeff_step1_WP0.GetYaxis().SetTitle("Efficiency")
gr_hardP_pt_trackeff_step1_WP0.GetXaxis().SetRangeUser(20.,200.)
gr_hardP_pt_trackeff_step1_WP0.GetYaxis().SetRangeUser(0.,1.2)

#c_hardP_pt_trackeff_WP0.Update()

#c_hardP_pt_trackeff_WP0.SaveAs("test.png")
"""



