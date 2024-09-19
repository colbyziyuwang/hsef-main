#include "grid_pathfinding.h"
#include "grid_pathfinding_colours.h"

using namespace GridPathfindingColours;
sf::Color getBackgroundColorForLocationType(GridLocationType locationType);

void createInteractiveGrid(const tgui::Panel::Ptr& editing_view, const tgui::Panel::Ptr& graph_view);
GridPathfindingVisualizer::GridPathfindingVisualizer()
          : window(sf::VideoMode(1000, 600), "Grid Pathfinding Scenario"),
            gui(window), editing_view(tgui::ScrollablePanel::create()), graph_view(tgui::ScrollablePanel::create()), result_view(tgui::ScrollablePanel::create()) {
    createPanes();
}

void GridPathfindingVisualizer::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            gui.handleEvent(event);
        }

        window.clear();
        gui.draw();
        window.display();
    }
}

void GridPathfindingVisualizer::createPanes() {
    editing_view->setPosition(0, 0);
    editing_view->setSize("25%", "100%");
    editing_view->getRenderer()->setBackgroundColor(sf::Color(230, 230, 230));
    editing_view->getRenderer()->setBorders(1);

    graph_view->setPosition("25%", 0);
    graph_view->setSize("50%", "100%");
    graph_view->getRenderer()->setBackgroundColor(sf::Color(255, 255, 255));
    graph_view->getRenderer()->setBorders(1);

    result_view->setPosition("75%", 0);
    result_view->setSize("25%", "100%");
    result_view->getRenderer()->setBackgroundColor(sf::Color(230, 230, 230));
    result_view->getRenderer()->setBorders(1);

    gui.add(editing_view);
    gui.add(graph_view);
    gui.add(result_view);

    this->editing_view = editing_view;
    this->graph_view = graph_view;
    this->result_view = result_view;

    createWidgets();
}

void GridPathfindingVisualizer::createInteractiveGrid() {
    GridMap map(path);

    float cell_width = (graph_view->getSize().x / static_cast<float>(grid_width)) - 0.3f;
    float cell_height = (graph_view->getSize().y / static_cast<float>(grid_height)) - 0.3f;

    min_zoom = std::min(graph_view->getSize().x / (cell_width * grid_width),
              graph_view->getSize().y / (cell_height * grid_height));

    float scaled_cell_width = cell_width * zoom_level;
    float scaled_cell_height = cell_height * zoom_level;

    // Create a 2D vector to store the buttons representing the grid cells
    std::vector<std::vector<tgui::Button::Ptr>> gridButtons(grid_width, std::vector<tgui::Button::Ptr>(grid_height));

    auto cellClickHandler = [this](int x, int y) {
        if (active_button == 1) {
            start_state_value = {x, y};
        } else if (active_button == 2) {
            goal_state_value = {x, y};
        }
    };

    for (int x = 0; x < grid_width; ++x) {
        for (int y = 0; y < grid_height; ++y) {
            auto button = tgui::Button::create();
            button->setPosition(x * scaled_cell_width, y * scaled_cell_height);
            button->setSize(scaled_cell_width, scaled_cell_height);
            button->setText("");
            if (x == start_state_value.x && y == start_state_value.y) {
                button->getRenderer()->setBackgroundColor(START_STATE_COLOR);
            } else if (x == goal_state_value.x && y == goal_state_value.y) {
                button->getRenderer()->setBackgroundColor(GOAL_STATE_COLOR);
            } else {
                button->getRenderer()->setBackgroundColor(getBackgroundColorForLocationType(map.getLocationType(x, y)));
            }
            button->getRenderer()->setBackgroundColorHover(sf::Color(255, 182, 193));

            button->onPress([this, x, y, cellClickHandler]() {
                cellClickHandler(x, y);
                graph_view->removeAllWidgets();
                createGrid();
            });
            graph_view->add(button);
            gridButtons[x][y] = button;
        }
    }
}

