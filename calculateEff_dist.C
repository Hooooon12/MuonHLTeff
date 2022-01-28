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

//My object = TLorentzVector + nametag + pileup + L1 matcher info + TP info//
class Object: public TLorentzVector{

private:

  TString j_tag;
  int j_truePU;
  double j_dR;
  double j_l1ptByQ;
  double j_l1etaByQ;
  double j_l1phiByQ;
  double j_l1chargeByQ;
  int j_l1qByQ;
  double j_l1drByQ;
  double j_hltIterL3OIMuonTrackAssociated_bestMatchTrk_pt;
  double j_hltIter0IterL3MuonTrackAssociated_bestMatchTrk_pt;
  double j_hltIter2IterL3MuonTrackAssociated_bestMatchTrk_pt;
  double j_hltIter0IterL3FromL1MuonTrackAssociated_bestMatchTrk_pt;
  double j_hltIter2IterL3FromL1MuonTrackAssociated_bestMatchTrk_pt;
  double j_hltIter2IterL3MuonMergedAssociated_bestMatchTrk_pt;
  double j_hltIter2IterL3FromL1MuonMergedAssociated_bestMatchTrk_pt;
  double j_hltIterL3MuonMergedAssociated_bestMatchTrk_pt;
  double j_hltIterL3MuonAndMuonFromL1MergedAssociated_bestMatchTrk_pt;
  double j_iterL3MuonNoIDTrackAssociated_bestMatchTrk_pt;
  double j_iterL3MuonTrackAssociated_bestMatchTrk_pt;
  double j_quality;
  float j_hltIter2IterL3MuonTrackAssociated_mva;
  float j_hltIter2IterL3FromL1MuonTrackAssociated_mva;
  int j_hltIter2IterL3MuonTrackAssociated_bestMatchTP_status;
  int j_hltIter2IterL3FromL1MuonTrackAssociated_bestMatchTP_status;
  int j_hltIter2IterL3MuonTrackAssociated_bestMatchTP_pdgId;
  int j_hltIter2IterL3FromL1MuonTrackAssociated_bestMatchTP_pdgId;
  float j_hltIter0IterL3MuonTrackAssociated_mva;
  float j_hltIter0IterL3FromL1MuonTrackAssociated_mva;
  int j_hltIter0IterL3MuonTrackAssociated_bestMatchTP_status;
  int j_hltIter0IterL3FromL1MuonTrackAssociated_bestMatchTP_status;
  int j_hltIter0IterL3MuonTrackAssociated_bestMatchTP_pdgId;
  int j_hltIter0IterL3FromL1MuonTrackAssociated_bestMatchTP_pdgId;

public:

//Initialize the variables
  Object(){
    j_tag = "";
    j_truePU = -999;
    j_dR = -999.;
    j_l1ptByQ = -999.;
    j_l1etaByQ = -999.;
    j_l1phiByQ = -999.;
    j_l1chargeByQ = -999.;
    j_l1qByQ = -999;
    j_l1drByQ = -999.;
    j_hltIterL3OIMuonTrackAssociated_bestMatchTrk_pt = -999.;
    j_hltIter0IterL3MuonTrackAssociated_bestMatchTrk_pt = -999.;
    j_hltIter2IterL3MuonTrackAssociated_bestMatchTrk_pt = -999.;
    j_hltIter0IterL3FromL1MuonTrackAssociated_bestMatchTrk_pt = -999.;
    j_hltIter2IterL3FromL1MuonTrackAssociated_bestMatchTrk_pt = -999.;
    j_hltIter2IterL3MuonMergedAssociated_bestMatchTrk_pt = -999.;
    j_hltIter2IterL3FromL1MuonMergedAssociated_bestMatchTrk_pt = -999.;
    j_hltIterL3MuonMergedAssociated_bestMatchTrk_pt = -999.;
    j_hltIterL3MuonAndMuonFromL1MergedAssociated_bestMatchTrk_pt = -999.;
    j_iterL3MuonNoIDTrackAssociated_bestMatchTrk_pt = -999.;
    j_iterL3MuonTrackAssociated_bestMatchTrk_pt = -999.;
    j_quality = -999.;
    j_hltIter2IterL3MuonTrackAssociated_mva = -999.;
    j_hltIter2IterL3FromL1MuonTrackAssociated_mva = -999.;
    j_hltIter2IterL3MuonTrackAssociated_bestMatchTP_status = -999.;
    j_hltIter2IterL3FromL1MuonTrackAssociated_bestMatchTP_status = -999.;
    j_hltIter2IterL3MuonTrackAssociated_bestMatchTP_pdgId = -999.;
    j_hltIter2IterL3FromL1MuonTrackAssociated_bestMatchTP_pdgId = -999.;
    j_hltIter0IterL3MuonTrackAssociated_mva = -999.;
    j_hltIter0IterL3FromL1MuonTrackAssociated_mva = -999.;
    j_hltIter0IterL3MuonTrackAssociated_bestMatchTP_status = -999.;
    j_hltIter0IterL3FromL1MuonTrackAssociated_bestMatchTP_status = -999.;
    j_hltIter0IterL3MuonTrackAssociated_bestMatchTP_pdgId = -999.;
    j_hltIter0IterL3FromL1MuonTrackAssociated_bestMatchTP_pdgId = -999.;
  }

  ~Object() {}

//Set the variables
  void SetTag(TString this_tag){
    j_tag = this_tag;
  }
  void SetTruePU(int this_truePU){
    j_truePU = this_truePU;
  }
  void SetdR(double this_dR){
    j_dR = this_dR;
  }
  void Setl1ptByQ(double this_l1ptByQ){
    j_l1ptByQ = this_l1ptByQ;
  }
  void Setl1etaByQ(double this_l1etaByQ){
    j_l1etaByQ = this_l1etaByQ;
  }
  void Setl1phiByQ(double this_l1phiByQ){
    j_l1phiByQ = this_l1phiByQ;
  }
  void Setl1chargeByQ(double this_l1chargeByQ){
    j_l1chargeByQ = this_l1chargeByQ;
  }
  void Setl1qByQ(int this_l1qByQ){
    j_l1qByQ = this_l1qByQ;
  }
  void Setl1drByQ(double this_l1drByQ){
    j_l1drByQ = this_l1drByQ;
  }
  void SetHltIterL3OIMuonTrackAssociated_bestMatchTrk_pt(double this_pt){
    j_hltIterL3OIMuonTrackAssociated_bestMatchTrk_pt = this_pt;
  }
  void SetHltIter0IterL3MuonTrackAssociated_bestMatchTrk_pt(double this_pt){
    j_hltIter0IterL3MuonTrackAssociated_bestMatchTrk_pt = this_pt;
  }
  void SetHltIter2IterL3MuonTrackAssociated_bestMatchTrk_pt(double this_pt){
    j_hltIter2IterL3MuonTrackAssociated_bestMatchTrk_pt = this_pt;
  }
  void SetHltIter0IterL3FromL1MuonTrackAssociated_bestMatchTrk_pt(double this_pt){
    j_hltIter0IterL3FromL1MuonTrackAssociated_bestMatchTrk_pt = this_pt;
  }
  void SetHltIter2IterL3FromL1MuonTrackAssociated_bestMatchTrk_pt(double this_pt){
    j_hltIter2IterL3FromL1MuonTrackAssociated_bestMatchTrk_pt = this_pt;
  }
  void SetHltIter2IterL3MuonMergedAssociated_bestMatchTrk_pt(double this_pt){
    j_hltIter2IterL3MuonMergedAssociated_bestMatchTrk_pt = this_pt;
  }
  void SetHltIter2IterL3FromL1MuonMergedAssociated_bestMatchTrk_pt(double this_pt){
    j_hltIter2IterL3FromL1MuonMergedAssociated_bestMatchTrk_pt = this_pt;
  }
  void SetHltIterL3MuonMergedAssociated_bestMatchTrk_pt(double this_pt){
    j_hltIterL3MuonMergedAssociated_bestMatchTrk_pt = this_pt;
  }
  void SetHltIterL3MuonAndMuonFromL1MergedAssociated_bestMatchTrk_pt(double this_pt){
    j_hltIterL3MuonAndMuonFromL1MergedAssociated_bestMatchTrk_pt = this_pt;
  }
  void SetIterL3MuonNoIDTrackAssociated_bestMatchTrk_pt(double this_pt){
    j_iterL3MuonNoIDTrackAssociated_bestMatchTrk_pt = this_pt;
  }
  void SetIterL3MuonTrackAssociated_bestMatchTrk_pt(double this_pt){
    j_iterL3MuonTrackAssociated_bestMatchTrk_pt = this_pt;
  }
  void SetQuality(double this_quality){
    j_quality = this_quality;
  }
  void SetHltIter2IterL3MuonTrackAssociated_mva(float this_mva){
    j_hltIter2IterL3MuonTrackAssociated_mva = this_mva;
  }
  void SetHltIter2IterL3FromL1MuonTrackAssociated_mva(float this_mva){
    j_hltIter2IterL3FromL1MuonTrackAssociated_mva = this_mva;
  }
  void SetHltIter2IterL3MuonTrackAssociated_bestMatchTP_status(int this_status){
    j_hltIter2IterL3MuonTrackAssociated_bestMatchTP_status = this_status;
  }
  void SetHltIter2IterL3FromL1MuonTrackAssociated_bestMatchTP_status(int this_status){
    j_hltIter2IterL3FromL1MuonTrackAssociated_bestMatchTP_status = this_status;
  }
  void SetHltIter2IterL3MuonTrackAssociated_bestMatchTP_pdgId(int this_pdgId){
    j_hltIter2IterL3MuonTrackAssociated_bestMatchTP_pdgId = this_pdgId;
  }
  void SetHltIter2IterL3FromL1MuonTrackAssociated_bestMatchTP_pdgId(int this_pdgId){
    j_hltIter2IterL3FromL1MuonTrackAssociated_bestMatchTP_pdgId = this_pdgId;
  }
  void SetHltIter0IterL3MuonTrackAssociated_mva(float this_mva){
    j_hltIter0IterL3MuonTrackAssociated_mva = this_mva;
  }
  void SetHltIter0IterL3FromL1MuonTrackAssociated_mva(float this_mva){
    j_hltIter0IterL3FromL1MuonTrackAssociated_mva = this_mva;
  }
  void SetHltIter0IterL3MuonTrackAssociated_bestMatchTP_status(int this_status){
    j_hltIter0IterL3MuonTrackAssociated_bestMatchTP_status = this_status;
  }
  void SetHltIter0IterL3FromL1MuonTrackAssociated_bestMatchTP_status(int this_status){
    j_hltIter0IterL3FromL1MuonTrackAssociated_bestMatchTP_status = this_status;
  }
  void SetHltIter0IterL3MuonTrackAssociated_bestMatchTP_pdgId(int this_pdgId){
    j_hltIter0IterL3MuonTrackAssociated_bestMatchTP_pdgId = this_pdgId;
  }
  void SetHltIter0IterL3FromL1MuonTrackAssociated_bestMatchTP_pdgId(int this_pdgId){
    j_hltIter0IterL3FromL1MuonTrackAssociated_bestMatchTP_pdgId = this_pdgId;
  }

//Call the variables
  TString Tag(){
    return j_tag;
  }
  int truePU(){
    return j_truePU;
  }
  double dR(){
    return j_dR;
  }
  double l1ptByQ(){
    return j_l1ptByQ;
  }
  double l1etaByQ(){
    return j_l1etaByQ;
  }
  double l1phiByQ(){
    return j_l1phiByQ;
  }
  double l1chargeByQ(){
    return j_l1chargeByQ;
  }
  int l1qByQ(){
    return j_l1qByQ;
  }
  double l1drByQ(){
    return j_l1drByQ;
  }
  double hltIterL3OIMuonTrackAssociated_bestMatchTrk_pt(){
    return j_hltIterL3OIMuonTrackAssociated_bestMatchTrk_pt;
  }
  double hltIter0IterL3MuonTrackAssociated_bestMatchTrk_pt(){
    return j_hltIter0IterL3MuonTrackAssociated_bestMatchTrk_pt;
  }
  double hltIter2IterL3MuonTrackAssociated_bestMatchTrk_pt(){
    return j_hltIter2IterL3MuonTrackAssociated_bestMatchTrk_pt;
  }
  double hltIter0IterL3FromL1MuonTrackAssociated_bestMatchTrk_pt(){
    return j_hltIter0IterL3FromL1MuonTrackAssociated_bestMatchTrk_pt;
  }
  double hltIter2IterL3FromL1MuonTrackAssociated_bestMatchTrk_pt(){
    return j_hltIter2IterL3FromL1MuonTrackAssociated_bestMatchTrk_pt;
  }
  double hltIter2IterL3MuonMergedAssociated_bestMatchTrk_pt(){
    return j_hltIter2IterL3MuonMergedAssociated_bestMatchTrk_pt;
  }
  double hltIter2IterL3FromL1MuonMergedAssociated_bestMatchTrk_pt(){
    return j_hltIter2IterL3FromL1MuonMergedAssociated_bestMatchTrk_pt;
  }
  double hltIterL3MuonMergedAssociated_bestMatchTrk_pt(){
    return j_hltIterL3MuonMergedAssociated_bestMatchTrk_pt;
  }
  double hltIterL3MuonAndMuonFromL1MergedAssociated_bestMatchTrk_pt(){
    return j_hltIterL3MuonAndMuonFromL1MergedAssociated_bestMatchTrk_pt;
  }
  double iterL3MuonNoIDTrackAssociated_bestMatchTrk_pt(){
    return j_iterL3MuonNoIDTrackAssociated_bestMatchTrk_pt;
  }
  double iterL3MuonTrackAssociated_bestMatchTrk_pt(){
    return j_iterL3MuonTrackAssociated_bestMatchTrk_pt;
  }
  double quality(){
    return j_quality;
  }
  float hltIter2IterL3MuonTrackAssociated_mva(){
    return j_hltIter2IterL3MuonTrackAssociated_mva;
  }
  float hltIter2IterL3FromL1MuonTrackAssociated_mva(){
    return j_hltIter2IterL3FromL1MuonTrackAssociated_mva;
  }
  int hltIter2IterL3MuonTrackAssociated_bestMatchTP_status(){
    return j_hltIter2IterL3MuonTrackAssociated_bestMatchTP_status;
  }
  int hltIter2IterL3FromL1MuonTrackAssociated_bestMatchTP_status(){
    return j_hltIter2IterL3FromL1MuonTrackAssociated_bestMatchTP_status;
  }
  int hltIter2IterL3MuonTrackAssociated_bestMatchTP_pdgId(){
    return j_hltIter2IterL3MuonTrackAssociated_bestMatchTP_pdgId;
  }
  int hltIter2IterL3FromL1MuonTrackAssociated_bestMatchTP_pdgId(){
    return j_hltIter2IterL3FromL1MuonTrackAssociated_bestMatchTP_pdgId;
  }
  float hltIter0IterL3MuonTrackAssociated_mva(){
    return j_hltIter0IterL3MuonTrackAssociated_mva;
  }
  float hltIter0IterL3FromL1MuonTrackAssociated_mva(){
    return j_hltIter0IterL3FromL1MuonTrackAssociated_mva;
  }
  int hltIter0IterL3MuonTrackAssociated_bestMatchTP_status(){
    return j_hltIter0IterL3MuonTrackAssociated_bestMatchTP_status;
  }
  int hltIter0IterL3FromL1MuonTrackAssociated_bestMatchTP_status(){
    return j_hltIter0IterL3FromL1MuonTrackAssociated_bestMatchTP_status;
  }
  int hltIter0IterL3MuonTrackAssociated_bestMatchTP_pdgId(){
    return j_hltIter0IterL3MuonTrackAssociated_bestMatchTP_pdgId;
  }
  int hltIter0IterL3FromL1MuonTrackAssociated_bestMatchTP_pdgId(){
    return j_hltIter0IterL3FromL1MuonTrackAssociated_bestMatchTP_pdgId;
  }

};

