#include <TChain.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TLorentzVector.h>
#include <TStopwatch.h>
#include <TTimeStamp.h>
#include <TString.h>
#include <TLegend.h>
#include <THStack.h>
#include <TPad.h>
#include <TCanvas.h>
#include <TColor.h>
#include <TAttMarker.h>
#include <TF1.h>
#include <TStyle.h>

#include <vector>

//Customized header files
#include <Lepton.h>
#include <ControlPlots.h>

void AssignAccThreshold(TString HLTname, TString *HLT, Double_t *LeadPtCut, Double_t *SubPtCut, Double_t *LeadEtaCut, Double_t *SubEtaCut);
void CompareMuon(Muon *Mu1, Muon *Mu2, Muon *leadMu, Muon *subMu);
void CompareGenLepton(GenLepton *genlep1, GenLepton *genlep2, GenLepton *leadgenlep, GenLepton *subgenlep);
void GenMatching(TString HLTname, TString MuonType, NtupleHandle* ntuple, vector<Muon>* MuonCollection);
Bool_t isPassDimuonVertexCut(NtupleHandle *ntuple, Double_t Pt1, Double_t Pt2);
static inline void loadBar(int x, int n, int r, int w);
void MuonPlots(TString HLTname = "IsoMu20")
{
	TTimeStamp ts_start;
	cout << "[Start Time(local time): " << ts_start.AsString("l") << "]" << endl;

	TStopwatch totaltime;
	totaltime.Start();
	
	TString Cut;
	TString HLT;
	Double_t LeadPtCut = 9999;
	Double_t SubPtCut = 9999;
	Double_t LeadEtaCut = 9999;
	Double_t SubEtaCut = 9999;
	AssignAccThreshold(HLTname, &HLT, &LeadPtCut, &SubPtCut, &LeadEtaCut, &SubEtaCut);
	cout << "===========================================================" << endl;
	cout << "Trigger: " << HLT << endl;
	cout << "leading lepton pT Cut: " << LeadPtCut << endl;
	cout << "Sub-leading lepton pT Cut: " << SubPtCut << endl;
	cout << "leading lepton Eta Cut: " << LeadEtaCut << endl;
	cout << "sub-leading lepton Eta Cut: " << SubEtaCut << endl;
	cout << "===========================================================" << endl;

	Double_t Factor = 569.0171*2008.4*3/4.5275/10;

	//TFile *f = new TFile("ROOTFile_Histogram_InvMass_"+HLTname+"_Data.root", "RECREATE");
	//TFile *f = new TFile("ROOTFile_Histogram_InvMass_60to120_Data.root", "RECREATE");
	//TFile *fcut = new TFile("MuonCut.root", "RECREATE");
	//TFile *fallcut = new TFile("MuonCutAll.root", "RECREATE");

	TString BaseLocation = "/data4/Users/kplee/DYntuple";
	//Each ntuple directory & corresponding Tags
	vector< TString > ntupleDirectory; vector< TString > Tag;
	//MC
	// ntupleDirectory.push_back( "Spring15DR/25ns/DYLL_M10to50_v2_AddTauInfo" ); Tag.push_back( "DYMuMu_M20to50" );
	ntupleDirectory.push_back( "Spring15DR/25ns/DYLL_M50_v1" ); Tag.push_back( "DYMuMu" );
	// ntupleDirectory.push_back( "Spring15DR/25ns/DYLL_M10to50_v2_AddTauInfo" ); Tag.push_back( "DYTauTau_M20to50" );
	// ntupleDirectory.push_back( "Spring15DR/25ns/DYLL_M50_v1" ); Tag.push_back( "DYTauTau" );
	// ntupleDirectory.push_back( "Spring15DR/25ns/ttbar_v1" );  Tag.push_back( "ttbar" );
	// ntupleDirectory.push_back( "Spring15DR/25ns/WJets_v1" );  Tag.push_back( "WJets" );
	// ntupleDirectory.push_back( "Spring15DR/25ns/WW_v1" );  Tag.push_back( "WW" );
	// ntupleDirectory.push_back( "Spring15DR/25ns/WZ_v1" );  Tag.push_back( "WZ" );
	// ntupleDirectory.push_back( "Spring15DR/25ns/ZZ_v1" );  Tag.push_back( "ZZ" );

	//Data
	ntupleDirectory.push_back( "Run2015C/GoldenJSON/SingleMuon_v3_Run246908to256869" ); Tag.push_back( "Data" ); // -- Run2015C -- //

	vector< TString > CutName;
	CutName.push_back( "isGLB" );
	CutName.push_back( "isPF" );
	CutName.push_back( "chi2dof" );
	CutName.push_back( "muonHits" );
	CutName.push_back( "nMatches" );
	CutName.push_back( "dxyVTX" );
	CutName.push_back( "dzVTX" );
	CutName.push_back( "pixelHits" );
	CutName.push_back( "trackerLayers" );
	CutName.push_back( "trkiso" );

for(Int_t i_cut = 0; i_cut < 10; i_cut++)
{
	Cut = CutName[i_cut];
	TFile *f = new TFile("MuonTight_"+Cut+".root", "RECREATE");
	cout << "TightMuon minus " << Cut << endl;
	//Loop for all samples
	const Int_t Ntup = ntupleDirectory.size();
	for(Int_t i_tup = 0; i_tup<Ntup; i_tup++)
	{
		TStopwatch looptime;
		looptime.Start();

		cout << "\t<" << Tag[i_tup] << ">" << endl;

		//ControlPlots *Plots = new ControlPlots( Tag[i_tup] );
		//Histograms for cuts
/*
 *        TH1D *h_isGLB_Pt = new TH1D("h_isGLB_Pt"+Tag[i_tup], "", 250, 0, 500);
 *        TH1D *h_isGLB_eta = new TH1D("h_isGLB_eta"+Tag[i_tup], "", 60, -3, 3);
 *        TH1D *h_isGLB_phi = new TH1D("h_isGLB_phi"+Tag[i_tup], "", 80, -4, 4);
 *        TH1D *h_isPF_Pt = new TH1D("h_isPF_Pt"+Tag[i_tup], "", 250, 0, 500);
 *        TH1D *h_isPF_eta = new TH1D("h_isPF_eta"+Tag[i_tup], "", 60, -3, 3);
 *        TH1D *h_isPF_phi = new TH1D("h_isPF_phi"+Tag[i_tup], "", 80, -4, 4);
 *        TH1D *h_chi2dof_Pt = new TH1D("h_chi2dof_Pt"+Tag[i_tup], "", 250, 0, 500);
 *        TH1D *h_chi2dof_eta = new TH1D("h_chi2dof_eta"+Tag[i_tup], "", 60, -3, 3);
 *        TH1D *h_chi2dof_phi = new TH1D("h_chi2dof_phi"+Tag[i_tup], "", 80, -4, 4);
 *        TH1D *h_muonHits_Pt = new TH1D("h_muonHits_Pt"+Tag[i_tup], "", 250, 0, 500);
 *        TH1D *h_muonHits_eta = new TH1D("h_muonHits_eta"+Tag[i_tup], "", 60, -3, 3);
 *        TH1D *h_muonHits_phi = new TH1D("h_muonHits_phi"+Tag[i_tup], "", 80, -4, 4);
 *        TH1D *h_nMatches_Pt = new TH1D("h_nMatches_Pt"+Tag[i_tup], "", 250, 0, 500);
 *        TH1D *h_nMatches_eta = new TH1D("h_nMatches_eta"+Tag[i_tup], "", 60, -3, 3);
 *        TH1D *h_nMatches_phi = new TH1D("h_nMatches_phi"+Tag[i_tup], "", 80, -4, 4);
 *        TH1D *h_dxyVTX_Pt = new TH1D("h_dxyVTX_Pt"+Tag[i_tup], "", 250, 0, 500);
 *        TH1D *h_dxyVTX_eta = new TH1D("h_dxyVTX_eta"+Tag[i_tup], "", 60, -3, 3);
 *        TH1D *h_dxyVTX_phi = new TH1D("h_dxyVTX_phi"+Tag[i_tup], "", 80, -4, 4);
 *        TH1D *h_dzVTX_Pt = new TH1D("h_dzVTX_Pt"+Tag[i_tup], "", 250, 0, 500);
 *        TH1D *h_dzVTX_eta = new TH1D("h_dzVTX_eta"+Tag[i_tup], "", 60, -3, 3);
 *        TH1D *h_dzVTX_phi = new TH1D("h_dzVTX_phi"+Tag[i_tup], "", 80, -4, 4);
 *        TH1D *h_pixelHits_Pt = new TH1D("h_pixelHits_Pt"+Tag[i_tup], "", 250, 0, 500);
 *        TH1D *h_pixelHits_eta = new TH1D("h_pixelHits_eta"+Tag[i_tup], "", 60, -3, 3);
 *        TH1D *h_pixelHits_phi = new TH1D("h_pixelHits_phi"+Tag[i_tup], "", 80, -4, 4);
 *        TH1D *h_trackerLayers_Pt = new TH1D("h_trackerLayers_Pt"+Tag[i_tup], "", 250, 0, 500);
 *        TH1D *h_trackerLayers_eta = new TH1D("h_trackerLayers_eta"+Tag[i_tup], "", 60, -3, 3);
 *        TH1D *h_trackerLayers_phi = new TH1D("h_trackerLayers_phi"+Tag[i_tup], "", 80, -4, 4);
 *
 *        TH1D *h_trkiso_Pt = new TH1D("h_trkiso_Pt"+Tag[i_tup], "", 250, 0, 500);
 *        TH1D *h_trkiso_eta = new TH1D("h_trkiso_eta"+Tag[i_tup], "", 60, -3, 3);
 *        TH1D *h_trkiso_phi = new TH1D("h_trkiso_phi"+Tag[i_tup], "", 80, -4, 4);
 */
		//TH1D *h_cut_Pt = new TH1D("h_cut_Pt"+Tag[i_tup], "", 250, 0, 500);
		//TH1D *h_cut_eta = new TH1D("h_cut_eta"+Tag[i_tup], "", 60, -3, 3);
		//TH1D *h_cut_phi = new TH1D("h_cut_phi"+Tag[i_tup], "", 80, -4, 4);



		TChain *chain = new TChain("recoTree/DYTree");
		chain->Add(BaseLocation+"/"+ntupleDirectory[i_tup]+"/ntuple_*.root");
		if( Tag[i_tup] == "Data" )
		{
			// -- Run2015D -- //
			chain->Add("/data4/Users/kplee/DYntuple/Run2015D/GoldenJSON/v20151016_v3JSON_Run2015D_SingleMuon_Run246908to256869/*.root");
			chain->Add("/data4/Users/kplee/DYntuple/Run2015D/GoldenJSON/v20151010_v2JSON_SingleMuon_Run256870to257599/*.root");
			chain->Add("/data4/Users/kplee/DYntuple/Run2015D/GoldenJSON/v20151009_SingleMuon_Run257600toRun258159/*.root");
		}

		NtupleHandle *ntuple = new NtupleHandle( chain );

		Bool_t isNLO = 0;
		if( Tag[i_tup] == "DYMuMu" || Tag[i_tup] == "DYTauTau" || Tag[i_tup] == "WJets" || Tag[i_tup] == "DYMuMu_M20to50" || Tag[i_tup] == "DYTauTau_M20to50" )
		{
			isNLO = 1;
			cout << "\t" << Tag[i_tup] << ": generated with NLO mode - Weights are applied" << endl;
		}

		Int_t count_Zpeak = 0;
		Double_t SumWeight = 0;
		Double_t SumWeight_DYMuMu_M20to50 = 0;
		//Double_t SumWeight_DYTauTau_M20to50 = 0;

		Int_t NEvents = chain->GetEntries();
		cout << "\t[Total Events: " << NEvents << "]" << endl;
		for(Int_t i=0; i<NEvents; i++)
		{
			loadBar(i+1, NEvents, 100, 100);

			ntuple->GetEvent(i);

			//Bring weights for NLO MC events
			Double_t GenWeight;
			if( isNLO == 1 )
				GenWeight = ntuple->GENEvt_weight;
			else
				GenWeight = 1;
			// cout << "Weight of " << i << "th Event: " << GenWeight << endl;
			SumWeight += GenWeight;

			Int_t GenMassFlag = -1;
			//Take the events within 20<M<50 in gen-level
/*
 *            if( Tag[i_tup] == "DYMuMu_M20to50" )
 *            {
 *                GenMassFlag = 0;
 *                vector<GenLepton> GenLeptonCollection;
 *                Int_t NGenLeptons = ntuple->gnpair;
 *                for(Int_t i_gen=0; i_gen<NGenLeptons; i_gen++)
 *                {
 *                    GenLepton genlep;
 *                    genlep.FillFromNtuple(ntuple, i_gen);
 *                    if( genlep.isMuon() && genlep.fromHardProcessFinalState )
 *                        GenLeptonCollection.push_back( genlep );
 *                }
 *
 *                if( GenLeptonCollection.size() == 2 )
 *                {
 *                    GenLepton genlep1 = GenLeptonCollection[0];
 *                    GenLepton genlep2 = GenLeptonCollection[1];
 *
 *                    TLorentzVector gen_v1 = genlep1.Momentum;
 *                    TLorentzVector gen_v2 = genlep2.Momentum;
 *                    TLorentzVector gen_vtot = gen_v1 + gen_v2;
 *                    Double_t gen_M = gen_vtot.M();
 *
 *                    if( gen_M > 20 && gen_M < 50 ) //Take the events within 20<M<50 in gen-level
 *                    {
 *                        GenMassFlag = 1;
 *                        SumWeight_DYMuMu_M20to50 += GenWeight;
 *                    }
 *                }
 *            }
 *            else if( Tag[i_tup] == "DYTauTau_M20to50" )
 *            {
 *                GenMassFlag = 0;
 *                vector<GenLepton> GenLeptonCollection;
 *                Int_t NGenLeptons = ntuple->gnpair;
 *                for(Int_t i_gen=0; i_gen<NGenLeptons; i_gen++)
 *                {
 *                    GenLepton genlep;
 *                    genlep.FillFromNtuple(ntuple, i_gen);
 *                    if( abs(genlep.ID) == 15 && genlep.fromHardProcessDecayed )
 *                        GenLeptonCollection.push_back( genlep );
 *                }
 *
 *                if( GenLeptonCollection.size() == 2 )
 *                {
 *                    GenLepton genlep1 = GenLeptonCollection[0];
 *                    GenLepton genlep2 = GenLeptonCollection[1];
 *
 *                    TLorentzVector gen_v1 = genlep1.Momentum;
 *                    TLorentzVector gen_v2 = genlep2.Momentum;
 *                    TLorentzVector gen_vtot = gen_v1 + gen_v2;
 *                    Double_t gen_M = gen_vtot.M();
 *
 *                    if( gen_M > 20 && gen_M < 50 ) //Take the events within 20<M<50 in gen-level
 *                    {
 *                        GenMassFlag = 1;
 *                        SumWeight_DYTauTau_M20to50 += GenWeight;
 *                    }
 *                }
 *            }
 *            else
 */
				GenMassFlag = 1; // -- other cases: pass



			if( ntuple->isTriggered( HLT ) && GenMassFlag)
			{
				//Collect Reconstruction level information
				vector< Muon > MuonCollection;
				Int_t NLeptons = ntuple->nMuon;
				for(Int_t i_reco=0; i_reco<NLeptons; i_reco++)
				{
					Muon mu;
					mu.FillFromNtuple(ntuple, i_reco);
						MuonCollection.push_back( mu );
				}

				//Select muons directly from Z/gamma by matching with gen-level final state muons from hard process
				if( Tag[i_tup] == "DYMuMu" )
					GenMatching(HLTname, "fromHardProcess", ntuple, &MuonCollection);
/*
 *                //Select muons directly from tau by matching with gen-level final state muons from prompt tau
 *                else if( Tag[i_tup] == "DYTauTau" )
 *                    GenMatching(HLTname, "fromTau", ntuple, &MuonCollection);
 *
 */

				//Collect qualified muons among muons
				vector< Muon > QMuonCollection;
				for(Int_t j=0; j<(int)MuonCollection.size(); j++)
				{
/*
 *                    if( MuonCollection[j].isGLB == 1)
 *                    {
 *                        h_isGLB_Pt->Fill( MuonCollection[j].Pt, GenWeight );
 *                        h_isGLB_eta->Fill( MuonCollection[j].eta, GenWeight);
 *                        h_isGLB_phi->Fill( MuonCollection[j].phi, GenWeight);
 *                    }
 *                    if( MuonCollection[j].isPF == 1)
 *                    {
 *                        h_isPF_Pt->Fill( MuonCollection[j].Pt, GenWeight );
 *                        h_isPF_eta->Fill( MuonCollection[j].eta, GenWeight);
 *                        h_isPF_phi->Fill( MuonCollection[j].phi, GenWeight);
 *                    }
 *                    if( MuonCollection[j].chi2dof < 10)
 *                    {
 *                        h_chi2dof_Pt->Fill( MuonCollection[j].Pt, GenWeight );
 *                        h_chi2dof_eta->Fill( MuonCollection[j].eta, GenWeight);
 *                        h_chi2dof_phi->Fill( MuonCollection[j].phi, GenWeight);
 *                    }
 *                    if( MuonCollection[j].muonHits > 0)
 *                    {
 *                        h_muonHits_Pt->Fill( MuonCollection[j].Pt, GenWeight );
 *                        h_muonHits_eta->Fill( MuonCollection[j].eta, GenWeight);
 *                        h_muonHits_phi->Fill( MuonCollection[j].phi, GenWeight);
 *                    }
 *                    if( MuonCollection[j].nMatches > 1)
 *                    {
 *                        h_nMatches_Pt->Fill( MuonCollection[j].Pt, GenWeight );
 *                        h_nMatches_eta->Fill( MuonCollection[j].eta, GenWeight);
 *                        h_nMatches_phi->Fill( MuonCollection[j].phi, GenWeight);
 *                    }
 *                    if( fabs(MuonCollection[j].dxyVTX) < 0.2)
 *                    {
 *                        h_dxyVTX_Pt->Fill( MuonCollection[j].Pt, GenWeight );
 *                        h_dxyVTX_eta->Fill( MuonCollection[j].eta, GenWeight);
 *                        h_dxyVTX_phi->Fill( MuonCollection[j].phi, GenWeight);
 *                    }
 *                    if( fabs(MuonCollection[j].dzVTX) < 0.5 )
 *                    {
 *                        h_dzVTX_Pt->Fill( MuonCollection[j].Pt, GenWeight );
 *                        h_dzVTX_eta->Fill( MuonCollection[j].eta, GenWeight);
 *                        h_dzVTX_phi->Fill( MuonCollection[j].phi, GenWeight);
 *                    }
 *                    if( MuonCollection[j].pixelHits > 0 )
 *                    {
 *                        h_pixelHits_Pt->Fill( MuonCollection[j].Pt, GenWeight );
 *                        h_pixelHits_eta->Fill( MuonCollection[j].eta, GenWeight);
 *                        h_pixelHits_phi->Fill( MuonCollection[j].phi, GenWeight);
 *                    }
 *                    if( MuonCollection[j].trackerLayers > 5)
 *                    {
 *                        h_trackerLayers_Pt->Fill( MuonCollection[j].Pt, GenWeight );
 *                        h_trackerLayers_eta->Fill( MuonCollection[j].eta, GenWeight);
 *                        h_trackerLayers_phi->Fill( MuonCollection[j].phi, GenWeight);
 *                    }
 *                    if( MuonCollection[j].trkiso < 0.10)
 *                    {
 *                        h_trkiso_Pt->Fill( MuonCollection[j].Pt, GenWeight );
 *                        h_trkiso_eta->Fill( MuonCollection[j].eta, GenWeight);
 *                        h_trkiso_phi->Fill( MuonCollection[j].phi, GenWeight);
 *
 *                    }
 */
					/*
					 *if ( MuonCollection[j].isTightMuon() && MuonCollection[j].trkiso < 0.10 )
					 *{
					 *    h_cut_Pt->Fill( MuonCollection[j].Pt, GenWeight );
					 *    h_cut_eta->Fill( MuonCollection[j].Pt, GenWeight );
					 *    h_cut_phi->Fill( MuonCollection[j].Pt, GenWeight );
					 *}
					 */
					//if( MuonCollection[j].isTightMuon() && MuonCollection[j].trkiso < 0.10)
						//QMuonCollection.push_back( MuonCollection[j] );
					if( Cut == "isGLB")
					{	
						if( MuonCollection[j].isTightMuon_minus_isGLB())
							QMuonCollection.push_back( MuonCollection[j] );
					} else if( Cut == "isPF")
					{
						if( MuonCollection[j].isTightMuon_minus_isPF())
							QMuonCollection.push_back( MuonCollection[j] );
					} else if( Cut == "chi2dof")
					{
						if( MuonCollection[j].isTightMuon_minus_chi2dof())
							QMuonCollection.push_back( MuonCollection[j] );
					} else if( Cut == "muonHits")
					{
						if( MuonCollection[j].isTightMuon_minus_muonHits())
							QMuonCollection.push_back( MuonCollection[j] );
					} else if( Cut == "nMatches")
					{
						if( MuonCollection[j].isTightMuon_minus_nMatches())
							QMuonCollection.push_back( MuonCollection[j] );
					} else if( Cut == "dxyVTX")
					{
						if( MuonCollection[j].isTightMuon_minus_dxyVTX())
							QMuonCollection.push_back( MuonCollection[j] );
					} else if( Cut == "dzVTX")
					{
						if( MuonCollection[j].isTightMuon_minus_dzVTX())
							QMuonCollection.push_back( MuonCollection[j] );
					} else if( Cut == "pixelHits")
					{
						if( MuonCollection[j].isTightMuon_minus_pixelHits())
							QMuonCollection.push_back( MuonCollection[j] );
					} else if( Cut == "trackerLayers")
					{
						if( MuonCollection[j].isTightMuon_minus_trackerLayers())
							QMuonCollection.push_back( MuonCollection[j] );
					} else if( Cut == "trkiso")
					{
						if( MuonCollection[j].isTightMuon_minus_trkiso())
							QMuonCollection.push_back( MuonCollection[j] );
					}
				}

				//Give Acceptance cuts
				if( QMuonCollection.size() >= 2)
				{
					Muon leadMu, subMu;
					CompareMuon(&QMuonCollection[0], &QMuonCollection[1], &leadMu, &subMu);
					if( !(leadMu.Pt > LeadPtCut && subMu.Pt > SubPtCut && abs(leadMu.eta) < LeadEtaCut && abs(subMu.eta) < SubEtaCut) )
						QMuonCollection.clear();
				}

				if( QMuonCollection.size() == 2)
				{
					Muon recolep1 = QMuonCollection[0];
					Muon recolep2 = QMuonCollection[1];
					TLorentzVector reco_v1 = recolep1.Momentum;
					TLorentzVector reco_v2 = recolep2.Momentum;
					Double_t reco_M = (reco_v1 + reco_v2).M();

					if( reco_M > 60 && reco_M < 120 && isPassDimuonVertexCut(ntuple, recolep1.cktpT, recolep2.cktpT) )
					{
						Plots->FillHistograms(ntuple, HLT, recolep1, recolep2, GenWeight);

						//Count # events in the Z-peak region for each sample
						//if( reco_M > 60 && reco_M < 120 )
							count_Zpeak++;
					}
				}

			} //End of if( isTriggered )

		} //End of event iteration

		cout << "\tcount_Zpeak(" << Tag[i_tup] << "): " << count_Zpeak << endl;

		// if( Tag[i_tup] == "DYTauTau_M20to50" )
		// {
		// 	for(Int_t i_hist=0; i_hist < (Int_t)Plots->Histo.size(); i_hist++)
		// 		Plots->Histo[i_hist]->Scale( 2.49997e+10 / 4.48119e+11 );
		// }




		Plots->WriteHistograms( f );
		//fcut->cd();
		//h_isGLB_Pt->Write();
		//h_isGLB_eta->Write();
		//h_isGLB_phi->Write();
		//h_isPF_Pt->Write();
		//h_isPF_eta->Write();
		//h_isPF_phi->Write();
		//h_chi2dof_Pt->Write();
		//h_chi2dof_eta->Write();
		//h_chi2dof_phi->Write();
		//h_muonHits_Pt->Write();
		//h_muonHits_eta->Write();
		//h_muonHits_phi->Write();
		//h_nMatches_Pt->Write();
		//h_nMatches_eta->Write();
		//h_nMatches_phi->Write();
		//h_dxyVTX_Pt->Write();
		//h_dxyVTX_eta->Write();
		//h_dxyVTX_phi->Write();
		//h_dzVTX_Pt->Write();
		//h_dzVTX_eta->Write();
		//h_dzVTX_phi->Write();
		//h_pixelHits_Pt->Write();
		//h_pixelHits_eta->Write();
		//h_pixelHits_phi->Write();
		//h_trackerLayers_Pt->Write();
		//h_trackerLayers_eta->Write();
		//h_trackerLayers_phi->Write();
		//h_trkiso_Pt->Write();
		//h_trkiso_eta->Write();
		//h_trkiso_phi->Write();
		//fallcut->cd();
		//h_cut_Pt->Write();
		//h_cut_eta->Write();
		//h_cut_phi->Write();



		if(isNLO == 1)
			cout << "\tTotal sum of weights: " << SumWeight << endl;

/*
 *        if( Tag[i_tup] == "DYMuMu_M20to50" )
 *            cout << "\tSum of weights in 20<M<50 for DYMuMu events: " << SumWeight_DYMuMu_M20to50 << endl;
 *
 *        if( Tag[i_tup] == "DYTauTau_M20to50" )
 *            cout << "\tSum of weights in 20<M<50 for DYTauTau events: " << SumWeight_DYTauTau_M20to50 << endl;
 *
 */
		Double_t LoopRunTime = looptime.CpuTime();
		cout << "\tLoop RunTime(" << Tag[i_tup] << "): " << LoopRunTime << " seconds\n" << endl;
	} //end of i_tup iteration
}//end of i_cut iteration

//	TH1D *h_isGLB_Pt_Scaled = (TH1D *) h_isGLB_PtDYMuMu->Clone();
//	h_isGLB_Pt_Scaled->Scale(1/Factor);
//	h_isGLB_Pt_Scaled->Write();


	Double_t TotalRunTime = totaltime.CpuTime();
	cout << "Total RunTime: " << TotalRunTime << " seconds" << endl;

	TTimeStamp ts_end;
	cout << "[End Time(local time): " << ts_end.AsString("l") << "]" << endl;
}

