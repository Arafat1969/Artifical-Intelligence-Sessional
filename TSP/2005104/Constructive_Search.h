#ifndef CONSTRUCTIVE_SEARCH_H
#define CONSTRUCTIVE_SEARCH_H

#include <vector>
#include "city.h"
using namespace std;
class ConstructiveSearch {
protected:
    int size;
    vector<City> cities;
    vector<int> tour;
    vector<bool> visited;

public:
    ConstructiveSearch(int size, const vector<City>& cities)
        : size(size), cities(cities), visited(size+1, false) {}

    double calculateTotalDistance() const {
        double totalDistance = 0.0;
        for (size_t i = 0; i < tour.size() - 1; ++i) {
            totalDistance += cities[tour[i]].distanceTo(cities[tour[i + 1]]);
        }
        totalDistance += cities[tour.back()].distanceTo(cities[tour[0]]);
        return totalDistance;
    }

    virtual vector<int> findTour() = 0;

};

#endif // CONSTRUCTIVE_SEARCH_H
