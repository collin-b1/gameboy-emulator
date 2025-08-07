#pragma once

#include "definitions.h"
#include "ppu/obj.h"
#include <span>

class IFrameListener
{
public:
    virtual ~IFrameListener() = default;

    virtual void on_frame_ready(const std::span<const u32> frame_buffer, size_t size) = 0;
};

class ISpriteFrameListener
{
public:
    virtual ~ISpriteFrameListener() = default;

    virtual void on_frame_ready(const std::array<Object, 40> &sprites) = 0;
};