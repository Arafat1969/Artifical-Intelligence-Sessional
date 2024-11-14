#ifndef GREEDY_HEURISTIC_H
#define GREEDY_HEURISTIC_H

#include "constructive_search.h"
#include <algorithm>

class GreedyHeuristic : public ConstructiveSearch {
public:
    vector<pair<pair<City, City>, double>> edges;

    GreedyHeuristic(int size, vector<City> cities) : ConstructiveSearch(size, cities) {}

    void createEdges() {
        edges.clear();
        for (int i = 0; i < size; ++i) {
            for (int j = i + 1; j < size; ++j) {
                edges.push_back({{cities[i], cities[j]}, cities[i].distanceTo(cities[j])});
            }
        }
    }

    vector<int> findTour() override {
        if (size == 0) return {};

        createEdges();
        sort(edges.begin(), edges.end(), [](const auto& a, const auto& b) {
            return a.second < b.second;
        });

        vector<pair<int, int>> tourEdges;
        visited[0] = true;

        for (const auto& edge : edges) {
            if (tourEdges.size() >= size - 1) break;
            int city1 = edge.first.first.id;
            int city2 = edge.first.second.id;
            if (!visited[city1] || !visited[city2]) {
                visited[city1] = visited[city2] = true;
                tourEdges.push_back({city1, city2});
            }
        }

        tour.clear();
        tour.push_back(0);
        for (size_t i = 0; i < size - 1; ++i) {
            for (const auto& edge : tourEdges) {
                if (edge.first == tour.back() && find(tour.begin(), tour.end(), edge.second) == tour.end()) {
                    tour.push_back(edge.second);
                    break;
                }
                if (edge.second == tour.back() && find(tour.begin(), tour.end(), edge.first) == tour.end()) {
                    tour.push_back(edge.first);
                    break;
                }
            }
        }
        tour.push_back(0);
        return tour;
    }
};

#endif // GREEDY_HEURISTIC_H
