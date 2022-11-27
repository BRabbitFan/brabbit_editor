#pragma once

#include <set>
#include <functional>

#include <QCoreApplication>

namespace br {

namespace tr {

inline QString load(const char* source) { return QObject::tr(source); }

inline QString base_append() { return load("base_append"); }  // 添加
inline QString base_remove() { return load("base_remove"); }  // 移除
inline QString game_play() { return load("game_play"); }  // 玩
inline QString media_play() { return load("media_play"); }  // 播放
inline QString main_append() { return load("main_append"); }  // 添加
inline QString main_remove() { return load("main_remove"); }  // 移除
inline QString main_play() { return load("main_play"); }  // 播放
inline QString test_test() { return load("test_test"); }  // 测试
inline QString ex_append() { return load("ex_append"); }  // 添加
inline QString ex_remove() { return load("ex_remove"); }  // 移除
inline QString ex_play() { return load("ex_play"); }  // 玩
inline QString ex_playex() { return load("ex_playex"); }  // 播放

}

}
