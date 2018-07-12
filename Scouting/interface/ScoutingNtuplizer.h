// System include files
#include <memory>
#include <iostream>
#include <vector>
#include <utility>

// CMSSW include files
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DataFormats/Scouting/interface/ScoutingCaloJet.h"
#include "DataFormats/Scouting/interface/ScoutingParticle.h"
#include "DataFormats/Scouting/interface/ScoutingVertex.h"
#include "DataFormats/Scouting/interface/ScoutingMuon.h"


// Root include files
#include "TLorentzVector.h"
#include "TFile.h"
#include "TTree.h"
#include "TClonesArray.h"

class ScoutingNtuplizer : public edm::EDAnalyzer {
   public:
      explicit ScoutingNtuplizer(const edm::ParameterSet&);
      ~ScoutingNtuplizer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;
      virtual int GetCollections(const edm::Event&);
      
      void ResetVariables();

    // ----------member data ---------------------------
      
      
      // Jets
      edm::EDGetTokenT<ScoutingCaloJetCollection> token_jets;
      edm::Handle<ScoutingCaloJetCollection> jets;

      int jet_num;
      std::vector<float> jet_pt;
      std::vector<float> jet_eta;
      std::vector<float> jet_phi;
      std::vector<float> jet_m;
      
      std::vector<float> jet_Area;
	  std::vector<float> jet_maxEInEmTowers;
	  std::vector<float> jet_maxEInHadTowers;
	  std::vector<float> jet_hadEnergyInHB;
	  std::vector<float> jet_hadEnergyInHE;
	  std::vector<float> jet_hadEnergyInHF;
	  std::vector<float> jet_emEnergyInEB;
       std::vector<float> jet_emEnergyInEE;
       std::vector<float> jet_emEnergyInHF;
       std::vector<float> jet_towersArea;
       std::vector<float> jet_mvaDiscriminator;
       std::vector<float> jet_btagDiscriminator;      
     
      double HT;

  
 
    // Muon Data    
      edm::EDGetTokenT<ScoutingMuonCollection> token_muons;
      edm::Handle<ScoutingMuonCollection> muons;
  
      int muon_num;
      std::vector<int> muon_q;
      std::vector<float> muon_pt;
      std::vector<float> muon_eta;
      std::vector<float> muon_phi;
      
      std::vector<int> muon_isGlobalMuon;
      std::vector<int> muon_isTrackerMuon;
      


    // Event Data
      edm::EDGetTokenT<double> token_rho;
      edm::EDGetTokenT<double> token_MET_pt;
      edm::EDGetTokenT<double> token_MET_phi;
      
      edm::Handle<double> handle_rho;
      edm::Handle<double> handle_MET_pt;
      edm::Handle<double> handle_MET_phi;
      
      edm::EDGetTokenT<ScoutingVertexCollection> token_vertices;
      edm::Handle<ScoutingVertexCollection> vertices;

      int vertex_num;   
      
      std::vector<float> vtx_x;
      std::vector<float> vtx_y;
      std::vector<float> vtx_z;
      
      std::vector<float> vtx_ex;
      std::vector<float> vtx_ey;
      std::vector<float> vtx_ez;
      
      
      float rho;
      float MET_pt;
      float MET_phi;
      
      int run;
      int lumi;
      int event;
      
      std::string file_name;
      TFile *file;
      TTree *tree;
      
     int min_muons;
    // TODO trigger information
};
