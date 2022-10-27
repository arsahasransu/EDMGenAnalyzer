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
#include "TH2.h"
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
  
  reco::GenParticle* findLastCopyDaughter(reco::GenParticle *);

  const edm::EDGetTokenT<GenEventInfoProduct> tokenGenInfo_;
  const edm::EDGetTokenT< std::vector<reco::GenParticle> > tokenGenPart_;
  TH1D *fHistjpsimult, *fHistjpsigenstat, *fHistjpsipt, *fHistjpsieta, *fHistjpsiphi, *fHistjpsinmoms, *fHistjpsimompdg1, *fHistjpsimompdg2, *fHistjpsinsons, *fHistjpsisonpdg1, *fHistjpsisonpdg2;
  TH1D *fHistgoodjpsimult, *fHistgoodjpsigenstat, *fHistgoodjpsipt, *fHistgoodjpsieta, *fHistgoodjpsiphi, *fHistgoodjpsinmoms, *fHistgoodjpsimompdg1, *fHistgoodjpsimompdg2, *fHistgoodjpsinsons, *fHistgoodjpsisonpdg1, *fHistgoodjpsisonpdg2;
  TH1D *fHistFSelnum, *fHistFSelpt, *fHistFSeleta, *fHistFSelphi, *fHistFSleadelpt, *fHistFSleadeleta, *fHistFSleadelphi, *fHistFSsubleadelpt, *fHistFSsubleadeleta, *fHistFSsubleadelphi, *fHistFSel1el2mass, *fHistFSel1el2deta, *fHistFSel1el2dphi, *fHistFSel1el2dr;
  TH1D *fHistFSjpsisonelnum, *fHistFSjpsisonelpt, *fHistFSjpsisoneleta, *fHistFSjpsisonelphi, *fHistFSjpsisonleadelpt, *fHistFSjpsisonleadeleta, *fHistFSjpsisonleadelphi, *fHistFSjpsisonsubleadelpt, *fHistFSjpsisonsubleadeleta, *fHistFSjpsisonsubleadelphi, *fHistFSjpsisonel1el2mass, *fHistFSjpsisonel1el2deta, *fHistFSjpsisonel1el2dphi, *fHistFSjpsisonel1el2dr;
  TH2D *f2DHistGoodJPsiFSJpsisonleadelE, *f2DHistGoodJPsiFSJpsisonsubleadelE, *f2DHistGoodJPsiFSJpsisonleadelpT, *f2DHistGoodJPsiFSJpsisonsubleadelpT, *f2DHistGoodJPsiEFSJpsisonleadsubleadelDr, *f2DHistGoodJPsiEFSJpsisonleadsubleadelDeta, *f2DHistGoodJPsiEFSJpsisonleadsubleadelDphi, *f2DHistGoodJPsiEFSJpsisonleadsubleadelDpt;

};

