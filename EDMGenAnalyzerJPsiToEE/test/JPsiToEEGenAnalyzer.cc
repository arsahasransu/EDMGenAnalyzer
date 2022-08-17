#include <iostream>

#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"

// essentials !!!
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TH1.h"
#include "TLorentzVector.h"

#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

class JPsiToEEGenAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources> {
public:
  //
  explicit JPsiToEEGenAnalyzer(const edm::ParameterSet&);
  ~JPsiToEEGenAnalyzer() = default;  // no need to delete ROOT stuff
                                // as it'll be deleted upon closing TFile

  void analyze(const edm::Event&, const edm::EventSetup&) override;
  void beginJob() override;

private:
  const edm::EDGetTokenT<GenEventInfoProduct> tokenGenInfo_;
  const edm::EDGetTokenT< std::vector<reco::GenParticle> > tokenGenPart_;
  TH1D* fHistjpsinum;
  TH1D* fHistjpsipt;
  TH1D* fHistFSelnum;
  TH1D* fHisteeMass;
  TH1D* fHistept;
  TH1D* fHisteeta;
  TH1D* fHistephi;
  TH1D* fHistleadept;
  TH1D* fHistleadeeta;
  TH1D* fHistleadephi;
  TH1D* fHistsubleadept;
  TH1D* fHistsubleadeeta;
  TH1D* fHistsubleadephi;
};

JPsiToEEGenAnalyzer::JPsiToEEGenAnalyzer(const edm::ParameterSet& pset)
    : tokenGenInfo_(consumes<GenEventInfoProduct>(edm::InputTag("generator"))),
      tokenGenPart_(consumes< std::vector<reco::GenParticle> >(edm::InputTag("genParticles"))),
      fHistjpsinum(nullptr),
      fHistjpsipt(nullptr),
      fHistFSelnum(nullptr),
      fHisteeMass(nullptr),
      fHistept(nullptr),
      fHisteeta(nullptr),
      fHistephi(nullptr),
      fHistleadept(nullptr) {
  // actually, pset is NOT in use - we keep it here just for illustratory putposes
  usesResource(TFileService::kSharedResource);
}

void JPsiToEEGenAnalyzer::beginJob() {
  edm::Service<TFileService> fs;
  fHistjpsinum = fs->make<TH1D>("Histjpsinum", "J/#psi multiplicity", 100, 0, 100.);
  fHistjpsipt = fs->make<TH1D>("Histjpsipt", "J/#psi pt", 1000, 0, 100.);
  fHistFSelnum = fs->make<TH1D>("HistFSelnum", "final state electron multiplicity", 100, 0, 100.);
  fHisteeMass = fs->make<TH1D>("HisteeMass", "2-e inv. mass", 10000, 0, 100.);
  fHistept = fs->make<TH1D>("Histept", "electron pt", 1000, 0., 100.);
  fHistleadept = fs->make<TH1D>("Histleadept", "lead electron pt", 1000, 0., 100.);
}

void JPsiToEEGenAnalyzer::analyze(const edm::Event& e, const edm::EventSetup&) {
  // here's an example of accessing GenEventInfoProduct
  const edm::Handle<GenEventInfoProduct>& GenInfoHandle = e.getHandle(tokenGenInfo_);
  double qScale = GenInfoHandle->qScale();
  double pthat = (GenInfoHandle->hasBinningValues() ? (GenInfoHandle->binningValues())[0] : 0.0);
  std::cout << " qScale = " << qScale << " pthat = " << pthat << std::endl;

  edm::Handle<std::vector<reco::GenParticle>> genPartH;
  e.getByToken(tokenGenPart_, genPartH);
  bool genPartValid = genPartH.isValid();

  int genJpsicnt = 0;
  int genElFScnt = 0;
  std::vector<reco::GenParticle*> FSelectrons;

  if(genPartValid) {
    
    for(auto gen_iter=genPartH->begin(); gen_iter!=genPartH->end(); ++gen_iter) {

      if(gen_iter->pdgId()==443) {
	genJpsicnt++;
	fHistjpsipt->Fill(gen_iter->pt());
      }
      if(abs(gen_iter->pdgId()==11) && gen_iter->isLastCopy()) {
	FSelectrons.push_back(gen_iter->clone());
	genElFScnt++;
      }
    }

      fHistjpsinum->Fill(genJpsicnt);
      fHistFSelnum->Fill(genElFScnt);
  }

  for(unsigned int ctr1=0; ctr1<FSelectrons.size(); ctr1++) {
    fHistept->Fill(FSelectrons[ctr1]->pt());
    for(unsigned int ctr2=ctr1+1; ctr2<FSelectrons.size(); ctr2++) {
      TLorentzVector el1, el2;
      el1.SetPtEtaPhiM(FSelectrons[ctr1]->pt(),FSelectrons[ctr1]->eta(),FSelectrons[ctr1]->phi(),0.0005);
      el2.SetPtEtaPhiM(FSelectrons[ctr2]->pt(),FSelectrons[ctr2]->eta(),FSelectrons[ctr2]->phi(),0.0005);
      fHisteeMass->Fill((el1+el2).M());
    }
  }

  FSelectrons.clear();
  
  return;
}

typedef JPsiToEEGenAnalyzer JPsiToEEGenExampleAnalyzer;
DEFINE_FWK_MODULE(JPsiToEEGenExampleAnalyzer);
