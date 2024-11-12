#include<bits/stdc++.h>
#include "city.h"
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
            reading_coordinates = true;
            continue;
        }

        if (reading_coordinates) {
            istringstream ss(line);
            int id;
            double x;
            double y;
            ss >> id >> x >> y;
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

}
