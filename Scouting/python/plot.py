from ROOT import TTree, TFile, TCanvas, gROOT, TH1D, std, AddressOf
from array import array
from ctypes import *

#put root in batch mode
gROOT.SetBatch(1)


#read file and get the ntuple tree
data_file = './scouting_ntuple_pt_50.root'
rfile = TFile(data_file)
tree = rfile.Get("scoutingntuplizer")

# Disable all branches except the ones we are interested in for perforrmance
# Seriously this cuts the runtime in half
tree.SetBranchStatus("*", 0)
tree.SetBranchStatus("HT", 1)
tree.SetBranchStatus("jet_pt", 1)
tree.SetBranchStatus("jet_num", 1)

#book histogram
HT_h = TH1D('HT', 'HT', 40, 175, 500)
pt0_h = TH1D("pt of leading jet", "pt0", 90, 50, 300)

nevents = tree.GetEntries()
for i in range(0, nevents):
    tree.GetEntry(i)
    HT_h.Fill(tree.HT)
    if tree.jet_num > 0:
        pt0_h.Fill(tree.jet_pt[0])


c1 = TCanvas("c1", "c", 800, 450);
HT_h.Draw()
c1.SaveAs("HT_50.png")
c1.Clear()
pt0_h.Draw()
c1.SaveAs("leading_jet_50.png")
c1.Clear()

