#pragma once

#include <SFML/Graphics.hpp>
#include <chrono>
#include <vector>
#include <set>
#include <list>
#include <unordered_map>
#include "../include/graph.hpp"


using namespace std;
using namespace chrono;
using namespace sf;

struct Dot {
    int x;
    double y;

    Dot &operator=(const Dot &other) {
        if (this != &other) {
            x = other.x;
            y = other.y;
        }
        return *this;
    }
};

struct DataSeries {
    vector<Dot> data;
    string title;
    Color color;

    DataSeries() : data(), title(), color() {}

    DataSeries(const std::vector<Dot> &data, const std::string &title, const Color &color)
            : data(data), title(title), color(color) {}

    DataSeries(const std::string &title, const Color &color)
            : title(title), color(color) {}

    DataSeries(const std::vector<Dot> &data, const Color &color)
            : data(data), color(color) {}
};

std::vector<Dot> cppPlotCore(int values);

void plotLegend(sf::RenderWindow &window, sf::Font &font, const Sequence<DataSeries> &series);

void plotAxes(sf::RenderWindow &window, sf::Font &font, float padding);

template<typename TStruct, typename StdStruct>
void plotInit(int values, Sequence<DataSeries> &series);
