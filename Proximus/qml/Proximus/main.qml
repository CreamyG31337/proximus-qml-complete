import QtQuick 1.1
import com.nokia.meego 1.0

PageStackWindow {
    id: appWindow
    initialPage: mainPage
    anchors.margins: UiConstants.DefaultMargin
    showStatusBar: true
    MainPage {
        id: mainPage
        //anchoring this does nothing
    }
    StatusBar{
        id: statusBar
    }

    Component.onCompleted: {
      //  tabGroup.currentTab = tabStatus;
        theme.inverted = objQSettings.getValue("/settings/THEME/inverted",true)
        //theme.color = 14//one day...
    }

    Rectangle {//thanks http://fiferboy.blogspot.com/2011/10/inactive-doesnt-have-to-be-boring.html
        id: overlayRect
        anchors.fill: parent
        color: "#60000000"
        visible: !platformWindow.active
        Label {
            anchors.centerIn: parent
            width: parent.width
            text: "Proximus"
            font.pixelSize: 70
            font.bold: true
            color: "white"
            horizontalAlignment: Text.AlignHCenter
        }
    }

}
