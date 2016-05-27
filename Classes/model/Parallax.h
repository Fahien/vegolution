#ifndef __PARALLAX_H__
#define __PARALLAX_H__

#include "cocos2d.h"
#include "MainActor.h"

class Parallax : public cocos2d::Node
{
public:
    Parallax(MainActor* actor);

    static Parallax* create(const std::string name, MainActor* actor);

    void createBackgrounds(const std::string name);

    void update(float delta);
private:
    static constexpr size_t backgroundMax {5};

    std::vector<std::vector<Actor*>> backgrounds_;

    float scale_;

    MainActor* actor_;
};

#endif // __PARALLAX_H__
