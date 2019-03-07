#ifndef SAA_HPP
#define SAA_HPP

#pragma once

#include <ilcplex/ilocplex.h>
#include <vector>
#include <tuple>
#include <unordered_map>
#include <limits>
#include <string>
#include <cassert>
#include <math.h>
#include <numeric>
#include "scenarios.hpp"
#include "model.hpp"

class Model;
class Scenarios;


class SAA {
    protected: 
        
        Model _model, _modelV;
        const Scenarios _scenarios;
        std::vector<int> _orderQuantity;
        std::vector<double> _lb;
        std::vector< double> _ub;
        
        
    
   private: 
        void setObjective(std::vector<int> reduced, double p, double c, double s);
        void setObjective2(std::vector<int> reduced, double p, double c, double s,double lambda);


    public: 
        SAA(const Scenarios & scenarios);
        void solve(int batchnumber, int scenariosperopt, double p,double c, double s, int o, double lambda);        
        void calcUB(int batchnumber, double p, double c, double s);
        double calcGap();
        std::vector<int> optquantity(){return _orderQuantity;};
        std::vector<double> showUB(){return _ub;};
        std::vector<double> showLB(){return _lb;};
        
};
#endif