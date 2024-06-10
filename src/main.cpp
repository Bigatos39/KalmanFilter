#include "./SimulateMonteCarlo/Simulator.cc"
#include "./KalmanFilter/KalmanFilter.cc"


// HeaderFile
#include "./KalmanFilter/HeaderFile/KalmanFilter.h"
#include "./KalmanFilter/HeaderFile/Track.h"
#include "./SimulateMonteCarlo/HeaderFile/Simulator.h"
#include "./SimulateMonteCarlo/HeaderFile/Geometry.h"

#include "TROOT.h"
#include "TRootCanvas.h"
#include "TRint.h"
#include "TRandom3.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TF1.h"
#include "TMath.h"

#include <iostream>
#include <algorithm>

double GausFunctionForPull(double *x, double *par)
{
	return ((1.0 / (par[1] * sqrt(2 * TMath::Pi()))) * exp(-0.5 * pow((x[0] - par[0]) / par[1], 2)) * par[2]);
}

double GausFitForPullFunction(double *x, double *par)
{
	return GausFunctionForPull(x, par) + GausFunctionForPull(x, &par[3]);
}

int main()
{
	TRint app("myApp",0,0);

	double SigmaScatter = 0.01;
	const double Sigma = 0.5;
    const int bin = 100;
	int Det = 10;
	
	Geometry geo;
	geo.zs.resize(Det);
	for (int i = 0; i < Det; i++)
	{
		geo.zs[i] =  30 * i;
	}

	Simulator sim;
	sim.SetGeometry(geo);
	sim.SetSigmaScatter(SigmaScatter);
	sim.SetSigma(Sigma);

	KalmanFilter kf;
	TRandom3 rand;

	const int Nevent = 200000;
	Track tracks[Nevent];
	
	for (int i = 0; i < Nevent; i++)
	{
		kf.SetjEvent(i);

		Track &track = tracks[i];
		PositionParam par;
		par.Z() = 0;
		par.X() = rand.Uniform(-10, 10);
		par.Tx() = rand.Uniform(-0.5, 0.5);

		sim.Simulate(par, track);
		kf.Fit(track);
	}

	// Show Result
	TCanvas *c_XPull = new TCanvas();
	TCanvas *c_TxPull = new TCanvas();
	TCanvas *c_chi2 = new TCanvas();
	TCanvas *c_prob = new TCanvas();
	TH1 *h_XPull = new TH1F("X Pull", "X Pull", bin, -5, 5);
	TH1 *h_TxPull = new TH1F("Tx Pull", "Tx Pull", bin, -5, 5);
	TH1 *h_chi2 = new TH1F("Chi Square", "Chi Square", bin, 0, 40);
	TH1 *h_prob = new TH1F("Prob", "Prob", bin, -0.05, 1.05);

	for (int i = 0; i < Nevent; i++)
	{
		Track &track = tracks[i];
		if (track.rCovMatrix.Cxx() > 0)
		{
			double XPull = (track.Param.X() - track.Points.back().X()) / sqrt(track.rCovMatrix.Cxx());
			h_XPull -> Fill(XPull);
		}

		if (track.rCovMatrix.Ctt() > 0)
		{
			double TxPull = (track.Param.Tx() - track.Points.back().Tx()) / sqrt(track.rCovMatrix.Ctt());
			h_TxPull -> Fill(TxPull);
		}

		double chi2 = track.chi2;
		h_chi2 -> Fill(chi2);

		double prob = TMath::Prob(chi2, track.ndf);
		h_prob -> Fill(prob);
	}

	TF1 *fitFunc = new TF1("fitFunc", GausFitForPullFunction, -5, 5, 6);
    fitFunc -> SetParameters(0., 1., 2000, 0., 10, 1);

	c_XPull -> cd();
	h_XPull -> Draw();
    h_XPull -> Fit(fitFunc);

	c_TxPull -> cd();
	h_TxPull -> Draw();
    h_TxPull -> Fit(fitFunc);

	c_chi2 -> cd();
	h_chi2 -> Draw();
	
	c_prob -> cd();
	h_prob -> Draw();

	c_chi2 -> Modified();
	c_prob -> Modified();
	c_XPull -> Modified();
	c_TxPull -> Modified();

	c_chi2 -> Update();
	c_prob -> Update();
	c_XPull -> Update();
	c_TxPull -> Update();

	h_chi2 -> GetXaxis() -> SetTitle("chi2");
	h_chi2 -> GetYaxis() -> SetTitle("Entries");
	h_prob -> GetXaxis() -> SetTitle("prob");
	h_prob -> GetYaxis() -> SetTitle("Entries");

	h_XPull -> GetXaxis() -> SetTitle("pull_x");
	h_XPull -> GetYaxis() -> SetTitle("Entries");

	h_TxPull -> GetXaxis() -> SetTitle("pull_tx");
	h_TxPull -> GetYaxis() -> SetTitle("Entries");

	
	c_chi2 -> SaveAs("/Users/nguyenhoangkhavi/Desktop/Track - Fitting/Khoá luận/Figures/Chi2_WithKGaus.pdf");
	c_prob -> SaveAs("/Users/nguyenhoangkhavi/Desktop/Track - Fitting/Khoá luận/Figures/Prob_WithKGaus.pdf");
	c_XPull -> SaveAs("/Users/nguyenhoangkhavi/Desktop/Track - Fitting/Khoá luận/Figures/PullX_WithKGaus.pdf");
	c_TxPull -> SaveAs("/Users/nguyenhoangkhavi/Desktop/Track - Fitting/Khoá luận/Figures/PullTx_WithKGaus.pdf");

	app.Run();

	// Delete Pointer
    delete fitFunc;
    delete c_TxPull;
    delete c_prob;
    delete c_XPull;
    delete c_chi2;
    delete h_prob;
    delete h_chi2;
    delete h_XPull;
    delete h_TxPull;

	return 0;
}
