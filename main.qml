import QtQml
import QtQuick
import QtQuick.Window

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