void AssignAccThreshold(TString HLTname, TString *HLT, Double_t *LeadPtCut, Double_t *SubPtCut, Double_t *LeadEtaCut, Double_t *SubEtaCut)
{
	if( HLTname == "IsoMu20" )
	{
		*HLT = "HLT_IsoMu20_v*";
		*LeadPtCut = 21;
		*SubPtCut = 10;
		*LeadEtaCut = 2.4;
		*SubEtaCut = 2.4;
	}
	else if( HLTname == "Mu45_eta2p1" )
	{
		*HLT = "HLT_Mu45_eta2p1_v*";
		*LeadPtCut = 46;
		*SubPtCut = 10;
		*LeadEtaCut = 2.1;
		*SubEtaCut = 2.4;
	}
	else
	{
		cout << "Wrong HLT name!: " << HLTname << endl;
		return;
	}

}

void CompareMuon(Muon *Mu1, Muon *Mu2, Muon *leadMu, Muon *subMu)
{
    if( Mu1->Pt > Mu2->Pt )
    {
        *leadMu = *Mu1;
        *subMu = *Mu2;
    }
    else
    {
        *leadMu = *Mu2;
        *subMu = *Mu1;
    }
}

void CompareGenLepton(GenLepton *genlep1, GenLepton *genlep2, GenLepton *leadgenlep, GenLepton *subgenlep)
{
	if( genlep1->Pt > genlep2->Pt )
	{
		*leadgenlep = *genlep1;
		*subgenlep = *genlep2;
	}
	else
	{
		*leadgenlep = *genlep2;
		*subgenlep = *genlep1;
	}
}

