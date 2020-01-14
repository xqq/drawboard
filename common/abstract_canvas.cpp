//
// Created by zhengqian on 2020/01/05.
//

#include <cstring>
#include "log.hpp"
#include "abstract_canvas.hpp"

uint32_t AbstractCanvas::BeginDraw(uint32_t uid, uint32_t sequence_id, uint32_t color) {
    Log::Info("BeginDraw");
    auto target_user = point_table_.find(uid);
    if (target_user == point_table_.end()) {
        target_user = point_table_.insert({uid, std::unordered_map<uint32_t, BatchInfo>()}).first;
    }

    BatchInfo batch_info(color);
    target_user->second.insert({sequence_id, batch_info});
    return 0;
}

void AbstractCanvas::EndDraw(uint32_t uid, uint32_t sequence_id) {

}

void AbstractCanvas::DrawPoint(uint32_t uid, uint32_t sequence_id, Point point) {
    Log::InfoF("DrawPoint: uid = %d, sequence_id = %d, x = %d, y = %d\n", uid, sequence_id, point.x, point.y);
    auto target_user = point_table_.find(uid);
    if (target_user == point_table_.end()) {
        Log::Info("return1");
        return;
    }

    auto target_batch = target_user->second.find(sequence_id);
    if (target_batch == target_user->second.end()) {
        Log::Info("return2");
        return;
    }

    target_batch->second.points.push_back(point);
    is_bitmap_valid_ = false;
}

void AbstractCanvas::ClearBatch(uint32_t uid, uint32_t sequence_id) {
    auto target_user = &point_table_.find(uid)->second;
    auto iter = target_user->find(sequence_id);
    if (iter != target_user->end()) {
        target_user->erase(iter);
        is_bitmap_valid_ = false;
    }
}

void AbstractCanvas::Render(int width, int height) {
    if (is_bitmap_valid_) {
        return;
    }

    Log::InfoF("Render: width = %d, height = %d\n", width, height);
    viewport_width_ = width;
    viewport_height_ = height;

    size_t target_size = width * height;
    if (pixel_buffer_.size() != target_size) {
        pixel_buffer_.resize(target_size);
        InitPixelBuffer(width, height);
    }

    for (auto& user_pair : point_table_) {
        for (auto& batch_pair : user_pair.second) {
            BatchInfo* batch_info = &batch_pair.second;
            if (batch_info->points.empty()) {
                continue;
            }

            Point prev_point = batch_info->points[0];
            RenderPoint(prev_point, batch_info->color);
            for (size_t i = 1; i < batch_info->points.size(); i++) {
                Point current_point = batch_info->points[i];
                RenderLine(prev_point, current_point, batch_info->color);
                prev_point = current_point;
            }
        }
    }

    is_bitmap_valid_ = true;
}

void AbstractCanvas::RenderPoint(Point p, uint32_t color) {
    size_t addr = p.y * viewport_width_ + p.x;
    pixel_buffer_[addr] = color;
}

void AbstractCanvas::RenderLine(Point p1, Point p2, uint32_t color){
    Log::InfoF("RenderLine: x1 = %d, y1 = %d, x2 = %d, y2 = %d, color = %d\n", p1.x, p1.y, p2.x, p2.y, color);
    bool steep = std::abs(p2.y - p1.y) > std::abs(p2.x - p1.x);
    if (steep) {
        std::swap(p1.x, p1.y);
        std::swap(p2.x, p2.y);
    }
    if (p1.x > p2.x) {
        std::swap(p1, p2);
    }
    int delta_x = p2.x - p1.x;
    int delta_y = std::abs(p2.y - p1.y);
    int error = delta_x / 2;
    int ystep = p1.y < p2.y ? 1 : -1;
    int y = p1.y;

    for (int x = p1.x; x <= p2.x; x++) {
        if (steep) {
            RenderPoint(Point(y, x), color);
        } else {
            RenderPoint(Point(x, y), color);
        }
        error -= delta_y;
        if (error < 0) {
            y += ystep;
            error += delta_x;
        }
    }
}

void AbstractCanvas::InitPixelBuffer(int width, int height) {
    Log::InfoF("InitPixelBuffer: width = %d, height = %d\n", width, height);
    pixel_buffer_.resize(width * height);
    memset(reinterpret_cast<void*>(&pixel_buffer_[0]), 255, sizeof(uint32_t) * width * height);
}

const char* AbstractCanvas::GetPixelBuffer() {
    return reinterpret_cast<const char*>(&pixel_buffer_[0]);
}
