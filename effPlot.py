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
import math
from ROOT import *

def TGraph_To_TH1(hist_frame, graph):
  this_hist = hist_frame.Clone()
  Nbins = hist_frame.GetNbinsX()

  for i in range(Nbins+2):
    this_hist.SetBinContent(i, 0)
    this_hist.SetBinError(i, 0)

  for i in range(graph.GetN()):
    this_hist.SetBinContent(i+1,graph.GetY()[i])
    this_hist.SetBinError(i+1,math.sqrt(graph.GetY()[i]))

  #sanity check
  #print Nbins, graph.GetN()
  #if not Nbins == graph.GetN():
  #  print "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!ERROR!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
  #  return
  #print "original graph contents / converted hist contents"
  #for i in range(graph.GetN()):
  #  print graph.GetY()[i] / this_hist.GetBinContent(i+1)
  #  if not graph.GetY()[i] / this_hist.GetBinContent(i+1) == 1.:
  #    print "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!ERROR!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
  #    return

  return this_hist

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


parser = argparse.ArgumentParser()
parser.add_argument('-p', '--process', nargs='*', dest="proc", default=["DY","TT","Jpsi","Bs","DY4","QCD","Zprime"], help="physics process: DY, TT, Jpsi, Bs, DY4, QCD, Zprime")
parser.add_argument('-f', '--filter', nargs='*', dest="fltr", default=["WP","N"], help="filtering method: WP, N")
parser.add_argument('-m', '--measure', nargs='*', dest="measure", default=["trackeff","eff","purity","dR","profile","mva"], help="what to measure: trackeff, eff, purity, profile, mva")
parser.add_argument('-v', '--variable', nargs='*', dest="var", default=["pt","eta","phi","truePU"], help="measure as a function of: pt, eta, phi, truePU")
parser.add_argument('-d', '--denominator', nargs='*', dest="den", default=["hardP","hardP_L1"], help="denominator of efficiency: hardP, hardP_L1")
parser.add_argument('-n', '--numerator', nargs='*', dest="num", default=["IOFromL1","L3NoID","L3","OI","Iter0FromL2","Iter2FromL2","Iter0FromL1","Iter2FromL1", "L1"], help="numerator of efficiency or denominator of purity: L3NoID, L3, OI, Iter0(2)FromL2(1), L1") #L1 : for dR measurement
parser.add_argument('-t', '--trigger', nargs='*', dest="trig", default=["single","double"], help="which trigger to check: single(IsoMu24), double(Mu17Mu8)")
args = parser.parse_args()

gROOT.SetBatch(kTRUE)
#gROOT.SetBatch(kFALSE) #JH : if you need plot by plot check
#os.system('mkdir -p Winter21_new/')

#Xaxis title
Xtitle = {'pt' : "#scale[1.8]{p_{T}(#mu) [GeV]}", 'eta' : "#scale[1.8]{#eta(#mu)}", 'phi' : "#scale[1.8]{#varphi(#mu)}", 'truePU' : "#scale[1.8]{# of truePU}"}

#Yaxis title #TODO

#pt Xaxis range
Xrange = {'single' : [0., 200.], 'double' : [0., 200.], 'Jpsi' : [0., 200.], 'Bs' : [0., 200.], 'QCD' : [0., 55.], 'Gun' : [0., 1000.], 'Zprime' : [20., 3000.,],}
SetLogX = {'DY' : 0, 'TT' : 0, 'Jpsi' : 0, 'Bs' : 0, 'QCD' : 0, 'DY4' : 0, 'Gun' : 1, 'Zprime' : 1,}

#Graph colors
#graphColors = [kRed+1, kRed-2, kAzure+2, kAzure-1, kViolet+5]
graphColors = [kBlack, kGreen+2, kBlue, kRed]
markerStyles = {
                'eff' : [20, 22, 33, 34],
                'purity' : [20, 20, 20, 20],
               }
markerSizes = {
               'eff' : [1.75, 2., 2.3, 1.4],
               'purity' : [1, 1, 1, 1],
              }

#Other comments on the plot
txt_CMS = TText(0.1,0.905,"CMS simulation")
txt_CMS.SetNDC()
txt_L1pt = TLatex()
txt_mupt = TLatex()
txt_sample = TLatex()
txt_sample_setting = {
                      'Zprime' : [0.74,0.91,"#scale[0.8]{Zprime (14TeV)}"],
                      'DY'     : [0.78,0.91,"#scale[0.8]{DY (14TeV)}"],
                      'TT'     : [0.75,0.91,"#scale[0.8]{TTbar (14TeV)}"],
                      'Jpsi'   : [0.77,0.91,"#scale[0.8]{Jpsi (14TeV)}"],
                      'Bs'     : [0.78,0.91,"#scale[0.8]{Bs (14TeV)}"],
                      'Gun'    : [0.71,0.91,"#scale[0.8]{Muon Gun (14TeV)}"],
                     }
txt_sample_setting_onepad = {
                             'Zprime' : [0.73,0.91,"#scale[0.6]{Zprime (14TeV)}"],
                             'DY'     : [0.78,0.91,"#scale[0.6]{DY (14TeV)}"],
                             'TT'     : [0.74,0.91,"#scale[0.6]{TTbar (14TeV)}"],
                             'Jpsi'   : [0.76,0.91,"#scale[0.6]{Jpsi (14TeV)}"],
                             'Bs'     : [0.78,0.91,"#scale[0.6]{Bs (14TeV)}"],
                             'Gun'    : [0.69,0.91,"#scale[0.6]{Muon Gun (14TeV)}"],
                            }

