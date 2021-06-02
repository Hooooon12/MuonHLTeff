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
      FillHist(gens.at(j).Tag()+"_pt_dR_"+recos.at(Matched).Tag()+"_2D",gens.at(j).Pt(),gens.at(j).DeltaR(recos.at(Matched)),weight,3000,0,1500,100,0,10);
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

    for(int this_step=0; this_step<5; this_step++){

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


    for(int this_step=0; this_step<5; this_step++){

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


void calculateEff_new(TString input, TString output){

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
  double L1Muon_etaAtVtx[5000];
  double L1Muon_phiAtVtx[5000];
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
  //vector<double> *hltIter3IterL3MuonTrack_pt = 0;
  //vector<double> *hltIter3IterL3MuonTrack_eta = 0;
  //vector<double> *hltIter3IterL3MuonTrack_phi = 0;
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
  //fChain->SetBranchAddress("hltIter3IterL3MuonTrack_pt",&hltIter3IterL3MuonTrack_pt);
  //fChain->SetBranchAddress("hltIter3IterL3MuonTrack_eta",&hltIter3IterL3MuonTrack_eta);
  //fChain->SetBranchAddress("hltIter3IterL3MuonTrack_phi",&hltIter3IterL3MuonTrack_phi);
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

    //==Call Jpsi candidates==//
    vector<Object> JpsiCand;
    vector<Object> JpsiCand_above;
    Object Lep1, Lep2;
    double tmpMassDiff = 10000.;
    int i1 = 0, i2 = 0;
    if(Final_idx.size() > 1){
      for(int j=0; j<Final_idx.size(); j++){
        for(int k=j+1; k<Final_idx.size(); k++){
          Object tmpLep1, tmpLep2;
          tmpLep1.SetPtEtaPhiM(genParticle_pt[Final_idx.at(j)],genParticle_eta[Final_idx.at(j)],genParticle_phi[Final_idx.at(j)],0);
          tmpLep2.SetPtEtaPhiM(genParticle_pt[Final_idx.at(k)],genParticle_eta[Final_idx.at(k)],genParticle_phi[Final_idx.at(k)],0);
          if( fabs( (tmpLep1+tmpLep2).M() - 3.1 ) < tmpMassDiff ){
            tmpMassDiff = fabs( (tmpLep1+tmpLep2).M() - 3.1 );
            Lep1 = tmpLep1; Lep2 = tmpLep2;
          }
        }
      }
      if(3. < (Lep1+Lep2).M() && (Lep1+Lep2).M() < 3.2){
        Lep1.SetTag("hardP");
        Lep2.SetTag("hardP");
        Lep1.SetTruePU(truePU);
        Lep2.SetTruePU(truePU);
        JpsiCand.push_back(Lep1);
        JpsiCand.push_back(Lep2);

        if(Lep1.Pt() > 5) JpsiCand_above.push_back(Lep1);
        if(Lep2.Pt() > 5) JpsiCand_above.push_back(Lep2);
      }
    }
    //Sanity check//
    //if(JpsiCand.size() > 0) cout << "N of JpsiCand : " << JpsiCand.size() << ", Jpsi mass : " << (JpsiCand[0]+JpsiCand[1]).M() << ", pt : " << JpsiCand[0].Pt() << ", " << JpsiCand[1].Pt() << endl;

  
    //==tracks to measure eff==//
    vector<Object> L3OITracks;
    for(int j=0; j<hltIterL3OIMuonTrack_pt->size(); j++){
      Object L3OITrack;
      L3OITrack.SetPtEtaPhiM(hltIterL3OIMuonTrack_pt->at(j),hltIterL3OIMuonTrack_eta->at(j),hltIterL3OIMuonTrack_phi->at(j),0);
      L3OITrack.SetTag("OI");
      L3OITracks.push_back(L3OITrack);
      //cout << "hltIterL3OIMuonTrack_pt: " << hltIterL3OIMuonTrack_pt->at(j) << endl;
    }
    //if(L3OITracks.size()>0) cout << "hltIterL3OIMuonTrack_pt: " << L3OITracks.at(0).Pt() << endl;
  
    vector<Object> Iter0L3Tracks;
    for(int j=0; j<hltIter0IterL3MuonTrack_pt->size(); j++){
      Object Iter0L3Track;
      Iter0L3Track.SetPtEtaPhiM(hltIter0IterL3MuonTrack_pt->at(j),hltIter0IterL3MuonTrack_eta->at(j),hltIter0IterL3MuonTrack_phi->at(j),0);
      Iter0L3Track.SetTag("Iter0FromL2");
      Iter0L3Tracks.push_back(Iter0L3Track);
      //cout << "hltIter0IterL3MuonTrack_pt: " << hltIter0IterL3MuonTrack_pt->at(j) << endl;
    }
  
    vector<Object> Iter2L3Tracks;
    for(int j=0; j<hltIter2IterL3MuonTrack_pt->size(); j++){
      Object Iter2L3Track;
      Iter2L3Track.SetPtEtaPhiM(hltIter2IterL3MuonTrack_pt->at(j),hltIter2IterL3MuonTrack_eta->at(j),hltIter2IterL3MuonTrack_phi->at(j),0);
      Iter2L3Track.SetTag("Iter2FromL2");
      Iter2L3Tracks.push_back(Iter2L3Track);
      //cout << "hltIter2IterL3MuonTrack_pt: " << hltIter2IterL3MuonTrack_pt->at(j) << endl;
    }
  
    //vector<Object> Iter3L3Tracks;
    //for(int j=0; j<hltIter3IterL3MuonTrack_pt->size(); j++){
    //  Object Iter3L3Track;
    //  Iter3L3Track.SetPtEtaPhiM(hltIter3IterL3MuonTrack_pt->at(j),hltIter3IterL3MuonTrack_eta->at(j),hltIter3IterL3MuonTrack_phi->at(j),0);
    //  Iter3L3Tracks.push_back(Iter3L3Track);
    //  //cout << "hltIter3IterL3MuonTrack_pt: " << hltIter3IterL3MuonTrack_pt->at(j) << endl;
    //}
  
    vector<Object> Iter0L3FromL1Tracks;
    for(int j=0; j<hltIter0IterL3FromL1MuonTrack_pt->size(); j++){
      Object Iter0L3FromL1Track;
      Iter0L3FromL1Track.SetPtEtaPhiM(hltIter0IterL3FromL1MuonTrack_pt->at(j),hltIter0IterL3FromL1MuonTrack_eta->at(j),hltIter0IterL3FromL1MuonTrack_phi->at(j),0);
      Iter0L3FromL1Track.SetTag("Iter0FromL1");
      Iter0L3FromL1Tracks.push_back(Iter0L3FromL1Track);
      //cout << "hltIter0IterL3FromL1MuonTrack_pt: " << hltIter0IterL3FromL1MuonTrack_pt->at(j) << endl;
    }
  
    vector<Object> Iter2L3FromL1Tracks;
    for(int j=0; j<hltIter2IterL3FromL1MuonTrack_pt->size(); j++){
      Object Iter2L3FromL1Track;
      Iter2L3FromL1Track.SetPtEtaPhiM(hltIter2IterL3FromL1MuonTrack_pt->at(j),hltIter2IterL3FromL1MuonTrack_eta->at(j),hltIter2IterL3FromL1MuonTrack_phi->at(j),0);
      Iter2L3FromL1Track.SetTag("Iter2FromL1");
      Iter2L3FromL1Track.SetTruePU(truePU);
      Iter2L3FromL1Tracks.push_back(Iter2L3FromL1Track);
      NIter2FromL1Track+=1;
      //cout << "hltIter2IterL3FromL1MuonTrack_pt: " << hltIter2IterL3FromL1MuonTrack_pt->at(j) << endl;
    }
  
    //vector<Object> Iter3L3FromL1Tracks;
    //for(int j=0; j<hltIter3IterL3FromL1MuonTrack_pt->size(); j++){
    //  Object Iter3L3FromL1Track;
    //  Iter3L3FromL1Track.SetPtEtaPhiM(hltIter3IterL3FromL1MuonTrack_pt->at(j),hltIter3IterL3FromL1MuonTrack_eta->at(j),hltIter3IterL3FromL1MuonTrack_phi->at(j),0);
    //  Iter3L3FromL1Tracks.push_back(Iter3L3FromL1Track);
    //  //cout << "hltIter3IterL3FromL1MuonTrack_pt: " << hltIter3IterL3FromL1MuonTrack_pt->at(j) << endl;
    //}
 
    vector< vector<Object> > Tracks;
    Tracks.push_back(L3OITracks);
    Tracks.push_back(Iter0L3Tracks);
    Tracks.push_back(Iter2L3Tracks);
    Tracks.push_back(Iter0L3FromL1Tracks);
    Tracks.push_back(Iter2L3FromL1Tracks);
 

    vector<Object> this_gens;
    vector<Object> this_gens_above;
    
    if(input.Contains("Jpsi")){
      this_gens = JpsiCand;
      this_gens_above = JpsiCand_above;
    }
    else if(input.Contains("QCD")){
      this_gens = Finals;
      this_gens_above = Finals_above;
    }
    else{
      this_gens = hardPs;
      this_gens_above = hardPs_above;
    }



    //if(input.Contains("Jpsi")){
    //  if(JpsiCand.size() > 0){
    //    do_eff_den_pt(JpsiCand, genEventWeight); 
    //    do_trackeff_pt(JpsiCand, Tracks, 0.3, genEventWeight, "all");
    //    do_eff_num_pt(JpsiCand, L3Muons_NoID, 0.1, genEventWeight);
    //    do_eff_num_pt(JpsiCand, L3Muons, 0.1, genEventWeight);
    //              
    //    do_eff_den_others(JpsiCand_above, genEventWeight); 
    //    do_trackeff_others(JpsiCand_above, Tracks, 0.3, genEventWeight, "all");
    //    do_eff_num_others(JpsiCand_above, L3Muons_NoID, 0.1, genEventWeight);
    //    do_eff_num_others(JpsiCand_above, L3Muons, 0.1, genEventWeight);
    //  }
    //}
   
    //else if(input.Contains("QCD")){
    //  do_eff_den_pt(Finals, genEventWeight); 
    //  do_trackeff_pt(Finals, Tracks, 0.3, genEventWeight, "all");
    //  do_eff_num_pt(Finals, L3Muons_NoID, 0.1, genEventWeight);
    //  do_eff_num_pt(Finals, L3Muons, 0.1, genEventWeight);
    //            
    //  do_eff_den_others(Finals_above, genEventWeight); 
    //  do_trackeff_others(Finals_above, Tracks, 0.3, genEventWeight, "all");
    //  do_eff_num_others(Finals_above, L3Muons_NoID, 0.1, genEventWeight);
    //  do_eff_num_others(Finals_above, L3Muons, 0.1, genEventWeight);
    //}

    //else{

  //====================================Efficiency vs pt========================================//
 
      do_eff_den_pt(this_gens, genEventWeight); 
      do_trackeff_pt(this_gens, Tracks, 0.3, genEventWeight, "all");
      do_eff_num_pt(this_gens, L3Muons_NoID, 0.1, genEventWeight);
      do_eff_num_pt(this_gens, L3Muons, 0.1, genEventWeight);

      do_eff_den_pt(hardPs_L1matched, genEventWeight); 
      do_trackeff_pt(hardPs_L1matched, Tracks, 0.3, genEventWeight, "all");
      do_eff_num_pt(hardPs_L1matched, L3Muons_NoID, 0.1, genEventWeight);
      do_eff_num_pt(hardPs_L1matched, L3Muons, 0.1, genEventWeight);

      do_eff_den_pt(hardPs_L1matched_med, genEventWeight); 
      do_trackeff_pt(hardPs_L1matched_med, Tracks, 0.3, genEventWeight, "all");
      do_eff_num_pt(hardPs_L1matched_med, L3Muons_NoID, 0.1, genEventWeight);
      do_eff_num_pt(hardPs_L1matched_med, L3Muons, 0.1, genEventWeight);

  //====================================Efficiency vs eta, phi, truePU========================================//

      do_eff_den_others(this_gens_above, genEventWeight); 
      do_trackeff_others(this_gens_above, Tracks, 0.3, genEventWeight, "all");
      do_eff_num_others(this_gens_above, L3Muons_NoID, 0.1, genEventWeight);
      do_eff_num_others(this_gens_above, L3Muons, 0.1, genEventWeight);

      do_eff_den_others(hardPs_above_L1matched, genEventWeight); 
      do_trackeff_others(hardPs_above_L1matched, Tracks, 0.3, genEventWeight, "all");
      do_eff_num_others(hardPs_above_L1matched, L3Muons_NoID, 0.1, genEventWeight);
      do_eff_num_others(hardPs_above_L1matched, L3Muons, 0.1, genEventWeight);

      do_eff_den_others(hardPs_above_L1matched_med, genEventWeight); 
      do_trackeff_others(hardPs_above_L1matched_med, Tracks, 0.3, genEventWeight, "all");
      do_eff_num_others(hardPs_above_L1matched_med, L3Muons_NoID, 0.1, genEventWeight);
      do_eff_num_others(hardPs_above_L1matched_med, L3Muons, 0.1, genEventWeight);

    //}

  //==========================================Purity====================================================//

    do_purity(L3Muons_NoID, Finals, 0.1, genEventWeight);
    do_purity(L3Muons, Finals, 0.1, genEventWeight);
    do_purity(Iter2L3FromL1Tracks, Finals, 0.1, genEventWeight);

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
    



/*
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
*/

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

  //}

}



//TH1 -> GetCumulative (forward = false)
//First, draw Nsignal vs mva(sigmoid), Nbkg vs mva(sigmoid).
//Then calculate GetCumulative(some bin)/GetCumulative(0).
//Iter2 track, Iter2FromL1 track