vector<Object> SelectL1(vector<Object> L1MuonsRaw, double qualcut, int ptcut){

  TString this_tag;
  TString ptcut_t = Form("%d",ptcut);
  if(qualcut == 11) this_tag = "L1SQ"+ptcut_t;
  else if(qualcut == 7) this_tag = "L1DQ"+ptcut_t;
  else this_tag = "L1Raw"+ptcut_t;
  if(L1MuonsRaw.size()!=0){
    if(L1MuonsRaw.at(0).Tag().Contains("AtVtx")) this_tag = this_tag+"_AtVtx";
  }

  vector<Object> L1s_Selected;
  for(int j=0; j<L1MuonsRaw.size(); j++){
    Object L1_tmp = L1MuonsRaw.at(j);
    if(L1MuonsRaw.at(j).quality() > qualcut && L1MuonsRaw.at(j).Pt() > ptcut){
      L1_tmp.SetPtEtaPhiM(L1MuonsRaw.at(j).Pt(), L1MuonsRaw.at(j).Eta(), L1MuonsRaw.at(j).Phi(), 0);
      L1_tmp.SetTag(this_tag);
      L1_tmp.SetQuality(L1MuonsRaw.at(j).quality());
      L1s_Selected.push_back(L1_tmp);
    }
  }

  return L1s_Selected;

}

vector<Object> SelectL1Matched(vector<Object> hardPs, double dRcut, double qualcut, int ptcut){

  TString this_tag;
  TString ptcut_t = Form("%d",ptcut);
  if(qualcut == 11) this_tag = "L1Matched_SQ"+ptcut_t;
  else if(qualcut == 7) this_tag = "L1Matched_DQ"+ptcut_t;
  else this_tag = "L1Matched_Raw"+ptcut_t;

  vector<Object> L1s_Matched;
  for(int j=0; j<hardPs.size(); j++){
    if(-1<hardPs.at(j).l1ptByQ()&&hardPs.at(j).l1drByQ()<dRcut){
      if(hardPs.at(j).l1qByQ() > qualcut && hardPs.at(j).l1ptByQ() > ptcut){
        Object L1_tmp;
        L1_tmp.SetPtEtaPhiM(hardPs.at(j).l1ptByQ(), hardPs.at(j).l1etaByQ(), hardPs.at(j).l1phiByQ(), 0);
        L1_tmp.SetTag(this_tag);
        L1_tmp.SetQuality(hardPs.at(j).l1qByQ());
        //if(L1_tmp.Tag().Contains("Raw0")){
        //  cout << j << "th gen muon;" << endl;
        //  cout << hardPs.at(j).Tag() << " pt, eta, phi : " << hardPs.at(j).Pt() << ", " << hardPs.at(j).Eta() << ", " << hardPs.at(j).Phi() << endl;
        //  cout << L1_tmp.Tag() << " pt, eta, phi : " << hardPs.at(j).l1ptByQ() << ", " << hardPs.at(j).l1etaByQ() << ", " << hardPs.at(j).l1phiByQ() << endl;
        //}
        L1s_Matched.push_back(L1_tmp);
      }
    }
  }

  return L1s_Matched;

}

vector<Object> MatchTPtoL1(vector<Object> TPs, vector<Object> hardPs, double qualcut, int ptcut, double dRcut){ // why ptcut int? to get nice ptcut_t.

  TString this_tag;
  TString ptcut_t = Form("%d",ptcut);
  if(qualcut == 11) this_tag = "TP_L1SQ"+ptcut_t;
  else if(qualcut == 7) this_tag = "TP_L1DQ"+ptcut_t;
  else this_tag = "TP_L1Raw"+ptcut_t;

  vector<Object> TPs_L1matched;
  for(int j=0; j<TPs.size(); j++){
    Object TP_L1matched = TPs.at(j);
    for(int k=0; k<hardPs.size(); k++){
      if( TPs.at(j).Pt() == hardPs.at(k).Pt() && TPs.at(j).Eta() == hardPs.at(k).Eta() && TPs.at(j).Phi() == hardPs.at(k).Phi() ){
        if(hardPs.at(k).l1qByQ() > qualcut && hardPs.at(k).l1ptByQ() > ptcut && fabs(hardPs.at(k).l1drByQ()) < dRcut){
          TP_L1matched.SetTag(this_tag);
          TPs_L1matched.push_back(TP_L1matched);
        }
      }
    }
  }

  return TPs_L1matched;

}


bool PtCompare(TLorentzVector p1, TLorentzVector p2){ return (p1.Pt() > p2.Pt()); }

void check_Meson_dR(vector<Object> gens, double weight){

  if(gens.size() != 2){
    return;
  }

  sort(gens.begin(), gens.end(), PtCompare);

  FillHist(gens.at(0).Tag()+"_pt1_dR_"+gens.at(0).Tag()+"_2D",gens.at(0).Pt(),gens.at(0).DeltaR(gens.at(1)),weight,12000,0,6000,1000,0,10);
  FillHist(gens.at(0).Tag()+"_pt2_dR_"+gens.at(0).Tag()+"_2D",gens.at(1).Pt(),gens.at(0).DeltaR(gens.at(1)),weight,12000,0,6000,1000,0,10);

}


void draw_dR(vector<Object> gens, vector<Object> recos, double weight){

  //cout << "Nrecos : " << recos.size() << endl;
  if(recos.size() == 0){
    return;
  }

  vector<int> this_matched;

  for(int j=0; j<gens.size(); j++){
    //cout << "for " << j << "th " << gens.at(j).Tag() << ":" << endl;
    double tmpDeltaR = 999.;
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
      FillHist(gens.at(j).Tag()+"_pt_dR_"+recos.at(Matched).Tag()+"_2D",gens.at(j).Pt(),gens.at(j).DeltaR(recos.at(Matched)),weight,12000,0,6000,1000,0,10);
      FillHist(gens.at(j).Tag()+"_pt_ptres_"+recos.at(Matched).Tag()+"_2D",gens.at(j).Pt(),(recos.at(Matched).Pt()-gens.at(j).Pt())/gens.at(j).Pt(),weight,12000,0,6000,10000,0,10);
      FillHist(gens.at(j).Tag()+"_ptres_dR_"+recos.at(Matched).Tag()+"_2D",(recos.at(Matched).Pt()-gens.at(j).Pt())/gens.at(j).Pt(),gens.at(j).DeltaR(recos.at(Matched)),weight,10000,0,10,1000,0,10);
      FillProf(gens.at(j).Tag()+"_pt_dR_"+recos.at(Matched).Tag()+"_pf",gens.at(j).Pt(),gens.at(j).DeltaR(recos.at(Matched)),weight,12000,0,6000,0,10);
      this_matched.push_back(Matched);
    }
  }

}

void draw_l1drByQ(vector<Object> gens, double qualcut, int ptcut, double weight){

  TString this_tag;
  TString ptcut_t = Form("%d",ptcut);
  if(qualcut == 11) this_tag = "L1Matched_l1drByQ_SQ"+ptcut_t;
  else if(qualcut == 7) this_tag = "L1Matched_l1drByQ_DQ"+ptcut_t;
  else this_tag = "L1Matched_l1drByQ_Raw"+ptcut_t;

  for(int j=0; j<gens.size(); j++){
    if(-1<gens.at(j).l1ptByQ()){
      FillHist("hardP_pt_dR_"+this_tag+"_2D",gens.at(j).Pt(),gens.at(j).l1drByQ(),weight,12000,0,6000,1000,0,10);
      FillHist("hardP_pt_ptres_"+this_tag+"_2D",gens.at(j).Pt(),(gens.at(j).l1ptByQ()-gens.at(j).Pt())/gens.at(j).Pt(),weight,12000,0,6000,10000,0,10);
      FillHist("hardP_ptres_dR_"+this_tag+"_2D",(gens.at(j).l1ptByQ()-gens.at(j).Pt())/gens.at(j).Pt(),gens.at(j).l1drByQ(),weight,10000,0,10,1000,0,10);
    }
  }

}


void do_eff_den_pt(vector<Object> gens, double weight){

  for(int j=0; j<gens.size(); j++){
    FillHist(gens.at(j).Tag()+"_pt",gens.at(j).Pt(),weight,12000,0,6000);
  }

}

