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
    const float OBJECT_DELAY = 0.9f;
    const float INITIAL_SPEED = 10.0f;
    const sf::Vector2f INITIAL_POSITION = {500.0f, 200.0f};
    const int MAX_OBJECTS = 500;
    sf::Clock clock;

    float x_positions[57] = {500.0, 445.0, 555.0, 610.0, 500.0, 390.0, 335.0, 445.0, 555.0, 675.0, 280.0, 610.0, 500.0, 390.0, 730.0, 225.0, 335.0, 445.0, 555.0, 675.0, 785.0, 225.0, 225.0, 225.0, 225.0, 225.0, 225.0, 335.0, 335.0, 335.0, 335.0, 335.0, 335.0, 445.0, 445.0, 445.0, 445.0, 445.0, 445.0, 555.0, 555.0, 555.0, 555.0, 555.0, 555.0, 675.0, 675.0, 675.0, 675.0, 675.0, 675.0, 785.0, 785.0, 785.0, 785.0, 785.0, 785.0 };
    float y_positions[57] = {350.0, 420.0, 420.0, 490.0, 490.0, 490.0, 560.0, 560.0, 560.0, 560.0, 630.0, 630.0, 630.0, 630.0, 630.0, 700.0, 700.0, 700.0, 700.0, 700.0, 700.0, 800.0, 840.0, 880.0, 920.0, 960.0, 1000, 800.0, 840.0, 880.0, 920.0, 960.0, 1000, 800.0, 840.0, 880.0, 920.0, 960.0, 1000, 800.0, 840.0, 880.0, 920.0, 960.0, 1000, 800.0, 840.0, 880.0, 920.0, 960.0, 1000, 800.0, 840.0, 880.0, 920.0, 960.0, 1000 };

    for(int i = 0; i < 57; i++) {
        auto& object1 = solver.addObject(sf::Vector2f(x_positions[i], y_positions[i]), 20.0f, false);
        solver.setObjectVelocity(object1, INITIAL_SPEED * sf::Vector2f{0.01, 1});
        object1.color = sf::Color::White;
    }

    float x_rect[6] = {205, 315, 425, 535, 655, 765};

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
            auto& object = solver.addObject(INITIAL_POSITION, 10.0f, true);
            std::random_device rd;  // Will be used to obtain a seed for the random number engine
            std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
            std::uniform_int_distribution<> dis(0, 1);
            float random_number = dis(gen);
            solver.setObjectVelocity(object, INITIAL_SPEED * sf::Vector2f{random_number ? -0.05:0.05, 1});
            object.color = sf::Color(255, 70, 64);
        }

        //Update the solver
        solver.update();
        //Clear the window with a black background
        window.clear(sf::Color::Black);
        //Render using the renderer passing the solver
        renderer.render(solver);
        for(int i=0; i< 6; i++) {
            sf::RectangleShape rect1;
            rect1.setPosition(sf::Vector2f(x_rect[i], 780));
            rect1.setSize(sf::Vector2f(40, 220));
            rect1.setFillColor(sf::Color(100, 100, 100));
            window.draw(rect1);
        }
		window.display();
    }
    return 0;
}