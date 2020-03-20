#ifndef __ROBOTS_AGENT__H
#define __ROBOTS_AGENT__H 

#include "enviro.h"

using namespace enviro;

class RobotController : public Process, public AgentInterface {

    public:
    RobotController() : Process(), AgentInterface() {}

    void init() {
        notice_collisions_with("Enemy", [&](Event &e) { //if you crash to your enemy, you both die.
            Agent& enemy = find_agent(e.value()["id"]);
            remove_agent(id());
        });
        // how I set to control the tank.
        watch("keydown", [&](Event &e) {
            auto k = e.value()["key"].get<std::string>();
            if ( k == " " && !firing ) {
                  Agent& bullet = add_agent("Bullet", 
                    x() + 17*cos(angle()), 
                    y() + 17*sin(angle()), 
                    angle(), 
                    BULLET_STYLE);    
                    bullet.apply_force(100,0);
                  firing = true;
            } else if ( k == "w" ) {
                  v = v_m;              
            } else if ( k == "s" ) {
                  v = -v_m;  
            } else if ( k == "a" ) {
                  omega = -omega_m;
            } else if ( k == "d" ) {
                  omega = omega_m;
            } 
        });        
        watch("keyup", [&](Event &e) {
            auto k = e.value()["key"].get<std::string>();
            if ( k == " " ) {
                firing = false;
            } else if ( k == "w" || k == "s" ) {
                  v = 0;               
            } else if ( k == "a" ) {
                  omega = 0;
            } else if ( k == "d" ) {
                  omega = 0;
            } 
        });
        center(x(), y());
        zoom(1.5);
    }
    void start() {}
    //the tank movement status 
    void update() {
        track_velocity(v,omega,10,400);
        center(x(), y());
    }
    void stop() {}    
    
    double v, omega;
    double const v_m = 30, omega_m = 1;
    bool firing;
    const json BULLET_STYLE = {
                    {"fill","black"},
                    {"stroke","#888"},
                    {"stroke-width","5px"},
                    {"stroke-opacity","0.25"}
                    };

};

class Robot : public Agent {
    public:
    Robot(json spec, World& world) : Agent(spec, world) {
        add_process(c);
    }
    private:
    RobotController c;
};
DECLARE_INTERFACE(Robot)

#endif