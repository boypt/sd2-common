#pragma once

#include <Arduino.h>

namespace sd2 {

// 跨午夜休眠窗口判断：
//  - hour < 0（时间未知）不进入休眠
//  - start / end 任一 < 0 或相等 = 禁用
//  - start < end: [start, end)；start > end: 跨午夜（如 22:00-07:00）
inline bool inSleepWindow(int hour, int startHour, int endHour) {
    if (hour < 0 || startHour < 0 || endHour < 0 || startHour == endHour)
        return false;
    if (startHour < endHour)
        return hour >= startHour && hour < endHour;
    return hour >= startHour || hour < endHour;
}

// 休眠状态机：用当前本地小时驱动，进入/退出时触发回调
class SleepScheduler {
public:
    typedef void (*Callback)();

    SleepScheduler() = default;
    SleepScheduler(int startHour, int endHour) { setWindow(startHour, endHour); }

    void setWindow(int startHour, int endHour) {
        startHour_ = startHour;
        endHour_ = endHour;
    }

    // 传入当前本地小时（未知传 -1）；返回 true 表示刚发生状态变化
    bool update(int hour) {
        bool want = inSleepWindow(hour, startHour_, endHour_);
        if (want == sleeping_)
            return false;
        sleeping_ = want;
        if (want) {
            if (onEnter_) onEnter_();
        } else {
            if (onExit_) onExit_();
        }
        return true;
    }

    bool sleeping() const { return sleeping_; }

    // 休眠/唤醒期间轮询间隔不同时直接取
    uint32_t interval(uint32_t awakeMs, uint32_t sleepMs) const {
        return sleeping_ ? sleepMs : awakeMs;
    }

    void onEnter(Callback cb) { onEnter_ = cb; }
    void onExit(Callback cb) { onExit_ = cb; }

private:
    int startHour_ = -1;
    int endHour_ = -1;
    bool sleeping_ = false;
    Callback onEnter_ = nullptr;
    Callback onExit_ = nullptr;
};

} // namespace sd2
