#include "./HeaderFile/Geometry.h"
#include "./HeaderFile/Track.h"
#include "./HeaderFile/Simulator.h"
#include "TF1.h"

const float SigmaScatter = 0.01;
const float Sigma = 1;
const int det = 10;
const int d = 300;
const float Inf = 1000;

Simulator::Simulator(): fGeometry(), fScatter(SigmaScatter), fSigma(Sigma)
{
	fGeometry.zs.resize(det);
	for (int i = 0; i < det; i++)
	{
		fGeometry.zs[i] = d * i;
	}

	fRandom = new TRandom3();
}

double GausFunction(double *x, double *par)
{
	return (1.0 / (par[1] * sqrt(2 * TMath::Pi()))) * exp(-0.5 * pow((x[0] - par[0]) / par[1], 2) * par[2]);
}
double GaussianSumFilter(double *x, double *par) // Gaus(mean, stdDev, const)
{
	return GausFunction(x, par) + GausFunction(x, &par[3]);
}

void Simulator::Scatter(PositionParam &tp)
{
	TF1 *func = new TF1("func", GaussianSumFilter, -0.5, 0.5, 6);
	double param[] = {0, 0.01, 1, 0.02, 0.1, 1};
	func -> SetParameters(param);

//	const double alpha = fRandom -> Gaus(0, fScatter);
	const double alpha = func -> GetRandom();
	tp.Tx() = tan(atan(tp.Tx()) + alpha);
}

void Simulator::Simulate(const PositionParam &param, Track &track)
{
	track.Points.clear();
	PositionParam curPoint = param;
	for (int i = 0; i < fGeometry.GetNDet(); i++)
	{
		const float z = fGeometry.zs[i];
		curPoint.X() = curPoint.X(z);
		curPoint.Z() = z;

		Scatter(curPoint);
		track.Points.push_back(curPoint);
	}

	track.hits.clear();
	for (int i = 0; i < fGeometry.GetNDet(); i++)
	{
		const float z = track.Points[i].z;
		float x = fRandom -> Gaus(track.Points[i].X(), fSigma);

		track.hits.push_back(Position(x, z));
	}
}

void Simulator::SetGeometry(const Geometry &g)
{
	fGeometry = g;
}

void Simulator::SetSigmaScatter(const float &s)
{
	fScatter = s;
}

void Simulator::SetSigma(const float &s)
{
	fSigma = s;
}
