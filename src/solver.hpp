#pragma once
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <random>


//Here we are creating a verletobject
struct VerletObject
{
    //We have the position, the last position, the acceleration and the color
    int obj_index;
    sf::Vector2f position;
    sf::Vector2f position_last;
    sf::Vector2f acceleration;
    sf::Color color = sf::Color(104, 104, 104);
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
        
        int grid_x = (int)position.x / 10;
        int grid_y = (int)position.y / 10;
        mygrid[grid_x][grid_y].push_back(global_index);
        global_index++;
        
        return vector_of_objects.emplace_back(position, radius, movable);
    }

    void update(){
        //takes the time which is a float and updates it with the frame time
        local_time += frame_dt;
        const float STEP_DT = getStepDt();
        for (int i{num_sub_steps}; i--;) {
            applyGravity();
            checkCollisions();
            applyConstraint(); 
            updateObjects(STEP_DT);
        }
    }

    void setSimulationUpdateRate(int rate){
        frame_dt = 1.0f / static_cast<float>(rate);
    }

    void setConstraint(sf::Vector2f position, float radius){
        center_contraints = position;
        radius_contraint = radius;
    }

    void setSubStepsCount(int sub_steps) {
        num_sub_steps = sub_steps;
    }

    void setObjectVelocity(VerletObject& object, sf::Vector2f v) {
        object.setVelocity(v, getStepDt());
    }

    const std::vector<VerletObject>& getObjects() const {
        return vector_of_objects;
    }

    sf::Vector3f getConstraint() const{
        return {center_contraints.x, center_contraints.y, radius_contraint};
    }

    int getObjectsCount() const{
        return vector_of_objects.size();
    }

    float getTime() const{
        return local_time;
    }

    float getStepDt() const{
        return frame_dt / static_cast<float>(num_sub_steps);
    }

private:
    int num_sub_steps = 1;
    sf::Vector2f object_gravity = {0.0f, 1000.0f};
    sf::Vector2f center_contraints;
    float radius_contraint  = 100.0f;
    std::vector<VerletObject> vector_of_objects;
    float local_time = 0.0f;
    float frame_dt = 0.0f;
    std::vector<int> mygrid[100][100];
    int global_index = 0;

    void applyGravity(){
        for (auto& obj : vector_of_objects) {
            obj.accelerate(object_gravity);
        }
    }

    
    
    //This formula for collison check is from https://github.com/johnBuffer/VerletSFML
    void checkCollisions(){
        const float    response_coef = 0.75f;
        const int objects_count = vector_of_objects.size();
        std::vector<int> objects_in_area;
        // Iterate on all objects
        for (int i{0}; i < objects_count; ++i) {
            VerletObject& object_1 = vector_of_objects[i];


            //From this object, get the list of indexes around it
            //std::vector<int> surrounding_contents = getSurroundingCells(object_1);
            //We then removet the index of the current object
            //surrounding_contents.erase(std::remove(surrounding_contents.begin(), surrounding_contents.end(), i), surrounding_contents.end());

            // Iterate on object involved in new collision pairs
            for (int k{i + 1}; k < objects_count; ++k) {
                VerletObject&      object_2 = vector_of_objects[k];
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
        for (auto& obj : vector_of_objects) {
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
        for(int i{0}; i< vector_of_objects.size(); i++){
            int grid_x = (int)vector_of_objects[i].position.x / 10;
            int grid_y = (int)vector_of_objects[i].position.y / 10;
            std::vector<int> temp_vec = mygrid[grid_x][grid_y];
            temp_vec.erase(std::remove(temp_vec.begin(), temp_vec.end(), i), temp_vec.end());
            mygrid[grid_x][grid_y] = temp_vec;

            vector_of_objects[i].update(dt);

            int grid_x_new = (int)vector_of_objects[i].position.x / 10;
            int grid_y_new = (int)vector_of_objects[i].position.y / 10;
            mygrid[grid_x_new][grid_y_new].push_back(i);
        }
        
        
        /*
        for (auto& obj : vector_of_objects) {

            int grid_x = (int)obj.position.x / 10;
            int grid_y = (int)obj.position.y / 10;
            std::vector<int> temp_vec = mygrid[grid_x][grid_y];
            temp_vec.erase(std::remove(temp_vec.begin(), temp_vec.end(), obj.obj_index), temp_vec.end());
            mygrid[grid_x][grid_y] = temp_vec;
            
            obj.update(dt);

                        
        }
        */
    }

    std::vector<int> getContentsAtCell(VerletObject& obj){
        int grid_x = (int)obj.position.x / 10;
        int grid_y = (int)obj.position.y / 10;
        return mygrid[grid_x][grid_y];
    }

    std::vector<int> getSurroundingCells(VerletObject obj){
        //Get the position on the grid of the object
        int grid_x = (int)obj.position.x / 10;
        int grid_y = (int)obj.position.y / 10;
        std::vector<int> builder;
        addToBuilder(builder, mygrid[grid_x - 1][grid_y - 1]);
        addToBuilder(builder, mygrid[grid_x - 1][grid_y]);
        addToBuilder(builder, mygrid[grid_x + 1][grid_y + 1]);
        addToBuilder(builder, mygrid[grid_x][grid_y - 1]);
        addToBuilder(builder, mygrid[grid_x][grid_y]);
        addToBuilder(builder, mygrid[grid_x][grid_y + 1]);
        addToBuilder(builder, mygrid[grid_x+1][grid_y-1]);
        addToBuilder(builder, mygrid[grid_x+1][grid_y]);
        addToBuilder(builder, mygrid[grid_x+1][grid_y+1]);
        return builder;
    }

    void addToBuilder(std::vector<int>& builder, std::vector<int> values){
        for(int i{0}; i < values.size(); i++){
            builder.emplace_back(values[i]);
        }
    }


    void selectColumn(VerletObject &obj){
        for(int i{0}; i < 30; i++){
            if(abs(obj.position.x - (i*33.3 + 18.15)) < 15.5){
                if(obj.position.x < (i*33.3 + 3)){
                    obj.position.x = i*33.3 + 3;
                }
                if(obj.position.x > (i*33.3 + 18.15) + 15.15){
                    obj.position.x = (i*33.3 + 18.15) + 15.15;
                }
            }
        }

        std::random_device rd;  // Will be used to obtain a seed for the random number engine
        std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
        std::uniform_int_distribution<> dis(0, 1);
        std::uniform_int_distribution<> dis2(0, 20);
        float random_number = dis(gen);
        if(dis2(gen) == 1){
            obj.position.x += (random_number ? -0.01:0.01);
        }

    }
};