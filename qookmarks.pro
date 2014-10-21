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
    $$PWD/src/manager                                       \
    $$PWD/src/model                                         \
    $$PWD/src/widgets/bookmarkview                          \
    $$PWD/src/widgets/tagview                               \
    $$PWD/src/widgets/tageditdialog                         \
    $$PWD/src/widgets/bookmarkeditdialog                    \
    $$PWD/src/import                                        \

HEADERS        +=                                           \
    src/manager/ctagitemdata.h                              \
    src/manager/ctagitem.h                                  \
    src/manager/cbookmarkitemdata.h                         \
    src/manager/cbookmarkitem.h                             \
    src/manager/cbookmarkmgr.h                              \
    src/manager/tagutils.h                                  \
    src/model/ctagitemmodel.h                               \
    src/model/cbookmarkitemmodel.h                          \
    src/model/cbookmarksortfilterproxymodel.h               \
    src/widgets/tagview/ctagtreeview.h                      \
    src/widgets/bookmarkview/cbookmarktreeview.h            \
    src/widgets/tageditdialog/ctageditdialog.h              \
    src/import/importbookmarkchromium.h                     \
    src/import/cimportbookmarkdialog.h                      \
    src/consts.h                                            \
    src/global.h                                            \
    src/cmainwindow.h                                       \
    src/caboutdialog.h                                      \
    src/manager/cbookmarkfilter.h                           \
    src/widgets/bookmarkview/cbookmarkreadlaterdelegate.h \
    src/widgets/bookmarkview/cbookmarkfavoritesdelegate.h \
    src/widgets/bookmarkeditdialog/cbookmarkeditdialog.h

SOURCES        +=                                           \
    src/manager/ctagitemdata.cpp                            \
    src/manager/ctagitem.cpp                                \
    src/manager/cbookmarkitemdata.cpp                       \
    src/manager/cbookmarkitem.cpp                           \
    src/manager/cbookmarkmgr.cpp                            \
    src/manager/tagutils.cpp                                \
    src/model/ctagitemmodel.cpp                             \
    src/model/cbookmarkitemmodel.cpp                        \
    src/model/cbookmarksortfilterproxymodel.cpp             \
    src/widgets/tagview/ctagtreeview.cpp                    \
    src/widgets/bookmarkview/cbookmarktreeview.cpp          \
    src/widgets/tageditdialog/ctageditdialog.cpp            \
    src/import/importbookmarkchromium.cpp                   \
    src/import/cimportbookmarkdialog.cpp                    \
    src/cmainwindow.cpp                                     \
    src/caboutdialog.cpp                                    \
    src/manager/cbookmarkfilter.cpp                         \
    src/widgets/bookmarkview/cbookmarkreadlaterdelegate.cpp \
    src/widgets/bookmarkview/cbookmarkfavoritesdelegate.cpp \
    src/widgets/bookmarkeditdialog/cbookmarkeditdialog.cpp

FORMS          +=                                           \
    src/widgets/tageditdialog/ctageditdialog.ui             \
    src/import/cimportbookmarkdialog.ui                     \
    src/cmainwindow.ui                                      \
    src/caboutdialog.ui                                     \
    src/widgets/bookmarkeditdialog/cbookmarkeditdialog.ui

RESOURCES      +=                                           \
    qrc/icons.qrc                                           \
    qrc/about.qrc                                           \

OTHER_FILES    +=                                           \
    AUTHORS                                                 \
    CHANGELOG                                               \
    LICENSE                                                 \
    README.md                                               \
    docs/project-tag-tree.txt                               \
    docs/project-tag-props.md                               \
    docs/project-functions.md                               \
    docs/project-bookmark-props.md                          \

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
