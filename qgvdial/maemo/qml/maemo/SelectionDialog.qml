/*
qgvdial is a cross platform Google Voice Dialer
Copyright (C) 2009-2014  Yuvraaj Kelkar

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

Contact: yuvraaj@gmail.com
*/

import Qt 4.7

Rectangle {
    id: container

    property string selected: "All"

    property real delWidth: 5
    property real delHeight: 40

    signal done(bool accepted)

    visible: false
    color: "black"

    ListView {
        id: listView

        anchors {
            top: parent.top
            topMargin: 10
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
        }

        clip: true
        spacing: 2
        width: container.delWidth + 10

        model: ListModel {
            ListElement { name: "All" }
            ListElement { name: "Placed" }
            ListElement { name: "Missed" }
            ListElement { name: "Received" }
            ListElement { name: "Voicemail" }
            ListElement { name: "SMS" }
        }

        delegate: Rectangle {
            width: container.delWidth + 2
            height: container.delHeight + 2

            color: "transparent"
            border.color: name.toUpperCase() === container.selected.toUpperCase() ? "orange" : "transparent"

            Text {
                id: textDelegate

                anchors.centerIn: parent

                text: name
                color: "white"

                onPaintedWidthChanged: {
                    if (paintedWidth + 10 > container.delWidth) {
                        container.delWidth = paintedWidth + 10;
                        //console.debug("Updated delW = " + container.delWidth + " because of " + text);
                    }
                }

                onPaintedHeightChanged: {
                    if (paintedHeight + 10 > container.delHeight) {
                        container.delHeight = paintedHeight + 10;
                        //console.debug("Updated delH = " + container.delHeight + " because of " + text);
                    }
                }
            }//TextOneLine

            MouseArea {
                id: mouseArea

                anchors.fill: parent

                onClicked: {
                    container.selected = name;
                    container.done(true);
                }
            }//MouseArea
        }//Rectangle
    }//ListView
}//Item
