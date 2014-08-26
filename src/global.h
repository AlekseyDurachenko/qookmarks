// Copyright 2014, Durachenko Aleksey V. <durachenko.aleksey@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
#ifndef GLOBAL_H
#define GLOBAL_H

const char g_programName[] = "qookmarks";

#define G_SETTINGS_INIT()                                                   \
        QSettings settings(QSettings::IniFormat, QSettings::UserScope,      \
                g_programName, g_programName);                              \

#define G_FULL_PROGRAM_NAME()                                               \
        (QString("%1 - %2")                                                 \
                .arg(g_programName)                                         \
                .arg(G_VERSION))                                            \

#define G_SHORT_PROGRAM_NAME()                                              \
        (QString("%1")                                                      \
                .arg(g_programName))                                        \

#endif
