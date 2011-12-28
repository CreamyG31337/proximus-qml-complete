# Add more folders to ship with the application, here
folder_01.source = qml/Proximus
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

folder_02.source = images
folder_02.target = "."
DEPLOYMENTFOLDERS += folder_02

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

symbian:TARGET.UID3 = 0xE5A169FD

# Smart Installer package's UID
# This UID is from the protected range and therefore the package will
# fail to install if self-signed. By default qmake uses the unprotected
# range value if unprotected UID is defined for the application and
# 0x2002CCCF value if protected UID is given to the application
#symbian:DEPLOYMENT.installer_header = 0x2002CCCF

# Allow network access on Symbian
symbian:TARGET.CAPABILITY += NetworkServices

maemo5 {
    CONFIG += mobility12
    INCLUDEPATH += /usr/include/profiled \
                 /usr/include/dbus-1.0 \
                 /usr/lib/dbus-1.0/include \
                 /opt/qtm12/
    QT += dbus
} else {
    simulator {
    CONFIG += mobility
    #no idea what to do for this...
    } else {
    #harmattan (or symbian). Why can't we just get an environment macro for harmattan? >:(
    CONFIG += mobility
    CONFIG += qdeclarative-boostable
    INCLUDEPATH += /usr/include/applauncherd
    INCLUDEPATH += /usr/include/qt4
#    #the macro Q_WS_SIMULATOR is not working properly in the following two files!!
#    SOURCES += profileclient.cpp
#    HEADERS += profileclient.h
    #conflict in some dbus stuff i need from glib-2.0 which is the only way to set active profile in harmattan at this time >:(
    #CONFIG += no_keywords
    INCLUDEPATH += /usr/include/glib-2.0 \
                   /usr/lib/glib-2.0/include
    QT += dbus #(dbus is not supported by simulator)
    }
}

#CONFIG += meegotouch
#gps
MOBILITY += location
#profiles (read only?? whyyyyyyyy); SystemAlignedTimer
MOBILITY += systeminfo
#calendar
MOBILITY += organizer
#gconf
#MOBILITY += publishsubscribe

equals(QT_MAJOR_VERSION, 4):lessThan(QT_MINOR_VERSION, 7){
    MOBILITY += bearer
#    INCLUDEPATH += ../../src/bearer
} else {
    # use Bearer Management classes in QtNetwork module
}

QT += xml network svg

# Add dependency to Symbian components
# CONFIG += qt-components

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    profileclient.cpp

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()

HEADERS += \
    main.h \
    profileclient.h
