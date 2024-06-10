#ifndef KF_H
#define KF_H

#include "Track.h"

class KalmanFilter
{
	public:
		KalmanFilter();
		inline void Fit(Track &track);
		void SetjEvent(int &j);
		void SetiDet(int &i);
	private:
		inline void Initilize(Track &track) const;
		inline void Predict(Track &track, float zNew) const;
		inline void Noise(Track &track) const;
		inline void Update(Track &track, float xNew) const;

		float fScatter;
		float fSigma;
		int jEvent, iDet;
};

#endif
