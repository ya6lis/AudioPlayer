QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    audioslider.cpp \
    main.cpp \
    mainwindow.cpp \
    songwidget.cpp

HEADERS += \
    audioslider.h \
    mainwindow.h \
    songwidget.h

FORMS += \
    mainwindow.ui \
    songwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
	icons.qrc

CONFIG(release, debug|release) {
	win32: QMAKE_POST_LINK = $$(QTDIR)/bin/windeployqt $$OUT_PWD/release

}
RC_ICONS = icons/gordon.ico
