import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0

Page{
    //tools: commonTools
    id: rulePage
    property string ruleName: "InvalidRuleName"
    anchors.topMargin: 5
    anchors.bottomMargin: 5
    anchors.leftMargin: 2
    anchors.rightMargin: 12

    function setCoord(latitude, longitude)
    {
        txtLocLatitude.text = latitude;
        txtLocLongitude.text = longitude;
    }

    function setTime(whichTime, timeToSet)
    {
        if (whichTime == 1)
        {
            btnTime1.text = timeToSet
        }
        if (whichTime == 2)
        {
            btnTime2.text = timeToSet
        }
    }

    function showError(errorString)
    {
        errorMessageBanner.text = errorString;
        errorMessageBanner.show();
    }

    Timer {
        id:sliderTimer
        interval: 3000; running: false; repeat: false
        onTriggered: radiusSlider.enabled = true
    }

    MapPage {
        id: mapPage
    }    

    InfoBanner{
        id: errorMessageBanner
        z: 99
    }
    ScrollDecorator {
        id: scrolldecorator
        flickableItem: flickable
    }

    Flickable{
    id: flickable
    anchors.fill: parent
    contentHeight: 1075
    //contentHeight: childrenRect.height
        TextField{
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            width: 300
            height: 45
            id: txtRuleName
            text:{
                if (ruleName != "InvalidRuleName"){
                    text = ruleName;
                }
                else
                    text = ""
            }
            placeholderText: "Enter Name For This Rule"
            font.pixelSize: 26
            validator: RegExpValidator{regExp: /(\w+ *)+/}
            maximumLength: 64
        }

////////////////////////ACTIONS
        Rectangle{
            id: recActions
            color: "transparent"
            height: 175
            width: parent.width
            anchors.top: txtRuleName.bottom
            border.width: 2
            border.color: "steelblue"
            radius: 10
            anchors.margins: 5

            Label{
                id: lblActionHeader
                text: "Actions"
                font.italic: true
                font.bold: true
                anchors.top: parent.top //txtRuleName.bottom
                anchors.margins: 5
                anchors.left: parent.left
            }
            Switch{
                anchors.top: lblActionHeader.bottom
                anchors.margins: 5
                anchors.left: parent.left
                id: swChangeProfile
                checked: objQSettings.getValue("/rules/" + ruleName + "/Actions/Profile/enabled",false)
            }
            Label{
                anchors.margins: 7
                id: lblSwProfile
                anchors.verticalCenter: swChangeProfile.verticalCenter
                anchors.left: swChangeProfile.right
                height: swUseLocation.height
                verticalAlignment: Text.AlignVCenter
                text: "Switch Profile "
            }
            TumblerButton12{
                id: btnChooseProfile
                anchors.left: lblSwProfile.right
                anchors.verticalCenter: lblSwProfile.verticalCenter
                text: objQSettings.getValue("/rules/" + ruleName + "/Actions/Profile/NAME","choose")
                onClicked: tDialog.open();
                enabled: swChangeProfile.checked
            }
            TumblerDialog12{
                id: tDialog
                titleText: "Select Profile"
                columns: [ profileColumn ]
                acceptButtonText: "Ok"
                onAccepted: btnChooseProfile.text = profileColumn.items[profileColumn.selectedIndex]; //thanks for depricating label...
            }
            TumblerColumn12{
                id: profileColumn
                items: objProfileClient.profileTypes()
            }
            Switch{
                anchors.top: swChangeProfile.bottom
                anchors.margins: 5
                anchors.left: parent.left
                id: swCreateReminder
                checked: objQSettings.getValue("/rules/" + ruleName + "/Actions/Reminder/enabled",false)
            }
        /////////////CREATE REMINDER
            Label{
                anchors.margins: 7
                anchors.top: swChangeProfile.bottom
                anchors.left: swCreateReminder.right
                height: swCreateReminder.height
                verticalAlignment: Text.AlignVCenter
                text: "Create Reminder "
            }
            TextField{
                id: txtReminderText
                width: rulePage.width - 20
                height: 45
                anchors.top: swCreateReminder.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                text: objQSettings.getValue("/rules/" + ruleName + "/Actions/Reminder/TEXT","")
                placeholderText: "enter reminder message"
                font.pixelSize: 26
                validator: RegExpValidator{regExp: /(\w+ *)+/}
                maximumLength: 255
                enabled: swCreateReminder.checked
            }
        }
//////////////////RULES
        Label{
            id: lblActivationHeader
            text: "Trigger When"
            font.italic: true
            font.bold: true
            anchors.top: recActions.bottom//txtReminderText.bottom
            anchors.margins: 5
        }
//////////////////LOCATION
        Rectangle{
            id: recLocation
            color: "transparent"
            height: 220
            width: parent.width
            anchors.top: lblActivationHeader.bottom
            border.width: 2
            border.color: "steelblue"
            radius: 10
            anchors.margins: 5
            Switch{
                anchors.top: parent.top
                anchors.margins: 5
                anchors.left: parent.left
                id: swUseLocation
                checked: objQSettings.getValue("/rules/" + ruleName + "/Location/enabled",false)
            }
            Label{
                anchors.top: parent.top
                anchors.left: swUseLocation.right
                anchors.margins: 5
                height: swUseLocation.height
                verticalAlignment: Text.AlignVCenter
                text: "Location match"
            }
            Label{
                anchors.top: parent.top
                anchors.right: swUseLocationNot.left
                anchors.margins: 5
                height: swUseLocation.height
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignRight
                text: "NOT"
            }
            Switch{
                id: swUseLocationNot
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.margins: 5
                checked: objQSettings.getValue("/rules/" + ruleName + "/Location/NOT",false)
                enabled: swUseLocation.checked
            }

            Label {
                anchors.top: swUseLocation.bottom
                anchors.left: parent.left
                anchors.margins: 5
                id: lblLocationRadius
                height: 45
                verticalAlignment: Text.AlignVCenter
                text: "Radius (m):"
            }
            CountBubble{
                id: cbRadius
                anchors.verticalCenter: lblLocationRadius.verticalCenter
                anchors.left: lblLocationRadius.right
                anchors.margins: 5
                value: radiusSlider.value
                largeSized: true
                onValueChanged: {
                    if (cbRadius.value == radiusSlider.maximumValue && radiusSlider.pressed){
                        radiusSlider.enabled = false
                        showError("Increasing max range...\nClick somewhere in the middle if you want to adjust it further")
                        radiusSlider.maximumValue = radiusSlider.maximumValue * 10
                        radiusSlider.stepSize = radiusSlider.stepSize * 10
                        sliderTimer.start()

                    }
                    if (cbRadius.value == 0 && radiusSlider.stepSize >= 10 && radiusSlider.pressed){ //don't need fractions
                        radiusSlider.enabled = false
                        showError("Decreasing max range...\nClick somewhere in the middle if you want to adjust it further")
                        radiusSlider.maximumValue = radiusSlider.maximumValue / 10
                        radiusSlider.stepSize = radiusSlider.stepSize / 10
                        sliderTimer.start()
                    }
                }
            }
            Slider{
                id: radiusSlider
                width: parent.width - lblLocationRadius.width - cbRadius.width
                anchors.verticalCenter: lblLocationRadius.verticalCenter
                anchors.left: cbRadius.right
                anchors.margins: 5
                value: objQSettings.getValue("/rules/" + ruleName + "/Location/RADIUS",100)
                stepSize: 10
                maximumValue: 2000
                enabled: swUseLocation.checked
                Component.onCompleted: {
                    if (value > maximumValue)
                    {
                        while (value > maximumValue)
                        {
                            maximumValue = maximumValue * 10
                            stepSize = stepSize * 10
                        }
                    }
                }
            }
            Label {
                height: 45
                verticalAlignment: Text.AlignVCenter
                anchors.top: radiusSlider.bottom
                anchors.margins: 5
                anchors.left: parent.left
                id: lblLocationLatitude
                text: "Latitude: "
            }

            TextField{
                id: txtLocLatitude
                anchors.top: radiusSlider.bottom
                anchors.left: txtLocLongitude.left
                anchors.margins: 5
                width: 180
                height: 45
                text: objQSettings.getValue("/rules/" + ruleName + "/Location/LATITUDE",0)
                font.pixelSize: 21
                validator: DoubleValidator{}
                maximumLength: 13
                placeholderText: "click this >>"
                enabled: swUseLocation.checked
                inputMethodHints: Qt.ImhFormattedNumbersOnly
            }
            Label {
                anchors.top: txtLocLatitude.bottom
                anchors.margins: 5
                anchors.left: parent.left
                height: 45
                verticalAlignment: Text.AlignVCenter
                id: lblLocationLongitude
                text: "Longitude: "
            }
            TextField{
                id: txtLocLongitude
                anchors.top: txtLocLatitude.bottom
                anchors.left: lblLocationLongitude.right
                anchors.margins: 5
                width: 180
                height: 45
                text: objQSettings.getValue("/rules/" + ruleName + "/Location/LONGITUDE",0)
                font.pixelSize: 21
                validator: DoubleValidator{}
                maximumLength: 13
                placeholderText: "click this >>"
                enabled: swUseLocation.checked
                inputMethodHints: Qt.ImhFormattedNumbersOnly
            }

            Button{
                id: btnFillFromMap
                anchors {
                    right: parent.right
                    top: txtLocLatitude.top
                    margins: 5
                }
                width: 150
                height: 45*2
                text: qsTr("Fill From\nMap")
                //onClicked: appWindow.pageStack.push(mapPage)
                //doing this way everywhere seems dumb because everything is created too early,
                //gps activates when u launch the app. but it still does that.
                onClicked: {
                    if (cbRadius.value < 10) cbRadius.value = 10;//not much point trying to choose a 0 radius
                    appWindow.pageStack.push(Qt.resolvedUrl("MapPage.qml"),
                                             {longitudeReq: txtLocLongitude.text, latitudeReq: txtLocLatitude.text,
                                                 radiusSize: cbRadius.value} );
                }
                style: PositiveButtonStyle {}
                enabled: swUseLocation.checked
            }
        }
        /////////////////////////CALENDAR
        Rectangle{
            id: recCalendar
            color: "transparent"
            height: 110
            width: parent.width
            anchors.top: recLocation.bottom
            border.width: 2
            border.color: "steelblue"
            radius: 10
            anchors.margins: 5
            Switch{
                id: swUseCalendar
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.margins: 5
                checked: objQSettings.getValue("/rules/" + ruleName + "/Calendar/enabled",false)
            }
            Label{
                anchors.top: parent.top
                anchors.left: swUseCalendar.right
                anchors.margins: 5
                height: swUseCalendar.height
                verticalAlignment: Text.AlignVCenter
                text: "Calendar keyword match"
                font.pixelSize: 20
            }
            Label{
                height: swUseCalendar.height
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignRight
                text: "NOT"
                anchors.right: swUseCalendarNOT.left
                anchors.top: parent.top
                anchors.margins: 5
            }
            Switch{
                id: swUseCalendarNOT
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.margins: 5
                checked: objQSettings.getValue("/rules/" + ruleName + "/Calendar/NOT",false)
                enabled: swUseCalendar.checked
            }
            TextField{
                id: txtCalendarKeywords
                width: parent.width - 20
                height: 45
                anchors.top: swUseCalendarNOT.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.margins: 5
                text: objQSettings.getValue("/rules/" + ruleName + "/Calendar/KEYWORDS","")
                placeholderText: "enter keywords seperated by spaces"
                font.pixelSize: 26
                validator: RegExpValidator{regExp: /(\w+ *)+/}
                maximumLength: 255
                enabled: swUseCalendar.checked
            }
        }
        ////////////////Days of week
        Rectangle{
            id: recDaysOfWeek
            color: "transparent"
            height: 175
            width: parent.width
            anchors.top: recCalendar.bottom
            border.width: 2
            border.color: "steelblue"
            radius: 10
            anchors.margins: 5
            Switch{
                id: swDaysOfWeek
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.margins: 5
                checked: objQSettings.getValue("/rules/" + ruleName + "/DaysOfWeek/enabled",false)
            }
            Label{
                anchors.top: parent.top
                anchors.left: swDaysOfWeek.right
                anchors.margins: 5
                height: swDaysOfWeekNOT.height
                verticalAlignment: Text.AlignVCenter
                text: "On these days"
            }
            Label{
                anchors.top: parent.top
                anchors.right: swDaysOfWeekNOT.left
                anchors.margins: 5
                height: swDaysOfWeekNOT.height
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignRight
                text: "NOT"
            }
            Switch{
                id: swDaysOfWeekNOT
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.margins: 5
                checked: objQSettings.getValue("/rules/" + ruleName + "/DaysOfWeek/NOT",false)
                enabled: swDaysOfWeek.checked
            }
            Button{
                id: btnSelectDausOfWeek
                anchors.top: swDaysOfWeek.bottom
                anchors.margins: 5
                anchors.horizontalCenter: parent.horizontalCenter
                text:"Choose Days"
                onClicked: daysOfWeekSelectionDialog.open();
            }
            MultiSelectionDialog {
                id: daysOfWeekSelectionDialog
                titleText: "Choose Days"
                acceptButtonText: "OK"
                selectedIndexes: objQSettings.getValue("/rules/" + ruleName + "/DaysOfWeek/INDEXES",[]);
                model: ListModel{
                    ListElement { name: "Sunday" }
                    ListElement { name: "Monday" }
                    ListElement { name: "Tuesday" }
                    ListElement { name: "Wednesday" }
                    ListElement { name: "Thursday" }
                    ListElement { name: "Friday" }
                    ListElement { name: "Saturday" }
                }
            }
            Grid{
                id: daysOfWeekGrid
                anchors.top: btnSelectDausOfWeek.bottom
                anchors.margins: 5
                anchors.left: parent.left
                    columns: screen.orientation == Screen.Landscape || screen.orientation == Screen.LandscapeInverted ? 8 : 4
                Component{
                    id: name_delegate
                    Rectangle{
                        width: 100
                        height: 30
                        color: "lightgray"
                        Label{
                            anchors.centerIn: parent
                            text: daysOfWeekSelectionDialog.model.get(daysOfWeekSelectionDialog.selectedIndexes[index]).name
                            font.pixelSize: 15
                            font.bold: true
                        }
                    }
                }
                Rectangle {
                    width: 100
                    height: 30
                    color: "white"
                    Label{
                        y: 10
                        anchors.centerIn: parent
                        text: "Selected:"
                        font.pixelSize: 15
                        font.bold: true
                        color: "black"
                    }
                }
                Repeater{
                    model: daysOfWeekSelectionDialog.selectedIndexes
                    delegate: name_delegate
                }
            }
        }

        //////////////////////////TIMES
        Rectangle{
            id: recTimes
            color: "transparent"
            height: 113
            width: parent.width
            anchors.top: recDaysOfWeek.bottom
            border.width: 2
            border.color: "steelblue"
            radius: 10
            anchors.margins: 5
            Switch{
                id: swUseTime
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.margins: 5
                checked: objQSettings.getValue("/rules/" + ruleName + "/Time/enabled",false)
            }
            Label{
                anchors.top: parent.top
                anchors.left: swUseTime.right
                anchors.margins: 5
                height: swUseTimeNot.height
                verticalAlignment: Text.AlignVCenter
                text: "Between these times"
            }
            Label{
                anchors.top: parent.top
                anchors.right: swUseTimeNot.left
                anchors.margins: 5
                height: swUseTimeNot.height
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignRight
                text: "NOT"
            }
            Switch{
                id: swUseTimeNot
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.margins: 5
                checked: objQSettings.getValue("/rules/" + ruleName + "/Time/NOT",false)
                enabled: swUseTime.checked
            }
            TumblerButton{
                id: btnTime1
                anchors {
                    left: parent.left
                    top: swUseTimeNot.bottom
                    margins: 5
                }
                width: 225
                text: objQSettings.getValue("/rules/" + ruleName + "/Time/TIME1","?").slice(0,5)
                onClicked: {
                    var _hours = 0
                    var _minutes = 0
                    if (btnTime1.text != "?"){
                        var time = btnTime1.text.match(/(\d\d):(\d\d)/);
                        _hours = (time[1]); // parseint doesn't like eights and nines?? the hell?
                        _minutes = (time[2]);
                    }
                    appWindow.pageStack.push(Qt.resolvedUrl("TimePicker.qml"),
                      {time: 1, hours: _hours , minutes: _minutes } );

                }
                style: TumblerButtonStyle {}
                enabled: swUseTime.checked
            }
            TumblerButton{
                id: btnTime2
                anchors {
                    left: btnTime1.right
                    top: swUseTimeNot.bottom
                    margins: 5
                }
                width: 225
                text: objQSettings.getValue("/rules/" + ruleName + "/Time/TIME2","?").slice(0,5)
                onClicked: {
                    var _hours = 0
                    var _minutes = 0
                    if (btnTime2.text != "?"){
                        var time = btnTime2.text.match(/(\d\d):(\d\d)/);
                        _hours = (time[1]);
                        _minutes = (time[2]);
                    }
                    appWindow.pageStack.push(Qt.resolvedUrl("TimePicker.qml"),
                      {time: 2, hours: _hours , minutes: _minutes } );

                }
                enabled: swUseTime.checked
            }
        }

        /////////////////////NEAR WIFI
        Rectangle{
            id: recWiFi
            color: "transparent"
            height: 110
            width: parent.width
            anchors.top: recTimes.bottom
            border.width: 2
            border.color: "steelblue"
            radius: 10
            anchors.margins: 5
            Switch{
                id: swUseWIFI
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.margins: 5
                checked: objQSettings.getValue("/rules/" + ruleName + "/WiFi/enabled",false)
            }
            Label{
                anchors.top: parent.top
                anchors.left: swUseWIFI.right
                anchors.margins: 5
                height: swUseWIFI.height
                verticalAlignment: Text.AlignVCenter
                text: "Near any of these WiFi"
            }
            Label{
                anchors.top: parent.top
                anchors.right: swUseWIFINot.left
                anchors.margins: 5
                height: swUseTimeNot.height
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignRight
                text: "NOT"
            }
            Switch{
                id: swUseWIFINot
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.margins: 5
                checked: objQSettings.getValue("/rules/" + ruleName + "/WiFi/NOT",false)
                enabled: swUseWIFI.checked
            }
            TextField{
                id: txtWiFiNetworks
                width: parent.width - 20
                height: 45
                anchors.top: swUseWIFI.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.margins: 5
                text: objQSettings.getValue("/rules/" + ruleName + "/WiFi/SSIDs","")
                placeholderText: "enter SSIDs seperated by spaces"
                font.pixelSize: 26
                validator: RegExpValidator{regExp: /(\w+ *)+/}
                maximumLength: 255
                enabled: swUseWIFI.checked
            }
        }

        //////////////////////////SAVE  OR CANCEL

        Button{
            id: btnCancel
            anchors {
                right: parent.right
                bottom: parent.bottom
                margins: 5
            }
            width: 150
            text: qsTr("Cancel")
            onClicked: {
                appWindow.pageStack.pop()
            }
        }
        Button{
            id: btnSave
            anchors {
                right: btnCancel.left
                bottom: parent.bottom
                margins: 5
            }
            width: 150
            text: qsTr("Save")
            onClicked: {
                var errorString = "";                
                if (!txtRuleName.acceptableInput){
                    errorString += "Rule name is blank or not valid\n"
                }
                if (swChangeProfile.checked && btnChooseProfile.text == "choose") {
                    errorString += "Choose a profile to switch to\n"
                }
                if (swCreateReminder.checked && !txtReminderText.acceptableInput) {
                    errorString += "Enter a short message to display for the reminder or disable that section\n"
                }
                if (txtCalendarKeywords.enabled && !txtCalendarKeywords.acceptableInput){
                    errorString += "Enter calendar keywords or disable that section\n"
                }
                if (btnTime1.enabled && (btnTime1.text == "?" || btnTime2.text == "?")){
                    errorString += "Set valid times for both Time 1 and Time 2 or disable that section\n"
                }
                if (btnTime1.enabled && (btnTime1.text == btnTime2.text)){
                    errorString += "Time1 and Time2 must be different if time checking is enabled\n"
                }
                if (txtLocLatitude.enabled && (!txtLocLatitude.acceptableInput || !txtLocLongitude.acceptableInput )){
                    errorString += "Use the fill from map button, manually enter valid coordinates, or disable that section\n"
                }
                if (swUseWIFI.checked && !txtWiFiNetworks.acceptableInput){
                    errorString += "Enter at least one valid SSID or disable that section\n"
                }
                if(swDaysOfWeek.checked && !daysOfWeekSelectionDialog.selectedIndexes.length > 0) {
                    errorString += "Choose at least one day of the week or disable that section\n"
                }

                if (errorString.length > 1){
                    showError(errorString);
                    return;
                }

                //check, if about to replace, was old rule enabled or not? default to enabled if not replace
                if (objQSettings.getValue("/rules/" + ruleName + "/enabled",true))
                    objQSettings.setValue("/rules/" + txtRuleName.text +"/enabled",true)
                else
                    objQSettings.setValue("/rules/" + txtRuleName.text +"/enabled",false)

                //check if name changed and remove old one if so
                if (ruleName != txtRuleName.text)
                {
                    objQSettings.remove("/rules/" + ruleName)
                    console.log("replacing " + ruleName + " with " + txtRuleName.text)
                    //deselect rule on List

                }

                objQSettings.setValue("/rules/" + txtRuleName.text + "/Actions/Profile/enabled",swChangeProfile.checked);
                objQSettings.setValue("/rules/" + txtRuleName.text + "/Actions/Profile/NAME", btnChooseProfile.text);

                objQSettings.setValue("/rules/" + txtRuleName.text + "/Actions/Reminder/enabled",swCreateReminder.checked);
                if (swCreateReminder.checked){
                    objQSettings.setValue("/rules/" + txtRuleName.text + "/Actions/Reminder/TEXT",txtReminderText.text);
                }

                objQSettings.setValue("/rules/" + txtRuleName.text + "/Location/enabled",swUseLocation.checked);
                if (swUseLocation.checked){
                    objQSettings.setValue("/rules/" + txtRuleName.text + "/Location/NOT",swUseLocationNot.checked);
                    objQSettings.setValue("/rules/" + txtRuleName.text + "/Location/RADIUS",cbRadius.value);
                    objQSettings.setValue("/rules/" + txtRuleName.text + "/Location/LATITUDE",txtLocLatitude.text);
                    objQSettings.setValue("/rules/" + txtRuleName.text + "/Location/LONGITUDE",txtLocLongitude.text);
                }

                objQSettings.setValue("/rules/" + txtRuleName.text + "/Time/enabled",swUseTime.checked);
                if (swUseTime.checked){
                    objQSettings.setValue("/rules/" + txtRuleName.text + "/Time/NOT",swUseTimeNot.checked);
                    objQSettings.setValue("/rules/" + txtRuleName.text + "/Time/TIME1",btnTime1.text + ":00");
                    objQSettings.setValue("/rules/" + txtRuleName.text + "/Time/TIME2",btnTime2.text + ":00");
                }

                objQSettings.setValue("/rules/" + txtRuleName.text + "/Calendar/enabled",swUseCalendar.checked);
                if (swUseCalendar.checked){
                    objQSettings.setValue("/rules/" + txtRuleName.text + "/Calendar/NOT",swUseCalendarNOT.checked);
                    objQSettings.setValue("/rules/" + txtRuleName.text + "/Calendar/KEYWORDS",txtCalendarKeywords.text);
                }

                objQSettings.setValue("/rules/" + txtRuleName.text + "/Calendar/enabled",swUseCalendar.checked);
                if (swUseCalendar.checked){
                    objQSettings.setValue("/rules/" + txtRuleName.text + "/Calendar/NOT",swUseCalendarNOT.checked);
                    objQSettings.setValue("/rules/" + txtRuleName.text + "/Calendar/KEYWORDS",txtCalendarKeywords.text);
                }

                objQSettings.setValue("/rules/" + txtRuleName.text + "/DaysOfWeek/enabled",swDaysOfWeek.checked);
                if (swDaysOfWeek.checked) {
                    objQSettings.setValue("/rules/" + txtRuleName.text + "/DaysOfWeek/NOT",swDaysOfWeekNOT.checked);
                    objQSettings.setValue("/rules/" + txtRuleName.text + "/DaysOfWeek/INDEXES",daysOfWeekSelectionDialog.selectedIndexes);
                    console.log("stored dayofweek indexes: " + daysOfWeekSelectionDialog.selectedIndexes);
                }

                objQSettings.setValue("/rules/" + txtRuleName.text + "/WiFi/enabled",swUseWIFI.checked);
                if (swUseWIFI.checked){
                    objQSettings.setValue("/rules/" + txtRuleName.text + "/WiFi/NOT",swUseWIFINot.checked);
                    objQSettings.setValue("/rules/" + txtRuleName.text + "/WiFi/SSIDs",txtWiFiNetworks.text);
                }

                objProximusUtils.refreshRulesModel(); //why can't i call the function in settingsPage now??
                appWindow.pageStack.pop()
            }
        }
    }
}
