void twohistinone()
{

	//TCanvas *c = new TCanvas("c", "2in1", 0, 40, 500, 2000);

//	gROOT->ForceStyle();
	TH1F *h1 = new TH1F ("h1", "h1", 100, -10, 10);
	
	h1->FillRandom("gaus", 500);
	h1->Draw("color");

	TH1F *h2 = new TH1F ("h2", "h2", 100, -10, 10);

	h2->FillRandom("gaus", 200);
	h2->SetLineColor(kRed);
	h2->Draw("same");
}
