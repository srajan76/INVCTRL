#ifndef SCENARIOS_HPP
#define SCENARIOS_HPP

#pragma once

#include <vector>
#include <random> 
#include <iostream>
#include <math.h>

class Scenarios {
    protected: 
        int _seed = 2019;
        int _numScenarios = 2000;
        int _UBScenarios = 1000;
        std::vector<int> _omega;

    public:
        Scenarios();
        
        void setSeed(int seed) { _seed = seed; };
        int getNumScenarios() const { return _numScenarios; };
        int getUBScenarios() const { return _UBScenarios; };
        int setNumScenarios(int numScenarios) {_numScenarios = numScenarios;};
        
        
        void generateScenarios(double mean, double stddev);
        std::vector<int> getLBScenarios(int scenariosperopt, int batchnumber) const;
        std::vector<int> getUBScenarios(int batchnumber) const;
};

#endif 