#include <TFile.h>
#include <TH1D.h>
#include <TString.h>
#include <TPad.h>
#include <TCanvas.h>
#include <TImage.h>
#include <vector>

// Example displaying two histograms and their ratio.
// Author: Olivier Couet
void ratioplot () {
   // Define two gaussian histograms. Note the X and Y title are defined
   // at booking time using the convention "Hist_title ; X_title ; Y_title"
   /*
    *TH1F *h1 = new TH1F("h1", "Two gaussian plots and their ratio;x title; h1 and h2 gaussian histograms", 100, -5, 5);
    *TH1F *h2 = new TH1F("h2", "h2", 100, -5, 5);
    *h1->FillRandom("gaus");
    *h2->FillRandom("gaus");
	*/
	//vector<TString> Cut;
	//Cut.push_back("isGLB");
	//Cut.push_back("isPF");
	//Cut.push_back("chi2dof");
	//Cut.push_back("muonHits");
	//Cut.push_back("nMatches");
	//Cut.push_back("dxyVTX");
	//Cut.push_back("dzVTX");
	//Cut.push_back("pixelHits");
	//Cut.push_back("trackerLayers");
	//Cut.push_back("trkiso");

	vector<TString> Type;	vector<TString> XDim;								vector<TString> YDim;
	Type.push_back("Pt");	XDim.push_back("p_{T}[GeV/c]");			YDim.push_back("Number of Muons");
	Type.push_back("eta");	XDim.push_back("eta");						YDim.push_back("Number of Muons");
	Type.push_back("mass");	XDim.push_back("M_{{mumu}[GeV/c^{2}]");	YDim.push_back("Number of Events");
	Type.push_back("diPt");	XDim.push_back("Dimuon p_{T}[GeV/c]");				YDim.push_back("Number of Events");
	Type.push_back("diRap");XDim.push_back("Dimuon Rapidity");					YDim.push_back("Number of Events");

	//Double_t Factor = 569.0171*2008.4*3/4.5275/10; // Wrong value;
	Double_t Factor = ((569.0171*2008.4)*3)/(4.5275*(1e11));

	TFile *f1;
	TH1D *h1, *h2;
	TH1F *h3;
	TCanvas *c, *c0, *c1, *c2, *c3, *c4;
	TPad *pad1;
	TPad *pad2;
	TLegend *leg;
	TImage *img = TImage::Create();

	//c0 = new TCanvas("c0", "canvas", 900, 900);
	//c1 = new TCanvas("c1", "canvas", 900, 900);
	//c2 = new TCanvas("c2", "canvas", 900, 900);
	//c3 = new TCanvas("c3", "canvas", 900, 900);
	//c4 = new TCanvas("c4", "canvas", 900, 900);
	// vector<TCanvas> c;
	// c.push_back(c0);	c.push_back(c1);	c.push_back(c2);	c.push_back(c3);	c.push_back(c4);
	//Int_t i = 0;
	 Int_t j = 3;
	// for (Int_t i = 0; i < 10; i++)
	{
		f1 = new TFile ("MuonTightM60to120Pt25.root");

		//for (Int_t j = 0; j < 5; j++)
		{

			 c = new TCanvas("c", "canvas", 900, 900);
			cout << Type[j] << " drawing.." <<  endl;

			h1 = (TH1D *)f1->Get("h_"+Type[j]+"_Data");
			h2 = (TH1D *)f1->Get("h_"+Type[j]+"_DYMuMu");
			//h2->Scale(Factor);

			cout << Type[j] << " h1, h2 constructed" << endl;

			//h1->
			// Define the Canvas
			//TCanvas *c = new TCanvas("c", "canvas", 800, 800);
			//if (j == 0)
				//c0->cd();          // Go back to the main canvas before defining pad2
			//if (j == 1)
				//c1->cd();
			//if (j == 2)
				//c2->cd();
			//if (j == 3)
				//c3->cd();
			//if (j == 4)
				//c4->cd();
			c->cd();
			// Upper plot will be in pad1
			pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
			pad1->SetBottomMargin(0); // Upper and lower plot are joined
			pad1->SetGridx();         // Vertical grid
			pad1->Draw();             // Draw the upper pad: pad1
			pad1->cd();               // pad1 becomes the current pad
			pad1->SetTitle();
			h2->SetStats(0);
			h1->SetStats(0); // No statistics on upper plot
			// if (Type[j] == "Pt" || Type[j] == "diPt")
				pad1->SetLogy();

			cout << "h1, h2 before draw" << endl;

			//h1->Draw("E1");    // Draw Data
			h2->Draw("hist"); // Draw DYMuMu
			h1->Draw("E1same");

			cout << "h1, h2 draw.." << endl;
			// Do not draw the Y axis label on the upper plot and redraw a small
			// axis instead, in order to avoid the first label (0) to be clipped.
			//h1->GetYaxis()->SetLabelSize(0.);
			//TGaxis *axis = new TGaxis( -5, 20, -5, 220, 20,220,510,"");
			//TGaxis *axis = (TGaxis *)h1->GetGaxis();
			//axis->SetLabelFont(43); // Absolute font size in pixel (precision 3)
			//axis->SetLabelSize(15);
			//axis->Draw();

			leg = new TLegend (.80, .85, 1, 1);
			leg->AddEntry(h1, "Data");
			leg->AddEntry(h2, "DYMuMu");
			leg->Draw();

			// lower plot will be in pad
			//if (j == 0)
				//c0->cd();          // Go back to the main canvas before defining pad2
			//if (j == 1)
				//c1->cd();
			//if (j == 2)
				//c2->cd();
			//if (j == 3)
				//c3->cd();
			//if (j == 4)
				//c4->cd();
			
			c->cd();
			pad2 = new TPad("pad2", "pad2", 0, 0.1, 1, 0.3);
			pad2->SetTopMargin(0.0001);
			pad2->SetBottomMargin(0.3);
			pad2->SetGridx(); // vertical grid
			pad2->SetGridy();
			pad2->Draw();
			pad2->cd();       // pad2 becomes the current pad

			// Define the ratio plot
			h3 = (TH1F*)h1->Clone("h3");
			h3->SetLineColor(kBlack);
			h3->SetMinimum(0.7);  // Define Y ..
			h3->SetMaximum(1.2); // .. range
			h3->Sumw2();
			h3->SetStats(0);      // No statistics on lower plot
			h3->Divide(h2);
			// h3->SetMarkerStyle(21);
			h3->Draw("ep");       // Draw the ratio plot

			// h1 settings
			h1->SetLineColor(kBlack);
			h1->SetTitle("CMS Preliminary L=569.017pb^{-1}");
			h2->SetTitle("CMS Preliminary L=569.017pb^{-1}");
			// h1->SetMarkerStyle(21);
			h1->SetLineWidth(2);

			if (Type[j] == "mass")
			{
				h1->GetXaxis()->SetRangeUser(60,120);
				h2->GetXaxis()->SetRangeUser(60,120);
				h3->GetXaxis()->SetRangeUser(60,120);
			}

			// Y axis h1 plot settings
			h1->GetYaxis()->SetTitle(YDim[j]);
			h2->GetYaxis()->SetTitle(YDim[j]);
			h1->GetYaxis()->SetTitleSize(20);
			h1->GetYaxis()->SetTitleFont(43);
			h1->GetYaxis()->SetTitleOffset(1.55);

			// h2 settings
			h2->SetFillColor(kYellow);
			h2->SetLineWidth(0);

			// Ratio plot (h3) settings
			h3->SetTitle(""); // Remove the ratio title

			// Y axis ratio plot settings
			h3->GetYaxis()->SetTitle("Data/DY");
			h3->GetYaxis()->SetNdivisions(505);
			h3->GetYaxis()->SetTitleSize(20);
			h3->GetYaxis()->SetTitleFont(43);
			h3->GetYaxis()->SetTitleOffset(1.55);
			h3->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
			h3->GetYaxis()->SetLabelSize(15);
			h3->GetYaxis()->CenterTitle();

			// X axis ratio plot settings
			h3->GetXaxis()->SetTitle(XDim[j]);
			h3->GetXaxis()->SetTitleSize(20);
			h3->GetXaxis()->SetTitleFont(43);
			h3->GetXaxis()->SetTitleOffset(4.);
			h3->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
			h3->GetXaxis()->SetLabelSize(15);



			//img = TImage::Create();
			//if (j == 0)
				//img->FromPad(c0);
			//if (j == 1)
				//img->FromPad(c1);
			//if (j == 2)
				//img->FromPad(c2);
			//if (j == 3)
				//img->FromPad(c3);
			//if (j == 4)
				//img->FromPad(c4);

			img->FromPad(c);
			img->WriteImage("./pictures/TightM60to120_"+Type[j]+"_log.pdf");
			cout << Type[j] << "_TightM60to120_log.png output" << endl;

			// delete h3;
			// delete h2;
			// delete h1;
			// delete pad1;
			// delete pad2;
			//delete c;

			//f1->Close();

			//delete f1;

			// cout << "file closed" << endl;
		} // iteration Type end
	} // iteration Cut end
}
