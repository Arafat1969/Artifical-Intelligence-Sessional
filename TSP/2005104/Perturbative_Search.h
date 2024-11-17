#ifndef PERTURBATIVE_SEARCH_H
#define PERTURBATIVE_SEARCH_H

#include <vector>
#include "city.h"

using namespace std;

class PerturbativeSearch {
protected:
    int size;
    vector<int> given_tour;
    vector<int> improved_tour;
    vector<City> cities;

public:
    PerturbativeSearch(vector<City> cities, vector<int> tour)
        : size(cities.size()), cities(cities), given_tour(tour) {
        improved_tour = given_tour;
    }

    double calculateTotalDistance() {
        double totalDistance = 0.0;
        for (size_t i = 0; i < improved_tour.size() - 1; ++i) {
            totalDistance += cities[improved_tour[i]].distanceTo(cities[improved_tour[i + 1]]);
        }
        totalDistance += cities[improved_tour.back()].distanceTo(cities[improved_tour[0]]);
        return totalDistance;
    }

    virtual void optimizeTour() = 0;

    vector<int> getImprovedTour() {
        return improved_tour;
    }
};

#endif // PERTURBATIVE_SEARCH_H