JPsiToEEGenAnalyzer::JPsiToEEGenAnalyzer(const edm::ParameterSet& pset) : 
  tokenGenInfo_(consumes<GenEventInfoProduct>(edm::InputTag("generator"))),
  tokenGenPart_(consumes< std::vector<reco::GenParticle> >(edm::InputTag("genParticles"))),
  fHistjpsimult(nullptr), fHistjpsigenstat(nullptr), fHistjpsipt(nullptr), fHistjpsieta(nullptr), fHistjpsiphi(nullptr), fHistjpsinmoms(nullptr), fHistjpsimompdg1(nullptr), fHistjpsimompdg2(nullptr), fHistjpsinsons(nullptr), fHistjpsisonpdg1(nullptr), fHistjpsisonpdg2(nullptr),
  fHistgoodjpsimult(nullptr), fHistgoodjpsigenstat(nullptr), fHistgoodjpsipt(nullptr), fHistgoodjpsieta(nullptr), fHistgoodjpsiphi(nullptr), fHistgoodjpsinmoms(nullptr), fHistgoodjpsimompdg1(nullptr), fHistgoodjpsimompdg2(nullptr), fHistgoodjpsinsons(nullptr), fHistgoodjpsisonpdg1(nullptr), fHistgoodjpsisonpdg2(nullptr),
  fHistFSelnum(nullptr), fHistFSelpt(nullptr), fHistFSeleta(nullptr), fHistFSelphi(nullptr), fHistFSleadelpt(nullptr), fHistFSleadeleta(nullptr), fHistFSleadelphi(nullptr), fHistFSsubleadelpt(nullptr), fHistFSsubleadeleta(nullptr), fHistFSsubleadelphi(nullptr), fHistFSel1el2mass(nullptr), fHistFSel1el2deta(nullptr), fHistFSel1el2dphi(nullptr), fHistFSel1el2dr(nullptr),
  fHistFSjpsisonelnum(nullptr), fHistFSjpsisonelpt(nullptr), fHistFSjpsisoneleta(nullptr), fHistFSjpsisonelphi(nullptr), fHistFSjpsisonleadelpt(nullptr), fHistFSjpsisonleadeleta(nullptr), fHistFSjpsisonleadelphi(nullptr), fHistFSjpsisonsubleadelpt(nullptr), fHistFSjpsisonsubleadeleta(nullptr), fHistFSjpsisonsubleadelphi(nullptr), fHistFSjpsisonel1el2mass(nullptr), fHistFSjpsisonel1el2deta(nullptr), fHistFSjpsisonel1el2dphi(nullptr), fHistFSjpsisonel1el2dr(nullptr),
  f2DHistGoodJPsiFSJpsisonleadelE(nullptr), f2DHistGoodJPsiFSJpsisonsubleadelE(nullptr), f2DHistGoodJPsiFSJpsisonleadelpT(nullptr), f2DHistGoodJPsiFSJpsisonsubleadelpT(nullptr), f2DHistGoodJPsiEFSJpsisonleadsubleadelDr(nullptr), f2DHistGoodJPsiEFSJpsisonleadsubleadelDeta(nullptr), f2DHistGoodJPsiEFSJpsisonleadsubleadelDphi(nullptr), f2DHistGoodJPsiEFSJpsisonleadsubleadelDpt(nullptr) {

  // actually, pset is NOT in use - we keep it here just for illustratory putposes
  usesResource(TFileService::kSharedResource);
  
  }

