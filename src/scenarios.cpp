#include "model.hpp"
#include "scenarios.hpp"
#include "saa.hpp"
#include <cassert>

Scenarios::Scenarios():
        _seed(2019),
        _numScenarios(2000),
        _UBScenarios(1000),
        _omega() {};

std::vector<int> Scenarios ::getLBScenarios(int scenariosperopt, int batchnumber) const {
        int batch = std::floor((_numScenarios-_UBScenarios)/scenariosperopt);
        if (batch< 10) {
        std::cerr << "batches created are less than or equal to 10."<< 
        "Please reduce number of scenarios per optimization accordingly" <<
        "OR increase the number of scenarios for the problem" << std::endl;
        exit(1);
        }
        int from = scenariosperopt * (batchnumber - 1);
        int to = scenariosperopt * batchnumber;
        std::vector<int> reduced;
        for(int h =from; h<  to; ++h)
        reduced.push_back(_omega[h]);
        return reduced;
        };

 void Scenarios ::generateScenarios(double mean, double stddev){
        std::mt19937 generator(_seed);
        std::normal_distribution<double> distribution (mean, stddev);
        while (_omega.size() <= _numScenarios)
         {
            int demand = int(distribution(generator));
            if (demand>=0) 
            _omega.push_back(demand);
            
        }
        return;
        };

std::vector<int> Scenarios ::getUBScenarios(int batchnumber) const{
        int from = _numScenarios- _UBScenarios + 100 * (batchnumber - 1);
        int to = _numScenarios- _UBScenarios + 100 * (batchnumber);
        std::vector<int> reduced;
        for(int h =from; h< to; ++h)
        reduced.push_back(_omega[h]);
        return reduced;
        };
        
        
