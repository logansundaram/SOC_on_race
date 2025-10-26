#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
#include <iomanip>
#include <math.h>
#include <cmath>
#include <vector>

#include "simulate.cpp"

double Calc_SOC(double Current, double delta_t);
double Calc_V1(double prev_V1, double R1, double I_L, double C_1, double delta_t);
double Calc_V2(double prev_V2, double R2, double I_L, double C_2, double delta_t);
double Calc_IL(double R0, double OCV, double V1, double V2, double P);