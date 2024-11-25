#include<bits/stdc++.h>
#include "city.h"
#include "Constructive_Search.h"
#include "NNH.h"
#include "Random_Insertion.h"
#include "Greedy_Heuristics.h"
#include "perturbative_search.h"
#include "Node_Shift.h"
#include "Node_Swap.h"
#include "Two_Opt.h"


using namespace std;

vector<string> inputs={
    "../TSP_assignment_task_benchmark_data/a280.tsp",
    "../TSP_assignment_task_benchmark_data/berlin52.tsp",
    "../TSP_assignment_task_benchmark_data/bier127.tsp",
    "../TSP_assignment_task_benchmark_data/ch130.tsp",
    "../TSP_assignment_task_benchmark_data/ch150.tsp",
    "../TSP_assignment_task_benchmark_data/eil101.tsp",
    "../TSP_assignment_task_benchmark_data/eil51.tsp",
    "../TSP_assignment_task_benchmark_data/eil76.tsp",
    "../TSP_assignment_task_benchmark_data/kroA100.tsp",
    "../TSP_assignment_task_benchmark_data/kroB100.tsp",
    "../TSP_assignment_task_benchmark_data/kroC100.tsp",
    "../TSP_assignment_task_benchmark_data/kroD100.tsp",
    "../TSP_assignment_task_benchmark_data/kroE100.tsp",
    "../TSP_assignment_task_benchmark_data/lin105.tsp",
    "../TSP_assignment_task_benchmark_data/lin318.tsp",
    "../TSP_assignment_task_benchmark_data/pr124.tsp",
    "../TSP_assignment_task_benchmark_data/pr144.tsp",
    "../TSP_assignment_task_benchmark_data/pr76.tsp",
    "../TSP_assignment_task_benchmark_data/rat195.tsp",
    "../TSP_assignment_task_benchmark_data/rat99.tsp",
    "../TSP_assignment_task_benchmark_data/st70.tsp"
};

vector<string> outputs={
    "../Output/a280_out.tsp",
    "../Output/berlin52_out.tsp",
    "../Output/bier127_out.tsp",
    "../Output/ch130_out.tsp",
    "../Output/ch150_out.tsp",
    "../Output/eil101_out.tsp",
    "../Output/eil51_out.tsp",
    "../Output/eil76_out.tsp",
    "../Output/kroA100_out.tsp",
    "../Output/kroB100_out.tsp",
    "../Output/kroC100_out.tsp",
    "../Output/kroD100_out.tsp",
    "../Output/kroE100_out.tsp",
    "../Output/lin105_out.tsp",
    "../Output/lin318_out.tsp",
    "../Output/pr124_out.tsp",
    "../Output/pr144_out.tsp",
    "../Output/pr76_out.tsp",
    "../Output/rat195_out.tsp",
    "../Output/rat99_out.tsp",
    "../Output/st70_out.tsp"
};

vector<City> parse_tsp_data(const string &filename) {
    vector<City> cities;
    ifstream myInput;
    myInput.open(filename);
    if(!myInput){
        cout<<"File not valid"<<endl;
    }
    string line;
    bool reading_coordinates = false;

    while (getline(myInput, line)) {
        if (line.find("NODE_COORD_SECTION") != string::npos) {
            //cout<<"Reading coordinates"<<endl;
            reading_coordinates = true;
            continue;
            
        }
        if (line.find("EOF") != string::npos) {
            reading_coordinates = false;
            break;
        }

        if (reading_coordinates) {
            istringstream ss(line);
            int id;
            double x;
            double y;
            ss >> id >> x >> y;
            //cout<<id<<" "<<x<<" "<<y<<endl;
            City city(id,x,y);
            if (ss) {
                cities.push_back(city);
            }
        }
    }

    myInput.close();
    return cities;
}

void write_tsp_data(const string &filename, vector<pair<double,vector<double>>> &output) {
    ofstream myOutput;
    myOutput.open(filename);
    if(!myOutput){
        cout<<"File not valid"<<endl;
    }
    vector<string> algo_names = {"NNH","Random Insertion","Greedy Heuristic"};
    for(int i=0;i<output.size();i++){
        myOutput<<algo_names[i]<<" : "<<output[i].first<<endl;
        myOutput<<algo_names[i]<<" + 2-opt : "<<output[i].second[0]<<endl;
        myOutput<<algo_names[i]<<" + Node Shift : "<<output[i].second[1]<<endl;
        myOutput<<algo_names[i]<<" + Node Swap : "<<output[i].second[2]<<endl;
    }
    myOutput.close();
}



