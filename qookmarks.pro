# Copyright 2014, Durachenko Aleksey V. <durachenko.aleksey@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
TARGET          = qookmarks
TEMPLATE        = app

CONFIG         += console debug_and_release
QT             += core gui network sql

DESTDIR         = bin
OBJECTS_DIR     = build/release_obj
MOC_DIR         = build/release_moc
UI_DIR          = build/release_ui
RCC_DIR         = build/release_rcc

include($$PWD/libs/qt-json/qt-json.pri)

DEFINES        +=                                           \

INCLUDEPATH    +=                                           \

INCLUDEPATH    +=                                           \
    src                                                     \

HEADERS        +=                                           \
    src/consts.h                                            \
    src/global.h                                            \
    src/cmainwindow.h                                       \
    src/caboutdialog.h                                      \
    src/cbookmarkitem.h                                     \
    src/cbookmarkmgr.h                                      \
    src/cbookmarkitemdata.h                                 \
    src/cstorage.h                                          \
    src/cbookmarkitemmodel.h                                \
    src/ctagitem.h                                          \
    src/ctagitemdata.h                                      \
    src/ctagitemmodel.h                                     \
    src/cbookmarktagproxymodel.h \
    src/ctagtreeview.h \
    src/ctageditdialog.h \
    src/cbookmarktreeview.h \
    src/cbookmarkeditdialog.h

SOURCES        +=                                           \
    src/cmainwindow.cpp                                     \
    src/caboutdialog.cpp                                    \
    src/cbookmarkitem.cpp                                   \
    src/cbookmarkmgr.cpp                                    \
    src/cbookmarkitemdata.cpp                               \
    src/cstorage.cpp                                        \
    src/cbookmarkitemmodel.cpp                              \
    src/ctagitem.cpp                                        \
    src/ctagitemdata.cpp                                    \
    src/ctagitemmodel.cpp                                   \
    src/cbookmarktagproxymodel.cpp \
    src/ctagtreeview.cpp \
    src/ctageditdialog.cpp \
    src/cbookmarktreeview.cpp \
    src/cbookmarkeditdialog.cpp

FORMS          +=                                           \
    src/cmainwindow.ui                                      \
    src/caboutdialog.ui \
    src/ctageditdialog.ui \
    src/cbookmarkeditdialog.ui

RESOURCES      +=                                           \
    qrc/icons.qrc                                           \
    qrc/about.qrc                                           \
    qrc/sql.qrc                                             \

OTHER_FILES    +=                                           \
    AUTHORS                                                 \
    CHANGELOG                                               \
    LICENSE                                                 \
    README.md                                               \

# if you want to test the project, do: qmake "QT += testlib"
contains(QT, testlib) {
    SOURCES   +=                                            \

    HEADERS   +=                                            \

} else {
    SOURCES   +=                                            \
        src/main.cpp                                        \

}

win32 {
    CONFIG += embed_manifest_exe
    RC_FILE = qookmarks.rc
}

# debug
build_pass:CONFIG(debug, debug|release) {
    TARGET      = $$join(TARGET,,,d)
    OBJECTS_DIR = $$join(OBJECTS_DIR,,,d)
    MOC_DIR     = $$join(MOC_DIR,,,d)
    UI_DIR      = $$join(UI_DIR,,,d)
    RCC_DIR     = $$join(RCC_DIR,,,d)

    win32 {
    }

    unix {
        LIBS +=
        DEFINES += "G_VERSION=\"\\\"$$system($$PWD/get_version.sh)\\\"\""
    }
}

# release
build_pass:CONFIG(release, debug|release) {
    win32 {
        CONFIG -= console
        LIBS +=
        DEFINES += "G_VERSION=\"\\\"$$system('sh -c $$PWD/get_version.sh')\\\"\""
    }

    unix {
        LIBS +=
        DEFINES += "G_VERSION=\"\\\"$$system($$PWD/get_version.sh)\\\"\""
    }
}
