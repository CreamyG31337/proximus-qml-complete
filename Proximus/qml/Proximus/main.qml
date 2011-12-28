import QtQuick 1.1
import com.nokia.meego 1.0

PageStackWindow {
    id: appWindow
    initialPage: mainPage
    MainPage {
        id: mainPage
        //anchoring this does nothing
    }
    StatusBar{
        id: statusBar
    }

    Component.onCompleted: {
      //  tabGroup.currentTab = tabStatus;
    }



}
