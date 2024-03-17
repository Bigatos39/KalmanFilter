#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "Geometry.h"
#include "Track.h"
#include "TRandom3.h"


class Simulator
{
public:
    Simulator();
    void Simulate(const PositionParam &param, Track &track);
    void SetGeometry(const Geometry &g);
    void SetSigmaScatter(const float &s);
    void SetSigma(const float &s);
private:
    void Scatter(PositionParam &tp);

    Geometry fGeometry;
    float fScatter;
    float fSigma;
    TRandom3 *fRandom;
};

#endif
