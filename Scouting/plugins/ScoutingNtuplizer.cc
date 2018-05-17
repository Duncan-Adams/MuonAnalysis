// -*- C++ -*-
//
// Package:    MuonAnalysis/ScoutingNtuplizer
// Class:      ScoutingNtuplizer
// 
/**\class ScoutingNtuplizer ScoutingNtuplizer.cc MuonAnalysis/ScoutingNtuplizer/plugins/ScoutingNtuplizer.cc

 Description: make ntuples from scoutingcalomuon data
*/
//
// Original Author:  Duncan Adams
//         Created:  Wed, 16 May 2018 02:08:47 GMT
//
// Heavily Based on code by David Sheffield, Ian Graham, and Abhijith Gandrakota

#include "MuonAnalysis/Scouting/interface/ScoutingNtuplizer.h"
using namespace std;
using namespace edm;



//*****************************//
// constructors and destructor //
//*****************************//
ScoutingNtuplizer::ScoutingNtuplizer(const edm::ParameterSet& iConfig):
    token_jets(consumes<ScoutingCaloJetCollection>(
                   iConfig.getParameter<InputTag>("jet_collection"))),
    token_muons(consumes<ScoutingMuonCollection>(
                    iConfig.getParameter<InputTag>("muon_collection"))),
    token_rho(consumes<double>(
                  iConfig.getParameter<InputTag>("rho"))),
    token_MET_pt(consumes<double>(iConfig.getParameter<InputTag>("MET_pt"))),
    token_MET_phi(consumes<double>(iConfig.getParameter<InputTag>("MET_phi"))),
    token_vertices(consumes<ScoutingVertexCollection>(
                       iConfig.getParameter<InputTag>("vertex_collection"))),
    file_name(iConfig.getParameter<string>("output_file_name"))

{
   //now do what ever initialization is needed
   file = new TFile(file_name.c_str(), "RECREATE");
   tree = new TTree("scoutingntuplizer", "Tree for scouting data");

   tree->Branch("rho", &rho, "rho/F");
   tree->Branch("Run", &run, "Run/I");
   tree->Branch("Lumi", &lumi, "Lumi/I");
   tree->Branch("Event", &event, "Event/I");
   
   tree->Branch("jet_num", &jet_num, "jet_num/I");
   tree->Branch("jet_pt",  &jet_pt);
   tree->Branch("jet_eta", &jet_eta);
   tree->Branch("jet_phi", &jet_phi);
   tree->Branch("jet_m", &jet_m);
    
   tree->Branch("muon_num", &muon_num, "muon_num/I");
   tree->Branch("muon_pt",  &muon_pt);
   tree->Branch("muon_eta", &muon_eta);
   tree->Branch("muon_phi", &muon_phi);
   
   tree->Branch("MET_pt",  &MET_pt);
   tree->Branch("MET_phi", &MET_phi);


}


ScoutingNtuplizer::~ScoutingNtuplizer() {
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
    file->cd();
    tree->Write();
    file->Close();
}


//
// member functions
//

// ------------ method called for each event  ------------
void ScoutingNtuplizer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
   using namespace edm;
   int getCollectionsResult = GetCollections(iEvent);
    if (getCollectionsResult)
	return;
    
	ResetVariables();
	
    rho = *handle_rho;
    run = iEvent.id().run();
    lumi = iEvent.id().luminosityBlock();
    event = iEvent.id().event();
    
    //Jets
    for (auto &j: *jets) {
		if(fabs(j.eta()) <= 2.4) {
			jet_pt.push_back(j.pt());
			jet_eta.push_back(j.eta());
			jet_phi.push_back(j.phi());
			jet_m.push_back(j.m());		
			jet_num += 1;
	    }
	}
	
    
    //Muons
    for (auto &m: *muons) {
	    if(m.isGlobalMuon()) {
	        muon_pt.push_back(m.pt());
	        muon_eta.push_back(m.eta()); 
	        muon_phi.push_back(m.phi()); 
	        muon_num += 1;
	    }
	}
    
    //MET
    MET_pt = *handle_MET_pt;
    MET_phi = *handle_MET_phi;
    
    //Vertices
    
    
    tree->Fill();
}


// ------------ method called once each job just before starting event loop  ------------
void ScoutingNtuplizer::beginJob() {

}

// ------------ method called once each job just after ending the event loop  ------------
void ScoutingNtuplizer::endJob() {
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ScoutingNtuplizer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

int ScoutingNtuplizer::GetCollections(const edm::Event& iEvent) {
    // Get jets
    iEvent.getByToken(token_jets, jets);
    if (!jets.isValid()) {
        throw edm::Exception(edm::errors::ProductNotFound)
	    << "Could not find ScoutingCaloJetCollection." << endl;
	return 1;
    }
   
   
    // Get rho
    iEvent.getByToken(token_rho, handle_rho);
    if (!handle_rho.isValid()) {
        throw edm::Exception(edm::errors::ProductNotFound)
            << "Could not find rho." << endl;
        return 1;
    }	
	
	// Get muons
    iEvent.getByToken(token_muons, muons);
    if (!muons.isValid()) {
        throw edm::Exception(edm::errors::ProductNotFound)
            << "Could not find ScoutingMuonCollection." << endl;
        return 1;
    }
    
    // Get MET
    iEvent.getByToken(token_MET_pt, handle_MET_pt);
    if (!handle_MET_pt.isValid()) {
        throw edm::Exception(edm::errors::ProductNotFound)
	    << "Could not find MET." << endl;
	return 1;
    }

    iEvent.getByToken(token_MET_phi, handle_MET_phi);
    if (!handle_MET_phi.isValid()) {
        throw edm::Exception(edm::errors::ProductNotFound)
	    << "Could not find MET_phi." << endl;
	return 1;
    }
   
    // Get Vertices
    iEvent.getByToken(token_vertices, vertices);
    if (!vertices.isValid()) {
        throw edm::Exception(edm::errors::ProductNotFound)
	    << "Could not find ScoutingVertexCollection." << endl;
	return 1;
    }
    
   
	
	return 0;
}

void ScoutingNtuplizer::ResetVariables() {
    
    jet_num = 0;
    jet_pt.clear();
    jet_eta.clear();
    jet_phi.clear();
    jet_m.clear();
    
    rho = 0.0;
    run = 0;
    lumi = 0;
    event = 0;
    
    MET_phi = 0;
    MET_pt = 0;
    
    muon_num = 0;
    muon_pt.clear();
    muon_eta.clear();
    muon_phi.clear();
	
}

//define this as a plug-in
DEFINE_FWK_MODULE(ScoutingNtuplizer);
