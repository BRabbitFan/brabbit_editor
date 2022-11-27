pragma Singleton

import QtQml 2.13

QtObject {

  function load(source) { return qsTr(source) }

  readonly property string base_append: load("base_append")  // 添加
  readonly property string base_remove: load("base_remove")  // 移除
  readonly property string game_play: load("game_play")  // 玩
  readonly property string media_play: load("media_play")  // 播放
  readonly property string main_append: load("main_append")  // 添加
  readonly property string main_remove: load("main_remove")  // 移除
  readonly property string main_play: load("main_play")  // 播放
  readonly property string test_test: load("test_test")  // 测试
  readonly property string ex_append: load("ex_append")  // 添加
  readonly property string ex_remove: load("ex_remove")  // 移除
  readonly property string ex_play: load("ex_play")  // 玩
  readonly property string ex_playex: load("ex_playex")  // 播放

}
