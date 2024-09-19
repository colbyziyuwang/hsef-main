#ifndef GRID_PATHFINDING_VISUALIZER_H_
#define GRID_PATHFINDING_VISUALIZER_H_

#include <SFML/Graphics.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <filesystem>
#include <iostream>

#include "environments/grid_pathfinding/grid_map.h"

class GridPathfindingVisualizer {
public:
    GridPathfindingVisualizer();

    void run();

private:
    sf::RenderWindow window;
    tgui::Gui gui;

    tgui::ScrollablePanel::Ptr editing_view;
    tgui::ScrollablePanel::Ptr graph_view;
    tgui::ScrollablePanel::Ptr result_view;

    int grid_width{};
    int grid_height{};

    int active_button{};
    sf::Vector2i start_state_value;
    sf::Vector2i goal_state_value;

    std::string path = "A:/new/visualizer/assets/test";
    tgui::EditBox::Ptr file_path_edit_box;
    tgui::Button::Ptr load_map_button;

    float zoom_level = 1.0f;
    float min_zoom = 0.5f;
    float max_zoom = 2.0f;

    void createPanes();
    void createGrid();
    void createInteractiveGrid();
    void createWidgets();
    void loadMapAndUpdatePath(const std::string& newPath);
    void zoomIn();
    void zoomOut();
    void recreateGrid();
    void runAStar();
};

#endif  //GRID_PATHFINDING_VISUALIZER_H_