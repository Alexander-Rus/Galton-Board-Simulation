# Galton Board Simulation in C++
This is a physics simulation project to try and re-create a Galton Board using C++. 

<p align="center">
<img src="https://raw.githubusercontent.com/Alexander-Rus/Galton-Board-Simulation-in-C-/main/img/Screenshot%20from%202023-01-08%2014-45-41.png"
  alt="Galton Board"
  width="894" height="255">
</p>

## Glaton Board

A Galton board is a device that is intended to show the central limit theorem. A vertical board has pegs arranged in a triangular pattern, balls are then dropped from the very top and will bounce off the pegs. In theory, the ball has a 50/50 percent chance of bouncing either left or right, and will then strike another peg. This process continues until all balls reach the bottom and land in specific columns. The number of balls in each column will show a bell curve, and this is supposed to demonstrate the central limit theorem.

<p align="center">
<img src="https://raw.githubusercontent.com/Alexander-Rus/Galton-Board-Simulation-in-C-/main/img/1.png"
  alt="Galton Board Example"
  width="1920" height="1080">
</p>

The goal of this project was the re-create this effect in c++. In order to do this balls would need to be able to realistically interact with pegs, and be collected at the bottom of the board. 

## C++ Simulation
c++ is often used as a tool for physcis simulation due to it's efficiency. To calculate the physical interaction between the balls and pegs, I used Verlet integration.Verlet integration approximates the position and velocity of an object at a discrete time step. Given the position and velocity of an object at time t, Verlet integration can be used to compute the position of the object at time t+dt (where dt is the time step). This is done using the following equations:

> x(t+dt) = x(t) + v(t) * dt + (a(t) * dt^2) / 2
> v(t+dt) = v(t) + (a(t) + a(t+dt)) / 2 * dt

Here, x(t) is the position of the object at time t, v(t) is its velocity at time t, and a(t) is its acceleration at time t. These equations allow you to compute the position and velocity of the object at the next time step, given its position, velocity, and acceleration at the current time step.

Verlet integration is useful because it can be implemented for more simplistic simulations like this.

## Results
The implementation of the physcics simulation was successful. The objects are able to interact with eachother, stike pegs, and fall into the columns at the bottom of the board.

However this simulation does not perfectly reflect a real like example of a Glaton board and this is likely due to a few reasons.
* The placement of the pegs is an approximation, and likely do need to be placed in better positions to give the exact 50/50 percent chance of going left of right
* The balls are able to bounce off the pegs and eachother, but often times will bounce too much and automatically go to the furthest columns.
* The initial velocity has to be set to have a slight left or right value, otherwise the balls will land ontop of each other and form a tower.
<p align="center">
<img src="https://raw.githubusercontent.com/Alexander-Rus/Galton-Board-Simulation-in-C-/main/img/Screenshot%20from%202023-01-08%2014-42-17.png"
  alt="Example balls stacking"
  width="1002" height="1045">
</p>

The code to prevent this from happening is the following:
```            
float random_number = dis(gen);
solver.setObjectVelocity(object, INITIAL_SPEED * sf::Vector2f{random_number ? -0.05:0.05, 1});
```



## Run Locally
**Make sure you have SFML installed locally**
1. Clone this repository.
2. I have included a make file. in the main directory type `make all`
3. in the `bin` directory there should be an executable file, you can run it from the main directory by typing `./bin/main`