for this_proc, this_fltr in [(this_proc,this_fltr) for this_proc in args.proc for this_fltr in args.fltr]:

  #Call the input root files
  if this_fltr == "WP":
    this_files = [
                  "calculateEff_result_newModel/Run3_Winter21_"+this_proc+"_WP00.root",
                  "calculateEff_result_newModel/Run3_Winter21_"+this_proc+"_WP02.root",
                  "calculateEff_result_newModel/Run3_Winter21_"+this_proc+"_WP04.root",
                  "calculateEff_result_newModel/Run3_Winter21_"+this_proc+"_WP10.root",
                 ]
  elif this_fltr == "N":
    this_files = ["result_"+this_proc+"_WP00.root","result_"+this_proc+"_N50.root","result_"+this_proc+"_N10.root","result_"+this_proc+"_N5.root"]
    #this_files = ["Run3_result_"+this_proc+"_WP00.root","Run3_result_"+this_proc+"_N50.root","Run3_result_"+this_proc+"_N10.root","Run3_result_"+this_proc+"_N5.root","Run3_result_"+this_proc+"_N0.root"]
  
  files = [TFile.Open(this_files[i]) for i in range(len(this_files))]
  
  for this_measure, this_var, this_den, this_num, this_trig in [(this_measure,this_var,this_den,this_num,this_trig) for this_measure in args.measure for this_var in args.var for this_den in args.den for this_num in args.num for this_trig in args.trig]:

    #Call the target histograms
    if this_trig == "single" or (this_trig == "double" and this_var == "pt"):
      this_trigLabel = ""
    elif this_trig == "double":
      this_trigLabel = "_med"
    
    if 'L1' in this_den:
      this_den_vars = [files[i].Get(this_den+this_trigLabel+"_"+this_var) for i in range(len(files))]
      this_den_var_eff_nums = [files[i].Get(this_den+this_trigLabel+"_"+this_var+"_eff_"+this_num) for i in range(len(files))]
    else:
      this_den_vars = [files[i].Get(this_den+"_"+this_var) for i in range(len(files))]
      this_den_var_eff_nums = [files[i].Get(this_den+"_"+this_var+"_eff_"+this_num) for i in range(len(files))]
    this_num_vars = [files[i].Get(this_num+"_"+this_var) for i in range(len(files))]
    #this_num_var_puritys = [files[i].Get(this_num+"_"+this_var+"_purity") for i in range(len(files))] #JH : dR matching
    this_num_var_puritys = [files[i].Get(this_num+"_"+this_var+"_Asso") for i in range(len(files))] #JH : hit association

    #Set the legend name
    if this_fltr == "WP":
      lg_names = ["WP 0","WP 0.02","WP 0.04","WP 0.10"]
    elif this_fltr == "N":
      #lg_names = ["Nocut","N50","N10","N5"]
      lg_names = ["Nocut","N50","N10","N5","N0"]
    
    #Bins
    ptval = array.array('d', [5, 10, 15, 20, 25, 30, 40, 50, 60, 120, 200]) #JH : Avoid starting from 0. hardP_L1_pt histo starts with pt 8 --> ZeroDivisionError
    if this_measure == "purity":
      ptval = array.array('d', [5, 10, 15, 20, 25, 30, 40, 50, 60, 120, 200]) #JH : To adjust y range (relatively low purity at low pt makes plot range inconsistent)
    if this_proc == "Bs":
      ptval = array.array('d', [5, 10, 15, 20, 25, 30, 40, 50, 60, 120, 200]) #JH
    if this_proc == "Jpsi":
      ptval = array.array('d', [5, 10, 15, 20, 25, 30, 40, 50, 60, 120]) #JH
    if this_proc == "Gun":
      ptval = array.array('d', [5, 10, 15, 20, 25, 30, 40, 50, 60, 120, 200, 300, 500, 700, 1000]) #JH
    if this_proc == "Zprime":
      ptval = array.array('d', [20, 25, 30, 40, 50, 60, 120, 200, 300, 500, 700, 1000, 1500, 2000, 2500, 3000]) #JH
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
      if this_num == "L1" : continue

      den_vars = [this_den_vars[i].Rebin(len(vals[this_var])-1,'',vals[this_var]) for i in range(len(this_den_vars))]
      den_var_eff_nums = [this_den_var_eff_nums[i].Rebin(len(vals[this_var])-1,'',vals[this_var]) for i in range(len(this_den_var_eff_nums))]
      #print "current pt binning :", ptval
      #for i in range(den_var_eff_nums[0].GetNbinsX()+2):
      #  print i,"th num bin center :", den_var_eff_nums[0].GetBinCenter(i),", content :", den_var_eff_nums[0].GetBinContent(i)
      #for i in range(den_vars[0].GetNbinsX()+2):
      #  print i,"th WP00 den bin center :", den_vars[0].GetBinCenter(i),", content :", den_vars[0].GetBinContent(i)
      #for i in range(den_vars[1].GetNbinsX()+2):
      #  print i,"th WP02 den bin center :", den_vars[1].GetBinCenter(i),", content :", den_vars[1].GetBinContent(i)
      #for i in range(den_vars[2].GetNbinsX()+2):
      #  print i,"th WP04 den bin center :", den_vars[2].GetBinCenter(i),", content :", den_vars[2].GetBinContent(i)
      #for i in range(den_vars[3].GetNbinsX()+2):
      #  print i,"th WP10 den bin center :", den_vars[3].GetBinCenter(i),", content :", den_vars[3].GetBinContent(i)

      #### we know the origin of the efficiency increasing. (sometimes fake tracks are chosen while merging)

    ######First, check the denominators are consistent each other among the WPs.
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
        gr_den_var.SetMarkerStyle(markerStyles['eff'][igr])
        gr_den_var.SetMarkerSize(markerSizes['eff'][igr])
        gr_den_var.SetMarkerColor(graphColors[igr])
        gr_den_var.SetLineColor(graphColors[igr])
        gr_den_var.SetLineWidth(3)

        if igr == len(den_vars)-1:
          option_gr = "AP"

          gr_den_var.SetTitle("")
          if this_var == "pt": #JH : You need this line, because leaving other variable bins as default is clean.
            gr_den_var.GetXaxis().SetRangeUser(Xrange[this_trig][0],Xrange[this_trig][1]) #JH : Xrange['single'] or Xrange['double']
            if this_proc == "Gun" or this_proc == "Zprime":
              gr_den_var.GetXaxis().SetRangeUser(Xrange[this_proc][0],Xrange[this_proc][1]) #JH
              gPad.SetLogx()
          gr_den_var.GetXaxis().SetTickLength(0.025)
          gr_den_var.GetXaxis().SetLabelSize(0.)
          gr_den_var.GetYaxis().SetLabelSize(0.025)
          gr_den_var.GetYaxis().SetTitle("Denominators")
          yran1 = TMath.MinElement(len(gr_den_var.GetY()),gr_den_var.GetY())*0.9 #JH : You cannot use gr.GetMinimum() or gr.GetMaximum. see https://root-forum.cern.ch/t/tgraph-getmaximum-getminimum/8867/2
          yran2 = TMath.MaxElement(len(gr_den_var.GetY()),gr_den_var.GetY())*1.1
          gr_den_var.GetYaxis().SetRangeUser(yran1,yran2)
          
        gr_den_var.Draw(option_gr)
        gr_den_vars = [gr_den_var] + gr_den_vars

        lg_den_var.AddEntry(gr_den_var,lg_names[igr])

      lg_den_var.Draw()
      
      txt_CMS.Draw()
      txt_sample.DrawLatexNDC(txt_sample_setting[this_proc][0],txt_sample_setting[this_proc][1],txt_sample_setting[this_proc][2])
      if "L1" in this_den:
        if this_trig == "single":
          txt_L1pt.DrawLatexNDC(0.12,0.86,"#scale[0.8]{gen #mu matched with p_{T}(L1 #mu) > 22GeV}")
          if this_var != "pt":
            txt_mupt.DrawLatexNDC(0.12,0.82,"#scale[0.8]{p_{T}(#mu) > 26GeV}")
        if this_trig == "double":
          txt_L1pt.DrawLatexNDC(0.12,0.86,"#scale[0.8]{gen #mu matched with p_{T}(L1 #mu) > 8GeV}")
          if this_var != "pt":
            txt_mupt.DrawLatexNDC(0.12,0.82,"#scale[0.8]{p_{T}(#mu) > 10GeV}")
      else:
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

        gr_ratio_den_var.SetMarkerStyle(markerStyles['eff'][irat])
        gr_ratio_den_var.SetMarkerSize(markerSizes['eff'][irat])
        gr_ratio_den_var.SetMarkerColor(graphColors[irat])
        gr_ratio_den_var.SetLineColor(graphColors[irat])
        gr_ratio_den_var.SetLineWidth(3)

        if irat == len(den_vars)-1:
          option_rat = "AP"

          gr_ratio_den_var.SetTitle("")
          if this_var == "pt":
            gr_ratio_den_var.GetXaxis().SetRangeUser(Xrange[this_trig][0],Xrange[this_trig][1]) #JH : Xrange['single'] or Xrange['double']
            if this_proc == "Gun" or this_proc == "Zprime":
              gr_ratio_den_var.GetXaxis().SetRangeUser(Xrange[this_proc][0],Xrange[this_proc][1]) #JH
              gPad.SetLogx()
          gr_ratio_den_var.GetXaxis().SetTitle(this_Xtitle)
          gr_ratio_den_var.GetXaxis().SetTitleOffset(1.7)
          gr_ratio_den_var.GetXaxis().SetTickLength(0.05)
          gr_ratio_den_var.GetXaxis().SetLabelSize(0.06)
          gr_ratio_den_var.GetYaxis().SetLabelSize(0.05)
          gr_ratio_den_var.GetYaxis().SetTitle("#scale[1.8]{Ratio to Nocut}")
          gr_ratio_den_var.GetYaxis().SetTitleOffset(0.8)
          #yran1 = TMath.MinElement(len(gr_ratio_den_var.GetY()),gr_ratio_den_var.GetY())*0.999 #JH
          #yran2 = TMath.MaxElement(len(gr_ratio_den_var.GetY()),gr_ratio_den_var.GetY())*1.001 #JH
          yran1 = 0.8 #JH
          yran2 = 1.2 #JH
          gr_ratio_den_var.GetYaxis().SetRangeUser(yran1,yran2)

        gr_ratio_den_var.Draw(option_rat)
        gr_ratio_den_vars.append(gr_ratio_den_var) #JH : I really don't know why but this appending prevent the bug (???)

      os.system("mkdir -p Winter21_new/"+this_proc+"/eff_den_valid/")
      c_den_var.SaveAs("Winter21_new/"+this_proc+"/eff_den_valid/"+this_proc+"_"+this_fltr+"_"+this_den+this_trigLabel+"_"+this_var+".png")


    ######Then, check whether the numerators also show the efficiency-increasing behavior.
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
        gr_den_var_num.SetMarkerStyle(markerStyles['eff'][igr])
        gr_den_var_num.SetMarkerSize(markerSizes['eff'][igr])
        gr_den_var_num.SetMarkerColor(graphColors[igr])
        gr_den_var_num.SetLineColor(graphColors[igr])
        gr_den_var_num.SetLineWidth(3)

        if igr == len(den_var_eff_nums)-1:
          option_gr = "AP"

          gr_den_var_num.SetTitle("")
          if this_var == "pt": #JH : You need this line, because I want to customize ONLY pt bins, not other bins.
            gr_den_var_num.GetXaxis().SetRangeUser(Xrange[this_trig][0],Xrange[this_trig][1]) #JH : Xrange['single'] or Xrange['double']
            if this_proc == "Gun" or this_proc == "Zprime":
              gr_den_var_num.GetXaxis().SetRangeUser(Xrange[this_proc][0],Xrange[this_proc][1]) #JH
              gPad.SetLogx()
          gr_den_var_num.GetXaxis().SetTickLength(0.025)
          gr_den_var_num.GetXaxis().SetLabelSize(0.)
          gr_den_var_num.GetYaxis().SetLabelSize(0.025)
          gr_den_var_num.GetYaxis().SetTitle("Numerators")
          yran1 = TMath.MinElement(len(gr_den_var_num.GetY()),gr_den_var_num.GetY())*0.9 #JH : You cannot use gr.GetMinimum() or gr.GetMaximum. see https://root-forum.cern.ch/t/tgraph-getmaximum-getminimum/8867/2
          yran2 = TMath.MaxElement(len(gr_den_var_num.GetY()),gr_den_var_num.GetY())*1.2
          gr_den_var_num.GetYaxis().SetRangeUser(yran1,yran2) #JH
          
        gr_den_var_num.Draw(option_gr)
        gr_den_var_nums = [gr_den_var_num] + gr_den_var_nums

        lg_den_var_num.AddEntry(gr_den_var_num,lg_names[igr])

      lg_den_var_num.Draw()
      
      txt_CMS.Draw()
      txt_sample.DrawLatexNDC(txt_sample_setting[this_proc][0],txt_sample_setting[this_proc][1],txt_sample_setting[this_proc][2])
      if this_proc == "DY" or this_proc == "TT" or this_proc == "Zprime":
        if this_den == "hardP_L1":
          if this_trig == "single":
            txt_L1pt.DrawLatexNDC(0.12,0.86,"#scale[0.8]{gen #mu matched with p_{T}(L1 #mu) > 22GeV}")
            if this_var != "pt":
              txt_mupt.DrawLatexNDC(0.12,0.82,"#scale[0.8]{p_{T}(#mu) > 26GeV}")
          if this_trig == "double":
            txt_L1pt.DrawLatexNDC(0.12,0.86,"#scale[0.8]{gen #mu matched with p_{T}(L1 #mu) > 8GeV}")
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
        gr_ratio_den_var_num.Divide(den_var_eff_nums[irat],den_var_eff_nums[0],"pois") #JH : "pois" interpret two histos as independent, not the efficiency. (which will return "<TROOT::TEfficiency::CheckEntries>: Histograms are not consistent: passed bin content > total bin content" error if >1. see https://root.cern.ch/doc/master/classTGraphAsymmErrors.html#a78cd209f4da9a169848ab23f539e1c94)
       
        gr_ratio_den_var_num.SetMarkerStyle(markerStyles['eff'][irat])
        gr_ratio_den_var_num.SetMarkerSize(markerSizes['eff'][irat])
        gr_ratio_den_var_num.SetMarkerColor(graphColors[irat])
        gr_ratio_den_var_num.SetLineColor(graphColors[irat])
        gr_ratio_den_var_num.SetLineWidth(3)

        if irat == len(den_var_eff_nums)-1:
          option_rat = "AP"

          gr_ratio_den_var_num.SetTitle("")
          if this_var == "pt":
            gr_ratio_den_var_num.GetXaxis().SetRangeUser(Xrange[this_trig][0],Xrange[this_trig][1]) #JH : Xrange['single'] or Xrange['double']
            if this_proc == "Gun" or this_proc == "Zprime":
              gr_ratio_den_var_num.GetXaxis().SetRangeUser(Xrange[this_proc][0],Xrange[this_proc][1]) #JH
              gPad.SetLogx()
          gr_ratio_den_var_num.GetXaxis().SetTitle(this_Xtitle)
          gr_ratio_den_var_num.GetXaxis().SetTitleOffset(1.7)
          gr_ratio_den_var_num.GetXaxis().SetTickLength(0.05)
          gr_ratio_den_var_num.GetXaxis().SetLabelSize(0.06)
          gr_ratio_den_var_num.GetYaxis().SetLabelSize(0.05)
          gr_ratio_den_var_num.GetYaxis().SetTitle("#scale[1.8]{Ratio to Nocut}")
          gr_ratio_den_var_num.GetYaxis().SetTitleOffset(0.8)
          if "Iter2" in this_num:
            yran1 = TMath.MinElement(len(gr_ratio_den_var_num.GetY()),gr_ratio_den_var_num.GetY())*0.95 #JH
            yran2 = TMath.MaxElement(len(gr_ratio_den_var_num.GetY()),gr_ratio_den_var_num.GetY())*2. #JH : Iter2 eff drop
            gr_ratio_den_var_num.GetYaxis().SetRangeUser(yran1,yran2) #JH
          else:
            #gr_ratio_den_var_num.GetYaxis().SetRangeUser(0.999,1.001) #JH
            gr_ratio_den_var_num.GetYaxis().SetRangeUser(0.8,1.2) #JH

        gr_ratio_den_var_num.Draw(option_rat)
        gr_ratio_den_var_nums.append(gr_ratio_den_var_num) #JH : I really don't know why but this appending prevent the bug (???)

      os.system("mkdir -p Winter21_new/"+this_proc+"/eff_num_valid/")
      c_den_var_num.SaveAs("Winter21_new/"+this_proc+"/eff_num_valid/"+this_proc+"_"+this_fltr+"_"+this_den+this_trigLabel+"_"+this_var+"_"+this_num+".png")

      #"""

    ######Now start to calculate efficiency

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

      #for igr in reversed(range(len(this_den_vars))):
      for igr in range(len(this_den_vars)): #XXX ORDER TEST
        option_gr = "P SAME"

        gr_den_var_eff_num = TGraphAsymmErrors(len(vals[this_var])-1)
        gr_den_var_eff_num.Divide(den_var_eff_nums[igr],den_vars[igr],"n e0")
        gr_den_var_eff_num.SetMarkerStyle(markerStyles['eff'][igr])
        gr_den_var_eff_num.SetMarkerSize(markerSizes['eff'][igr])
        gr_den_var_eff_num.SetMarkerColor(graphColors[igr])
        gr_den_var_eff_num.SetLineColor(graphColors[igr])
        gr_den_var_eff_num.SetLineWidth(2)

        #if igr == len(this_den_vars)-1:
        if igr == 0: #XXX ORDER TEST
          option_gr = "AP"

          gr_den_var_eff_num.SetTitle("")
          if this_var == "pt": #JH : You need this line, because I want to customize ONLY pt bins, not other bins.
            gr_den_var_eff_num.GetXaxis().SetRangeUser(Xrange[this_trig][0],Xrange[this_trig][1]) #JH : Xrange['single'] or Xrange['double']
            if this_proc == "Gun" or this_proc == "Zprime":
              gr_den_var_eff_num.GetXaxis().SetRangeUser(Xrange[this_proc][0],Xrange[this_proc][1]) #JH
              gPad.SetLogx()
          gr_den_var_eff_num.GetXaxis().SetTickLength(0.025)
          gr_den_var_eff_num.GetXaxis().SetLabelSize(0.)
          gr_den_var_eff_num.GetYaxis().SetLabelSize(0.025)
          gr_den_var_eff_num.GetYaxis().SetTitle("Efficiency")
          yran1 = TMath.MinElement(len(gr_den_var_eff_num.GetY()),gr_den_var_eff_num.GetY())*0.98 #JH : You cannot use gr.GetMinimum() or gr.GetMaximum. see https://root-forum.cern.ch/t/tgraph-getmaximum-getminimum/8867/2
          yran2 = TMath.MaxElement(len(gr_den_var_eff_num.GetY()),gr_den_var_eff_num.GetY())*1.02
          if this_proc == "Jpsi":
            if this_fltr == "N":
              yran2 = TMath.MaxElement(len(gr_den_var_eff_num.GetY()),gr_den_var_eff_num.GetY())*1.05
          if "Iter2" in this_num:
            yran1 = TMath.MinElement(len(gr_den_var_eff_num.GetY()),gr_den_var_eff_num.GetY())*0.95 #JH : You cannot use gr.GetMinimum() or gr.GetMaximum. see https://root-forum.cern.ch/t/tgraph-getmaximum-getminimum/8867/2
            yran2 = TMath.MaxElement(len(gr_den_var_eff_num.GetY()),gr_den_var_eff_num.GetY())*2. #JH : Iter2 eff drop

          gr_den_var_eff_num.GetYaxis().SetRangeUser(yran1,yran2) #JH : TODO
          
        gr_den_var_eff_num.Draw(option_gr)
        #gr_den_var_eff_nums = [gr_den_var_eff_num] + gr_den_var_eff_nums
        gr_den_var_eff_nums = gr_den_var_eff_nums + [gr_den_var_eff_num] #ORDER TEST

        lg_den_var_eff_num.AddEntry(gr_den_var_eff_num,lg_names[igr])

      lg_den_var_eff_num.Draw()
      
      txt_CMS.Draw()
      txt_sample.DrawLatexNDC(txt_sample_setting[this_proc][0],txt_sample_setting[this_proc][1],txt_sample_setting[this_proc][2])

      if "L1" in this_den:
        if this_trig == "single":
          txt_L1pt.DrawLatexNDC(0.12,0.86,"#scale[0.8]{gen #mu matched with p_{T}(L1 #mu) > 22GeV}")
          if this_var != "pt":
            txt_mupt.DrawLatexNDC(0.12,0.82,"#scale[0.8]{p_{T}(#mu) > 26GeV}")
        if this_trig == "double":
          txt_L1pt.DrawLatexNDC(0.12,0.86,"#scale[0.8]{gen #mu matched with p_{T}(L1 #mu) > 8GeV}")
          if this_var != "pt":
            txt_mupt.DrawLatexNDC(0.12,0.82,"#scale[0.8]{p_{T}(#mu) > 10GeV}")
      elif this_den == "hardP" or this_den == "TP":
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

      #for irat in reversed(range(len(this_den_vars))):
      for irat in range(len(this_den_vars)): #XXX ORDER TEST
        option_rat = "P SAME"

        #JH : Use below if you want to use Divide (exploit various error type). but meaningless if ratio > 1. Then you can only use "pois" error, not the other errors.

        #hist_den_var_eff_nums = []
        #for i in range(len(gr_den_var_eff_nums)):
        #  hist_den_var_eff_nums.append(TGraph_To_TH1(den_var_eff_nums[0], gr_den_var_eff_nums[i]))

        #print irat,"th WP;"
        #print "N of graph points :", len(vals[this_var])-1
        #print "N of histo bins :", hist_den_var_eff_nums[0].GetNbinsX()
        #gr_ratio_den_var_eff_num = TGraphAsymmErrors(len(vals[this_var])-1)
        #gr_ratio_den_var_eff_num.Divide(hist_den_var_eff_nums[irat],hist_den_var_eff_nums[0],"poisv") #JH : "pois" interpret two histos as independent, not the efficiency. (which will return "<TROOT::TEfficiency::CheckEntries>: Histograms are not consistent: passed bin content > total bin content" error if >1. see https://root.cern.ch/doc/master/classTGraphAsymmErrors.html#a78cd209f4da9a169848ab23f539e1c94) ; v == verbose

        gr_ratio_den_var_eff_num = TGraphErrors(len(vals[this_var])-1)
        for i in range(gr_den_var_eff_nums[0].GetN()):
          #print i,"th x :",gr_den_var_eff_nums[0].GetX()[i],", y :",gr_den_var_eff_nums[0].GetY()[i] #JH
          num_tmp = gr_den_var_eff_nums[irat].GetY()[i]
          num_err_tmp = gr_den_var_eff_nums[irat].GetErrorYhigh(i) if gr_den_var_eff_nums[irat].GetErrorYhigh(i) > gr_den_var_eff_nums[irat].GetErrorYlow(i) else gr_den_var_eff_nums[irat].GetErrorYlow(i)
          den_tmp = gr_den_var_eff_nums[0].GetY()[i]
          den_err_tmp = gr_den_var_eff_nums[0].GetErrorYhigh(i) if gr_den_var_eff_nums[0].GetErrorYhigh(i) > gr_den_var_eff_nums[0].GetErrorYlow(i) else gr_den_var_eff_nums[0].GetErrorYlow(i)
          eff_ratio_tmp = num_tmp/den_tmp if den_tmp > 0. else 0.
          eff_ratio_err_tmp = eff_ratio_tmp * math.sqrt(pow(num_err_tmp/num_tmp,2) + pow(den_err_tmp/den_tmp,2))
          try:
            gr_ratio_den_var_eff_num.SetPoint(i,gr_den_var_eff_nums[0].GetX()[i],eff_ratio_tmp)
            gr_ratio_den_var_eff_num.SetPointError(i,(vals[this_var][i+1]-vals[this_var][i])/2,eff_ratio_err_tmp)
          except:
            pass
        
        gr_ratio_den_var_eff_num.SetMarkerStyle(markerStyles['eff'][irat])
        gr_ratio_den_var_eff_num.SetMarkerSize(markerSizes['eff'][irat])
        gr_ratio_den_var_eff_num.SetMarkerColor(graphColors[irat])
        gr_ratio_den_var_eff_num.SetLineColor(graphColors[irat])
        gr_ratio_den_var_eff_num.SetLineWidth(2)

        #if irat == len(this_den_vars)-1:
        if irat == 0: #XXX ORDER TEST
          option_rat = "AP"

          gr_ratio_den_var_eff_num.SetTitle("")
          if this_var == "pt":
            gr_ratio_den_var_eff_num.GetXaxis().SetRangeUser(Xrange[this_trig][0],Xrange[this_trig][1]) #JH : Xrange['single'] or Xrange['double']
            if this_proc == "Gun" or this_proc == "Zprime":
              gr_ratio_den_var_eff_num.GetXaxis().SetRangeUser(Xrange[this_proc][0],Xrange[this_proc][1]) #JH
              gPad.SetLogx()
          gr_ratio_den_var_eff_num.GetXaxis().SetTitle(this_Xtitle) #JH : TODO
          gr_ratio_den_var_eff_num.GetXaxis().SetTitleOffset(1.7)
          gr_ratio_den_var_eff_num.GetXaxis().SetTickLength(0.05)
          gr_ratio_den_var_eff_num.GetXaxis().SetLabelSize(0.06)
          gr_ratio_den_var_eff_num.GetYaxis().SetLabelSize(0.05)
          gr_ratio_den_var_eff_num.GetYaxis().SetTitle("#scale[1.8]{Ratio to Nocut}")
          gr_ratio_den_var_eff_num.GetYaxis().SetTitleOffset(0.8)
          #print gr_ratio_den_var_eff_num.GetY()
          yran1 = TMath.MinElement(len(gr_ratio_den_var_eff_num.GetY()),gr_ratio_den_var_eff_num.GetY())*0.98 #JH : TODO
          yran2 = TMath.MaxElement(len(gr_ratio_den_var_eff_num.GetY()),gr_ratio_den_var_eff_num.GetY())*1.02 #JH : TODO
          if this_proc == "Jpsi":
            if this_fltr == "N":
              yran2 = TMath.MaxElement(len(gr_ratio_den_var_eff_num.GetY()),gr_ratio_den_var_eff_num.GetY())*1.05 #JH : TODO
          if "Iter2" in this_num:
            yran1 = TMath.MinElement(len(gr_ratio_den_var_eff_num.GetY()),gr_ratio_den_var_eff_num.GetY())*0.95 #JH : You cannot use gr.GetMinimum() or gr.GetMaximum. see https://root-forum.cern.ch/t/tgraph-getmaximum-getminimum/8867/2
            yran2 = TMath.MaxElement(len(gr_ratio_den_var_eff_num.GetY()),gr_ratio_den_var_eff_num.GetY())*2. #JH : Iter2 eff drop

          gr_ratio_den_var_eff_num.GetYaxis().SetRangeUser(yran1,yran2) #JH : TODO
          #gr_ratio_den_var_eff_num.GetYaxis().SetRangeUser(0.9,1.1) #JH : TODO

        gr_ratio_den_var_eff_num.Draw(option_rat)
        gr_ratio_den_var_eff_nums.append(gr_ratio_den_var_eff_num) #JH : I really don't know why but this appending prevent the bug (???)

      os.system("mkdir -p Winter21_new/"+this_proc+"/eff/") #JH : ERROR this makes error. why????
      c_den_var_eff_num.SaveAs("Winter21_new/"+this_proc+"/eff/"+this_proc+"_"+this_fltr+"_"+this_den+this_trigLabel+"_"+this_var+"_eff_"+this_num+".png")

   
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
      elif (this_proc == "Zprime" or this_proc == "Gun") and this_var == "pt":
        lg_num_var_purity = TLegend(0.7,0.15,0.9,0.4) #JH : TODO
      else:
        lg_num_var_purity = TLegend(0.7,0.7,0.9,0.9) #JH : TODO

      gr_num_var_puritys = []

      for igr in reversed(range(len(this_num_vars))):
      #for igr in [4,3,0]: #JH : to confirm N0 and N5 are really overlapped or just a bug
        option_gr = "P SAME"

        gr_num_var_purity = TGraphAsymmErrors(len(vals[this_var])-1)
        gr_num_var_purity.Divide(num_var_puritys[igr],num_vars[igr],"n e0")
        gr_num_var_purity.SetMarkerStyle(markerStyles['purity'][igr])
        gr_num_var_purity.SetMarkerSize(markerSizes['purity'][igr])
        gr_num_var_purity.SetMarkerColor(graphColors[igr])
        gr_num_var_purity.SetLineColor(graphColors[igr])
        gr_num_var_purity.SetLineWidth(2)

        if igr == len(this_num_vars)-1:
          option_gr = "AP"

          gr_num_var_purity.SetTitle("")
          if (this_proc == "Zprime" or this_proc == "Gun") and this_var == "pt":
            gr_num_var_purity.GetXaxis().SetRangeUser(Xrange[this_proc][0],Xrange[this_proc][1]) #JH
            gPad.SetLogx()
          gr_num_var_purity.GetXaxis().SetTickLength(0.025)
          gr_num_var_purity.GetXaxis().SetLabelSize(0.)
          gr_num_var_purity.GetYaxis().SetLabelSize(0.025)
          gr_num_var_purity.GetYaxis().SetTitle("Purity")
          #if this_var == "pt":
          #  if this_proc == "Jpsi" or this_proc == "QCD":
          #    gr_num_var_purity.GetXaxis().SetRangeUser(Xrange[this_proc][0],Xrange[this_proc][1]) #JH
          yran1 = TMath.MinElement(len(gr_num_var_purity.GetY()),gr_num_var_purity.GetY())*0.7 #JH : TODO
          if "FromL1" in this_num:
            yran1 = 0 #JH : TODO
          #if this_proc == "Jpsi":
          #  if this_var == "pt" or this_var == "phi":
          #    yran1 = TMath.MinElement(len(gr_num_var_purity.GetY()),gr_num_var_purity.GetY())*0.7 #JH : TODO
          yran2 = TMath.MaxElement(len(gr_num_var_purity.GetY()),gr_num_var_purity.GetY())*1.1 #JH : TODO
          #if this_var == "pt" or this_var == "phi":
          #  yran1 = TMath.MinElement(len(gr_num_var_purity.GetY()),gr_num_var_purity.GetY())*0.9 #JH
          gr_num_var_purity.GetYaxis().SetRangeUser(yran1,yran2) #JH : TODO

        gr_num_var_purity.Draw(option_gr)
        gr_num_var_puritys = [gr_num_var_purity] + gr_num_var_puritys

        lg_num_var_purity.AddEntry(gr_num_var_purity,lg_names[igr])
      
      lg_num_var_purity.Draw()
      
      txt_CMS.Draw()
      txt_sample.DrawLatexNDC(txt_sample_setting[this_proc][0],txt_sample_setting[this_proc][1],txt_sample_setting[this_proc][2])
      
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
          num_tmp = gr_num_var_puritys[irat].GetY()[i]
          num_err_tmp = gr_num_var_puritys[irat].GetErrorYhigh(i) if gr_num_var_puritys[irat].GetErrorYhigh(i) > gr_num_var_puritys[irat].GetErrorYlow(i) else gr_num_var_puritys[irat].GetErrorYlow(i)
          den_tmp = gr_num_var_puritys[0].GetY()[i]
          den_err_tmp = gr_num_var_puritys[0].GetErrorYhigh(i) if gr_num_var_puritys[0].GetErrorYhigh(i) > gr_num_var_puritys[0].GetErrorYlow(i) else gr_num_var_puritys[0].GetErrorYlow(i)
          pur_ratio_tmp = num_tmp/den_tmp
          pur_ratio_err_tmp = pur_ratio_tmp * math.sqrt(pow(num_err_tmp/num_tmp,2) + pow(den_err_tmp/den_tmp,2))
          gr_ratio_num_var_purity.SetPoint(i,gr_num_var_puritys[0].GetX()[i],pur_ratio_tmp)
          gr_ratio_num_var_purity.SetPointError(i,(vals[this_var][i+1]-vals[this_var][i])/2,pur_ratio_err_tmp)
        
        gr_ratio_num_var_purity.SetMarkerStyle(markerStyles['purity'][irat])
        gr_ratio_num_var_purity.SetMarkerSize(markerSizes['purity'][irat])
        gr_ratio_num_var_purity.SetMarkerColor(graphColors[irat])
        gr_ratio_num_var_purity.SetLineColor(graphColors[irat])
        gr_ratio_num_var_purity.SetLineWidth(2)

        if irat == len(this_num_vars)-1:
        #if irat == 2: #JH : to confirm N0 and N5 are really overlapped or just a bug
          option_rat = "AP"

          gr_ratio_num_var_purity.SetTitle("")
          if (this_proc == "Zprime" or this_proc == "Gun") and this_var == "pt":
            gr_ratio_num_var_purity.GetXaxis().SetRangeUser(Xrange[this_proc][0],Xrange[this_proc][1]) #JH
            gPad.SetLogx()
          gr_ratio_num_var_purity.GetXaxis().SetTitle(this_Xtitle) #JH : TODO
          gr_ratio_num_var_purity.GetXaxis().SetTitleOffset(1.7)
          gr_ratio_num_var_purity.GetXaxis().SetTickLength(0.05)
          gr_ratio_num_var_purity.GetXaxis().SetLabelSize(0.06)
          gr_ratio_num_var_purity.GetYaxis().SetLabelSize(0.05)
          gr_ratio_num_var_purity.GetYaxis().SetTitle("#scale[1.8]{Ratio to Nocut}")
          gr_ratio_num_var_purity.GetYaxis().SetTitleOffset(0.8)
          #if this_var == "pt":
          #  if this_proc == "Jpsi" or this_proc == "QCD":
          #    gr_ratio_num_var_purity.GetXaxis().SetRangeUser(Xrange[this_proc][0],Xrange[this_proc][1]) #JH
          yran1 = TMath.MinElement(len(gr_ratio_num_var_purity.GetY()),gr_ratio_num_var_purity.GetY())*0.85 #JH : TODO
          if "FromL1" in this_num:
            yran1 = 0 #JH : TODO
          #if this_proc == "Jpsi":
          #  if this_var == "pt" or this_var == "phi":
          #    yran1 = TMath.MinElement(len(gr_ratio_num_var_purity.GetY()),gr_ratio_num_var_purity.GetY())*0.85 #JH : TODO
          yran2 = TMath.MaxElement(len(gr_ratio_num_var_purity.GetY()),gr_ratio_num_var_purity.GetY())*1.1 #JH : TODO
          #if this_var == "phi":
          #  yran1 = TMath.MinElement(len(gr_num_var_purity_WP3.GetY()),gr_num_var_purity_WP3.GetY())*0.948 #JH
          gr_ratio_num_var_purity.GetYaxis().SetRangeUser(yran1,yran2) #JH : TODO
          
        gr_ratio_num_var_purity.Draw(option_rat)
        gr_ratio_num_var_puritys.append(gr_ratio_num_var_purity)

      os.system("mkdir -p Winter21_new/"+this_proc+"/purity/")
      c_num_var_purity.SaveAs("Winter21_new/"+this_proc+"/purity/"+this_proc+"_"+this_fltr+"_"+this_num+"_"+this_var+"_purity.png")


