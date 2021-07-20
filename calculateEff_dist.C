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
map<TString,TH2D*> hists_2D;
map<TString,TProfile*> pfs;

void FillHist(TString histname, double value, double weight, int n_bin, double x_min, double x_max){
  auto it = hists.find(histname);
  TH1D* hist=NULL;
  if( it==hists.end() ){
    hist = new TH1D(histname, "", n_bin, x_min, x_max);
    hists[histname] = hist;
  }else hist=it->second;
  hist->Fill(value, weight);
}

void FillHist(TString histname, double value_x, double value_y, double weight, int n_binx, double x_min, double x_max, int n_biny, double y_min, double y_max){
  auto it = hists_2D.find(histname);
  TH2D* hist=NULL;
  if( it==hists_2D.end() ){
    hist = new TH2D(histname, "", n_binx, x_min, x_max, n_biny, y_min, y_max);
    hists_2D[histname] = hist;
  }else hist=it->second;
  hist->Fill(value_x, value_y, weight);
}

void FillProf(TString pfname, double value_x, double value_y, double weight, int n_binx, double x_min, double x_max, double y_min, double y_max){
  auto it = pfs.find(pfname);
  TProfile* pf=NULL;
  if( it==pfs.end() ){
    pf = new TProfile(pfname, "", n_binx, x_min, x_max, y_min, y_max);
    pfs[pfname] = pf;
  }else pf=it->second;
  pf->Fill(value_x, value_y, weight);
}

//My object = TLorentzVector + nametag + pileup//
class Object: public TLorentzVector{

private:

  TString j_tag;
  int j_truePU;

public:

  Object(){
    j_tag = "";
    j_truePU = -999;
  }

  ~Object() {}

  void SetTag(TString this_tag){
    j_tag = this_tag;
  }

  void SetTruePU(int this_truePU){
    j_truePU = this_truePU;
  }

  TString Tag(){
    return j_tag;
  }

  int truePU(){
    return j_truePU;
  }

};


void match(Object gens, Object L1){
//TODO
}


bool PtCompare(TLorentzVector p1, TLorentzVector p2){ return (p1.Pt() > p2.Pt()); }

void check_Jpsi_dR(vector<Object> gens, double weight){

  if(gens.size() != 2){
    return;
  }

  sort(gens.begin(), gens.end(), PtCompare);

  FillHist(gens.at(0).Tag()+"_pt1_dR_"+gens.at(0).Tag()+"_2D",gens.at(0).Pt(),gens.at(0).DeltaR(gens.at(1)),weight,3000,0,1500,1000,0,10);
  FillHist(gens.at(0).Tag()+"_pt2_dR_"+gens.at(0).Tag()+"_2D",gens.at(1).Pt(),gens.at(0).DeltaR(gens.at(1)),weight,3000,0,1500,1000,0,10);

}


void draw_dR(vector<Object> gens, vector<Object> recos, double weight){

  //cout << "Nrecos : " << recos.size() << endl;
  if(recos.size() == 0){
    return;
  }

  vector<int> this_matched;

  for(int j=0; j<gens.size(); j++){
    //cout << "for " << j << "th " << gens.at(j).Tag() << ":" << endl;
    double tmpDeltaR = 10.;
    int Matched = -1;
    for(int k=0; k<recos.size(); k++){
      //cout << "for " << k << "th " << recos.at(k).Tag() << ":" << endl;
      if( find(this_matched.begin(), this_matched.end(), k) == this_matched.end() ){
        Object tmpMatched = recos.at(k);
        if(gens.at(j).DeltaR(tmpMatched) < tmpDeltaR){
          tmpDeltaR = gens.at(j).DeltaR(tmpMatched);
          //cout << "tmpDeltaR : " << tmpDeltaR << endl;
          Matched = k;
        }
      }
    }
    if(Matched != -1){
      //cout << j << "th " << gens.at(j).Tag() << ", " << Matched << "th " << recos.at(Matched).Tag() << " : dR = " << gens.at(j).DeltaR(recos.at(Matched)) << endl;
      FillHist(gens.at(j).Tag()+"_pt_dR_"+recos.at(Matched).Tag()+"_2D",gens.at(j).Pt(),gens.at(j).DeltaR(recos.at(Matched)),weight,3000,0,1500,1000,0,10);
      FillHist(gens.at(j).Tag()+"_ptres_dR_"+recos.at(Matched).Tag()+"_2D",( recos.at(Matched).Pt()-gens.at(j).Pt() ) / gens.at(j).Pt(),gens.at(j).DeltaR(recos.at(Matched)),weight,10000,0,10,1000,0,10);
      FillProf(gens.at(j).Tag()+"_pt_dR_"+recos.at(Matched).Tag()+"_pf",gens.at(j).Pt(),gens.at(j).DeltaR(recos.at(Matched)),weight,3000,0,1500,0,10);
      this_matched.push_back(Matched);
    }
  }

}

void do_eff_den_pt(vector<Object> gens, double weight){

  for(int j=0; j<gens.size(); j++){
    FillHist(gens.at(j).Tag()+"_pt",gens.at(j).Pt(),weight,3000,0,1500);
  }

}

void do_eff_den_others(vector<Object> gens_above, double weight){

  for(int j=0; j<gens_above.size(); j++){
    FillHist(gens_above.at(j).Tag()+"_eta",gens_above.at(j).Eta(),weight,48,-2.4,2.4);
    FillHist(gens_above.at(j).Tag()+"_phi",gens_above.at(j).Phi(),weight,63,-3.15,3.15);
    FillHist(gens_above.at(j).Tag()+"_truePU",gens_above.at(j).truePU(),weight,100,0,100);
  }

}

void do_neweff_den_vars(vector<Object> dens, double weight){

  for(int j=0; j<dens.size(); j++){
    FillHist(dens.at(j).Tag()+"_pt",dens.at(j).Pt(),weight,3000,0,1500);
    FillHist(dens.at(j).Tag()+"_eta",dens.at(j).Eta(),weight,48,-2.4,2.4);
    FillHist(dens.at(j).Tag()+"_phi",dens.at(j).Phi(),weight,63,-3.15,3.15);
    FillHist(dens.at(j).Tag()+"_truePU",dens.at(j).truePU(),weight,100,0,100);
  }

}

void do_neweff_num_vars(vector<Object> nums, double weight){

  for(int j=0; j<nums.size(); j++){
    FillHist("TP_pt_eff_"+nums.at(j).Tag(),nums.at(j).Pt(),weight,3000,0,1500);
    FillHist("TP_eta_eff_"+nums.at(j).Tag(),nums.at(j).Eta(),weight,48,-2.4,2.4);
    FillHist("TP_phi_eff_"+nums.at(j).Tag(),nums.at(j).Phi(),weight,63,-3.15,3.15);
    FillHist("TP_truePU_eff_"+nums.at(j).Tag(),nums.at(j).truePU(),weight,100,0,100);
  }

}

void do_trackeff_pt(vector<Object> gens, vector< vector<Object> > Tracks, double dR, double weight, TString step){

  if(step == "all"){

    for(int j=0; j<gens.size(); j++){
      int matched = 0;
      for(int k=0; k<Tracks[0].size(); k++){
        if(gens.at(j).DeltaR(Tracks[0].at(k)) < dR){
          matched = 1;
          FillHist(gens.at(j).Tag()+"_pt_trackeff_step1",gens.at(j).Pt(),weight,3000,0,1500);
          FillHist(gens.at(j).Tag()+"_pt_trackeff_step2",gens.at(j).Pt(),weight,3000,0,1500);
          FillHist(gens.at(j).Tag()+"_pt_trackeff_step3",gens.at(j).Pt(),weight,3000,0,1500);
          FillHist(gens.at(j).Tag()+"_pt_trackeff_step4",gens.at(j).Pt(),weight,3000,0,1500);
          FillHist(gens.at(j).Tag()+"_pt_trackeff_step5",gens.at(j).Pt(),weight,3000,0,1500);
          break;
        }
      }
      if(matched == 0){
        for(int k=0; k<Tracks[1].size(); k++){
          if(gens.at(j).DeltaR(Tracks[1].at(k)) < dR){
            matched = 1;
            FillHist(gens.at(j).Tag()+"_pt_trackeff_step2",gens.at(j).Pt(),weight,3000,0,1500);
            FillHist(gens.at(j).Tag()+"_pt_trackeff_step3",gens.at(j).Pt(),weight,3000,0,1500);
            FillHist(gens.at(j).Tag()+"_pt_trackeff_step4",gens.at(j).Pt(),weight,3000,0,1500);
            FillHist(gens.at(j).Tag()+"_pt_trackeff_step5",gens.at(j).Pt(),weight,3000,0,1500);
            break;
          }
        }
      }
      if(matched == 0){
        for(int k=0; k<Tracks[2].size(); k++){
          if(gens.at(j).DeltaR(Tracks[2].at(k)) < dR){
            matched = 1;
            FillHist(gens.at(j).Tag()+"_pt_trackeff_step3",gens.at(j).Pt(),weight,3000,0,1500);
            FillHist(gens.at(j).Tag()+"_pt_trackeff_step4",gens.at(j).Pt(),weight,3000,0,1500);
            FillHist(gens.at(j).Tag()+"_pt_trackeff_step5",gens.at(j).Pt(),weight,3000,0,1500);
            break;
          }
        }
      }
      if(matched == 0){
        for(int k=0; k<Tracks[3].size(); k++){
          if(gens.at(j).DeltaR(Tracks[3].at(k)) < dR){
            matched = 1;
            FillHist(gens.at(j).Tag()+"_pt_trackeff_step4",gens.at(j).Pt(),weight,3000,0,1500);
            FillHist(gens.at(j).Tag()+"_pt_trackeff_step5",gens.at(j).Pt(),weight,3000,0,1500);
            break;
          }
        }
      }
      if(matched == 0){
        for(int k=0; k<Tracks[4].size(); k++){
          if(gens.at(j).DeltaR(Tracks[4].at(k)) < dR){
            matched = 1;
            FillHist(gens.at(j).Tag()+"_pt_trackeff_step5",gens.at(j).Pt(),weight,3000,0,1500);
            break;
          }
        }
      }
      //if(matched == 1) Nmatched_hardP_tracks_pt+=1;
    }

    for(int this_step=0; this_step<Tracks.size(); this_step++){

      vector<int> this_matched;

      for(int j=0; j<gens.size(); j++){
        for(int k=0; k<Tracks[this_step].size(); k++){
          if( find(this_matched.begin(), this_matched.end(), k) == this_matched.end() ){
            if(gens.at(j).DeltaR(Tracks[this_step].at(k)) < dR){
              FillHist(gens.at(j).Tag()+"_pt_eff_"+Tracks[this_step].at(k).Tag(),gens.at(j).Pt(),weight,3000,0,1500);
              this_matched.push_back(k);
              break;
            }
          }
        }
      }

    }

  }

  else if(step.Contains("step")){

    int this_step = step[step.Length()-1]-'0' -1; //JH : char - '0' = int. see https://stackoverflow.com/questions/439573/how-to-convert-a-single-char-into-an-int

    vector<int> this_matched;
    for(int j=0; j<gens.size(); j++){
      for(int k=0; k<Tracks[this_step].size(); k++){
        if( find(this_matched.begin(), this_matched.end(), k) == this_matched.end() ){
          if(gens.at(j).DeltaR(Tracks[this_step].at(k)) < dR){
            FillHist(gens.at(j).Tag()+"_pt_eff_"+Tracks[this_step].at(k).Tag(),gens.at(j).Pt(),weight,3000,0,1500);
            this_matched.push_back(k);
            break;
          }
        }
      }
    }

  }

}

void do_trackeff_others(vector<Object> gens_above, vector< vector<Object> > Tracks, double dR, double weight, TString step){

  if(step == "all"){

    for(int j=0; j<gens_above.size(); j++){
      int matched = 0;
      for(int k=0; k<Tracks[0].size(); k++){
        if(gens_above.at(j).DeltaR(Tracks[0].at(k)) < dR){
          matched = 1;
          FillHist(gens_above.at(j).Tag()+"_eta_trackeff_step1",gens_above.at(j).Eta(),weight,48,-2.4,2.4);
          FillHist(gens_above.at(j).Tag()+"_eta_trackeff_step2",gens_above.at(j).Eta(),weight,48,-2.4,2.4);
          FillHist(gens_above.at(j).Tag()+"_eta_trackeff_step3",gens_above.at(j).Eta(),weight,48,-2.4,2.4);
          FillHist(gens_above.at(j).Tag()+"_eta_trackeff_step4",gens_above.at(j).Eta(),weight,48,-2.4,2.4);
          FillHist(gens_above.at(j).Tag()+"_eta_trackeff_step5",gens_above.at(j).Eta(),weight,48,-2.4,2.4);
          FillHist(gens_above.at(j).Tag()+"_phi_trackeff_step1",gens_above.at(j).Phi(),weight,63,-3.15,3.15);
          FillHist(gens_above.at(j).Tag()+"_phi_trackeff_step2",gens_above.at(j).Phi(),weight,63,-3.15,3.15);
          FillHist(gens_above.at(j).Tag()+"_phi_trackeff_step3",gens_above.at(j).Phi(),weight,63,-3.15,3.15);
          FillHist(gens_above.at(j).Tag()+"_phi_trackeff_step4",gens_above.at(j).Phi(),weight,63,-3.15,3.15);
          FillHist(gens_above.at(j).Tag()+"_phi_trackeff_step5",gens_above.at(j).Phi(),weight,63,-3.15,3.15);
          FillHist(gens_above.at(j).Tag()+"_truePU_trackeff_step1",gens_above.at(j).truePU(),weight,100,0,100);
          FillHist(gens_above.at(j).Tag()+"_truePU_trackeff_step2",gens_above.at(j).truePU(),weight,100,0,100);
          FillHist(gens_above.at(j).Tag()+"_truePU_trackeff_step3",gens_above.at(j).truePU(),weight,100,0,100);
          FillHist(gens_above.at(j).Tag()+"_truePU_trackeff_step4",gens_above.at(j).truePU(),weight,100,0,100);
          FillHist(gens_above.at(j).Tag()+"_truePU_trackeff_step5",gens_above.at(j).truePU(),weight,100,0,100);
          break;
        }
      }
      if(matched == 0){
        for(int k=0; k<Tracks[1].size(); k++){
          if(gens_above.at(j).DeltaR(Tracks[1].at(k)) < dR){
            matched = 1;
            FillHist(gens_above.at(j).Tag()+"_eta_trackeff_step2",gens_above.at(j).Eta(),weight,48,-2.4,2.4);
            FillHist(gens_above.at(j).Tag()+"_eta_trackeff_step3",gens_above.at(j).Eta(),weight,48,-2.4,2.4);
            FillHist(gens_above.at(j).Tag()+"_eta_trackeff_step4",gens_above.at(j).Eta(),weight,48,-2.4,2.4);
            FillHist(gens_above.at(j).Tag()+"_eta_trackeff_step5",gens_above.at(j).Eta(),weight,48,-2.4,2.4);
            FillHist(gens_above.at(j).Tag()+"_phi_trackeff_step2",gens_above.at(j).Phi(),weight,63,-3.15,3.15);
            FillHist(gens_above.at(j).Tag()+"_phi_trackeff_step3",gens_above.at(j).Phi(),weight,63,-3.15,3.15);
            FillHist(gens_above.at(j).Tag()+"_phi_trackeff_step4",gens_above.at(j).Phi(),weight,63,-3.15,3.15);
            FillHist(gens_above.at(j).Tag()+"_phi_trackeff_step5",gens_above.at(j).Phi(),weight,63,-3.15,3.15);
            FillHist(gens_above.at(j).Tag()+"_truePU_trackeff_step2",gens_above.at(j).truePU(),weight,100,0,100);
            FillHist(gens_above.at(j).Tag()+"_truePU_trackeff_step3",gens_above.at(j).truePU(),weight,100,0,100);
            FillHist(gens_above.at(j).Tag()+"_truePU_trackeff_step4",gens_above.at(j).truePU(),weight,100,0,100);
            FillHist(gens_above.at(j).Tag()+"_truePU_trackeff_step5",gens_above.at(j).truePU(),weight,100,0,100);
            break;
          }
        }
      }
      if(matched == 0){
        for(int k=0; k<Tracks[2].size(); k++){
          if(gens_above.at(j).DeltaR(Tracks[2].at(k)) < dR){
            matched = 1;
            FillHist(gens_above.at(j).Tag()+"_eta_trackeff_step3",gens_above.at(j).Eta(),weight,48,-2.4,2.4);
            FillHist(gens_above.at(j).Tag()+"_eta_trackeff_step4",gens_above.at(j).Eta(),weight,48,-2.4,2.4);
            FillHist(gens_above.at(j).Tag()+"_eta_trackeff_step5",gens_above.at(j).Eta(),weight,48,-2.4,2.4);
            FillHist(gens_above.at(j).Tag()+"_phi_trackeff_step3",gens_above.at(j).Phi(),weight,63,-3.15,3.15);
            FillHist(gens_above.at(j).Tag()+"_phi_trackeff_step4",gens_above.at(j).Phi(),weight,63,-3.15,3.15);
            FillHist(gens_above.at(j).Tag()+"_phi_trackeff_step5",gens_above.at(j).Phi(),weight,63,-3.15,3.15);
            FillHist(gens_above.at(j).Tag()+"_truePU_trackeff_step3",gens_above.at(j).truePU(),weight,100,0,100);
            FillHist(gens_above.at(j).Tag()+"_truePU_trackeff_step4",gens_above.at(j).truePU(),weight,100,0,100);
            FillHist(gens_above.at(j).Tag()+"_truePU_trackeff_step5",gens_above.at(j).truePU(),weight,100,0,100);
            break;
          }
        }
      }
      if(matched == 0){
        for(int k=0; k<Tracks[3].size(); k++){
          if(gens_above.at(j).DeltaR(Tracks[3].at(k)) < dR){
            matched = 1;
            FillHist(gens_above.at(j).Tag()+"_eta_trackeff_step4",gens_above.at(j).Eta(),weight,48,-2.4,2.4);
            FillHist(gens_above.at(j).Tag()+"_eta_trackeff_step5",gens_above.at(j).Eta(),weight,48,-2.4,2.4);
            FillHist(gens_above.at(j).Tag()+"_phi_trackeff_step4",gens_above.at(j).Phi(),weight,63,-3.15,3.15);
            FillHist(gens_above.at(j).Tag()+"_phi_trackeff_step5",gens_above.at(j).Phi(),weight,63,-3.15,3.15);
            FillHist(gens_above.at(j).Tag()+"_truePU_trackeff_step4",gens_above.at(j).truePU(),weight,100,0,100);
            FillHist(gens_above.at(j).Tag()+"_truePU_trackeff_step5",gens_above.at(j).truePU(),weight,100,0,100);
            break;
          }
        }
      }
      if(matched == 0){
        for(int k=0; k<Tracks[4].size(); k++){
          if(gens_above.at(j).DeltaR(Tracks[4].at(k)) < dR){
            matched = 1;
            FillHist(gens_above.at(j).Tag()+"_eta_trackeff_step5",gens_above.at(j).Eta(),weight,48,-2.4,2.4);
            FillHist(gens_above.at(j).Tag()+"_phi_trackeff_step5",gens_above.at(j).Phi(),weight,63,-3.15,3.15);
            FillHist(gens_above.at(j).Tag()+"_truePU_trackeff_step5",gens_above.at(j).truePU(),weight,100,0,100);
            break;
          }
        }
      }
      //if(matched == 1) Nmatched_hardP_tracks_pt+=1;
    }


    for(int this_step=0; this_step<Tracks.size(); this_step++){

      vector<int> this_matched;

      for(int j=0; j<gens_above.size(); j++){
        for(int k=0; k<Tracks[this_step].size(); k++){
          if( find(this_matched.begin(), this_matched.end(), k) == this_matched.end() ){
            if(gens_above.at(j).DeltaR(Tracks[this_step].at(k)) < dR){
              FillHist(gens_above.at(j).Tag()+"_eta_eff_"+Tracks[this_step].at(k).Tag(),gens_above.at(j).Eta(),weight,48,-2.4,2.4);
              FillHist(gens_above.at(j).Tag()+"_phi_eff_"+Tracks[this_step].at(k).Tag(),gens_above.at(j).Phi(),weight,63,-3.15,3.15);
              FillHist(gens_above.at(j).Tag()+"_truePU_eff_"+Tracks[this_step].at(k).Tag(),gens_above.at(j).truePU(),weight,100,0,100);
              this_matched.push_back(k);
              break;
            }
          }
        }
      }

    }

  }

  else if(step.Contains("step")){

    int this_step = step[step.Length()-1]-'0' -1; //JH : char - '0' = int. see https://stackoverflow.com/questions/439573/how-to-convert-a-single-char-into-an-int

      vector<int> this_matched;

      for(int j=0; j<gens_above.size(); j++){
        for(int k=0; k<Tracks[this_step].size(); k++){
          if( find(this_matched.begin(), this_matched.end(), k) == this_matched.end() ){
            if(gens_above.at(j).DeltaR(Tracks[this_step].at(k)) < dR){
              FillHist(gens_above.at(j).Tag()+"_eta_eff_"+Tracks[this_step].at(k).Tag(),gens_above.at(j).Eta(),weight,48,-2.4,2.4);
              FillHist(gens_above.at(j).Tag()+"_phi_eff_"+Tracks[this_step].at(k).Tag(),gens_above.at(j).Phi(),weight,63,-3.15,3.15);
              FillHist(gens_above.at(j).Tag()+"_truePU_eff_"+Tracks[this_step].at(k).Tag(),gens_above.at(j).truePU(),weight,100,0,100);
              this_matched.push_back(k);
              break;
            }
          }
        }
      }

  }

}

