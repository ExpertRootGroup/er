void merge_graphs(TString outputDirectory = "digi_graphs")
{
	TString pwd(gSystem->pwd());
	TString workDir = pwd + "/" + outputDirectory;

	TSystemDirectory dire(workDir, workDir);
	TList *files = dire.GetListOfFiles();

	TMultiGraph* mg = new TMultiGraph();

	TIter next(files);
	while (TSystemFile* curFile = (TSystemFile*)next())
	{
		TString curFileName = curFile->GetName();

		if (curFileName.Contains("graph") && curFileName.Contains(".root")) {
			TString fullPath = workDir + "/" + curFile->GetName();
			cout << fullPath << endl;

			TFile f(fullPath, "READ");

			if (f.IsZombie()) {
				cerr << "Error opening file '" << fullPath << "'" << endl;
				return;
			}

			TString graphName("Graph");
			TGraph* g = (TGraph*)f.Get(graphName);
			if (g == NULL) {
				cerr << "Graph '" << graphName << "' not found." << endl;
				return;
			} else {
				mg->Add(g);
			}
			f.Close();

		}

	}

	TCanvas* canv = new TCanvas("canv", "canv");
	canv->cd();
	mg->Draw("AP");
	mg->SetTitle("Gas Eloss vs. Semi Eloss; GeV; GeV");

/////////////////////////////////////////////////////////////////
	mg->GetXaxis()->SetLimits(0., 0.050); // Up to 50 MeV
	mg->SetMinimum(0.);
	mg->SetMaximum(0.010); // Up to 10 MeV;
/////////////////////////////////////////////////////////////////

	gPad->SetGrid(2, 2);
	gPad->Modified();

	TString toSaveCanvDir;
	toSaveCanvDir.Form("%s/eloss.pdf", outputDirectory.Data());
	canv->SaveAs(toSaveCanvDir);

	TString oFileName;
	oFileName.Form("%s/eloss.root", outputDirectory.Data());
	TFile* oFile = new TFile(oFileName, "recreate");
	if (oFile->IsZombie()) {
		cerr << "CAN NOT CREATED ROOT FILE " << oFileName << endl;
		return;
	}
	canv->Write();
	oFile->Close();
}