########dR########
    if this_measure == "dR":

      if "L1" in this_den : continue #JH : only hardP supported
      if not "pt" in this_var : continue #JH : only dR vs pt, ptres supported

      if "L1" in this_num and not "Iter" in this_num:
        AtVtx_iter = ["","AtVtx_"]
      else:
        AtVtx_iter = [""]
        if "med" in this_trigLabel : continue #JH : do not re-run for single and double. The rebinning will be doubled.

      for AtVtx in AtVtx_iter:
        ####Call 2D histos...####
        print "Getting",this_den+"_"+this_var+"_dR_"+this_num+this_trigLabel+"_"+AtVtx+"2D","..."
        this_den_var_dR_nums_2D = [files[i].Get(this_den+"_"+this_var+"_dR_"+this_num+this_trigLabel+"_"+AtVtx+"2D") for i in range(len(files))]
        #den_var_dR_nums_2D = [this_den_var_dR_nums_2D[i].Rebin2D(10,10) for i in range(len(this_den_var_dR_nums_2D))] #JH : variable binning not supported for TH2
        #den_var_dR_nums_2D = [this_den_var_dR_nums_2D[i].RebinX(10) for i in range(len(this_den_var_dR_nums_2D))] #JH : variable binning not supported for TH2
        den_var_dR_nums_2D = [this_den_var_dR_nums_2D[i] for i in range(len(this_den_var_dR_nums_2D))] #JH : variable binning not supported for TH2
        ####Call profiles...####
        print "Also getting",this_den+"_"+this_var+"_dR_"+this_num+this_trigLabel+"_"+AtVtx+"pf","..."
        this_den_var_dR_nums_pf = [files[i].Get(this_den+"_"+this_var+"_dR_"+this_num+this_trigLabel+"_"+AtVtx+"pf") for i in range(len(files))]
        #den_var_dR_nums_pf = [this_den_var_dR_nums_pf[i].Rebin(len(vals["pt"])-1,'',vals["pt"]) for i in range(len(this_den_var_dR_nums_pf))] #JH : variable binning not supported for TH2
        den_var_dR_nums_pf = [this_den_var_dR_nums_pf[i].Rebin(2) for i in range(len(this_den_var_dR_nums_pf))] #JH : variable binning not supported for TH2

        ####Draw 2D histo####
        for i in range(len(files)):

          if "WP" in this_files[i]:
            this_WP = this_files[i][-9:-5] #JH : WPXX
          elif "_N" in this_files[i]:
            this_WP = this_files[i].split('.')[-2].split('_')[-1] #JH : N* ....

          #os.system("mkdir -p test/dR/"+this_proc+"/"+this_WP+"/logY")
          #os.system("mkdir -p test/dR/"+this_proc+"/profile")

          c_dR_2D = TCanvas("c_dR_2D","c_dR_2D",200,200,900,800)
          c_dR_2D.cd()

          den_var_dR_nums_2D[i].SetStats(0)
          den_var_dR_nums_2D[i].GetXaxis().SetRangeUser(0,200)
          den_var_dR_nums_2D[i].GetYaxis().SetRangeUser(0,1)
          gPad.SetLogz()
          den_var_dR_nums_2D[i].Draw("COLZ")
          
          txt_CMS.Draw()
          txt_sample.DrawLatexNDC(txt_sample_setting[this_proc][0],txt_sample_setting[this_proc][1],txt_sample_setting[this_proc][2])

          os.system("mkdir -p Winter21_new/"+this_proc+"/dR/"+this_WP)
          c_dR_2D.SaveAs("Winter21_new/"+this_proc+"/dR/"+this_WP+"/"+this_proc+"_"+this_WP+"_"+this_den+"_"+this_var+"_dR_"+this_num+this_trigLabel+"_"+AtVtx+"2D.png")


        ####Draw the profile####
        c_dR_pf = TCanvas("c_dR_pf","c_dR_pf",200,200,900,800)
        c_dR_pf.cd()

        lg_dR_pf = TLegend(0.7,0.1,0.9,0.3)

        for i in reversed(range(len(files))):
          option_pf = "P SAME"

          den_var_dR_nums_pf[i].SetMarkerStyle(markerStyles['purity'][i])
          den_var_dR_nums_pf[i].SetMarkerSize(markerSizes['purity'][i])
          den_var_dR_nums_pf[i].SetMarkerColor(graphColors[i])
          den_var_dR_nums_pf[i].SetLineColor(graphColors[i])
          den_var_dR_nums_pf[i].SetLineWidth(3)

          if i == len(files)-1:
            option_pf = ""

            den_var_dR_nums_pf[i].SetTitle("")
            den_var_dR_nums_pf[i].SetStats(0)
            den_var_dR_nums_pf[i].GetXaxis().SetTickLength(0.025)
            den_var_dR_nums_pf[i].GetXaxis().SetLabelSize(0.025)
            den_var_dR_nums_pf[i].GetYaxis().SetLabelSize(0.025)
            den_var_dR_nums_pf[i].GetYaxis().SetTitle("dR")
            den_var_dR_nums_pf[i].GetXaxis().SetRangeUser(0,200) #JH : Xrange['single'] or Xrange['double']
            
          den_var_dR_nums_pf[i].Draw(option_pf)

          lg_dR_pf.AddEntry(den_var_dR_nums_pf[i],lg_names[i])

        lg_dR_pf.Draw()
        
        txt_CMS.Draw()
        txt_sample.DrawLatexNDC(txt_sample_setting[this_proc][0],txt_sample_setting[this_proc][1],txt_sample_setting[this_proc][2])


