void create_target_3h_geo()
{
  TString erPath = gSystem->Getenv("VMCWORKDIR");

  // Output paths
  TString outGeoFilenameRoot = erPath + "/geometry/target.3h.geo.root";
  TString outGeoFilenameGdml = erPath + "/geometry/target.3h.gdml";

  // Input paths
  TString medFile = erPath + "/geometry/media.geo";

  // Materials and media
  FairGeoLoader* geoLoad = new FairGeoLoader("TGeo", "FairGeoLoader");
  FairGeoInterface* geoFace = geoLoad->getGeoInterface();
  geoFace->setMediaFile(medFile);
  geoFace->readMedia();
  FairGeoMedia* geoMedia = geoFace->getMedia();
  FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();

  // Geometry manager
  TGeoManager* geoM = (TGeoManager*)gROOT->FindObject("FAIRGeom");

  TString mediumName;

  mediumName = "mylar";
  FairGeoMedium* mmylar = geoMedia->getMedium(mediumName);
  if (!mmylar) Fatal("create_target_3h_geo", "FairMedium %s not found", mediumName.Data());
  geoBuild->createMedium(mmylar);
  TGeoMedium* pmylar = geoM->GetMedium(mediumName);
  if (!pmylar) Fatal("create_target_3h_geo", "Medium %s not found", mediumName.Data());

  mediumName = "tritium";
  FairGeoMedium* mtritium = geoMedia->getMedium(mediumName);
  if (!mtritium) Fatal("create_target_3h_geo", "FairMedium %s not found", mediumName.Data());
  geoBuild->createMedium(mtritium);
  TGeoMedium* ptritium = geoM->GetMedium(mediumName);
  if (!ptritium) Fatal("create_target_3h_geo", "Medium %s not found", mediumName.Data());

  // General dimensions
  Double_t transX = 0.; // cm
  Double_t transY = 0.; // cm
  Double_t transZ = 1.; // cm
  Double_t target3HR = 2.; // cm
  Double_t target3HZ = .02; // cm
  Double_t shellOuterThickness = 0.5; // cm default=[0.5]
  Double_t shellZThickness = 0.25; // cm default=[0.25]

  // Shapes
  TGeoTube* target3HShape = new TGeoTube("target3HShape",
    0.,
    target3HR,
    target3HZ/2.);
  TGeoTube* targetShellShape = new TGeoTube("targetShellShape",
    0.,
    target3HR + shellOuterThickness,
    (target3HZ + 2*shellZThickness)/2.);

  // Volumes
  TGeoVolume* target3HVol = new TGeoVolume("target3HVol", target3HShape, ptritium);
  TGeoVolume* targetShellVol = new TGeoVolume("targetShellVol", targetShellShape, pmylar);

  // Matrices
  TGeoRotation* rotNoRot = new TGeoRotation("rotNoRot", 0., 0., 0.);
  rotNoRot->RegisterYourself();

  // Structure
  targetShellVol->AddNode(target3HVol, 1);

  // This is the one but last level in the hierarchy
  // This volume-assembly is the only volume to be inserted into TOP
  TGeoVolumeAssembly* subdetectorVolAss = new TGeoVolumeAssembly("target_3h");
  subdetectorVolAss->AddNode(targetShellVol, 1,
    new TGeoCombiTrans("mTarget3HVolInTarget", transX, transY, transZ, rotNoRot));

  // World ------------------------------------
  TGeoVolumeAssembly* topVolAss = new TGeoVolumeAssembly("TOP");
  topVolAss->AddNode(subdetectorVolAss, 1);

  // Finalize
  geoM->SetTopVolume(topVolAss);
  geoM->CloseGeometry();
  geoM->CheckOverlaps();
  geoM->PrintOverlaps();
  //geoM->CheckGeometry();
  //geoM->CheckGeometryFull();
  //geoM->Test();

  // Export
  //geoM->Export(outGeoFilenameGdml);
  TFile* outGeoFileRoot = new TFile(outGeoFilenameRoot, "RECREATE");
  geoM->GetTopVolume()->Write();
  outGeoFileRoot->Close();

  // Draw
  TBrowser* bro = new TBrowser("bro", "bro");
  geoM->GetTopVolume()->Draw("ogl");
}
