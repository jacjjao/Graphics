#pragma once

class Drawable
{
public:
    virtual ~Drawable() = default;

    virtual void draw() = 0;
    virtual void resize() = 0;
};