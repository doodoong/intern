{
	TH2F *h1 = new TH2F("h1", "2D", 20, -7, 7, 20, -7, 7);

	Float_t px, py;
	for (Int_t i = 0; i < 1000; i++) {
		gRandom->Rannor(px, py);
		h1->Fill(px, py);
	}

	h1->Fill(1, 1, 10);
	h1->Fill(2, 2, 20);
	h1->Draw("colorz");
}