void GenMatching(TString HLTname, TString MuonType, NtupleHandle* ntuple, vector<Muon>* MuonCollection)
{
	TString HLT;
	Double_t LeadPtCut = 9999;
	Double_t SubPtCut = 9999;
	Double_t LeadEtaCut = 9999;
	Double_t SubEtaCut = 9999;

	AssignAccThreshold(HLTname, &HLT, &LeadPtCut, &SubPtCut, &LeadEtaCut, &SubEtaCut);

	vector<GenLepton> GenLeptonCollection;
	Int_t NGenLeptons = ntuple->gnpair;

	if( MuonType == "PromptFinalState" )
	{
		for(Int_t i_gen=0; i_gen<NGenLeptons; i_gen++)
		{
			GenLepton genlep;
			genlep.FillFromNtuple(ntuple, i_gen);
			if( genlep.isMuon() && genlep.isPromptFinalState )
				GenLeptonCollection.push_back( genlep );
		}
	}
	else if( MuonType == "fromTau")
	{
		for(Int_t i_gen=0; i_gen<NGenLeptons; i_gen++)
		{
			GenLepton genlep;
			genlep.FillFromNtuple(ntuple, i_gen);
			if( genlep.isMuon() && genlep.isDirectPromptTauDecayProductFinalState )
				GenLeptonCollection.push_back( genlep );
		}

	}
	else if( MuonType == "fromHardProcess" )
	{
		for(Int_t i_gen=0; i_gen<NGenLeptons; i_gen++)
		{
			GenLepton genlep;
			genlep.FillFromNtuple(ntuple, i_gen);
			if( genlep.isMuon() && genlep.fromHardProcessFinalState )
				GenLeptonCollection.push_back( genlep );
		}
	}
	else
	{
		cout << "Incorrect MuonType!" << endl;
		return;
	}

	//Give Acceptance Cuts
	if( GenLeptonCollection.size() >= 2 )
	{
		GenLepton leadGenLep, subGenLep;
		CompareGenLepton(&GenLeptonCollection[0], &GenLeptonCollection[1], &leadGenLep, &subGenLep);
		if( !(leadGenLep.Pt > LeadPtCut && subGenLep.Pt > SubPtCut && abs(leadGenLep.eta) < LeadEtaCut && abs(subGenLep.eta) < SubEtaCut) )
			GenLeptonCollection.clear();
	}



	Int_t NMuons = (Int_t)MuonCollection->size();
	vector<Muon> RecoMuonCollection;
	//Copy all muons in MuonCollection into RecoMuonCollection
	for(Int_t i_mu=0; i_mu<NMuons; i_mu++)
		RecoMuonCollection.push_back( MuonCollection->at(i_mu) );

	MuonCollection->clear();

	Int_t NGen = (Int_t)GenLeptonCollection.size();
	for(Int_t i_gen=0; i_gen<NGen; i_gen++)
	{
		GenLepton genlep = GenLeptonCollection[i_gen];
		Double_t gen_Pt = genlep.Pt;
		Double_t gen_eta = genlep.eta;
		Double_t gen_phi = genlep.phi;

		Int_t i_matched = -1;
		Double_t dPtMin = 1e10;
		for(Int_t i_reco=0; i_reco<NMuons; i_reco++)
		{
			Muon mu = RecoMuonCollection[i_reco];
			Double_t reco_Pt = mu.Pt;
			Double_t reco_eta = mu.eta;
			Double_t reco_phi = mu.phi;

			Double_t dR = sqrt( (gen_eta-reco_eta)*(gen_eta-reco_eta) + (gen_phi-reco_phi)*(gen_phi-reco_phi) );
			Double_t dPt = fabs(gen_Pt - reco_Pt);
			if( dR < 0.3 )
			{
				if( dPt < dPtMin )
				{
					i_matched = i_reco;
					dPtMin = dPt;
				}
			}
		}

		if( i_matched != -1 )
			MuonCollection->push_back( RecoMuonCollection[i_matched] );
	}

	return;
}

