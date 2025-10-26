#include "simulate.h"

std::vector<std::pair<double, double>> power; // First column is delta t (s), second column is Power (Wh)

double Q_n = 2.5 * 3600;       // for 18650-25R 9P36S, change for each case
double Remaining_Charge = Q_n; // Amp-Seconds
double P_diss = 0.0;
double E_diss = 0.0;
/*
PARAMETER FLAGS FOR READING IN CSV
C1 = 0
C2 = 1
R0 = 2
R1 = 3
R2 = 4
OCV = 5
power = 6
power_charge = 7
*/

#define C1_FLAG 0
#define C2_FLAG 1
#define R0_FLAG 2
#define R1_FLAG 3
#define R2_FLAG 4
#define OCV_DISCHARGE_FLAG 5
#define POWER_FLAG 6
#define OCV_CHARGE_FLAG 7

double Calc_SOC(double Current, double delta_t)
{
    Remaining_Charge = Remaining_Charge + Current * delta_t;
    double new_SOC = Remaining_Charge / Q_n;
    return new_SOC;
}

double Calc_V1(double prev_V1, double R1, double I_L, double C_1, double delta_t)
{
    double new_V1 = prev_V1 + delta_t * (-1 * prev_V1 / R1 + I_L) / C_1;
    return new_V1;
}

double Calc_V2(double prev_V2, double R2, double I_L, double C_2, double delta_t)
{
    double new_V2 = prev_V2 + delta_t * (-1 * prev_V2 / R2 + I_L) / C_2;
    return new_V2;
}

double Calc_IL(double R0, double OCV, double V1, double V2, double P)
{

    if (P == 0)
        return 0;
    // std::cout << "R0; " << R0 << " v1: " << V1 << " V2 " << V2 << "Power_out: " << P<<  "OCV: " << OCV<< std::endl;
    double I_L1 = (-1 * (OCV + V1 + V2) + sqrt(abs(pow(OCV + V1 + V2, 2) - 4 * (R0 * -P)))) / (2 * R0);
    double I_L2 = (-1 * (OCV + V1 + V2) - sqrt(abs(pow(OCV + V1 + V2, 2) - 4 * (R0 * -P)))) / (2 * R0);
    if (abs(I_L1) > abs(I_L2))
    {
        return I_L2;
    }
    else
    {
        return I_L1;
    }
}
double return_OCV(double SOC, double Power)
{

    // error checking
    if (SOC >= 1)
    {
        SOC = 1;
    }
    else if (SOC < 0)
    {
        SOC = 0;
    }

    // ocv polyfit charging -4783.78848787996x^10 + 27347.9312861984x^9 +	-67026.6988407755x^8 +
    //	92144.6069787111x^7	- 78029.8368377740x^6 + 42070.3825796194x^5 - 14424.1632098688x^4
    //	3053.81797893469x^3	- 375.676774229697x^2 + 24.9063595277509x - 2.72423333860835

    // ocv polyfit discharging 4313.04847052565x^10 + 24891.2165949856x^9 + -61632.9921177867x^8 + 85715.6445387034x^7
    //-73578.8063784470x^6 + 40329.2056130290x^5 -14113.3560968725x^4 + 3067.12971117682x^3	-390.407463955115x^2
    //+ 26.9957718142629x + 2.60049866650654

    if (Power > 0)
    {
        double OCV = -4783.78848787996 * pow(SOC, 10) + 27347.9312861984 * pow(SOC, 9) - 67026.6988407755 * pow(SOC, 8) +
                     92144.6069787111 * pow(SOC, 7) - 78029.8368377740 * pow(SOC, 6) + 42070.3825796194 * pow(SOC, 5) - 14424.1632098688 * pow(SOC, 4) + 3053.81797893469 * pow(SOC, 3) - 375.676774229697 * pow(SOC, 2) + 24.9063595277509 * SOC + 2.72423333860835;

        return OCV;
    }
    else
    {
        double OCV = -4313.04847052565 * pow(SOC, 10) + 24891.2165949856 * pow(SOC, 9) - 61632.9921177867 * pow(SOC, 8) + 85715.6445387034 * pow(SOC, 7) - 73578.8063784470 * pow(SOC, 6) + 40329.2056130290 * pow(SOC, 5) - 14113.3560968725 * pow(SOC, 4) + 3067.12971117682 * pow(SOC, 3) - 390.407463955115 * pow(SOC, 2) + 26.9957718142629 * SOC + 2.60049866650654;
        return OCV;
    }
}

