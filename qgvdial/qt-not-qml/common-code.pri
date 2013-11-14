TARGET=qgvdial

# Add files and directories to ship with the application
# by adapting the examples below.
# file1.source = myfile
# dir1.source = mydir
DEPLOYMENTFOLDERS = # file1 dir1

INCLUDEPATH += $$PWD

SOURCES  += $$PWD/MainWindow.cpp \
            $$PWD/MainWindow_p.cpp \
            $$PWD/ContactDialog.cpp \
            $$PWD/InboxEntryDialog.cpp \
            $$PWD/CINumberDialog.cpp \
            $$PWD/CiListView.cpp \
            $$PWD/GvNumComboBox.cpp \
            $$PWD/DblClickLabel.cpp \
            $$PWD/DummySystemTray.cpp
HEADERS  += $$PWD/MainWindow.h \
            $$PWD/MainWindow_p.h \
            $$PWD/ContactDialog.h \
            $$PWD/InboxEntryDialog.h \
            $$PWD/CINumberDialog.h \
            $$PWD/CiListView.h \
            $$PWD/GvNumComboBox.h \
            $$PWD/DblClickLabel.h \
            $$PWD/DummySystemTray.h

FORMS    += $$PWD/mainwindow.ui \
            $$PWD/ContactDialog.ui \
            $$PWD/InboxEntryDialog.ui \
            $$PWD/CINumberDialog.ui

RESOURCES += $$PWD/qtnotqml.qrc

include($$PWD/../common/common.pri)
include($$PWD/../../api/api.pri)
include($$PWD/../features/openssl/openssl.pri)
include($$PWD/../features/cookie-cutter/osdependent/cc-osdependent.pri)

!blackberry {
    include($$PWD/../features/qtsingleapplication/qtsingleapplication.pri)
}
