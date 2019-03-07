#include "model.hpp"
#include "scenarios.hpp"
#include "saa.hpp"
#include <cassert>

SAA::SAA(const Scenarios & scenarios):
    _model(), 
    _scenarios(scenarios),
    _orderQuantity(), 
    _lb(), 
    _ub() {};
 
void SAA ::setObjective(std::vector<int> reduced, double p, double c, double s){
        IloNumVar q(_model.getEnv(),0, IloInfinity, ILOFLOAT);
        IloExpr expr(_model.getEnv());
        for(int i =0; i< reduced.size(); ++i) 
        expr += -(p-c)*reduced[i] + (c-s)*IloMax(q-reduced[i],0)
                 +(p-c)*IloMax(reduced[i]-q,0);
        IloObjective obj(_model.getEnv(), expr/reduced.size(), IloObjective::Minimize);
        _model.getModel().add(obj);
         
        IloCplex cplex(_model.getEnv());
        cplex.extract(_model.getModel());
        cplex.solve();
        
        _orderQuantity.push_back(cplex.getValue(q));
        _lb.push_back(cplex.getObjValue());
        expr.end();
        obj.end();
        q.end();
        
         return;
        };

void SAA ::setObjective2(std::vector<int> reduced, double p, double c, double s,double lambda){
        
        IloNumVar q2(_modelV.getEnv(),0, IloInfinity, ILOFLOAT);
        IloExprArray mu(_modelV.getEnv(), reduced.size());
        IloExprArray sigma(_modelV.getEnv(), reduced.size());
        IloExpr meanmu(_modelV.getEnv());
        IloExpr meansig(_modelV.getEnv());
        for(int i =0; i< reduced.size(); ++i) {
        mu[i] = (p-c)*reduced[i] - (c-s)*IloMax(q2-reduced[i],0)
                 -(p-c)*IloMax(reduced[i]-q2,0);
        meanmu += mu[i];   
        }
        meanmu = meanmu/reduced.size();
        for(int i =0; i< reduced.size(); ++i) {
        sigma[i]  = IloMax(meanmu -mu[i],0);
        meansig += sigma[i];
        }

        IloObjective obj2(_modelV.getEnv(), lambda*meansig/reduced.size()-meanmu, IloObjective::Minimize);
        _modelV.getModel().add(obj2);

        IloCplex cplex(_modelV.getEnv());
        cplex.extract(_modelV.getModel());
        cplex.exportModel("test.lp");
        cplex.solve();
        _orderQuantity.push_back(cplex.getValue(q2));
        _lb.push_back(cplex.getObjValue());
        mu.end();
        obj2.end();
        sigma.end();
        q2.end();
        meanmu.end();
        meansig.end();
        return;
        };

void SAA ::solve(int batchnumber, int scenariosperopt, double p,double c, double s, int o, double lambda){
    auto scenarios = _scenarios.getLBScenarios(
       scenariosperopt, batchnumber);
    assert(scenarios.size() == scenariosperopt);
    if(o ==0)
    setObjective(scenarios, p,c,s);
    else
    {
    setObjective2(scenarios, p,c,s,lambda);
    }
    
    return;
    };
        
void SAA ::calcUB(int batchnumber, double p, double c, double s){
    auto scenarios = _scenarios.getUBScenarios(
        batchnumber);
    assert(scenarios.size() == 100);
    double UB =0;
    for ( int i =0; i < scenarios.size();++i)
        UB += -(p-c)*scenarios[i] + (c-s)*std::max(_orderQuantity[batchnumber-1]-scenarios[i],0) +(p-c+s)*std::max(scenarios[i]- _orderQuantity[batchnumber-1],0);
    _ub.push_back(UB/scenarios.size());
    return;
    };
    
double SAA ::calcGap(){
    double kappa = 2.2621; // alpha = 0.05, degrees of freedom = 9
    auto L= std::accumulate(std::begin(_lb), std::end(_lb), 0.0)/_lb.size();
    double var;
    for (int l = 0; l < _lb.size();++l)
        var += std::pow(_lb[l] - L,2);
    auto e_l = std::sqrt(var/(_lb.size()-1)) * kappa/std::sqrt(10);
    auto U= std::accumulate(std::begin(_ub), std::end(_ub), 0.0)/_ub.size();
    var=0;
    for (int l = 0; l < _ub.size();++l)
        var += std::pow(_ub[l] - U,2);
    auto e_u = std::sqrt(var/(_ub.size()-1))* kappa/std::sqrt(10);
     return std::abs(std::max((U-L+ e_l + e_u),0.0)/(L- e_l)*100);
    };
     