import QtQml 2.13
import QtQuick 2.13
import QtQuick.Window 2.13

import "./"

Window {
  id: _window
  width: 640
  height: 480
  visible: true
  title: Tr.ex_append

  flags: Qt.Window

  MainWindow {
    id: main_window
    anchors.fill: parent
    window: _window
  }

  Component.onCompleted: {
    console.log(kernal.arguments)
    kernal.window = _window
  }

  Connections {
    target: kernal.languageManager
    function onCurrentLanguageChanged(language) {
      console.log(language)
    }
  }

}
