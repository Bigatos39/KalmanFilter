#include "./HeaderFile/Track.h"
#include "./HeaderFile/KalmanFilter.h"
#include <cmath>
#include <iostream>

KalmanFilter::KalmanFilter(): fScatter(SigmaScatter), fSigma(Sigma){};

void KalmanFilter::SetjEvent(int &j)
{
	jEvent = j;
}
void KalmanFilter::SetiDet(int &i) {
	iDet = i;
}
void KalmanFilter::UpdateStdDev(float &sigma){
	fScatter = sigma;
}

void KalmanFilter::Fit(Track &track)
{
	int i = 0;
	Initilize(track);
	Predict(track, track.hits[i].z);
	Update(track, track.hits[i].x);
	i++;
	
	for (; i < 10; i++) {
		Predict(track, track.hits[i].z);
		Noise(track);
		Update(track, track.hits[i].x);
	}
}

void KalmanFilter::Initilize(Track &track) const
{
	// Position
	track.Param.X() = 0;
	track.Param.Z() = 0;

	// Scatter
	track.Param.Tx() = 0;

	// Covariance Matrix
	track.rCovMatrix.Cxx() = Inf;
	track.rCovMatrix.Cxt() = 0;
	track.rCovMatrix.Ctt() = Inf;

	track.chi2 = 0;
	track.ndf = -2;
}

void KalmanFilter::Predict(Track &track, float zNew) const
{
    float &x = track.Param.X();
    float &tx = track.Param.Tx();
    float &z = track.Param.Z();

    float &Cxx = track.rCovMatrix.Cxx();
    float &Cxt = track.rCovMatrix.Cxt();
    float &Ctt = track.rCovMatrix.Ctt();

	float d = zNew - z;

	x += d * tx; // x = x + tx * d
    z = zNew;

    // C = F * C * F.T
    const float Ctx = Cxt;
    Cxt += d * Ctt;
    Cxx += d * (Ctx + Cxt);
}

void KalmanFilter::Noise(Track &track) const
{
    // C = C + Q
	float &tx = track.Param.Tx();
	float &Ctt = track.rCovMatrix.Ctt();
	const float Q = (1 + pow(tx, 2)) * pow(fScatter, 2);
	Ctt += Q;
}

void KalmanFilter::Update(Track &track, float xNew) const
{
	float &x = track.Param.X();
	float &tx = track.Param.Tx();

	float &Cxx = track.rCovMatrix.Cxx();
	float &Cxt = track.rCovMatrix.Cxt();
	float &Ctt = track.rCovMatrix.Ctt();

    float residual = xNew - x;

    float S = pow(fSigma, 2) + Cxx; // H * C * H.T = C
    float InvS = 1 / S;

//	cout << "Meas: " << xNew << "\tEst: " << x << endl;

    // K = (1 / S) * C
    float K0 = Cxx * InvS;
    float K1 = Cxt * InvS;

    // Update State Vector: r = r - K * (x - xNew)
    x  += K0 * residual;
    tx += K1 * residual;

    // Update Covariance Matrix: C = C - K * H * C
    float C1 = Cxx;
    float C2 = Cxt;
    Cxx -= K0 * C1;
    Cxt -= K1 * C1;
    Ctt -= K1 * C2;

    track.chi2 += residual * InvS * residual;
	track.ndf += 1;
}