void GridPathfindingVisualizer::createGrid() {
    graph_view->removeAllWidgets();

    GridMap map(path);

    grid_width = map.getHeight();
    grid_height = map.getWidth();

    float cell_width = (graph_view->getSize().x / static_cast<float>(grid_width)) - 0.3f;
    float cell_height = (graph_view->getSize().y / static_cast<float>(grid_height)) - 0.3f;

    min_zoom = std::min(graph_view->getSize().x / (cell_width * grid_width),
              graph_view->getSize().y / (cell_height * grid_height));

    float scaled_cell_width = cell_width * zoom_level;
    float scaled_cell_height = cell_height * zoom_level;

    for (int x = 0; x < grid_width; ++x) {
        for (int y = 0; y < grid_height; ++y) {
            auto square = tgui::Panel::create();
            square->setPosition(x * scaled_cell_width, y * scaled_cell_height);
            square->setSize(scaled_cell_width, scaled_cell_height);
            if (x == start_state_value.x && y == start_state_value.y) {
                square->getRenderer()->setBackgroundColor(START_STATE_COLOR);
            } else if (x == goal_state_value.x && y == goal_state_value.y) {
                square->getRenderer()->setBackgroundColor(GOAL_STATE_COLOR);
            } else {
                square->getRenderer()->setBackgroundColor(getBackgroundColorForLocationType(map.getLocationType(x, y)));
            }
            square->getRenderer()->setBorders(1);
            graph_view->add(square);
        }
    }
}

void GridPathfindingVisualizer::createWidgets() {
    graph_view->removeAllWidgets();
    createGrid();

    auto start_state = tgui::Button::create();
    start_state->setPosition(10, 10);
    start_state->setSize(100, 30);
    start_state->setText("Start State");
    start_state->onPress([this]() {
        graph_view->removeAllWidgets();
        active_button = 1;
        createInteractiveGrid();
    });

    auto goal_state = tgui::Button::create();
    goal_state->setPosition(start_state->getPosition().x + start_state->getSize().x + 10, 10);
    goal_state->setSize(100, 30);
    goal_state->setText("Goal State");
    goal_state->onPress([this]() {
        graph_view->removeAllWidgets();
        active_button = 2;
        createInteractiveGrid();
    });

    auto zoom_in_button = tgui::Button::create();
    zoom_in_button->setPosition(10, 50);
    zoom_in_button->setSize(100, 30);
    zoom_in_button->setText("Zoom In (+)");
    zoom_in_button->onPress([this]() {
        zoomIn();
    });

    auto zoom_out_button = tgui::Button::create();
    zoom_out_button->setPosition(zoom_in_button->getPosition().x + zoom_in_button->getSize().x + 10, 50);
    zoom_out_button->setSize(100, 30);
    zoom_out_button->setText("Zoom Out (-)");
    zoom_out_button->onPress([this]() {
        zoomOut();
    });

    file_path_edit_box = tgui::EditBox::create();
    file_path_edit_box->setPosition(10, 90);
    file_path_edit_box->setSize(120, 30);
    file_path_edit_box->setDefaultText("  Enter map file path...");
    file_path_edit_box->setTextSize(10);

    load_map_button = tgui::Button::create();
    load_map_button->setPosition(file_path_edit_box->getPosition().x + file_path_edit_box->getSize().x + 10, 90);
    load_map_button->setSize(80, 30);
    load_map_button->setText("Load Map");
    load_map_button->onPress([this]() {
        tgui::String tgui_path = file_path_edit_box->getText();
        std::string new_path = tgui_path.toStdString();
        if (!new_path.empty()) {
            loadMapAndUpdatePath(new_path);
        }
    });

    auto run_astar = tgui::Button::create();
    run_astar->setPosition(10, 130);
    run_astar->setSize(210, 30);
    run_astar->setText("Run AStart (A*)");
    run_astar->onPress([this]() {
        runAStar();
    });

    editing_view->add(start_state);
    editing_view->add(goal_state);
    editing_view->add(zoom_in_button);
    editing_view->add(zoom_out_button);
    editing_view->add(file_path_edit_box);
    editing_view->add(load_map_button);
    editing_view->add(run_astar);
}

sf::Color getBackgroundColorForLocationType(GridLocationType locationType) {
    switch (locationType) {
        case GridLocationType::passable:
            return PASSABLE_COLOR;
        case GridLocationType::obstacle:
            return OBSTACLE_COLOR;
        case GridLocationType::tree:
            return TREE_COLOR;
        case GridLocationType::swamp:
            return SWAMP_COLOR;
        case GridLocationType::water:
            return WATER_COLOR;
        default:
            return OUTSIDE_GRID_COLOR;
    }
}

void GridPathfindingVisualizer::loadMapAndUpdatePath(const std::string& newPath) {
    path = newPath;
    graph_view->removeAllWidgets();
    createGrid();
}

void GridPathfindingVisualizer::zoomIn() {
    if (zoom_level < max_zoom) {
        zoom_level += 0.1f;
        recreateGrid();
    }
}

void GridPathfindingVisualizer::zoomOut() {
    if (zoom_level > min_zoom) {
        zoom_level -= 0.1f;
        recreateGrid();
    }
}

void GridPathfindingVisualizer::recreateGrid() {
    graph_view->removeAllWidgets();
    createGrid();
}

void GridPathfindingVisualizer::runAStar() {
}
