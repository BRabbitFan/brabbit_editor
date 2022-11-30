import QtQuick 2.13
import QtQuick.Window 2.13
import QtQuick.Controls 2.13

import DockController 1.0

Window {
  id: root
  flags: Qt.FramelessWindowHint | Qt.Dialog

  property var window: null
  property alias controller: controller

  property int adsorbedX: x
  property int adsorbedY: y + tab_bar.height
  property int adsorbedWidth: width
  property int adsorbedHeight: height - tab_bar.height

  color: "transparent"

  visible: controller.contextVisible

  Connections {
    target: window

    onXChanged: {
      x = window.x
    }

    onYChanged: {
      y = window.y
    }
  }

  DockController {
    id: controller
    context: root
  }

  function append(item) {
    controller.appendItem(item)
  }

  function remove(item) {
    controller.removeItem(item)
  }

  TabBar {
    id: tab_bar

    anchors.top: parent.top
    anchors.left: parent.left
    anchors.right: parent.right

    height: 40

    background: Rectangle {
      anchors.fill: parent
      color: "green"
    }

    Repeater {
      model: controller.contextTitleList

      TabButton {
        id: button

        anchors.top: parent.top
        anchors.bottom: parent.bottom

        width: 120

        background: Rectangle {
          anchors.fill: parent
          color: button.checked ? "orange" : "yellow"
        }

        text: modelData

        onDoubleClicked: {
          var item = controller.findItemByTitle(modelData)
          if (item) { item.adsorbed = false }
        }
      }
    }

    onCurrentItemChanged: {
      if (!currentItem) { return }
      var item = controller.findItemByTitle(currentItem.text)
      if (item) { controller.focusItem(item) }
    }
  }
}
