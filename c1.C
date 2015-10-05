{
//=========Macro generated from canvas: c1/Test
//=========  (Mon Oct  5 13:14:20 2015) by ROOT version5.34/14
   TCanvas *c1 = new TCanvas("c1", "Test",56,52,700,500);
   gStyle->SetOptStat(0);
   c1->Range(0,0,1,1);
   c1->SetFillColor(0);
   c1->SetBorderMode(0);
   c1->SetBorderSize(2);
   c1->SetFrameBorderMode(0);
  
// ------------>Primitives in pad: c1_1
   TPad *c1_1 = new TPad("c1_1", "c1_1",0.01,0.51,0.49,0.99);
   c1_1->Draw();
   c1_1->cd();
   c1_1->Range(-1.25,-0.445845,11.25,1.231654);
   c1_1->SetFillColor(0);
   c1_1->SetBorderMode(0);
   c1_1->SetBorderSize(2);
   c1_1->SetFrameBorderMode(0);
   c1_1->SetFrameBorderMode(0);
   
   TF1 *func1 = new TF1("func1","sin(x)/x",0,10);
   func1->SetNpx(2000);
   func1->SetFillColor(19);
   func1->SetFillStyle(0);
   func1->SetLineColor(2);
   func1->SetLineWidth(2);
   func1->GetXaxis()->SetLabelFont(42);
   func1->GetXaxis()->SetLabelSize(0.035);
   func1->GetXaxis()->SetTitleSize(0.035);
   func1->GetXaxis()->SetTitleFont(42);
   func1->GetYaxis()->SetLabelFont(42);
   func1->GetYaxis()->SetLabelSize(0.035);
   func1->GetYaxis()->SetTitleSize(0.035);
   func1->GetYaxis()->SetTitleFont(42);
   func1->Draw("");
   
   TPaveText *pt = new TPaveText(0.4271408,0.9261441,0.5728592,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("sin(x)/x");
   pt->Draw();
   c1_1->Modified();
   c1->cd();
  
// ------------>Primitives in pad: c1_2
   c1_2 = new TPad("c1_2", "c1_2",0.51,0.51,0.99,0.99);
   c1_2->Draw();
   c1_2->cd();
   c1_2->Range(-1.25,-0.445845,11.25,1.231654);
   c1_2->SetFillColor(0);
   c1_2->SetBorderMode(0);
   c1_2->SetBorderSize(2);
   c1_2->SetFrameBorderMode(0);
   c1_2->SetFrameBorderMode(0);
   
   TF1 *func1 = new TF1("func1","sin(x)/x",0,10);
   func1->SetNpx(2000);
   func1->SetFillColor(19);
   func1->SetFillStyle(0);
   func1->SetLineColor(2);
   func1->SetLineWidth(2);
   func1->GetXaxis()->SetLabelFont(42);
   func1->GetXaxis()->SetLabelSize(0.035);
   func1->GetXaxis()->SetTitleSize(0.035);
   func1->GetXaxis()->SetTitleFont(42);
   func1->GetYaxis()->SetLabelFont(42);
   func1->GetYaxis()->SetLabelSize(0.035);
   func1->GetYaxis()->SetTitleSize(0.035);
   func1->GetYaxis()->SetTitleFont(42);
   func1->Draw("axis");
   c1_2->Modified();
   c1->cd();
  
// ------------>Primitives in pad: c1_3
   c1_3 = new TPad("c1_3", "c1_3",0.01,0.01,0.49,0.49);
   c1_3->Draw();
   c1_3->cd();
   c1_3->Range(-1.25,-0.445845,11.25,1.231654);
   c1_3->SetFillColor(0);
   c1_3->SetBorderMode(0);
   c1_3->SetBorderSize(2);
   c1_3->SetFrameBorderMode(0);
   c1_3->SetFrameBorderMode(0);
   
   TF1 *func1 = new TF1("func1","sin(x)/x",0,10);
   func1->SetNpx(2000);
   func1->SetFillColor(19);
   func1->SetFillStyle(0);
   func1->SetLineColor(2);
   func1->SetLineWidth(2);
   func1->GetXaxis()->SetLabelFont(42);
   func1->GetXaxis()->SetLabelSize(0.035);
   func1->GetXaxis()->SetTitleSize(0.035);
   func1->GetXaxis()->SetTitleFont(42);
   func1->GetYaxis()->SetLabelFont(42);
   func1->GetYaxis()->SetLabelSize(0.035);
   func1->GetYaxis()->SetTitleSize(0.035);
   func1->GetYaxis()->SetTitleFont(42);
   func1->Draw("col");
   
   pt = new TPaveText(0.4271408,0.9261441,0.5728592,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   text = pt->AddText("sin(x)/x");
   pt->Draw();
   c1_3->Modified();
   c1->cd();
  
// ------------>Primitives in pad: c1_4
   c1_4 = new TPad("c1_4", "c1_4",0.51,0.01,0.99,0.49);
   c1_4->Draw();
   c1_4->cd();
   c1_4->Range(0,0,1,1);
   c1_4->SetFillColor(0);
   c1_4->SetBorderMode(0);
   c1_4->SetBorderSize(2);
   c1_4->SetFrameBorderMode(0);
   c1_4->Modified();
   c1->cd();
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
}
