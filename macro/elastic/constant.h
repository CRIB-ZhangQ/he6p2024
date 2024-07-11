#ifndef _CONSTANT_H_
#define _CONSTANT_H_

#include <TMath.h>

const double Pi = 4.0* atan(1);
const double u = 931.49432;
const double deg2rad = Pi / 180.0;
const double rad2deg = 180.0 / acos(-1.0);
const double sigma_x = 5.791, sigma_y = 4.225;

//the beam information
const double E_6He = 43.65;
//const double E_6He = 90;
const double E_3H = 21.777;
const int    nstrips = 16;
const double downscale = 10000.0;
const double purity = 1.0;
const double Ntarget = 3.99263e20;
const double efficiency = 0.875;


//the mass of the reactions of the 6He(p,p0)6He, 
//6He(p,p1)6He, 6He(p,a)t, 6He(p, d)5He, 3H(p,p)t,
//the unit is of atom mass u 
struct mass{
    const double reac1[4]{6.01885889,    1.007825031898, 6.01885889,    1.007825031898};
    const double reac2[4]{6.01885889,    1.007825031898, 6.01885889,    1.007825031898}; 
    const double reac3[4]{6.01885889,    1.007825031898, 5.012223624,   2.01410177785}; 
    const double reac4[4]{6.01885889,    1.007825031898, 4.002602,      3.01604927767}; 
    const double reac5[4]{3.01604927767, 1.007825031898, 3.01604927767, 1.007825031898}; 
};  


//the charge of the reactions of the 6He(p,p0)6He, 
//6He(p,p1)6He, 6He(p,a)t, 6He(p, d)5He, 3H(p,p)t
struct charge{
    const int reac1[4]{2, 1, 2, 1}; 
    const int reac2[4]{2, 1, 2, 1}; 
    const int reac3[4]{2, 1, 2, 1}; 
    const int reac4[4]{2, 1, 2, 1}; 
    const int reac5[4]{1, 1, 1, 1}; 
    
};

//the Qvalue of the reactions of the 6He(p,p0)6He, 
//6He(p,p1)6He, 6He(p,a)t, 6He(p, d)5He, 3H(p,p)t
struct Qvalue{
    const double He6_p0{0.0};
    const double He6_p1{-1.8};
    
    const double H3_p{0.0};

    const double a_t{7.50634};

    const double He5_d{0.514};
};

//the thickness of silicon detectors for the telescope 1-6, the unit is um
struct teles{
    const float tel1[4]{301.0, 1494.0, 1486.0, 1485.0};
    const float tel2[4]{300.0, 1494.0, 1486.0, 1484.0};
    const float tel3[3]{75.0, 1494.0, 1507.0};
    const float tel4[2]{73.0, 1491.0};
    const float tel5[2]{59.0, 489.0};
    const float tel6[2]{42.0, 1465.0};
};

#endif

