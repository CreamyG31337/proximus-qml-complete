import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0
import net.appcheck.Proximus 1.0

Page {
    id: tabStatus
    tools: commonTools
    Flickable{
        anchors.fill: parent
        contentHeight: txtLog.height
        //contentWidth: childrenRect.width
        TextArea {
            //platformInverted doesn't work.

            id: txtLog
            text: qsTr("Welcome to Proximus")
            anchors {top: parent.top; left: parent.left; right: parent.right;}
            readOnly: true;           
            Connections {
                target: objProximusLog
                onNewLogInfo: {
                    if (txtLog.text.length > 4000){
                        txtLog.text = "(...log truncated)\n" + txtLog.text.slice(-2000);
                    }
                    txtLog.text += "\n" + Qt.formatTime(new Date(),"hh:mm:ss") + ": " + LogText;
                }
            }
        }
    }
    Component.onCompleted: {
        txtLog.text += "\n" + objProximusUtils.isServiceRunning();
        txtLog.text += "\nSettings read from " + objQSettings.fileName();
        txtLog.text += "\n\nNokia's positioning methods may use too much resources, especially when GPS is disabled. Please see bug #641 at harmattan-bugs.nokia.com for more info. There's an option to disable positioning completely for now.";
    }
}
