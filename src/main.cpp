#include <ilcplex/ilocplex.h>
#include <optionParser.hpp>
#include "model.hpp"
#include "saa.hpp"
#include "scenarios.hpp"



int main(int argc, char* argv[]){

    op::OptionParser opt;
    opt.add_option("h", "help", "shows option help" ); 
    opt.add_option("m", "scenarios", "number of scenarios chosen per optimization problem", "100" );
    opt.add_option("t", "totalscenarios", "number of total scenarios generated for the problem", "2000" );
    opt.add_option("o", "objective", "choose expected return: 0  or risk: 1", "0" );
    opt.add_option("l", "lambda", "choose a value between 0 and 1", "0.0" );
    // parse the options and verify that all went well
    bool correct_parsing = opt.parse_options(argc, argv);
    
    if(!correct_parsing) return EXIT_FAILURE;
    if(op::str2bool(opt["h"])) { 
        opt.show_help();
        return 0;
    }
    const int m = op::str2int(opt["m"]);
    const int o = op::str2int(opt["o"]);
    const double lambda = op::str2int(opt["l"]);
    Scenarios scen;
    scen.setNumScenarios(op::str2int(opt["t"]));
    //scen.setSeed(2265); chnage to create any random instance. current seed set at 2019.
    double price =15;
    double cost =2;
    double salvage = -3;
    double mean = 400;
    double stddev = 100;
    scen.generateScenarios(mean,stddev);
    SAA newsvendor(scen);
    /* for LB we  solve 10 optimization problems with 'm' scenarios each. For each value
     of optimal Q we calculate an average value of UB  based on 100 scenarios each.
     Using these 10 values of UB and LB we  calculate the means and std deviations of these
    and find the deviation based on 10 sample student -t distribution and the degree 
     of confidence alpha to perform the relative optimal gap calculations  (alpha =0.05) */
    
    for (int k =0; k <10; ++k) 
    newsvendor.solve(k+1,m,price,cost,salvage,o,lambda);
   
    if( o ==0) {//Did not implement UB calculations for risk model
    for (int k =1; k <11; ++k) 
    newsvendor.calcUB(k, price, cost, salvage);
    std::cout << newsvendor.calcGap()<< std::endl;
    }
    
    
   for (int i =0; i < newsvendor.optquantity().size(); ++i)
    std::cout << newsvendor.optquantity()[i]<<newsvendor.showLB()[i]<< std::endl;

    //AVERAGE OPTIMAL QUANTITY FOR RISK AVERSE CASE ( run with o =1)
    auto oq = newsvendor.optquantity();
    auto Optq= std::accumulate(std::begin(oq), std::end(oq), 0.0)/oq.size();
    std:: cout <<Optq <<std::endl;


    //AVERAGE LB and UB FOR EXPECTED RETURN ONLY
   /* auto LB = newsvendor.showLB();
    auto UB = newsvendor.showUB();
    auto L= std::accumulate(std::begin(LB), std::end(LB), 0.0)/LB.size();
    auto U= std::accumulate(std::begin(UB), std::end(UB), 0.0)/UB.size();
    std:: cout <<L  << "\t" << U<< std::endl
    */
   
 
    return 0;
}



