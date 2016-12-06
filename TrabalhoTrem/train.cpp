#include "train.h"

Train::Train(int id, Shape shape, Position initalPosition, std::vector<Semaphore *> *semaphores, Direction d) {
    this->id = id;
    this->shape = shape;
    this->currPos = initalPosition;
    this->speed = 100;
    this->enable = true;
    this->sems = semaphores;
    this->move_direction = d;
}

Train::~Train() {
    threadTrem.detach();
}

void Train::start() {
    threadTrem = std::thread(&Train::run, this);
}

void Train::move() {
    int walk_size = 5;
    if (move_direction == Direction::CLOCKWISE) {
        if (currPos.y == shape.top() && currPos.x < shape.right()) {
            currPos.x += walk_size;
        } else if (currPos.x == shape.right() && currPos.y < shape.bottom()) {
            currPos.y += walk_size;
        } else if (currPos.x > shape.left() && currPos.y == shape.bottom()) {
            currPos.x -= walk_size;
        } else {
            currPos.y -= walk_size;
        }
    } else {
        if (currPos.y == shape.top() && currPos.x > shape.left()) {
            currPos.x -= walk_size;
        } else if (currPos.x == shape.left() && currPos.y < shape.bottom()) {
            currPos.y += walk_size;
        } else if (currPos.x < shape.right() && currPos.y == shape.bottom()) {
            currPos.x += walk_size;
        } else {
            currPos.y -= walk_size;
        }
    }
}

void Train::run() {
    while (true) {
        if (enable) {
            emit updateGUI(id, currPos.x, currPos.y);
            verifySection();
            move();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(speed));
    }
}

void Train::verifySection() {
    int safety_margin = 20;
    if (id == 1) {
        // Entering on first Critial Section
        if (currPos.x == shape.left() && currPos.y == shape.bottom() - safety_margin) {
            sems->at(0)->P();
        }
        // Exiting from first Critial Section
        if (currPos.x == shape.right() && currPos.y == shape.bottom() - safety_margin) {
            sems->at(0)->V();
        }
    } else if (id == 2) {
        // Entering on first Critial Section
        if (currPos.x == shape.left() && currPos.y == shape.top() + safety_margin) {
            sems->at(0)->P();
            sems->at(1)->P();
        }
        // Exiting from first Critial Section
        if (currPos.x == shape.right() && currPos.y == shape.top() + safety_margin) {
            sems->at(0)->V();
        }
        // Exiting from second Critial Section
        if (currPos.x == shape.right() - safety_margin && currPos.y == shape.bottom()) {
            sems->at(1)->V();
        }
    } else if (id == 3) {
        // Entering on third Critial Section
        if (currPos.x == shape.right() && currPos.y == shape.bottom() - safety_margin) {
            sems->at(2)->P();
            sems->at(1)->P();
        }
        // Exiting from third Critial Section
        if (currPos.x == shape.left() && currPos.y == shape.bottom() - safety_margin) {
            sems->at(2)->V();
        }
        // Exiting from second Critial Section
        if (currPos.x == shape.left() + safety_margin && currPos.y == shape.top()) {
            sems->at(1)->V();
        }
    } else if (id == 4) {
        // Entering on third Critial Section
        if (currPos.x == shape.left() && currPos.y == shape.top() + safety_margin) {
            sems->at(2)->P();
        }
        // Exiting from third Critial Section
        if (currPos.x == shape.right() && currPos.y == shape.top() + safety_margin) {
            sems->at(2)->V();
        }
    }
}

void Train::setSpeed(int speed) {
    if (speed > 0)
        this->speed = speed;
    this->speed = 10;
}

void Train::setEnable(bool enable) {
    this->enable = enable;
}
