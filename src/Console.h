/*
    QEagleScaler * Qt based library for managing Eagle CAD XML files
    Copyright (C) 2012-2022 Mirai Computing (mirai.computing@gmail.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
//------------------------------------------------------------------------------
#ifndef CONSOLE_H
#define CONSOLE_H
//------------------------------------------------------------------------------
//#include <>
//------------------------------------------------------------------------------
#include "QEagleLib.h"
//------------------------------------------------------------------------------

class CEagleLibraryManager
{
 public:
  CEagleLibraryManager(void);
  ~CEagleLibraryManager(void);
 public:
  void clear(void);
  void init(int argc, char *argv[]);
  int run(void);
  bool executeCommand(const QStringList& arguments);
  bool executeCommand(const QString& command);
  bool executeScript(const QString& scriptName);
  void showHelpMessage(const QString& keyword = "");
  void shell(void);
 public:
  void show(const QString& source);
  void rewrite(const QString& source, const QString& target);
  void rewriteAll(const QString& source, const QString& target);
  void scale(const QString& source, const QString& target, const double value);
  void changeLibrarySuffix(const QString& source, const QString& target,
                           const QString& oldstr, const QString& newstr);
 private:
  void notImplemented(const QString& feature = "");
 private:
  QStringList m_Arguments;
  bool m_Initialized;
  bool m_ShellActive;
  //
  Eagle::CEagleDocument m_Source;
  Eagle::CEagleDocument m_Target;
  Eagle::CVersionNumber m_TargetVersion;
};

#endif // CONSOLE_H
//------------------------------------------------------------------------------