########mva########
    if this_measure == "mva":

      #print files[0]
      #print files[0].Get(this_num+"_mva")

      this_mva      = files[0].Get(this_num+"_mva")
      this_sig_mva  = files[0].Get(this_num+"_sig_mva")
      this_fake_mva = files[0].Get(this_num+"_fake_mva")
      this_cu_sig_mva  = this_sig_mva.GetCumulative(kFALSE)
      this_cu_fake_mva = this_fake_mva.GetCumulative(kFALSE)
      this_cu_sig_mva.Scale(1./this_cu_sig_mva.GetBinContent(1))
      this_cu_fake_mva.Scale(1./this_cu_fake_mva.GetBinContent(1))
      this_cu_sig_mva.SetStats(0)
      this_cu_fake_mva.SetStats(0)

      c_mva = TCanvas("c_mva","c_mva",200,200,900,800)
      c_mva.cd()

      gPad.SetLogy()

      this_cu_fake_mva.SetMarkerStyle(20)
      this_cu_fake_mva.SetMarkerSize(1)
      this_cu_fake_mva.SetMarkerColor(kRed)
      this_cu_fake_mva.SetLineColor(kRed)
      this_cu_fake_mva.GetXaxis().SetTitle("MVA threshold")
      this_cu_fake_mva.GetYaxis().SetTitleOffset(1.2)
      this_cu_fake_mva.GetYaxis().SetLabelSize(0.03)
      this_cu_fake_mva.GetYaxis().SetTitle("Fraction of tracks with score > threshold")
      this_cu_fake_mva.GetYaxis().SetRangeUser(0.0001,100.)
      this_cu_sig_mva.SetMarkerStyle(20)
      this_cu_sig_mva.SetMarkerSize(1)
      this_cu_sig_mva.SetMarkerColor(kBlack)
      this_cu_sig_mva.SetLineColor(kBlack)

      this_cu_fake_mva.Draw()
      this_cu_sig_mva.Draw("same")

      lg_mva = TLegend(0.15,0.7,0.7,0.8)
      lg_mva.SetBorderSize(0)
      lg_mva.AddEntry(this_cu_sig_mva,"Muon matched tracks")
      lg_mva.AddEntry(this_cu_fake_mva,"Combinatorial tracks")
      lg_mva.Draw()

      txt_CMS = TText(0.1,0.905,"CMS simulation")
      txt_CMS.SetNDC()
      txt_CMS.SetTextSize(0.04)
      txt_CMS.Draw()
      txt_sample.DrawLatexNDC(txt_sample_setting_onepad[this_proc][0],txt_sample_setting_onepad[this_proc][1],txt_sample_setting_onepad[this_proc][2])
      #txt_sample.SetTextSize(0.035)

      os.system("mkdir -p Winter21_new/"+this_proc+"/mva/")
      c_mva.SaveAs("Winter21_new/"+this_proc+"/mva/"+this_proc+"_"+this_fltr+"_"+this_num+"_mva.png")


