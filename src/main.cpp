#include <iostream>
#include <SFML/Graphics.hpp>
#include "solver.hpp"
#include "renderer.hpp"
#include <random>


int main() {
    
    // Set the dimensions of the window
    const int WINDOW_WIDTH = 1000;
    const int WINDOW_HEIGHT = 1000;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 1;
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Galton Board", sf::Style::Default, settings);
    const int FRAME_RATE = 60;
    window.setFramerateLimit(FRAME_RATE);

    //Create the solver and renderer
    Solver solver;
    Renderer renderer{window};


    solver.setConstraint({static_cast<float>(WINDOW_WIDTH) * 0.5, static_cast<float>(WINDOW_HEIGHT) * 0.5}, 450.0);
    solver.setSubStepsCount(8); 
    solver.setSimulationUpdateRate(FRAME_RATE);


    // Simulation parameters
    const float OBJECT_DELAY = 0.2f;
    const float INITIAL_SPEED = 10.0f;
    const sf::Vector2f INITIAL_POSITION = {500.0, 100.0};
    const int MAX_OBJECTS = 1000;
    sf::Clock clock;

    for(int j{0}; j < 6; j++){
        for(int i{0}; i < 30; i++){
            auto& object1 = solver.addObject(sf::Vector2f(i*33.3 + 1.5, 400 + (j * 50)), 4.0f, false);
            solver.addObject(sf::Vector2f(i*33.3+18.15, 400 + (j * 50 + 25)), 4.0f, false);
        }
    }
    

     // Check for inputs to close window
    while (window.isOpen()) {
        sf::Event event{};

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }
        }
        
        if (solver.getObjectsCount() < MAX_OBJECTS && clock.getElapsedTime().asSeconds() >= OBJECT_DELAY) {
            
            clock.restart();
            std::random_device rd; 
            std::mt19937 gen(rd()); 
            std::uniform_int_distribution<> dis(300, 700);
            float random_number = dis(gen);

            auto& object = solver.addObject(sf::Vector2f(random_number, 100.0), 6.0f, true);
            solver.setObjectVelocity(object, sf::Vector2f(INITIAL_SPEED, 1));
            object.color = sf::Color(255, 70, 64);
        }

        //Update the solver
        solver.update();
        //Clear the window with a black background
        window.clear(sf::Color(33, 33, 33, 32));
        //Render using the renderer passing the solver
        renderer.render(solver);
        for(int i=0; i<31; i++) {
            sf::RectangleShape rect1;
            rect1.setPosition(sf::Vector2f(i * 33.3, 700));
            rect1.setSize(sf::Vector2f(3, 300));
            rect1.setFillColor(sf::Color(104, 104, 104));
            window.draw(rect1);
        }
		window.display();
    }
    return 0;
}