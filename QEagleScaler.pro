#
#    QEagleScaler * Qt based library for managing Eagle CAD XML files
#    Copyright (C) 2012-2022 Mirai Computing (mirai.computing@gmail.com)
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

QT       += core xml

QT       -= gui

TARGET = QEagleScaler
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++98

INCLUDEPATH += lib src

SOURCES += src/QEagleScaler.cpp\
        src/QEagleLib.cpp\
        src/Console.cpp\

HEADERS  += src/QEagleLib.h\
	src/Console.h
