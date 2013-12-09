TARGET = qgvdial

# Add more folders to ship with the application, here
folder_01.source = qml/symbian
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

MOC_DIR = moc
OBJECTS_DIR = obj

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

CONFIG *= mobility
MOBILITY *= feedback

QT *= phonon

# Add dependency to Symbian components
CONFIG += qt-components

# The Symbian telephony stack library and the equivalent of openssl
LIBS += -letel3rdparty -llibcrypto

DEFINES += ENABLE_FUZZY_TIMER

INCLUDEPATH += $$PWD
SOURCES  += SymbianPhoneFactory.cpp
HEADERS  += platform_specific.h \
            SymbianPhoneFactory.h

include(../common/common.pri)
include(../../api/api.pri)
include(../features/openssl/openssl.pri)
include(../features/dirs/linux/linux-dirs.pri)
include(../features/qml/mainwindow/qml-mainwindow.pri)
include(../features/cookie-cutter/osdependent/cc-osdependent.pri)
include(../features/osver/nokia/symbian/osv-symbian.pri)

RESOURCES += symbian.qrc

#####################################################
# Deployment information
#####################################################
symbian {
TARGET.UID3 = 0x2003B499
TARGET.CAPABILITY += NetworkServices ReadUserData ReadDeviceData WriteDeviceData SwEvent
TARGET.EPOCSTACKSIZE = 0x14000          # 80 KB stack size
TARGET.EPOCHEAPSIZE = 0x020000 0x2000000 # 128 KB - 20 MB
}

# Smart Installer package's UID
# This UID is from the protected range and therefore the package will
# fail to install if self-signed. By default qmake uses the unprotected
# range value if unprotected UID is defined for the application and
# 0x2002CCCF value if protected UID is given to the application
symbian:DEPLOYMENT.installer_header = 0x2002CCCF

vendorinfo = "%{\"Yuvraaj Kelkar\"}" \
             ":\"Yuvraaj Kelkar\""
my_deployment.pkg_prerules = vendorinfo
DEPLOYMENT += my_deployment
#####################################################

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()
