#ifndef G4Track_H
#define G4Track_H

#include <vector>
#include <TVector.h>

using namespace std;

struct G4Point {
	TVectorT<float> r;
};
struct G4Points {
	vector<G4Point> pointInDet;
};
struct G4Track {
	vector<G4Points> pointTrack;
	vector<float> Nevents;
	vector<float> stdDevX;
	vector<float> stdDevTx;
};

#endif
