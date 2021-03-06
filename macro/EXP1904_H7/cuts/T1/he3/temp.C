void readCuts();
void readPar(TString fileName,Float_t *par1,Float_t *par2,Int_t size=16);

TCutG *cutCsI[16],*cut3h[16],*cutX_C[32];
TCutG *cuthe3_1[16],*cutSQ20_1[16],*cutSQ1_1[16];
TCutG *cuthe3_2[16],*cutSQ20_2[16],*cutSQ1_2[16];
TCutG *cuthe3_3[16],*cutSQ20_3[16],*cutSQ1_3[16];
TCutG *cuthe3_4[16],*cutSQ20_4[16],*cutSQ1_4[16];
TCutG *cutHe3_total1;
TCutG *cut6Li_1;

Float_t pCsI_1[16],pCsI_2[16];

Int_t nCoins;

void createcut(){
  nCoins = 0;
  readPar("CsI_anh",pCsI_1,pCsI_2);
  readCuts();
  // return;

   TCutG *cutg1 = new TCutG("cutg1",11);
   cutg1->SetVarX("a1_1+a20_1_un");
   cutg1->SetVarY("a20_1");
   cutg1->SetTitle("Graph");
   cutg1->SetFillColor(1);
   cutg1->SetPoint(0,3.04789,5.27392);
   cutg1->SetPoint(1,5.77623,3.58816);
   cutg1->SetPoint(2,9.66781,2.43878);
   cutg1->SetPoint(3,16.1608,1.63073);
   cutg1->SetPoint(4,27.9413,1.13615);
   cutg1->SetPoint(5,30.6062,0.948065);
   cutg1->SetPoint(6,29.7979,0.750602);
   cutg1->SetPoint(7,11.106,0.376858);
   cutg1->SetPoint(8,1.48279,0.941099);
   cutg1->SetPoint(9,2.07499,5.24605);
   cutg1->SetPoint(10,3.04789,5.27392);


  TChain *ch = new TChain("tree");

  ch->Add("/media/ivan/data/exp1904/analysed/novPars/selected/temp/trigger2/h7_ct_*");
  cout << ch->GetEntries() << endl;

  TString cut,hdraw;

  // TString canName;
  // TCanvas *c_arr[16];
  // for(Int_t i=0;i<16;i++){
  //   canName.Form("c%d",i+1);
  //   c_arr[i] = new TCanvas(canName.Data(),"title",1800,1000);
  // }
  
  for(Int_t i=8;i<9;i++) {
    // c_arr->cd(i+1);
    // c_arr[i]->cd();
    // Int_t i =0;
    ch->SetMarkerStyle(1);
    ch->SetMarkerColor(kBlack);      
    cut.Form("flag1 && trigger==2 && a20_1<4 && a1_1+a20_1_un<35 && n20_1==%d",i);
    // cut += cutg1->GetName();
    hdraw.Form("a20_1:a1_1+a20_1_un>>h%d1",i);
    ch->Draw(hdraw.Data(),cut.Data(),"");
    // c_arr[i]->Update();

    ch->SetMarkerStyle(20);
    ch->SetMarkerSize(0.5);   
    ch->SetMarkerColor(kGreen);      
    // cut.Form("n20_1==%d && nhe3_1",i);
    cut += " && nh3 && flagCent";
    hdraw.Form("a20_1:a1_1+a20_1_un>>h%d_triton1",i);
    ch->Draw(hdraw.Data(),cut.Data(),"same");
    // c_arr[i]->Update();

    cuthe3_1[i]->SetLineColor(kRed);
    cuthe3_1[i]->Draw("same");
    // c_arr[i]->Update();

    // canName.Form("pdf/id_%d.png",i+1);
    // c_arr[i]->SaveAs(canName.Data());
  }


}

