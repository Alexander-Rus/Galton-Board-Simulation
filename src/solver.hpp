#pragma once
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>


//Here we are creating a verletobject
struct VerletObject
{
    //We have the position, the last position, the acceleration and the color
    sf::Vector2f position;
    sf::Vector2f position_last;
    sf::Vector2f acceleration;
    sf::Color color = sf::Color::White;
    float radius = 10.0f;
    bool movable = true;


    VerletObject() = default;

    VerletObject(sf::Vector2f position_, float radius_, bool movable_)
        : position{position_}
        , position_last{position_}
        , acceleration{0.0f, 0.0f}
        , radius{radius_}
        , movable{movable_}
    {}

    void update(float dt) {
        // Calculate the displacement and update position if object is movable
        const sf::Vector2f displacement = position - position_last;

        if(!this->movable){
            position = position_last;
        } else {
        position_last = position;
        position = position + displacement + acceleration * (dt * dt);
        }
        acceleration  = {};
    }

    //Increase acceleration by a
    void accelerate(sf::Vector2f a) {
        acceleration += a;
    }

    //This is a method to set the velocity given the dt and the velocity, set the position last
    void setVelocity(sf::Vector2f v, float dt) {
        position_last = position - (v * dt);
    }

    void addVelocity(sf::Vector2f v, float dt) {
        position_last -= v * dt;
    }

    sf::Vector2f getVelocity(float dt) const {
        return (position - position_last) / dt;
    }
};


class Solver{

public:    
    Solver() = default;

    //Add the object to the list of objects
    VerletObject& addObject(sf::Vector2f position, float radius, bool movable){
        //Return the vector of verlet objects
        return m_objects.emplace_back(position, radius, movable);
    }

    void update(){
        //takes the time which is a float and updates it with the frame time
        m_time += m_frame_dt;
        const float STEP_DT = getStepDt();
        for (int i{m_sub_steps}; i--;) {
            applyGravity();
            checkCollisions();
            applyConstraint(); 
            updateObjects(STEP_DT);
        }
    }

    void setSimulationUpdateRate(int rate){
        m_frame_dt = 1.0f / static_cast<float>(rate);
    }

    void setConstraint(sf::Vector2f position, float radius){
        m_constraint_center = position;
        m_constraint_radius = radius;
    }

    void setSubStepsCount(int sub_steps) {
        m_sub_steps = sub_steps;
    }

    void setObjectVelocity(VerletObject& object, sf::Vector2f v) {
        object.setVelocity(v, getStepDt());
    }

    const std::vector<VerletObject>& getObjects() const {
        return m_objects;
    }

    sf::Vector3f getConstraint() const{
        return {m_constraint_center.x, m_constraint_center.y, m_constraint_radius};
    }

    int getObjectsCount() const{
        return m_objects.size();
    }

    float getTime() const{
        return m_time;
    }

    float getStepDt() const{
        return m_frame_dt / static_cast<float>(m_sub_steps);
    }

private:
    int m_sub_steps = 1;
    sf::Vector2f m_gravity = {0.0f, 1000.0f};
    sf::Vector2f m_constraint_center;
    float m_constraint_radius  = 100.0f;
    std::vector<VerletObject> m_objects;
    float m_time = 0.0f;
    float m_frame_dt = 0.0f;

    void applyGravity(){
        for (auto& obj : m_objects) {
            obj.accelerate(m_gravity);
        }
    }

    //This formula for collison check is from https://github.com/johnBuffer/VerletSFML
    void checkCollisions()
    {
        const float    response_coef = 0.75f;
        const int objects_count = m_objects.size();
        // Iterate on all objects
        for (int i{0}; i < objects_count; ++i) {
            VerletObject& object_1 = m_objects[i];
            // Iterate on object involved in new collision pairs
            for (int k{i + 1}; k < objects_count; ++k) {
                VerletObject&      object_2 = m_objects[k];
                const sf::Vector2f v        = object_1.position - object_2.position;
                const float        dist2    = v.x * v.x + v.y * v.y;
                const float        min_dist = object_1.radius + object_2.radius;
                // Check overlapping
                if (dist2 < min_dist * min_dist) {
                    const float        dist  = sqrt(dist2);
                    const sf::Vector2f n     = v / dist;
                    const float mass_ratio_1 = object_1.radius / (object_1.radius + object_2.radius);
                    const float mass_ratio_2 = object_2.radius / (object_1.radius + object_2.radius);
                    const float delta        = 0.5f * response_coef * (dist - min_dist);
                    // Update positions
                    object_1.position -= n * (mass_ratio_2 * delta);
                    object_2.position += n * (mass_ratio_1 * delta);
                }
            }
        }
    }

    // Prevent objects from leaving bounds of simulation
    void applyConstraint(){
        for (auto& obj : m_objects) {
            if (obj.position.y >= 994.0f) {
                obj.position.y = 994.0f;
            }
            if (obj.position.x <= 4.0f) {
                obj.position.x = 4.0f;
            } else if (obj.position.x >= 994.0f) {
                obj.position.x = 994.0f;
            }

            if(obj.position.y > 700){
                selectColumn(obj);
            }
        }
    }

    void updateObjects(float dt){
        for (auto& obj : m_objects) {
            obj.update(dt);
        }
    }

    void selectColumn(VerletObject &obj){
        float entrance_x = obj.position.x;
        
    }
};