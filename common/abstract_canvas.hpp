//
// Created by zhengqian on 2020/01/05.
//

#ifndef DRAWBOARD_ABSTRACT_CANVAS_HPP
#define DRAWBOARD_ABSTRACT_CANVAS_HPP

#include <cstddef>
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <mutex>

struct Point {
    int x;
    int y;
    Point() : x(0), y(0) {}
    Point(int _x, int _y) : x(_x), y(_y) {}
};

struct BatchInfo {
    uint32_t color;
    std::vector<Point> points;
    explicit BatchInfo(uint32_t _color) : color(_color) {}
};

class AbstractCanvas {
public:
    AbstractCanvas() : is_bitmap_valid_(false) {}
    void BeginDraw(uint32_t uid, uint32_t sequence_id, uint32_t color);
    void EndDraw(uint32_t uid, uint32_t sequence_id);
    void DrawPoint(uint32_t uid, uint32_t sequence_id, Point point);
    void ClearBatch(uint32_t uid, uint32_t sequence_id);
    void Render(int width, int height);
    void InitPixelBuffer(int width, int height);
    void ClearPixelBuffer();
    const char* GetPixelBuffer();
private:
    void RenderPoint(Point p, uint32_t color);
    void RenderLine(Point p1, Point p2, uint32_t color);
private:
    std::mutex mutex_;
    int viewport_width_;
    int viewport_height_;
    std::vector<uint32_t> pixel_buffer_;
    bool is_bitmap_valid_;
    // uid -> sequence_id -> point_batch
    std::unordered_map<uint32_t, std::unordered_map<uint32_t, BatchInfo>> point_table_;
};


#endif //DRAWBOARD_ABSTRACT_CANVAS_HPP
