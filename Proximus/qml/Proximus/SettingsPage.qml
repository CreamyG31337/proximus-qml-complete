import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.1

Page {
    anchors.margins: 7
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

    InfoBanner{
        id: infoMessageBanner
        z: 99
        topMargin: parent.height - 100 //show at bottom instead
    }
    Label {
        id: lblSettings
        //anchors.centerIn: parent
        anchors{top: parent.top
            horizontalCenter: parent.horizontalCenter
        }
        text: qsTr("Global Settings")
        font.bold: true
        font.pixelSize: 25
        anchors.topMargin: 5
    }
    Row{
        id: rowSettings1
        spacing: 10
        anchors.top: lblSettings.bottom
        anchors.margins: 6
        Switch{
            id: swGPS
            checked: objQSettings.getValue("/settings/GPS/enabled",true)
            onCheckedChanged: {
                objQSettings.setValue("/settings/GPS/enabled",swGPS.checked)
                if (swGPS.checked)
                    showInfo("Service will now reset and use all positioning methods (more battery used)");
                else
                    showInfo("Service will now reset use non-satellite positioning methods (less battery used)");
            }
        }
        Text{
            height: swGPS.height
            verticalAlignment: Text.AlignVCenter
            text: "Use GPS"
            font.pixelSize: 22
            color: "black"
        }
        Switch{
            id: swService
            anchors.leftMargin: 5
            checked: objQSettings.getValue("/settings/Service/enabled",false)
            onCheckedChanged: {
                objQSettings.setValue("/settings/Service/enabled",swService.checked)
                if (swService.checked)
                    showInfo("Service will start again when you reboot the phone");
                else
                    showInfo("Service will shut down momentarily");
            }
        }
        Text{
            height: swGPS.height
            verticalAlignment: Text.AlignVCenter
            text: "Enable Service"
            font.pixelSize: 22
            color: "black"
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
        Text{
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
    Label {
        id: lblRules
        //anchors.centerIn: parent
        anchors{
            top: rowSettings3.bottom
            topMargin: 10
        }
        text: qsTr("Rules: ")
        font.pixelSize: 25
        font.bold: true
    }
    Button{
        id: btnNew
        anchors {
            right: parent.right
            top: rowSettings3.verticalCenter
        }
        width: 150
        text: qsTr("New")
        onClicked: {
            settingsPage.selectedRuleName = ""
            rulesList.currentIndex = -1;
            appWindow.pageStack.push(Qt.resolvedUrl("RulePage.qml"))
        }
        style: PositiveButtonStyle {}
    }
    Button{
        id: btnEdit
        anchors {
            right: parent.right
            top: btnNew.bottom
            topMargin: 5
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
            topMargin: 5
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
            topMargin: 5
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
            topMargin: 5
        }
        width: 150
        text: qsTr("Delete")
        onClicked: {
            objQSettings.remove("/rules/"+  settingsPage.selectedRuleName)
        resetList();
        }
        style: NegativeButtonStyle{}
    }

    ListView{
        id: rulesList
        model: objRulesModel
        height: tabStatus.height - 225
        clip: true
        delegate:  Text{
            height: 40;
            font.pixelSize: 25;
            text: "Rule " + index + " " + model.modelData.name;
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
        anchors {
            top: lblRules.bottom
            left: parent.left
            right: btnDelete.left
           // bottom: parent.bottom
        }
        highlight: Rectangle {color: "lightsteelblue"; radius: 6; width: rulesList.width}
        focus: true
        Component.onCompleted: {//not sure how to get already selected name, just remove highlight then...
            rulesList.currentIndex = -1;
        }
    }
}
