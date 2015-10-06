{
	TCanvas *c1 = new TCanvas("c1", "THStack", 10, 10, 700, 700);
	//c1->Divide(2, 1);

	THStack *hs = new THStack ("hs", "THStack");
	
	TH1F *h1 = new TH1F ("h1", "test hstack", 100, -4, 4);
	h1->FillRandom("gaus", 20000);
	h1->SetFillColor(kRed);
	hs->Add(h1);

	TH1F *h2 = new TH1F ("h2", "test hstack2", 100, -4, 4);
	h2->FillRandom("gaus", 15000);
	h2->SetFillColor(kBlue);
	hs->Add(h2);

	TH1F *h3 = new TH1F ("h3", "test hstack3", 100, -4, 4);
	h3->FillRandom ("gaus", 10000);
	h3->SetFillColor(kGreen);
	hs->Add(h3);

	c1->cd(1);
	hs->Draw();
   /* c1->cd(2);*/
	/*hs->Draw("nostack");*/
}
