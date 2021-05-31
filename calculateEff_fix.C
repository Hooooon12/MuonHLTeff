#include "TLorentzVector.h"
#include "TH1D.h"
#include "TFile.h"
#include "TChain.h"
#include "TString.h"
#include "TProfile.h"
#include "TROOT.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

map<TString,TH1D*> hists;

void FillHist(TString histname, double value, double weight, int n_bin, double x_min, double x_max){
  auto it = hists.find(histname);
  TH1D* hist=NULL;
  if( it==hists.end() ){
    hist = new TH1D(histname, "", n_bin, x_min, x_max);
    hists[histname] = hist;
  }else hist=it->second;
  hist->Fill(value, weight);
}

void calculateEff_fix(TString input, TString output){

  //ifstream in("list_input_fix.txt");
  //string line;

  //while(getline(in,line)){
  //  istringstream is(line);
  //  TString this_line = line;
  //  if(this_line(0,1)=="#"||this_line=="") continue;

  //  TString input, output;
  //  is >> input;
  //  is >> output;

  if(input.Contains("SKIP")) return;

    //==Set tree name and root files==//
    TChain *fChain = new TChain("ntupler/ntuple");

    fChain->Add(input);

    double genEventWeight;
    int truePU;
    int nGenParticle;
    int genParticle_ID[5000];
    int genParticle_status[5000];
    int genParticle_isPromptFinalState[5000];
    int genParticle_fromHardProcessFinalState[5000];
    double genParticle_px[5000];
    double genParticle_py[5000];
    double genParticle_pz[5000];
    double genParticle_energy[5000]; 
    double genParticle_pt[5000];
    double genParticle_eta[5000];
    double genParticle_phi[5000];
    double genParticle_charge[5000];
    int nL1Muon;
    double L1Muon_pt[5000];
    double L1Muon_eta[5000];
    double L1Muon_phi[5000];
    double L1Muon_charge[5000];
    double L1Muon_quality[5000];
    int nIterL3MuonNoID;
    double iterL3MuonNoID_pt[5000];
    double iterL3MuonNoID_eta[5000];
    double iterL3MuonNoID_phi[5000];
    double iterL3MuonNoID_charge[5000];
    int nIterL3Muon;
    double iterL3Muon_pt[5000];
    double iterL3Muon_eta[5000];
    double iterL3Muon_phi[5000];
    double iterL3Muon_charge[5000];
    vector<double> *hltIterL3OIMuonTrack_pt = 0;
    vector<double> *hltIterL3OIMuonTrack_eta = 0;
    vector<double> *hltIterL3OIMuonTrack_phi = 0;
    vector<double> *hltIter0IterL3MuonTrack_pt = 0;
    vector<double> *hltIter0IterL3MuonTrack_eta = 0;
    vector<double> *hltIter0IterL3MuonTrack_phi = 0;
    int nhltIter2IterL3MuonTrack;
    vector<double> *hltIter2IterL3MuonTrack_pt = 0;
    vector<double> *hltIter2IterL3MuonTrack_eta = 0;
    vector<double> *hltIter2IterL3MuonTrack_phi = 0;
    vector<double> *hltIter3IterL3MuonTrack_pt = 0;
    vector<double> *hltIter3IterL3MuonTrack_eta = 0;
    vector<double> *hltIter3IterL3MuonTrack_phi = 0;
    vector<double> *hltIter0IterL3FromL1MuonTrack_pt = 0;
    vector<double> *hltIter0IterL3FromL1MuonTrack_eta = 0;
    vector<double> *hltIter0IterL3FromL1MuonTrack_phi = 0;
    int nhltIter2IterL3FromL1MuonTrack;
    vector<double> *hltIter2IterL3FromL1MuonTrack_pt = 0;
    vector<double> *hltIter2IterL3FromL1MuonTrack_eta = 0;
    vector<double> *hltIter2IterL3FromL1MuonTrack_phi = 0;
    vector<double> *hltIter3IterL3FromL1MuonTrack_pt = 0;
    vector<double> *hltIter3IterL3FromL1MuonTrack_eta = 0;
    vector<double> *hltIter3IterL3FromL1MuonTrack_phi = 0;
    
    fChain->SetBranchAddress("genEventWeight",&genEventWeight);
    fChain->SetBranchAddress("truePU",&truePU);
    fChain->SetBranchAddress("nGenParticle",&nGenParticle);
    fChain->SetBranchAddress("genParticle_ID",&genParticle_ID);
    fChain->SetBranchAddress("genParticle_status",&genParticle_status);
    fChain->SetBranchAddress("genParticle_isPromptFinalState",&genParticle_isPromptFinalState);
    fChain->SetBranchAddress("genParticle_fromHardProcessFinalState",&genParticle_fromHardProcessFinalState);
    fChain->SetBranchAddress("genParticle_px",&genParticle_px);
    fChain->SetBranchAddress("genParticle_py",&genParticle_py);
    fChain->SetBranchAddress("genParticle_pz",&genParticle_pz);
    fChain->SetBranchAddress("genParticle_energy",&genParticle_energy);
    fChain->SetBranchAddress("genParticle_pt",&genParticle_pt);
    fChain->SetBranchAddress("genParticle_eta",&genParticle_eta);
    fChain->SetBranchAddress("genParticle_phi",&genParticle_phi);
    fChain->SetBranchAddress("genParticle_charge",&genParticle_charge);
    fChain->SetBranchAddress("nL1Muon",&nL1Muon);
    fChain->SetBranchAddress("L1Muon_pt",&L1Muon_pt);
    fChain->SetBranchAddress("L1Muon_eta",&L1Muon_eta);
    fChain->SetBranchAddress("L1Muon_phi",&L1Muon_phi);
    fChain->SetBranchAddress("L1Muon_charge",&L1Muon_charge);
    fChain->SetBranchAddress("L1Muon_quality",&L1Muon_quality);
    fChain->SetBranchAddress("nIterL3MuonNoID",&nIterL3MuonNoID);
    fChain->SetBranchAddress("iterL3MuonNoID_pt",&iterL3MuonNoID_pt);
    fChain->SetBranchAddress("iterL3MuonNoID_eta",&iterL3MuonNoID_eta);
    fChain->SetBranchAddress("iterL3MuonNoID_phi",&iterL3MuonNoID_phi);
    fChain->SetBranchAddress("iterL3MuonNoID_charge",&iterL3MuonNoID_charge);
    fChain->SetBranchAddress("nIterL3Muon",&nIterL3Muon);
    fChain->SetBranchAddress("iterL3Muon_pt",&iterL3Muon_pt);
    fChain->SetBranchAddress("iterL3Muon_eta",&iterL3Muon_eta);
    fChain->SetBranchAddress("iterL3Muon_phi",&iterL3Muon_phi);
    fChain->SetBranchAddress("iterL3Muon_charge",&iterL3Muon_charge);
    fChain->SetBranchAddress("hltIterL3OIMuonTrack_pt",&hltIterL3OIMuonTrack_pt);
    fChain->SetBranchAddress("hltIterL3OIMuonTrack_eta",&hltIterL3OIMuonTrack_eta);
    fChain->SetBranchAddress("hltIterL3OIMuonTrack_phi",&hltIterL3OIMuonTrack_phi);
    fChain->SetBranchAddress("hltIter0IterL3MuonTrack_pt",&hltIter0IterL3MuonTrack_pt);
    fChain->SetBranchAddress("hltIter0IterL3MuonTrack_eta",&hltIter0IterL3MuonTrack_eta);
    fChain->SetBranchAddress("hltIter0IterL3MuonTrack_phi",&hltIter0IterL3MuonTrack_phi);
    fChain->SetBranchAddress("nhltIter2IterL3MuonTrack",&nhltIter2IterL3MuonTrack);
    fChain->SetBranchAddress("hltIter2IterL3MuonTrack_pt",&hltIter2IterL3MuonTrack_pt);
    fChain->SetBranchAddress("hltIter2IterL3MuonTrack_eta",&hltIter2IterL3MuonTrack_eta);
    fChain->SetBranchAddress("hltIter2IterL3MuonTrack_phi",&hltIter2IterL3MuonTrack_phi);
    fChain->SetBranchAddress("hltIter3IterL3MuonTrack_pt",&hltIter3IterL3MuonTrack_pt);
    fChain->SetBranchAddress("hltIter3IterL3MuonTrack_eta",&hltIter3IterL3MuonTrack_eta);
    fChain->SetBranchAddress("hltIter3IterL3MuonTrack_phi",&hltIter3IterL3MuonTrack_phi);
    fChain->SetBranchAddress("hltIter0IterL3FromL1MuonTrack_pt",&hltIter0IterL3FromL1MuonTrack_pt);
    fChain->SetBranchAddress("hltIter0IterL3FromL1MuonTrack_eta",&hltIter0IterL3FromL1MuonTrack_eta);
    fChain->SetBranchAddress("hltIter0IterL3FromL1MuonTrack_phi",&hltIter0IterL3FromL1MuonTrack_phi);
    fChain->SetBranchAddress("nhltIter2IterL3FromL1MuonTrack",&nhltIter2IterL3FromL1MuonTrack);
    fChain->SetBranchAddress("hltIter2IterL3FromL1MuonTrack_pt",&hltIter2IterL3FromL1MuonTrack_pt);
    fChain->SetBranchAddress("hltIter2IterL3FromL1MuonTrack_eta",&hltIter2IterL3FromL1MuonTrack_eta);
    fChain->SetBranchAddress("hltIter2IterL3FromL1MuonTrack_phi",&hltIter2IterL3FromL1MuonTrack_phi);
    fChain->SetBranchAddress("hltIter3IterL3FromL1MuonTrack_pt",&hltIter3IterL3FromL1MuonTrack_pt);
    fChain->SetBranchAddress("hltIter3IterL3FromL1MuonTrack_eta",&hltIter3IterL3FromL1MuonTrack_eta);
    fChain->SetBranchAddress("hltIter3IterL3FromL1MuonTrack_phi",&hltIter3IterL3FromL1MuonTrack_phi);
    
    
    long int Nevents = fChain->GetEntries();
    cout << "total " << Nevents << " events" << endl;
    
    int NhardP = 0;
    int NhardP_above = 0;
    int NhardP_L1 = 0;
    int NL3_NoID = 0;
    int NL3 = 0;
    int NIter2FromL1Track = 0;
    int NFinal = 0;
    double Nmatched_hardP_tracks_pt = 0;
    double Nmatched_hardP_tracks_others = 0;
    double Nmatched_hardP_L3NoID_pt = 0;
    double Nmatched_hardP_L3NoID_others = 0;
    double Nmatched_hardP_L3_pt = 0;
    double Nmatched_hardP_L3_others = 0;
    double Nmatched_hardP_L1_tracks_pt = 0;
    double Nmatched_hardP_L1_L3NoID_pt = 0;
    double Nmatched_hardP_L1_L3_pt = 0;
    double Nmatched_L3NoID_Final = 0;
    double Nmatched_L3_Final = 0;
    double Nmatched_Iter2FromL1Track_Final = 0;
   
    //Call the histogram//
    hists.clear();
    TProfile *pf_track = new TProfile("pf_track","pf_track",25,30,80,0,5000);
    TProfile *pf_trackFromL1 = new TProfile("pf_trackFromL1","pf_trackFromL1",25,30,80,0,5000);

    //To avoid double counting//
    vector<int> this_matched;

    for(int i=0; i<Nevents; i++){
      fChain->GetEntry(i);
      //cout << "==========" << i << "th event==========" << endl;
      //int this_NhardP = 0;
    
    //===============================Call needed objects=======================================//
    
      //==pick index of hardP muons with eta < 2.4==//
      vector<int> hardP_idx;
      for(int j=0; j<nGenParticle; j++){
        if(genParticle_fromHardProcessFinalState[j]==1&&fabs(genParticle_eta[j])<2.4){
          hardP_idx.push_back(j);
          NhardP+=1;
        }
      }
    
      //==make hardP muon's 4vectors==//
      vector<TLorentzVector> hardPs;
      for(int j=0; j<hardP_idx.size(); j++){
        TLorentzVector hardP;
        hardP.SetPtEtaPhiM(genParticle_pt[hardP_idx.at(j)],genParticle_eta[hardP_idx.at(j)],genParticle_phi[hardP_idx.at(j)],0);
        hardPs.push_back(hardP);
      }
      //==hardP muon above 26GeV==//
      vector<TLorentzVector> hardPs_above;
      for(int j=0; j<hardP_idx.size(); j++){
        TLorentzVector hardP;
        hardP.SetPtEtaPhiM(genParticle_pt[hardP_idx.at(j)],genParticle_eta[hardP_idx.at(j)],genParticle_phi[hardP_idx.at(j)],0);
        if(hardP.Pt() > 26){
          hardPs_above.push_back(hardP);
          NhardP_above+=1;
        }
      }
      //==hardP muon above 10GeV==//
      vector<TLorentzVector> hardPs_above_med;
      for(int j=0; j<hardP_idx.size(); j++){
        TLorentzVector hardP;
        hardP.SetPtEtaPhiM(genParticle_pt[hardP_idx.at(j)],genParticle_eta[hardP_idx.at(j)],genParticle_phi[hardP_idx.at(j)],0);
        if(hardP.Pt() > 10){
          hardPs_above_med.push_back(hardP);
        }
      }
      //==L1Muon with fine quality and above trigger filter thresh.==//
      vector<TLorentzVector> L1Muons;
      for(int j=0; j<nL1Muon; j++){
        TLorentzVector L1Muon;
        if(L1Muon_quality[j] > 11 && L1Muon_pt[j] > 22){ // quality 8 / L1 5 --> L3 muon pt 8GeV. vs eta, phi, truePU --> gen matching with pt > 10GeV. (to measure in the plateau.) Numerator --> matching with L3 and also "L1". Numerator should always be a subset of denominator.
          L1Muon.SetPtEtaPhiM(L1Muon_pt[j],L1Muon_eta[j],L1Muon_phi[j],0);
          L1Muons.push_back(L1Muon);
        }
      }
      //==L1Muon with medium quality and above double muon trigger filter thresh.==//
      vector<TLorentzVector> L1Muons_med;
      for(int j=0; j<nL1Muon; j++){
        TLorentzVector L1Muon_med;
        if(L1Muon_quality[j] > 7 && L1Muon_pt[j] > 5){ // quality 8 / L1 5 --> L3 muon pt 8GeV. vs eta, phi, truePU --> gen matching with pt > 10GeV. (to measure in the plateau.) Numerator --> matching with L3 and also "L1". Numerator should always be a subset of denominator.
          L1Muon_med.SetPtEtaPhiM(L1Muon_pt[j],L1Muon_eta[j],L1Muon_phi[j],0);
          L1Muons_med.push_back(L1Muon_med);
        }
      }
      //==L3MuonNoID==//
      vector<TLorentzVector> L3Muons_NoID;
      for(int j=0; j<nIterL3MuonNoID; j++){
        TLorentzVector L3Muon_NoID;
        L3Muon_NoID.SetPtEtaPhiM(iterL3MuonNoID_pt[j],iterL3MuonNoID_eta[j],iterL3MuonNoID_phi[j],0);
        L3Muons_NoID.push_back(L3Muon_NoID);
        NL3_NoID+=1;
      }
      //==L3Muon==//
      vector<TLorentzVector> L3Muons;
      for(int j=0; j<nIterL3Muon; j++){
        TLorentzVector L3Muon;
        L3Muon.SetPtEtaPhiM(iterL3Muon_pt[j],iterL3Muon_eta[j],iterL3Muon_phi[j],0);
        L3Muons.push_back(L3Muon);
        NL3+=1;
      }
      //==hardP muon matched with fine L1==//
      vector<TLorentzVector> hardPs_L1matched;
      this_matched.clear();
      for(int j=0; j<hardPs.size(); j++){
        TLorentzVector hardP_L1matched = hardPs.at(j);
        for(int k=0; k<L1Muons.size(); k++){
          if( find(this_matched.begin(), this_matched.end(), k) == this_matched.end() ){
            if(hardP_L1matched.DeltaR(L1Muons.at(k))<0.3){
              hardPs_L1matched.push_back(hardP_L1matched);
              NhardP_L1+=1;
              this_matched.push_back(k);
              break;
            }
          }
        }
      }
      //==hardP muon above 26GeV matched with fine L1==//
      vector<TLorentzVector> hardPs_above_L1matched;
      this_matched.clear();
      for(int j=0; j<hardPs_above.size(); j++){
        TLorentzVector hardP_above_L1matched = hardPs_above.at(j);
        for(int k=0; k<L1Muons.size(); k++){
          if( find(this_matched.begin(), this_matched.end(), k) == this_matched.end() ){
            if(hardP_above_L1matched.DeltaR(L1Muons.at(k))<0.3){
              hardPs_above_L1matched.push_back(hardP_above_L1matched);
              this_matched.push_back(k);
              break;
            }
          }
        }
      }
      //==hardP muon matched with medium L1==//
      vector<TLorentzVector> hardPs_L1matched_med;
      this_matched.clear();
      for(int j=0; j<hardPs.size(); j++){
        TLorentzVector hardP_L1matched_med = hardPs.at(j);
        for(int k=0; k<L1Muons_med.size(); k++){
          if( find(this_matched.begin(), this_matched.end(), k) == this_matched.end() ){
            if(hardP_L1matched_med.DeltaR(L1Muons_med.at(k))<0.3){
              hardPs_L1matched_med.push_back(hardP_L1matched_med);
              this_matched.push_back(k);
              break;
            }
          }
        }
      }
      //==hardP muon above 10GeV matched with medium L1==//
      vector<TLorentzVector> hardPs_above_L1matched_med;
      this_matched.clear();
      for(int j=0; j<hardPs_above_med.size(); j++){
        TLorentzVector hardP_above_L1matched_med = hardPs_above_med.at(j);
        for(int k=0; k<L1Muons_med.size(); k++){
          if( find(this_matched.begin(), this_matched.end(), k) == this_matched.end() ){
            if(hardP_above_L1matched_med.DeltaR(L1Muons_med.at(k))<0.3){
              hardPs_above_L1matched_med.push_back(hardP_above_L1matched_med);
              this_matched.push_back(k);
              break;
            }
          }
        }
      }
      //==pick index of final state muons with eta < 2.4==//
      vector<int> Final_idx;
      for(int j=0; j<nGenParticle; j++){
        if(genParticle_status[j]==1&&fabs(genParticle_eta[j])<2.4){
          Final_idx.push_back(j);
          NFinal+=1;
        }
      }
      //==make final muon's 4vectors==//
      vector<TLorentzVector> Finals;
      for(int j=0; j<Final_idx.size(); j++){
        TLorentzVector Final;
        Final.SetPtEtaPhiM(genParticle_pt[Final_idx.at(j)],genParticle_eta[Final_idx.at(j)],genParticle_phi[Final_idx.at(j)],0);
        Finals.push_back(Final);
      }
    
      //==tracks to measure eff==//
      vector<TLorentzVector> L3OITracks;
      for(int j=0; j<hltIterL3OIMuonTrack_pt->size(); j++){
        TLorentzVector L3OITrack;
        L3OITrack.SetPtEtaPhiM(hltIterL3OIMuonTrack_pt->at(j),hltIterL3OIMuonTrack_eta->at(j),hltIterL3OIMuonTrack_phi->at(j),0);
        L3OITracks.push_back(L3OITrack);
        //cout << "hltIterL3OIMuonTrack_pt: " << hltIterL3OIMuonTrack_pt->at(j) << endl;
      }
      //if(L3OITracks.size()>0) cout << "hltIterL3OIMuonTrack_pt: " << L3OITracks.at(0).Pt() << endl;
    
      vector<TLorentzVector> Iter0L3Tracks;
      for(int j=0; j<hltIter0IterL3MuonTrack_pt->size(); j++){
        TLorentzVector Iter0L3Track;
        Iter0L3Track.SetPtEtaPhiM(hltIter0IterL3MuonTrack_pt->at(j),hltIter0IterL3MuonTrack_eta->at(j),hltIter0IterL3MuonTrack_phi->at(j),0);
        Iter0L3Tracks.push_back(Iter0L3Track);
        //cout << "hltIter0IterL3MuonTrack_pt: " << hltIter0IterL3MuonTrack_pt->at(j) << endl;
      }
    
      vector<TLorentzVector> Iter2L3Tracks;
      for(int j=0; j<hltIter2IterL3MuonTrack_pt->size(); j++){
        TLorentzVector Iter2L3Track;
        Iter2L3Track.SetPtEtaPhiM(hltIter2IterL3MuonTrack_pt->at(j),hltIter2IterL3MuonTrack_eta->at(j),hltIter2IterL3MuonTrack_phi->at(j),0);
        Iter2L3Tracks.push_back(Iter2L3Track);
        //cout << "hltIter2IterL3MuonTrack_pt: " << hltIter2IterL3MuonTrack_pt->at(j) << endl;
      }
    
      vector<TLorentzVector> Iter3L3Tracks;
      for(int j=0; j<hltIter3IterL3MuonTrack_pt->size(); j++){
        TLorentzVector Iter3L3Track;
        Iter3L3Track.SetPtEtaPhiM(hltIter3IterL3MuonTrack_pt->at(j),hltIter3IterL3MuonTrack_eta->at(j),hltIter3IterL3MuonTrack_phi->at(j),0);
        Iter3L3Tracks.push_back(Iter3L3Track);
        //cout << "hltIter3IterL3MuonTrack_pt: " << hltIter3IterL3MuonTrack_pt->at(j) << endl;
      }
    
      vector<TLorentzVector> Iter0L3FromL1Tracks;
      for(int j=0; j<hltIter0IterL3FromL1MuonTrack_pt->size(); j++){
        TLorentzVector Iter0L3FromL1Track;
        Iter0L3FromL1Track.SetPtEtaPhiM(hltIter0IterL3FromL1MuonTrack_pt->at(j),hltIter0IterL3FromL1MuonTrack_eta->at(j),hltIter0IterL3FromL1MuonTrack_phi->at(j),0);
        Iter0L3FromL1Tracks.push_back(Iter0L3FromL1Track);
        //cout << "hltIter0IterL3FromL1MuonTrack_pt: " << hltIter0IterL3FromL1MuonTrack_pt->at(j) << endl;
      }
    
      vector<TLorentzVector> Iter2L3FromL1Tracks;
      for(int j=0; j<hltIter2IterL3FromL1MuonTrack_pt->size(); j++){
        TLorentzVector Iter2L3FromL1Track;
        Iter2L3FromL1Track.SetPtEtaPhiM(hltIter2IterL3FromL1MuonTrack_pt->at(j),hltIter2IterL3FromL1MuonTrack_eta->at(j),hltIter2IterL3FromL1MuonTrack_phi->at(j),0);
        Iter2L3FromL1Tracks.push_back(Iter2L3FromL1Track);
        NIter2FromL1Track+=1;
        //cout << "hltIter2IterL3FromL1MuonTrack_pt: " << hltIter2IterL3FromL1MuonTrack_pt->at(j) << endl;
      }
    
      vector<TLorentzVector> Iter3L3FromL1Tracks;
      for(int j=0; j<hltIter3IterL3FromL1MuonTrack_pt->size(); j++){
        TLorentzVector Iter3L3FromL1Track;
        Iter3L3FromL1Track.SetPtEtaPhiM(hltIter3IterL3FromL1MuonTrack_pt->at(j),hltIter3IterL3FromL1MuonTrack_eta->at(j),hltIter3IterL3FromL1MuonTrack_phi->at(j),0);
        Iter3L3FromL1Tracks.push_back(Iter3L3FromL1Track);
        //cout << "hltIter3IterL3FromL1MuonTrack_pt: " << hltIter3IterL3FromL1MuonTrack_pt->at(j) << endl;
      }
    
    
    //====================================Efficiency vs pt========================================//
    
      //==Eff 1-1. hard procecss muon-tracker track matching==//
      for(int j=0; j<hardPs.size(); j++){
        FillHist("hardP_pt",hardPs.at(j).Pt(),1.,1500,0,1500);
        int matched = 0;
        for(int k=0; k<L3OITracks.size(); k++){
          if(hardPs.at(j).DeltaR(L3OITracks.at(k)) < 0.3){
            //cout << j << "th hardP muon matched with " << k << "th L3OItrack within dR: " << hardPs.at(j).DeltaR(L3OITracks.at(k)) << endl;
            //cout << "pt: " << hardPs.at(j).Pt() << " vs " << L3OITracks.at(k).Pt() << endl;
            matched = 1;
            FillHist("hardP_pt_trackeff_step1",hardPs.at(j).Pt(),1.,1500,0,1500);
            FillHist("hardP_pt_trackeff_step2",hardPs.at(j).Pt(),1.,1500,0,1500);
            FillHist("hardP_pt_trackeff_step3",hardPs.at(j).Pt(),1.,1500,0,1500);
            FillHist("hardP_pt_trackeff_step4",hardPs.at(j).Pt(),1.,1500,0,1500);
            FillHist("hardP_pt_trackeff_step5",hardPs.at(j).Pt(),1.,1500,0,1500);
            break;
          }
        }
        if(matched == 0){
          for(int k=0; k<Iter0L3Tracks.size(); k++){
            if(hardPs.at(j).DeltaR(Iter0L3Tracks.at(k))<0.3){
              //cout << j << "th hardP muon matched with " << k << "th Iter0L3track within dR: " << hardPs.at(j).DeltaR(Iter0L3Tracks.at(k)) << endl;
              //cout << "pt: " << hardPs.at(j).Pt() << " vs " << Iter0L3Tracks.at(k).Pt() << endl;
              matched = 1;
              FillHist("hardP_pt_trackeff_step2",hardPs.at(j).Pt(),1.,1500,0,1500);
              FillHist("hardP_pt_trackeff_step3",hardPs.at(j).Pt(),1.,1500,0,1500);
              FillHist("hardP_pt_trackeff_step4",hardPs.at(j).Pt(),1.,1500,0,1500);
              FillHist("hardP_pt_trackeff_step5",hardPs.at(j).Pt(),1.,1500,0,1500);
              break;
            }
          }
        }
        if(matched == 0){
          for(int k=0; k<Iter2L3Tracks.size(); k++){
            if(hardPs.at(j).DeltaR(Iter2L3Tracks.at(k))<0.3){
              //cout << j << "th hardP muon matched with " << k << "th Iter2L3track within dR: " << hardPs.at(j).DeltaR(Iter2L3Tracks.at(k)) << endl;
              //cout << "pt: " << hardPs.at(j).Pt() << " vs " << Iter2L3Tracks.at(k).Pt() << endl;
              matched = 1;
              FillHist("hardP_pt_trackeff_step3",hardPs.at(j).Pt(),1.,1500,0,1500);
              FillHist("hardP_pt_trackeff_step4",hardPs.at(j).Pt(),1.,1500,0,1500);
              FillHist("hardP_pt_trackeff_step5",hardPs.at(j).Pt(),1.,1500,0,1500);
              break;
            }
          }
        }
        if(matched == 0){
          for(int k=0; k<Iter0L3FromL1Tracks.size(); k++){
            if(hardPs.at(j).DeltaR(Iter0L3FromL1Tracks.at(k))<0.3){
              //cout << j << "th hardP muon matched with " << k << "th Iter0L3FromL1track within dR: " << hardPs.at(j).DeltaR(Iter0L3FromL1Tracks.at(k)) << endl;
              //cout << "pt: " << hardPs.at(j).Pt() << " vs " << Iter0L3FromL1Tracks.at(k).Pt() << endl;
              matched = 1;
              FillHist("hardP_pt_trackeff_step4",hardPs.at(j).Pt(),1.,1500,0,1500);
              FillHist("hardP_pt_trackeff_step5",hardPs.at(j).Pt(),1.,1500,0,1500);
              break;
            }
          }
        }
        if(matched == 0){
          for(int k=0; k<Iter2L3FromL1Tracks.size(); k++){
            if(hardPs.at(j).DeltaR(Iter2L3FromL1Tracks.at(k))<0.3){
              //cout << j << "th hardP muon matched with " << k << "th Iter2L3FromL1track within dR: " << hardPs.at(j).DeltaR(Iter2L3FromL1Tracks.at(k)) << endl;
              //cout << "pt: " << hardPs.at(j).Pt() << " vs " << Iter2L3FromL1Tracks.at(k).Pt() << endl;
              matched = 1;
              FillHist("hardP_pt_trackeff_step5",hardPs.at(j).Pt(),1.,1500,0,1500);
              break;
            }
          }
        }
        if(matched == 1) Nmatched_hardP_tracks_pt+=1;
      }
    
      //==Eff 1-2-1. hard procecss muon-L3 muon(NoID) matching==//
      this_matched.clear();
      for(int j=0; j<hardPs.size(); j++){
        for(int k=0; k<L3Muons_NoID.size(); k++){
          if( find(this_matched.begin(), this_matched.end(), k) == this_matched.end() ){
            if(hardPs.at(j).DeltaR(L3Muons_NoID.at(k)) < 0.1){
              //cout << j << "th hardP muon matched with " << k << "th L3OItrack within dR: " << hardPs.at(j).DeltaR(L3OITracks.at(k)) << endl;
              //cout << "pt: " << hardPs.at(j).Pt() << " vs " << L3OITracks.at(k).Pt() << endl;
              Nmatched_hardP_L3NoID_pt+=1;
              FillHist("hardP_pt_eff_L3NoID",hardPs.at(j).Pt(),1.,1500,0,1500);
              this_matched.push_back(k);
              break;
            }
          }
        }
      }
    
      //==Eff 1-2-2. hard procecss muon-L3 muon matching==//
      this_matched.clear();
      for(int j=0; j<hardPs.size(); j++){
        for(int k=0; k<L3Muons.size(); k++){
          if( find(this_matched.begin(), this_matched.end(), k) == this_matched.end() ){
            if(hardPs.at(j).DeltaR(L3Muons.at(k)) < 0.1){
              //cout << j << "th hardP muon matched with " << k << "th L3OItrack within dR: " << hardPs.at(j).DeltaR(L3OITracks.at(k)) << endl;
              //cout << "pt: " << hardPs.at(j).Pt() << " vs " << L3OITracks.at(k).Pt() << endl;
              Nmatched_hardP_L3_pt+=1;
              FillHist("hardP_pt_eff_L3",hardPs.at(j).Pt(),1.,1500,0,1500);
              this_matched.push_back(k);
              break;
            }
          }
        }
      }
    
    
      //==Eff 2-1. hard process muon matched with fine L1-tracker track matching==//
      for(int j=0; j<hardPs_L1matched.size(); j++){
        FillHist("hardP_L1_pt",hardPs_L1matched.at(j).Pt(),1.,1500,0,1500);
        int matched = 0;
        for(int k=0; k<L3OITracks.size(); k++){
          if(hardPs_L1matched.at(j).DeltaR(L3OITracks.at(k)) < 0.3){
            //cout << j << "th hardP muon matched with " << k << "th L3OItrack within dR: " << hardPs_L1matched.at(j).DeltaR(L3OITracks.at(k)) << endl;
            //cout << "pt: " << hardPs_L1matched.at(j).Pt() << " vs " << L3OITracks.at(k).Pt() << endl;
            matched = 1;
            FillHist("hardP_L1_pt_trackeff_step1",hardPs_L1matched.at(j).Pt(),1.,1500,0,1500);
            FillHist("hardP_L1_pt_trackeff_step2",hardPs_L1matched.at(j).Pt(),1.,1500,0,1500);
            FillHist("hardP_L1_pt_trackeff_step3",hardPs_L1matched.at(j).Pt(),1.,1500,0,1500);
            FillHist("hardP_L1_pt_trackeff_step4",hardPs_L1matched.at(j).Pt(),1.,1500,0,1500);
            FillHist("hardP_L1_pt_trackeff_step5",hardPs_L1matched.at(j).Pt(),1.,1500,0,1500);
            break;
          }
        }
        if(matched == 0){
          for(int k=0; k<Iter0L3Tracks.size(); k++){
            if(hardPs_L1matched.at(j).DeltaR(Iter0L3Tracks.at(k))<0.3){
              //cout << j << "th hardP muon matched with " << k << "th Iter0L3track within dR: " << hardPs_L1matched.at(j).DeltaR(Iter0L3Tracks.at(k)) << endl;
              //cout << "pt: " << hardPs_L1matched.at(j).Pt() << " vs " << Iter0L3Tracks.at(k).Pt() << endl;
              matched = 1;
              FillHist("hardP_L1_pt_trackeff_step2",hardPs_L1matched.at(j).Pt(),1.,1500,0,1500);
              FillHist("hardP_L1_pt_trackeff_step3",hardPs_L1matched.at(j).Pt(),1.,1500,0,1500);
              FillHist("hardP_L1_pt_trackeff_step4",hardPs_L1matched.at(j).Pt(),1.,1500,0,1500);
              FillHist("hardP_L1_pt_trackeff_step5",hardPs_L1matched.at(j).Pt(),1.,1500,0,1500);
              break;
            }
          }
        }
        if(matched == 0){
          for(int k=0; k<Iter2L3Tracks.size(); k++){
            if(hardPs_L1matched.at(j).DeltaR(Iter2L3Tracks.at(k))<0.3){
              //cout << j << "th hardP muon matched with " << k << "th Iter2L3track within dR: " << hardPs_L1matched.at(j).DeltaR(Iter2L3Tracks.at(k)) << endl;
              //cout << "pt: " << hardPs_L1matched.at(j).Pt() << " vs " << Iter2L3Tracks.at(k).Pt() << endl;
              matched = 1;
              FillHist("hardP_L1_pt_trackeff_step3",hardPs_L1matched.at(j).Pt(),1.,1500,0,1500);
              FillHist("hardP_L1_pt_trackeff_step4",hardPs_L1matched.at(j).Pt(),1.,1500,0,1500);
              FillHist("hardP_L1_pt_trackeff_step5",hardPs_L1matched.at(j).Pt(),1.,1500,0,1500);
              break;
            }
          }
        }
        if(matched == 0){
          for(int k=0; k<Iter0L3FromL1Tracks.size(); k++){
            if(hardPs_L1matched.at(j).DeltaR(Iter0L3FromL1Tracks.at(k))<0.3){
              //cout << j << "th hardP muon matched with " << k << "th Iter0L3FromL1track within dR: " << hardPs_L1matched.at(j).DeltaR(Iter0L3FromL1Tracks.at(k)) << endl;
              //cout << "pt: " << hardPs_L1matched.at(j).Pt() << " vs " << Iter0L3FromL1Tracks.at(k).Pt() << endl;
              matched = 1;
              FillHist("hardP_L1_pt_trackeff_step4",hardPs_L1matched.at(j).Pt(),1.,1500,0,1500);
              FillHist("hardP_L1_pt_trackeff_step5",hardPs_L1matched.at(j).Pt(),1.,1500,0,1500);
              break;
            }
          }
        }
        if(matched == 0){
          for(int k=0; k<Iter2L3FromL1Tracks.size(); k++){
            if(hardPs_L1matched.at(j).DeltaR(Iter2L3FromL1Tracks.at(k))<0.3){
              //cout << j << "th hardP muon matched with " << k << "th Iter2L3FromL1track within dR: " << hardPs_L1matched.at(j).DeltaR(Iter2L3FromL1Tracks.at(k)) << endl;
              //cout << "pt: " << hardPs_L1matched.at(j).Pt() << " vs " << Iter2L3FromL1Tracks.at(k).Pt() << endl;
              matched = 1;
              FillHist("hardP_L1_pt_trackeff_step5",hardPs_L1matched.at(j).Pt(),1.,1500,0,1500);
              break;
            }
          }
        }
        if(matched == 1) Nmatched_hardP_L1_tracks_pt+=1;
      }
    
      //==Eff 2-2-1. hard procecss muon matched with fine L1-L3 muon(NoID) matching==//
      this_matched.clear();
      for(int j=0; j<hardPs_L1matched.size(); j++){
        for(int k=0; k<L3Muons_NoID.size(); k++){
          if( find(this_matched.begin(), this_matched.end(), k) == this_matched.end() ){
            if(hardPs_L1matched.at(j).DeltaR(L3Muons_NoID.at(k)) < 0.1){
              Nmatched_hardP_L1_L3NoID_pt+=1;
              FillHist("hardP_L1_pt_eff_L3NoID",hardPs_L1matched.at(j).Pt(),1.,1500,0,1500);
              this_matched.push_back(k);
              break;
            }
          }
        }
      }
    
      //==Eff 2-2-2. hard procecss muon matched with fine L1-L3 muon matching==//
      this_matched.clear();
      for(int j=0; j<hardPs_L1matched.size(); j++){
        for(int k=0; k<L3Muons.size(); k++){
          if( find(this_matched.begin(), this_matched.end(), k) == this_matched.end() ){
            if(hardPs_L1matched.at(j).DeltaR(L3Muons.at(k)) < 0.1){
              Nmatched_hardP_L1_L3_pt+=1;
              FillHist("hardP_L1_pt_eff_L3",hardPs_L1matched.at(j).Pt(),1.,1500,0,1500);
              this_matched.push_back(k);
              break;
            }
          }
        }
      }
    
      //==Eff 2-3-1. hard procecss muon matched with medium L1-L3 muon(NoID) matching==//
      this_matched.clear();
      for(int j=0; j<hardPs_L1matched_med.size(); j++){
        FillHist("hardP_L1_med_pt",hardPs_L1matched_med.at(j).Pt(),1.,1500,0,1500);
        for(int k=0; k<L3Muons_NoID.size(); k++){
          if( find(this_matched.begin(), this_matched.end(), k) == this_matched.end() ){
            if(hardPs_L1matched_med.at(j).DeltaR(L3Muons_NoID.at(k)) < 0.1){
              FillHist("hardP_L1_med_pt_eff_L3NoID",hardPs_L1matched_med.at(j).Pt(),1.,1500,0,1500);
              this_matched.push_back(k);
              break;
            }
          }
        }
      }
    
      //==Eff 2-3-2. hard procecss muon matched with medium L1-L3 muon matching==//
      this_matched.clear();
      for(int j=0; j<hardPs_L1matched_med.size(); j++){
        for(int k=0; k<L3Muons.size(); k++){
          if( find(this_matched.begin(), this_matched.end(), k) == this_matched.end() ){
            if(hardPs_L1matched_med.at(j).DeltaR(L3Muons.at(k)) < 0.1){
              FillHist("hardP_L1_med_pt_eff_L3",hardPs_L1matched_med.at(j).Pt(),1.,1500,0,1500);
              this_matched.push_back(k);
              break;
            }
          }
        }
      }
    
 
    //====================================Efficiency vs eta, phi, truePU========================================//
    
      //==Eff 1-1. hard procecss muon-tracker track matching==//
      for(int j=0; j<hardPs_above.size(); j++){
        FillHist("hardP_eta",hardPs_above.at(j).Eta(),1.,48,-2.4,2.4);
        FillHist("hardP_phi",hardPs_above.at(j).Phi(),1.,63,-3.15,3.15);
        FillHist("hardP_truePU",truePU,1.,100,0,100);
        int matched = 0;
        for(int k=0; k<L3OITracks.size(); k++){
          if(hardPs_above.at(j).DeltaR(L3OITracks.at(k)) < 0.3){
            //cout << j << "th hardP muon matched with " << k << "th L3OItrack within dR: " << hardPs_above.at(j).DeltaR(L3OITracks.at(k)) << endl;
            //cout << "pt: " << hardPs_above.at(j).Pt() << " vs " << L3OITracks.at(k).Pt() << endl;
            matched = 1;
            FillHist("hardP_eta_trackeff_step1",hardPs_above.at(j).Eta(),1.,48,-2.4,2.4);
            FillHist("hardP_eta_trackeff_step2",hardPs_above.at(j).Eta(),1.,48,-2.4,2.4);
            FillHist("hardP_eta_trackeff_step3",hardPs_above.at(j).Eta(),1.,48,-2.4,2.4);
            FillHist("hardP_eta_trackeff_step4",hardPs_above.at(j).Eta(),1.,48,-2.4,2.4);
            FillHist("hardP_eta_trackeff_step5",hardPs_above.at(j).Eta(),1.,48,-2.4,2.4);
            FillHist("hardP_phi_trackeff_step1",hardPs_above.at(j).Phi(),1.,63,-3.15,3.15);
            FillHist("hardP_phi_trackeff_step2",hardPs_above.at(j).Phi(),1.,63,-3.15,3.15);
            FillHist("hardP_phi_trackeff_step3",hardPs_above.at(j).Phi(),1.,63,-3.15,3.15);
            FillHist("hardP_phi_trackeff_step4",hardPs_above.at(j).Phi(),1.,63,-3.15,3.15);
            FillHist("hardP_phi_trackeff_step5",hardPs_above.at(j).Phi(),1.,63,-3.15,3.15);
            FillHist("hardP_truePU_trackeff_step1",truePU,1.,100,0,100);
            FillHist("hardP_truePU_trackeff_step2",truePU,1.,100,0,100);
            FillHist("hardP_truePU_trackeff_step3",truePU,1.,100,0,100);
            FillHist("hardP_truePU_trackeff_step4",truePU,1.,100,0,100);
            FillHist("hardP_truePU_trackeff_step5",truePU,1.,100,0,100);
            break;
          }
        }
        if(matched == 0){
          for(int k=0; k<Iter0L3Tracks.size(); k++){
            if(hardPs_above.at(j).DeltaR(Iter0L3Tracks.at(k))<0.3){
              //cout << j << "th hardP muon matched with " << k << "th Iter0L3track within dR: " << hardPs_above.at(j).DeltaR(Iter0L3Tracks.at(k)) << endl;
              //cout << "pt: " << hardPs_above.at(j).Pt() << " vs " << Iter0L3Tracks.at(k).Pt() << endl;
              matched = 1;
              FillHist("hardP_eta_trackeff_step2",hardPs_above.at(j).Eta(),1.,48,-2.4,2.4);
              FillHist("hardP_eta_trackeff_step3",hardPs_above.at(j).Eta(),1.,48,-2.4,2.4);
              FillHist("hardP_eta_trackeff_step4",hardPs_above.at(j).Eta(),1.,48,-2.4,2.4);
              FillHist("hardP_eta_trackeff_step5",hardPs_above.at(j).Eta(),1.,48,-2.4,2.4);
              FillHist("hardP_phi_trackeff_step2",hardPs_above.at(j).Phi(),1.,63,-3.15,3.15);
              FillHist("hardP_phi_trackeff_step3",hardPs_above.at(j).Phi(),1.,63,-3.15,3.15);
              FillHist("hardP_phi_trackeff_step4",hardPs_above.at(j).Phi(),1.,63,-3.15,3.15);
              FillHist("hardP_phi_trackeff_step5",hardPs_above.at(j).Phi(),1.,63,-3.15,3.15);
              FillHist("hardP_truePU_trackeff_step2",truePU,1.,100,0,100);
              FillHist("hardP_truePU_trackeff_step3",truePU,1.,100,0,100);
              FillHist("hardP_truePU_trackeff_step4",truePU,1.,100,0,100);
              FillHist("hardP_truePU_trackeff_step5",truePU,1.,100,0,100);
              break;
            }
          }
        }
        if(matched == 0){
          for(int k=0; k<Iter2L3Tracks.size(); k++){
            if(hardPs_above.at(j).DeltaR(Iter2L3Tracks.at(k))<0.3){
              //cout << j << "th hardP muon matched with " << k << "th Iter2L3track within dR: " << hardPs_above.at(j).DeltaR(Iter2L3Tracks.at(k)) << endl;
              //cout << "pt: " << hardPs_above.at(j).Pt() << " vs " << Iter2L3Tracks.at(k).Pt() << endl;
              matched = 1;
              FillHist("hardP_eta_trackeff_step3",hardPs_above.at(j).Eta(),1.,48,-2.4,2.4);
              FillHist("hardP_eta_trackeff_step4",hardPs_above.at(j).Eta(),1.,48,-2.4,2.4);
              FillHist("hardP_eta_trackeff_step5",hardPs_above.at(j).Eta(),1.,48,-2.4,2.4);
              FillHist("hardP_phi_trackeff_step3",hardPs_above.at(j).Phi(),1.,63,-3.15,3.15);
              FillHist("hardP_phi_trackeff_step4",hardPs_above.at(j).Phi(),1.,63,-3.15,3.15);
              FillHist("hardP_phi_trackeff_step5",hardPs_above.at(j).Phi(),1.,63,-3.15,3.15);
              FillHist("hardP_truePU_trackeff_step3",truePU,1.,100,0,100);
              FillHist("hardP_truePU_trackeff_step4",truePU,1.,100,0,100);
              FillHist("hardP_truePU_trackeff_step5",truePU,1.,100,0,100);
              break;
            }
          }
        }
        if(matched == 0){
          for(int k=0; k<Iter0L3FromL1Tracks.size(); k++){
            if(hardPs_above.at(j).DeltaR(Iter0L3FromL1Tracks.at(k))<0.3){
              //cout << j << "th hardP muon matched with " << k << "th Iter0L3FromL1track within dR: " << hardPs_above.at(j).DeltaR(Iter0L3FromL1Tracks.at(k)) << endl;
              //cout << "pt: " << hardPs_above.at(j).Pt() << " vs " << Iter0L3FromL1Tracks.at(k).Pt() << endl;
              matched = 1;
              FillHist("hardP_eta_trackeff_step4",hardPs_above.at(j).Eta(),1.,48,-2.4,2.4);
              FillHist("hardP_eta_trackeff_step5",hardPs_above.at(j).Eta(),1.,48,-2.4,2.4);
              FillHist("hardP_phi_trackeff_step4",hardPs_above.at(j).Phi(),1.,63,-3.15,3.15);
              FillHist("hardP_phi_trackeff_step5",hardPs_above.at(j).Phi(),1.,63,-3.15,3.15);
              FillHist("hardP_truePU_trackeff_step4",truePU,1.,100,0,100);
              FillHist("hardP_truePU_trackeff_step5",truePU,1.,100,0,100);
              break;
            }
          }
        }
        if(matched == 0){
          for(int k=0; k<Iter2L3FromL1Tracks.size(); k++){
            if(hardPs_above.at(j).DeltaR(Iter2L3FromL1Tracks.at(k))<0.3){
              //cout << j << "th hardP muon matched with " << k << "th Iter2L3FromL1track within dR: " << hardPs_above.at(j).DeltaR(Iter2L3FromL1Tracks.at(k)) << endl;
              //cout << "pt: " << hardPs_above.at(j).Pt() << " vs " << Iter2L3FromL1Tracks.at(k).Pt() << endl;
              matched = 1;
              FillHist("hardP_eta_trackeff_step5",hardPs_above.at(j).Eta(),1.,48,-2.4,2.4);
              FillHist("hardP_phi_trackeff_step5",hardPs_above.at(j).Phi(),1.,63,-3.15,3.15);
              FillHist("hardP_truePU_trackeff_step5",truePU,1.,100,0,100);
              break;
            }
          }
        }
        if(matched == 1) Nmatched_hardP_tracks_others+=1;
      }
    
      //==Eff 1-2-1. hard procecss muon-L3 muon(NoID) matching==//
      this_matched.clear();
      for(int j=0; j<hardPs_above.size(); j++){
        for(int k=0; k<L3Muons_NoID.size(); k++){
          if( find(this_matched.begin(), this_matched.end(), k) == this_matched.end() ){
            if(hardPs_above.at(j).DeltaR(L3Muons_NoID.at(k)) < 0.1){
              //cout << j << "th hardP muon matched with " << k << "th L3OItrack within dR: " << hardPs_above.at(j).DeltaR(L3OITracks.at(k)) << endl;
              //cout << "pt: " << hardPs_above.at(j).Pt() << " vs " << L3OITracks.at(k).Pt() << endl;
              Nmatched_hardP_L3NoID_others+=1;
              FillHist("hardP_eta_eff_L3NoID",hardPs_above.at(j).Eta(),1.,48,-2.4,2.4);
              FillHist("hardP_phi_eff_L3NoID",hardPs_above.at(j).Phi(),1.,63,-3.15,3.15);
              FillHist("hardP_truePU_eff_L3NoID",truePU,1.,100,0,100);
              this_matched.push_back(k);
              break;
            }
          }
        }
      }
    
      //==Eff 1-2-2. hard procecss muon-L3 muon matching==//
      this_matched.clear();
      for(int j=0; j<hardPs_above.size(); j++){
        for(int k=0; k<L3Muons.size(); k++){
          if( find(this_matched.begin(), this_matched.end(), k) == this_matched.end() ){
            if(hardPs_above.at(j).DeltaR(L3Muons.at(k)) < 0.1){
              //cout << j << "th hardP muon matched with " << k << "th L3OItrack within dR: " << hardPs_above.at(j).DeltaR(L3OITracks.at(k)) << endl;
              //cout << "pt: " << hardPs_above.at(j).Pt() << " vs " << L3OITracks.at(k).Pt() << endl;
              Nmatched_hardP_L3_others+=1;
              FillHist("hardP_eta_eff_L3",hardPs_above.at(j).Eta(),1.,48,-2.4,2.4);
              FillHist("hardP_phi_eff_L3",hardPs_above.at(j).Phi(),1.,63,-3.15,3.15);
              FillHist("hardP_truePU_eff_L3",truePU,1.,100,0,100);
              this_matched.push_back(k);
              break;
            }
          }
        }
      }
    
    
      //==Eff 2-1. hard process muon matched with fine L1-tracker track matching==//
      for(int j=0; j<hardPs_above_L1matched.size(); j++){
        FillHist("hardP_L1_eta",hardPs_above_L1matched.at(j).Eta(),1.,48,-2.4,2.4);
        FillHist("hardP_L1_phi",hardPs_above_L1matched.at(j).Phi(),1.,63,-3.15,3.15);
        FillHist("hardP_L1_truePU",truePU,1.,100,0,100);
        int matched = 0;
        for(int k=0; k<L3OITracks.size(); k++){
          if(hardPs_above_L1matched.at(j).DeltaR(L3OITracks.at(k)) < 0.3){
            //cout << j << "th hardP muon matched with " << k << "th L3OItrack within dR: " << hardPs_above_L1matched.at(j).DeltaR(L3OITracks.at(k)) << endl;
            //cout << "pt: " << hardPs_above_L1matched.at(j).Pt() << " vs " << L3OITracks.at(k).Pt() << endl;
            matched = 1;
            FillHist("hardP_L1_eta_trackeff_step1",hardPs_above_L1matched.at(j).Eta(),1.,48,-2.4,2.4);
            FillHist("hardP_L1_eta_trackeff_step2",hardPs_above_L1matched.at(j).Eta(),1.,48,-2.4,2.4);
            FillHist("hardP_L1_eta_trackeff_step3",hardPs_above_L1matched.at(j).Eta(),1.,48,-2.4,2.4);
            FillHist("hardP_L1_eta_trackeff_step4",hardPs_above_L1matched.at(j).Eta(),1.,48,-2.4,2.4);
            FillHist("hardP_L1_eta_trackeff_step5",hardPs_above_L1matched.at(j).Eta(),1.,48,-2.4,2.4);
            FillHist("hardP_L1_phi_trackeff_step1",hardPs_above_L1matched.at(j).Phi(),1.,63,-3.15,3.15);
            FillHist("hardP_L1_phi_trackeff_step2",hardPs_above_L1matched.at(j).Phi(),1.,63,-3.15,3.15);
            FillHist("hardP_L1_phi_trackeff_step3",hardPs_above_L1matched.at(j).Phi(),1.,63,-3.15,3.15);
            FillHist("hardP_L1_phi_trackeff_step4",hardPs_above_L1matched.at(j).Phi(),1.,63,-3.15,3.15);
            FillHist("hardP_L1_phi_trackeff_step5",hardPs_above_L1matched.at(j).Phi(),1.,63,-3.15,3.15);
            FillHist("hardP_L1_truePU_trackeff_step1",truePU,1.,100,0,100);
            FillHist("hardP_L1_truePU_trackeff_step2",truePU,1.,100,0,100);
            FillHist("hardP_L1_truePU_trackeff_step3",truePU,1.,100,0,100);
            FillHist("hardP_L1_truePU_trackeff_step4",truePU,1.,100,0,100);
            FillHist("hardP_L1_truePU_trackeff_step5",truePU,1.,100,0,100);
            break;
          }
        }
        if(matched == 0){
          for(int k=0; k<Iter0L3Tracks.size(); k++){
            if(hardPs_above_L1matched.at(j).DeltaR(Iter0L3Tracks.at(k))<0.3){
              //cout << j << "th hardP muon matched with " << k << "th Iter0L3track within dR: " << hardPs_above_L1matched.at(j).DeltaR(Iter0L3Tracks.at(k)) << endl;
              //cout << "pt: " << hardPs_above_L1matched.at(j).Pt() << " vs " << Iter0L3Tracks.at(k).Pt() << endl;
              matched = 1;
              FillHist("hardP_L1_eta_trackeff_step2",hardPs_above_L1matched.at(j).Eta(),1.,48,-2.4,2.4);
              FillHist("hardP_L1_eta_trackeff_step3",hardPs_above_L1matched.at(j).Eta(),1.,48,-2.4,2.4);
              FillHist("hardP_L1_eta_trackeff_step4",hardPs_above_L1matched.at(j).Eta(),1.,48,-2.4,2.4);
              FillHist("hardP_L1_eta_trackeff_step5",hardPs_above_L1matched.at(j).Eta(),1.,48,-2.4,2.4);
              FillHist("hardP_L1_phi_trackeff_step2",hardPs_above_L1matched.at(j).Phi(),1.,63,-3.15,3.15);
              FillHist("hardP_L1_phi_trackeff_step3",hardPs_above_L1matched.at(j).Phi(),1.,63,-3.15,3.15);
              FillHist("hardP_L1_phi_trackeff_step4",hardPs_above_L1matched.at(j).Phi(),1.,63,-3.15,3.15);
              FillHist("hardP_L1_phi_trackeff_step5",hardPs_above_L1matched.at(j).Phi(),1.,63,-3.15,3.15);
              FillHist("hardP_L1_truePU_trackeff_step2",truePU,1.,100,0,100);
              FillHist("hardP_L1_truePU_trackeff_step3",truePU,1.,100,0,100);
              FillHist("hardP_L1_truePU_trackeff_step4",truePU,1.,100,0,100);
              FillHist("hardP_L1_truePU_trackeff_step5",truePU,1.,100,0,100);
              break;
            }
          }
        }
        if(matched == 0){
          for(int k=0; k<Iter2L3Tracks.size(); k++){
            if(hardPs_above_L1matched.at(j).DeltaR(Iter2L3Tracks.at(k))<0.3){
              //cout << j << "th hardP muon matched with " << k << "th Iter2L3track within dR: " << hardPs_above_L1matched.at(j).DeltaR(Iter2L3Tracks.at(k)) << endl;
              //cout << "pt: " << hardPs_above_L1matched.at(j).Pt() << " vs " << Iter2L3Tracks.at(k).Pt() << endl;
              matched = 1;
              FillHist("hardP_L1_eta_trackeff_step3",hardPs_above_L1matched.at(j).Eta(),1.,48,-2.4,2.4);
              FillHist("hardP_L1_eta_trackeff_step4",hardPs_above_L1matched.at(j).Eta(),1.,48,-2.4,2.4);
              FillHist("hardP_L1_eta_trackeff_step5",hardPs_above_L1matched.at(j).Eta(),1.,48,-2.4,2.4);
              FillHist("hardP_L1_phi_trackeff_step3",hardPs_above_L1matched.at(j).Phi(),1.,63,-3.15,3.15);
              FillHist("hardP_L1_phi_trackeff_step4",hardPs_above_L1matched.at(j).Phi(),1.,63,-3.15,3.15);
              FillHist("hardP_L1_phi_trackeff_step5",hardPs_above_L1matched.at(j).Phi(),1.,63,-3.15,3.15);
              FillHist("hardP_L1_truePU_trackeff_step3",truePU,1.,100,0,100);
              FillHist("hardP_L1_truePU_trackeff_step4",truePU,1.,100,0,100);
              FillHist("hardP_L1_truePU_trackeff_step5",truePU,1.,100,0,100);
              break;
            }
          }
        }
        if(matched == 0){
          for(int k=0; k<Iter0L3FromL1Tracks.size(); k++){
            if(hardPs_above_L1matched.at(j).DeltaR(Iter0L3FromL1Tracks.at(k))<0.3){
              //cout << j << "th hardP muon matched with " << k << "th Iter0L3FromL1track within dR: " << hardPs_above_L1matched.at(j).DeltaR(Iter0L3FromL1Tracks.at(k)) << endl;
              //cout << "pt: " << hardPs_above_L1matched.at(j).Pt() << " vs " << Iter0L3FromL1Tracks.at(k).Pt() << endl;
              matched = 1;
              FillHist("hardP_L1_eta_trackeff_step4",hardPs_above_L1matched.at(j).Eta(),1.,48,-2.4,2.4);
              FillHist("hardP_L1_eta_trackeff_step5",hardPs_above_L1matched.at(j).Eta(),1.,48,-2.4,2.4);
              FillHist("hardP_L1_phi_trackeff_step4",hardPs_above_L1matched.at(j).Phi(),1.,63,-3.15,3.15);
              FillHist("hardP_L1_phi_trackeff_step5",hardPs_above_L1matched.at(j).Phi(),1.,63,-3.15,3.15);
              FillHist("hardP_L1_truePU_trackeff_step4",truePU,1.,100,0,100);
              FillHist("hardP_L1_truePU_trackeff_step5",truePU,1.,100,0,100);
              break;
            }
          }
        }
        if(matched == 0){
          for(int k=0; k<Iter2L3FromL1Tracks.size(); k++){
            if(hardPs_above_L1matched.at(j).DeltaR(Iter2L3FromL1Tracks.at(k))<0.3){
              //cout << j << "th hardP muon matched with " << k << "th Iter2L3FromL1track within dR: " << hardPs_above_L1matched.at(j).DeltaR(Iter2L3FromL1Tracks.at(k)) << endl;
              //cout << "pt: " << hardPs_above_L1matched.at(j).Pt() << " vs " << Iter2L3FromL1Tracks.at(k).Pt() << endl;
              matched = 1;
              FillHist("hardP_L1_eta_trackeff_step5",hardPs_above_L1matched.at(j).Eta(),1.,48,-2.4,2.4);
              FillHist("hardP_L1_phi_trackeff_step5",hardPs_above_L1matched.at(j).Phi(),1.,63,-3.15,3.15);
              FillHist("hardP_L1_truePU_trackeff_step5",truePU,1.,100,0,100);
              break;
            }
          }
        }
      }
    
      //==Eff 2-2-1. hard procecss muon matched with fine L1-L3 muon(NoID) matching==//
      this_matched.clear();
      for(int j=0; j<hardPs_above_L1matched.size(); j++){
        for(int k=0; k<L3Muons_NoID.size(); k++){
          if( find(this_matched.begin(), this_matched.end(), k) == this_matched.end() ){
            if(hardPs_above_L1matched.at(j).DeltaR(L3Muons_NoID.at(k)) < 0.1){
              FillHist("hardP_L1_eta_eff_L3NoID",hardPs_above_L1matched.at(j).Eta(),1.,48,-2.4,2.4);
              FillHist("hardP_L1_phi_eff_L3NoID",hardPs_above_L1matched.at(j).Phi(),1.,63,-3.15,3.15);
              FillHist("hardP_L1_truePU_eff_L3NoID",truePU,1.,100,0,100);
              this_matched.push_back(k);
              break;
            }
          }
        }
      }
    
      //==Eff 2-2-2. hard procecss muon matched with fine L1-L3 muon matching==//
      this_matched.clear();
      for(int j=0; j<hardPs_above_L1matched.size(); j++){
        for(int k=0; k<L3Muons.size(); k++){
          if( find(this_matched.begin(), this_matched.end(), k) == this_matched.end() ){
            if(hardPs_above_L1matched.at(j).DeltaR(L3Muons.at(k)) < 0.1){
              FillHist("hardP_L1_eta_eff_L3",hardPs_above_L1matched.at(j).Eta(),1.,48,-2.4,2.4);
              FillHist("hardP_L1_phi_eff_L3",hardPs_above_L1matched.at(j).Phi(),1.,63,-3.15,3.15);
              FillHist("hardP_L1_truePU_eff_L3",truePU,1.,100,0,100);
              this_matched.push_back(k);
              break;
            }
          }
        }
      }
    
      //==Eff 2-3-1. hard procecss muon matched with medium L1-L3 muon(NoID) matching==//
      this_matched.clear();
      for(int j=0; j<hardPs_above_L1matched_med.size(); j++){
        FillHist("hardP_L1_med_eta",hardPs_above_L1matched_med.at(j).Eta(),1.,48,-2.4,2.4);
        FillHist("hardP_L1_med_phi",hardPs_above_L1matched_med.at(j).Phi(),1.,63,-3.15,3.15);
        FillHist("hardP_L1_med_truePU",truePU,1.,100,0,100);
        for(int k=0; k<L3Muons_NoID.size(); k++){
          if( find(this_matched.begin(), this_matched.end(), k) == this_matched.end() ){
            if(hardPs_above_L1matched_med.at(j).DeltaR(L3Muons_NoID.at(k)) < 0.1){
              FillHist("hardP_L1_med_eta_eff_L3NoID",hardPs_above_L1matched_med.at(j).Eta(),1.,48,-2.4,2.4);
              FillHist("hardP_L1_med_phi_eff_L3NoID",hardPs_above_L1matched_med.at(j).Phi(),1.,63,-3.15,3.15);
              FillHist("hardP_L1_med_truePU_eff_L3NoID",truePU,1.,100,0,100);
              this_matched.push_back(k);
              break;
            }
          }
        }
      }
    
      //==Eff 2-3-2. hard procecss muon matched with medium L1-L3 muon matching==//
      this_matched.clear();
      for(int j=0; j<hardPs_above_L1matched_med.size(); j++){
        for(int k=0; k<L3Muons.size(); k++){
          if( find(this_matched.begin(), this_matched.end(), k) == this_matched.end() ){
            if(hardPs_above_L1matched_med.at(j).DeltaR(L3Muons.at(k)) < 0.1){
              FillHist("hardP_L1_med_eta_eff_L3",hardPs_above_L1matched_med.at(j).Eta(),1.,48,-2.4,2.4);
              FillHist("hardP_L1_med_phi_eff_L3",hardPs_above_L1matched_med.at(j).Phi(),1.,63,-3.15,3.15);
              FillHist("hardP_L1_med_truePU_eff_L3",truePU,1.,100,0,100);
              this_matched.push_back(k);
              break;
            }
          }
        }
      }
 

    //==========================================Purity====================================================//
    
      //==Purity 1. L3 muon(NoID)-final muon matching==//
      this_matched.clear();
      for(int j=0; j<L3Muons_NoID.size(); j++){
        FillHist("L3NoID_pt",L3Muons_NoID.at(j).Pt(),1.,1500,0,1500);
        FillHist("L3NoID_eta",L3Muons_NoID.at(j).Eta(),1.,48,-2.4,2.4);
        FillHist("L3NoID_phi",L3Muons_NoID.at(j).Phi(),1.,63,-3.15,3.15);
        FillHist("L3NoID_truePU",truePU,1.,100,0,100);
        for(int k=0; k<Finals.size(); k++){
          if( find(this_matched.begin(), this_matched.end(), k) == this_matched.end() ){
            if(L3Muons_NoID.at(j).DeltaR(Finals.at(k)) < 0.1){
              Nmatched_L3NoID_Final+=1;
              FillHist("L3NoID_pt_purity",L3Muons_NoID.at(j).Pt(),1.,1500,0,1500);
              FillHist("L3NoID_eta_purity",L3Muons_NoID.at(j).Eta(),1.,48,-2.4,2.4);
              FillHist("L3NoID_phi_purity",L3Muons_NoID.at(j).Phi(),1.,63,-3.15,3.15);
              FillHist("L3NoID_truePU_purity",truePU,1.,100,0,100);
              this_matched.push_back(k);
              break;
            }
          }
        }
      }
    
      //==Purity 2. L3 muon-final muon matching==//
      this_matched.clear();
      for(int j=0; j<L3Muons.size(); j++){
        FillHist("L3_pt",L3Muons.at(j).Pt(),1.,1500,0,1500);
        FillHist("L3_eta",L3Muons.at(j).Eta(),1.,48,-2.4,2.4);
        FillHist("L3_phi",L3Muons.at(j).Phi(),1.,63,-3.15,3.15);
        FillHist("L3_truePU",truePU,1.,100,0,100);
        for(int k=0; k<Finals.size(); k++){
          if( find(this_matched.begin(), this_matched.end(), k) == this_matched.end() ){
            if(L3Muons.at(j).DeltaR(Finals.at(k)) < 0.1){
              //cout << j << "th L3 muon matched with " << k << "th final muon within dR: " << L3Muons.at(j).DeltaR(Finals.at(k)) << endl;
              //cout << "pt: " << L3Muons.at(j).Pt() << " vs " << Finals.at(k).Pt() << endl;
              Nmatched_L3_Final+=1;
              FillHist("L3_pt_purity",L3Muons.at(j).Pt(),1.,1500,0,1500);
              FillHist("L3_eta_purity",L3Muons.at(j).Eta(),1.,48,-2.4,2.4);
              FillHist("L3_phi_purity",L3Muons.at(j).Phi(),1.,63,-3.15,3.15);
              FillHist("L3_truePU_purity",truePU,1.,100,0,100);
              this_matched.push_back(k);
              break;
            }
          }
        }
      }

      //==Purity 3. Iter2FromL1 track-final muon matching==//
      this_matched.clear();
      for(int j=0; j<Iter2L3FromL1Tracks.size(); j++){
        FillHist("Iter2L3FromL1Track_pt",Iter2L3FromL1Tracks.at(j).Pt(),1.,1500,0,1500);
        FillHist("Iter2L3FromL1Track_eta",Iter2L3FromL1Tracks.at(j).Eta(),1.,48,-2.4,2.4);
        FillHist("Iter2L3FromL1Track_phi",Iter2L3FromL1Tracks.at(j).Phi(),1.,63,-3.15,3.15);
        FillHist("Iter2L3FromL1Track_truePU",truePU,1.,100,0,100);
        for(int k=0; k<Finals.size(); k++){
          if( find(this_matched.begin(), this_matched.end(), k) == this_matched.end() ){
            if(Iter2L3FromL1Tracks.at(j).DeltaR(Finals.at(k)) < 0.1){
              Nmatched_Iter2FromL1Track_Final+=1;
              FillHist("Iter2L3FromL1Track_pt_purity",Iter2L3FromL1Tracks.at(j).Pt(),1.,1500,0,1500);
              FillHist("Iter2L3FromL1Track_eta_purity",Iter2L3FromL1Tracks.at(j).Eta(),1.,48,-2.4,2.4);
              FillHist("Iter2L3FromL1Track_phi_purity",Iter2L3FromL1Tracks.at(j).Phi(),1.,63,-3.15,3.15);
              FillHist("Iter2L3FromL1Track_truePU_purity",truePU,1.,100,0,100);
              this_matched.push_back(k);
              break;
            }
          }
        }
      }
 
    pf_track->Fill(truePU,nhltIter2IterL3MuonTrack);
    pf_trackFromL1->Fill(truePU,nhltIter2IterL3FromL1MuonTrack);

    }
    
    cout << "=================================================" << endl;
    cout << "total NhardPs : " << NhardP << endl;
    cout << "total NhardPs above 26GeV : " << NhardP_above << endl;
    cout << "total track pt eff : " << Nmatched_hardP_tracks_pt/NhardP << endl;
    cout << "total track others eff : " << Nmatched_hardP_tracks_others/NhardP_above << endl;
    cout << "L3NoID pt eff : " << Nmatched_hardP_L3NoID_pt/NhardP << endl;
    cout << "L3NoID others eff : " << Nmatched_hardP_L3NoID_others/NhardP_above << endl;
    cout << "L3 pt eff : " << Nmatched_hardP_L3_pt/NhardP << endl;
    cout << "L3 others eff : " << Nmatched_hardP_L3_others/NhardP_above << endl;
    cout << "total NhardPs_L1 : " << NhardP_L1 << endl;
    cout << "total track pt eff : " << Nmatched_hardP_L1_tracks_pt/NhardP_L1 << endl;
    cout << "L3NoID pt eff : " << Nmatched_hardP_L1_L3NoID_pt/NhardP_L1 << endl;
    cout << "L3 pt eff : " << Nmatched_hardP_L1_L3_pt/NhardP_L1 << endl;
    cout << "=================================================" << endl;
    cout << "L3NoID purity : " << Nmatched_L3NoID_Final/NL3_NoID << endl;
    cout << "L3 purity : " << Nmatched_L3_Final/NL3 << endl;
    cout << "Iter2FromL1Track purity : " << Nmatched_Iter2FromL1Track_Final/NIter2FromL1Track << endl;
    cout << "/////////////////////////////////////////////////" << endl;

    TFile fout(output,"recreate");

    for(const auto& [histname,hist]:hists){
      hist->Write();
    }
    pf_track->Write();
    pf_trackFromL1->Write();
    fout.Close();

    for(auto& [histname,hist]:hists){
      delete hists[histname];
    }
    delete pf_track;
    delete pf_trackFromL1;

  //}

}



//TH1 -> GetCumulative (forward = false)
//First, draw Nsignal vs mva(sigmoid), Nbkg vs mva(sigmoid).
//Then calculate GetCumulative(some bin)/GetCumulative(0).
//Iter2 track, Iter2FromL1 track
