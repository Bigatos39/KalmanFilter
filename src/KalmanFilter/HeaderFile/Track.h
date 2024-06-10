#ifndef TRACK_H
#define TRACK_H

#include <vector>

struct Position
{
	Position(): x(), z() {};
	Position(float xNew, float zNew): x(xNew), z(zNew) {};
	float x, z;
};

struct PositionParam
{
	PositionParam(): z(0)
	{
		r[0] = 0;
		r[1] = 0;
	};

	PositionParam(float xNew, float txNew, float zNew): z(zNew)
	{
		r[0] = xNew;
		r[1] = txNew;
	};

	float X(float zNew) const
	{
		return r[1] * (zNew - z) + r[0];
	};
	float &X()
	{
		return r[0];
	};
	float &Tx()
	{
		return r[1];
	};
	float &Z()
	{
		return z;
	};

	float r[2];
	float z;
};

struct CovMatrix
{
	float &Cxx()
	{
		return c[0];
	};
	float &Cxt()
	{
		return c[1];
	};
	float &Ctt()
	{
		return c[2];
	};

	float c[3];
};

struct Track
{
	std::vector<Position> hits;
	std::vector<PositionParam> Points;
	PositionParam Param;
	CovMatrix rCovMatrix;

	float chi2;
	float ndf;
};

#endif
