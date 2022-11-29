import QtQuick
import QtQuick.Controls

import DockController

Window {
  id: root
  flags: Qt.FramelessWindowHint | Qt.Dialog

  property var window: null

  property int adsorbX: x
  property int adsorbY: y
  property int adsorbWidth: width
  property int adsorbHeight: height

  color: "transparent"

  Connections {
    target: window

    function onXChanged() {
      x = window.x
    }

    function onYChanged() {
      y = window.y
    }
  }

  property var itemList: []

  DockController {
    id: controller
    context: root
  }

  function getMaxWidth() {
    return root.width
  }

  function getMaxHeight() {
    return root.height - tab_bar.height
  }

  function append(item) {
    controller.appendItem(item)

//    item.width = root.width
//    item.height = root.height - tab_bar.height

//    if (item.maximum) {
//      item.x = root.x
//      item.y = root.y + tab_bar.height
//      item.visible = true
//    }
  }

  function remove(item) {
//    item.maximum = false
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
      model: controller.titleList

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

        onToggled: {
          if (toggled) {
            var item = controller.findItemByTitle(modelData)
            if (!item) { return }
            controller.focusItem(item)
//            item.active()
          }
        }

      }
    }
  }

  property var maximumArea: maximum_area
  Rectangle {
    id: maximum_area

    color: "transparent"

    anchors.top: tab_bar.bottom
    anchors.bottom: root.bottom
    anchors.left: root.left
    anchors.right: root.right
  }
}
