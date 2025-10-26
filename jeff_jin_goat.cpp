
/*void run_sim()
{

    // First time step
    // initial conditions
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
    // Calculate load current
    double I_L = 0;

    double p_diss = 0.0;
    double e_diss = 0.0;
    double e_del = 0.0;
    int num_cells = 324;
    const double max_timestep = 0.5;
    for (int i = 0; i < power.size(); i++)
    {
        if (i == 2586)
        {
        }
        double time_remaining = power[i].first;
        while (time_remaining > 0)
        {
            double ts;
            if (time_remaining < max_timestep)
            {
                ts = time_remaining;
            }
            else
            {
                ts = max_timestep;
            }
            // Calculate SOC based on previous I_L
            SOC = Calc_SOC(I_L, ts); // Simple Coulomb Counter
            // Use new SOC to find new paramters
            R0 = return_Param(R0_FLAG, SOC);
            R1 = return_Param(R1_FLAG, SOC);
            R2 = return_Param(R2_FLAG, SOC);
            C1 = return_Param(C1_FLAG, SOC);
            C2 = return_Param(C2_FLAG, SOC);
            OCV = return_OCV(SOC, p_out);
            // change units on p_out
            p_out = ((float)power[i].second / (float)num_cells);
            // Calculate V1, V2, and load current
            V1 = Calc_V1(V1, R1, I_L, C1, ts);
            V2 = Calc_V2(V2, R2, I_L, C2, ts);
            I_L = Calc_IL(R0, OCV, V1, V2, p_out);
            // std::cout << "V1: " << V1 << "V2: " << V2 << std::endl;

            // std::cout << "Load Current:  " << I_L << " SOC: " << SOC << std::endl;
            // std::cout << "OCV: " << OCV << std::endl;
            // calculate total load current
            p_diss = calc_P_diss(V1, V2, I_L, R0, R1, R2); // Power Lost to Heat
            e_diss += p_diss * ts / 3600;                  // Wh
            e_del += p_out * ts / 3600;                    // Wh
            // std::cout << i << std::endl;
            time_remaining -= ts;
            if (SOC <= 0.0001)
                break;
        }
        if (SOC <= 0.0001)
            break;
        std::cout << "power: " << p_diss << " energy: " << e_diss << " i: " << i << " SOC: " << SOC << std::endl;
        if (e_diss == 0.0495186)
        {
        }
    }
    std::cout << "Total Power Dissipated: " << p_diss << std::endl;
    std::cout << "Total Energy Dissipated: " << e_diss << std::endl;
    std::cout << "SoC is " << SOC << std::endl;
    std::cout << "Total Energy Delivered to Load:" << e_del << std::endl;
}*/
