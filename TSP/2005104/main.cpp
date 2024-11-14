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
            cout<<"Reading coordinates"<<endl;
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
            cout<<id<<" "<<x<<" "<<y<<endl;
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
    cout<<cities.size()<<endl;
}
