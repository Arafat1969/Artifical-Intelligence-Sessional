#ifndef RANDOM_INSERTION_H
#define RANDOM_INSERTION_H

#include "constructive_search.h"
#include <random>
using namespace std;

class RandomInsertion : public ConstructiveSearch {
    mt19937 rng;

public:
    RandomInsertion(int size, vector<City> cities, int seed = 42)
        : ConstructiveSearch(size, cities), rng(seed) {}

    int findBestInsertion(const vector<int>& tour, const City& city) {
        double minIncrease = __INT32_MAX__;
        int bestPosition = 0;
        for (size_t i = 0; i < tour.size(); ++i) {
            int city1 = tour[i];
            int city2 = tour[(i + 1) % tour.size()];
            double currentDistance =city.distanceTo(cities[city1])+city.distanceTo(cities[city2]);
            double originalDistance = cities[city1].distanceTo(cities[city2]);
            double increase = currentDistance - originalDistance;
            
            if (increase < minIncrease) {
                minIncrease = increase;
                bestPosition = i + 1;
            }
        }
        return bestPosition;
    }

    vector<int> findTour() override {
        uniform_int_distribution<int> dist(1, size);
        int startCity = dist(rng);
        tour.clear();
        tour.push_back(startCity);
        visited[startCity] = true;

        for (int i = 1; i < size; ++i) {
            int randomCity = dist(rng);
            while (visited[randomCity]) randomCity = dist(rng);

            int bestPos = findBestInsertion(tour, cities[randomCity]);
            tour.insert(tour.begin() + bestPos, randomCity);
            visited[randomCity] = true;
        }
        tour.push_back(tour[0]);
        return tour;
    }
};

#endif // RANDOM_INSERTION_H
