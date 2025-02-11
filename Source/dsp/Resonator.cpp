#include "Resonator.h"
#include <cmath>
#include <JuceHeader.h>

Resonator::Resonator()
{
	for (int i = 0; i < globals::MAX_PARTIALS; ++i) {
		partials.push_back(Partial(i + 1));
	}
	bfree = {1.50561873, 2.49975267, 3.5, 4.5, 5.5, 6.5, 7.5, 8.5, 9.5, 10.5, 11.5, 12.5, 13.5, 14.5, 15.5, 16.5, 17.5, 18.5, 19.5, 20.5, 21.5, 22.5, 23.5, 24.5, 25.5, 26.5, 27.5, 28.5, 29.5, 30.5, 31.5, 32.5, 33.5, 34.5, 35.5, 36.5, 37.5, 38.5, 39.5, 40.5, 41.5, 42.5, 43.5, 44.5, 45.5, 46.5, 47.5, 48.5, 49.5, 50.5, 51.5, 52.5, 53.5, 54.5, 55.5, 56.5, 57.5, 58.5, 59.5, 60.5, 61.5, 62.5, 63.5, 64.5};
	models = {{
		// string model: fk *= k
		{1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0, 18.0, 19.0, 20.0, 21.0, 22.0, 23.0, 24.0, 25.0, 26.0, 27.0, 28.0, 29.0, 30.0, 31.0, 32.0, 33.0, 34.0, 35.0, 36.0, 37.0, 38.0, 39.0, 40.0, 41.0, 42.0, 43.0, 44.0, 45.0, 46.0, 47.0, 48.0, 49.0, 50.0, 51.0, 52.0, 53.0, 54.0, 55.0, 56.0, 57.0, 58.0, 59.0, 60.0, 61.0, 62.0, 63.0, 64.0},
		// beam model: fmn *= sqrt(m**4 + (2*Bfree[n])**4)
		// where Bfree[n] is the nth solution of cos($pi*x) = 1/cos($pi*x)
		// https://nathan.ho.name/posts/exploring-modal-synthesis
		{1.0, 2.742118730723096, 5.372437705395023, 8.879796231350905, 13.26432297929055, 18.525894832548836, 24.664464701923304, 31.680011427264994, 39.57480167993186, 48.34386187557112, 57.98998201587341, 68.51312839987956, 79.91327932335997, 92.19042029669127, 105.34454134738507, 119.37563542989709, 134.286605658006, 150.07132597088105, 166.73305349293955, 184.27177735520195, 202.68748915580662, 221.9801823173097, 242.14985163115497, 263.1964929295172, 285.1237904740257, 307.92409320904443, 331.6013888047332, 356.1556713642617, 381.58693597293706, 407.89517850825996, 435.08039549149447, 463.1425839706985, 492.0862467609114, 521.9021136490643, 552.5949669402571, 584.164802709515, 616.6116175480827, 649.9354084840451, 684.1361729168341, 719.2139085629082, 755.1739518807949, 792.0053758900387, 829.7137826702212, 868.2991693165571, 907.7615332399005, 948.1008721264657, 989.3171839034187, 1031.4104667093834, 1074.3868982163283, 1118.233875919467, 1162.9578340759736, 1208.5587703942417, 1255.0366827948008, 1302.3915693871795, 1350.6234284496627, 1399.7322584115318, 1449.7250824905104, 1500.5876119645127, 1552.3271202881517, 1604.9436055754263, 1658.4370660924033, 1712.8075002427393, 1768.0549065547818, 1824.179283670064 },
		// squared model: fk *= k**2
		{1.0, 4.0, 9.0, 16.0, 24.999999999999996, 36.0, 49.0, 64.0, 81.0, 99.99999999999999, 121.0, 144.0, 169.0, 196.0, 225.0, 256.0, 289.0, 324.0, 361.0, 399.99999999999994, 441.0, 484.0, 529.0, 576.0, 625.0, 676.0, 729.0, 784.0, 840.9999999999999, 900.0, 961.0, 1024.0, 1089.0, 1156.0, 1225.0, 1296.0, 1369.0, 1444.0, 1521.0, 1599.9999999999998, 1680.9999999999998, 1764.0, 1849.0, 1936.0, 2024.9999999999998, 2116.0, 2209.0, 2304.0, 2401.0, 2500.0, 2601.0, 2704.0, 2808.9999999999995, 2916.0, 3025.0, 3136.0, 3248.9999999999995, 3363.9999999999995, 3481.0, 3600.0, 3720.9999999999995, 3844.0, 3968.9999999999995, 4096.0},
		// membrane model: fmn *= sqrt(m**2 + (0.78*n)**2)
		{1.0, 1.46109285817633, 2.0065176070089703, 2.583401188563129, 3.17463993575823, 3.773492277607296, 4.376834180480457, 4.983035133780558, 1.6926924291749292, 2.0, 2.427204269735683, 2.92218571635266, 3.455943631122641, 4.013035214017941, 4.584962933678762, 5.166802377126258, 2.4441536557186816, 2.6662106880321503, 3.0, 3.4128944891151236, 3.8797198113361246, 4.38327857452899, 4.912287098306147, 5.459352181097535, 3.213415363586011, 3.3853848583498585, 3.6540595515354726, 4.0, 4.405039956758808, 4.854408539471366, 5.336920061454232, 5.84437143270532, 3.990195644111825, 4.129945958302949, 4.352904109387891, 4.647109099145228, 5.0, 5.400083818557902, 5.837665519855606, 6.304942539213217, 4.770823412246371, 4.888307311437363, 5.078077287524788, 5.332421376064301, 5.6426141771832246, 6.0, 6.3966736257417525, 6.825788978230247, 5.553676490089963, 5.654919530531964, 5.819745309187935, 6.042953272833355, 6.318359065318225, 6.639470349789949, 7.0, 7.394184316141261, 6.337930329019764, 6.426830727172022, 6.5723263585085, 6.770769716699717, 7.017670523558341, 7.308119103070945, 7.63714857122281, 8.0},
		// plate model: fmn *= m**2 + (0.78*n)**2
		{1.0, 2.134792340213877, 4.026112907237005, 6.673961701069386, 10.078338721711017, 14.239243969161897, 19.156677443422033, 24.83063914449142, 2.8652076597861225, 4.0, 5.891320567023128, 8.539169360855508, 11.943546381497141, 16.10445162894802, 21.021885103208156, 26.695846804277544, 5.973887092762994, 7.108679432976872, 9.0, 11.647848793832381, 15.052225814474012, 19.21313106192489, 24.130564536185027, 29.804526237254414, 10.326038298930614, 11.46083063914449, 13.352151206167619, 16.0, 19.404377020641633, 23.565282268092513, 28.482715742352646, 34.15667744342203, 15.921661278288981, 17.056453618502857, 18.947774185525986, 21.595622979358367, 25.0, 29.16090524745088, 34.07833872171101, 39.7523004227804, 22.760756030838103, 23.895548371051976, 25.786868938075106, 28.434717731907487, 31.839094752549116, 36.0, 40.91743347426013, 46.591395175329524, 30.843322556577967, 31.97811489679184, 33.86943546381497, 36.517284257647354, 39.92166127828899, 44.082566525739864, 49.0, 54.67396170106939, 40.16936085550858, 41.304153195722456, 43.195473762745586, 45.84332255657796, 49.2476995772196, 53.408604824670476, 58.32603829893061, 64.0},
		// drumhead model: fmn *= Jmn
		// where Jmn is the bessel root(m,n)
		{1.0, 1.593340505695112, 2.1355487866494034, 2.295417267427694, 2.6530664045492145, 2.9172954551172228, 3.155464815408362, 3.5001474903090264, 3.5984846739581138, 3.6474511791052775, 4.058931883331434, 4.131738159726707, 4.230439127905234, 4.6010445344331075, 4.610051645437306, 4.831885262930598, 4.903280573212368, 5.1307689067016575, 5.412118429982582, 5.5403985098530635, 5.650842376925684, 5.976540221648715, 6.152609171589257, 6.1631367313038865, 6.208732130572546, 6.528612451522295, 6.746213299505839, 6.848991602808508, 7.0707081490386905, 7.325257332462771, 7.468242109085181, 7.514500962483965, 7.604536126938166, 7.892520026843893, 8.071028338967128, 8.1568737689496, 8.45000551018646, 8.66047555520746, 8.781093075730398, 8.820447105611922, 8.999214496283312, 9.238840557670077, 9.390589484063241, 9.464339027734203, 9.807815107462856, 9.98784275554081, 10.092254814868133, 10.126502295693772, 10.368705458854519, 10.574713443493692, 10.706875023386747, 10.77153891878896, 11.152639282954734, 11.310212368186301, 11.402312929615599, 11.722758172320448, 11.903823217314876, 12.020976194473256, 12.48894011894477, 12.6291936518746, 13.066558649839825, 13.228284530761863, 13.819314942198952, 14.40316086180383},
		// marimba model: I couldn't program this one, instead the model was created
		// using frequency analysis on a marimba sample from chromaphone
		{1.0, 3.9393939393939394, 10.575757575757576, 19.0, 26.757575757575758, 36.93939393939394, 49.18181818181818, 63.18181818181818, 78.9090909090909, 96.39393939393939, 115.63636363636364, 136.6060606060606, 159.36363636363637, 183.8181818181818, 210.06060606060606, 238.03030303030303, 267.75757575757575, 299.24242424242425, 442.6363636363636, 568.5454545454545, 614.0, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 100000},
		// closed tube harmonics, used for freqency shifts
		{1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0, 18.0, 19.0, 20.0, 21.0, 22.0, 23.0, 24.0, 25.0, 26.0, 27.0, 28.0, 29.0, 30.0, 31.0, 32.0, 33.0, 34.0, 35.0, 36.0, 37.0, 38.0, 39.0, 40.0, 41.0, 42.0, 43.0, 44.0, 45.0, 46.0, 47.0, 48.0, 49.0, 50.0, 51.0, 52.0, 53.0, 54.0, 55.0, 56.0, 57.0, 58.0, 59.0, 60.0, 61.0, 62.0, 63.0, 64.0},
		// open tube harmonics, used for frequency shifts
		{1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 49, 51, 53, 55, 57, 59, 61, 63, 65, 67, 69, 71, 73, 75, 77, 79, 81, 83, 85, 87, 89, 91, 93, 95, 97, 99, 101, 103, 105, 107, 109, 111, 113, 115, 117, 119, 121, 123, 125, 127}
	}};
}

