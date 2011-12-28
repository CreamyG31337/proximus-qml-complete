// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.1

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
        id: btnWipe
        text: "Clear Settings"
        anchors.centerIn: parent
        enabled: false
        onClicked: {
         objQSettings.clear()
         objProximusUtils.refreshRulesModel();
        }
        style: NegativeButtonStyle {}
    }
    Switch {
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: 70
        id: swIsItSafe
        checked: false
        onCheckedChanged: btnWipe.enabled = swIsItSafe.checked
    }
    Label{
        anchors.left: swIsItSafe.right
        anchors.verticalCenter: swIsItSafe.verticalCenter
        text: "I really want to delete all rules and settings"
        width: parent.width - swIsItSafe.width
        anchors.leftMargin: 10
    }

}
