import QtQuick

Window {
  flags: Qt.FramelessWindowHint | Qt.Dialog
  property var window: null

  color: "red"

  Connections {
    target: window
    onXChanged:{
      x = window.x
    }
    onYChanged:{
      y = window.y
    }
  }
}
