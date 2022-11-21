#pragma once

class Drawable
{
public:
    virtual ~Drawable() noexcept = default;

    virtual void draw() = 0;
};