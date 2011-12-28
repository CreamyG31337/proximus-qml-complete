symbian:TARGET.UID3 = 0xE4F86A89

# Smart Installer package's UID
# This UID is from the protected range
# and therefore the package will fail to install if self-signed
# By default qmake uses the unprotected range value if unprotected UID is defined for the application
# and 0x2002CCCF value if protected UID is given to the application
#symbian:DEPLOYMENT.installer_header = 0x2002CCCF

# Allow network access on Symbian
symbian:TARGET.CAPABILITY += NetworkServices

# If your application uses the Qt Mobility libraries, uncomment
# the following lines and add the respective components to the
# MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=

SOURCES += main.cpp \
    controller.cpp \
    profileclient.cpp

HEADERS += \
    controller.h \
    profileclient.h

CONFIG += mobility
QT += xml network svg dbus
MOBILITY += location
#profiles (read only?? whyyyyyyyy); SystemAlignedTimer
MOBILITY += systeminfo
#calendar
MOBILITY += organizer

# Please do not modify the following two lines. Required for deployment.
include(deployment.pri)
qtcAddDeployment()