void do_eff_den_others(vector<Object> gens_above, double weight){

  for(int j=0; j<gens_above.size(); j++){
    FillHist(gens_above.at(j).Tag()+"_eta",gens_above.at(j).Eta(),weight,48,-2.4,2.4);
    FillHist(gens_above.at(j).Tag()+"_phi",gens_above.at(j).Phi(),weight,63,-3.15,3.15);
    FillHist(gens_above.at(j).Tag()+"_truePU",gens_above.at(j).truePU(),weight,100,0,100);
  }

}

void do_neweff_pt(vector<Object> TPs, double weight){

  for(int j=0; j<TPs.size(); j++){
    FillHist(TPs.at(j).Tag()+"_pt",TPs.at(j).Pt(),weight,12000,0,6000);
    if(TPs.at(j).hltIterL3OIMuonTrackAssociated_bestMatchTrk_pt() > -1){
      FillHist(TPs.at(j).Tag()+"_pt_eff_TPtoOI",TPs.at(j).Pt(),weight,12000,0,6000);
    }
    if(TPs.at(j).hltIter0IterL3MuonTrackAssociated_bestMatchTrk_pt() > -1){
      FillHist(TPs.at(j).Tag()+"_pt_eff_TPtoIter0FromL2",TPs.at(j).Pt(),weight,12000,0,6000);
    }
    if(TPs.at(j).hltIter2IterL3MuonTrackAssociated_bestMatchTrk_pt() > -1){
      FillHist(TPs.at(j).Tag()+"_pt_eff_TPtoIter2FromL2",TPs.at(j).Pt(),weight,12000,0,6000);
    }
    if(TPs.at(j).hltIter0IterL3FromL1MuonTrackAssociated_bestMatchTrk_pt() > -1){
      FillHist(TPs.at(j).Tag()+"_pt_eff_TPtoIter0FromL1",TPs.at(j).Pt(),weight,12000,0,6000);
    }
    if(TPs.at(j).hltIter2IterL3FromL1MuonTrackAssociated_bestMatchTrk_pt() > -1){
      FillHist(TPs.at(j).Tag()+"_pt_eff_TPtoIter2FromL1",TPs.at(j).Pt(),weight,12000,0,6000);
    }
    //if(TPs.at(j).hltIter2IterL3MuonMergedAssociated_bestMatchTrk_pt() > -1){
    //  FillHist(TPs.at(j).Tag()+"_pt_eff_TPtoIOFromL2",TPs.at(j).Pt(),weight,12000,0,6000);
    //}
    //if(TPs.at(j).hltIter2IterL3FromL1MuonMergedAssociated_bestMatchTrk_pt() > -1){
    //  FillHist(TPs.at(j).Tag()+"_pt_eff_TPtoIOFromL1",TPs.at(j).Pt(),weight,12000,0,6000);
    //} //JH : deprecated?
    if(TPs.at(j).hltIterL3MuonMergedAssociated_bestMatchTrk_pt() > -1){
      FillHist(TPs.at(j).Tag()+"_pt_eff_TPtoUpToFromL2",TPs.at(j).Pt(),weight,12000,0,6000);
    }
    if(TPs.at(j).hltIterL3MuonAndMuonFromL1MergedAssociated_bestMatchTrk_pt() > -1){
      FillHist(TPs.at(j).Tag()+"_pt_eff_TPtoUpToFromL1",TPs.at(j).Pt(),weight,12000,0,6000);
    }
    if(TPs.at(j).iterL3MuonNoIDTrackAssociated_bestMatchTrk_pt() > -1){
      FillHist(TPs.at(j).Tag()+"_pt_eff_TPtoL3NoIDTrack",TPs.at(j).Pt(),weight,12000,0,6000);
    }
    if(TPs.at(j).iterL3MuonTrackAssociated_bestMatchTrk_pt() > -1){
      FillHist(TPs.at(j).Tag()+"_pt_eff_TPtoL3Track",TPs.at(j).Pt(),weight,12000,0,6000);
    }

  }

}

void do_neweff_others(vector<Object> TPs, double turn_on, double weight){

  for(int j=0; j<TPs.size(); j++){
    if( TPs.at(j).Pt() > turn_on ){
      if(turn_on == 10.) TPs.at(j).SetTag(TPs.at(j).Tag()+"_med");
      FillHist(TPs.at(j).Tag()+"_eta",TPs.at(j).Eta(),weight,48,-2.4,2.4);
      FillHist(TPs.at(j).Tag()+"_phi",TPs.at(j).Phi(),weight,63,-3.15,3.15);
      FillHist(TPs.at(j).Tag()+"_truePU",TPs.at(j).truePU(),weight,100,0,100);
      if(TPs.at(j).hltIterL3OIMuonTrackAssociated_bestMatchTrk_pt() > -1){
        FillHist(TPs.at(j).Tag()+"_eta_eff_TPtoOI",TPs.at(j).Eta(),weight,48,-2.4,2.4);
        FillHist(TPs.at(j).Tag()+"_phi_eff_TPtoOI",TPs.at(j).Phi(),weight,63,-3.15,3.15);
        FillHist(TPs.at(j).Tag()+"_truePU_eff_TPtoOI",TPs.at(j).truePU(),weight,100,0,100);
      }
      if(TPs.at(j).hltIter0IterL3MuonTrackAssociated_bestMatchTrk_pt() > -1){
        FillHist(TPs.at(j).Tag()+"_eta_eff_TPtoIter0FromL2",TPs.at(j).Eta(),weight,48,-2.4,2.4);
        FillHist(TPs.at(j).Tag()+"_phi_eff_TPtoIter0FromL2",TPs.at(j).Phi(),weight,63,-3.15,3.15);
        FillHist(TPs.at(j).Tag()+"_truePU_eff_TPtoIter0FromL2",TPs.at(j).truePU(),weight,100,0,100);
      }
      if(TPs.at(j).hltIter2IterL3MuonTrackAssociated_bestMatchTrk_pt() > -1){
        FillHist(TPs.at(j).Tag()+"_eta_eff_TPtoIter2FromL2",TPs.at(j).Eta(),weight,48,-2.4,2.4);
        FillHist(TPs.at(j).Tag()+"_phi_eff_TPtoIter2FromL2",TPs.at(j).Phi(),weight,63,-3.15,3.15);
        FillHist(TPs.at(j).Tag()+"_truePU_eff_TPtoIter2FromL2",TPs.at(j).truePU(),weight,100,0,100);
      }
      if(TPs.at(j).hltIter0IterL3FromL1MuonTrackAssociated_bestMatchTrk_pt() > -1){
        FillHist(TPs.at(j).Tag()+"_eta_eff_TPtoIter0FromL1",TPs.at(j).Eta(),weight,48,-2.4,2.4);
        FillHist(TPs.at(j).Tag()+"_phi_eff_TPtoIter0FromL1",TPs.at(j).Phi(),weight,63,-3.15,3.15);
        FillHist(TPs.at(j).Tag()+"_truePU_eff_TPtoIter0FromL1",TPs.at(j).truePU(),weight,100,0,100);
      }
      if(TPs.at(j).hltIter2IterL3FromL1MuonTrackAssociated_bestMatchTrk_pt() > -1){
        FillHist(TPs.at(j).Tag()+"_eta_eff_TPtoIter2FromL1",TPs.at(j).Eta(),weight,48,-2.4,2.4);
        FillHist(TPs.at(j).Tag()+"_phi_eff_TPtoIter2FromL1",TPs.at(j).Phi(),weight,63,-3.15,3.15);
        FillHist(TPs.at(j).Tag()+"_truePU_eff_TPtoIter2FromL1",TPs.at(j).truePU(),weight,100,0,100);
      }
      //if(TPs.at(j).hltIter2IterL3MuonMergedAssociated_bestMatchTrk_pt() > -1){
      //  FillHist(TPs.at(j).Tag()+"_eta_eff_TPtoIOFromL2",TPs.at(j).Eta(),weight,48,-2.4,2.4);
      //  FillHist(TPs.at(j).Tag()+"_phi_eff_TPtoIOFromL2",TPs.at(j).Phi(),weight,63,-3.15,3.15);
      //  FillHist(TPs.at(j).Tag()+"_truePU_eff_TPtoIOFromL2",TPs.at(j).truePU(),weight,100,0,100);
      //}
      //if(TPs.at(j).hltIter2IterL3FromL1MuonMergedAssociated_bestMatchTrk_pt() > -1){
      //  FillHist(TPs.at(j).Tag()+"_eta_eff_TPtoIOFromL1",TPs.at(j).Eta(),weight,48,-2.4,2.4);
      //  FillHist(TPs.at(j).Tag()+"_phi_eff_TPtoIOFromL1",TPs.at(j).Phi(),weight,63,-3.15,3.15);
      //  FillHist(TPs.at(j).Tag()+"_truePU_eff_TPtoIOFromL1",TPs.at(j).truePU(),weight,100,0,100);
      //} //JH : deprecated?
      if(TPs.at(j).hltIterL3MuonMergedAssociated_bestMatchTrk_pt() > -1){
        FillHist(TPs.at(j).Tag()+"_eta_eff_TPtoUpToFromL2",TPs.at(j).Eta(),weight,48,-2.4,2.4);
        FillHist(TPs.at(j).Tag()+"_phi_eff_TPtoUpToFromL2",TPs.at(j).Phi(),weight,63,-3.15,3.15);
        FillHist(TPs.at(j).Tag()+"_truePU_eff_TPtoUpToFromL2",TPs.at(j).truePU(),weight,100,0,100);
      }
      if(TPs.at(j).hltIterL3MuonAndMuonFromL1MergedAssociated_bestMatchTrk_pt() > -1){
        FillHist(TPs.at(j).Tag()+"_eta_eff_TPtoUpToFromL1",TPs.at(j).Eta(),weight,48,-2.4,2.4);
        FillHist(TPs.at(j).Tag()+"_phi_eff_TPtoUpToFromL1",TPs.at(j).Phi(),weight,63,-3.15,3.15);
        FillHist(TPs.at(j).Tag()+"_truePU_eff_TPtoUpToFromL1",TPs.at(j).truePU(),weight,100,0,100);
      }
      if(TPs.at(j).iterL3MuonNoIDTrackAssociated_bestMatchTrk_pt() > -1){
        FillHist(TPs.at(j).Tag()+"_eta_eff_TPtoL3NoIDTrack",TPs.at(j).Eta(),weight,48,-2.4,2.4);
        FillHist(TPs.at(j).Tag()+"_phi_eff_TPtoL3NoIDTrack",TPs.at(j).Phi(),weight,63,-3.15,3.15);
        FillHist(TPs.at(j).Tag()+"_truePU_eff_TPtoL3NoIDTrack",TPs.at(j).truePU(),weight,100,0,100);
      }
      if(TPs.at(j).iterL3MuonTrackAssociated_bestMatchTrk_pt() > -1){
        FillHist(TPs.at(j).Tag()+"_eta_eff_TPtoL3Track",TPs.at(j).Eta(),weight,48,-2.4,2.4);
        FillHist(TPs.at(j).Tag()+"_phi_eff_TPtoL3Track",TPs.at(j).Phi(),weight,63,-3.15,3.15);
        FillHist(TPs.at(j).Tag()+"_truePU_eff_TPtoL3Track",TPs.at(j).truePU(),weight,100,0,100);
      }
    }
  }

}

void do_neweff_L1MuonsMatched_pt(vector<Object> hardPs, double dRcut, double qualcut, int ptcut, double weight){

  TString this_tag;
  TString ptcut_t = Form("%d",ptcut);
  if(qualcut == 11) this_tag = "L1Matched_l1drByQ_SQ"+ptcut_t;
  else if(qualcut == 7) this_tag = "L1Matched_l1drByQ_DQ"+ptcut_t;
  else this_tag = "L1Matched_l1drByQ_Raw"+ptcut_t;

  for(int j=0; j<hardPs.size(); j++){
    if(-1<hardPs.at(j).l1ptByQ()&&hardPs.at(j).l1drByQ()<dRcut){
      if(hardPs.at(j).l1qByQ() > qualcut && hardPs.at(j).l1ptByQ() > ptcut){
        FillHist(hardPs.at(j).Tag()+"_pt_eff_"+this_tag,hardPs.at(j).Pt(),weight,12000,0,6000);
      }
    }
  }

}

