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



int main(){
    string filename = "a280.tsp";
    vector<City> cities = parse_tsp_data(filename);
    double cost,cost1,cost2,cost3;
    vector<int> tour;
    //cout<<cities.size()<<endl;
    // NNH nearest(cities);
    // tour = nearest.findTour();
    // cost=nearest.calculateTotalDistance();
    // TwoOpt twoopt_nnh(cities,tour);
    // NodeShift nshift_nnh(cities,tour);
    // NodeSwap nswap_nnh(cities,tour);
    // twoopt_nnh.optimizeTour();
    // nshift_nnh.optimizeTour();
    // nswap_nnh.optimizeTour();
    // cost1 = twoopt_nnh.calculateTotalDistance();
    // cost2 = nshift_nnh.calculateTotalDistance();
    // cost3 = nswap_nnh.calculateTotalDistance();
    // cout<<"Cost for NNH : "<<cost<<endl;
    // cout<<"Cost for NNH + 2-opt : "<<cost1<<endl;
    // cout<<"Cost for NNH + Node Shift : "<<cost2<<endl;
    // cout<<"Cost for NNH + Node Swap : "<<cost3<<endl;
    // RandomInsertion random(cities);
    // tour= random.findTour();
    // cost=random.calculateTotalDistance();
    // TwoOpt twoopt_random(cities,tour);
    // NodeShift nshift_random(cities,tour);
    // NodeSwap nswap_random(cities,tour);
    // twoopt_random.optimizeTour();
    // nshift_random.optimizeTour();
    // nswap_random.optimizeTour();
    // cost1 = twoopt_random.calculateTotalDistance();
    // cost2 = nshift_random.calculateTotalDistance();
    // cost3 = nswap_random.calculateTotalDistance();
    // cout<<"Cost for Random Insertion : "<<cost<<endl;
    // cout<<"Cost for Random Insertion + 2-opt : "<<cost1<<endl;
    // cout<<"Cost for Random Insertion + Node Shift : "<<cost2<<endl;
    // cout<<"Cost for Random Insertion + Node Swap : "<<cost3<<endl;
    GreedyHeuristic greedy(cities);
    tour= greedy.findTour();
    cost=greedy.calculateTotalDistance();
    //TwoOpt twoopt_greedy(cities,tour);
    NodeShift nshift_greedy(cities,tour);
    //NodeSwap nswap_greedy(cities,tour);
    //twoopt_greedy.optimizeTour();
    nshift_greedy.optimizeTour();
    //nswap_greedy.optimizeTour();
    //cost1 = twoopt_greedy.calculateTotalDistance();
    cost2 = nshift_greedy.calculateTotalDistance();
    //cost3 = nswap_greedy.calculateTotalDistance();
    cout<<"Cost for Greedy approach : "<<cost<<endl;
    //cout<<"Cost for Greedy approach + 2-opt : "<<cost1<<endl;
    cout<<"Cost for Greedy approach + Node Shift : "<<cost2<<endl;
    //cout<<"Cost for Greedy approach + Node Swap : "<<cost3<<endl;
    // cout<<endl;
    // cout<<"Tour size: "<<tour.size()<<endl;
    // cout<<endl;
}
