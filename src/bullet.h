#ifndef __BULLET_AGENT__H
#define __BULLET_AGENT__H 

#include "enviro.h"

using namespace enviro;

class BulletController : public Process, public AgentInterface {

    public:
    BulletController() : Process(), AgentInterface() {}
    //Design the Bullet function. AKA how does the bullet work.
    void init() {
        notice_collisions_with("Enemy", [&](Event &e) { //when bullet collide with enemy.
            remove_agent(e.value()["id"]);
            remove_agent(id());
        });  
    }
    void start() {}
    void update() {
        if ( counter++ > 20 ) {
            remove_agent(id());
        }
    }
    void stop() {}

    double counter;
};

class Bullet : public Agent {
    public:
    Bullet(json spec, World& world) : Agent(spec, world) {
        add_process(c);
    }
    private:
    BulletController c;
};

DECLARE_INTERFACE(Bullet)

#endif