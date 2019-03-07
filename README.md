# INVCTRL
The code is organized into header files /include , source fles /src and external packages used (in /third party). 
You will need to create a make file linking all your src files and third party files to run the program.

This program solves the SAA problem for the inventory control max(expected return) model as well as the risk model 
(as detailed in the report)

There are three classes created:
Scenarios- for generating random normal scenarios for computing bounds for SAA.
Model- for storing and accesing the Cplex environment and variables for the optimization.
SAA- for implementing the sample average approximation algorithm for the function to be optimized.

main instantiates objects of SAA and Scenarios to implement the algorithm.
SAA class uses directly the objects instantiated  as Model .

The header files has all the public (and private functions) for each class which are listed
The src files have the implementation of all the functions(methods/procedures) listed in the respective header files.

Once compiled the code builds a run object which can provide parameters to make different runs 
through the Option parser package from the command line without changing/accessing the main program. The help (-h)
 command in runtime will provide the details of parameters that can be changed for every run

Std C++ library functionality has been employed to generate random variables and perform math/statistical operations
These libraries have been included in every header file as required.
