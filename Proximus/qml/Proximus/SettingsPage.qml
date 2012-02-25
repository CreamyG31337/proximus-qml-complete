import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0

Page {
    anchors.topMargin: 5
    anchors.bottomMargin: 5
    anchors.leftMargin: 2
    anchors.rightMargin: 12
    id: settingsPage
    tools: commonTools

    property string selectedRuleName

    function resetList(){
        objProximusUtils.refreshRulesModel();
        rulesList.currentIndex = -1;
        btnEnable.visible = false;
        btnDisable.visible = false;
        btnEdit.enabled = false;
        btnDelete.enabled = false;
    }

    function showInfo(infoString)
    {
        infoMessageBanner.text = infoString;
        iHateThis.start(); //need to use timer to show message because the same click that
        //triggers the banner dismisses it.
    }
    Timer {
        id: iHateThis
        interval: 75; running: false; repeat: false
        onTriggered: infoMessageBanner.show();
    }
    Timer {
        id: blockEvents
        interval: 200; running: true; repeat: false
    }

    InfoBanner{
        id: infoMessageBanner
        z: 99
        topMargin: parent.height - 100 //show at bottom instead
    }
    ScrollDecorator {
        id: scrolldecorator
        flickableItem: flickable
    }
    Flickable{
        id: flickable
        anchors.fill: parent
        contentHeight: 800
        Row{
            id: rowSettings0
            spacing: 10
            anchors.top: parent.top
            anchors.margins: 6
            Switch{
                id: swPositioning
                checked: objQSettings.getValue("/settings/Positioning/enabled",true)
                onCheckedChanged: { //wow this crappy harmattan QML is missing both the pressed and clicked events and properties for switches.
                    if (!blockEvents.running) {//so we get to use this instead. (onChecked fires too early -- when the component is created)
                        objQSettings.setValue("/settings/Positioning/enabled",swPositioning.checked)
                      //  swGPS.enabled = swPositioning.checked;
                        if (swPositioning.checked)
                            showInfo("Service will reset and allow positioning.");
                        else
                            showInfo("Service will reset and disable positioning. If you use it in your rules, it won't be evaluated.");
                    }
                }
            }
            Label{
                height: swPositioning.height
                verticalAlignment: Text.AlignVCenter
                text: "Use Nokia Positioning methods"
            }
        }
        Row{
            id: rowSettings1
            spacing: 10
            anchors.top: rowSettings0.bottom
            anchors.margins: 6
            Switch{
                id: swGPS
                enabled: swPositioning.checked;
                checked: objQSettings.getValue("/settings/GPS/enabled",true)
                onCheckedChanged: { //wow this crappy harmattan QML is missing both the pressed and clicked events and properties for switches.
                    if (!blockEvents.running) {//so we get to use this instead. (onChecked fires too early -- when the component is created)
                        objQSettings.setValue("/settings/GPS/enabled",swGPS.checked)
                        if (swGPS.checked)
                            showInfo("Service will now reset and use all positioning methods (more battery used)");
                        else
                            showInfo("Service will now reset use non-satellite positioning methods (SUPPOSED to be less battery used, YMMV)");
                    }
                }
            }
            Label{
                height: swGPS.height
                verticalAlignment: Text.AlignVCenter
                text: "Use GPS"
            }
            Switch{
                id: swService
                anchors.leftMargin: 5
                checked: objQSettings.getValue("/settings/Service/enabled",false)
                onCheckedChanged: {
                    if (!blockEvents.running){
                        objQSettings.setValue("/settings/Service/enabled",swService.checked)
                        if (swService.checked)
                            showInfo("Service will start again when you reboot the phone");
                        else
                            showInfo("Service will shut down momentarily");
                    }
                }
            }
            Label{
                height: swGPS.height
                verticalAlignment: Text.AlignVCenter
                text: "Enable Service"
                font.pixelSize: 22
            }
        }

        Row{
            id: rowSettings25
            spacing: 10
            anchors.top: rowSettings1.bottom
            anchors.margins: 6
            CountBubble{
                anchors.leftMargin: 5
                anchors.verticalCenter: txtGPSUI.verticalCenter
                id: cbGPSInterval
                value: gPSintervalSlider.value
                largeSized: true
                anchors.margins: 10
                width: swGPS.width
            }
            Label{
                id: txtGPSUI
                text: "GPS Update Interval (seconds)"
                font.pixelSize: 22
            }
        }
        Row{
            id: rowSettings3
            spacing: 10
            anchors.top: rowSettings25.bottom
            anchors.margins: 6
            Slider{
                id: gPSintervalSlider
                value: objQSettings.getValue("/settings/GPS/interval",60)
                stepSize: 15
                maximumValue: 1800
                width: settingsPage.width - btnNew.width
                onPressedChanged: {
                    if (!gPSintervalSlider.pressed)
                    {
                        objQSettings.setValue("/settings/GPS/interval",cbGPSInterval.value)
                    }
                }
            }
        }

        Row{
            id: rowSettings4
            spacing: 10
            anchors.top: rowSettings3.bottom
            anchors.margins: 6
            CountBubble{
                anchors.leftMargin: 5
                anchors.verticalCenter: txtWiFi.verticalCenter
                id: cbWiFiInterval
                value: wifiIntervalSlider.value
                largeSized: true
                anchors.margins: 10
                width: swGPS.width
            }
            Label{
                id: txtWiFi
                text: "WiFi Scan Interval (minutes)"
                font.pixelSize: 22
            }
        }
        Row{
            id: rowSettings5
            spacing: 10
            anchors.top: rowSettings4.bottom
            anchors.margins: 6
            Slider{
                id: wifiIntervalSlider
                value: objQSettings.getValue("/settings/WIFI/interval",60)
                stepSize: 1
                minimumValue: 1
                maximumValue: 60
                width: settingsPage.width - btnNew.width
                onPressedChanged: {
                    if (!wifiIntervalSlider.pressed)
                    {
                        objQSettings.setValue("/settings/WIFI/interval",wifiIntervalSlider.value)
                    }
                }
            }
        }
        Rectangle{
            id: recRules
            color: "transparent"
            height: 400
            width: parent.width
            anchors.top: rowSettings5.bottom
            border.width: 2
            border.color: "steelblue"
            radius: 10
            anchors.margins: 0
            Label {
                id: lblRules
                //anchors.centerIn: parent
                anchors{
                    top: parent.top
                    left: parent.left
                    margins: 4
                }
                text: qsTr("Rules: ")
                font.pixelSize: 25
                font.bold: true
            }
            Button{
                id: btnNew
                anchors {
                    right: parent.right
                    top: parent.top
                    margins: 4
                }
                width: 150
                text: qsTr("New")
                onClicked: {
                    console.log(rulesList.count)
                    rulesList.currentIndex = -1;
                    appWindow.pageStack.push(Qt.resolvedUrl("RulePage.qml"),
                                             {ruleNum: rulesList.count})
                }
                style: PositiveButtonStyle {}
            }
            Button{
                id: btnEdit
                anchors {
                    right: parent.right
                    top: btnNew.bottom
                    margins: 4
                }
                enabled: false
                width: 150
                text: qsTr("Edit")
                onClicked: {
                    appWindow.pageStack.push(Qt.resolvedUrl("RulePage.qml"),
                                             {ruleName: settingsPage.selectedRuleName});
                }
            }
            Button{
                id: btnEnable
                anchors {
                    right: parent.right
                    top: btnEdit.bottom
                    margins: 4
                }
                visible: false
                width: 150
                text: qsTr("Enable")
                onClicked: {
                objQSettings.setValue("/rules/" +  settingsPage.selectedRuleName + "/enabled",true)
                resetList();
                }
            }
            Button{
                id: btnDisable
                anchors {
                    right: parent.right
                    top: btnEdit.bottom
                    margins: 4
                }
                visible: false
                width: 150
                text: qsTr("Disable")
                onClicked: {
                objQSettings.setValue("/rules/" +  settingsPage.selectedRuleName + "/enabled",false)
                resetList();
                }
            }
            Button{
                id: btnDelete
                enabled: false
                anchors {
                    right: parent.right
                    top: btnEnable.bottom
                    margins: 4
                }
                width: 150
                text: qsTr("Delete")
                onClicked: {
                    //should probably confirm this
                    objProximusUtils.deleteRule(rulesList.currentIndex + 1)
                    rulesList.currentIndex = -1;
                    btnEnable.visible = false;
                    btnDisable.visible = false;
                    btnEdit.enabled = false;
                    btnDelete.enabled = false;
                }
                style: NegativeButtonStyle{}
            }
            ScrollDecorator {
                id: scrolldecorator2
                flickableItem: rulesList
            }
            ListView{
                id: rulesList
                model: objRulesModel
               // height: 315
                width: parent.width - btnEdit.width - 50
                clip: true
                anchors {
                    top: lblRules.bottom
                    left: parent.left
                    margins: 4
                    //right: btnDelete.left
                    bottom: btnMoveUp.top
                }
                delegate:
                    Label{
                        height: 40;
                        font.pixelSize: 25;
                       // text: "Rule " + index + " " + model.modelData.name;
                        verticalAlignment: Text.AlignVCenter
                        text: "#" + model.modelData.number + " " + model.modelData.name;
                        color:(model.modelData.enabled == true)?'green':'red';
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if (model.modelData.enabled == true){
                                    btnDisable.visible = true
                                    btnEnable.visible = false
                                }
                                else{
                                    btnEnable.visible = true
                                    btnDisable.visible = false
                                }
                                btnEdit.enabled = true;
                                btnDelete.enabled = true;
                                rulesList.currentIndex = index
                                settingsPage.selectedRuleName =  model.modelData.name;
                            }
                        }
                    }

                highlight: Rectangle {color: "lightsteelblue"; radius: 6; width: rulesList.width}
                focus: true
                Component.onCompleted: {//not sure how to get already selected name, just remove highlight then...
                    rulesList.currentIndex = -1;
                }
            }
            Button{
                id: btnMoveUp
                enabled: true
                anchors {
                    left: parent.left
                    bottom: parent.bottom
                    margins: 4
                }
                width: 150
                text: qsTr("+Up+")
                onClicked: {
                    if (rulesList.currentIndex >= 1){
                        var i = rulesList.currentIndex;
                        objProximusUtils.moveRuleUp(rulesList.currentIndex + 1);
                        rulesList.currentIndex = i - 1
                    }
                }
            }
            Button{
                id: btnMoveDown
                enabled: true
                anchors {
                    left: btnMoveUp.right
                    bottom: parent.bottom
                    margins: 4
                }
                width: 150
                text: qsTr("-Down-")
                onClicked: {
                    if ((rulesList.currentIndex + 1) !== rulesList.count){
                        var i = rulesList.currentIndex;
                        objProximusUtils.moveRuleDown(rulesList.currentIndex + 1);
                        rulesList.currentIndex = i + 1
                    }
                }
            }
        }//rectangle
    }//flickable
}