void Resonator::setParams(double _srate, bool _on, int model, int _partials, double _decay, double damp, double tone, double hit,
	double _rel, double inharm, double _ratio, double _cut, double _radius, double vel_decay, double vel_hit, double vel_inharm)
{
	on = _on;
	nmodel = model;
	npartials = _partials;
	decay = _decay;
	radius = _radius;
	rel = _rel;
	srate = _srate;
	ratio = _ratio;
	cut = _cut;

	filter.hp(srate, cut, 0.707);

	for (Partial& partial : partials) {
		partial.damp = damp;
		partial.decay = decay;
		partial.hit = hit;
		partial.inharm = inharm;
		partial.rel = _rel;
		partial.tone = tone;
		partial.vel_decay = vel_decay;
		partial.vel_hit = vel_hit;
		partial.vel_inharm = vel_inharm;
		partial.srate = _srate;
	}

	waveguide.decay = decay;
	waveguide.radius = radius;
	waveguide.is_closed = model == Models::ClosedTube;
	waveguide.srate = srate;
	waveguide.vel_decay = vel_decay;
	waveguide.rel = rel;

	// recalc models that depend on ratio var
	if (nmodel == Models::Beam) recalcBeam();
	if (nmodel == Models::Membrane) recalcMembrane();
	if (nmodel == Models::Plate) recalcPlate();
}

