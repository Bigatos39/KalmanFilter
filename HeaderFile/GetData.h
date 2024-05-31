#ifndef GD_H
#define GD_H

#include "G4Track.h"

/* ---------------------- Get Data From Root File ---------------------- */
class GetDataFromG4beamline {
	public:
		GetDataFromG4beamline(const char* fileName): FILENAME(fileName) {};
//		virtual ~GetDataFromG4beamline();

		inline void GetData(G4Track &g4Track) const;
		inline void CalculateScatter(G4Track &g4Track) const;
		inline void CalculateStdDev(G4Track &g4Track) const;

	private:
		const char* FILENAME;
};

#endif