Bool_t isPassDimuonVertexCut(NtupleHandle *ntuple, Double_t Pt1, Double_t Pt2)
{
	vector<double> *PtCollection1 = ntuple->vtxTrkCkt1Pt;
	vector<double> *PtCollection2 = ntuple->vtxTrkCkt2Pt;
	vector<double> *VtxProbCollection = ntuple->vtxTrkProb;

	Int_t NPt1 = (Int_t)PtCollection1->size();
	Int_t NPt2 = (Int_t)PtCollection2->size();
	Int_t NProb = (Int_t)VtxProbCollection->size();

	if( NPt1 != NPt2 || NPt2 != NProb || NPt1 != NProb )
		cout << "NPt1: " << NPt1 << " NPt2: " << NPt2 << " Nprob: " << NProb << endl;

	// cout << "Pt1: " << Pt1 << " Pt2: " << Pt2 << endl;

	Double_t VtxProb = -1;
	for(Int_t i=0; i<NProb; i++)
	{
		// cout << "\tPtCollection1->at("<< i << "): " << PtCollection1->at(i) << " PtCollection2->at("<< i << "): " << PtCollection2->at(i) << endl;
		if( ( PtCollection1->at(i) == Pt1 && PtCollection2->at(i) == Pt2 )  || ( PtCollection1->at(i) == Pt2 && PtCollection2->at(i) == Pt1 ) )
		{
			VtxProb = VtxProbCollection->at(i);
			break;
		}
	}

	// cout << "VtxProb: " << VtxProb << endl;
	if( VtxProb > 0.02 )
		return 1;
	else
		return 0;
}

static inline void loadBar(int x, int n, int r, int w)
{
    // Only update r times.
    if( x == n )
    	cout << endl;

    if ( x % (n/r +1) != 0 ) return;


    // Calculuate the ratio of complete-to-incomplete.
    float ratio = x/(float)n;
    int   c     = ratio * w;

    // Show the percentage complete.
    printf("%3d%% [", (int)(ratio*100) );

    // Show the load bar.
    for (int x=0; x<c; x++) cout << "=";

    for (int x=c; x<w; x++) cout << " ";

    // ANSI Control codes to go back to the
    // previous line and clear it.
	cout << "]\r" << flush;

}