void readCuts() {

  TFile *f;
  TString cutName;

  cutName.Form("/home/ivan/work/macro/h7_1904/cuts/T1/li6_cut.root");
  f = new TFile(cutName.Data());
  cut6Li_1 = (TCutG*)f->Get("CUTG");
  if (!cut6Li_1) {
    cout << "no cut " << cutName.Data() << endl;
    exit(-1);
  }    
  delete f;

  for(Int_t i=0;i<16;i++) {
    cutName.Form("/home/ivan/work/macro/h7_1904/cutsNovPars/C_T/triton/h3_%d.root",i);
    f = new TFile(cutName.Data());
    cut3h[i] = (TCutG*)f->Get("CUTG");
    if (!cut3h[i]) {
      cout << "no cut " << cutName.Data() << endl;
      exit(-1);
    }    
    delete f;
  }

  for(Int_t i=0;i<16;i++) {
    cutName.Form("/home/ivan/work/macro/h7_1904/cutsNovPars/C_T/tCsI/tCsI_full_%d.root",i);
    f = new TFile(cutName.Data());
    cutCsI[i] = (TCutG*)f->Get("CUTG");
    if (!cutCsI[i]) {
      cout << "no cut " << cutName.Data() << endl;
      exit(-1);
    }
    delete f;
  }

  for(Int_t i=0;i<32;i++) {
    cutName.Form("/home/ivan/work/macro/h7_1904/cutsNovPars/C_T/tX_C/tX_C_%d.root",i);
    f = new TFile(cutName.Data());
    cutX_C[i] = (TCutG*)f->Get("CUTG");
    if (!cutX_C[i]) {
      cout << i  << " no cut"<< endl;
      exit(-1);
    }
    delete f;
  }

  for(Int_t i=0;i<16;i++) {
    cutName.Form("/home/ivan/work/macro/h7_1904/cuts/T1/he3/canvas/he3_%d.root",i);
    f = new TFile(cutName.Data());
    cuthe3_1[i] = (TCutG*)f->Get("CUTG");
    if (!cuthe3_1[i]) {
      cout << "no cut " << cutName.Data() << endl;
      exit(-1);
    }
    delete f;
  }

  for(Int_t i=0;i<16;i++) {
    cutName.Form("/home/ivan/work/macro/h7_1904/cutsNovPars/T2/he3/cut_he3_%d.root",i);
    f = new TFile(cutName.Data());
    cuthe3_2[i] = (TCutG*)f->Get("CUTG");
    if (!cuthe3_2[i]) {
      cout << "no cut " << cutName.Data() << endl;
      exit(-1);
    }
    delete f;
  }

  for(Int_t i=0;i<16;i++) {
    cutName.Form("/home/ivan/work/macro/h7_1904/cutsNovPars/T3/he3/cut_he3_%d.root",i);
    f = new TFile(cutName.Data());
    cuthe3_3[i] = (TCutG*)f->Get("CUTG");
    if (!cuthe3_3[i]) {
      cout << "no cut " << cutName.Data() << endl;
      exit(-1);
    }
    delete f;
  }

  for(Int_t i=0;i<16;i++) {
    cutName.Form("/home/ivan/work/macro/h7_1904/cutsNovPars/T4/he3/cut_he3_%d.root",i);
    f = new TFile(cutName.Data());
    cuthe3_4[i] = (TCutG*)f->Get("CUTG");
    if (!cuthe3_4[i]) {
      cout << "no cut " << cutName.Data() << endl;
      exit(-1);
    }
    delete f;
  }

  cout << " CUTS are read" << endl;
}


void readPar(TString fileName,Float_t *par1,Float_t *par2,Int_t size=16){

  TString line;
  ifstream myfile;
  Int_t count=0;
  TString file = "/home/ivan/work/macro/h7_1904/parameters/" + fileName + ".cal";
  myfile.open(file.Data());
  while (! myfile.eof() ){
    line.ReadLine(myfile);
    if(count < 0){
      count++;
      continue;
    }
    if(line.IsNull()) break;
    sscanf(line.Data(),"%g %g", par1+count,par2+count);
    count++;
  }
  cout << endl << fileName.Data() << endl;
  for(Int_t i=0;i<size;i++) cout << par1[i] << " " << par2[i] << endl;

  return;
}

