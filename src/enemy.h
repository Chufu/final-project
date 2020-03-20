#ifndef __ENEMY_AGENT__H
#define __ENEMY_AGENT__H 

#include "enviro.h"

using namespace enviro;

class enemyController : public Process, public AgentInterface {

    public:
    enemyController() : Process(), AgentInterface() {}

    void init() {
        notice_collisions_with("Bullet", [&](Event &e) { //bullet will kill enemy, and enemy reemove
            Agent& bullet = find_agent(e.value()["id"]);
            vx = bullet.velocity().x;
            vy = bullet.velocity().y;
            ignore_collisions_with("Bullet");
            hit = true;
        });
        notice_collisions_with("Robot", [&](Event &e) { // if tank crashes to enemy, they both die.
            Agent& robot = find_agent(e.value()["id"]);
            remove_agent(id());
        });
    }
    void start() {}
    // This is hw your enemy move.
    void update() {
        double d = sqrt(x()*x() + y()*y()),
               vx = -x() / ( 1 + d ),
               vy = -y() / ( 1 + d );        
        omni_apply_force(
            (rand() % fmax) - fmax/2 + 2*vx, 
            (rand() % fmax) - fmax/2 + 2*vy
        );
    }
    void stop() {}
    const int fmax = 100.0;
    bool hit;
    double vx, vy;  
};

class enemy : public Agent {
    public:
    enemy(json spec, World& world) : Agent(spec, world) {
        add_process(c);
    }
    private:
    enemyController c;
};

DECLARE_INTERFACE(enemy)

#endif