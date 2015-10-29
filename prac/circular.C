{
	gROOT->cd();
	TTree *T = new TTree ("T", "test circular buffers");
	TRandom r;
	Float_t px, py, pz;
	Double_t random;
	UShort_t i;
	T->Branch ("px", &px, "px/F");
	T->Branch ("py", &py, "py/F");
	T->Branch ("pz", &pz, "pz/F");
	T->Branch ("random", &random, "random/D");
	T->Branch ("i", &i, "i/s");
	T->SetCircular (20000);
	for (i = 0; i< 65000; i++){
		r.Rannor(px,py);
		pz = px*px + py*py;
		random - r.Rndm();
		T->Fill();
	}
	T->Print();
}
