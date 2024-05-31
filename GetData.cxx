#include "./HeaderFile/G4Track.h"
#include "./HeaderFile//GetData.h"

#include <iostream>
#include <TFile.h>
#include <TString.h>
#include <TTree.h>

using namespace std;

const int DET = 10;

inline void GetDataFromG4beamline::GetData(G4Track &g4Track) const {
	TFile* f = new TFile(FILENAME);

	if (!f) {
		cout << "Failed to open file " << FILENAME << endl;
		return;
	}

	for (int i = 0; i < DET; i++) {
		TString treeName = Form("VirtualDetector/Det%d", i + 1);
		TTree* tree = static_cast<TTree*> (f -> Get(treeName));

		if (!tree) {
			cout << "Failed to load tree " << treeName << endl;
			break;
		}

		int Nevent = tree -> GetEntries();
		g4Track.pointTrack.resize(Nevent);
		g4Track.Nevents.push_back(Nevent);

		for (int j = 0; j < tree -> GetEntries(); j++) {
			g4Track.pointTrack[j].pointInDet.resize(DET);
			g4Track.pointTrack[j].pointInDet[i].r.ResizeTo(2);

			float x;
			tree -> SetBranchAddress("x", &x);
			tree -> GetEntry(j);

			g4Track.pointTrack[j].pointInDet[i].r[0] = x;
		}

		delete tree;
	}

	f -> Close();
	delete f;
}

inline void GetDataFromG4beamline::CalculateScatter(G4Track &g4Track) const {
	for (int i = 0; i < DET - 1; i++) {
		for (int entry = 0; entry < g4Track.Nevents[i]; entry++) {
			float tx; // tan(scatter)
			float dz = 300; // const in g4beamline
			float x1 = g4Track.pointTrack[entry].pointInDet[i].r[0];
			float x2 = g4Track.pointTrack[entry].pointInDet[i + 1].r[0];	
			tx = (x2 - x1) / dz;
			// Save
			g4Track.pointTrack[entry].pointInDet[i].r[1] = tx;
		}
	}
}

inline void GetDataFromG4beamline::CalculateStdDev(G4Track &g4Track) const {
	for (int i = 0; i < DET; i++) {
		float stdDevMeanTx;
		for (int j = 0; j < g4Track.Nevents[i]; j++) {
			stdDevMeanTx += atan(g4Track.pointTrack[j].pointInDet[i].r[1]);
		}
		stdDevMeanTx = stdDevMeanTx / g4Track.Nevents[i];

		float stdDevTx;
		for (int j = 0; j < g4Track.Nevents[j]; j++) {
			stdDevTx += pow(stdDevMeanTx - atan(g4Track.pointTrack[j].pointInDet[i].r[1]), 2);
		}
		stdDevTx = sqrt(stdDevTx / pow(g4Track.Nevents[i], 2));

		cout << stdDevTx << endl;

		g4Track.stdDevTx.push_back(stdDevTx);
		g4Track.stdDevX.push_back(0.5);
	}
}