void JPsiToEEGenAnalyzer::beginJob() {
  edm::Service<TFileService> fs;
  fHistjpsimult = fs->make<TH1D>("Hist_jpsi_mult", "J/#psi multiplicity", 100, 0, 100);
  fHistjpsigenstat = fs->make<TH1D>("Hist_jpsi_gen_status", "J/#psi generator status", 20000, -10000, 10000);
  fHistjpsipt = fs->make<TH1D>("Hist_jpsi_pt", "J/#psi pt", 1000, 0.0, 100.0);
  fHistjpsieta = fs->make<TH1D>("Hist_jpsi_eta", "J/#psi eta", 1000, -5.0, 5.0);
  fHistjpsiphi = fs->make<TH1D>("Hist_jpsi_phi", "J/#psi phi", 6600, -3.3, 3.3);
  fHistjpsinmoms = fs->make<TH1D>("Hist_jpsi_nmoms", "J/#psi no.of mothers", 10,0,10);
  fHistjpsimompdg1 = fs->make<TH1D>("Hist_jpsi_mom_pdg1", "J/#psi pdg of mother1", 20000, -10000, 10000);
  fHistjpsimompdg2 = fs->make<TH1D>("Hist_jpsi_mom_pdg2", "J/#psi pdg of mother2", 20000, -10000, 10000);
  fHistjpsinsons = fs->make<TH1D>("Hist_jpsi_nsons", "J/#psi no.of sons", 10,0,10);
  fHistjpsisonpdg1 = fs->make<TH1D>("Hist_jpsi_son_pdg1", "J/#psi pdg of mother1", 20000, -10000, 10000);
  fHistjpsisonpdg2 = fs->make<TH1D>("Hist_jpsi_son_pdg2", "J/#psi pdg of mother2", 20000, -10000, 10000);
  fHistgoodjpsimult = fs->make<TH1D>("Hist_goodjpsi_mult", "J/#psi multiplicity", 100, 0, 100);
  fHistgoodjpsigenstat = fs->make<TH1D>("Hist_goodjpsi_gen_status", "J/#psi generator status", 20000, -10000, 10000);
  fHistgoodjpsipt = fs->make<TH1D>("Hist_goodjpsi_pt", "J/#psi pt", 1000, 0.0, 100.0);
  fHistgoodjpsieta = fs->make<TH1D>("Hist_goodjpsi_eta", "J/#psi eta", 1000, -5.0, 5.0);
  fHistgoodjpsiphi = fs->make<TH1D>("Hist_goodjpsi_phi", "J/#psi phi", 6600, -3.3, 3.3);
  fHistgoodjpsinmoms = fs->make<TH1D>("Hist_goodjpsi_nmoms", "J/#psi no.of mothers", 10,0,10);
  fHistgoodjpsimompdg1 = fs->make<TH1D>("Hist_goodjpsi_mom_pdg1", "J/#psi pdg of mother1", 20000, -10000, 10000);
  fHistgoodjpsimompdg2 = fs->make<TH1D>("Hist_goodjpsi_mom_pdg2", "J/#psi pdg of mother2", 20000, -10000, 10000);
  fHistgoodjpsinsons = fs->make<TH1D>("Hist_goodjpsi_nsons", "J/#psi no.of sons", 10,0,10);
  fHistgoodjpsisonpdg1 = fs->make<TH1D>("Hist_goodjpsi_son_pdg1", "J/#psi pdg of mother1", 20000, -10000, 10000);
  fHistgoodjpsisonpdg2 = fs->make<TH1D>("Hist_goodjpsi_son_pdg2", "J/#psi pdg of mother2", 20000, -10000, 10000);
  fHistFSelnum = fs->make<TH1D>("Hist_FSel_num", "final state electron multiplicity", 100, 0, 100.);
  fHistFSelpt = fs->make<TH1D>("Hist_FSel_pt", "final state electron pt", 1000, 0., 100.);
  fHistFSeleta = fs->make<TH1D>("Hist_FSel_eta", "final state electron eta", 1000, -5., 5.);
  fHistFSelphi = fs->make<TH1D>("Hist_FSel_phi", "final state electron phi", 1000, -5., 5.);
  fHistFSleadelpt = fs->make<TH1D>("Hist_FSel_lead_pt", "final state leading electron pt", 1000, 0., 100.);
  fHistFSleadeleta = fs->make<TH1D>("Hist_FSel_lead_eta", "final state leading electron eta", 1000, -5., 5.);
  fHistFSleadelphi = fs->make<TH1D>("Hist_FSel_lead_phi", "final state leading electron phi", 1000, -5., 5.);
  fHistFSsubleadelpt = fs->make<TH1D>("Hist_FSel_sublead_pt", "final state subleading electron pt", 1000, 0., 100.);
  fHistFSsubleadeleta = fs->make<TH1D>("Hist_FSel_sublead_eta", "final state subleading electron eta", 1000, -5., 5.);
  fHistFSsubleadelphi = fs->make<TH1D>("Hist_FSel_sublead_phi", "final state subleading electron phi", 1000, -5., 5.);
  fHistFSel1el2mass = fs->make<TH1D>("Hist_FSel_e1e2Mass", "final state M(e_{1},e_{2})", 10000, 0, 100.);
  fHistFSel1el2deta = fs->make<TH1D>("Hist_FSel_e1e2deta", "final state #Delta#eta(e_{1},e_{2})", 10000, -5., 5.);
  fHistFSel1el2dphi = fs->make<TH1D>("Hist_FSel_e1e2dphi", "final state #Delta#phi(e_{1},e_{2})", 10000, -5., 5.);
  fHistFSel1el2dr = fs->make<TH1D>("Hist_FSel_e1e2dr", "final state #DeltaR(e_{1},e_{2})", 10000, 0., 10.);
  fHistFSjpsisonelnum = fs->make<TH1D>("Hist_FSjpsisonel_num", "final state electron multiplicity", 100, 0, 100.);
  fHistFSjpsisonelpt = fs->make<TH1D>("Hist_FSjpsisonel_pt", "final state electron pt", 1000, 0., 100.);
  fHistFSjpsisoneleta = fs->make<TH1D>("Hist_FSjpsisonel_eta", "final state electron eta", 1000, -5., 5.);
  fHistFSjpsisonelphi = fs->make<TH1D>("Hist_FSjpsisonel_phi", "final state electron phi", 1000, -5., 5.);
  fHistFSjpsisonleadelpt = fs->make<TH1D>("Hist_FSjpsisonel_lead_pt", "final state leading electron pt", 1000, 0., 100.);
  fHistFSjpsisonleadeleta = fs->make<TH1D>("Hist_FSjpsisonel_lead_eta", "final state leading electron eta", 1000, -5., 5.);
  fHistFSjpsisonleadelphi = fs->make<TH1D>("Hist_FSjpsisonel_lead_phi", "final state leading electron phi", 1000, -5., 5.);
  fHistFSjpsisonsubleadelpt = fs->make<TH1D>("Hist_FSjpsisonel_sublead_pt", "final state subleading electron pt", 1000, 0., 100.);
  fHistFSjpsisonsubleadeleta = fs->make<TH1D>("Hist_FSjpsisonel_sublead_eta", "final state subleading electron eta", 1000, -5., 5.);
  fHistFSjpsisonsubleadelphi = fs->make<TH1D>("Hist_FSjpsisonel_sublead_phi", "final state subleading electron phi", 1000, -5., 5.);
  fHistFSjpsisonel1el2mass = fs->make<TH1D>("Hist_FSjpsisonel_e1e2Mass", "final state M(e_{1},e_{2})", 10000, 0, 100.);
  fHistFSjpsisonel1el2deta = fs->make<TH1D>("Hist_FSjpsisonel_e1e2deta", "final state #Delta#eta(e_{1},e_{2})", 10000, -5., 5.);
  fHistFSjpsisonel1el2dphi = fs->make<TH1D>("Hist_FSjpsisonel_e1e2dphi", "final state #Delta#phi(e_{1},e_{2})", 10000, -5., 5.);
  fHistFSjpsisonel1el2dr = fs->make<TH1D>("Hist_FSjpsisonel_e1e2dr", "final state #DeltaR(e_{1},e_{2})", 10000, 0., 10.);
  f2DHistGoodJPsiFSJpsisonleadelE = fs->make<TH2D>("Hist2D_Goodjpsi_FSjpsisonleadel_E", "E_{J/#psi}:E_{e_{1}(J/#psi son)}", 1000, 0., 100., 1000, 0., 100.);
  f2DHistGoodJPsiFSJpsisonsubleadelE = fs->make<TH2D>("Hist2D_Goodjpsi_FSjpsisonsubleadel_E", "E_{J/#psi}:E_{e_{2}(J/#psi son)}", 1000, 0., 100., 1000, 0., 100.);
  f2DHistGoodJPsiFSJpsisonleadelpT = fs->make<TH2D>("Hist2D_Goodjpsi_FSjpsisonleadel_pT", "pT_{J/#psi}:pT_{e_{1}(J/#psi son)}", 1000, 0., 100., 1000, 0., 100.);
  f2DHistGoodJPsiFSJpsisonsubleadelpT = fs->make<TH2D>("Hist2D_Goodjpsi_FSjpsisonsubleadel_pT", "pT_{J/#psi}:pT_{e_{2}(J/#psi son)}", 1000, 0., 100., 1000, 0., 100.);
  f2DHistGoodJPsiEFSJpsisonleadsubleadelDr = fs->make<TH2D>("Hist2D_Goodjpsi_E_FSjpsisonleadsubleadel_Dr", "E_{J/#psi}:#DeltaR_{e_{1}(J/#psi son), e_{2}(J/#psi son)}", 1000, 0., 100., 10000, 0., 10.);
  f2DHistGoodJPsiEFSJpsisonleadsubleadelDeta = fs->make<TH2D>("Hist2D_Goodjpsi_E_FSjpsisonleadsubleadel_Deta", "E_{J/#psi}:#Delta#eta_{e_{1}(J/#psi son), e_{2}(J/#psi son)}", 1000, 0., 100., 10000, -5., 5.);
  f2DHistGoodJPsiEFSJpsisonleadsubleadelDphi = fs->make<TH2D>("Hist2D_Goodjpsi_E_FSjpsisonleadsubleadel_Dphi", "E_{J/#psi}:#Delta#phi_{e_{1}(J/#psi son), e_{2}(J/#psi son)}", 1000, 0., 100., 10000, -5., 5.);
  f2DHistGoodJPsiEFSJpsisonleadsubleadelDpt = fs->make<TH2D>("Hist2D_Goodjpsi_E_FSjpsisonleadsubleadel_Dpt", "E_{J/#psi}:#DeltapT_{e_{1}(J/#psi son), e_{2}(J/#psi son)}", 1000, 0., 100., 10000, 0., 100.);
}