int main(){
    //string filename = "../TSP_assignment_task_benchmark_data/a280.tsp";
    // vector<pair<double,vector<double>>> output;
    int i;
    cin>>i;
    ofstream myOutput;
    myOutput.open(outputs[i]);
    vector<City> cities;
    vector<int> tour;
    double cost,cost1,cost2,cost3;
    //for(int i=0;i<inputs.size();i++){
        cities = parse_tsp_data(inputs[i]);
        myOutput<<"File: "<<inputs[i]<<endl;
        cout<<cities.size()<<endl;
        NNH nearest(cities);
        tour = nearest.findTour();
        cost=nearest.calculateTotalDistance();
        // output[0].first = nearest.calculateTotalDistance();
        TwoOpt twoopt_nnh(cities,tour);
        NodeShift nshift_nnh(cities,tour);
        NodeSwap nswap_nnh(cities,tour);
        twoopt_nnh.optimizeTour();
        nshift_nnh.optimizeTour();
        nswap_nnh.optimizeTour();
        cost1 = twoopt_nnh.calculateTotalDistance();
        cost2 = nshift_nnh.calculateTotalDistance();
        cost3 = nswap_nnh.calculateTotalDistance();
        // output[0].second.push_back(twoopt_nnh.calculateTotalDistance());
        // output[0].second.push_back(nshift_nnh.calculateTotalDistance());
        // output[0].second.push_back(nswap_nnh.calculateTotalDistance());
        myOutput<<"NNH : "<<cost<<endl;
        myOutput<<"NNH + 2-opt : "<<cost1<<endl;
        myOutput<<"NNH + Node Shift : "<<cost2<<endl;
        myOutput<<"NNH + Node Swap : "<<cost3<<endl;
        RandomInsertion random(cities);
        tour= random.findTour();
        cost=random.calculateTotalDistance();
        // output[1].first = random.calculateTotalDistance();
        TwoOpt twoopt_random(cities,tour);
        NodeShift nshift_random(cities,tour);
        NodeSwap nswap_random(cities,tour);
        twoopt_random.optimizeTour();
        nshift_random.optimizeTour();
        nswap_random.optimizeTour();
        cost1 = twoopt_random.calculateTotalDistance();
        cost2 = nshift_random.calculateTotalDistance();
        cost3 = nswap_random.calculateTotalDistance();
        // output[1].second.push_back(twoopt_random.calculateTotalDistance());
        // output[1].second.push_back(nshift_random.calculateTotalDistance());
        // output[1].second.push_back(nswap_random.calculateTotalDistance());
        myOutput<<"Random Insertion : "<<cost<<endl;
        myOutput<<"Random Insertion + 2-opt : "<<cost1<<endl;
        myOutput<<"Random Insertion + Node Shift : "<<cost2<<endl;
        myOutput<<"Random Insertion + Node Swap : "<<cost3<<endl;
        GreedyHeuristic greedy(cities);
        tour= greedy.findTour();
        cost=greedy.calculateTotalDistance();
        // output[2].first = greedy.calculateTotalDistance();
        TwoOpt twoopt_greedy(cities,tour);
        NodeShift nshift_greedy(cities,tour);
        NodeSwap nswap_greedy(cities,tour);
        twoopt_greedy.optimizeTour();
        nshift_greedy.optimizeTour();
        nswap_greedy.optimizeTour();
        cost1 = twoopt_greedy.calculateTotalDistance();
        cost2 = nshift_greedy.calculateTotalDistance();
        cost3 = nswap_greedy.calculateTotalDistance();
        // output[2].second.push_back(twoopt_greedy.calculateTotalDistance());
        // output[2].second.push_back(nshift_greedy.calculateTotalDistance());
        // output[2].second.push_back(nswap_greedy.calculateTotalDistance());
        myOutput<<"Greedy approach : "<<cost<<endl;
        myOutput<<"Greedy approach + 2-opt : "<<cost1<<endl;
        myOutput<<"Greedy approach + Node Shift : "<<cost2<<endl;
        myOutput<<"Greedy approach + Node Swap : "<<cost3<<endl;
        // cout<<endl;
        // cout<<"Tour size: "<<tour.size()<<endl;
        // cout<<endl;
        // write_tsp_data(outputs[i],output);
        // tour.clear();
        // cities.clear();
        // output.clear();
    //}
    return 0;
}
