import QtQuick 1.1
import com.nokia.meego 1.0
Page{
    property int time // 1 or 2
    property int hours: 0
    property int minutes: 0

    Component.onCompleted: {
        timePicker.hours = hours
        timePicker.minutes = minutes
    }

    onHeightChanged: {//on rotate move text from behind time picker
        if ( test.height < test.width) {
            lblTime.horizontalAlignment = Text.AlignLeft
        }
        else {
            lblTime.horizontalAlignment = Text.AlignHCenter
        }
}
    Item {
        id: test
        anchors.fill: parent
        Label {
            anchors.top: parent.top
            id: lblTime
            height: 60
            width: parent.width
            font.pixelSize: 52
            text: ((timePicker.hours < 10 ? "0" : "") + timePicker.hours ) + ":" + ((timePicker.minutes < 10 ? "0" : "") + timePicker.minutes)
        }
        TimePickerBase {
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            id: timePicker
            backgroundImage: "../../images/clock.png"
            hourDotImage: "../../images/hour.png"
            minutesDotImage: "../../images/minute.png"
        }
        Button{
            id:btnSaveTime
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            width: 120
            text: "Save"
            onClicked: {

                if (time == 1)
                {
                    appWindow.pageStack.pop();
                    appWindow.pageStack.currentPage.setTime(1, ((timePicker.hours < 10 ? "0" : "") + timePicker.hours ) + ":" + ((timePicker.minutes < 10 ? "0" : "") + timePicker.minutes))
                }
                if (time == 2)
                {
                    appWindow.pageStack.pop();
                    appWindow.pageStack.currentPage.setTime(2, ((timePicker.hours < 10 ? "0" : "") + timePicker.hours ) + ":" + ((timePicker.minutes < 10 ? "0" : "") + timePicker.minutes))
                }
            }
        }
    }
}
