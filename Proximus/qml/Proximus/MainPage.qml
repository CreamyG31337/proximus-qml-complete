import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.1

Page {
    tools: commonTools
    anchors.fill: parent
    ToolBarLayout {
        id: commonTools
        anchors {
            left: parent.left;
            right: parent.right;
            bottom: parent.bottom
        }
        ToolIcon{
            id: toolIcoBack
            iconId: "toolbar-back";
            onClicked: { myMenu.close(); pageStack.pop(); }
            visible: false
        }
        ButtonRow {
            style: TabButtonStyle { }
            TabButton {
                tab: tabStatus
                text: "Status"
            }
            TabButton {
                tab: settingsPage
                text: "Settings"
            }
        }
        ToolIcon {
            platformIconId: "toolbar-view-menu"
            anchors.right: (parent === undefined) ? undefined : parent.right
            onClicked: (myMenu.status == DialogStatus.Closed) ? myMenu.open() : myMenu.close()
        }
    }
    TabGroup {
        id: tabGroup
        currentTab: tabStatus
        anchors.fill: parent
        // define the content for tab 1
        StatusPage {
            id: tabStatus
            anchors { fill: tabGroup;}
        }
        // define the content for tab 2
        SettingsPage {
            id: settingsPage
            anchors { fill: tabGroup;}
        }
    }
    Menu {
        id: myMenu
        visualParent: pageStack
        MenuLayout {
            MenuItem { text: qsTr("About Proximus"); onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))}
            MenuItem { text: qsTr("Send Feedback"); onClicked: pageStack.push(Qt.resolvedUrl("FeedbackPage.qml"))}
            MenuItem { text: qsTr("Clear Settings"); onClicked: pageStack.push(Qt.resolvedUrl("ResetPage.qml"))}
        }
    }
}
