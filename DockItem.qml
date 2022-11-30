import QtQuick 2.13
import QtQuick.Window 2.13

import "./"

Window {
  id: root

  flags: Qt.FramelessWindowHint | Qt.Dialog

  property var context: null
  property var controller: context.controller

  property bool adsorbed: false
  readonly property alias titleHeight: title.height

  Connections {
    target: context
    enabled: adsorbed

    onAdsorbedXChanged: {
      x = context.adsorbedX
    }

    onAdsorbedYChanged: {
      y = context.adsorbedY
    }

    onAdsorbedWidthChanged: {
      width = context.adsorbedWidth
    }

    onAdsorbedHeightChanged: {
      height = context.adsorbedHeight
    }
  }

  QtObject {
    id: cache
    property int x: 0
    property int y: 0
    property int width: 0
    property int height: 0
  }


  onAdsorbedChanged: {
    visible = true

    if (adsorbed) {
      cache.x = root.x
      cache.y = root.y
      cache.width = root.width
      cache.height = root.height

      root.x = context.adsorbedX
      root.y = context.adsorbedY
      root.width = context.adsorbedWidth
      root.height = context.adsorbedHeight

    } else {
      root.x = cache.x
      root.y = cache.y
      root.width = cache.width
      root.height = cache.height

      root.raise()
    }

    controller.syncContext()
  }

  Rectangle {
    id: title

    x: 0
    y: 0
    width: root.width
    height: root.adsorbed ? 0 : 40

    color: "lightcoral"

    Text {
      anchors.centerIn: title
      text: root.title
    }

    MouseArea {
      anchors.fill: title

      onDoubleClicked: {
        root.adsorbed = !root.adsorbed
      }
    }
  }

  Component.onCompleted: {
    context.append(root)
    raise()
  }
}
