import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0

Page {
    id: tabStatus
    tools:     commonTools
    TextArea {
        id: txtLog
        text: qsTr("Welcome to Proximus")
        anchors {top: parent.top; left: parent.left; right: parent.right;}
        readOnly: true;
    }
    Component.onCompleted: {
        txtLog.text += "\n" + objProximusUtils.isServiceRunning();
        txtLog.text += "\nSettings read from " + objQSettings.fileName();
        txtLog.text += "\n\nNokia's non-satellite positioning methods may be bugged. Please see bug #641 at harmattan-bugs.nokia.com for more info. I added a new option to disable positioning completely for now.";
    }

}
