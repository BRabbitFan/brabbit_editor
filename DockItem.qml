import QtQuick

import "./"

Window {
  id: root

  flags: Qt.FramelessWindowHint | Qt.Dialog

  title: ""
  property var panel: null

  property bool maximum: true

  signal sigMaximumChanged(bool maximum);

  onMaximumChanged: {
    root.sigMaximumChanged(maximum)
    visible = true
  }

  Rectangle {
    id: panel

    x: 0
    y: 0
    width: root.width
    height: root.height

    color: "lightsteelblue"

    Text {
      anchors.centerIn: panel

      text: root.title
    }

    MouseArea {
      anchors.fill: panel

      onDoubleClicked: {
        root.maximum = !root.maximum
        console.log(root.maximum)
      }
    }
  }
}
