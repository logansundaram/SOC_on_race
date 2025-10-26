#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
#include <iomanip>
#include <math.h>
#include <cmath>
#include <vector>

double Calc_SOC(double Current, double delta_t);
double Calc_V1(double prev_V1, double R1, double I_L, double C_1, double delta_t);
double Calc_V2(double prev_V2, double R2, double I_L, double C_2, double delta_t);
double Calc_IL(double R0, double OCV, double V1, double V2, double P);
double return_OCV(double SOC, double Power);
double return_Param(int parameter_flag, double SOC);
double calc_P_diss(double V1, double V2, double IL, double R0, double R1, double R2);
void print_summary(double p_diss, double e_diss, double SOC, double e_del);
bool run_sim_dynamic(double &V1, double &V2, double &I_L, double &SOC, double &p_out, double &delta_t, double &p_diss, double &e_diss, double &e_del, int &num_cells, std::pair<double, double> &power, int &i, double &R0, double &R1, double &R2, double &C1, double &C2, double &OCV);
void read_power();