void JPsiToEEGenAnalyzer::analyze(const edm::Event& e, const edm::EventSetup&) {
  // here's an example of accessing GenEventInfoProduct
  const edm::Handle<GenEventInfoProduct>& GenInfoHandle = e.getHandle(tokenGenInfo_);
  //double qScale = GenInfoHandle->qScale();
  //double pthat = (GenInfoHandle->hasBinningValues() ? (GenInfoHandle->binningValues())[0] : 0.0);
  //std::cout << " qScale = " << qScale << " pthat = " << pthat << std::endl;

  edm::Handle<std::vector<reco::GenParticle>> genPartH;
  e.getByToken(tokenGenPart_, genPartH);
  bool genPartValid = genPartH.isValid();

  int genJpsicnt = 0, genGoodJpsicnt = 0;
  int genElFScnt = 0, genElFSjpsisoncnt = 0;
  reco::GenParticle *leadFSel, *subleadFSel;
  reco::GenParticle *oneFSjpsisonel, *twoFSjpsisonel;

  if(genPartValid) {
    
    // Loop over genParticle collection
    for(auto gen_iter=genPartH->begin(); gen_iter!=genPartH->end(); ++gen_iter) {
      
      // Condition for JPsi 1S particles
      if(abs(gen_iter->pdgId())==443) {
	genJpsicnt++;
	fHistjpsipt->Fill(gen_iter->pt());
	fHistjpsieta->Fill(gen_iter->eta());
	fHistjpsiphi->Fill(gen_iter->phi());
	fHistjpsinmoms->Fill(gen_iter->numberOfMothers());
	fHistjpsimompdg1->Fill(gen_iter->mother(0)->pdgId());
	if(gen_iter->numberOfMothers()>=2) fHistjpsimompdg2->Fill(gen_iter->mother(1)->pdgId());
	fHistjpsinsons->Fill(gen_iter->numberOfDaughters());
	fHistjpsisonpdg1->Fill(gen_iter->daughter(0)->pdgId());
	if(gen_iter->numberOfDaughters()>=2) fHistjpsisonpdg2->Fill(gen_iter->daughter(1)->pdgId());
	
	if( gen_iter->numberOfDaughters()==2 && (gen_iter->daughter(0)->pdgId()*gen_iter->daughter(1)->pdgId())==-121) { // Condition for a JPsi to be good.
	  genGoodJpsicnt++;
	  fHistgoodjpsipt->Fill(gen_iter->pt());
	  fHistgoodjpsieta->Fill(gen_iter->eta());
	  fHistgoodjpsiphi->Fill(gen_iter->phi());
	  fHistgoodjpsinmoms->Fill(gen_iter->numberOfMothers());
	  fHistgoodjpsimompdg1->Fill(gen_iter->mother(0)->pdgId());
	  if(gen_iter->numberOfMothers()>=2) fHistgoodjpsimompdg2->Fill(gen_iter->mother(1)->pdgId());
	  fHistgoodjpsinsons->Fill(gen_iter->numberOfDaughters());
	  fHistgoodjpsisonpdg1->Fill(gen_iter->daughter(0)->pdgId());
	  fHistgoodjpsisonpdg2->Fill(gen_iter->daughter(1)->pdgId());

	  oneFSjpsisonel = gen_iter->daughterRef(0)->clone(); // Get the daughter of the JPsi
	  twoFSjpsisonel = gen_iter->daughterRef(1)->clone();
	  oneFSjpsisonel = findLastCopyDaughter(oneFSjpsisonel); // Obtain the last copy of the particle
	  twoFSjpsisonel = findLastCopyDaughter(twoFSjpsisonel);
	  genElFSjpsisoncnt += 2;
	  fHistFSjpsisonelpt->Fill(oneFSjpsisonel->pt());
	  fHistFSjpsisonelpt->Fill(twoFSjpsisonel->pt());
	  fHistFSjpsisoneleta->Fill(oneFSjpsisonel->eta());
	  fHistFSjpsisoneleta->Fill(twoFSjpsisonel->eta());
	  fHistFSjpsisonelphi->Fill(oneFSjpsisonel->phi());
	  fHistFSjpsisonelphi->Fill(twoFSjpsisonel->phi());
	  TLorentzVector el1, el2; // Sort the daughter of Jpsi to leading and sub-leading particles
	  if(oneFSjpsisonel->pt()>twoFSjpsisonel->pt()) {
	    el1.SetPtEtaPhiM(oneFSjpsisonel->pt(), oneFSjpsisonel->eta(), oneFSjpsisonel->phi(), oneFSjpsisonel->mass());
	    el2.SetPtEtaPhiM(twoFSjpsisonel->pt(), twoFSjpsisonel->eta(), twoFSjpsisonel->phi(), twoFSjpsisonel->mass());
	  }
	  else {
	    el2.SetPtEtaPhiM(oneFSjpsisonel->pt(), oneFSjpsisonel->eta(), oneFSjpsisonel->phi(), oneFSjpsisonel->mass());
	    el1.SetPtEtaPhiM(twoFSjpsisonel->pt(), twoFSjpsisonel->eta(), twoFSjpsisonel->phi(), twoFSjpsisonel->mass());
	  }
	  fHistFSjpsisonleadelpt->Fill(el1.Pt());
	  fHistFSjpsisonleadeleta->Fill(el1.Eta());
	  fHistFSjpsisonleadelphi->Fill(el1.Phi());
	  fHistFSjpsisonsubleadelpt->Fill(el2.Pt());
	  fHistFSjpsisonsubleadeleta->Fill(el2.Eta());
	  fHistFSjpsisonsubleadelphi->Fill(el2.Phi());
	  fHistFSjpsisonel1el2mass->Fill((el1+el2).M());
	  fHistFSjpsisonel1el2deta->Fill(el1.Eta()-el2.Eta());
	  fHistFSjpsisonel1el2dphi->Fill(el1.DeltaPhi(el2));
	  fHistFSjpsisonel1el2dr->Fill(el1.DeltaR(el2));
	  f2DHistGoodJPsiFSJpsisonleadelE->Fill(gen_iter->energy(), el1.Energy());
	  f2DHistGoodJPsiFSJpsisonsubleadelE->Fill(gen_iter->energy(), el2.Energy());
	  f2DHistGoodJPsiFSJpsisonleadelpT->Fill(gen_iter->energy(), el1.Pt());
	  f2DHistGoodJPsiFSJpsisonsubleadelpT->Fill(gen_iter->energy(), el2.Pt());
	  f2DHistGoodJPsiEFSJpsisonleadsubleadelDr->Fill(gen_iter->energy(), el1.DeltaR(el2));
	  f2DHistGoodJPsiEFSJpsisonleadsubleadelDeta->Fill(gen_iter->energy(), el1.Eta()-el2.Eta());
	  f2DHistGoodJPsiEFSJpsisonleadsubleadelDphi->Fill(gen_iter->energy(), el1.DeltaPhi(el2));
	  f2DHistGoodJPsiEFSJpsisonleadsubleadelDpt->Fill(gen_iter->energy(), el1.Pt()-el2.Pt());
	} // End of the good Jpsi condition
      }// End of JPsi 1S gen particle condition

      // Condition over FS electron
      if(abs(gen_iter->pdgId())==11 && gen_iter->isLastCopy()) {
	genElFScnt++;
	fHistFSelpt->Fill(gen_iter->pt());
	fHistFSeleta->Fill(gen_iter->eta());
	fHistFSelphi->Fill(gen_iter->phi());
	if(genElFScnt==1) {
	  leadFSel = gen_iter->clone();
	}
	else if(genElFScnt==2) {
	  if(leadFSel->pt()>gen_iter->pt()) {
	    subleadFSel = gen_iter->clone();
	  }
	  else {
	    subleadFSel = leadFSel;
	    leadFSel = gen_iter->clone();
	  }
	}
	else {
	  if(gen_iter->pt()>leadFSel->pt()) {
	    subleadFSel = leadFSel;
	    leadFSel = gen_iter->clone();
	  }
	  else {
	    if(gen_iter->pt()>subleadFSel->pt()) {
	      subleadFSel = gen_iter->clone();
	    }
	  }
	}
      } // End of condition over FS electron
      
    } // End of loop over genParticle collection
  }
  fHistjpsimult->Fill(genJpsicnt);
  fHistgoodjpsimult->Fill(genGoodJpsicnt);
  fHistFSelnum->Fill(genElFScnt);
  if(genElFScnt>=1) {
    fHistFSleadelpt->Fill(leadFSel->pt());
    fHistFSleadeleta->Fill(leadFSel->eta());
    fHistFSleadelphi->Fill(leadFSel->phi());
  }
  if(genElFScnt>=2) {
    fHistFSsubleadelpt->Fill(subleadFSel->pt());
    fHistFSsubleadeleta->Fill(subleadFSel->eta());
    fHistFSsubleadelphi->Fill(subleadFSel->phi());
    TLorentzVector el1, el2;
    el1.SetPtEtaPhiM(leadFSel->pt(), leadFSel->eta(), leadFSel->phi(), leadFSel->mass());
    el2.SetPtEtaPhiM(subleadFSel->pt(), subleadFSel->eta(), subleadFSel->phi(), subleadFSel->mass());
    fHistFSel1el2mass->Fill((el1+el2).M());
    fHistFSel1el2deta->Fill(leadFSel->eta()-subleadFSel->eta());
    fHistFSel1el2dphi->Fill(el1.DeltaPhi(el2));
    fHistFSel1el2dr->Fill(el1.DeltaR(el2));
  }
  fHistFSjpsisonelnum->Fill(genElFSjpsisoncnt);
  
  return;
}

reco::GenParticle* JPsiToEEGenAnalyzer::findLastCopyDaughter(reco::GenParticle *son) {  
  while(!son->isLastCopy()) { // Loop until you find the last copy
    for(unsigned int ctr=0; ctr<son->numberOfDaughters(); ctr++) {
      if(son->pdgId()==son->daughterRef(ctr)->pdgId()) { // PdgId of particle shouldn't change
	son = (reco::GenParticle*) son->daughterRef(ctr)->clone();
	break;
      }
    } // end of for
  }
  return son;
}

typedef JPsiToEEGenAnalyzer JPsiToEEGenExampleAnalyzer;
DEFINE_FWK_MODULE(JPsiToEEGenExampleAnalyzer);
