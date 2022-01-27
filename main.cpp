/**
 * @file main.cpp
 *
 * @author Jordana Mecler
 *
 * Contact: jmecler@inf.puc-rio.br
 *
 */

//#include <Windows.h>
//#include <stdlib.h>
#include "CommandLine.h"
#include "Parameters.h"
#include "Population.h"
#include "Genetic.h"
#include <algorithm>

#define min(a, b) ((a < b) ? a : b)

using namespace std;

int main(int argc, char* argv[]) {

    double bestCost;
    double averageCost, averageTime;

    unsigned int runs = 1;
    Parameters *parameters;
    Population *population;
    clock_t nb_ticks_allowed;

    try {
        // Read arguments from command line and set problem parameters
        CommandLine commandLine(argc, argv);
        if (!commandLine.isValid())
            throw string(
                    "(ERROR) Wrong command line. Format: [instance_path] [solution_path] [seed] [population_size] [max_population_size] [number_elite] [number_close_individuals] [max_diversify]");
        nb_ticks_allowed = commandLine.getCpuTime() * CLOCKS_PER_SEC;
        parameters = new Parameters(commandLine.getSeed(), commandLine.getInstancesPaths(), commandLine.getInstancesNames(),
                            commandLine.getSolutionPath(), commandLine.getPopulationSize(), commandLine.getMaxPopulationSize(),
                            commandLine.getNumberElite(), commandLine.getNumberCloseIndividuals(), commandLine.getMaxDiversify());
//        runs = commandLine.getRuns();

        string instanceFile = commandLine.getInstancesPaths();
        cout << "Starting " << instanceFile <<  endl;
        cout << endl;

        double cost, totalAvgCost = 0, totalBestCost = 0;
        double totalTime = 0;
//        double totalImprovPrim = 0, totalImprovSec = 0;

        ofstream outputFile;

                const clock_t startTime = clock();
                parameters->terminate = false;
                parameters->readFile(instanceFile);

                parameters->setAuxiliaryParams();

                // Start population
                population = new Population(parameters);

                // Run genetic algorithm
                Genetic solver(parameters, population, nb_ticks_allowed, false);
                solver.evolve(min(parameters->numJobs * 20, 1000));

                cost = population->getBestIndividual()->solutionCost.evaluation;
                vector<unsigned int> chromosome = population->getBestIndividual()->chromosome;
                totalTime = (float(clock() - startTime) / CLOCKS_PER_SEC);
                delete population;

                // Write result to solution file
                outputFile.open(parameters->solutionPath, ofstream::out | ofstream::app);
                //outputFile << instanceFile << endl;
                outputFile << cost << ","  << totalTime << endl;

                for (auto const& c : chromosome)
                        outputFile << c + 1 << ' ';
                outputFile << endl;
                outputFile.close();


        delete parameters;

        cout << "Finishing " << commandLine.getInstancesPaths() << "/" <<  commandLine.getInstancesNames() << " without errors" << endl;
        cout << endl;

        return 0;
    }
    catch (const string &error) {
        cout << error << endl;
        return 0;
    }
}