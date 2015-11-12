#include <TFile.h>
#include <TH1D.h>
#include <TString.h>
#include <TPad.h>
#include <TCanvas.h>
#include <TImage.h>
#include <vector>

// Example displaying two histograms and their ratio.
// Author: Olivier Couet
void ratioplot2 () {
   // Define two gaussian histograms. Note the X and Y title are defined
   // at booking time using the convention "Hist_title ; X_title ; Y_title"
   /*
    *TH1F *h1 = new TH1F("h1", "Two gaussian plots and their ratio;x title; h1 and h2 gaussian histograms", 100, -5, 5);
    *TH1F *h2 = new TH1F("h2", "h2", 100, -5, 5);
    *h1->FillRandom("gaus");
    *h2->FillRandom("gaus");
	*/
	vector<TString> Cut;
	Cut.push_back("isGLB");
	Cut.push_back("isPF");
	Cut.push_back("chi2dof");
	Cut.push_back("muonHits");
	Cut.push_back("nMatches");
	Cut.push_back("dxyVTX");
	Cut.push_back("dzVTX");
	Cut.push_back("pixelHits");
	Cut.push_back("trackerLayers");
	Cut.push_back("trkiso");

	vector<TString> Type;
	Type.push_back("Pt_");
	Type.push_back("eta");
	Type.push_back("mass");

	
	

	//Double_t Factor = 569.0171*2008.4*3/4.5275/10; // Wrong value;
	Double_t Factor = ((569.0171*2008.4)*3)/(4.5275*(1e11));

	TFile *f1;
	TH1D *h1, *h2;
	TH1F *h3;
	//TCanvas *c;
	TCanvas *c = new TCanvas("c", "canvas", 800,800);
	TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
	TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
	TImage *img = TImage::Create();
	
	Int_t i = 0;
	Int_t j = 1;
	for (Int_t i = 0; i < 10; i++)
	{	
		f1 = new TFile ("MuonTight_"+Cut[i]+".root");

		for (Int_t j = 0; j < 3; j++)
		{

			c = new TCanvas("c", "canvas", 800, 800);
			cout << Type[j] << " drawing.." <<  endl;

			h1 = (TH1D *)f1->Get("h_"+Type[j]+"_Data");
			h2 = (TH1D *)f1->Get("h_"+Type[j]+"_DYMuMu");
			h2->Scale(Factor);

			cout << Type[j] << " h1, h2 constructed" << endl;

			//h1->
			// Define the Canvas
			//TCanvas *c = new TCanvas("c", "canvas", 800, 800);

			// Upper plot will be in pad1
			pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
			pad1->SetBottomMargin(0); // Upper and lower plot are joined
			pad1->SetGridx();         // Vertical grid
			pad1->Draw();             // Draw the upper pad: pad1
			pad1->cd();               // pad1 becomes the current pad
			//h1->SetStats(0);          // No statistics on upper plot

			cout << "h1, h2 before draw" << endl;
			h2->Draw();               // Draw h1
			h1->Draw("same");         // Draw h2 on top of h1
			
			cout << "h1, h2 draw.." << endl;
			// Do not draw the Y axis label on the upper plot and redraw a small
			// axis instead, in order to avoid the first label (0) to be clipped.
			//h1->GetYaxis()->SetLabelSize(0.);
			//TGaxis *axis = new TGaxis( -5, 20, -5, 220, 20,220,510,"");
			//TGaxis *axis = (TGaxis *)h1->GetGaxis();
			//axis->SetLabelFont(43); // Absolute font size in pixel (precision 3)
			//axis->SetLabelSize(15);
			//axis->Draw();

			// lower plot will be in pad
			c->cd();          // Go back to the main canvas before defining pad2
			pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
			pad2->SetTopMargin(0.01);
			pad2->SetBottomMargin(0.2);
			pad2->SetGridx(); // vertical grid
			pad2->Draw();
			pad2->cd();       // pad2 becomes the current pad

			// Define the ratio plot
			h3 = (TH1F*)h1->Clone("h3");
			h3->SetLineColor(kBlack);
			h3->SetMinimum(0);  // Define Y ..
			h3->SetMaximum(2); // .. range
			h3->Sumw2();
			h3->SetStats(0);      // No statistics on lower plot
			h3->Divide(h2);
			h3->SetMarkerStyle(21);
			h3->Draw("ep");       // Draw the ratio plot

			// h1 settings
			h1->SetLineColor(kBlue+1);
			h1->SetLineWidth(2);

			// Y axis h1 plot settings
			h1->GetYaxis()->SetTitleSize(20);
			h1->GetYaxis()->SetTitleFont(43);
			h1->GetYaxis()->SetTitleOffset(1.55);

			// h2 settings
			h2->SetLineColor(kRed);
			h2->SetLineWidth(2);

			// Ratio plot (h3) settings
			h3->SetTitle(""); // Remove the ratio title

			// Y axis ratio plot settings
			//h3->GetYaxis()->SetTitle("ratio h1/h2 ");
			h3->GetYaxis()->SetNdivisions(505);
			h3->GetYaxis()->SetTitleSize(20);
			h3->GetYaxis()->SetTitleFont(43);
			h3->GetYaxis()->SetTitleOffset(1.55);
			h3->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
			h3->GetYaxis()->SetLabelSize(15);

			// X axis ratio plot settings
			h3->GetXaxis()->SetTitleSize(20);
			h3->GetXaxis()->SetTitleFont(43);
			h3->GetXaxis()->SetTitleOffset(4.);
			h3->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
			h3->GetXaxis()->SetLabelSize(15);

			//TImage *img = TImage::Create();

			img->FromPad(c);
			img->WriteImage(Type[j]+"_Tightminus_"+Cut[i]+".png");

			cout << Type[j] << "_Tightminus_" << Cut[i] << ".png output" << endl;
			//delete h3;
			//delete h2;
			//delete h1;
			//delete pad1;
			//delete pad2;
			delete c;


			//delete f1;

			cout << "file closed" << endl;
		} // iteration Type end
	} // iteration Cut end
}

