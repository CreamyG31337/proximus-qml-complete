// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import com.nokia.meego 1.0
//import com.nokia.extras 1.1

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

    Button{
     id: btnSendEmail
     onClicked: Qt.openUrlExternally("mailto:proximus@appcheck.net?subject=Proximus%20Feedback")
     text: "Launch Mail"
     anchors.centerIn: parent
    }

    Label{
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: 50
        text: "You can send your feedback via email.\n Click the button above."
        horizontalAlignment: Text.AlignHCenter
    }
}
