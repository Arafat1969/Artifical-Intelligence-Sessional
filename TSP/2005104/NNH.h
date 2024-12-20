#ifndef NNH_H
#define NNH_H

#include "constructive_search.h"

class NNH : public ConstructiveSearch {
public:
    NNH(vector<City> cities) : ConstructiveSearch(cities) {}

    vector<int> findTour() override {
        int current = cities[0].id;
        tour.clear();
        tour.push_back(current);
        visited[current] = true;

        for (int i = 1; i < size; ++i) {
            double min_dist = __INT32_MAX__;
            int next_city = -1;

            for (int j = 0; j < size; ++j) {
                if (!visited[cities[j].id]) {
                    double dist = cities[current].distanceTo(cities[j]);
                    if (dist < min_dist) {
                        min_dist = dist;
                        next_city = cities[j].id;
                    }
                }
            }

            tour.push_back(next_city);
            visited[next_city] = true;
            current = next_city;
        }

        tour.push_back(tour[0]);
        return tour;
    }
};

#endif // NNH_H