void do_neweff_L1MuonsMatched_others(vector<Object> hardPs, double dRcut, double qualcut, int ptcut, double turn_on, double weight){

  TString this_tag;
  TString ptcut_t = Form("%d",ptcut);
  if(qualcut == 11) this_tag = "L1Matched_l1drByQ_SQ"+ptcut_t;
  else if(qualcut == 7) this_tag = "L1Matched_l1drByQ_DQ"+ptcut_t;
  else this_tag = "L1Matched_l1drByQ_Raw"+ptcut_t;

  for(int j=0; j<hardPs.size(); j++){
    if(-1<hardPs.at(j).l1ptByQ()&&hardPs.at(j).l1drByQ()<dRcut){
      if(hardPs.at(j).l1qByQ() > qualcut && hardPs.at(j).l1ptByQ() > ptcut){
        if( hardPs.at(j).Pt() > turn_on ){
          if(turn_on == 10.) hardPs.at(j).SetTag(hardPs.at(j).Tag()+"_med");
          FillHist(hardPs.at(j).Tag()+"_eta_eff_"+this_tag,hardPs.at(j).Eta(),weight,48,-2.4,2.4);
          FillHist(hardPs.at(j).Tag()+"_phi_eff_"+this_tag,hardPs.at(j).Phi(),weight,63,-3.15,3.15);
          FillHist(hardPs.at(j).Tag()+"_truePU_eff_"+this_tag,hardPs.at(j).truePU(),weight,100,0,100);
        }
      }
    }
  }

}

