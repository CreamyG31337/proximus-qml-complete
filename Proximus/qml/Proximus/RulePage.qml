import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.1

Page{
    //tools: commonTools
    id: rulePage
    property string ruleName: "InvalidRuleName"
    anchors.margins: 7

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

    Flickable{
    anchors.fill: parent
    contentHeight: 600
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
        Label{
            id: lblActionHeader
            text: "Action to take"
            anchors.top:  txtRuleName.bottom
            height: 20
            visible: false
        }
        Switch{
            anchors.top: lblActionHeader.bottom
            id: swChangeProfile
            checked: objQSettings.getValue("/rules/" + ruleName + "/Actions/Profile/enabled",true)
        }
        Label{
            anchors.margins: 7
            id: lblSwProfile
            anchors.top: lblActionHeader.bottom
            anchors.left: swChangeProfile.right
            height: swUseLocation.height
            verticalAlignment: Text.AlignVCenter
            text: "Switch Profile "
        }
        TumblerButton{
            id: btnChooseProfile
            anchors.left: lblSwProfile.right
            anchors.verticalCenter: lblSwProfile.verticalCenter
            text: objQSettings.getValue("/rules/" + ruleName + "/Actions/Profile/NAME","choose")
            onClicked: tDialog.open();
            enabled: swChangeProfile.checked
        }
        TumblerDialog{
            id: tDialog
            titleText: "Select Profile"
            columns: [ profileColumn ]
            acceptButtonText: "Ok"
            onAccepted: btnChooseProfile.text = profileColumn.items[profileColumn.selectedIndex]; //thanks for depricating label...
        }
        TumblerColumn{
            id: profileColumn
            items: objProfileClient.profileTypes()
        }

//////////////////RULES
        Label{
            id: lblActivationHeader
            text: "When..."
            anchors.top: swChangeProfile.bottom
            anchors.margins: 5
            height: 20
        }
//////////////////LOCATION
        Switch{
            anchors.top: lblActivationHeader.bottom
            anchors.margins: 5
            id: swUseLocation
            checked: objQSettings.getValue("/rules/" + ruleName + "/Location/enabled",true)
        }
        Label{
            anchors.top: lblActivationHeader.bottom
            anchors.left: swUseLocation.right
            height: swUseLocation.height
            verticalAlignment: Text.AlignVCenter
            text: "Location match"
            color: "black"
        }
        Label{
            anchors.top: lblActivationHeader.bottom
            anchors.right: swUseLocationNot.left
            height: swUseLocation.height
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignRight
            text: "NOT"
            font.pixelSize: 24
            color: "black"

        }
        Switch{
            id: swUseLocationNot
            anchors.top: lblActivationHeader.bottom
            anchors.right: parent.right
            anchors.margins: 5
            checked: objQSettings.getValue("/rules/" + ruleName + "/Location/NOT",false)
            enabled: swUseLocation.checked
        }

        Label {
            anchors.top: swUseLocation.bottom
            id: lblLocationRadius
            height: 45
            verticalAlignment: Text.AlignVCenter
            text: "Radius (m): "
        }
        CountBubble{
            id: cbRadius
            anchors.verticalCenter: lblLocationRadius.verticalCenter
            anchors.left: lblLocationRadius.right
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
            id: lblLocationLatitude
            text: "Latitude: "
        }

        TextField{
            id: txtLocLatitude
            anchors.top: radiusSlider.bottom
            anchors.left: txtLocLongitude.left
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
        /////////////////////////CALENDAR

        Switch{
            id: swUseCalendar
            anchors.top: btnFillFromMap.bottom
            anchors.left: parent.left
            anchors.margins: 5
            checked: objQSettings.getValue("/rules/" + ruleName + "/Calendar/enabled",true)
        }
        Label{
            anchors.top: btnFillFromMap.bottom
            anchors.left: swUseCalendar.right
            height: swUseCalendar.height
            verticalAlignment: Text.AlignVCenter
            text: "Calendar keyword match"
            font.pixelSize: 24
            color: "black"
        }
        Label{
            height: swUseCalendar.height
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignRight
            text: "NOT"
            font.pixelSize: 24
            color: "black"
            anchors.right: swUseCalendarNOT.left
            anchors.top: btnFillFromMap.bottom
        }
        Switch{
            id: swUseCalendarNOT
            anchors.right: parent.right
            anchors.top: btnFillFromMap.bottom
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
            text: objQSettings.getValue("/rules/" + ruleName + "/Calendar/KEYWORDS","")
            placeholderText: "enter keywords seperated by spaces"
            font.pixelSize: 26
            validator: RegExpValidator{regExp: /(\w+ *)+/}
            maximumLength: 255
            enabled: swUseCalendar.checked
        }

        //////////////////////////TIMES
        Switch{
            id: swUseTime
            anchors.top: txtCalendarKeywords.bottom
            anchors.left: parent.left
            anchors.margins: 5
            checked: objQSettings.getValue("/rules/" + ruleName + "/Time/enabled",true)
        }
        Label{
            anchors.top: txtCalendarKeywords.bottom
            anchors.left: swUseTime.right
            height: swUseTimeNot.height
            verticalAlignment: Text.AlignVCenter
            text: "Between these times"
            color: "black"
        }
        Label{
            anchors.top: txtCalendarKeywords.bottom
            anchors.right: swUseTimeNot.left
            height: swUseTimeNot.height
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignRight
            text: "NOT"
            font.pixelSize: 24
            color: "black"

        }
        Switch{
            id: swUseTimeNot
            anchors.top: txtCalendarKeywords.bottom
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
            text: objQSettings.getValue("/rules/" + ruleName + "/Time/TIME1","?").slice(0,4)
            onClicked: {
                var _hours = 0
                var _minutes = 0
                if (btnTime1.text != "?"){
                    var time = btnTime1.text.match(/(\d?\d):?(\d\d)(:00)?/);
                    _hours = parseInt(time[1]);
                    _minutes = parseInt(time[2]);
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
            text: objQSettings.getValue("/rules/" + ruleName + "/Time/TIME2","?").slice(0,4)
            onClicked: {
                var _hours = 0
                var _minutes = 0
                if (btnTime1.text != "?"){
                    var time = btnTime1.text.match(/(\d?\d):?(\d\d)(:00)?/);
                    _hours = parseInt(time[1]);
                    _minutes = parseInt(time[2]);
                }
                appWindow.pageStack.push(Qt.resolvedUrl("TimePicker.qml"),
                  {time: 2, hours: _hours , minutes: _minutes } );

            }
            enabled: swUseTime.checked
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
                }

                objQSettings.setValue("/rules/" + txtRuleName.text + "/Actions/Profile/enabled",swChangeProfile.checked);
                objQSettings.setValue("/rules/" + txtRuleName.text + "/Actions/Profile/NAME", btnChooseProfile.text);

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
                objProximusUtils.refreshRulesModel(); //why can't i call the function in settingsPage now??
                appWindow.pageStack.pop()
            }
        }
    }
}
