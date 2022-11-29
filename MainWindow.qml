import QtQuick
import QtQuick.Layouts

import "./"

Rectangle {
  id: main_window
  color: "lightgray"

  property var window: null

  DockContext {
    id: dock_context
    x: window.mapToGlobal(0,0).x
    y: window.mapToGlobal(0,0).y
    width: main_window.width
    height: main_window.height
    visible: true

    window : main_window.window

    Component.onCompleted: {
      Global.dockContext = dock_context
    }
  }

  DockItem {
    id: dock_item_1
    title: "item_1"
    Component.onCompleted: {
      dock_context.append(this)
    }
  }

  DockItem {
    id: dock_item_2
    title: "item_2"
    Component.onCompleted: {
      dock_context.append(this)
    }
  }

  DockItem {
    id: dock_item_3
    title: "item_3"
    Component.onCompleted: {
      dock_context.append(this)
    }
  }

//  ColumnLayout {
//    id: main_col
//    spacing: 10
//    anchors.fill: parent

//    RowLayout {
//      id: control_row
//      Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
//      Layout.fillWidth: true
//      height: 40
//      spacing: 10

//      BaseButton {
//        id: append_button
//        Layout.fillWidth: true

//        text: Tr.base_append

//        onClicked: {
//          list_model.append({
//            button_name_: list_view.count + 1
//          })
//        }
//      }

//      BaseButton {
//        id: remove_button
//        Layout.fillWidth: true

//        text: Tr.ex_play

//        onClicked: {
//          list_model.remove(list_view.count - 1)
//        }
//      }

//      Repeater {
////        model: [
////          { "i18n": "en_US", },
////          { "i18n": "en_UK", },
////          { "i18n": "zh_CN", },
////          { "i18n": "zh_TW", },
////        ]

//        model: kernal.languageManager.languages

//        delegate: BaseButton {
//          Layout.fillWidth: true

//          text: modelData.source + " - " + modelData.i18n
//          onClicked: {
//            kernal.languageManager.currentLanguage = modelData.i18n
//          }
//        }
//      }
//    }

//    ListView {
//      id: list_view
//      Layout.alignment: Qt.AlignBottom | Qt.AlignHCenter
//      Layout.fillWidth: true
//      Layout.fillHeight: true
//      spacing: 10

//      model: ListModel {
//        id: list_model
//      }

//      delegate: BaseButton {
//        width: control_row.width
//        text: button_name_ + " of " + list_model.count + " " + Tr.ex_playex
//      }
//    }
//  }







//  Rectangle {
//    id: top_bar

//    anchors {
//      top: main_window.top
//      left: main_window.left
//      right: main_window.right
//      topMargin: 0
//      leftMargin: 0
//      rightMargin: 0
//    }

//    height: 38

//    Text {
//      id: text
//      text: Tr.base_append
//      anchors.fill: parent
//    }

//    color: "red"
//  }

//  Rectangle {
//    id: bottom_bar

//    anchors {
//      bottom: main_window.bottom
//      left: main_window.left
//      right: main_window.right
//      topMargin: 0
//      leftMargin: 0
//      rightMargin: 0
//    }

//    height: 38

//    color: "yellow"
//    visible: false
//  }

//  Rectangle {
//    id: left_bar

//    anchors {
//      top: main_window.top
//      bottom: main_window.bottom
//      left: main_window.left
//      topMargin: 0
//      bottomMargin: 0
//      leftMargin: 0
//    }

//    width: 38

//    color: "green"
//    visible: false
//  }

//  Rectangle {
//    id: right_bar

//    anchors {
//      top: main_window.top
//      bottom: main_window.bottom
//      right: main_window.right
//      topMargin: 0
//      bottomMargin: 0
//      rightMargin: 0
//    }

//    width: 38

//    color: "blue"
//    visible: false
//  }

//  Rectangle {
//    id: center_panel

//    anchors {
//      top: top_bar.bottom
//      bottom: bottom_bar.top
//      left: left_bar.right
//      right: right_bar.left
//      margins: 2
//    }

//    TextEdit {
//      anchors.fill: parent
//      anchors.margins: 10
//    }
//  }
}