void do_eff_num_pt(vector<Object> gens, vector<Object> recos, double dR, double weight){

  vector<int> this_matched;

  for(int j=0; j<gens.size(); j++){
    for(int k=0; k<recos.size(); k++){
      if( find(this_matched.begin(), this_matched.end(), k) == this_matched.end() ){
        if(gens.at(j).DeltaR(recos.at(k)) < dR){
          //cout << j << "th gen muon matched with " << k << "th reco within dR: " << gens.at(j).DeltaR(recos.at(k)) << endl;
          //cout << "pt: " << gens.at(j).Pt() << " vs " << recos.at(k).Pt() << endl;
          //Nmatched_hardP_L3NoID_pt+=1;
          FillHist(gens.at(j).Tag()+"_pt_eff_"+recos.at(k).Tag(),gens.at(j).Pt(),weight,3000,0,1500);
          this_matched.push_back(k);
          break;
        }
      }
    }
  }

}

void do_eff_num_others(vector<Object> gens_above, vector<Object> recos, double dR, double weight){

  vector<int> this_matched;

  for(int j=0; j<gens_above.size(); j++){
    for(int k=0; k<recos.size(); k++){
      if( find(this_matched.begin(), this_matched.end(), k) == this_matched.end() ){
        if(gens_above.at(j).DeltaR(recos.at(k)) < dR){
          //cout << j << "th gen muon matched with " << k << "th reco within dR: " << gens_above.at(j).DeltaR(recos.at(k)) << endl;
          //cout << "pt: " << gens_above.at(j).Pt() << " vs " << recos.at(k).Pt() << endl;
          //Nmatched_hardP_L3NoID_others+=1;
          FillHist(gens_above.at(j).Tag()+"_eta_eff_"+recos.at(k).Tag(),gens_above.at(j).Eta(),weight,48,-2.4,2.4);
          FillHist(gens_above.at(j).Tag()+"_phi_eff_"+recos.at(k).Tag(),gens_above.at(j).Phi(),weight,63,-3.15,3.15);
          FillHist(gens_above.at(j).Tag()+"_truePU_eff_"+recos.at(k).Tag(),gens_above.at(j).truePU(),weight,100,0,100);
          this_matched.push_back(k);
          break;
        }
      }
    }
  }

}

void do_purity(vector<Object> recos, vector<Object> Finals, double dR, double weight){

  vector<int> this_matched;

  for(int j=0; j<recos.size(); j++){
    FillHist(recos.at(j).Tag()+"_pt",recos.at(j).Pt(),weight,3000,0,1500);
    FillHist(recos.at(j).Tag()+"_eta",recos.at(j).Eta(),weight,48,-2.4,2.4);
    FillHist(recos.at(j).Tag()+"_phi",recos.at(j).Phi(),weight,63,-3.15,3.15);
    FillHist(recos.at(j).Tag()+"_truePU",recos.at(j).truePU(),weight,100,0,100);
    for(int k=0; k<Finals.size(); k++){
      if( find(this_matched.begin(), this_matched.end(), k) == this_matched.end() ){
        if(recos.at(j).DeltaR(Finals.at(k)) < dR){
          //Nmatched_L3NoID_Final+=1;
          FillHist(recos.at(j).Tag()+"_pt_purity",recos.at(j).Pt(),weight,3000,0,1500);
          FillHist(recos.at(j).Tag()+"_eta_purity",recos.at(j).Eta(),weight,48,-2.4,2.4);
          FillHist(recos.at(j).Tag()+"_phi_purity",recos.at(j).Phi(),weight,63,-3.15,3.15);
          FillHist(recos.at(j).Tag()+"_truePU_purity",recos.at(j).truePU(),weight,100,0,100);
          this_matched.push_back(k);
          break;
        }
      }
    }
  }

}

void do_purity(vector<Object> recos, vector<Object> recos_Asso, vector<Object> Finals, double dR, double weight){

  vector<int> this_matched;

  for(int j=0; j<recos.size(); j++){
    FillHist(recos.at(j).Tag()+"_pt",recos.at(j).Pt(),weight,3000,0,1500);
    FillHist(recos.at(j).Tag()+"_eta",recos.at(j).Eta(),weight,48,-2.4,2.4);
    FillHist(recos.at(j).Tag()+"_phi",recos.at(j).Phi(),weight,63,-3.15,3.15);
    FillHist(recos.at(j).Tag()+"_truePU",recos.at(j).truePU(),weight,100,0,100);
    for(int k=0; k<Finals.size(); k++){
      if( find(this_matched.begin(), this_matched.end(), k) == this_matched.end() ){
        if(recos.at(j).DeltaR(Finals.at(k)) < dR){
          //Nmatched_L3NoID_Final+=1;
          FillHist(recos.at(j).Tag()+"_pt_purity",recos.at(j).Pt(),weight,3000,0,1500);
          FillHist(recos.at(j).Tag()+"_eta_purity",recos.at(j).Eta(),weight,48,-2.4,2.4);
          FillHist(recos.at(j).Tag()+"_phi_purity",recos.at(j).Phi(),weight,63,-3.15,3.15);
          FillHist(recos.at(j).Tag()+"_truePU_purity",recos.at(j).truePU(),weight,100,0,100);
          this_matched.push_back(k);
          break;
        }
      }
    }
  }

  for(int j=0; j<recos_Asso.size(); j++){
    FillHist(recos.at(j).Tag()+"_pt_Asso",recos_Asso.at(j).Pt(),weight,3000,0,1500);
    FillHist(recos.at(j).Tag()+"_eta_Asso",recos_Asso.at(j).Eta(),weight,48,-2.4,2.4);
    FillHist(recos.at(j).Tag()+"_phi_Asso",recos_Asso.at(j).Phi(),weight,63,-3.15,3.15);
    FillHist(recos.at(j).Tag()+"_truePU_Asso",recos_Asso.at(j).truePU(),weight,100,0,100);
  }

}