void do_trackeff_pt(vector<Object> gens, vector< vector<Object> > Tracks, double dR, double weight, TString step){

  if(step == "all"){

    for(int j=0; j<gens.size(); j++){
      int matched = 0;
      for(int k=0; k<Tracks[0].size(); k++){
        if(gens.at(j).DeltaR(Tracks[0].at(k)) < dR){
          matched = 1;
          FillHist(gens.at(j).Tag()+"_pt_trackeff_step1",gens.at(j).Pt(),weight,12000,0,6000);
          FillHist(gens.at(j).Tag()+"_pt_trackeff_step2",gens.at(j).Pt(),weight,12000,0,6000);
          FillHist(gens.at(j).Tag()+"_pt_trackeff_step3",gens.at(j).Pt(),weight,12000,0,6000);
          FillHist(gens.at(j).Tag()+"_pt_trackeff_step4",gens.at(j).Pt(),weight,12000,0,6000);
          FillHist(gens.at(j).Tag()+"_pt_trackeff_step5",gens.at(j).Pt(),weight,12000,0,6000);
          break;
        }
      }
      if(matched == 0){
        for(int k=0; k<Tracks[1].size(); k++){
          if(gens.at(j).DeltaR(Tracks[1].at(k)) < dR){
            matched = 1;
            FillHist(gens.at(j).Tag()+"_pt_trackeff_step2",gens.at(j).Pt(),weight,12000,0,6000);
            FillHist(gens.at(j).Tag()+"_pt_trackeff_step3",gens.at(j).Pt(),weight,12000,0,6000);
            FillHist(gens.at(j).Tag()+"_pt_trackeff_step4",gens.at(j).Pt(),weight,12000,0,6000);
            FillHist(gens.at(j).Tag()+"_pt_trackeff_step5",gens.at(j).Pt(),weight,12000,0,6000);
            break;
          }
        }
      }
      if(matched == 0){
        for(int k=0; k<Tracks[2].size(); k++){
          if(gens.at(j).DeltaR(Tracks[2].at(k)) < dR){
            matched = 1;
            FillHist(gens.at(j).Tag()+"_pt_trackeff_step3",gens.at(j).Pt(),weight,12000,0,6000);
            FillHist(gens.at(j).Tag()+"_pt_trackeff_step4",gens.at(j).Pt(),weight,12000,0,6000);
            FillHist(gens.at(j).Tag()+"_pt_trackeff_step5",gens.at(j).Pt(),weight,12000,0,6000);
            break;
          }
        }
      }
      if(matched == 0){
        for(int k=0; k<Tracks[3].size(); k++){
          if(gens.at(j).DeltaR(Tracks[3].at(k)) < dR){
            matched = 1;
            FillHist(gens.at(j).Tag()+"_pt_trackeff_step4",gens.at(j).Pt(),weight,12000,0,6000);
            FillHist(gens.at(j).Tag()+"_pt_trackeff_step5",gens.at(j).Pt(),weight,12000,0,6000);
            break;
          }
        }
      }
      if(matched == 0){
        for(int k=0; k<Tracks[4].size(); k++){
          if(gens.at(j).DeltaR(Tracks[4].at(k)) < dR){
            matched = 1;
            FillHist(gens.at(j).Tag()+"_pt_trackeff_step5",gens.at(j).Pt(),weight,12000,0,6000);
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
              FillHist(gens.at(j).Tag()+"_pt_eff_"+Tracks[this_step].at(k).Tag(),gens.at(j).Pt(),weight,12000,0,6000);
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
            FillHist(gens.at(j).Tag()+"_pt_eff_"+Tracks[this_step].at(k).Tag(),gens.at(j).Pt(),weight,12000,0,6000);
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
          //if(recos.at(k).Tag().Contains("Raw0")){
          //  cout << j << "th gen muon matched with " << k << "th " << recos.at(k).Tag() << ";" << endl;
          //  cout << "gen pt, eta, phi : " << gens.at(j).Pt() << ", " << gens.at(j).Eta() << ", " << gens.at(j).Phi() << endl;
          //  cout << "L1 pt, eta phi : " << recos.at(k).Pt() << ", " << recos.at(k).Eta() << ", " << recos.at(k).Phi() << endl;
          //}
          //Nmatched_hardP_L3NoID_pt+=1;
          FillHist(gens.at(j).Tag()+"_pt_eff_"+recos.at(k).Tag(),gens.at(j).Pt(),weight,12000,0,6000);
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
    FillHist(recos.at(j).Tag()+"_pt",recos.at(j).Pt(),weight,12000,0,6000);
    FillHist(recos.at(j).Tag()+"_eta",recos.at(j).Eta(),weight,48,-2.4,2.4);
    FillHist(recos.at(j).Tag()+"_phi",recos.at(j).Phi(),weight,63,-3.15,3.15);
    FillHist(recos.at(j).Tag()+"_truePU",recos.at(j).truePU(),weight,100,0,100);
    for(int k=0; k<Finals.size(); k++){
      if( find(this_matched.begin(), this_matched.end(), k) == this_matched.end() ){
        if(recos.at(j).DeltaR(Finals.at(k)) < dR){
          //Nmatched_L3NoID_Final+=1;
          FillHist(recos.at(j).Tag()+"_pt_purity",recos.at(j).Pt(),weight,12000,0,6000);
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
    FillHist(recos.at(j).Tag()+"_pt",recos.at(j).Pt(),weight,12000,0,6000);
    FillHist(recos.at(j).Tag()+"_eta",recos.at(j).Eta(),weight,48,-2.4,2.4);
    FillHist(recos.at(j).Tag()+"_phi",recos.at(j).Phi(),weight,63,-3.15,3.15);
    FillHist(recos.at(j).Tag()+"_truePU",recos.at(j).truePU(),weight,100,0,100);
    for(int k=0; k<Finals.size(); k++){
      if( find(this_matched.begin(), this_matched.end(), k) == this_matched.end() ){
        if(recos.at(j).DeltaR(Finals.at(k)) < dR){
          //Nmatched_L3NoID_Final+=1;
          FillHist(recos.at(j).Tag()+"_pt_purity",recos.at(j).Pt(),weight,12000,0,6000);
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
    FillHist(recos.at(j).Tag()+"_pt_Asso",recos_Asso.at(j).Pt(),weight,12000,0,6000);
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

  int runNum;
  int lumiBlockNum;
  unsigned long long eventNum;
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
  double genParticle_l1ptByQ[5000];
  double genParticle_l1etaByQ[5000];
  double genParticle_l1phiByQ[5000];
  double genParticle_l1chargeByQ[5000];
  int genParticle_l1qByQ[5000];
  double genParticle_l1drByQ[5000];
  int nL1Muon;
  double L1Muon_pt[5000];
  double L1Muon_eta[5000];
  double L1Muon_phi[5000];
  double L1Muon_etaAtVtx[5000];
  double L1Muon_phiAtVtx[5000];
  double L1Muon_charge[5000];
  double L1Muon_quality[5000];
  int nL2Muon;
  double L2Muon_pt[5000];
  double L2Muon_eta[5000];
  double L2Muon_phi[5000];
  double L2Muon_charge[5000];
  double L2Muon_trkPt[5000];
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
  vector<float> *hltIter0IterL3MuonTrackAssociated_mva = 0;
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
  vector<float> *hltIter2IterL3MuonTrackAssociated_mva = 0;
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
  vector<float> *hltIter0IterL3FromL1MuonTrackAssociated_mva = 0;
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
  vector<float> *hltIter2IterL3FromL1MuonTrackAssociated_mva = 0;
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
  
  fChain->SetBranchAddress("runNum",&runNum);
  fChain->SetBranchAddress("lumiBlockNum",&lumiBlockNum);
  fChain->SetBranchAddress("eventNum",&eventNum);
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
  fChain->SetBranchAddress("genParticle_l1ptByQ",&genParticle_l1ptByQ);
  fChain->SetBranchAddress("genParticle_l1etaByQ",&genParticle_l1etaByQ);
  fChain->SetBranchAddress("genParticle_l1phiByQ",&genParticle_l1phiByQ);
  fChain->SetBranchAddress("genParticle_l1chargeByQ",&genParticle_l1chargeByQ);
  fChain->SetBranchAddress("genParticle_l1qByQ",&genParticle_l1qByQ);
  fChain->SetBranchAddress("genParticle_l1drByQ",&genParticle_l1drByQ);
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
  fChain->SetBranchAddress("hltIter0IterL3MuonTrackAssociated_mva",&hltIter0IterL3MuonTrackAssociated_mva);
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
  fChain->SetBranchAddress("hltIter2IterL3MuonTrackAssociated_mva",&hltIter2IterL3MuonTrackAssociated_mva);
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
  fChain->SetBranchAddress("hltIter0IterL3FromL1MuonTrackAssociated_mva",&hltIter0IterL3FromL1MuonTrackAssociated_mva);
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
  fChain->SetBranchAddress("hltIter2IterL3FromL1MuonTrackAssociated_mva",&hltIter2IterL3FromL1MuonTrackAssociated_mva);
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
  int NTP = 0;
  int NTP_L1 = 0;
  int NTP_L1_med = 0;
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
  TProfile *pf_track_below8 = new TProfile("pf_track_below8","pf_track_below8",25,30,80,0,5000);
  TProfile *pf_track_above8 = new TProfile("pf_track_above8","pf_track_above8",25,30,80,0,5000);
  TProfile *pf_trackFromL1 = new TProfile("pf_trackFromL1","pf_trackFromL1",25,30,80,0,5000);
  TProfile *pf_trackFromL1_below8 = new TProfile("pf_trackFromL1_below8","pf_trackFromL1_below8",25,30,80,0,5000);
  TProfile *pf_trackFromL1_above8 = new TProfile("pf_trackFromL1_above8","pf_trackFromL1_above8",25,30,80,0,5000);

  //To avoid double counting//
  vector<int> this_matched;

  for(int i=0; i<Nevents; i++){
    //cout << "==========" << i << "th event==========" << endl;
    fChain->GetEntry(i);
    //cout << "GetEntry(" << i << ");" << endl;
    //int this_NhardP = 0;
  
  //===============================Call needed objects=======================================//
 
    //==pick index of final state muons with eta < 2.4==//
    vector<int> Final_idx;
    for(int j=0; j<nGenParticle; j++){
      if(fabs(genParticle_ID[j])==13&&genParticle_status[j]==1&&fabs(genParticle_eta[j])<2.4){
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

    //==pick index of hardP muons with eta < 2.4==//
    vector<int> hardP_idx;
    for(int j=0; j<nGenParticle; j++){
      if(fabs(genParticle_ID[j])==13&&genParticle_fromHardProcessFinalState[j]==1&&fabs(genParticle_eta[j])<2.4){
        hardP_idx.push_back(j);
      }
    }
    if(hardP_idx.size()==0){ //second try
      for(int j=0; j<nGenParticle; j++){
        if(fabs(genParticle_ID[j])==13&&genParticle_isPromptFinalState[j]==1&&fabs(genParticle_eta[j])<2.4){
          hardP_idx.push_back(j);
        }
      }
    }
  
    //==make hardP muon's 4vectors==//
    vector<Object> hardPs;
    vector<Object> hardPs_above; // hardP muons above single muon trigger turn-on (26 GeV)
    vector<Object> hardPs_above_med; // hardP muons above double muon trigger turn-on (10 GeV)
    double turn_on = 26.;
    double turn_on_med = 10.;

    if(output.Contains("Jpsi")||output.Contains("Bs")){
      double MesonMass;
      double MesonWidth = 0.1;
      if(output.Contains("Jpsi")){
        MesonMass = 3.097;
      }
       else if(output.Contains("Bs")){
        MesonMass = 5.366;
      }
      Object Lep1, Lep2;
      double tmpMassDiff = 10000.;
      int i1 = 0, i2 = 0;
      if(Final_idx.size() > 1){
        for(int j=0; j<Final_idx.size(); j++){
          for(int k=j+1; k<Final_idx.size(); k++){
            Object tmpLep1, tmpLep2;
            tmpLep1.SetPtEtaPhiM(genParticle_pt[Final_idx.at(j)],genParticle_eta[Final_idx.at(j)],genParticle_phi[Final_idx.at(j)],0);
            tmpLep2.SetPtEtaPhiM(genParticle_pt[Final_idx.at(k)],genParticle_eta[Final_idx.at(k)],genParticle_phi[Final_idx.at(k)],0);
            if( fabs( (tmpLep1+tmpLep2).M() - MesonMass ) < tmpMassDiff ){
              tmpMassDiff = fabs( (tmpLep1+tmpLep2).M() - MesonMass );
              Lep1 = tmpLep1; Lep2 = tmpLep2;
              i1 = Final_idx.at(j); i2 = Final_idx.at(k);
            }
          }
        }
        if( (MesonMass-MesonWidth) < (Lep1+Lep2).M() && (Lep1+Lep2).M() < (MesonMass+MesonWidth) ){
          Lep1.SetTag("hardP");
          Lep1.SetTruePU(truePU);
          Lep1.Setl1ptByQ(genParticle_l1ptByQ[i1]);
          Lep1.Setl1etaByQ(genParticle_l1etaByQ[i1]);
          Lep1.Setl1phiByQ(genParticle_l1phiByQ[i1]);
          Lep1.Setl1chargeByQ(genParticle_l1chargeByQ[i1]);
          Lep1.Setl1qByQ(genParticle_l1qByQ[i1]);
          Lep1.Setl1drByQ(genParticle_l1drByQ[i1]);
          Lep2.SetTag("hardP");
          Lep2.SetTruePU(truePU);
          Lep2.Setl1ptByQ(genParticle_l1ptByQ[i2]);
          Lep2.Setl1etaByQ(genParticle_l1etaByQ[i2]);
          Lep2.Setl1phiByQ(genParticle_l1phiByQ[i2]);
          Lep2.Setl1chargeByQ(genParticle_l1chargeByQ[i2]);
          Lep2.Setl1qByQ(genParticle_l1qByQ[i2]);
          Lep2.Setl1drByQ(genParticle_l1drByQ[i2]);
          hardPs.push_back(Lep1);
          hardPs.push_back(Lep2);
          NhardP+=2;

          if(Lep1.Pt() > turn_on){
            hardPs_above.push_back(Lep1);
            NhardP_above+=1;
          }
          if(Lep1.Pt() > turn_on_med){
            Lep1.SetTag("hardP_med");
            hardPs_above_med.push_back(Lep1);
          }
          if(Lep2.Pt() > turn_on){
            hardPs_above.push_back(Lep2);
            NhardP_above+=1;
          }
          if(Lep2.Pt() > turn_on_med){
            Lep2.SetTag("hardP_med");
            hardPs_above_med.push_back(Lep2);
          }
        }
      }
      //Sanity check//
      //if(hardP.size() > 0) cout << "N of hardP : " << hardP.size() << ", Meson mass : " << (hardP[0]+hardP[1]).M() << ", pt : " << hardP[0].Pt() << ", " << hardP[1].Pt() << endl;
      check_Meson_dR(hardPs, genEventWeight);
    }
    else if(output.Contains("QCD")){
      hardPs = Finals;
      hardPs_above = Finals_above;
    }
    else{
      for(int j=0; j<hardP_idx.size(); j++){
        Object hardP;
        hardP.SetPtEtaPhiM(genParticle_pt[hardP_idx.at(j)],genParticle_eta[hardP_idx.at(j)],genParticle_phi[hardP_idx.at(j)],0);
        hardP.SetTag("hardP");
        hardP.SetTruePU(truePU);
        hardP.Setl1ptByQ(genParticle_l1ptByQ[hardP_idx.at(j)]);
        hardP.Setl1etaByQ(genParticle_l1etaByQ[hardP_idx.at(j)]);
        hardP.Setl1phiByQ(genParticle_l1phiByQ[hardP_idx.at(j)]);
        hardP.Setl1chargeByQ(genParticle_l1chargeByQ[hardP_idx.at(j)]);
        hardP.Setl1qByQ(genParticle_l1qByQ[hardP_idx.at(j)]);
        hardP.Setl1drByQ(genParticle_l1drByQ[hardP_idx.at(j)]);
        hardPs.push_back(hardP);
        NhardP+=1;
        if(hardP.Pt() > turn_on){
          hardPs_above.push_back(hardP);
          NhardP_above+=1;
        }
        if(hardP.Pt() > turn_on_med){
          hardP.SetTag("hardP_med");
          hardPs_above_med.push_back(hardP);
        }
      }
    }

    //==Collect Raw L1Muons (get AtVtx separately, to easily use dR inherited from the TLorentzVector==//
    vector<Object> L1MuonsRaw;
    vector<Object> L1MuonsRaw_AtVtx;
    for(int j=0; j<nL1Muon; j++){
      Object L1Muon;
      Object L1Muon_AtVtx;
      Object L1MuonRaw;
      Object L1MuonRaw_AtVtx;
      L1MuonRaw.SetPtEtaPhiM(L1Muon_pt[j],L1Muon_eta[j],L1Muon_phi[j],0);
      L1MuonRaw.SetTag("L1Raw0");
      L1MuonRaw.SetQuality(L1Muon_quality[j]);
      L1MuonsRaw.push_back(L1MuonRaw);
      L1MuonRaw_AtVtx.SetPtEtaPhiM(L1Muon_pt[j],L1Muon_etaAtVtx[j],L1Muon_phiAtVtx[j],0);
      L1MuonRaw_AtVtx.SetTag("L1Raw0_AtVtx");
      L1MuonRaw_AtVtx.SetQuality(L1Muon_quality[j]);
      L1MuonsRaw_AtVtx.push_back(L1MuonRaw_AtVtx);
    }

    vector<Object> L1Muons_Raw0 = L1MuonsRaw;
    vector<Object> L1Muons_Raw8 = SelectL1(L1MuonsRaw, 0, 8);
    vector<Object> L1Muons_Raw22 = SelectL1(L1MuonsRaw, 0, 22);
    vector<Object> L1Muons_DQ0 = SelectL1(L1MuonsRaw, 7, 0);
    vector<Object> L1Muons_DQ8 = SelectL1(L1MuonsRaw, 7, 8);
    vector<Object> L1Muons_DQ22 = SelectL1(L1MuonsRaw, 7, 22);
    vector<Object> L1Muons_SQ0 = SelectL1(L1MuonsRaw, 11, 0);
    vector<Object> L1Muons_SQ8 = SelectL1(L1MuonsRaw, 11, 8);
    vector<Object> L1Muons_SQ22 = SelectL1(L1MuonsRaw, 11, 22);

    vector<Object> L1Muons_Raw0_AtVtx = L1MuonsRaw_AtVtx;
    vector<Object> L1Muons_Raw8_AtVtx = SelectL1(L1MuonsRaw_AtVtx, 0, 8);
    vector<Object> L1Muons_Raw22_AtVtx = SelectL1(L1MuonsRaw_AtVtx, 0, 22);
    vector<Object> L1Muons_DQ0_AtVtx = SelectL1(L1MuonsRaw_AtVtx, 7, 0);
    vector<Object> L1Muons_DQ8_AtVtx = SelectL1(L1MuonsRaw_AtVtx, 7, 8);
    vector<Object> L1Muons_DQ22_AtVtx = SelectL1(L1MuonsRaw_AtVtx, 7, 22);
    vector<Object> L1Muons_SQ0_AtVtx = SelectL1(L1MuonsRaw_AtVtx, 11, 0);
    vector<Object> L1Muons_SQ8_AtVtx = SelectL1(L1MuonsRaw_AtVtx, 11, 8);
    vector<Object> L1Muons_SQ22_AtVtx = SelectL1(L1MuonsRaw_AtVtx, 11, 22);

    //==L1Muon matched with the propagated gen particles==//
    vector<Object> L1MuonsMatched_Raw0 = SelectL1Matched(hardPs, 0.3, 0, 0);
    vector<Object> L1MuonsMatched_Raw8 = SelectL1Matched(hardPs, 0.3, 0, 8);
    vector<Object> L1MuonsMatched_Raw22 = SelectL1Matched(hardPs, 0.3, 0, 22);
    vector<Object> L1MuonsMatched_DQ0 = SelectL1Matched(hardPs, 0.3, 7, 0);
    vector<Object> L1MuonsMatched_DQ8 = SelectL1Matched(hardPs, 0.3, 7, 8);
    vector<Object> L1MuonsMatched_DQ22 = SelectL1Matched(hardPs, 0.3, 7, 22);
    vector<Object> L1MuonsMatched_SQ0 = SelectL1Matched(hardPs, 0.3, 11, 0);
    vector<Object> L1MuonsMatched_SQ8 = SelectL1Matched(hardPs, 0.3, 11, 8);
    vector<Object> L1MuonsMatched_SQ22 = SelectL1Matched(hardPs, 0.3, 11, 22);

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
    //==hardP muon matched with tight L1==//
    vector<Object> hardPs_L1matched;
    this_matched.clear();
    for(int j=0; j<hardPs.size(); j++){
      Object hardP_L1matched = hardPs.at(j);
      for(int k=0; k<L1Muons_SQ22.size(); k++){
        if( find(this_matched.begin(), this_matched.end(), k) == this_matched.end() ){
          if(hardP_L1matched.DeltaR(L1Muons_SQ22.at(k))<0.3){
            hardP_L1matched.SetTag("hardP_L1");
            hardPs_L1matched.push_back(hardP_L1matched);
            NhardP_L1+=1;
            this_matched.push_back(k);
            break;
          }
        }
      }
    }
    //==hardP muon above single muon trigger turn-on, matched with tight L1==//
    vector<Object> hardPs_above_L1matched;
    this_matched.clear();
    for(int j=0; j<hardPs_above.size(); j++){
      Object hardP_above_L1matched = hardPs_above.at(j);
      for(int k=0; k<L1Muons_SQ22.size(); k++){
        if( find(this_matched.begin(), this_matched.end(), k) == this_matched.end() ){
          if(hardP_above_L1matched.DeltaR(L1Muons_SQ22.at(k))<0.3){
            hardP_above_L1matched.SetTag("hardP_L1");
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
      for(int k=0; k<L1Muons_DQ8.size(); k++){
        if( find(this_matched.begin(), this_matched.end(), k) == this_matched.end() ){
          if(hardP_L1matched_med.DeltaR(L1Muons_DQ8.at(k))<0.3){
            hardP_L1matched_med.SetTag("hardP_L1_med");
            hardPs_L1matched_med.push_back(hardP_L1matched_med);
            this_matched.push_back(k);
            break;
          }
        }
      }
    }
    //==hardP muon above double muon trigger turn-on, matched with medium L1==//
    vector<Object> hardPs_above_L1matched_med;
    this_matched.clear();
    for(int j=0; j<hardPs_above_med.size(); j++){
      Object hardP_above_L1matched_med = hardPs_above_med.at(j);
      for(int k=0; k<L1Muons_DQ8.size(); k++){
        if( find(this_matched.begin(), this_matched.end(), k) == this_matched.end() ){
          if(hardP_above_L1matched_med.DeltaR(L1Muons_DQ8.at(k))<0.3){
            hardP_above_L1matched_med.SetTag("hardP_L1_med");
            hardPs_above_L1matched_med.push_back(hardP_above_L1matched_med);
            this_matched.push_back(k);
            break;
          }
        }
      }
    }

    //==Tracking Particles==//
    vector<Object> TPs;
    if ( ntpTo_hltIterL3OIMuonTrackAssociated != ntpTo_iterL3MuonTrackAssociated ){
      cout << "ERROR :: ntpTo_hltIterL3OIMuonTrackAssociated == " << ntpTo_hltIterL3OIMuonTrackAssociated << ", ntpTo_iterL3MuonTrackAssociated == " << ntpTo_iterL3MuonTrackAssociated << " ." << endl;
      exit(1);
    }
    for(int j=0; j<ntpTo_hltIterL3OIMuonTrackAssociated; j++){ //JH : I'm assuming here the tpTos are the same along each tracks
      if( fabs(tpTo_hltIterL3OIMuonTrackAssociated_gen_eta->at(j)) < 2.4 ){
        Object TP;
        TP.SetPtEtaPhiM(tpTo_hltIterL3OIMuonTrackAssociated_gen_pt->at(j),tpTo_hltIterL3OIMuonTrackAssociated_gen_eta->at(j),tpTo_hltIterL3OIMuonTrackAssociated_gen_phi->at(j),0);
        TP.SetTag("TP");
        TP.SetTruePU(truePU);
        TP.SetHltIterL3OIMuonTrackAssociated_bestMatchTrk_pt(tpTo_hltIterL3OIMuonTrackAssociated_bestMatchTrk_pt->at(j));
        TP.SetHltIter0IterL3MuonTrackAssociated_bestMatchTrk_pt(tpTo_hltIter0IterL3MuonTrackAssociated_bestMatchTrk_pt->at(j));
        TP.SetHltIter2IterL3MuonTrackAssociated_bestMatchTrk_pt(tpTo_hltIter2IterL3MuonTrackAssociated_bestMatchTrk_pt->at(j));
        TP.SetHltIter0IterL3FromL1MuonTrackAssociated_bestMatchTrk_pt(tpTo_hltIter0IterL3FromL1MuonTrackAssociated_bestMatchTrk_pt->at(j));
        TP.SetHltIter2IterL3FromL1MuonTrackAssociated_bestMatchTrk_pt(tpTo_hltIter2IterL3FromL1MuonTrackAssociated_bestMatchTrk_pt->at(j));
        //TP.SetHltIter2IterL3MuonMergedAssociated_bestMatchTrk_pt(tpTo_hltIter2IterL3MuonMergedAssociated_bestMatchTrk_pt->at(j));
        //TP.SetHltIter2IterL3FromL1MuonMergedAssociated_bestMatchTrk_pt(tpTo_hltIter2IterL3FromL1MuonMergedAssociated_bestMatchTrk_pt->at(j)); //JH : deprecated?
        TP.SetHltIterL3MuonMergedAssociated_bestMatchTrk_pt(tpTo_hltIterL3MuonMergedAssociated_bestMatchTrk_pt->at(j));
        TP.SetHltIterL3MuonAndMuonFromL1MergedAssociated_bestMatchTrk_pt(tpTo_hltIterL3MuonAndMuonFromL1MergedAssociated_bestMatchTrk_pt->at(j));
        TP.SetIterL3MuonNoIDTrackAssociated_bestMatchTrk_pt(tpTo_iterL3MuonNoIDTrackAssociated_bestMatchTrk_pt->at(j));
        TP.SetIterL3MuonTrackAssociated_bestMatchTrk_pt(tpTo_iterL3MuonTrackAssociated_bestMatchTrk_pt->at(j));
        TPs.push_back(TP);
        NTP+=1;
      }
    }

    vector<Object> TPs_L1Raw0 = MatchTPtoL1(TPs, hardPs, 0, 0, 0.3);
    vector<Object> TPs_L1Raw8 = MatchTPtoL1(TPs, hardPs, 0, 8, 0.3);
    vector<Object> TPs_L1Raw22 = MatchTPtoL1(TPs, hardPs, 0, 22, 0.3);
    vector<Object> TPs_L1DQ0 = MatchTPtoL1(TPs, hardPs, 7, 0, 0.3);
    vector<Object> TPs_L1DQ8 = MatchTPtoL1(TPs, hardPs, 7, 8, 0.3);
    vector<Object> TPs_L1DQ22 = MatchTPtoL1(TPs, hardPs, 7, 22, 0.3);
    vector<Object> TPs_L1SQ0 = MatchTPtoL1(TPs, hardPs, 11, 0, 0.3);
    vector<Object> TPs_L1SQ8 = MatchTPtoL1(TPs, hardPs, 11, 8, 0.3);
    vector<Object> TPs_L1SQ22 = MatchTPtoL1(TPs, hardPs, 11, 22, 0.3);

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

    vector< vector<Object> > Tracks;
    Tracks.push_back(L3OITracks);
    Tracks.push_back(Iter0L3Tracks);
    Tracks.push_back(Iter2L3Tracks);
    Tracks.push_back(Iter0L3FromL1Tracks);
    Tracks.push_back(Iter2L3FromL1Tracks);

    /*
  //====================================Efficiency vs pt========================================//

    //do_eff_den_pt(hardPs, genEventWeight);
    //do_trackeff_pt(hardPs, Tracks, 0.3, genEventWeight, "all");
    //do_eff_num_pt(hardPs, IOFromL1Tracks, 0.1, genEventWeight);
    //do_eff_num_pt(hardPs, L3Muons_NoID, 0.1, genEventWeight);
    //do_eff_num_pt(hardPs, L3Muons, 0.1, genEventWeight);

    //do_eff_den_pt(hardPs_L1matched, genEventWeight);
    //do_trackeff_pt(hardPs_L1matched, Tracks, 0.3, genEventWeight, "all");
    //do_eff_num_pt(hardPs_L1matched, IOFromL1Tracks, 0.1, genEventWeight);
    //do_eff_num_pt(hardPs_L1matched, L3Muons_NoID, 0.1, genEventWeight);
    //do_eff_num_pt(hardPs_L1matched, L3Muons, 0.1, genEventWeight);

    //do_eff_den_pt(hardPs_L1matched_med, genEventWeight);
    //do_trackeff_pt(hardPs_L1matched_med, Tracks, 0.3, genEventWeight, "all");
    //do_eff_num_pt(hardPs_L1matched_med, IOFromL1Tracks, 0.1, genEventWeight);
    //do_eff_num_pt(hardPs_L1matched_med, L3Muons_NoID, 0.1, genEventWeight);
    //do_eff_num_pt(hardPs_L1matched_med, L3Muons, 0.1, genEventWeight);

  //====================================Efficiency vs eta, phi, truePU========================================//

    //do_eff_den_others(hardPs_above, genEventWeight);
    //do_trackeff_others(hardPs_above, Tracks, 0.3, genEventWeight, "all");
    //do_eff_num_others(hardPs_above, IOFromL1Tracks, 0.1, genEventWeight);
    //do_eff_num_others(hardPs_above, L3Muons_NoID, 0.1, genEventWeight);
    //do_eff_num_others(hardPs_above, L3Muons, 0.1, genEventWeight);

    //do_eff_den_others(hardPs_above_L1matched, genEventWeight);
    //do_trackeff_others(hardPs_above_L1matched, Tracks, 0.3, genEventWeight, "all");
    //do_eff_num_others(hardPs_above_L1matched, IOFromL1Tracks, 0.1, genEventWeight);
    //do_eff_num_others(hardPs_above_L1matched, L3Muons_NoID, 0.1, genEventWeight);
    //do_eff_num_others(hardPs_above_L1matched, L3Muons, 0.1, genEventWeight);

    //do_eff_den_others(hardPs_above_L1matched_med, genEventWeight);
    //do_trackeff_others(hardPs_above_L1matched_med, Tracks, 0.3, genEventWeight, "all");
    //do_eff_num_others(hardPs_above_L1matched_med, IOFromL1Tracks, 0.1, genEventWeight);
    //do_eff_num_others(hardPs_above_L1matched_med, L3Muons_NoID, 0.1, genEventWeight);
    //do_eff_num_others(hardPs_above_L1matched_med, L3Muons, 0.1, genEventWeight);

    //do_eff_den_others(hardPs_above_med, genEventWeight);

  //=======================New efficiency (hit association or L1 matching) vs pt, eta, phi, truePU============================//

    do_neweff_pt(TPs, genEventWeight);
    do_neweff_pt(TPs_L1Raw0, genEventWeight);
    do_neweff_pt(TPs_L1Raw8, genEventWeight);
    do_neweff_pt(TPs_L1Raw22, genEventWeight);
    do_neweff_pt(TPs_L1DQ0, genEventWeight);
    do_neweff_pt(TPs_L1DQ8, genEventWeight);
    do_neweff_pt(TPs_L1DQ22, genEventWeight);
    do_neweff_pt(TPs_L1SQ0, genEventWeight);
    do_neweff_pt(TPs_L1SQ8, genEventWeight);
    do_neweff_pt(TPs_L1SQ22, genEventWeight);

    do_neweff_others(TPs, turn_on, genEventWeight);
    do_neweff_others(TPs_L1Raw0, turn_on, genEventWeight);
    do_neweff_others(TPs_L1Raw8, turn_on, genEventWeight);
    do_neweff_others(TPs_L1Raw22, turn_on, genEventWeight);
    do_neweff_others(TPs_L1DQ0, turn_on, genEventWeight);
    do_neweff_others(TPs_L1DQ8, turn_on, genEventWeight);
    do_neweff_others(TPs_L1DQ22, turn_on, genEventWeight);
    do_neweff_others(TPs_L1SQ0, turn_on, genEventWeight);
    do_neweff_others(TPs_L1SQ8, turn_on, genEventWeight);
    do_neweff_others(TPs_L1SQ22, turn_on, genEventWeight);

    do_neweff_others(TPs, turn_on_med, genEventWeight);
    do_neweff_others(TPs_L1Raw0, turn_on_med, genEventWeight);
    do_neweff_others(TPs_L1Raw8, turn_on_med, genEventWeight);
    do_neweff_others(TPs_L1Raw22, turn_on_med, genEventWeight);
    do_neweff_others(TPs_L1DQ0, turn_on_med, genEventWeight);
    do_neweff_others(TPs_L1DQ8, turn_on_med, genEventWeight);
    do_neweff_others(TPs_L1DQ22, turn_on_med, genEventWeight);
    do_neweff_others(TPs_L1SQ0, turn_on_med, genEventWeight);
    do_neweff_others(TPs_L1SQ8, turn_on_med, genEventWeight);
    do_neweff_others(TPs_L1SQ22, turn_on_med, genEventWeight);

  //==================================L1 efficiency comparison vs pt==========================================//
    
    //do_eff_num_pt(hardPs, L1Muons_Raw0, 0.3, genEventWeight);
    //do_eff_num_pt(hardPs, L1Muons_Raw8, 0.3, genEventWeight);
    //do_eff_num_pt(hardPs, L1Muons_Raw22, 0.3, genEventWeight);
    //do_eff_num_pt(hardPs, L1Muons_DQ0, 0.3, genEventWeight);
    //do_eff_num_pt(hardPs, L1Muons_DQ8, 0.3, genEventWeight);
    //do_eff_num_pt(hardPs, L1Muons_DQ22, 0.3, genEventWeight);
    //do_eff_num_pt(hardPs, L1Muons_SQ0, 0.3, genEventWeight);
    //do_eff_num_pt(hardPs, L1Muons_SQ8, 0.3, genEventWeight);
    //do_eff_num_pt(hardPs, L1Muons_SQ22, 0.3, genEventWeight);

    //do_eff_num_pt(hardPs, L1Muons_Raw0_AtVtx, 0.3, genEventWeight);
    //do_eff_num_pt(hardPs, L1Muons_Raw8_AtVtx, 0.3, genEventWeight);
    //do_eff_num_pt(hardPs, L1Muons_Raw22_AtVtx, 0.3, genEventWeight);
    //do_eff_num_pt(hardPs, L1Muons_DQ0_AtVtx, 0.3, genEventWeight);
    //do_eff_num_pt(hardPs, L1Muons_DQ8_AtVtx, 0.3, genEventWeight);
    //do_eff_num_pt(hardPs, L1Muons_DQ22_AtVtx, 0.3, genEventWeight);
    //do_eff_num_pt(hardPs, L1Muons_SQ0_AtVtx, 0.3, genEventWeight);
    //do_eff_num_pt(hardPs, L1Muons_SQ8_AtVtx, 0.3, genEventWeight);
    //do_eff_num_pt(hardPs, L1Muons_SQ22_AtVtx, 0.3, genEventWeight);

    //do_eff_num_pt(hardPs, L1MuonsMatched_Raw0, 0.3, genEventWeight);
    //do_eff_num_pt(hardPs, L1MuonsMatched_Raw8, 0.3, genEventWeight);
    //do_eff_num_pt(hardPs, L1MuonsMatched_Raw22, 0.3, genEventWeight);
    //do_eff_num_pt(hardPs, L1MuonsMatched_DQ0, 0.3, genEventWeight);
    //do_eff_num_pt(hardPs, L1MuonsMatched_DQ8, 0.3, genEventWeight);
    //do_eff_num_pt(hardPs, L1MuonsMatched_DQ22, 0.3, genEventWeight);
    //do_eff_num_pt(hardPs, L1MuonsMatched_SQ0, 0.3, genEventWeight);
    //do_eff_num_pt(hardPs, L1MuonsMatched_SQ8, 0.3, genEventWeight);
    //do_eff_num_pt(hardPs, L1MuonsMatched_SQ22, 0.3, genEventWeight);

  //==================================L1 efficiency comparison vs eta, phi, truePU (tight turn-on)==========================================//

    //do_eff_num_others(hardPs_above, L1Muons_Raw0, 0.3, genEventWeight);
    //do_eff_num_others(hardPs_above, L1Muons_Raw8, 0.3, genEventWeight);
    //do_eff_num_others(hardPs_above, L1Muons_Raw22, 0.3, genEventWeight);
    //do_eff_num_others(hardPs_above, L1Muons_DQ0, 0.3, genEventWeight);
    //do_eff_num_others(hardPs_above, L1Muons_DQ8, 0.3, genEventWeight);
    //do_eff_num_others(hardPs_above, L1Muons_DQ22, 0.3, genEventWeight);
    //do_eff_num_others(hardPs_above, L1Muons_SQ0, 0.3, genEventWeight);
    //do_eff_num_others(hardPs_above, L1Muons_SQ8, 0.3, genEventWeight);
    //do_eff_num_others(hardPs_above, L1Muons_SQ22, 0.3, genEventWeight);

    //do_eff_num_others(hardPs_above, L1Muons_Raw0_AtVtx, 0.3, genEventWeight);
    //do_eff_num_others(hardPs_above, L1Muons_Raw8_AtVtx, 0.3, genEventWeight);
    //do_eff_num_others(hardPs_above, L1Muons_Raw22_AtVtx, 0.3, genEventWeight);
    //do_eff_num_others(hardPs_above, L1Muons_DQ0_AtVtx, 0.3, genEventWeight);
    //do_eff_num_others(hardPs_above, L1Muons_DQ8_AtVtx, 0.3, genEventWeight);
    //do_eff_num_others(hardPs_above, L1Muons_DQ22_AtVtx, 0.3, genEventWeight);
    //do_eff_num_others(hardPs_above, L1Muons_SQ0_AtVtx, 0.3, genEventWeight);
    //do_eff_num_others(hardPs_above, L1Muons_SQ8_AtVtx, 0.3, genEventWeight);
    //do_eff_num_others(hardPs_above, L1Muons_SQ22_AtVtx, 0.3, genEventWeight);

    //do_eff_num_others(hardPs_above, L1MuonsMatched_Raw0, 0.3, genEventWeight);
    //do_eff_num_others(hardPs_above, L1MuonsMatched_Raw8, 0.3, genEventWeight);
    //do_eff_num_others(hardPs_above, L1MuonsMatched_Raw22, 0.3, genEventWeight);
    //do_eff_num_others(hardPs_above, L1MuonsMatched_DQ0, 0.3, genEventWeight);
    //do_eff_num_others(hardPs_above, L1MuonsMatched_DQ8, 0.3, genEventWeight);
    //do_eff_num_others(hardPs_above, L1MuonsMatched_DQ22, 0.3, genEventWeight);
    //do_eff_num_others(hardPs_above, L1MuonsMatched_SQ0, 0.3, genEventWeight);
    //do_eff_num_others(hardPs_above, L1MuonsMatched_SQ8, 0.3, genEventWeight);
    //do_eff_num_others(hardPs_above, L1MuonsMatched_SQ22, 0.3, genEventWeight);

  //==================================L1 efficiency comparison vs eta, phi, truePU (medium turn-on)==========================================//

    //do_eff_num_others(hardPs_above_med, L1Muons_Raw0, 0.3, genEventWeight);
    //do_eff_num_others(hardPs_above_med, L1Muons_Raw8, 0.3, genEventWeight);
    //do_eff_num_others(hardPs_above_med, L1Muons_Raw22, 0.3, genEventWeight);
    //do_eff_num_others(hardPs_above_med, L1Muons_DQ0, 0.3, genEventWeight);
    //do_eff_num_others(hardPs_above_med, L1Muons_DQ8, 0.3, genEventWeight);
    //do_eff_num_others(hardPs_above_med, L1Muons_DQ22, 0.3, genEventWeight);
    //do_eff_num_others(hardPs_above_med, L1Muons_SQ0, 0.3, genEventWeight);
    //do_eff_num_others(hardPs_above_med, L1Muons_SQ8, 0.3, genEventWeight);
    //do_eff_num_others(hardPs_above_med, L1Muons_SQ22, 0.3, genEventWeight);

    //do_eff_num_others(hardPs_above_med, L1Muons_Raw0_AtVtx, 0.3, genEventWeight);
    //do_eff_num_others(hardPs_above_med, L1Muons_Raw8_AtVtx, 0.3, genEventWeight);
    //do_eff_num_others(hardPs_above_med, L1Muons_Raw22_AtVtx, 0.3, genEventWeight);
    //do_eff_num_others(hardPs_above_med, L1Muons_DQ0_AtVtx, 0.3, genEventWeight);
    //do_eff_num_others(hardPs_above_med, L1Muons_DQ8_AtVtx, 0.3, genEventWeight);
    //do_eff_num_others(hardPs_above_med, L1Muons_DQ22_AtVtx, 0.3, genEventWeight);
    //do_eff_num_others(hardPs_above_med, L1Muons_SQ0_AtVtx, 0.3, genEventWeight);
    //do_eff_num_others(hardPs_above_med, L1Muons_SQ8_AtVtx, 0.3, genEventWeight);
    //do_eff_num_others(hardPs_above_med, L1Muons_SQ22_AtVtx, 0.3, genEventWeight);

    //do_eff_num_others(hardPs_above_med, L1MuonsMatched_Raw0, 0.3, genEventWeight);
    //do_eff_num_others(hardPs_above_med, L1MuonsMatched_Raw8, 0.3, genEventWeight);
    //do_eff_num_others(hardPs_above_med, L1MuonsMatched_Raw22, 0.3, genEventWeight);
    //do_eff_num_others(hardPs_above_med, L1MuonsMatched_DQ0, 0.3, genEventWeight);
    //do_eff_num_others(hardPs_above_med, L1MuonsMatched_DQ8, 0.3, genEventWeight);
    //do_eff_num_others(hardPs_above_med, L1MuonsMatched_DQ22, 0.3, genEventWeight);
    //do_eff_num_others(hardPs_above_med, L1MuonsMatched_SQ0, 0.3, genEventWeight);
    //do_eff_num_others(hardPs_above_med, L1MuonsMatched_SQ8, 0.3, genEventWeight);
    //do_eff_num_others(hardPs_above_med, L1MuonsMatched_SQ22, 0.3, genEventWeight);

  //==================================L1 matched by l1drByQ efficiency vs pt, eta, phi, truePU==========================================//

    //do_neweff_L1MuonsMatched_pt(hardPs, 0.3, 0, 0, genEventWeight);
    //do_neweff_L1MuonsMatched_pt(hardPs, 0.3, 0, 8, genEventWeight);
    //do_neweff_L1MuonsMatched_pt(hardPs, 0.3, 0, 22, genEventWeight);
    //do_neweff_L1MuonsMatched_pt(hardPs, 0.3, 7, 0, genEventWeight);
    //do_neweff_L1MuonsMatched_pt(hardPs, 0.3, 7, 8, genEventWeight);
    //do_neweff_L1MuonsMatched_pt(hardPs, 0.3, 7, 22, genEventWeight);
    //do_neweff_L1MuonsMatched_pt(hardPs, 0.3, 11, 0, genEventWeight);
    //do_neweff_L1MuonsMatched_pt(hardPs, 0.3, 11, 8, genEventWeight);
    //do_neweff_L1MuonsMatched_pt(hardPs, 0.3, 11, 22, genEventWeight);

    //do_neweff_L1MuonsMatched_others(hardPs, 0.3, 0, 0, turn_on, genEventWeight);
    //do_neweff_L1MuonsMatched_others(hardPs, 0.3, 0, 8, turn_on, genEventWeight);
    //do_neweff_L1MuonsMatched_others(hardPs, 0.3, 0, 22, turn_on, genEventWeight);
    //do_neweff_L1MuonsMatched_others(hardPs, 0.3, 7, 0, turn_on, genEventWeight);
    //do_neweff_L1MuonsMatched_others(hardPs, 0.3, 7, 8, turn_on, genEventWeight);
    //do_neweff_L1MuonsMatched_others(hardPs, 0.3, 7, 22, turn_on, genEventWeight);
    //do_neweff_L1MuonsMatched_others(hardPs, 0.3, 11, 0, turn_on, genEventWeight);
    //do_neweff_L1MuonsMatched_others(hardPs, 0.3, 11, 8, turn_on, genEventWeight);
    //do_neweff_L1MuonsMatched_others(hardPs, 0.3, 11, 22, turn_on, genEventWeight);

    //do_neweff_L1MuonsMatched_others(hardPs, 0.3, 0, 0, turn_on_med, genEventWeight);
    //do_neweff_L1MuonsMatched_others(hardPs, 0.3, 0, 8, turn_on_med, genEventWeight);
    //do_neweff_L1MuonsMatched_others(hardPs, 0.3, 0, 22, turn_on_med, genEventWeight);
    //do_neweff_L1MuonsMatched_others(hardPs, 0.3, 7, 0, turn_on_med, genEventWeight);
    //do_neweff_L1MuonsMatched_others(hardPs, 0.3, 7, 8, turn_on_med, genEventWeight);
    //do_neweff_L1MuonsMatched_others(hardPs, 0.3, 7, 22, turn_on_med, genEventWeight);
    //do_neweff_L1MuonsMatched_others(hardPs, 0.3, 11, 0, turn_on_med, genEventWeight);
    //do_neweff_L1MuonsMatched_others(hardPs, 0.3, 11, 8, turn_on_med, genEventWeight);
    //do_neweff_L1MuonsMatched_others(hardPs, 0.3, 11, 22, turn_on_med, genEventWeight);

  //==========================================Purity====================================================//

    do_purity(L3Muons_NoID, Finals, 0.1, genEventWeight);
    do_purity(L3Muons, Finals, 0.1, genEventWeight);
    do_purity(L3MuonTracks_NoID, L3MuonAssoTracks_NoID, Finals, 0.1, genEventWeight);
    do_purity(L3MuonTracks, L3MuonAssoTracks, Finals, 0.1, genEventWeight);
    do_purity(Iter2L3FromL1Tracks, Iter2L3FromL1AssoTracks, Finals, 0.1, genEventWeight);
    do_purity(Iter2L3Tracks, Iter2L3AssoTracks, Finals, 0.1, genEventWeight);
    do_purity(L3OITracks, L3OIAssoTracks, Finals, 0.1, genEventWeight);
    do_purity(IOFromL1Tracks, IOFromL1AssoTracks, Finals, 0.1, genEventWeight);

  //==========================================dR scan===================================================//

    //hardP vs tracks
    draw_dR(hardPs, Tracks[0], genEventWeight);
    draw_dR(hardPs, Tracks[1], genEventWeight);
    draw_dR(hardPs, Tracks[2], genEventWeight);
    draw_dR(hardPs, Tracks[3], genEventWeight);
    draw_dR(hardPs, Tracks[4], genEventWeight);
    //hardP vs L1s
    draw_dR(hardPs, L1Muons_Raw0, genEventWeight);
    draw_dR(hardPs, L1Muons_DQ8, genEventWeight);
    draw_dR(hardPs, L1Muons_SQ22, genEventWeight);
    draw_dR(hardPs, L1Muons_Raw0_AtVtx, genEventWeight);
    draw_dR(hardPs, L1Muons_DQ8_AtVtx, genEventWeight);
    draw_dR(hardPs, L1Muons_SQ22_AtVtx, genEventWeight);
    draw_l1drByQ(hardPs, 0, 0, genEventWeight);
    draw_l1drByQ(hardPs, 0, 8, genEventWeight);
    draw_l1drByQ(hardPs, 0, 22, genEventWeight);
    draw_l1drByQ(hardPs, 7, 0, genEventWeight);
    draw_l1drByQ(hardPs, 7, 8, genEventWeight);
    draw_l1drByQ(hardPs, 7, 22, genEventWeight);
    draw_l1drByQ(hardPs, 11, 0, genEventWeight);
    draw_l1drByQ(hardPs, 11, 8, genEventWeight);
    draw_l1drByQ(hardPs, 11, 22, genEventWeight);
    //hardP vs L3s
    draw_dR(hardPs, L3Muons_NoID, genEventWeight);
    draw_dR(hardPs, L3Muons, genEventWeight);
    
    */ //JH : restore this later when needed

  //==========================================track vs sigmoid MVA===================================================//

    //JH : Note that only Iter0s are allowed with the patatrack!

    for(int j=0; j<nhltIter0IterL3MuonTrackAssociated; j++){
      FillHist("Iter0FromL2_mva",1./(1.+exp(-1.*hltIter2IterL3MuonTrackAssociated_mva->at(j))),genEventWeight,100,0,1);
      if( hltIter0IterL3MuonTrackAssociated_bestMatchTP_status->at(j) == 1 && fabs(hltIter0IterL3MuonTrackAssociated_bestMatchTP_pdgId->at(j)) == 13 ){
        FillHist("Iter0FromL2_sig_mva",1./(1.+exp(-1.*hltIter2IterL3MuonTrackAssociated_mva->at(j))),genEventWeight,100,0,1);
      }
      else{
        FillHist("Iter0FromL2_fake_mva",1./(1.+exp(-1.*hltIter2IterL3MuonTrackAssociated_mva->at(j))),genEventWeight,100,0,1);
      }
      if(hltIter0IterL3MuonTrackAssociated_pt->at(j)<8.){
        FillHist("Iter0FromL2_mva_below8",1./(1.+exp(-1.*hltIter2IterL3MuonTrackAssociated_mva->at(j))),genEventWeight,100,0,1);
        if( hltIter0IterL3MuonTrackAssociated_bestMatchTP_status->at(j) == 1 && fabs(hltIter0IterL3MuonTrackAssociated_bestMatchTP_pdgId->at(j)) == 13 ){
          FillHist("Iter0FromL2_sig_mva_below8",1./(1.+exp(-1.*hltIter2IterL3MuonTrackAssociated_mva->at(j))),genEventWeight,100,0,1);
        }
        else{
          FillHist("Iter0FromL2_fake_mva_below8",1./(1.+exp(-1.*hltIter2IterL3MuonTrackAssociated_mva->at(j))),genEventWeight,100,0,1);
        }
      }
      else if(hltIter0IterL3MuonTrackAssociated_pt->at(j)>=8.){
        FillHist("Iter0FromL2_mva_above8",1./(1.+exp(-1.*hltIter2IterL3MuonTrackAssociated_mva->at(j))),genEventWeight,100,0,1);
        if( hltIter0IterL3MuonTrackAssociated_bestMatchTP_status->at(j) == 1 && fabs(hltIter0IterL3MuonTrackAssociated_bestMatchTP_pdgId->at(j)) == 13 ){
          FillHist("Iter0FromL2_sig_mva_above8",1./(1.+exp(-1.*hltIter2IterL3MuonTrackAssociated_mva->at(j))),genEventWeight,100,0,1);
        }
        else{
          FillHist("Iter0FromL2_fake_mva_above8",1./(1.+exp(-1.*hltIter2IterL3MuonTrackAssociated_mva->at(j))),genEventWeight,100,0,1);
        }
      }
    }
    for(int j=0; j<nhltIter0IterL3FromL1MuonTrackAssociated; j++){
      FillHist("Iter0FromL1_mva",1./(1.+exp(-1.*hltIter2IterL3FromL1MuonTrackAssociated_mva->at(j))),genEventWeight,100,0,1);
      if( hltIter0IterL3FromL1MuonTrackAssociated_bestMatchTP_status->at(j) == 1 && fabs(hltIter0IterL3FromL1MuonTrackAssociated_bestMatchTP_pdgId->at(j)) == 13 ){
        FillHist("Iter0FromL1_sig_mva",1./(1.+exp(-1.*hltIter2IterL3FromL1MuonTrackAssociated_mva->at(j))),genEventWeight,100,0,1);
      }
      else{
        FillHist("Iter0FromL1_fake_mva",1./(1.+exp(-1.*hltIter2IterL3FromL1MuonTrackAssociated_mva->at(j))),genEventWeight,100,0,1);
      }
      if(hltIter0IterL3FromL1MuonTrackAssociated_pt->at(j)<8.){
        FillHist("Iter0FromL1_mva_below8",1./(1.+exp(-1.*hltIter2IterL3FromL1MuonTrackAssociated_mva->at(j))),genEventWeight,100,0,1);
        if( hltIter0IterL3FromL1MuonTrackAssociated_bestMatchTP_status->at(j) == 1 && fabs(hltIter0IterL3FromL1MuonTrackAssociated_bestMatchTP_pdgId->at(j)) == 13 ){
          FillHist("Iter0FromL1_sig_mva_below8",1./(1.+exp(-1.*hltIter2IterL3FromL1MuonTrackAssociated_mva->at(j))),genEventWeight,100,0,1);
        }
        else{
          FillHist("Iter0FromL1_fake_mva_below8",1./(1.+exp(-1.*hltIter2IterL3FromL1MuonTrackAssociated_mva->at(j))),genEventWeight,100,0,1);
        }
      }
      else if(hltIter0IterL3FromL1MuonTrackAssociated_pt->at(j)>=8.){
        FillHist("Iter0FromL1_mva_above8",1./(1.+exp(-1.*hltIter2IterL3FromL1MuonTrackAssociated_mva->at(j))),genEventWeight,100,0,1);
        if( hltIter0IterL3FromL1MuonTrackAssociated_bestMatchTP_status->at(j) == 1 && fabs(hltIter0IterL3FromL1MuonTrackAssociated_bestMatchTP_pdgId->at(j)) == 13 ){
          FillHist("Iter0FromL1_sig_mva_above8",1./(1.+exp(-1.*hltIter2IterL3FromL1MuonTrackAssociated_mva->at(j))),genEventWeight,100,0,1);
        }
        else{
          FillHist("Iter0FromL1_fake_mva_above8",1./(1.+exp(-1.*hltIter2IterL3FromL1MuonTrackAssociated_mva->at(j))),genEventWeight,100,0,1);
        }
      }
    } //JH : FIXME why call Iter2 as Iter0? For now, Ntupler is not consistent so Iter2 actually contains Iter0. In addition, Iter0 hasn't been filled with info(!).

  //==========================================track profile vs pileup===================================================//

    //JH : pt-dependent profile
    int nhltIter0IterL3MuonTrack_below8 = 0;
    int nhltIter0IterL3MuonTrack_above8 = 0;
    int nhltIter0IterL3FromL1MuonTrack_below8 = 0;
    int nhltIter0IterL3FromL1MuonTrack_above8 = 0;
    for(unsigned int i=0; i<nhltIter0IterL3MuonTrack; i++){
      if(hltIter0IterL3MuonTrack_pt->at(i)<8.) nhltIter0IterL3MuonTrack_below8++;
      else if(hltIter0IterL3MuonTrack_pt->at(i)>=8.) nhltIter0IterL3MuonTrack_above8++;
    }
    for(unsigned int i=0; i<nhltIter0IterL3FromL1MuonTrack; i++){
      if(hltIter0IterL3FromL1MuonTrack_pt->at(i)<8.) nhltIter0IterL3FromL1MuonTrack_below8++;
      else if(hltIter0IterL3FromL1MuonTrack_pt->at(i)>=8.) nhltIter0IterL3FromL1MuonTrack_above8++;
    }

    pf_track->Fill(truePU,nhltIter0IterL3MuonTrack);
    pf_track_below8->Fill(truePU,nhltIter0IterL3MuonTrack_below8);
    pf_track_above8->Fill(truePU,nhltIter0IterL3MuonTrack_above8);
    pf_trackFromL1->Fill(truePU,nhltIter0IterL3FromL1MuonTrack);
    pf_trackFromL1_below8->Fill(truePU,nhltIter0IterL3FromL1MuonTrack_below8);
    pf_trackFromL1_above8->Fill(truePU,nhltIter0IterL3FromL1MuonTrack_above8);

  }
  
  cout << "=================================================" << endl;
  cout << "total NhardPs : " << NhardP << endl;
  cout << "total NhardPs above 26GeV : " << NhardP_above << endl;
  cout << "total NhardPs_L1 : " << NhardP_L1 << endl;
  cout << "total TPs : " << NTP << endl;
  cout << "total TPs matched to single muon L1 : " << NTP_L1 << endl;
  cout << "total TPs matched to double muon L1 : " << NTP_L1_med << endl;
  cout << "total track pt eff : " << Nmatched_hardP_tracks_pt/NhardP << endl;
  cout << "total track others eff : " << Nmatched_hardP_tracks_others/NhardP_above << endl;
  cout << "L3NoID pt eff : " << Nmatched_hardP_L3NoID_pt/NhardP << endl;
  cout << "L3NoID others eff : " << Nmatched_hardP_L3NoID_others/NhardP_above << endl;
  cout << "L3 pt eff : " << Nmatched_hardP_L3_pt/NhardP << endl;
  cout << "L3 others eff : " << Nmatched_hardP_L3_others/NhardP_above << endl;
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
  pf_track_below8->Write();
  pf_track_above8->Write();
  pf_trackFromL1->Write();
  pf_trackFromL1_below8->Write();
  pf_trackFromL1_above8->Write();
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
  delete pf_track_below8;
  delete pf_track_above8;
  delete pf_trackFromL1;
  delete pf_trackFromL1_below8;
  delete pf_trackFromL1_above8;

}



//TH1 -> GetCumulative (forward = false)
//First, draw Nsignal vs mva(sigmoid), Nbkg vs mva(sigmoid).
//Then calculate GetCumulative(some bin)/GetCumulative(0).
//Iter2 track, Iter2FromL1 track
