#pragma once

#include <array>
#include <cstdint>

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

// static constexpr std::array<u32, 4> pixel_colors{0xFFFFFFFF, 0xFFAAAAAA, 0xFF555555, 0xFF000000};
static constexpr std::array<u32, 4> pixel_colors{0xFF3F9E9A, 0xFF226B49, 0xFF0B450E, 0xFF092A1B};

static constexpr int SCREEN_WIDTH = 160;
static constexpr int SCREEN_HEIGHT = 144;
static constexpr int SCREEN_SIZE = SCREEN_WIDTH * SCREEN_HEIGHT;

static constexpr int DEBUG_RENDER_SCALE = 2;

static constexpr auto TILE_MAP_TILES_PER_ROW = 16;
static constexpr auto TILE_MAP_TOTAL_ROWS = 24;

static constexpr auto TILE_MAP_WIDTH = 8 * TILE_MAP_TILES_PER_ROW;
static constexpr auto TILE_MAP_HEIGHT = 8 * TILE_MAP_TOTAL_ROWS;

static constexpr auto TILE_MAP_SIZE = TILE_MAP_WIDTH * TILE_MAP_HEIGHT;