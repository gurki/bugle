import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.3
import QtQuick.Layouts 1.3


ApplicationWindow {
    id: window
    title: "Message Center"
    width: 640
    height: 480
    visible: true
    color: "#2e2f30"

    RowLayout {
        anchors.fill: parent

        ListView {
            id: listview
            property bool following: true

            Layout.fillWidth: true
            Layout.fillHeight: true
            anchors.margins: 7
            model: messageListProxy
            cacheBuffer: window.height * 8

            delegate: Item {
                width: parent.width
                height: childrenRect.height

                Text {
                    id: textItem
                    textFormat: Text.RichText
                    font.pixelSize: 14
                    font.family: "Consolas"
                    text: prettySummary
                }
            }
            onCountChanged: {
                positionViewAtEnd()
            }

            ScrollBar.vertical: ScrollBar {
                parent: listview.parent
                anchors.top: listview.top
                anchors.left: listview.right
                anchors.bottom: listview.bottom
            }
        }

        ColumnLayout {
            id: rightlayout
            Layout.margins: 7
            Layout.fillHeight: true
            spacing: 3

            Label {
                text: messageList.size
                color: "white"
            }
            TextArea {
                font.pixelSize: 14
                color: "white"
                placeholderText: "tag1, !tag2 tag3>0"
                onTextChanged: messageListProxy.tagFilter = text
            }
            Button {
                width: 128
                text: "info"
                onClicked: messageCenter.postq( "fyi, you pressed a button!", "info" )
            }
            Button {
                width: 128
                text: "debug"
                onClicked: messageCenter.postq( "golibug golibug", "debug" )
            }
            Item {
                Layout.fillHeight: true
            }
        }
    }

    Timer {
        interval: 1500
        onTriggered: messageCenter.postq( "drone dead, time for weekend", [ "drone", "status" ] )
        running: true
        repeat: true
    }
    Timer {
        interval: 2000
        onTriggered: messageCenter.postq( Math.random() * 10,
            ["info","debug","warning","error","success"][(Math.random() * 5).toFixed()]
        )
        running: true
        repeat: true
    }
    Timer {
        interval: 1000
        onTriggered: messageCenter.postq( "radio debug message", [
            "radio",
            "debug",
            {"priority":(Math.random()*4).toFixed()}
        ]);
        running: true
        repeat: true
    }

}