double return_Param(int parameter_flag, double SOC)
{
    // error checking
    if (SOC > 1)
    {
        SOC = 1;
    }
    else if (SOC < 0)
    {
        SOC = 0;
    }

    switch (parameter_flag)
    {
    case C1_FLAG:
        if (0.834953 < SOC && SOC <= 1.0)
        {
            return 153309.2267828;
        }
        else if (0.6692170892 < SOC && SOC <= 0.834953)
        {
            return -175720.8565447232 * SOC + 299651.6285285904;
        }
        else if (0.5469484539 < SOC && SOC <= 0.6692170892)
        {
            return -1229161.6630156341 * SOC + 1004632.2186795539;
        }
        else if (0.3883811021 < SOC && SOC <= 0.5469484539)
        {
            return 623039.1046992310 * SOC + -8426.1275344846;
        }
        else if (0.2355625405 < SOC && SOC <= 0.3883811021)
        {
            return 345173.9582399001 * SOC + 99491.4442825682;
        }
        else if (0.0 < SOC && SOC <= 0.2355625405)
        {
            return -96048.6266527533 * SOC + 203426.9573058586;
        }
        break;
    case C2_FLAG:
        if (0.6692170892 < SOC && SOC <= 1.0)
        {
            return -1195.3029576136 * SOC + 1693.3156710063;
        }
        else if (0.5469484539 < SOC && SOC <= 0.6692170892)
        {
            return -323.7877343021 * SOC + 1110.0827900683;
        }
        else if (0.3883811021 < SOC && SOC <= 0.5469484539)
        {
            return -261.0309967982 * SOC + 1075.7580895187;
        }
        else if (0.2355625405 < SOC && SOC <= 0.3883811021)
        {
            return 76.5806448999 * SOC + 944.6361080343;
        }
        else if (0.0 < SOC && SOC <= 0.2355625405)
        {
            return 2374.8196396644 * SOC + 403.2570917513;
        }
        break;
    case R0_FLAG:
        if (0.6692170892 < SOC && SOC <= 1.0)
        {
            return 0.0000000004 * SOC + 0.0164292978;
        }
        else if (0.5469484539 < SOC && SOC <= 0.6692170892)
        {
            return 0.0004160072 * SOC + 0.0161508989;
        }
        else if (0.3883811021 < SOC && SOC <= 0.5469484539)
        {
            return -0.0003260470 * SOC + 0.0165567643;
        }
        else if (0.2355625405 < SOC && SOC <= 0.3883811021)
        {

            return -0.0009930624 * SOC + 0.0168158205;
        }
        else if (0.0 < SOC && SOC <= 0.2355625405)
        {
            return -0.0040134822 * SOC + 0.0175273182;
        }
        break;
    case R1_FLAG:
        if (0.834953 < SOC && SOC <= 1)
        {
            return 0.0000375142865;
        }
        else if (0.6692170892 < SOC && SOC <= 0.834953)
        {
            return 0.0000452527 * SOC + -0.0000002839;
        }
        else if (0.5469484539 < SOC && SOC <= 0.6692170892)
        {
            return 0.0001251343 * SOC + -0.0000537420;
        }
        else if (0.3883811021 < SOC && SOC <= 0.5469484539)
        {
            return -0.0000479292 * SOC + 0.0000409148;
        }
        else if (0.2355625405 < SOC && SOC <= 0.3883811021)
        {
            return -0.0000523497 * SOC + 0.0000426316;
        }
        else if (0.0 < SOC && SOC <= 0.2355625405)
        {
            return 0.0000264995 * SOC + 0.0000240577;
        }
        break;
    case R2_FLAG:
        if (0.6692170892 < SOC && SOC <= 1.0)
        {
            return 0.0134485583 * SOC + 0.0017849537;
        }
        else if (0.5469484539 < SOC && SOC <= 0.6692170892)
        {
            return 0.0105284664 * SOC + 0.0037391291;
        }
        else if (0.3883811021 < SOC && SOC <= 0.5469484539)
        {
            return -0.0014170119 * SOC + 0.0102726900;
        }
        else if (0.2355625405 < SOC && SOC <= 0.3883811021)
        {
            return -0.0033225257 * SOC + 0.0110127556;
        }
        else if (0.0 < SOC && SOC <= 0.2355625405)
        {
            return -0.0271582221 * SOC + 0.0166275528;
        }
        break;
    }
    return -1;
}

