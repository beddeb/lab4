#include <utility>
#include <string>
#include <iostream>
#include <cassert>
#include "plot.hpp"


template<typename Struct>
std::vector<Dot> cppPlotCore(int values) {
    std::vector<Dot> datasort;
    Struct test_struct;
    const int step = values / 20;
    std::cout << "Generating plot data..." << std::endl;

    for (int i = step; i <= values; i += step) {
        std::vector<int> data;
        for (size_t j = 0; j < i; ++j) {
            data.push_back(j);
        }
        auto start = std::chrono::high_resolution_clock::now();
        if constexpr (std::is_same_v<Struct, std::vector<int>> || std::is_same_v<Struct, std::list<int>>) {
            for (const auto &value: data) {
                test_struct.push_back(value);
            }
        } else if constexpr (std::is_same_v<Struct, std::set<int>>) {
            for (const auto &value: data) {
                test_struct.insert(value);
            }
        } else if constexpr (std::is_same_v<Struct, HashTable<int, int>>) {
            for (const auto &value: data) {
                test_struct.insert(value, value);
            }
        } else if constexpr (std::is_same_v<Struct, std::unordered_map<int, int>>) {
            for (const auto &value: data) {
                test_struct[values] = values;
            }
        } else {
            for (const auto &value: data) {
                test_struct.add(value);
            }
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end - start;

        datasort.push_back({i, duration.count()});
    }

    std::cout << "Part of plot is completed!\n" << std::endl;
    return datasort;
}

void plotLegend(sf::RenderWindow &window, sf::Font &font, const Sequence<DataSeries> &series) {
    std::vector<std::pair<std::string, sf::Color>> legends;
    for (const auto &item: series) {
        legends.emplace_back(item.title, item.color);
    }
//    legends = {
//            {series[0].title, series[0].color},
//            {series[1].title, series[1].color}
//    };

    float legend_x = 50.f;
    float legend_y = 10.f;

    for (const auto &legend: legends) {
        sf::Text legend_text;
        legend_text.setFont(font);
        legend_text.setString(legend.first);
        legend_text.setCharacterSize(14);
        legend_text.setFillColor(legend.second);
        legend_text.setPosition(legend_x, legend_y);
        window.draw(legend_text);
        legend_x += legend_text.getLocalBounds().width + 20.f;
    }
}

void plotAxes(sf::RenderWindow &window, sf::Font &font, float padding) {
    // axis Ox
    sf::Text x_label;
    x_label.setFont(font);
    x_label.setString("Amount of data, counts");
    x_label.setCharacterSize(16);
    x_label.setFillColor(sf::Color::Black);
    x_label.setPosition(window.getSize().x / 2 - 70, window.getSize().y - padding + 30);
    window.draw(x_label);

    // axis Oy
    sf::Text y_label;
    y_label.setFont(font);
    y_label.setString("Time, mls");
    y_label.setCharacterSize(16);
    y_label.setFillColor(sf::Color::Black);
    y_label.setPosition(padding - 50, window.getSize().y / 2 - 100);
    y_label.setRotation(-90);
    window.draw(y_label);
}

template<typename TStruct, typename StdStruct>
void plotInit(int values, Sequence<DataSeries> &series) {
    series[0].data = cppPlotCore<TStruct>(values);
    series[1].data = cppPlotCore<StdStruct>(values);

    vector<Dot> coordinates_1 = series[0].data;
    vector<Dot> coordinates_2 = series[1].data;

    const int window_width = 800;
    const int window_height = 600;
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Chrono of structures");

    float min_x = coordinates_1[0].x, max_x = coordinates_1[0].x;
    float min_y = coordinates_1[0].y, max_y = coordinates_1[0].y;
    for (const auto &coords: {coordinates_1, coordinates_2}) {
        for (const auto &coord: coords) {
            if (coord.x < min_x) min_x = coord.x;
            if (coord.x > max_x) max_x = coord.x;
            if (coord.y < min_y) min_y = coord.y;
            if (coord.y > max_y) max_y = coord.y;
        }
    }

    const float padding = 50.f;
    float scale_x = (window.getSize().x - 2 * padding) / (max_x - min_x);
    float scale_y = (window.getSize().y - 2 * padding) / (max_y - min_y);

    sf::Font font;
    if (!font.loadFromFile("../externallibs/font.ttf")) {
    }

    sf::Vector2f offset(0.f, 0.f);
    bool is_dragging = false;
    sf::Vector2i previous_mouse_position;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
                window.close();

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                is_dragging = true;
                previous_mouse_position = sf::Mouse::getPosition(window);
            }
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                is_dragging = false;
            }
            if (event.type == sf::Event::MouseMoved && is_dragging) {
                sf::Vector2i current_mouse_position = sf::Mouse::getPosition(window);
                offset.x += (current_mouse_position.x - previous_mouse_position.x);
                offset.y += (current_mouse_position.y - previous_mouse_position.y);
                previous_mouse_position = current_mouse_position;
            }
        }

        window.clear(sf::Color::White);

        sf::VertexArray x_axis(sf::Lines, 2);
        x_axis[0].position = sf::Vector2f(padding + offset.x, window.getSize().y - padding + offset.y);
        x_axis[1].position = sf::Vector2f(window.getSize().x - padding + offset.x,
                                          window.getSize().y - padding + offset.y);
        x_axis[0].color = x_axis[1].color = sf::Color::Black;

        sf::VertexArray y_axis(sf::Lines, 2);
        y_axis[0].position = sf::Vector2f(padding + offset.x, padding + offset.y);
        y_axis[1].position = sf::Vector2f(padding + offset.x, window.getSize().y - padding + offset.y);
        y_axis[0].color = y_axis[1].color = sf::Color::Black;

        window.draw(x_axis);
        window.draw(y_axis);

        for (int i = 0; i <= 10; ++i) {
            float x_value = min_x + (max_x - min_x) * i / 10;
            float x_pos = padding + (x_value - min_x) * scale_x + offset.x;
            sf::Text x_label;
            x_label.setFont(font);
            x_label.setString(std::to_string(static_cast<int>(x_value)));
            x_label.setCharacterSize(12);
            x_label.setFillColor(sf::Color::Black);
            x_label.setPosition(x_pos, window.getSize().y - padding + 5 + offset.y);
            window.draw(x_label);

            float y_value = min_y + (max_y - min_y) * i / 10;
            float y_pos = window.getSize().y - padding - (y_value - min_y) * scale_y + offset.y;
            sf::Text y_label;
            y_label.setFont(font);
            y_label.setString(std::to_string(static_cast<int>(y_value)));
            y_label.setCharacterSize(12);
            y_label.setFillColor(sf::Color::Black);
            y_label.setPosition(padding - 30 + offset.x, y_pos - 10);
            window.draw(y_label);
        }

        auto draw_graph = [&](std::vector<Dot> coordinates, sf::Color color) {
            sf::VertexArray lines(sf::LinesStrip, coordinates.size());
            for (size_t i = 0; i < coordinates.size(); ++i) {
                float x = padding + (coordinates[i].x - min_x) * scale_x + offset.x;
                float y = window.getSize().y - padding - (coordinates[i].y - min_y) * scale_y + offset.y;
                lines[i].position = sf::Vector2f(x, y);
                lines[i].color = color;

                sf::CircleShape point(3);
                point.setFillColor(color);
                point.setPosition(x - point.getRadius(), y - point.getRadius());
                window.draw(point);
            }
            window.draw(lines);
        };

        draw_graph(coordinates_1, series[0].color);
        draw_graph(coordinates_2, series[1].color);
        plotLegend(window, font, series); // legend of plot
        plotAxes(window, font, padding); // axes Oy & Oy

        window.display();
    }
}

template void plotInit<Sequence<int>, std::vector<int>>(int values, Sequence<DataSeries> &series);

template void plotInit<ListSequence<int>, std::list<int>>(int values, Sequence<DataSeries> &series);

template void plotInit<ArraySequence<int>, std::vector<int>>(int values, Sequence<DataSeries> &series);

template void plotInit<HashTable<int, int>, std::unordered_map<int, int>>(int values, Sequence<DataSeries> &series);

template void plotInit<HashSet<int>, std::set<int>>(int values, Sequence<DataSeries> &series);
