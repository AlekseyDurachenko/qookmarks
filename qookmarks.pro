# Copyright 2014-2015, Durachenko Aleksey V. <durachenko.aleksey@gmail.com>
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

# project options
TEMPLATE        = app
TARGET          = qookmarks

CONFIG         += console debug_and_release
QT             += core gui network webkit


# release build options
release:DESTDIR     = bin
release:OBJECTS_DIR = build/release/obj
release:MOC_DIR     = build/release/moc
release:RCC_DIR     = build/release/rcc
release:UI_DIR      = build/release/ui
# debug build options
debug:DESTDIR       = bin
debug:OBJECTS_DIR   = build/debug/obj
debug:MOC_DIR       = build/debug/moc
debug:RCC_DIR       = build/debug/rcc
debug:UI_DIR        = build/debug/ui


# uncomment this line if you want to start the test
# or use: qmake "QT += testlib"
#QT      += testlib


# uncomment this line if you want use application as portable
# (all files will placed in the application directory)
#DEFINES += APP_PORTABLE


# uncomment this line if you want deploy the application
# (this line enables the many additional dialogs)
#DEFINES += APP_DEPLOY


# necessary variables used for versing
# (usage: qmake "DEFINES += APP_NAME=target-app-name APP_MAJOR=1 APP_MINOR=2 APP_PATCH=3")
app_name  = $$find(DEFINES, "APP_NAME=")
app_major = $$find(DEFINES, "APP_MAJOR=")
app_minor = $$find(DEFINES, "APP_MINOR=")
app_patch = $$find(DEFINES, "APP_PATCH=")
count(app_name,  0):DEFINES += "APP_NAME=$$TARGET"
count(app_major, 0):DEFINES += "APP_MAJOR=0"
count(app_minor, 0):DEFINES += "APP_MINOR=1"
count(app_patch, 0):DEFINES += "APP_PATCH=0"


# optional variables used for versing
# (svn revision, git revision or custom revision(examle: for libraries))
# (usage: qmake "DEFINES += APP_REV=1")
#app_rev = $$find(DEFINES, "APP_REV=")
#count(app_rev, 0):DEFINES += "APP_REV=1"


# build system variables used for versing
# (usage: qmake "DEFINES += APP_BUILD_NUMBER=2 APP_BUILD_DATE=\\\"'2014-01-01 12:12:12Z'\\\"")
#app_build_number = $$find(DEFINES, "APP_BUILD_NUMBER=")
#app_build_date   = $$find(DEFINES, "APP_BUILD_DATE=")
#count(app_build_number,  0):DEFINES += "APP_BUILD_NUMBER=2"
#count(app_build_date,    0):DEFINES += "APP_BUILD_DATE='2014-01-01 00:00:00'"


# current project (.pri)
include($$PWD/qookmarks-sources.pri)
include($$PWD/qookmarks-tests.pri)
include($$PWD/qookmarks-unixinstall.pri)
include($$PWD/qookmarks-unixrules.pri)


# the 3rd libraries (.pri)
include($$PWD/submodules/qt-json/qt-json.pri)
