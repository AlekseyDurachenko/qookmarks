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
QT             += core gui network webkit xml

DESTDIR         = bin
OBJECTS_DIR     = build/release_obj
MOC_DIR         = build/release_moc
UI_DIR          = build/release_ui
RCC_DIR         = build/release_rcc

include($$PWD/libs/qt-json/qt-json.pri)

DEFINES        +=                                           \

INCLUDEPATH    +=                                           \
    $$PWD/src                                               \
    $$PWD/src/import                                        \
    $$PWD/src/manager                                       \
    $$PWD/src/bookmarkview                                  \
    $$PWD/src/tagview                                       \

HEADERS        +=                                           \
    src/manager/ctagitemdata.h \
    src/manager/ctagitem.h \
    src/manager/cbookmarkmgr.h \
    src/manager/cbookmarkitemdata.h \
    src/manager/cbookmarkitem.h \
    src/import/importbookmarkchromium.h \
    src/import/cimportbookmarkdialog.h \
    src/global.h \
    src/ctageditdialog.h \
    src/consts.h \
    src/cmainwindow.h \
    src/cbookmarkeditdialog.h \
    src/caboutdialog.h \
    src/bookmarkview/cbookmarkitemmodel.h \
    src/bookmarkview/cbookmarktagproxymodel.h \
    src/tagview/ctagitemmodel.h \
    src/bookmarkview/cbookmarktreeview.h \
    src/tagview/ctagtreeview.h \
    src/manager/tagutils.h


SOURCES        +=                                           \
    src/manager/ctagitemdata.cpp \
    src/manager/ctagitem.cpp \
    src/manager/cbookmarkmgr.cpp \
    src/manager/cbookmarkitemdata.cpp \
    src/manager/cbookmarkitem.cpp \
    src/import/importbookmarkchromium.cpp \
    src/import/cimportbookmarkdialog.cpp \
    src/ctageditdialog.cpp \
    src/cmainwindow.cpp \
    src/cbookmarkeditdialog.cpp \
    src/caboutdialog.cpp \
    src/bookmarkview/cbookmarkitemmodel.cpp \
    src/bookmarkview/cbookmarktreeview.cpp \
    src/tagview/ctagtreeview.cpp \
    src/tagview/ctagitemmodel.cpp \
    src/bookmarkview/cbookmarktagproxymodel.cpp \
    src/manager/tagutils.cpp


FORMS          +=                                           \
    src/import/cimportbookmarkdialog.ui \
    src/ctageditdialog.ui \
    src/cmainwindow.ui \
    src/cbookmarkeditdialog.ui \
    src/caboutdialog.ui


RESOURCES      +=                                           \
    qrc/icons.qrc                                           \
    qrc/about.qrc                                           \
    qrc/sql.qrc                                             \

OTHER_FILES    +=                                           \
    AUTHORS                                                 \
    CHANGELOG                                               \
    LICENSE                                                 \
    README.md                                               \
    docs/project-tag-tree.txt \
    docs/project-tag-props.md \
    docs/project-functions.md \
    docs/project-bookmark-props.md

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
