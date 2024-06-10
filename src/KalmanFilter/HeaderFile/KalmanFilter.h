#ifndef KALMANFILTER_H
#define KALMANFILTER_H

#include "Track.h"

class KalmanFilter
{
	public:
		KalmanFilter();
		void Fit(Track &track);
		void SetjEvent(int &j);
		void SetiDet(int &i);
		void UpdateStdDev(float &sigma);
	private:
		void Initilize(Track &track) const;
		void Predict(Track &track, float zNew) const;
		void Noise(Track &track) const;
		void Update(Track &track, float xNew) const;

		float fScatter;
		float fSigma;
		int jEvent, iDet;
};

#endif
