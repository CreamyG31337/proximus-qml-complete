// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import com.nokia.meego 1.0

Page{

    ToolBarLayout {
        id: noTools
        anchors {
            left: parent.left;
            right: parent.right;
            bottom: parent.bottom
        }
        ToolIcon{
            id: toolIcoBack
            iconId: "toolbar-back";
            onClicked: { pageStack.pop(); }
        }
    }
    tools: noTools
    Label{
        id: lblVersion
        text: "Proximus version 1.2.6"
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 20
    }
    Label{
        id: lblPublisher
        text: "created by Lance Colton"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 20
        anchors.top: lblVersion.bottom
    }
    Label{
        id: lblLinkProximus
        text: "latest version / more info"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 20
        anchors.top: lblPublisher.bottom
        font.underline: true
        color: "steelblue"
        MouseArea{
            anchors.fill: parent
            onClicked: Qt.openUrlExternally("http://talk.maemo.org/showthread.php?t=81174")
        }
    }
    Label{
        id: lblLinkConky
        text: "see another app I ported to the N9 -- Conky"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 20
        anchors.top: lblLinkProximus.bottom
        font.underline: true
        color: "steelblue"
        MouseArea{
            anchors.fill: parent
            onClicked: Qt.openUrlExternally("http://talk.maemo.org/showthread.php?t=81392")
        }
    }

}
