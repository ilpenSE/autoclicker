QT       += core gui
QT += sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    appdatamanager.cpp \
    languagemanager.cpp \
    logger.cpp \
    macromanager.cpp \
    macroselectionwin.cpp \
    main.cpp \
    mainwindow.cpp \
    settingsmanager.cpp \
    settingswin.cpp \
    thememanager.cpp

HEADERS += \
    Enums.h \
    appdatamanager.h \
    consts.h \
    languagemanager.h \
    logger.h \
    macromanager.h \
    macroselectionwin.h \
    mainwindow.h \
    settingsmanager.h \
    settingswin.h \
    thememanager.h \
    versionmanager.h

FORMS += \
    macroselectionwin.ui \
    mainwindow.ui \
    settingswin.ui

TRANSLATIONS += \
    assets/locale/en_US.ts \
    assets/locale/tr_TR.ts \
    assets/locale/de_DE.ts \
    assets/locale/fr_FR.ts \
    assets/locale/it_IT.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc \
    translations.qrc