########truePU vs #tracks profile########
    if this_measure == "profile":
      if this_num == "Iter2FromL1":
        c_pf_track = TCanvas("pf_trackFromL1","pf_trackFromL1",200,200,900,800)
        c_pf_track.cd()

        lg_pf_track = TLegend(0.7,0.75,0.9,0.9)
        for igr in range(len(this_num_vars)):

          if igr == 0:
            option_gr = "" #JH : Don't use "AP" or "P" option for TProfile... idk the axis will be disappeared.

            this_pf = files[igr].Get("pf_trackFromL1")
            this_pf.SetStats(0)
            this_pf.SetTitle("")
            this_pf.SetMarkerStyle(markerStyles['purity'][igr])
            this_pf.SetMarkerSize(markerSizes['purity'][igr])
            this_pf.SetMarkerColor(graphColors[igr])
            this_pf.SetLineColor(graphColors[igr])
            this_pf.SetLineWidth(2)
            this_pf.GetXaxis().SetTitle("#scale[0.8]{# of truePU}")
            this_pf.GetXaxis().SetTickLength(0.025)
            this_pf.GetXaxis().SetLabelSize(0.025)
            this_pf.GetYaxis().SetLabelSize(0.025)
            this_pf.GetYaxis().SetTitle("average # of Iter2FromL1 tracks")
            this_pf.GetXaxis().SetRangeUser(30.,80.)
            this_pf.GetYaxis().SetRangeUser(0.,20.)

          else:
            option_gr = "SAME"

            this_pf = files[igr].Get("pf_trackFromL1")
            this_pf.SetStats(0)
            this_pf.SetMarkerStyle(markerStyles['purity'][igr])
            this_pf.SetMarkerSize(markerSizes['purity'][igr])
            this_pf.SetMarkerColor(graphColors[igr])
            this_pf.SetLineColor(graphColors[igr])
            this_pf.SetLineWidth(2)

          this_pf.Draw(option_gr)
          lg_pf_track.AddEntry(this_pf,lg_names[igr])

        lg_pf_track.Draw()
        
        txt_CMS = TText(0.1,0.905,"CMS simulation")
        txt_CMS.SetNDC()
        txt_CMS.SetTextSize(0.04)
        txt_CMS.Draw()
        txt_sample.DrawLatexNDC(txt_sample_setting_onepad[this_proc][0],txt_sample_setting_onepad[this_proc][1],txt_sample_setting_onepad[this_proc][2])

        os.system("mkdir -p Winter21_new/"+this_proc+"/profile/")
        c_pf_track.SaveAs("Winter21_new/"+this_proc+"/profile/"+this_proc+"_"+this_fltr+"_"+this_num+"_profile.png")


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



