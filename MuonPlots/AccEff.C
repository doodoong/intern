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
void AccEff(TString HLTname = "IsoMu20")
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

	Double_t Factor = ((569.0171*2008.4)*3)/(4.5275*(1e11));


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


	TFile *f = new TFile("AccEff.root", "RECREATE");
	//Loop for all samples
	const Int_t Ntup = ntupleDirectory.size();
	Int_t i_tup = 0;	// only for DY MC
	//for(Int_t i_tup = 0; i_tup<Ntup; i_tup++)
	{
		TStopwatch looptime;
		looptime.Start();

		cout << "\t<" << Tag[i_tup] << ">" << endl;

		ControlPlots *Plots = new ControlPlots( Tag[i_tup] );

		TH1D *h_Pt_genMu = new TH1D ("h_Pt_genMu", "", 250, 0, 500);
		TH1D *h_Pt_gen = new TH1D ("h_Pt_gen", "", 250, 0, 500);
		TH1D *h_eta_genMu = new TH1D ("h_eta_genMu", "",60, -3, 3);
		TH1D *h_eta_gen = new TH1D ("h_eta_gen", "",60, -3, 3);
		TH1D *h_mass_genMu = new TH1D ("h_mass_genMu", "",250, 0 , 500);

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
		Int_t count_gen_M60to120 = 0;
		Int_t count_gen_acc = 0; Int_t count_gen_acc2 = 0;
		Int_t count_reco_sel = 0;
		Double_t SumWeight = 0;
		Double_t SumWeight_DYMuMu_M20to50 = 0;

		Int_t NEvents = chain->GetEntries();
		cout << "\t[Total Events: " << NEvents << "]" << endl;
		//for(Int_t i=0; i<10000; i++)
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
			GenMassFlag = 1; // -- other cases: pass

			// total gen muons
			vector<GenLepton> GenLeptonCollection;
			Int_t NGenLeptons = ntuple->gnpair;
			Bool_t isDY = false;

			//cout << "---------------Event #" << i << "--NGenLeptons: " << NGenLeptons << "---------------------" <<  endl;
			for (Int_t i_gen = 0; i_gen < NGenLeptons; i_gen++)
			{
				GenLepton genlep;
				genlep.FillFromNtuple (ntuple, i_gen);
				//mu.FillFromNtuple (ntuple, i_gen); // NGenLeptons != Nmuons !!!

				h_Pt_gen->Fill( genlep.Pt, GenWeight );
				h_eta_genMu->Fill( genlep.eta, GenWeight );

				//cout << "i_gen: " <<i_gen << " isMuon: " <<  genlep.isMuon() << " fromH: " << genlep.fromHardProcessFinalState << " ";
				//cout << "Pt: " << genlep.Pt << " eta: " << genlep.eta << " " << endl;

				if (genlep.isMuon() && genlep.fromHardProcessFinalState)	// no need genmatching
				{
					GenLeptonCollection.push_back (genlep);

					if (GenLeptonCollection.size() == 2)
					{
						//cout << "DY event!" << endl;
						GenLepton genmu1;
						GenLepton genmu2;
						CompareGenLepton (&GenLeptonCollection[0], &GenLeptonCollection[1], &genmu1, &genmu2);
						TLorentzVector gen_v1 = genmu1.Momentum;
						TLorentzVector gen_v2 = genmu2.Momentum;
						Double_t gen_M = (gen_v1 + gen_v2).M();

						h_Pt_genMu->Fill( genmu1.Pt, GenWeight);
						h_Pt_genMu->Fill( genmu2.Pt, GenWeight);
						h_eta_genMu->Fill( genmu1.eta, GenWeight);
						h_eta_genMu->Fill( genmu2.eta, GenWeight);
						h_mass_genMu->Fill( gen_M, GenWeight);



						//cout << "gen_M: " << gen_M << endl;

						if (!(gen_M > 60 && gen_M < 120))
						{
							GenLeptonCollection.clear();
						} 
						else
						{
							count_gen_M60to120 += GenLeptonCollection.size()/2;
							//cout << "genlapcollection size: " << GenLeptonCollection.size() << endl;

							//cout << count_gen_M60to120 << endl;
							
							if (!(genmu1.Pt > 25 && genmu2.Pt > 25 && abs(genmu1.eta) < 2.4 && abs(genmu2.eta) < 2.4))
							{
								GenLeptonCollection.clear();
							} 
							else
							{
								count_gen_acc += GenLeptonCollection.size()/2;
								count_gen_acc2++;

								if (ntuple->isTriggered (HLT))
								{
									vector<Muon> MuonCollection;
									Int_t NLeptons = ntuple->nMuon;
									for (Int_t i_reco = 0; i_reco < NLeptons; i_reco++)
									{
										Muon mu;
										mu.FillFromNtuple (ntuple, i_reco);
										MuonCollection.push_back (mu);
									}

									vector <Muon> QMuonCollection;
									for (Int_t j = 0; j < (Int_t) MuonCollection.size(); j++)
										if (MuonCollection[j].isTightMuon() && MuonCollection[j].trkiso < 0.10)
											QMuonCollection.push_back(MuonCollection[j]);

									if (QMuonCollection.size() >= 2)
									{
										if (!(QMuonCollection[0].Pt > 25 && QMuonCollection[1].Pt > 25 && abs(QMuonCollection[0].eta) < 2.4 && abs(QMuonCollection[1].eta) < 2.4))
											QMuonCollection.clear();
									}

									if (QMuonCollection.size() == 2)
									{
										Muon recolep1 = QMuonCollection[0];
										Muon recolep2 = QMuonCollection[1];
										TLorentzVector reco_v1 = recolep1.Momentum;
										TLorentzVector reco_v2 = recolep2.Momentum;
										Double_t reco_M = (reco_v1 + reco_v2).M();

										if (reco_M > 60 && reco_M < 120 && isPassDimuonVertexCut (ntuple, recolep1.cktpT, recolep2.cktpT))
										{
											Plots->FillHistograms(ntuple, HLT, recolep1, recolep2, GenWeight);
											count_reco_sel++;
										}
									}

								} // End of if (isTriggered)
							} // End of gen pT, eta cut

						} // End of gen_M 60to120
						//break;
					} // genlepton# = 2
				} // end isMuon && fromHardProcessFinalState
			} // end iteration i_gen

		} //End of event iteration

		//cout << "\tcount_Zpeak(" << Tag[i_tup] << "): " << count_Zpeak << endl;
		cout << "\tcount_gen_M60to120: \t" << count_gen_M60to120 << endl;
		cout << "\tcount_gen_acc: \t\t" << count_gen_acc << " " << count_gen_acc2 << endl;
		cout << "\tcount_reco_sel: \t" << count_reco_sel << endl;
		cout << "\tAcceptance: \t\t" << ( (Double_t)count_gen_acc / (Double_t)count_gen_M60to120) << endl;
		cout << "\tEfficiency: \t\t" << ( (Double_t)count_reco_sel / (Double_t)count_gen_acc) << endl;
		cout << "\tAccxEff: \t\t" << ( (Double_t)count_reco_sel / (Double_t)count_gen_M60to120) << endl;

		//if (Tag[i_tpu] == "DYMuMu" )
		//{
		//for (Int_t i_hist = 0; i_hist < (Int_t)Plots->Histo.size(); i_hist++)
		//Plots->Histo[i_hist]->Scale (Factor);
		//}


		Plots->WriteHistograms( f );
		h_Pt_gen->Write();
		h_eta_gen->Write();
		h_Pt_genMu->Write();
		h_eta_genMu->Write();
		h_mass_genMu->Write();


		if(isNLO == 1)
			cout << "\tTotal sum of weights: " << SumWeight << endl;

		Double_t LoopRunTime = looptime.CpuTime();
		cout << "\tLoop RunTime(" << Tag[i_tup] << "): " << LoopRunTime << " seconds\n" << endl;
	} //end of i_tup iteration

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
		*LeadPtCut = 25;
		*SubPtCut = 25;
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

	//if (GenLeptonCollection.size() != 0 && GenLeptonCollection.size() != 2)
	//cout << "something wrong!" << endl;
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