void Resonator::update(double freq, double vel, bool isRelease)
{
	std::array<double, 64> model = models[nmodel]; // make a copy of model for frequency shifts when serial coupling

	if (nmodel < 7) {
		for (Partial& partial : partials) {
			partial.update(freq, model[partial.k - 1], model[model.size() - 1], vel, isRelease);
		}
	}
	else {
		waveguide.update(freq, vel, isRelease);
	}
}

void Resonator::activate()
{
	active = true;
	silence = 0;
}

double Resonator::process(double input)
{
	double out = 0.0;

	if (active) { // use active and silence to turn off strings process if not in use
		if (nmodel < 7) {
			for (int p = 0; p < npartials; ++p) {
				out += partials[p].process(input);
			}
		}
		else {
			// waveguide process
			out += waveguide.process(input);
		}
	}

	if (abs(out) + abs(input) > 0.00001) 
		silence = 0;
	else 
		silence += 1;
	if (silence >= srate) 
		active = false;

	return out;
}

void Resonator::clear()
{
	for (Partial& partial : partials) {
		partial.clear();
	}
	waveguide.clear();
}

void Resonator::recalcBeam()
{
	int i = 0;
	for (int m = 1; m <= 8; ++m) {
		for (int n = 1; n <= 8; ++n) {
			models[Models::Beam][i] = sqrt(pow(m, 4.0) + pow(ratio * bfree[i], 4.0));
			i += 1;
		}
	}
	auto f0 = models[Models::Beam][0];
	for (int j = 0; j < 64; ++j) {
		models[Models::Beam][j] = models[Models::Beam][j] / f0; // freqs to ratio
	}
}

void Resonator::recalcMembrane()
{
	int i = 0;
	for (int m = 1; m <= 8; ++m) {
		for (int n = 1; n <= 8; ++n) {
			models[Models::Membrane][i] = sqrt(pow(m, 2.0) + pow(ratio * n, 2.0));
			i += 1;
		}
	}
	auto f0 = models[Models::Membrane][0];
	for (int j = 0; j < 64; ++j) {
		models[Models::Membrane][j] = models[Models::Membrane][j] / f0; // freqs to ratio
	}
}

void Resonator::recalcPlate()
{
	int i = 0;
	for (int m = 1; m <= 8; ++m) {
		for (int n = 1; n <= 8; ++n) {
			models[Models::Plate][i] = pow(m, 2.0) + pow(ratio * n, 2.0);
			i += 1;
		}
	}
	auto f0 = models[Models::Plate][0];
	for (int j = 0; j < 64; ++j) {
		models[Models::Plate][j] = models[Models::Plate][j] / f0; // freqs to ratio
	}
}