double calc_P_diss(double V1, double V2, double IL, double R0, double R1, double R2)
{

    return pow(V1, 2) / R1 + pow(V2, 2) / R2 + pow(IL, 2) * R0;
}
void read_power(std::string parameter_csv)
{
    std::ifstream parameter_file(parameter_csv);
    if (!parameter_file.is_open())
    {
        throw std::runtime_error("Could not open file");
    }

    std::string line;
    std::string first_val, second_val;

    if (parameter_file.good())
    {
        while (std::getline(parameter_file, line))
        {
            std::stringstream ss(line);
            std::getline(ss, first_val, ',');
            std::getline(ss, second_val);

            double first = atof(first_val.c_str());
            double second = atof(second_val.c_str());

            std::pair<double, double> csv_pair;
            csv_pair = std::make_pair(first, second);

            power.push_back(csv_pair);
        }
    }

    parameter_file.close();
    if (parameter_file.is_open())
    {
        throw std::runtime_error("File not closed properly");
    }
}

void print_summary(double p_diss, double e_diss, double SOC, double e_del)
{
    // ending print statements after input is finished
    std::cout << "Total Power Dissipated: " << p_diss << std::endl;
    std::cout << "Total Energy Dissipated: " << e_diss << std::endl;
    std::cout << "SoC is " << SOC << std::endl;
    std::cout << "Total Energy Delivered to Load:" << e_del << std::endl;
}

bool run_sim_dynamic(double &V1, double &V2, double &I_L, double &SOC, double &p_out, double &delta_t,
                     double &p_diss, double &e_diss, double &e_del, int &num_cells,
                     std::pair<double, double> &power, int &i,
                     double &R0, double &R1, double &R2, double &C1, double &C2, double &OCV)
{
    const double max_timestep = 0.5;

    double time_remaining = power.first;
    p_out = ((double)power.second / (double)num_cells);

    while (time_remaining > 0)
    {
        double ts = (time_remaining < max_timestep) ? time_remaining : max_timestep;

        SOC = Calc_SOC(I_L, ts);

        R0 = return_Param(R0_FLAG, SOC);
        R1 = return_Param(R1_FLAG, SOC);
        R2 = return_Param(R2_FLAG, SOC);
        C1 = return_Param(C1_FLAG, SOC);
        C2 = return_Param(C2_FLAG, SOC);

        OCV = return_OCV(SOC, p_out);

        V1 = Calc_V1(V1, R1, I_L, C1, ts);
        V2 = Calc_V2(V2, R2, I_L, C2, ts);
        I_L = Calc_IL(R0, OCV, V1, V2, p_out);

        p_diss = calc_P_diss(V1, V2, I_L, R0, R1, R2);
        e_diss += p_diss * ts / 3600.0;
        e_del += p_out * ts / 3600.0;

        time_remaining -= ts;

        if (SOC <= 1e-4)
        {
            std::cout << "SOC depleted at row " << i << std::endl;
            return false;
        }
    }

    std::cout << "power: " << p_diss << " energy: " << e_diss << " i: " << i << " SOC: " << SOC << std::endl;
    return true;
}

void read_power()
{
    std::string junk;
    std::getline(std::cin, junk);
    // inital conditions
    double V1 = 0;
    double V2 = 0;
    double p_out = 0;
    double delta_t = 0;
    double SOC = 1;
    double R0 = return_Param(R0_FLAG, SOC);
    double R1 = return_Param(R1_FLAG, SOC);
    double R2 = return_Param(R2_FLAG, SOC);
    double C1 = return_Param(C1_FLAG, SOC);
    double C2 = return_Param(C2_FLAG, SOC);
    double OCV = return_OCV(SOC, p_out);
    double I_L = 0;
    double p_diss = 0.0;
    double e_diss = 0.0;
    double e_del = 0.0;
    int num_cells = 324;
    const double max_timestep = 0.5;
    int counter = 0;

    std::string line;
    std::string first_val, second_val;
    while (std::getline(std::cin, line))
    {
        if (line.empty())
            continue;

        std::stringstream ss(line);
        std::getline(ss, first_val, ',');
        std::getline(ss, second_val);

        double first = std::stod(first_val);
        double second = std::stod(second_val);

        std::pair<double, double> csv_pair;
        csv_pair = std::make_pair(first, second);

        counter++;

        if (!run_sim_dynamic(V1, V2, I_L, SOC, p_out, delta_t,
                             p_diss, e_diss, e_del, num_cells,
                             csv_pair, counter, R0, R1, R2, C1, C2, OCV))
        {
            break;
        }
    }
    print_summary(p_diss, e_diss, SOC, e_del);
}

int main(int argc, char **argv)
{
    // read_power("output_2014.csv");
    read_power();
    // run_sim();
    return 0;
}