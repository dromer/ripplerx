// Copyright 2025 tilr
// Resonator holds a number of Partials and a Waveguide
// depending on the selected model uses the Partials bank or Waveguide to process input
// also holds the partials ratios tuned for each model, 
// some of these are recalculated based on ratio parameter the others are fixed

#pragma once
#include <vector>
#include <array>
#include "../Globals.h"
#include "Partial.h"
#include "Waveguide.h"
#include "Filter.h"

enum Models {
	String,
	Beam,
	Squared,
	Membrane,
	Plate,
	Drumhead,
	Marimba,
	OpenTube,
	ClosedTube
};

class Resonator
{
public:
	Resonator();
	~Resonator() {};

	std::array<double, 64> bfree;
	std::array<std::array<double, 64>, 9> models;
	std::array<double, 64> fshifts = {};

	void setParams(double srate, bool on, int model, int partials, double decay, double damp, double tone, double hit,
		double rel, double inharm, double ratio, double cut,double radius, double vel_decay, double vel_hit, double vel_inharm);

	void activate();
	void update(double frequency, double vel, bool isRelease, std::array<double, 64> _model);
	void clear();
	double process(double x);

	void recalcBeam();
	void recalcMembrane();
	void recalcPlate();

	int silence = 0; // counter of samples of silence
	bool active = false; // returns to false if samples of silence run for a bit
	double srate = 0.0;
	bool on = false;
	int nmodel = 0;
	int npartials = 0;
	double decay = 0.0;
	double radius = 0.0;
	double rel = 0.0;
	double ratio = 0.0;
	double cut = 0.0;

	std::vector<Partial> partials;
	Waveguide waveguide{};
	Filter filter{};

private:

};