void calculateEff_dist(TString input, TString output){

  vector<TString> inputs;
  ifstream in(input);
  string line;

  while(getline(in,line)){
    istringstream is(line);
    TString this_line = line;
    if(this_line(0,1)=="#"||this_line=="") continue;

    TString this_input;
    is >> this_input;
    inputs.push_back(this_input);
  }

  //==Set tree name and root files==//
  TChain *fChain = new TChain("ntupler/ntuple");

  for (int i=0; i<inputs.size(); i++){
    fChain->Add(inputs.at(i));
  }


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
  double L1Muon_etaAtVtx[5000];
  double L1Muon_phiAtVtx[5000];
  double L1Muon_charge[5000];
  double L1Muon_quality[5000];
  int nIterL3IOFromL1;
  double iterL3IOFromL1_pt[5000];
  double iterL3IOFromL1_eta[5000];
  double iterL3IOFromL1_phi[5000];
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
  int nhltIterL3OIMuonTrack;
  vector<double> *hltIterL3OIMuonTrack_pt = 0;
  vector<double> *hltIterL3OIMuonTrack_eta = 0;
  vector<double> *hltIterL3OIMuonTrack_phi = 0;
  int nhltIter0IterL3MuonTrack;
  vector<double> *hltIter0IterL3MuonTrack_pt = 0;
  vector<double> *hltIter0IterL3MuonTrack_eta = 0;
  vector<double> *hltIter0IterL3MuonTrack_phi = 0;
  int nhltIter2IterL3MuonTrack;
  vector<double> *hltIter2IterL3MuonTrack_pt = 0;
  vector<double> *hltIter2IterL3MuonTrack_eta = 0;
  vector<double> *hltIter2IterL3MuonTrack_phi = 0;
  //vector<double> *hltIter3IterL3MuonTrack_pt = 0;
  //vector<double> *hltIter3IterL3MuonTrack_eta = 0;
  //vector<double> *hltIter3IterL3MuonTrack_phi = 0;
  int nhltIter0IterL3FromL1MuonTrack;
  vector<double> *hltIter0IterL3FromL1MuonTrack_pt = 0;
  vector<double> *hltIter0IterL3FromL1MuonTrack_eta = 0;
  vector<double> *hltIter0IterL3FromL1MuonTrack_phi = 0;
  int nhltIter2IterL3FromL1MuonTrack;
  vector<double> *hltIter2IterL3FromL1MuonTrack_pt = 0;
  vector<double> *hltIter2IterL3FromL1MuonTrack_eta = 0;
  vector<double> *hltIter2IterL3FromL1MuonTrack_phi = 0;
  //vector<double> *hltIter3IterL3FromL1MuonTrack_pt = 0;
  //vector<double> *hltIter3IterL3FromL1MuonTrack_eta = 0;
  //vector<double> *hltIter3IterL3FromL1MuonTrack_phi = 0;
  int nhltIterL3OIMuonTrackAssociated;
  vector<double> *hltIterL3OIMuonTrackAssociated_pt = 0;
  vector<double> *hltIterL3OIMuonTrackAssociated_eta = 0;
  vector<double> *hltIterL3OIMuonTrackAssociated_phi = 0;
  vector<double> *hltIterL3OIMuonTrackAssociated_bestMatchTP_pt = 0;
  vector<double> *hltIterL3OIMuonTrackAssociated_bestMatchTP_eta = 0;
  vector<double> *hltIterL3OIMuonTrackAssociated_bestMatchTP_phi = 0;
  vector<double> *hltIterL3OIMuonTrackAssociated_bestMatchTP_energy = 0;
  vector<int> *hltIterL3OIMuonTrackAssociated_bestMatchTP_status = 0;
  vector<int> *hltIterL3OIMuonTrackAssociated_bestMatchTP_pdgId = 0;
  int ntpTo_hltIterL3OIMuonTrackAssociated;
  vector<double> *tpTo_hltIterL3OIMuonTrackAssociated_gen_pt = 0;
  vector<double> *tpTo_hltIterL3OIMuonTrackAssociated_gen_eta = 0;
  vector<double> *tpTo_hltIterL3OIMuonTrackAssociated_gen_phi = 0;
  vector<double> *tpTo_hltIterL3OIMuonTrackAssociated_bestMatchTrk_pt = 0;
  vector<double> *tpTo_hltIterL3OIMuonTrackAssociated_bestMatchTrk_eta = 0;
  vector<double> *tpTo_hltIterL3OIMuonTrackAssociated_bestMatchTrk_phi = 0;
  int nhltIter0IterL3MuonTrackAssociated;
  vector<double> *hltIter0IterL3MuonTrackAssociated_pt = 0;
  vector<double> *hltIter0IterL3MuonTrackAssociated_eta = 0;
  vector<double> *hltIter0IterL3MuonTrackAssociated_phi = 0;
  vector<double> *hltIter0IterL3MuonTrackAssociated_bestMatchTP_pt = 0;
  vector<double> *hltIter0IterL3MuonTrackAssociated_bestMatchTP_eta = 0;
  vector<double> *hltIter0IterL3MuonTrackAssociated_bestMatchTP_phi = 0;
  vector<double> *hltIter0IterL3MuonTrackAssociated_bestMatchTP_energy = 0;
  vector<int> *hltIter0IterL3MuonTrackAssociated_bestMatchTP_status = 0;
  vector<int> *hltIter0IterL3MuonTrackAssociated_bestMatchTP_pdgId = 0;
  int ntpTo_hltIter0IterL3MuonTrackAssociated;
  vector<double> *tpTo_hltIter0IterL3MuonTrackAssociated_gen_pt = 0;
  vector<double> *tpTo_hltIter0IterL3MuonTrackAssociated_gen_eta = 0;
  vector<double> *tpTo_hltIter0IterL3MuonTrackAssociated_gen_phi = 0;
  vector<double> *tpTo_hltIter0IterL3MuonTrackAssociated_bestMatchTrk_pt = 0;
  vector<double> *tpTo_hltIter0IterL3MuonTrackAssociated_bestMatchTrk_eta = 0;
  vector<double> *tpTo_hltIter0IterL3MuonTrackAssociated_bestMatchTrk_phi = 0;
  int nhltIter2IterL3MuonTrackAssociated;
  vector<double> *hltIter2IterL3MuonTrackAssociated_pt = 0;
  vector<double> *hltIter2IterL3MuonTrackAssociated_eta = 0;
  vector<double> *hltIter2IterL3MuonTrackAssociated_phi = 0;
  vector<double> *hltIter2IterL3MuonTrackAssociated_bestMatchTP_pt = 0;
  vector<double> *hltIter2IterL3MuonTrackAssociated_bestMatchTP_eta = 0;
  vector<double> *hltIter2IterL3MuonTrackAssociated_bestMatchTP_phi = 0;
  vector<double> *hltIter2IterL3MuonTrackAssociated_bestMatchTP_energy = 0;
  vector<int> *hltIter2IterL3MuonTrackAssociated_bestMatchTP_status = 0;
  vector<int> *hltIter2IterL3MuonTrackAssociated_bestMatchTP_pdgId = 0;
  int ntpTo_hltIter2IterL3MuonTrackAssociated;
  vector<double> *tpTo_hltIter2IterL3MuonTrackAssociated_gen_pt = 0;
  vector<double> *tpTo_hltIter2IterL3MuonTrackAssociated_gen_eta = 0;
  vector<double> *tpTo_hltIter2IterL3MuonTrackAssociated_gen_phi = 0;
  vector<double> *tpTo_hltIter2IterL3MuonTrackAssociated_bestMatchTrk_pt = 0;
  vector<double> *tpTo_hltIter2IterL3MuonTrackAssociated_bestMatchTrk_eta = 0;
  vector<double> *tpTo_hltIter2IterL3MuonTrackAssociated_bestMatchTrk_phi = 0;
  int nhltIter0IterL3FromL1MuonTrackAssociated;
  vector<double> *hltIter0IterL3FromL1MuonTrackAssociated_pt = 0;
  vector<double> *hltIter0IterL3FromL1MuonTrackAssociated_eta = 0;
  vector<double> *hltIter0IterL3FromL1MuonTrackAssociated_phi = 0;
  vector<double> *hltIter0IterL3FromL1MuonTrackAssociated_bestMatchTP_pt = 0;
  vector<double> *hltIter0IterL3FromL1MuonTrackAssociated_bestMatchTP_eta = 0;
  vector<double> *hltIter0IterL3FromL1MuonTrackAssociated_bestMatchTP_phi = 0;
  vector<double> *hltIter0IterL3FromL1MuonTrackAssociated_bestMatchTP_energy = 0;
  vector<int> *hltIter0IterL3FromL1MuonTrackAssociated_bestMatchTP_status = 0;
  vector<int> *hltIter0IterL3FromL1MuonTrackAssociated_bestMatchTP_pdgId = 0;
  int ntpTo_hltIter0IterL3FromL1MuonTrackAssociated;
  vector<double> *tpTo_hltIter0IterL3FromL1MuonTrackAssociated_gen_pt = 0;
  vector<double> *tpTo_hltIter0IterL3FromL1MuonTrackAssociated_gen_eta = 0;
  vector<double> *tpTo_hltIter0IterL3FromL1MuonTrackAssociated_gen_phi = 0;
  vector<double> *tpTo_hltIter0IterL3FromL1MuonTrackAssociated_bestMatchTrk_pt = 0;
  vector<double> *tpTo_hltIter0IterL3FromL1MuonTrackAssociated_bestMatchTrk_eta = 0;
  vector<double> *tpTo_hltIter0IterL3FromL1MuonTrackAssociated_bestMatchTrk_phi = 0;
  int nhltIter2IterL3FromL1MuonTrackAssociated;
  vector<double> *hltIter2IterL3FromL1MuonTrackAssociated_pt = 0;
  vector<double> *hltIter2IterL3FromL1MuonTrackAssociated_eta = 0;
  vector<double> *hltIter2IterL3FromL1MuonTrackAssociated_phi = 0;
  vector<double> *hltIter2IterL3FromL1MuonTrackAssociated_bestMatchTP_pt = 0;
  vector<double> *hltIter2IterL3FromL1MuonTrackAssociated_bestMatchTP_eta = 0;
  vector<double> *hltIter2IterL3FromL1MuonTrackAssociated_bestMatchTP_phi = 0;
  vector<double> *hltIter2IterL3FromL1MuonTrackAssociated_bestMatchTP_energy = 0;
  vector<int> *hltIter2IterL3FromL1MuonTrackAssociated_bestMatchTP_status = 0;
  vector<int> *hltIter2IterL3FromL1MuonTrackAssociated_bestMatchTP_pdgId = 0;
  int ntpTo_hltIter2IterL3FromL1MuonTrackAssociated;
  vector<double> *tpTo_hltIter2IterL3FromL1MuonTrackAssociated_gen_pt = 0;
  vector<double> *tpTo_hltIter2IterL3FromL1MuonTrackAssociated_gen_eta = 0;
  vector<double> *tpTo_hltIter2IterL3FromL1MuonTrackAssociated_gen_phi = 0;
  vector<double> *tpTo_hltIter2IterL3FromL1MuonTrackAssociated_bestMatchTrk_pt = 0;
  vector<double> *tpTo_hltIter2IterL3FromL1MuonTrackAssociated_bestMatchTrk_eta = 0;
  vector<double> *tpTo_hltIter2IterL3FromL1MuonTrackAssociated_bestMatchTrk_phi = 0;
  int nhltIter2IterL3MuonMergedAssociated;
  vector<double> *hltIter2IterL3MuonMergedAssociated_pt = 0;
  vector<double> *hltIter2IterL3MuonMergedAssociated_eta = 0;
  vector<double> *hltIter2IterL3MuonMergedAssociated_phi = 0;
  vector<double> *hltIter2IterL3MuonMergedAssociated_bestMatchTP_pt = 0;
  vector<double> *hltIter2IterL3MuonMergedAssociated_bestMatchTP_eta = 0;
  vector<double> *hltIter2IterL3MuonMergedAssociated_bestMatchTP_phi = 0;
  vector<double> *hltIter2IterL3MuonMergedAssociated_bestMatchTP_energy = 0;
  vector<int> *hltIter2IterL3MuonMergedAssociated_bestMatchTP_status = 0;
  vector<int> *hltIter2IterL3MuonMergedAssociated_bestMatchTP_pdgId = 0;
  int ntpTo_hltIter2IterL3MuonMergedAssociated;
  vector<double> *tpTo_hltIter2IterL3MuonMergedAssociated_gen_pt = 0;
  vector<double> *tpTo_hltIter2IterL3MuonMergedAssociated_gen_eta = 0;
  vector<double> *tpTo_hltIter2IterL3MuonMergedAssociated_gen_phi = 0;
  vector<double> *tpTo_hltIter2IterL3MuonMergedAssociated_bestMatchTrk_pt = 0;
  vector<double> *tpTo_hltIter2IterL3MuonMergedAssociated_bestMatchTrk_eta = 0;
  vector<double> *tpTo_hltIter2IterL3MuonMergedAssociated_bestMatchTrk_phi = 0;
  int nhltIter2IterL3FromL1MuonMergedAssociated;
  vector<double> *hltIter2IterL3FromL1MuonMergedAssociated_pt = 0;
  vector<double> *hltIter2IterL3FromL1MuonMergedAssociated_eta = 0;
  vector<double> *hltIter2IterL3FromL1MuonMergedAssociated_phi = 0;
  vector<double> *hltIter2IterL3FromL1MuonMergedAssociated_bestMatchTP_pt = 0;
  vector<double> *hltIter2IterL3FromL1MuonMergedAssociated_bestMatchTP_eta = 0;
  vector<double> *hltIter2IterL3FromL1MuonMergedAssociated_bestMatchTP_phi = 0;
  vector<double> *hltIter2IterL3FromL1MuonMergedAssociated_bestMatchTP_energy = 0;
  vector<int> *hltIter2IterL3FromL1MuonMergedAssociated_bestMatchTP_status = 0;
  vector<int> *hltIter2IterL3FromL1MuonMergedAssociated_bestMatchTP_pdgId = 0;
  int ntpTo_hltIter2IterL3FromL1MuonMergedAssociated;
  vector<double> *tpTo_hltIter2IterL3FromL1MuonMergedAssociated_gen_pt = 0;
  vector<double> *tpTo_hltIter2IterL3FromL1MuonMergedAssociated_gen_eta = 0;
  vector<double> *tpTo_hltIter2IterL3FromL1MuonMergedAssociated_gen_phi = 0;
  vector<double> *tpTo_hltIter2IterL3FromL1MuonMergedAssociated_bestMatchTrk_pt = 0;
  vector<double> *tpTo_hltIter2IterL3FromL1MuonMergedAssociated_bestMatchTrk_eta = 0;
  vector<double> *tpTo_hltIter2IterL3FromL1MuonMergedAssociated_bestMatchTrk_phi = 0;
  int nhltIterL3MuonMergedAssociated;
  vector<double> *hltIterL3MuonMergedAssociated_pt = 0;
  vector<double> *hltIterL3MuonMergedAssociated_eta = 0;
  vector<double> *hltIterL3MuonMergedAssociated_phi = 0;
  vector<double> *hltIterL3MuonMergedAssociated_bestMatchTP_pt = 0;
  vector<double> *hltIterL3MuonMergedAssociated_bestMatchTP_eta = 0;
  vector<double> *hltIterL3MuonMergedAssociated_bestMatchTP_phi = 0;
  vector<double> *hltIterL3MuonMergedAssociated_bestMatchTP_energy = 0;
  vector<int> *hltIterL3MuonMergedAssociated_bestMatchTP_status = 0;
  vector<int> *hltIterL3MuonMergedAssociated_bestMatchTP_pdgId = 0;
  int ntpTo_hltIterL3MuonMergedAssociated;
  vector<double> *tpTo_hltIterL3MuonMergedAssociated_gen_pt = 0;
  vector<double> *tpTo_hltIterL3MuonMergedAssociated_gen_eta = 0;
  vector<double> *tpTo_hltIterL3MuonMergedAssociated_gen_phi = 0;
  vector<double> *tpTo_hltIterL3MuonMergedAssociated_bestMatchTrk_pt = 0;
  vector<double> *tpTo_hltIterL3MuonMergedAssociated_bestMatchTrk_eta = 0;
  vector<double> *tpTo_hltIterL3MuonMergedAssociated_bestMatchTrk_phi = 0;
  int nhltIterL3MuonAndMuonFromL1MergedAssociated;
  vector<double> *hltIterL3MuonAndMuonFromL1MergedAssociated_pt = 0;
  vector<double> *hltIterL3MuonAndMuonFromL1MergedAssociated_eta = 0;
  vector<double> *hltIterL3MuonAndMuonFromL1MergedAssociated_phi = 0;
  vector<double> *hltIterL3MuonAndMuonFromL1MergedAssociated_bestMatchTP_pt = 0;
  vector<double> *hltIterL3MuonAndMuonFromL1MergedAssociated_bestMatchTP_eta = 0;
  vector<double> *hltIterL3MuonAndMuonFromL1MergedAssociated_bestMatchTP_phi = 0;
  vector<double> *hltIterL3MuonAndMuonFromL1MergedAssociated_bestMatchTP_energy = 0;
  vector<int> *hltIterL3MuonAndMuonFromL1MergedAssociated_bestMatchTP_status = 0;
  vector<int> *hltIterL3MuonAndMuonFromL1MergedAssociated_bestMatchTP_pdgId = 0;
  int ntpTo_hltIterL3MuonAndMuonFromL1MergedAssociated;
  vector<double> *tpTo_hltIterL3MuonAndMuonFromL1MergedAssociated_gen_pt = 0;
  vector<double> *tpTo_hltIterL3MuonAndMuonFromL1MergedAssociated_gen_eta = 0;
  vector<double> *tpTo_hltIterL3MuonAndMuonFromL1MergedAssociated_gen_phi = 0;
  vector<double> *tpTo_hltIterL3MuonAndMuonFromL1MergedAssociated_bestMatchTrk_pt = 0;
  vector<double> *tpTo_hltIterL3MuonAndMuonFromL1MergedAssociated_bestMatchTrk_eta = 0;
  vector<double> *tpTo_hltIterL3MuonAndMuonFromL1MergedAssociated_bestMatchTrk_phi = 0;
  int niterL3MuonNoIDTrackAssociated;
  vector<double> *iterL3MuonNoIDTrackAssociated_pt = 0;
  vector<double> *iterL3MuonNoIDTrackAssociated_eta = 0;
  vector<double> *iterL3MuonNoIDTrackAssociated_phi = 0;
  vector<double> *iterL3MuonNoIDTrackAssociated_bestMatchTP_pt = 0;
  vector<double> *iterL3MuonNoIDTrackAssociated_bestMatchTP_eta = 0;
  vector<double> *iterL3MuonNoIDTrackAssociated_bestMatchTP_phi = 0;
  vector<double> *iterL3MuonNoIDTrackAssociated_bestMatchTP_energy = 0;
  vector<int> *iterL3MuonNoIDTrackAssociated_bestMatchTP_status = 0;
  vector<int> *iterL3MuonNoIDTrackAssociated_bestMatchTP_pdgId = 0;
  int ntpTo_iterL3MuonNoIDTrackAssociated;
  vector<double> *tpTo_iterL3MuonNoIDTrackAssociated_gen_pt = 0;
  vector<double> *tpTo_iterL3MuonNoIDTrackAssociated_gen_eta = 0;
  vector<double> *tpTo_iterL3MuonNoIDTrackAssociated_gen_phi = 0;
  vector<double> *tpTo_iterL3MuonNoIDTrackAssociated_bestMatchTrk_pt = 0;
  vector<double> *tpTo_iterL3MuonNoIDTrackAssociated_bestMatchTrk_eta = 0;
  vector<double> *tpTo_iterL3MuonNoIDTrackAssociated_bestMatchTrk_phi = 0;
  int niterL3MuonTrackAssociated;
  vector<double> *iterL3MuonTrackAssociated_pt = 0;
  vector<double> *iterL3MuonTrackAssociated_eta = 0;
  vector<double> *iterL3MuonTrackAssociated_phi = 0;
  vector<double> *iterL3MuonTrackAssociated_bestMatchTP_pt = 0;
  vector<double> *iterL3MuonTrackAssociated_bestMatchTP_eta = 0;
  vector<double> *iterL3MuonTrackAssociated_bestMatchTP_phi = 0;
  vector<double> *iterL3MuonTrackAssociated_bestMatchTP_energy = 0;
  vector<int> *iterL3MuonTrackAssociated_bestMatchTP_status = 0;
  vector<int> *iterL3MuonTrackAssociated_bestMatchTP_pdgId = 0;
  int ntpTo_iterL3MuonTrackAssociated;
  vector<double> *tpTo_iterL3MuonTrackAssociated_gen_pt = 0;
  vector<double> *tpTo_iterL3MuonTrackAssociated_gen_eta = 0;
  vector<double> *tpTo_iterL3MuonTrackAssociated_gen_phi = 0;
  vector<double> *tpTo_iterL3MuonTrackAssociated_bestMatchTrk_pt = 0;
  vector<double> *tpTo_iterL3MuonTrackAssociated_bestMatchTrk_eta = 0;
  vector<double> *tpTo_iterL3MuonTrackAssociated_bestMatchTrk_phi = 0;
  
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
  fChain->SetBranchAddress("L1Muon_etaAtVtx",&L1Muon_etaAtVtx);
  fChain->SetBranchAddress("L1Muon_phiAtVtx",&L1Muon_phiAtVtx);
  fChain->SetBranchAddress("L1Muon_charge",&L1Muon_charge);
  fChain->SetBranchAddress("L1Muon_quality",&L1Muon_quality);
  fChain->SetBranchAddress("nIterL3IOFromL1",&nIterL3IOFromL1);
  fChain->SetBranchAddress("iterL3IOFromL1_pt",&iterL3IOFromL1_pt);
  fChain->SetBranchAddress("iterL3IOFromL1_eta",&iterL3IOFromL1_eta);
  fChain->SetBranchAddress("iterL3IOFromL1_phi",&iterL3IOFromL1_phi);
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
  fChain->SetBranchAddress("nhltIterL3OIMuonTrack",&nhltIterL3OIMuonTrack);
  fChain->SetBranchAddress("hltIterL3OIMuonTrack_pt",&hltIterL3OIMuonTrack_pt);
  fChain->SetBranchAddress("hltIterL3OIMuonTrack_eta",&hltIterL3OIMuonTrack_eta);
  fChain->SetBranchAddress("hltIterL3OIMuonTrack_phi",&hltIterL3OIMuonTrack_phi);
  fChain->SetBranchAddress("nhltIter0IterL3MuonTrack",&nhltIter0IterL3MuonTrack);
  fChain->SetBranchAddress("hltIter0IterL3MuonTrack_pt",&hltIter0IterL3MuonTrack_pt);
  fChain->SetBranchAddress("hltIter0IterL3MuonTrack_eta",&hltIter0IterL3MuonTrack_eta);
  fChain->SetBranchAddress("hltIter0IterL3MuonTrack_phi",&hltIter0IterL3MuonTrack_phi);
  fChain->SetBranchAddress("nhltIter2IterL3MuonTrack",&nhltIter2IterL3MuonTrack);
  fChain->SetBranchAddress("hltIter2IterL3MuonTrack_pt",&hltIter2IterL3MuonTrack_pt);
  fChain->SetBranchAddress("hltIter2IterL3MuonTrack_eta",&hltIter2IterL3MuonTrack_eta);
  fChain->SetBranchAddress("hltIter2IterL3MuonTrack_phi",&hltIter2IterL3MuonTrack_phi);
  //fChain->SetBranchAddress("hltIter3IterL3MuonTrack_pt",&hltIter3IterL3MuonTrack_pt);
  //fChain->SetBranchAddress("hltIter3IterL3MuonTrack_eta",&hltIter3IterL3MuonTrack_eta);
  //fChain->SetBranchAddress("hltIter3IterL3MuonTrack_phi",&hltIter3IterL3MuonTrack_phi);
  fChain->SetBranchAddress("nhltIter0IterL3FromL1MuonTrack",&nhltIter0IterL3FromL1MuonTrack);
  fChain->SetBranchAddress("hltIter0IterL3FromL1MuonTrack_pt",&hltIter0IterL3FromL1MuonTrack_pt);
  fChain->SetBranchAddress("hltIter0IterL3FromL1MuonTrack_eta",&hltIter0IterL3FromL1MuonTrack_eta);
  fChain->SetBranchAddress("hltIter0IterL3FromL1MuonTrack_phi",&hltIter0IterL3FromL1MuonTrack_phi);
  fChain->SetBranchAddress("nhltIter2IterL3FromL1MuonTrack",&nhltIter2IterL3FromL1MuonTrack);
  fChain->SetBranchAddress("hltIter2IterL3FromL1MuonTrack_pt",&hltIter2IterL3FromL1MuonTrack_pt);
  fChain->SetBranchAddress("hltIter2IterL3FromL1MuonTrack_eta",&hltIter2IterL3FromL1MuonTrack_eta);
  fChain->SetBranchAddress("hltIter2IterL3FromL1MuonTrack_phi",&hltIter2IterL3FromL1MuonTrack_phi);
  //fChain->SetBranchAddress("hltIter3IterL3FromL1MuonTrack_pt",&hltIter3IterL3FromL1MuonTrack_pt);
  //fChain->SetBranchAddress("hltIter3IterL3FromL1MuonTrack_eta",&hltIter3IterL3FromL1MuonTrack_eta);
  //fChain->SetBranchAddress("hltIter3IterL3FromL1MuonTrack_phi",&hltIter3IterL3FromL1MuonTrack_phi);
  fChain->SetBranchAddress("nhltIterL3OIMuonTrackAssociated",&nhltIterL3OIMuonTrackAssociated);
  fChain->SetBranchAddress("hltIterL3OIMuonTrackAssociated_pt", &hltIterL3OIMuonTrackAssociated_pt);
  fChain->SetBranchAddress("hltIterL3OIMuonTrackAssociated_eta",&hltIterL3OIMuonTrackAssociated_eta);
  fChain->SetBranchAddress("hltIterL3OIMuonTrackAssociated_phi",&hltIterL3OIMuonTrackAssociated_phi);
  fChain->SetBranchAddress("hltIterL3OIMuonTrackAssociated_bestMatchTP_pt", &hltIterL3OIMuonTrackAssociated_bestMatchTP_pt);
  fChain->SetBranchAddress("hltIterL3OIMuonTrackAssociated_bestMatchTP_eta",&hltIterL3OIMuonTrackAssociated_bestMatchTP_eta);
  fChain->SetBranchAddress("hltIterL3OIMuonTrackAssociated_bestMatchTP_phi",&hltIterL3OIMuonTrackAssociated_bestMatchTP_phi);
  fChain->SetBranchAddress("hltIterL3OIMuonTrackAssociated_bestMatchTP_energy",&hltIterL3OIMuonTrackAssociated_bestMatchTP_energy);
  fChain->SetBranchAddress("hltIterL3OIMuonTrackAssociated_bestMatchTP_status",&hltIterL3OIMuonTrackAssociated_bestMatchTP_status);
  fChain->SetBranchAddress("hltIterL3OIMuonTrackAssociated_bestMatchTP_pdgId",&hltIterL3OIMuonTrackAssociated_bestMatchTP_pdgId);
  fChain->SetBranchAddress("ntpTo_hltIterL3OIMuonTrackAssociated",&ntpTo_hltIterL3OIMuonTrackAssociated);
  fChain->SetBranchAddress("tpTo_hltIterL3OIMuonTrackAssociated_gen_pt", &tpTo_hltIterL3OIMuonTrackAssociated_gen_pt);
  fChain->SetBranchAddress("tpTo_hltIterL3OIMuonTrackAssociated_gen_eta",&tpTo_hltIterL3OIMuonTrackAssociated_gen_eta);
  fChain->SetBranchAddress("tpTo_hltIterL3OIMuonTrackAssociated_gen_phi",&tpTo_hltIterL3OIMuonTrackAssociated_gen_phi);
  fChain->SetBranchAddress("tpTo_hltIterL3OIMuonTrackAssociated_bestMatchTrk_pt", &tpTo_hltIterL3OIMuonTrackAssociated_bestMatchTrk_pt);
  fChain->SetBranchAddress("tpTo_hltIterL3OIMuonTrackAssociated_bestMatchTrk_eta",&tpTo_hltIterL3OIMuonTrackAssociated_bestMatchTrk_eta);
  fChain->SetBranchAddress("tpTo_hltIterL3OIMuonTrackAssociated_bestMatchTrk_phi",&tpTo_hltIterL3OIMuonTrackAssociated_bestMatchTrk_phi);
  fChain->SetBranchAddress("nhltIter0IterL3MuonTrackAssociated",&nhltIter0IterL3MuonTrackAssociated);
  fChain->SetBranchAddress("hltIter0IterL3MuonTrackAssociated_pt", &hltIter0IterL3MuonTrackAssociated_pt);
  fChain->SetBranchAddress("hltIter0IterL3MuonTrackAssociated_eta",&hltIter0IterL3MuonTrackAssociated_eta);
  fChain->SetBranchAddress("hltIter0IterL3MuonTrackAssociated_phi",&hltIter0IterL3MuonTrackAssociated_phi);
  fChain->SetBranchAddress("hltIter0IterL3MuonTrackAssociated_bestMatchTP_pt", &hltIter0IterL3MuonTrackAssociated_bestMatchTP_pt);
  fChain->SetBranchAddress("hltIter0IterL3MuonTrackAssociated_bestMatchTP_eta",&hltIter0IterL3MuonTrackAssociated_bestMatchTP_eta);
  fChain->SetBranchAddress("hltIter0IterL3MuonTrackAssociated_bestMatchTP_phi",&hltIter0IterL3MuonTrackAssociated_bestMatchTP_phi);
  fChain->SetBranchAddress("hltIter0IterL3MuonTrackAssociated_bestMatchTP_energy",&hltIter0IterL3MuonTrackAssociated_bestMatchTP_energy);
  fChain->SetBranchAddress("hltIter0IterL3MuonTrackAssociated_bestMatchTP_status",&hltIter0IterL3MuonTrackAssociated_bestMatchTP_status);
  fChain->SetBranchAddress("hltIter0IterL3MuonTrackAssociated_bestMatchTP_pdgId",&hltIter0IterL3MuonTrackAssociated_bestMatchTP_pdgId);
  fChain->SetBranchAddress("ntpTo_hltIter0IterL3MuonTrackAssociated",&ntpTo_hltIter0IterL3MuonTrackAssociated);
  fChain->SetBranchAddress("tpTo_hltIter0IterL3MuonTrackAssociated_gen_pt", &tpTo_hltIter0IterL3MuonTrackAssociated_gen_pt);
  fChain->SetBranchAddress("tpTo_hltIter0IterL3MuonTrackAssociated_gen_eta",&tpTo_hltIter0IterL3MuonTrackAssociated_gen_eta);
  fChain->SetBranchAddress("tpTo_hltIter0IterL3MuonTrackAssociated_gen_phi",&tpTo_hltIter0IterL3MuonTrackAssociated_gen_phi);
  fChain->SetBranchAddress("tpTo_hltIter0IterL3MuonTrackAssociated_bestMatchTrk_pt", &tpTo_hltIter0IterL3MuonTrackAssociated_bestMatchTrk_pt);
  fChain->SetBranchAddress("tpTo_hltIter0IterL3MuonTrackAssociated_bestMatchTrk_eta",&tpTo_hltIter0IterL3MuonTrackAssociated_bestMatchTrk_eta);
  fChain->SetBranchAddress("tpTo_hltIter0IterL3MuonTrackAssociated_bestMatchTrk_phi",&tpTo_hltIter0IterL3MuonTrackAssociated_bestMatchTrk_phi);
  fChain->SetBranchAddress("nhltIter2IterL3MuonTrackAssociated",&nhltIter2IterL3MuonTrackAssociated);
  fChain->SetBranchAddress("hltIter2IterL3MuonTrackAssociated_pt", &hltIter2IterL3MuonTrackAssociated_pt);
  fChain->SetBranchAddress("hltIter2IterL3MuonTrackAssociated_eta",&hltIter2IterL3MuonTrackAssociated_eta);
  fChain->SetBranchAddress("hltIter2IterL3MuonTrackAssociated_phi",&hltIter2IterL3MuonTrackAssociated_phi);
  fChain->SetBranchAddress("hltIter2IterL3MuonTrackAssociated_bestMatchTP_pt", &hltIter2IterL3MuonTrackAssociated_bestMatchTP_pt);
  fChain->SetBranchAddress("hltIter2IterL3MuonTrackAssociated_bestMatchTP_eta",&hltIter2IterL3MuonTrackAssociated_bestMatchTP_eta);
  fChain->SetBranchAddress("hltIter2IterL3MuonTrackAssociated_bestMatchTP_phi",&hltIter2IterL3MuonTrackAssociated_bestMatchTP_phi);
  fChain->SetBranchAddress("hltIter2IterL3MuonTrackAssociated_bestMatchTP_energy",&hltIter2IterL3MuonTrackAssociated_bestMatchTP_energy);
  fChain->SetBranchAddress("hltIter2IterL3MuonTrackAssociated_bestMatchTP_status",&hltIter2IterL3MuonTrackAssociated_bestMatchTP_status);
  fChain->SetBranchAddress("hltIter2IterL3MuonTrackAssociated_bestMatchTP_pdgId",&hltIter2IterL3MuonTrackAssociated_bestMatchTP_pdgId);
  fChain->SetBranchAddress("ntpTo_hltIter2IterL3MuonTrackAssociated",&ntpTo_hltIter2IterL3MuonTrackAssociated);
  fChain->SetBranchAddress("tpTo_hltIter2IterL3MuonTrackAssociated_gen_pt", &tpTo_hltIter2IterL3MuonTrackAssociated_gen_pt);
  fChain->SetBranchAddress("tpTo_hltIter2IterL3MuonTrackAssociated_gen_eta",&tpTo_hltIter2IterL3MuonTrackAssociated_gen_eta);
  fChain->SetBranchAddress("tpTo_hltIter2IterL3MuonTrackAssociated_gen_phi",&tpTo_hltIter2IterL3MuonTrackAssociated_gen_phi);
  fChain->SetBranchAddress("tpTo_hltIter2IterL3MuonTrackAssociated_bestMatchTrk_pt", &tpTo_hltIter2IterL3MuonTrackAssociated_bestMatchTrk_pt);
  fChain->SetBranchAddress("tpTo_hltIter2IterL3MuonTrackAssociated_bestMatchTrk_eta",&tpTo_hltIter2IterL3MuonTrackAssociated_bestMatchTrk_eta);
  fChain->SetBranchAddress("tpTo_hltIter2IterL3MuonTrackAssociated_bestMatchTrk_phi",&tpTo_hltIter2IterL3MuonTrackAssociated_bestMatchTrk_phi);
  fChain->SetBranchAddress("nhltIter0IterL3FromL1MuonTrackAssociated",&nhltIter0IterL3FromL1MuonTrackAssociated);
  fChain->SetBranchAddress("hltIter0IterL3FromL1MuonTrackAssociated_pt", &hltIter0IterL3FromL1MuonTrackAssociated_pt);
  fChain->SetBranchAddress("hltIter0IterL3FromL1MuonTrackAssociated_eta",&hltIter0IterL3FromL1MuonTrackAssociated_eta);
  fChain->SetBranchAddress("hltIter0IterL3FromL1MuonTrackAssociated_phi",&hltIter0IterL3FromL1MuonTrackAssociated_phi);
  fChain->SetBranchAddress("hltIter0IterL3FromL1MuonTrackAssociated_bestMatchTP_pt", &hltIter0IterL3FromL1MuonTrackAssociated_bestMatchTP_pt);
  fChain->SetBranchAddress("hltIter0IterL3FromL1MuonTrackAssociated_bestMatchTP_eta",&hltIter0IterL3FromL1MuonTrackAssociated_bestMatchTP_eta);
  fChain->SetBranchAddress("hltIter0IterL3FromL1MuonTrackAssociated_bestMatchTP_phi",&hltIter0IterL3FromL1MuonTrackAssociated_bestMatchTP_phi);
  fChain->SetBranchAddress("hltIter0IterL3FromL1MuonTrackAssociated_bestMatchTP_energy",&hltIter0IterL3FromL1MuonTrackAssociated_bestMatchTP_energy);
  fChain->SetBranchAddress("hltIter0IterL3FromL1MuonTrackAssociated_bestMatchTP_status",&hltIter0IterL3FromL1MuonTrackAssociated_bestMatchTP_status);
  fChain->SetBranchAddress("hltIter0IterL3FromL1MuonTrackAssociated_bestMatchTP_pdgId",&hltIter0IterL3FromL1MuonTrackAssociated_bestMatchTP_pdgId);
  fChain->SetBranchAddress("ntpTo_hltIter0IterL3FromL1MuonTrackAssociated",&ntpTo_hltIter0IterL3FromL1MuonTrackAssociated);
  fChain->SetBranchAddress("tpTo_hltIter0IterL3FromL1MuonTrackAssociated_gen_pt", &tpTo_hltIter0IterL3FromL1MuonTrackAssociated_gen_pt);
  fChain->SetBranchAddress("tpTo_hltIter0IterL3FromL1MuonTrackAssociated_gen_eta",&tpTo_hltIter0IterL3FromL1MuonTrackAssociated_gen_eta);
  fChain->SetBranchAddress("tpTo_hltIter0IterL3FromL1MuonTrackAssociated_gen_phi",&tpTo_hltIter0IterL3FromL1MuonTrackAssociated_gen_phi);
  fChain->SetBranchAddress("tpTo_hltIter0IterL3FromL1MuonTrackAssociated_bestMatchTrk_pt", &tpTo_hltIter0IterL3FromL1MuonTrackAssociated_bestMatchTrk_pt);
  fChain->SetBranchAddress("tpTo_hltIter0IterL3FromL1MuonTrackAssociated_bestMatchTrk_eta",&tpTo_hltIter0IterL3FromL1MuonTrackAssociated_bestMatchTrk_eta);
  fChain->SetBranchAddress("tpTo_hltIter0IterL3FromL1MuonTrackAssociated_bestMatchTrk_phi",&tpTo_hltIter0IterL3FromL1MuonTrackAssociated_bestMatchTrk_phi);
  fChain->SetBranchAddress("nhltIter2IterL3FromL1MuonTrackAssociated",&nhltIter2IterL3FromL1MuonTrackAssociated);
  fChain->SetBranchAddress("hltIter2IterL3FromL1MuonTrackAssociated_pt", &hltIter2IterL3FromL1MuonTrackAssociated_pt);
  fChain->SetBranchAddress("hltIter2IterL3FromL1MuonTrackAssociated_eta",&hltIter2IterL3FromL1MuonTrackAssociated_eta);
  fChain->SetBranchAddress("hltIter2IterL3FromL1MuonTrackAssociated_phi",&hltIter2IterL3FromL1MuonTrackAssociated_phi);
  fChain->SetBranchAddress("hltIter2IterL3FromL1MuonTrackAssociated_bestMatchTP_pt", &hltIter2IterL3FromL1MuonTrackAssociated_bestMatchTP_pt);
  fChain->SetBranchAddress("hltIter2IterL3FromL1MuonTrackAssociated_bestMatchTP_eta",&hltIter2IterL3FromL1MuonTrackAssociated_bestMatchTP_eta);
  fChain->SetBranchAddress("hltIter2IterL3FromL1MuonTrackAssociated_bestMatchTP_phi",&hltIter2IterL3FromL1MuonTrackAssociated_bestMatchTP_phi);
  fChain->SetBranchAddress("hltIter2IterL3FromL1MuonTrackAssociated_bestMatchTP_energy",&hltIter2IterL3FromL1MuonTrackAssociated_bestMatchTP_energy);
  fChain->SetBranchAddress("hltIter2IterL3FromL1MuonTrackAssociated_bestMatchTP_status",&hltIter2IterL3FromL1MuonTrackAssociated_bestMatchTP_status);
  fChain->SetBranchAddress("hltIter2IterL3FromL1MuonTrackAssociated_bestMatchTP_pdgId",&hltIter2IterL3FromL1MuonTrackAssociated_bestMatchTP_pdgId);
  fChain->SetBranchAddress("ntpTo_hltIter2IterL3FromL1MuonTrackAssociated",&ntpTo_hltIter2IterL3FromL1MuonTrackAssociated);
  fChain->SetBranchAddress("tpTo_hltIter2IterL3FromL1MuonTrackAssociated_gen_pt", &tpTo_hltIter2IterL3FromL1MuonTrackAssociated_gen_pt);
  fChain->SetBranchAddress("tpTo_hltIter2IterL3FromL1MuonTrackAssociated_gen_eta",&tpTo_hltIter2IterL3FromL1MuonTrackAssociated_gen_eta);
  fChain->SetBranchAddress("tpTo_hltIter2IterL3FromL1MuonTrackAssociated_gen_phi",&tpTo_hltIter2IterL3FromL1MuonTrackAssociated_gen_phi);
  fChain->SetBranchAddress("tpTo_hltIter2IterL3FromL1MuonTrackAssociated_bestMatchTrk_pt", &tpTo_hltIter2IterL3FromL1MuonTrackAssociated_bestMatchTrk_pt);
  fChain->SetBranchAddress("tpTo_hltIter2IterL3FromL1MuonTrackAssociated_bestMatchTrk_eta",&tpTo_hltIter2IterL3FromL1MuonTrackAssociated_bestMatchTrk_eta);
  fChain->SetBranchAddress("tpTo_hltIter2IterL3FromL1MuonTrackAssociated_bestMatchTrk_phi",&tpTo_hltIter2IterL3FromL1MuonTrackAssociated_bestMatchTrk_phi);
  fChain->SetBranchAddress("nhltIter2IterL3MuonMergedAssociated",&nhltIter2IterL3MuonMergedAssociated);
  fChain->SetBranchAddress("hltIter2IterL3MuonMergedAssociated_pt", &hltIter2IterL3MuonMergedAssociated_pt);
  fChain->SetBranchAddress("hltIter2IterL3MuonMergedAssociated_eta",&hltIter2IterL3MuonMergedAssociated_eta);
  fChain->SetBranchAddress("hltIter2IterL3MuonMergedAssociated_phi",&hltIter2IterL3MuonMergedAssociated_phi);
  fChain->SetBranchAddress("hltIter2IterL3MuonMergedAssociated_bestMatchTP_pt", &hltIter2IterL3MuonMergedAssociated_bestMatchTP_pt);
  fChain->SetBranchAddress("hltIter2IterL3MuonMergedAssociated_bestMatchTP_eta",&hltIter2IterL3MuonMergedAssociated_bestMatchTP_eta);
  fChain->SetBranchAddress("hltIter2IterL3MuonMergedAssociated_bestMatchTP_phi",&hltIter2IterL3MuonMergedAssociated_bestMatchTP_phi);
  fChain->SetBranchAddress("hltIter2IterL3MuonMergedAssociated_bestMatchTP_energy",&hltIter2IterL3MuonMergedAssociated_bestMatchTP_energy);
  fChain->SetBranchAddress("hltIter2IterL3MuonMergedAssociated_bestMatchTP_status",&hltIter2IterL3MuonMergedAssociated_bestMatchTP_status);
  fChain->SetBranchAddress("hltIter2IterL3MuonMergedAssociated_bestMatchTP_pdgId",&hltIter2IterL3MuonMergedAssociated_bestMatchTP_pdgId);
  fChain->SetBranchAddress("ntpTo_hltIter2IterL3MuonMergedAssociated",&ntpTo_hltIter2IterL3MuonMergedAssociated);
  fChain->SetBranchAddress("tpTo_hltIter2IterL3MuonMergedAssociated_gen_pt", &tpTo_hltIter2IterL3MuonMergedAssociated_gen_pt);
  fChain->SetBranchAddress("tpTo_hltIter2IterL3MuonMergedAssociated_gen_eta",&tpTo_hltIter2IterL3MuonMergedAssociated_gen_eta);
  fChain->SetBranchAddress("tpTo_hltIter2IterL3MuonMergedAssociated_gen_phi",&tpTo_hltIter2IterL3MuonMergedAssociated_gen_phi);
  fChain->SetBranchAddress("tpTo_hltIter2IterL3MuonMergedAssociated_bestMatchTrk_pt", &tpTo_hltIter2IterL3MuonMergedAssociated_bestMatchTrk_pt);
  fChain->SetBranchAddress("tpTo_hltIter2IterL3MuonMergedAssociated_bestMatchTrk_eta",&tpTo_hltIter2IterL3MuonMergedAssociated_bestMatchTrk_eta);
  fChain->SetBranchAddress("tpTo_hltIter2IterL3MuonMergedAssociated_bestMatchTrk_phi",&tpTo_hltIter2IterL3MuonMergedAssociated_bestMatchTrk_phi);
  fChain->SetBranchAddress("nhltIter2IterL3FromL1MuonMergedAssociated",&nhltIter2IterL3FromL1MuonMergedAssociated);
  fChain->SetBranchAddress("hltIter2IterL3FromL1MuonMergedAssociated_pt",&hltIter2IterL3FromL1MuonMergedAssociated_pt);
  fChain->SetBranchAddress("hltIter2IterL3FromL1MuonMergedAssociated_eta",&hltIter2IterL3FromL1MuonMergedAssociated_eta);
  fChain->SetBranchAddress("hltIter2IterL3FromL1MuonMergedAssociated_phi",&hltIter2IterL3FromL1MuonMergedAssociated_phi);
  fChain->SetBranchAddress("hltIter2IterL3FromL1MuonMergedAssociated_bestMatchTP_pt",&hltIter2IterL3FromL1MuonMergedAssociated_bestMatchTP_pt);
  fChain->SetBranchAddress("hltIter2IterL3FromL1MuonMergedAssociated_bestMatchTP_eta",&hltIter2IterL3FromL1MuonMergedAssociated_bestMatchTP_eta);
  fChain->SetBranchAddress("hltIter2IterL3FromL1MuonMergedAssociated_bestMatchTP_phi",&hltIter2IterL3FromL1MuonMergedAssociated_bestMatchTP_phi);
  fChain->SetBranchAddress("hltIter2IterL3FromL1MuonMergedAssociated_bestMatchTP_energy",&hltIter2IterL3FromL1MuonMergedAssociated_bestMatchTP_energy);
  fChain->SetBranchAddress("hltIter2IterL3FromL1MuonMergedAssociated_bestMatchTP_status",&hltIter2IterL3FromL1MuonMergedAssociated_bestMatchTP_status);
  fChain->SetBranchAddress("hltIter2IterL3FromL1MuonMergedAssociated_bestMatchTP_pdgId",&hltIter2IterL3FromL1MuonMergedAssociated_bestMatchTP_pdgId);
  fChain->SetBranchAddress("ntpTo_hltIter2IterL3FromL1MuonMergedAssociated",&ntpTo_hltIter2IterL3FromL1MuonMergedAssociated);
  fChain->SetBranchAddress("tpTo_hltIter2IterL3FromL1MuonMergedAssociated_gen_pt",&tpTo_hltIter2IterL3FromL1MuonMergedAssociated_gen_pt);
  fChain->SetBranchAddress("tpTo_hltIter2IterL3FromL1MuonMergedAssociated_gen_eta",&tpTo_hltIter2IterL3FromL1MuonMergedAssociated_gen_eta);
  fChain->SetBranchAddress("tpTo_hltIter2IterL3FromL1MuonMergedAssociated_gen_phi",&tpTo_hltIter2IterL3FromL1MuonMergedAssociated_gen_phi);
  fChain->SetBranchAddress("tpTo_hltIter2IterL3FromL1MuonMergedAssociated_bestMatchTrk_pt",&tpTo_hltIter2IterL3FromL1MuonMergedAssociated_bestMatchTrk_pt);
  fChain->SetBranchAddress("tpTo_hltIter2IterL3FromL1MuonMergedAssociated_bestMatchTrk_eta",&tpTo_hltIter2IterL3FromL1MuonMergedAssociated_bestMatchTrk_eta);
  fChain->SetBranchAddress("tpTo_hltIter2IterL3FromL1MuonMergedAssociated_bestMatchTrk_phi",&tpTo_hltIter2IterL3FromL1MuonMergedAssociated_bestMatchTrk_phi);
  fChain->SetBranchAddress("nhltIterL3MuonMergedAssociated",&nhltIterL3MuonMergedAssociated);
  fChain->SetBranchAddress("hltIterL3MuonMergedAssociated_pt", &hltIterL3MuonMergedAssociated_pt);
  fChain->SetBranchAddress("hltIterL3MuonMergedAssociated_eta",&hltIterL3MuonMergedAssociated_eta);
  fChain->SetBranchAddress("hltIterL3MuonMergedAssociated_phi",&hltIterL3MuonMergedAssociated_phi);
  fChain->SetBranchAddress("hltIterL3MuonMergedAssociated_bestMatchTP_pt", &hltIterL3MuonMergedAssociated_bestMatchTP_pt);
  fChain->SetBranchAddress("hltIterL3MuonMergedAssociated_bestMatchTP_eta",&hltIterL3MuonMergedAssociated_bestMatchTP_eta);
  fChain->SetBranchAddress("hltIterL3MuonMergedAssociated_bestMatchTP_phi",&hltIterL3MuonMergedAssociated_bestMatchTP_phi);
  fChain->SetBranchAddress("hltIterL3MuonMergedAssociated_bestMatchTP_energy",&hltIterL3MuonMergedAssociated_bestMatchTP_energy);
  fChain->SetBranchAddress("hltIterL3MuonMergedAssociated_bestMatchTP_status",&hltIterL3MuonMergedAssociated_bestMatchTP_status);
  fChain->SetBranchAddress("hltIterL3MuonMergedAssociated_bestMatchTP_pdgId",&hltIterL3MuonMergedAssociated_bestMatchTP_pdgId);
  fChain->SetBranchAddress("ntpTo_hltIterL3MuonMergedAssociated",&ntpTo_hltIterL3MuonMergedAssociated);
  fChain->SetBranchAddress("tpTo_hltIterL3MuonMergedAssociated_gen_pt", &tpTo_hltIterL3MuonMergedAssociated_gen_pt);
  fChain->SetBranchAddress("tpTo_hltIterL3MuonMergedAssociated_gen_eta",&tpTo_hltIterL3MuonMergedAssociated_gen_eta);
  fChain->SetBranchAddress("tpTo_hltIterL3MuonMergedAssociated_gen_phi",&tpTo_hltIterL3MuonMergedAssociated_gen_phi);
  fChain->SetBranchAddress("tpTo_hltIterL3MuonMergedAssociated_bestMatchTrk_pt", &tpTo_hltIterL3MuonMergedAssociated_bestMatchTrk_pt);
  fChain->SetBranchAddress("tpTo_hltIterL3MuonMergedAssociated_bestMatchTrk_eta",&tpTo_hltIterL3MuonMergedAssociated_bestMatchTrk_eta);
  fChain->SetBranchAddress("tpTo_hltIterL3MuonMergedAssociated_bestMatchTrk_phi",&tpTo_hltIterL3MuonMergedAssociated_bestMatchTrk_phi);
  fChain->SetBranchAddress("nhltIterL3MuonAndMuonFromL1MergedAssociated",&nhltIterL3MuonAndMuonFromL1MergedAssociated);
  fChain->SetBranchAddress("hltIterL3MuonAndMuonFromL1MergedAssociated_pt", &hltIterL3MuonAndMuonFromL1MergedAssociated_pt);
  fChain->SetBranchAddress("hltIterL3MuonAndMuonFromL1MergedAssociated_eta",&hltIterL3MuonAndMuonFromL1MergedAssociated_eta);
  fChain->SetBranchAddress("hltIterL3MuonAndMuonFromL1MergedAssociated_phi",&hltIterL3MuonAndMuonFromL1MergedAssociated_phi);
  fChain->SetBranchAddress("hltIterL3MuonAndMuonFromL1MergedAssociated_bestMatchTP_pt", &hltIterL3MuonAndMuonFromL1MergedAssociated_bestMatchTP_pt);
  fChain->SetBranchAddress("hltIterL3MuonAndMuonFromL1MergedAssociated_bestMatchTP_eta",&hltIterL3MuonAndMuonFromL1MergedAssociated_bestMatchTP_eta);
  fChain->SetBranchAddress("hltIterL3MuonAndMuonFromL1MergedAssociated_bestMatchTP_phi",&hltIterL3MuonAndMuonFromL1MergedAssociated_bestMatchTP_phi);
  fChain->SetBranchAddress("hltIterL3MuonAndMuonFromL1MergedAssociated_bestMatchTP_energy",&hltIterL3MuonAndMuonFromL1MergedAssociated_bestMatchTP_energy);
  fChain->SetBranchAddress("hltIterL3MuonAndMuonFromL1MergedAssociated_bestMatchTP_status",&hltIterL3MuonAndMuonFromL1MergedAssociated_bestMatchTP_status);
  fChain->SetBranchAddress("hltIterL3MuonAndMuonFromL1MergedAssociated_bestMatchTP_pdgId",&hltIterL3MuonAndMuonFromL1MergedAssociated_bestMatchTP_pdgId);
  fChain->SetBranchAddress("ntpTo_hltIterL3MuonAndMuonFromL1MergedAssociated",&ntpTo_hltIterL3MuonAndMuonFromL1MergedAssociated);
  fChain->SetBranchAddress("tpTo_hltIterL3MuonAndMuonFromL1MergedAssociated_gen_pt", &tpTo_hltIterL3MuonAndMuonFromL1MergedAssociated_gen_pt);
  fChain->SetBranchAddress("tpTo_hltIterL3MuonAndMuonFromL1MergedAssociated_gen_eta",&tpTo_hltIterL3MuonAndMuonFromL1MergedAssociated_gen_eta);
  fChain->SetBranchAddress("tpTo_hltIterL3MuonAndMuonFromL1MergedAssociated_gen_phi",&tpTo_hltIterL3MuonAndMuonFromL1MergedAssociated_gen_phi);
  fChain->SetBranchAddress("tpTo_hltIterL3MuonAndMuonFromL1MergedAssociated_bestMatchTrk_pt", &tpTo_hltIterL3MuonAndMuonFromL1MergedAssociated_bestMatchTrk_pt);
  fChain->SetBranchAddress("tpTo_hltIterL3MuonAndMuonFromL1MergedAssociated_bestMatchTrk_eta",&tpTo_hltIterL3MuonAndMuonFromL1MergedAssociated_bestMatchTrk_eta);
  fChain->SetBranchAddress("tpTo_hltIterL3MuonAndMuonFromL1MergedAssociated_bestMatchTrk_phi",&tpTo_hltIterL3MuonAndMuonFromL1MergedAssociated_bestMatchTrk_phi);
  fChain->SetBranchAddress("niterL3MuonNoIDTrackAssociated",&niterL3MuonNoIDTrackAssociated);
  fChain->SetBranchAddress("iterL3MuonNoIDTrackAssociated_pt", &iterL3MuonNoIDTrackAssociated_pt);
  fChain->SetBranchAddress("iterL3MuonNoIDTrackAssociated_eta",&iterL3MuonNoIDTrackAssociated_eta);
  fChain->SetBranchAddress("iterL3MuonNoIDTrackAssociated_phi",&iterL3MuonNoIDTrackAssociated_phi);
  fChain->SetBranchAddress("iterL3MuonNoIDTrackAssociated_bestMatchTP_pt", &iterL3MuonNoIDTrackAssociated_bestMatchTP_pt);
  fChain->SetBranchAddress("iterL3MuonNoIDTrackAssociated_bestMatchTP_eta",&iterL3MuonNoIDTrackAssociated_bestMatchTP_eta);
  fChain->SetBranchAddress("iterL3MuonNoIDTrackAssociated_bestMatchTP_phi",&iterL3MuonNoIDTrackAssociated_bestMatchTP_phi);
  fChain->SetBranchAddress("iterL3MuonNoIDTrackAssociated_bestMatchTP_energy",&iterL3MuonNoIDTrackAssociated_bestMatchTP_energy);
  fChain->SetBranchAddress("iterL3MuonNoIDTrackAssociated_bestMatchTP_status",&iterL3MuonNoIDTrackAssociated_bestMatchTP_status);
  fChain->SetBranchAddress("iterL3MuonNoIDTrackAssociated_bestMatchTP_pdgId",&iterL3MuonNoIDTrackAssociated_bestMatchTP_pdgId);
  fChain->SetBranchAddress("ntpTo_iterL3MuonNoIDTrackAssociated",&ntpTo_iterL3MuonNoIDTrackAssociated);
  fChain->SetBranchAddress("tpTo_iterL3MuonNoIDTrackAssociated_gen_pt", &tpTo_iterL3MuonNoIDTrackAssociated_gen_pt);
  fChain->SetBranchAddress("tpTo_iterL3MuonNoIDTrackAssociated_gen_eta",&tpTo_iterL3MuonNoIDTrackAssociated_gen_eta);
  fChain->SetBranchAddress("tpTo_iterL3MuonNoIDTrackAssociated_gen_phi",&tpTo_iterL3MuonNoIDTrackAssociated_gen_phi);
  fChain->SetBranchAddress("tpTo_iterL3MuonNoIDTrackAssociated_bestMatchTrk_pt", &tpTo_iterL3MuonNoIDTrackAssociated_bestMatchTrk_pt);
  fChain->SetBranchAddress("tpTo_iterL3MuonNoIDTrackAssociated_bestMatchTrk_eta",&tpTo_iterL3MuonNoIDTrackAssociated_bestMatchTrk_eta);
  fChain->SetBranchAddress("tpTo_iterL3MuonNoIDTrackAssociated_bestMatchTrk_phi",&tpTo_iterL3MuonNoIDTrackAssociated_bestMatchTrk_phi);
  fChain->SetBranchAddress("niterL3MuonTrackAssociated",&niterL3MuonTrackAssociated);
  fChain->SetBranchAddress("iterL3MuonTrackAssociated_pt", &iterL3MuonTrackAssociated_pt);
  fChain->SetBranchAddress("iterL3MuonTrackAssociated_eta",&iterL3MuonTrackAssociated_eta);
  fChain->SetBranchAddress("iterL3MuonTrackAssociated_phi",&iterL3MuonTrackAssociated_phi);
  fChain->SetBranchAddress("iterL3MuonTrackAssociated_bestMatchTP_pt", &iterL3MuonTrackAssociated_bestMatchTP_pt);
  fChain->SetBranchAddress("iterL3MuonTrackAssociated_bestMatchTP_eta",&iterL3MuonTrackAssociated_bestMatchTP_eta);
  fChain->SetBranchAddress("iterL3MuonTrackAssociated_bestMatchTP_phi",&iterL3MuonTrackAssociated_bestMatchTP_phi);
  fChain->SetBranchAddress("iterL3MuonTrackAssociated_bestMatchTP_energy",&iterL3MuonTrackAssociated_bestMatchTP_energy);
  fChain->SetBranchAddress("iterL3MuonTrackAssociated_bestMatchTP_status",&iterL3MuonTrackAssociated_bestMatchTP_status);
  fChain->SetBranchAddress("iterL3MuonTrackAssociated_bestMatchTP_pdgId",&iterL3MuonTrackAssociated_bestMatchTP_pdgId);
  fChain->SetBranchAddress("ntpTo_iterL3MuonTrackAssociated",&ntpTo_iterL3MuonTrackAssociated);
  fChain->SetBranchAddress("tpTo_iterL3MuonTrackAssociated_gen_pt", &tpTo_iterL3MuonTrackAssociated_gen_pt);
  fChain->SetBranchAddress("tpTo_iterL3MuonTrackAssociated_gen_eta",&tpTo_iterL3MuonTrackAssociated_gen_eta);
  fChain->SetBranchAddress("tpTo_iterL3MuonTrackAssociated_gen_phi",&tpTo_iterL3MuonTrackAssociated_gen_phi);
  fChain->SetBranchAddress("tpTo_iterL3MuonTrackAssociated_bestMatchTrk_pt", &tpTo_iterL3MuonTrackAssociated_bestMatchTrk_pt);
  fChain->SetBranchAddress("tpTo_iterL3MuonTrackAssociated_bestMatchTrk_eta",&tpTo_iterL3MuonTrackAssociated_bestMatchTrk_eta);
  fChain->SetBranchAddress("tpTo_iterL3MuonTrackAssociated_bestMatchTrk_phi",&tpTo_iterL3MuonTrackAssociated_bestMatchTrk_phi);
  
  
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
  hists_2D.clear();
  pfs.clear();
  TProfile *pf_track = new TProfile("pf_track","pf_track",25,30,80,0,5000);
  TProfile *pf_trackFromL1 = new TProfile("pf_trackFromL1","pf_trackFromL1",25,30,80,0,5000);

  //To avoid double counting//
  vector<int> this_matched;

  for(int i=0; i<Nevents; i++){
    //cout << "==========" << i << "th event==========" << endl;
    fChain->GetEntry(i);
    //cout << "GetEntry(" << i << ");" << endl;
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
    vector<Object> hardPs;
    vector<Object> hardPs_above; // hardP muon above 26 GeV
    vector<Object> hardPs_above_med; // hardP muon above 10 GeV
    for(int j=0; j<hardP_idx.size(); j++){
      Object hardP;
      hardP.SetPtEtaPhiM(genParticle_pt[hardP_idx.at(j)],genParticle_eta[hardP_idx.at(j)],genParticle_phi[hardP_idx.at(j)],0);
      hardP.SetTag("hardP");
      hardP.SetTruePU(truePU);
      hardPs.push_back(hardP);
      NhardP+=1;
      if(hardP.Pt() > 26){
        hardPs_above.push_back(hardP);
        NhardP_above+=1;
      }
      if(hardP.Pt() > 10){
        hardPs_above_med.push_back(hardP);
      }
    }
    ////==hardP muon above 26GeV==//
    //vector<Object> hardPs_above;
    //for(int j=0; j<hardP_idx.size(); j++){
    //  Object hardP;
    //  hardP.SetPtEtaPhiM(genParticle_pt[hardP_idx.at(j)],genParticle_eta[hardP_idx.at(j)],genParticle_phi[hardP_idx.at(j)],0);
    //  if(hardP.Pt() > 26){
    //    hardP.SetTag("hardP");
    //    hardP.SetTruePU(truePU);
    //    hardPs_above.push_back(hardP);
    //    NhardP_above+=1;
    //  }
    //}
    ////==hardP muon above 10GeV==//
    //vector<Object> hardPs_above_med;
    //for(int j=0; j<hardP_idx.size(); j++){
    //  Object hardP;
    //  hardP.SetPtEtaPhiM(genParticle_pt[hardP_idx.at(j)],genParticle_eta[hardP_idx.at(j)],genParticle_phi[hardP_idx.at(j)],0);
    //  if(hardP.Pt() > 10){
    //    hardPs_above_med.push_back(hardP);
    //  }
    //}

    //==L1Muon with fine quality and above trigger filter thresh.==//
    vector<Object> L1Muons;
    vector<Object> L1Muons_AtVtx;
    for(int j=0; j<nL1Muon; j++){
      Object L1Muon;
      Object L1Muon_AtVtx;
      if(L1Muon_quality[j] > 11 && L1Muon_pt[j] > 22){ // quality 8 / L1 5 --> L3 muon pt 8GeV. vs eta, phi, truePU --> gen matching with pt > 10GeV. (to measure in the plateau.) Numerator --> matching with L3 and also "L1". Numerator should always be a subset of denominator.
        L1Muon.SetPtEtaPhiM(L1Muon_pt[j],L1Muon_eta[j],L1Muon_phi[j],0);
        L1Muon.SetTag("L1");
        L1Muons.push_back(L1Muon);
        L1Muon_AtVtx.SetPtEtaPhiM(L1Muon_pt[j],L1Muon_etaAtVtx[j],L1Muon_phiAtVtx[j],0);
        L1Muon_AtVtx.SetTag("L1_AtVtx");
        L1Muons_AtVtx.push_back(L1Muon_AtVtx);
      }
    }
    //==L1Muon with medium quality and above double muon trigger filter thresh.==//
    vector<Object> L1Muons_med;
    vector<Object> L1Muons_med_AtVtx;
    for(int j=0; j<nL1Muon; j++){
      Object L1Muon_med;
      Object L1Muon_med_AtVtx;
      if(L1Muon_quality[j] > 7 && L1Muon_pt[j] > 5){ // quality 8 / L1 5 --> L3 muon pt 8GeV. vs eta, phi, truePU --> gen matching with pt > 10GeV. (to measure in the plateau.) Numerator --> matching with L3 and also "L1". Numerator should always be a subset of denominator.
        L1Muon_med.SetPtEtaPhiM(L1Muon_pt[j],L1Muon_eta[j],L1Muon_phi[j],0);
        L1Muon_med.SetTag("L1_med");
        L1Muons_med.push_back(L1Muon_med);
        L1Muon_med_AtVtx.SetPtEtaPhiM(L1Muon_pt[j],L1Muon_etaAtVtx[j],L1Muon_phiAtVtx[j],0);
        L1Muon_med_AtVtx.SetTag("L1_med_AtVtx");
        L1Muons_med_AtVtx.push_back(L1Muon_med_AtVtx);
      }
    }

    //==L3MuonNoID==//
    vector<Object> L3Muons_NoID;
    for(int j=0; j<nIterL3MuonNoID; j++){
      Object L3Muon_NoID;
      L3Muon_NoID.SetPtEtaPhiM(iterL3MuonNoID_pt[j],iterL3MuonNoID_eta[j],iterL3MuonNoID_phi[j],0);
      L3Muon_NoID.SetTag("L3NoID");
      L3Muon_NoID.SetTruePU(truePU);
      L3Muons_NoID.push_back(L3Muon_NoID);
      NL3_NoID+=1;
    }
    //==L3Muon==//
    vector<Object> L3Muons;
    for(int j=0; j<nIterL3Muon; j++){
      Object L3Muon;
      L3Muon.SetPtEtaPhiM(iterL3Muon_pt[j],iterL3Muon_eta[j],iterL3Muon_phi[j],0);
      L3Muon.SetTag("L3");
      L3Muon.SetTruePU(truePU);
      L3Muons.push_back(L3Muon);
      NL3+=1;
    }
    //==hardP muon matched with fine L1==//
    vector<Object> hardPs_L1matched;
    this_matched.clear();
    for(int j=0; j<hardPs.size(); j++){
      Object hardP_L1matched = hardPs.at(j);
      for(int k=0; k<L1Muons.size(); k++){
        if( find(this_matched.begin(), this_matched.end(), k) == this_matched.end() ){
          if(hardP_L1matched.DeltaR(L1Muons.at(k))<0.3){
            hardP_L1matched.SetTag("hardP_L1");
            hardP_L1matched.SetTruePU(truePU);
            hardPs_L1matched.push_back(hardP_L1matched);
            NhardP_L1+=1;
            this_matched.push_back(k);
            break;
          }
        }
      }
    }
    //==hardP muon above 26GeV matched with fine L1==//
    vector<Object> hardPs_above_L1matched;
    this_matched.clear();
    for(int j=0; j<hardPs_above.size(); j++){
      Object hardP_above_L1matched = hardPs_above.at(j);
      for(int k=0; k<L1Muons.size(); k++){
        if( find(this_matched.begin(), this_matched.end(), k) == this_matched.end() ){
          if(hardP_above_L1matched.DeltaR(L1Muons.at(k))<0.3){
            hardP_above_L1matched.SetTag("hardP_L1");
            hardP_above_L1matched.SetTruePU(truePU);
            hardPs_above_L1matched.push_back(hardP_above_L1matched);
            this_matched.push_back(k);
            break;
          }
        }
      }
    }
    //==hardP muon matched with medium L1==//
    vector<Object> hardPs_L1matched_med;
    this_matched.clear();
    for(int j=0; j<hardPs.size(); j++){
      Object hardP_L1matched_med = hardPs.at(j);
      for(int k=0; k<L1Muons_med.size(); k++){
        if( find(this_matched.begin(), this_matched.end(), k) == this_matched.end() ){
          if(hardP_L1matched_med.DeltaR(L1Muons_med.at(k))<0.3){
            hardP_L1matched_med.SetTag("hardP_L1_med");
            hardP_L1matched_med.SetTruePU(truePU);
            hardPs_L1matched_med.push_back(hardP_L1matched_med);
            this_matched.push_back(k);
            break;
          }
        }
      }
    }
    //==hardP muon above 10GeV matched with medium L1==//
    vector<Object> hardPs_above_L1matched_med;
    this_matched.clear();
    for(int j=0; j<hardPs_above_med.size(); j++){
      Object hardP_above_L1matched_med = hardPs_above_med.at(j);
      for(int k=0; k<L1Muons_med.size(); k++){
        if( find(this_matched.begin(), this_matched.end(), k) == this_matched.end() ){
          if(hardP_above_L1matched_med.DeltaR(L1Muons_med.at(k))<0.3){
            hardP_above_L1matched_med.SetTag("hardP_L1_med");
            hardP_above_L1matched_med.SetTruePU(truePU);
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
    vector<Object> Finals;
    vector<Object> Finals_above;
    for(int j=0; j<Final_idx.size(); j++){
      Object Final;
      Final.SetPtEtaPhiM(genParticle_pt[Final_idx.at(j)],genParticle_eta[Final_idx.at(j)],genParticle_phi[Final_idx.at(j)],0);
      Final.SetTag("hardP"); //JH : Just for nonprompt efficiency
      Final.SetTruePU(truePU);
      Finals.push_back(Final);

      if(Final.Pt() > 10) Finals_above.push_back(Final);
    }

    //==Call Jpsi candidates==// : Use this just for Jpsi RAW only 
    //vector<Object> JpsiCand;
    //vector<Object> JpsiCand_above;
    //Object Lep1, Lep2;
    //double tmpMassDiff = 10000.;
    //int i1 = 0, i2 = 0;
    //if(Final_idx.size() > 1){
    //  for(int j=0; j<Final_idx.size(); j++){
    //    for(int k=j+1; k<Final_idx.size(); k++){
    //      Object tmpLep1, tmpLep2;
    //      tmpLep1.SetPtEtaPhiM(genParticle_pt[Final_idx.at(j)],genParticle_eta[Final_idx.at(j)],genParticle_phi[Final_idx.at(j)],0);
    //      tmpLep2.SetPtEtaPhiM(genParticle_pt[Final_idx.at(k)],genParticle_eta[Final_idx.at(k)],genParticle_phi[Final_idx.at(k)],0);
    //      if( fabs( (tmpLep1+tmpLep2).M() - 3.1 ) < tmpMassDiff ){
    //        tmpMassDiff = fabs( (tmpLep1+tmpLep2).M() - 3.1 );
    //        Lep1 = tmpLep1; Lep2 = tmpLep2;
    //      }
    //    }
    //  }
    //  if(3. < (Lep1+Lep2).M() && (Lep1+Lep2).M() < 3.2){
    //    Lep1.SetTag("hardP");
    //    Lep2.SetTag("hardP");
    //    Lep1.SetTruePU(truePU);
    //    Lep2.SetTruePU(truePU);
    //    JpsiCand.push_back(Lep1);
    //    JpsiCand.push_back(Lep2);

    //    if(Lep1.Pt() > 5) JpsiCand_above.push_back(Lep1);
    //    if(Lep2.Pt() > 5) JpsiCand_above.push_back(Lep2);
    //  }
    //}
    //Sanity check//
    //if(JpsiCand.size() > 0) cout << "N of JpsiCand : " << JpsiCand.size() << ", Jpsi mass : " << (JpsiCand[0]+JpsiCand[1]).M() << ", pt : " << JpsiCand[0].Pt() << ", " << JpsiCand[1].Pt() << endl;

    //==Tracking Particles==//
    vector<Object> TPs;
    if ( ntpTo_hltIterL3OIMuonTrackAssociated != ntpTo_iterL3MuonTrackAssociated ){
      cout << "ERROR :: ntpTo_hltIterL3OIMuonTrackAssociated == " << ntpTo_hltIterL3OIMuonTrackAssociated << ", ntpTo_iterL3MuonTrackAssociated == " << ntpTo_iterL3MuonTrackAssociated << " ." << endl;
      exit(1);
    }
    for(int j=0; j<ntpTo_hltIterL3OIMuonTrackAssociated; j++){ //JH : I'm assuming here the TPs are the same with each tracks
      if( fabs(tpTo_hltIterL3OIMuonTrackAssociated_gen_eta->at(j)) < 2.4 ){
        Object TP;
        TP.SetPtEtaPhiM(tpTo_hltIterL3OIMuonTrackAssociated_gen_pt->at(j),tpTo_hltIterL3OIMuonTrackAssociated_gen_eta->at(j),tpTo_hltIterL3OIMuonTrackAssociated_gen_phi->at(j),0);
        TP.SetTag("TP");
        TP.SetTruePU(truePU);
        TPs.push_back(TP);
      }
    }

    //==tracks to measure eff==//
    if ( nhltIterL3OIMuonTrack != nhltIterL3OIMuonTrackAssociated ){
      cout << "ERROR :: nhltIterL3OIMuonTrack == " << nhltIterL3OIMuonTrack << ", nhltIterL3OIMuonTrackAssociated == " << nhltIterL3OIMuonTrackAssociated << " ." << endl;
      exit(1);
    }
    vector<Object> L3OITracks;
    for(int j=0; j<nhltIterL3OIMuonTrack; j++){
      Object L3OITrack;
      L3OITrack.SetPtEtaPhiM(hltIterL3OIMuonTrack_pt->at(j),hltIterL3OIMuonTrack_eta->at(j),hltIterL3OIMuonTrack_phi->at(j),0);
      L3OITrack.SetTag("OI");
      L3OITrack.SetTruePU(truePU);
      L3OITracks.push_back(L3OITrack);
      //cout << "hltIterL3OIMuonTrack_pt: " << hltIterL3OIMuonTrack_pt->at(j) << endl;
    }
    vector<Object> L3OIAssoTracks;
    for(int j=0; j<nhltIterL3OIMuonTrackAssociated; j++){
      if( hltIterL3OIMuonTrackAssociated_bestMatchTP_status->at(j) == 1 && fabs(hltIterL3OIMuonTrackAssociated_bestMatchTP_pdgId->at(j)) == 13 ){
        Object L3OIAssoTrack;
        L3OIAssoTrack.SetPtEtaPhiM(hltIterL3OIMuonTrackAssociated_pt->at(j),hltIterL3OIMuonTrackAssociated_eta->at(j),hltIterL3OIMuonTrackAssociated_phi->at(j),0);
        L3OIAssoTrack.SetTag("OIAsso");
        L3OIAssoTrack.SetTruePU(truePU);
        L3OIAssoTracks.push_back(L3OIAssoTrack);
      }
    }
    vector<Object> TPtoL3OITracks;
    for(int j=0; j<ntpTo_hltIterL3OIMuonTrackAssociated; j++){
      if( fabs(tpTo_hltIterL3OIMuonTrackAssociated_gen_eta->at(j)) < 2.4 && tpTo_hltIterL3OIMuonTrackAssociated_bestMatchTrk_pt->at(j) > -1 ){
        Object TPtoL3OITrack;
        TPtoL3OITrack.SetPtEtaPhiM(tpTo_hltIterL3OIMuonTrackAssociated_gen_pt->at(j),tpTo_hltIterL3OIMuonTrackAssociated_gen_eta->at(j),tpTo_hltIterL3OIMuonTrackAssociated_gen_phi->at(j),0);
        TPtoL3OITrack.SetTag("TPtoOI");
        TPtoL3OITrack.SetTruePU(truePU);
        TPtoL3OITracks.push_back(TPtoL3OITrack);
      }
    }
  
    vector<Object> Iter0L3Tracks;
    for(int j=0; j<nhltIter0IterL3MuonTrack; j++){
      Object Iter0L3Track;
      Iter0L3Track.SetPtEtaPhiM(hltIter0IterL3MuonTrack_pt->at(j),hltIter0IterL3MuonTrack_eta->at(j),hltIter0IterL3MuonTrack_phi->at(j),0);
      Iter0L3Track.SetTag("Iter0FromL2");
      Iter0L3Track.SetTruePU(truePU);
      Iter0L3Tracks.push_back(Iter0L3Track);
      //cout << "hltIter0IterL3MuonTrack_pt: " << hltIter0IterL3MuonTrack_pt->at(j) << endl;
    }
    vector<Object> Iter0L3AssoTracks;
    for(int j=0; j<nhltIter0IterL3MuonTrackAssociated; j++){
      if( hltIter0IterL3MuonTrackAssociated_bestMatchTP_status->at(j) == 1 && fabs(hltIter0IterL3MuonTrackAssociated_bestMatchTP_pdgId->at(j)) == 13 ){
        Object Iter0L3AssoTrack;
        Iter0L3AssoTrack.SetPtEtaPhiM(hltIter0IterL3MuonTrackAssociated_pt->at(j),hltIter0IterL3MuonTrackAssociated_eta->at(j),hltIter0IterL3MuonTrackAssociated_phi->at(j),0);
        Iter0L3AssoTrack.SetTag("Iter0FromL2Asso");
        Iter0L3AssoTrack.SetTruePU(truePU);
        Iter0L3AssoTracks.push_back(Iter0L3AssoTrack);
      }
    }
    vector<Object> TPtoIter0L3Tracks;
    for(int j=0; j<ntpTo_hltIter0IterL3MuonTrackAssociated; j++){
      if( fabs(tpTo_hltIter0IterL3MuonTrackAssociated_gen_eta->at(j)) < 2.4 && tpTo_hltIter0IterL3MuonTrackAssociated_bestMatchTrk_pt->at(j) > -1 ){
        Object TPtoIter0L3Track;
        TPtoIter0L3Track.SetPtEtaPhiM(tpTo_hltIter0IterL3MuonTrackAssociated_gen_pt->at(j),tpTo_hltIter0IterL3MuonTrackAssociated_gen_eta->at(j),tpTo_hltIter0IterL3MuonTrackAssociated_gen_phi->at(j),0);
        TPtoIter0L3Track.SetTag("TPtoIter0FromL2");
        TPtoIter0L3Track.SetTruePU(truePU);
        TPtoIter0L3Tracks.push_back(TPtoIter0L3Track);
      }
    }
  
    vector<Object> Iter2L3Tracks;
    for(int j=0; j<nhltIter2IterL3MuonTrack; j++){
      Object Iter2L3Track;
      Iter2L3Track.SetPtEtaPhiM(hltIter2IterL3MuonTrack_pt->at(j),hltIter2IterL3MuonTrack_eta->at(j),hltIter2IterL3MuonTrack_phi->at(j),0);
      Iter2L3Track.SetTag("Iter2FromL2");
      Iter2L3Track.SetTruePU(truePU);
      Iter2L3Tracks.push_back(Iter2L3Track);
      //cout << "hltIter2IterL3MuonTrack_pt: " << hltIter2IterL3MuonTrack_pt->at(j) << endl;
    }
    vector<Object> Iter2L3AssoTracks;
    for(int j=0; j<nhltIter2IterL3MuonTrackAssociated; j++){
      if( hltIter2IterL3MuonTrackAssociated_bestMatchTP_status->at(j) == 1 && fabs(hltIter2IterL3MuonTrackAssociated_bestMatchTP_pdgId->at(j)) == 13 ){
        Object Iter2L3AssoTrack;
        Iter2L3AssoTrack.SetPtEtaPhiM(hltIter2IterL3MuonTrackAssociated_pt->at(j),hltIter2IterL3MuonTrackAssociated_eta->at(j),hltIter2IterL3MuonTrackAssociated_phi->at(j),0);
        Iter2L3AssoTrack.SetTag("Iter2FromL2Asso");
        Iter2L3AssoTrack.SetTruePU(truePU);
        Iter2L3AssoTracks.push_back(Iter2L3AssoTrack);
      }
    }
    vector<Object> TPtoIter2L3Tracks;
    for(int j=0; j<ntpTo_hltIter2IterL3MuonTrackAssociated; j++){
      if( fabs(tpTo_hltIter2IterL3MuonTrackAssociated_gen_eta->at(j)) < 2.4 && tpTo_hltIter2IterL3MuonTrackAssociated_bestMatchTrk_pt->at(j) > -1 ){
        Object TPtoIter2L3Track;
        TPtoIter2L3Track.SetPtEtaPhiM(tpTo_hltIter2IterL3MuonTrackAssociated_gen_pt->at(j),tpTo_hltIter2IterL3MuonTrackAssociated_gen_eta->at(j),tpTo_hltIter2IterL3MuonTrackAssociated_gen_phi->at(j),0);
        TPtoIter2L3Track.SetTag("TPtoIter2FromL2");
        TPtoIter2L3Track.SetTruePU(truePU);
        TPtoIter2L3Tracks.push_back(TPtoIter2L3Track);
      }
    }
  
    //vector<Object> Iter3L3Tracks;
    //for(int j=0; j<hltIter3IterL3MuonTrack_pt->size(); j++){
    //  Object Iter3L3Track;
    //  Iter3L3Track.SetPtEtaPhiM(hltIter3IterL3MuonTrack_pt->at(j),hltIter3IterL3MuonTrack_eta->at(j),hltIter3IterL3MuonTrack_phi->at(j),0);
    //  Iter3L3Tracks.push_back(Iter3L3Track);
    //  //cout << "hltIter3IterL3MuonTrack_pt: " << hltIter3IterL3MuonTrack_pt->at(j) << endl;
    //}
  
    vector<Object> Iter0L3FromL1Tracks;
    for(int j=0; j<nhltIter0IterL3FromL1MuonTrack; j++){
      Object Iter0L3FromL1Track;
      Iter0L3FromL1Track.SetPtEtaPhiM(hltIter0IterL3FromL1MuonTrack_pt->at(j),hltIter0IterL3FromL1MuonTrack_eta->at(j),hltIter0IterL3FromL1MuonTrack_phi->at(j),0);
      Iter0L3FromL1Track.SetTag("Iter0FromL1");
      Iter0L3FromL1Track.SetTruePU(truePU);
      Iter0L3FromL1Tracks.push_back(Iter0L3FromL1Track);
      //cout << "hltIter0IterL3FromL1MuonTrack_pt: " << hltIter0IterL3FromL1MuonTrack_pt->at(j) << endl;
    }
    vector<Object> Iter0L3FromL1AssoTracks;
    for(int j=0; j<nhltIter0IterL3FromL1MuonTrackAssociated; j++){
      if( hltIter0IterL3FromL1MuonTrackAssociated_bestMatchTP_status->at(j) == 1 && fabs(hltIter0IterL3FromL1MuonTrackAssociated_bestMatchTP_pdgId->at(j)) == 13 ){
        Object Iter0L3FromL1AssoTrack;
        Iter0L3FromL1AssoTrack.SetPtEtaPhiM(hltIter0IterL3FromL1MuonTrackAssociated_pt->at(j),hltIter0IterL3FromL1MuonTrackAssociated_eta->at(j),hltIter0IterL3FromL1MuonTrackAssociated_phi->at(j),0);
        Iter0L3FromL1AssoTrack.SetTag("Iter0FromL1Asso");
        Iter0L3FromL1AssoTrack.SetTruePU(truePU);
        Iter0L3FromL1AssoTracks.push_back(Iter0L3FromL1AssoTrack);
      }
    }
    vector<Object> TPtoIter0L3FromL1Tracks;
    for(int j=0; j<ntpTo_hltIter0IterL3FromL1MuonTrackAssociated; j++){
      if( fabs(tpTo_hltIter0IterL3FromL1MuonTrackAssociated_gen_eta->at(j)) < 2.4 && tpTo_hltIter0IterL3FromL1MuonTrackAssociated_bestMatchTrk_pt->at(j) > -1 ){
        Object TPtoIter0L3FromL1Track;
        TPtoIter0L3FromL1Track.SetPtEtaPhiM(tpTo_hltIter0IterL3FromL1MuonTrackAssociated_gen_pt->at(j),tpTo_hltIter0IterL3FromL1MuonTrackAssociated_gen_eta->at(j),tpTo_hltIter0IterL3FromL1MuonTrackAssociated_gen_phi->at(j),0);
        TPtoIter0L3FromL1Track.SetTag("TPtoIter0FromL1");
        TPtoIter0L3FromL1Track.SetTruePU(truePU);
        TPtoIter0L3FromL1Tracks.push_back(TPtoIter0L3FromL1Track);
      }
    }
  
    if ( nhltIter2IterL3FromL1MuonTrack != nhltIter2IterL3FromL1MuonTrackAssociated ){
      cout << "ERROR :: nhltIter2IterL3FromL1MuonTrack == " << nhltIter2IterL3FromL1MuonTrack << ", nhltIter2IterL3FromL1MuonTrackAssociated == " << nhltIter2IterL3FromL1MuonTrackAssociated << " ." << endl;
      exit(1);
    }
    vector<Object> Iter2L3FromL1Tracks;
    for(int j=0; j<nhltIter2IterL3FromL1MuonTrack; j++){
      Object Iter2L3FromL1Track;
      Iter2L3FromL1Track.SetPtEtaPhiM(hltIter2IterL3FromL1MuonTrack_pt->at(j),hltIter2IterL3FromL1MuonTrack_eta->at(j),hltIter2IterL3FromL1MuonTrack_phi->at(j),0);
      Iter2L3FromL1Track.SetTag("Iter2FromL1");
      Iter2L3FromL1Track.SetTruePU(truePU);
      Iter2L3FromL1Tracks.push_back(Iter2L3FromL1Track);
      NIter2FromL1Track+=1;
      //cout << "hltIter2IterL3FromL1MuonTrack_pt: " << hltIter2IterL3FromL1MuonTrack_pt->at(j) << endl;
    }
    vector<Object> Iter2L3FromL1AssoTracks;
    for(int j=0; j<nhltIter2IterL3FromL1MuonTrackAssociated; j++){
      if( hltIter2IterL3FromL1MuonTrackAssociated_bestMatchTP_status->at(j) == 1 && fabs(hltIter2IterL3FromL1MuonTrackAssociated_bestMatchTP_pdgId->at(j)) == 13 ){
        Object Iter2L3FromL1AssoTrack;
        Iter2L3FromL1AssoTrack.SetPtEtaPhiM(hltIter2IterL3FromL1MuonTrackAssociated_pt->at(j),hltIter2IterL3FromL1MuonTrackAssociated_eta->at(j),hltIter2IterL3FromL1MuonTrackAssociated_phi->at(j),0);
        Iter2L3FromL1AssoTrack.SetTag("Iter2FromL1Asso");
        Iter2L3FromL1AssoTrack.SetTruePU(truePU);
        Iter2L3FromL1AssoTracks.push_back(Iter2L3FromL1AssoTrack);
      }
    }
    vector<Object> TPtoIter2L3FromL1Tracks;
    for(int j=0; j<ntpTo_hltIter2IterL3FromL1MuonTrackAssociated; j++){
      if( fabs(tpTo_hltIter2IterL3FromL1MuonTrackAssociated_gen_eta->at(j)) < 2.4 && tpTo_hltIter2IterL3FromL1MuonTrackAssociated_bestMatchTrk_pt->at(j) > -1 ){
        Object TPtoIter2L3FromL1Track;
        TPtoIter2L3FromL1Track.SetPtEtaPhiM(tpTo_hltIter2IterL3FromL1MuonTrackAssociated_gen_pt->at(j),tpTo_hltIter2IterL3FromL1MuonTrackAssociated_gen_eta->at(j),tpTo_hltIter2IterL3FromL1MuonTrackAssociated_gen_phi->at(j),0);
        TPtoIter2L3FromL1Track.SetTag("TPtoIter2FromL1");
        TPtoIter2L3FromL1Track.SetTruePU(truePU);
        TPtoIter2L3FromL1Tracks.push_back(TPtoIter2L3FromL1Track);
      }
    }
  
    //vector<Object> Iter3L3FromL1Tracks;
    //for(int j=0; j<hltIter3IterL3FromL1MuonTrack_pt->size(); j++){
    //  Object Iter3L3FromL1Track;
    //  Iter3L3FromL1Track.SetPtEtaPhiM(hltIter3IterL3FromL1MuonTrack_pt->at(j),hltIter3IterL3FromL1MuonTrack_eta->at(j),hltIter3IterL3FromL1MuonTrack_phi->at(j),0);
    //  Iter3L3FromL1Tracks.push_back(Iter3L3FromL1Track);
    //  //cout << "hltIter3IterL3FromL1MuonTrack_pt: " << hltIter3IterL3FromL1MuonTrack_pt->at(j) << endl;
    //}

    //==IO(Iter0,2)FromL2==//
    vector<Object> IOFromL2Tracks;
    for(int j=0; j<nhltIter2IterL3MuonMergedAssociated; j++){
      Object IOFromL2Track;
      IOFromL2Track.SetPtEtaPhiM(hltIter2IterL3MuonMergedAssociated_pt->at(j),hltIter2IterL3MuonMergedAssociated_eta->at(j),hltIter2IterL3MuonMergedAssociated_phi->at(j),0);
      IOFromL2Track.SetTag("IOFromL2");
      IOFromL2Track.SetTruePU(truePU);
      IOFromL2Tracks.push_back(IOFromL2Track);
    }
    vector<Object> IOFromL2AssoTracks;
    for(int j=0; j<nhltIter2IterL3MuonMergedAssociated; j++){
      if( hltIter2IterL3MuonMergedAssociated_bestMatchTP_status->at(j) == 1 && fabs(hltIter2IterL3MuonMergedAssociated_bestMatchTP_pdgId->at(j)) == 13 ){
        Object IOFromL2AssoTrack;
        IOFromL2AssoTrack.SetPtEtaPhiM(hltIter2IterL3MuonMergedAssociated_pt->at(j),hltIter2IterL3MuonMergedAssociated_eta->at(j),hltIter2IterL3MuonMergedAssociated_phi->at(j),0);
        IOFromL2AssoTrack.SetTag("IOFromL2Asso");
        IOFromL2AssoTrack.SetTruePU(truePU);
        IOFromL2AssoTracks.push_back(IOFromL2AssoTrack);
      }
    }
    vector<Object> TPtoIOFromL2Tracks;
    for(int j=0; j<ntpTo_hltIter2IterL3MuonMergedAssociated; j++){
      if( fabs(tpTo_hltIter2IterL3MuonMergedAssociated_gen_eta->at(j)) < 2.4 && tpTo_hltIter2IterL3MuonMergedAssociated_bestMatchTrk_pt->at(j) > -1 ){
        Object TPtoIOFromL2Track;
        TPtoIOFromL2Track.SetPtEtaPhiM(tpTo_hltIter2IterL3MuonMergedAssociated_gen_pt->at(j),tpTo_hltIter2IterL3MuonMergedAssociated_gen_eta->at(j),tpTo_hltIter2IterL3MuonMergedAssociated_gen_phi->at(j),0);
        TPtoIOFromL2Track.SetTag("TPtoIOFromL2");
        TPtoIOFromL2Track.SetTruePU(truePU);
        TPtoIOFromL2Tracks.push_back(TPtoIOFromL2Track);
      }
    }


    //==IO(Iter0,2(,3 for Winter20_112X))FromL1==//
    vector<Object> IOFromL1Tracks;
    //if(output.Contains("Winter20")){
    //  //cout << "iterL3IOFromL1_size: " << nIterL3IOFromL1 << endl;
    //  for(int j=0; j<nIterL3IOFromL1; j++){
    //    Object IOFromL1Track;
    //    IOFromL1Track.SetPtEtaPhiM(iterL3IOFromL1_pt[j],iterL3IOFromL1_eta[j],iterL3IOFromL1_phi[j],0);
    //    IOFromL1Track.SetTag("IOFromL1");
    //    IOFromL1Track.SetTruePU(truePU);
    //    IOFromL1Tracks.push_back(IOFromL1Track);
    //    //cout << "iterL3IOFromL1_pt: " << iterL3IOFromL1_pt[j] << endl;
    //  }
    //}
    //else if(output.Contains("Winter21")){
    //  for(int j=0; j<nhltIter2IterL3FromL1MuonMergedAssociated; j++){
    //    Object IOFromL1Track;
    //    IOFromL1Track.SetPtEtaPhiM(hltIter2IterL3FromL1MuonMergedAssociated_pt->at(j),hltIter2IterL3FromL1MuonMergedAssociated_eta->at(j),hltIter2IterL3FromL1MuonMergedAssociated_phi->at(j),0);
    //    IOFromL1Track.SetTag("IOFromL1");
    //    IOFromL1Track.SetTruePU(truePU);
    //    IOFromL1Tracks.push_back(IOFromL1Track);
    //    //cout << "hltIter2IterL3FromL1MuonMergedAssociated_pt: " << hltIter2IterL3FromL1MuonMergedAssociated_pt->at(j) << endl;
    //  }
    //}
    for(int j=0; j<nhltIter2IterL3FromL1MuonMergedAssociated; j++){
      Object IOFromL1Track;
      IOFromL1Track.SetPtEtaPhiM(hltIter2IterL3FromL1MuonMergedAssociated_pt->at(j),hltIter2IterL3FromL1MuonMergedAssociated_eta->at(j),hltIter2IterL3FromL1MuonMergedAssociated_phi->at(j),0);
      IOFromL1Track.SetTag("IOFromL1");
      IOFromL1Track.SetTruePU(truePU);
      IOFromL1Tracks.push_back(IOFromL1Track);
      //cout << "hltIter2IterL3FromL1MuonMergedAssociated_pt: " << hltIter2IterL3FromL1MuonMergedAssociated_pt->at(j) << endl;
    }
    vector<Object> IOFromL1AssoTracks;
    for(int j=0; j<nhltIter2IterL3FromL1MuonMergedAssociated; j++){
      if( hltIter2IterL3FromL1MuonMergedAssociated_bestMatchTP_status->at(j) == 1 && fabs(hltIter2IterL3FromL1MuonMergedAssociated_bestMatchTP_pdgId->at(j)) == 13 ){
        Object IOFromL1AssoTrack;
        IOFromL1AssoTrack.SetPtEtaPhiM(hltIter2IterL3FromL1MuonMergedAssociated_pt->at(j),hltIter2IterL3FromL1MuonMergedAssociated_eta->at(j),hltIter2IterL3FromL1MuonMergedAssociated_phi->at(j),0);
        IOFromL1AssoTrack.SetTag("IOFromL1Asso");
        IOFromL1AssoTrack.SetTruePU(truePU);
        IOFromL1AssoTracks.push_back(IOFromL1AssoTrack);
      }
    }
    vector<Object> TPtoIOFromL1Tracks;
    for(int j=0; j<ntpTo_hltIter2IterL3FromL1MuonMergedAssociated; j++){
      if( fabs(tpTo_hltIter2IterL3FromL1MuonMergedAssociated_gen_eta->at(j)) < 2.4 && tpTo_hltIter2IterL3FromL1MuonMergedAssociated_bestMatchTrk_pt->at(j) > -1 ){
        Object TPtoIOFromL1Track;
        TPtoIOFromL1Track.SetPtEtaPhiM(tpTo_hltIter2IterL3FromL1MuonMergedAssociated_gen_pt->at(j),tpTo_hltIter2IterL3FromL1MuonMergedAssociated_gen_eta->at(j),tpTo_hltIter2IterL3FromL1MuonMergedAssociated_gen_phi->at(j),0);
        TPtoIOFromL1Track.SetTag("TPtoIOFromL1");
        TPtoIOFromL1Track.SetTruePU(truePU);
        TPtoIOFromL1Tracks.push_back(TPtoIOFromL1Track);
      }
    }

    //==OI+IO(Iter0,2,3)FromL2==//
    vector<Object> UpToFromL2Tracks;
    for(int j=0; j<nhltIterL3MuonMergedAssociated; j++){
      Object UpToFromL2Track;
      UpToFromL2Track.SetPtEtaPhiM(hltIterL3MuonMergedAssociated_pt->at(j),hltIterL3MuonMergedAssociated_eta->at(j),hltIterL3MuonMergedAssociated_phi->at(j),0);
      UpToFromL2Track.SetTag("UpToFromL2");
      UpToFromL2Track.SetTruePU(truePU);
      UpToFromL2Tracks.push_back(UpToFromL2Track);
    }
    vector<Object> UpToFromL2AssoTracks;
    for(int j=0; j<nhltIterL3MuonMergedAssociated; j++){
      if( hltIterL3MuonMergedAssociated_bestMatchTP_status->at(j) == 1 && fabs(hltIterL3MuonMergedAssociated_bestMatchTP_pdgId->at(j)) == 13 ){
        Object UpToFromL2AssoTrack;
        UpToFromL2AssoTrack.SetPtEtaPhiM(hltIterL3MuonMergedAssociated_pt->at(j),hltIterL3MuonMergedAssociated_eta->at(j),hltIterL3MuonMergedAssociated_phi->at(j),0);
        UpToFromL2AssoTrack.SetTag("UpToFromL2Asso");
        UpToFromL2AssoTrack.SetTruePU(truePU);
        UpToFromL2AssoTracks.push_back(UpToFromL2AssoTrack);
      }
    }
    vector<Object> TPtoUpToFromL2Tracks;
    for(int j=0; j<ntpTo_hltIterL3MuonMergedAssociated; j++){
      if( fabs(tpTo_hltIterL3MuonMergedAssociated_gen_eta->at(j)) < 2.4 && tpTo_hltIterL3MuonMergedAssociated_bestMatchTrk_pt->at(j) > -1 ){
        Object TPtoUpToFromL2Track;
        TPtoUpToFromL2Track.SetPtEtaPhiM(tpTo_hltIterL3MuonMergedAssociated_gen_pt->at(j),tpTo_hltIterL3MuonMergedAssociated_gen_eta->at(j),tpTo_hltIterL3MuonMergedAssociated_gen_phi->at(j),0);
        TPtoUpToFromL2Track.SetTag("TPtoUpToFromL2");
        TPtoUpToFromL2Track.SetTruePU(truePU);
        TPtoUpToFromL2Tracks.push_back(TPtoUpToFromL2Track);
      }
    }

    //==OI+IO(Iter0,2,3)FromL2+IO(Iter0,2,3)FromL1==//
    vector<Object> UpToFromL1Tracks;
    for(int j=0; j<nhltIterL3MuonAndMuonFromL1MergedAssociated; j++){
      Object UpToFromL1Track;
      UpToFromL1Track.SetPtEtaPhiM(hltIterL3MuonAndMuonFromL1MergedAssociated_pt->at(j),hltIterL3MuonAndMuonFromL1MergedAssociated_eta->at(j),hltIterL3MuonAndMuonFromL1MergedAssociated_phi->at(j),0);
      UpToFromL1Track.SetTag("UpToFromL1");
      UpToFromL1Track.SetTruePU(truePU);
      UpToFromL1Tracks.push_back(UpToFromL1Track);
    }
    vector<Object> UpToFromL1AssoTracks;
    for(int j=0; j<nhltIterL3MuonAndMuonFromL1MergedAssociated; j++){
      if( hltIterL3MuonAndMuonFromL1MergedAssociated_bestMatchTP_status->at(j) == 1 && fabs(hltIterL3MuonAndMuonFromL1MergedAssociated_bestMatchTP_pdgId->at(j)) == 13 ){
        Object UpToFromL1AssoTrack;
        UpToFromL1AssoTrack.SetPtEtaPhiM(hltIterL3MuonAndMuonFromL1MergedAssociated_pt->at(j),hltIterL3MuonAndMuonFromL1MergedAssociated_eta->at(j),hltIterL3MuonAndMuonFromL1MergedAssociated_phi->at(j),0);
        UpToFromL1AssoTrack.SetTag("UpToFromL1Asso");
        UpToFromL1AssoTrack.SetTruePU(truePU);
        UpToFromL1AssoTracks.push_back(UpToFromL1AssoTrack);
      }
    }
    vector<Object> TPtoUpToFromL1Tracks;
    for(int j=0; j<ntpTo_hltIterL3MuonAndMuonFromL1MergedAssociated; j++){
      if( fabs(tpTo_hltIterL3MuonAndMuonFromL1MergedAssociated_gen_eta->at(j)) < 2.4 && tpTo_hltIterL3MuonAndMuonFromL1MergedAssociated_bestMatchTrk_pt->at(j) > -1 ){
        Object TPtoUpToFromL1Track;
        TPtoUpToFromL1Track.SetPtEtaPhiM(tpTo_hltIterL3MuonAndMuonFromL1MergedAssociated_gen_pt->at(j),tpTo_hltIterL3MuonAndMuonFromL1MergedAssociated_gen_eta->at(j),tpTo_hltIterL3MuonAndMuonFromL1MergedAssociated_gen_phi->at(j),0);
        TPtoUpToFromL1Track.SetTag("TPtoUpToFromL1");
        TPtoUpToFromL1Track.SetTruePU(truePU);
        TPtoUpToFromL1Tracks.push_back(TPtoUpToFromL1Track);
      }
    }

    //==L3MuonNoID Track==//
    vector<Object> L3MuonTracks_NoID;
    for(int j=0; j<niterL3MuonNoIDTrackAssociated; j++){
      Object L3MuonTrack_NoID;
      L3MuonTrack_NoID.SetPtEtaPhiM(iterL3MuonNoIDTrackAssociated_pt->at(j),iterL3MuonNoIDTrackAssociated_eta->at(j),iterL3MuonNoIDTrackAssociated_phi->at(j),0);
      L3MuonTrack_NoID.SetTag("L3NoIDTrack");
      L3MuonTrack_NoID.SetTruePU(truePU);
      L3MuonTracks_NoID.push_back(L3MuonTrack_NoID);
    }
    vector<Object> L3MuonAssoTracks_NoID;
    for(int j=0; j<niterL3MuonNoIDTrackAssociated; j++){
      if( iterL3MuonNoIDTrackAssociated_bestMatchTP_status->at(j) == 1 && fabs(iterL3MuonNoIDTrackAssociated_bestMatchTP_pdgId->at(j)) == 13 ){
        Object L3MuonAssoTrack_NoID;
        L3MuonAssoTrack_NoID.SetPtEtaPhiM(iterL3MuonNoIDTrackAssociated_pt->at(j),iterL3MuonNoIDTrackAssociated_eta->at(j),iterL3MuonNoIDTrackAssociated_phi->at(j),0);
        L3MuonAssoTrack_NoID.SetTag("L3NoIDTrackAsso");
        L3MuonAssoTrack_NoID.SetTruePU(truePU);
        L3MuonAssoTracks_NoID.push_back(L3MuonAssoTrack_NoID);
      }
    }
    vector<Object> TPtoL3MuonTracks_NoID;
    for(int j=0; j<ntpTo_iterL3MuonNoIDTrackAssociated; j++){
      if( fabs(tpTo_iterL3MuonNoIDTrackAssociated_gen_eta->at(j)) < 2.4 && tpTo_iterL3MuonNoIDTrackAssociated_bestMatchTrk_pt->at(j) > -1 ){
        Object TPtoL3MuonTrack_NoID;
        TPtoL3MuonTrack_NoID.SetPtEtaPhiM(tpTo_iterL3MuonNoIDTrackAssociated_gen_pt->at(j),tpTo_iterL3MuonNoIDTrackAssociated_gen_eta->at(j),tpTo_iterL3MuonNoIDTrackAssociated_gen_phi->at(j),0);
        TPtoL3MuonTrack_NoID.SetTag("TPtoL3NoIDTrack");
        TPtoL3MuonTrack_NoID.SetTruePU(truePU);
        TPtoL3MuonTracks_NoID.push_back(TPtoL3MuonTrack_NoID);
      }
    }

    //==L3Muon Track==//
    vector<Object> L3MuonTracks;
    for(int j=0; j<niterL3MuonTrackAssociated; j++){
      Object L3MuonTrack;
      L3MuonTrack.SetPtEtaPhiM(iterL3MuonTrackAssociated_pt->at(j),iterL3MuonTrackAssociated_eta->at(j),iterL3MuonTrackAssociated_phi->at(j),0);
      L3MuonTrack.SetTag("L3Track");
      L3MuonTrack.SetTruePU(truePU);
      L3MuonTracks.push_back(L3MuonTrack);
    }
    vector<Object> L3MuonAssoTracks;
    for(int j=0; j<niterL3MuonTrackAssociated; j++){
      if( iterL3MuonTrackAssociated_bestMatchTP_status->at(j) == 1 && fabs(iterL3MuonTrackAssociated_bestMatchTP_pdgId->at(j)) == 13 ){
        Object L3MuonAssoTrack;
        L3MuonAssoTrack.SetPtEtaPhiM(iterL3MuonTrackAssociated_pt->at(j),iterL3MuonTrackAssociated_eta->at(j),iterL3MuonTrackAssociated_phi->at(j),0);
        L3MuonAssoTrack.SetTag("L3TrackAsso");
        L3MuonAssoTrack.SetTruePU(truePU);
        L3MuonAssoTracks.push_back(L3MuonAssoTrack);
      }
    }
    vector<Object> TPtoL3MuonTracks;
    for(int j=0; j<ntpTo_iterL3MuonTrackAssociated; j++){
      if( fabs(tpTo_iterL3MuonTrackAssociated_gen_eta->at(j)) < 2.4 && tpTo_iterL3MuonTrackAssociated_bestMatchTrk_pt->at(j) > -1 ){
        Object TPtoL3MuonTrack;
        TPtoL3MuonTrack.SetPtEtaPhiM(tpTo_iterL3MuonTrackAssociated_gen_pt->at(j),tpTo_iterL3MuonTrackAssociated_gen_eta->at(j),tpTo_iterL3MuonTrackAssociated_gen_phi->at(j),0);
        TPtoL3MuonTrack.SetTag("TPtoL3Track");
        TPtoL3MuonTrack.SetTruePU(truePU);
        TPtoL3MuonTracks.push_back(TPtoL3MuonTrack);
      }
    }



    vector< vector<Object> > Tracks;
    Tracks.push_back(L3OITracks);
    Tracks.push_back(Iter0L3Tracks);
    Tracks.push_back(Iter2L3Tracks);
    Tracks.push_back(Iter0L3FromL1Tracks);
    Tracks.push_back(Iter2L3FromL1Tracks);
 

    vector<Object> this_gens;
    vector<Object> this_gens_above;
   
    //Use this with Jpsi RAW only sample
    //if(input.Contains("Jpsi")){
    //  this_gens = JpsiCand;
    //  this_gens_above = JpsiCand_above;
    //  check_Jpsi_dR(this_gens, genEventWeight);
    //}
    //else if(input.Contains("QCD")){
    //  this_gens = Finals;
    //  this_gens_above = Finals_above;
    //}
    //else{
    //  this_gens = hardPs;
    //  this_gens_above = hardPs_above;
    //}
    this_gens = hardPs;
    this_gens_above = hardPs_above;

  //====================================Efficiency vs pt========================================//
 
    do_eff_den_pt(this_gens, genEventWeight);
    do_trackeff_pt(this_gens, Tracks, 0.3, genEventWeight, "all");
    do_eff_num_pt(this_gens, IOFromL1Tracks, 0.1, genEventWeight);
    do_eff_num_pt(this_gens, L3Muons_NoID, 0.1, genEventWeight);
    do_eff_num_pt(this_gens, L3Muons, 0.1, genEventWeight);

    do_eff_den_pt(hardPs_L1matched, genEventWeight);
    do_trackeff_pt(hardPs_L1matched, Tracks, 0.3, genEventWeight, "all");
    do_eff_num_pt(hardPs_L1matched, IOFromL1Tracks, 0.1, genEventWeight);
    do_eff_num_pt(hardPs_L1matched, L3Muons_NoID, 0.1, genEventWeight);
    do_eff_num_pt(hardPs_L1matched, L3Muons, 0.1, genEventWeight);

    do_eff_den_pt(hardPs_L1matched_med, genEventWeight);
    do_trackeff_pt(hardPs_L1matched_med, Tracks, 0.3, genEventWeight, "all");
    do_eff_num_pt(hardPs_L1matched_med, IOFromL1Tracks, 0.1, genEventWeight);
    do_eff_num_pt(hardPs_L1matched_med, L3Muons_NoID, 0.1, genEventWeight);
    do_eff_num_pt(hardPs_L1matched_med, L3Muons, 0.1, genEventWeight);

  //====================================Efficiency vs eta, phi, truePU========================================//

    do_eff_den_others(this_gens_above, genEventWeight);
    do_trackeff_others(this_gens_above, Tracks, 0.3, genEventWeight, "all");
    do_eff_num_others(this_gens_above, IOFromL1Tracks, 0.1, genEventWeight);
    do_eff_num_others(this_gens_above, L3Muons_NoID, 0.1, genEventWeight);
    do_eff_num_others(this_gens_above, L3Muons, 0.1, genEventWeight);

    do_eff_den_others(hardPs_above_L1matched, genEventWeight);
    do_trackeff_others(hardPs_above_L1matched, Tracks, 0.3, genEventWeight, "all");
    do_eff_num_others(hardPs_above_L1matched, IOFromL1Tracks, 0.1, genEventWeight);
    do_eff_num_others(hardPs_above_L1matched, L3Muons_NoID, 0.1, genEventWeight);
    do_eff_num_others(hardPs_above_L1matched, L3Muons, 0.1, genEventWeight);

    do_eff_den_others(hardPs_above_L1matched_med, genEventWeight);
    do_trackeff_others(hardPs_above_L1matched_med, Tracks, 0.3, genEventWeight, "all");
    do_eff_num_others(hardPs_above_L1matched_med, IOFromL1Tracks, 0.1, genEventWeight);
    do_eff_num_others(hardPs_above_L1matched_med, L3Muons_NoID, 0.1, genEventWeight);
    do_eff_num_others(hardPs_above_L1matched_med, L3Muons, 0.1, genEventWeight);

  //=======================New efficiency (hit association) vs pt, eta, phi, truePU============================//

    do_neweff_den_vars(TPs, genEventWeight);
    do_neweff_num_vars(TPtoL3OITracks, genEventWeight);
    do_neweff_num_vars(TPtoIter0L3Tracks, genEventWeight);
    do_neweff_num_vars(TPtoIter2L3Tracks, genEventWeight);
    do_neweff_num_vars(TPtoIter0L3FromL1Tracks, genEventWeight);
    do_neweff_num_vars(TPtoIter2L3FromL1Tracks, genEventWeight);
    do_neweff_num_vars(TPtoIOFromL2Tracks, genEventWeight);
    do_neweff_num_vars(TPtoIOFromL1Tracks, genEventWeight);
    do_neweff_num_vars(TPtoUpToFromL2Tracks, genEventWeight);
    do_neweff_num_vars(TPtoUpToFromL1Tracks, genEventWeight);
    do_neweff_num_vars(TPtoL3MuonTracks_NoID, genEventWeight);
    do_neweff_num_vars(TPtoL3MuonTracks, genEventWeight);

  //==========================================Purity====================================================//

    do_purity(L3Muons_NoID, Finals, 0.1, genEventWeight);
    do_purity(L3Muons, Finals, 0.1, genEventWeight);
    do_purity(L3MuonTracks_NoID, L3MuonAssoTracks_NoID, Finals, 0.1, genEventWeight);
    do_purity(L3MuonTracks, L3MuonAssoTracks, Finals, 0.1, genEventWeight);
    do_purity(Iter2L3FromL1Tracks, Iter2L3FromL1AssoTracks, Finals, 0.1, genEventWeight);
    do_purity(IOFromL1Tracks, IOFromL1AssoTracks, Finals, 0.1, genEventWeight);

  //==========================================dR scan===================================================//
    //hardP vs tracks
    draw_dR(this_gens, Tracks[0], genEventWeight);
    draw_dR(this_gens, Tracks[1], genEventWeight);
    draw_dR(this_gens, Tracks[2], genEventWeight);
    draw_dR(this_gens, Tracks[3], genEventWeight);
    draw_dR(this_gens, Tracks[4], genEventWeight);
    //hardP vs L1s
    draw_dR(this_gens, L1Muons, genEventWeight);
    draw_dR(this_gens, L1Muons_med, genEventWeight);
    draw_dR(this_gens, L1Muons_AtVtx, genEventWeight);
    draw_dR(this_gens, L1Muons_med_AtVtx, genEventWeight);
    //hardP vs L3s
    draw_dR(this_gens, L3Muons_NoID, genEventWeight);
    draw_dR(this_gens, L3Muons, genEventWeight);
    

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
  for(const auto& [histname,hist]:hists_2D){
    //cout << "save " << histname << endl;
    hist->Write();
  }
  for(const auto& [pfname,pf]:pfs){
    //cout << "save " << pfname << endl;
    pf->Write();
  }
  pf_track->Write();
  pf_trackFromL1->Write();
  fout.Close();

  for(auto& [histname,hist]:hists){
    delete hists[histname];
  }
  for(auto& [histname,hist]:hists_2D){
    delete hists_2D[histname];
  }
  for(auto& [pfname,pf]:pfs){
    delete pfs[pfname];
  }
  delete pf_track;
  delete pf_trackFromL1;

}



//TH1 -> GetCumulative (forward = false)
//First, draw Nsignal vs mva(sigmoid), Nbkg vs mva(sigmoid).
//Then calculate GetCumulative(some bin)/GetCumulative(0).
//Iter2 track, Iter2FromL1 track
