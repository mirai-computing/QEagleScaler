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
#include <iostream>
//------------------------------------------------------------------------------
#include "Console.h"
//------------------------------------------------------------------------------

QString unquote(const QString& str)
{
 QString s = str;
 if (s.startsWith("\"") || s.startsWith("'"))
 {
  s = s.right(s.length()-1);
 }
 if (s.endsWith("\"") || s.endsWith("'"))
 {
  s = s.left(s.length()-1);
 }
 return s;
}

CEagleLibraryManager::CEagleLibraryManager(void)
{
 clear();
}

CEagleLibraryManager::~CEagleLibraryManager(void)
{
 clear();
}

void CEagleLibraryManager::clear(void)
{
 m_Initialized = false;
 m_Arguments.clear();
 m_ShellActive = false;
 m_Source.clear();
 m_Target.clear();
 m_TargetVersion.assign(STRING(EAGLE_DTD_MIN_VERSION));
}

void CEagleLibraryManager::init(int argc, char *argv[])
{
 m_Arguments.clear();
 for (int i = 1; i < argc; i++)
 {
  m_Arguments.append(argv[i]);
 }
 m_Initialized = true;
}

int CEagleLibraryManager::run(void)
{
 if (!m_Initialized) return 0;
 executeCommand(m_Arguments);
 return 0;
}

bool CEagleLibraryManager::executeCommand(const QStringList& arguments)
{
 QStringListIterator args_it(arguments);
 if (0==arguments.count())
 {
  showHelpMessage();
  return true;
 }
 while (args_it.hasNext())
 {
  QString arg = args_it.next();
  if ("show"==arg)
  {
   if (args_it.hasNext())
   {
    QString source_name = args_it.next();
    show(source_name);
    continue;
   }
  }
  if ("rewrite"==arg)
  {
   if (args_it.hasNext())
   {
    QString source_name = args_it.next();
    if (args_it.hasNext())
    {
     QString target_name = args_it.next();
     if (args_it.hasNext())
     {
      QString version_str = args_it.next();
      m_TargetVersion.assign(version_str);
     }
     rewrite(source_name,target_name);
     continue;
    }
   }
  }
  if ("rewriteall"==arg)
  {
   if (args_it.hasNext())
   {
    QString source_name = args_it.next();
    if (args_it.hasNext())
    {
     QString target_name = args_it.next();
     if (args_it.hasNext())
     {
      QString version_str = args_it.next();
      m_TargetVersion.assign(version_str);
     }
     rewrite(source_name,target_name);
     continue;
    }
   }
  }
  if ("scale"==arg)
  {
   if (args_it.hasNext())
   {
    QString source_name = args_it.next();
    if (args_it.hasNext())
    {
     QString target_name = args_it.next();
     if (args_it.hasNext())
     {
      QString str_value = args_it.next();
      bool ok = false;
      double value = str_value.toDouble(&ok);
      if (ok)
      {
       if (args_it.hasNext())
       {
        QString version_str = args_it.next();
        m_TargetVersion.assign(version_str);
       }
       scale(source_name,target_name,value);
       continue;
      }
     }
    }
   }
  }
  if ("libsuffix"==arg)
  {
   if (args_it.hasNext())
   {
    QString source_name = args_it.next();
    if (args_it.hasNext())
    {
     QString target_name = args_it.next();
     if (args_it.hasNext())
     {
      QString oldsfx = args_it.next();
      if (args_it.hasNext())
      {
       QString newsfx = args_it.next();
       if (args_it.hasNext())
       {
        QString version_str = args_it.next();
        m_TargetVersion.assign(version_str);
       }
       changeLibrarySuffix(source_name,target_name,oldsfx,newsfx);
       continue;
      }
     }
    }
   }
  }
  if (("help"==arg) || ("--help"==arg))
  {
   if (args_it.hasNext())
   {
    QString keyword = args_it.next();
    showHelpMessage(keyword);
   }
   else
   {
    showHelpMessage();
   }
   continue;
  }
  if ("shell"==arg)
  {
   shell();
   continue;
  }
  if (("quit"==arg) || ("exit"==arg))
  {
   m_ShellActive = false;
   continue;
  }
  if ("exec"==arg)
  {
   if (args_it.hasNext())
   {
    QString scriptName = args_it.next();
    executeScript(scriptName);
    continue;
   }
  }
  if ("sys"==arg)
  {
   if (args_it.hasNext())
   {
    QString proc_name = args_it.next();
    QStringList proc_args;
    while (args_it.hasNext())
    {
     proc_args.append(args_it.next());
    }
    QProcess::execute(proc_name,proc_args);
    std::cin.clear();
    continue;
   }
  }
  std::cout<<"Command not recognized."<<std::endl;
  return false;
 }
 return true;
}

bool CEagleLibraryManager::executeCommand(const QString& command)
{
 if (command.isEmpty()) return false;
 std::cout<<"Executing command: "<<command.toStdString()<<std::endl;
 return executeCommand(command.split(" "));
}

bool CEagleLibraryManager::executeScript(const QString& scriptName)
{
 QFile script_file(scriptName);
 if (script_file.open(QFile::ReadOnly))
 {
  QTextStream script_stream(&script_file);
  QStringList script_text; QString script_line;
  while (!script_stream.atEnd())
  {
   script_line = script_stream.readLine();
   std::cout<<"Read script line: "<<script_line.toStdString()<<std::endl;
   script_text.append(script_line);
  }
  for (int i = 0; i < script_text.count(); i++)
  {
   if (!executeCommand(script_text.at(i)))
   {
    std::cout<<"Error executing script at line "<<i<<": "
             <<script_text.at(i).toStdString()<<std::endl;
    return false;
   }
  }
 }
 return true;
}

void CEagleLibraryManager::showHelpMessage(const QString& keyword)
{
 if (!m_ShellActive)
 {
  std::cout<<"Usage syntax: Console <command> [<options>]\n\n";
 }
 std::cout<<std::endl;
}

void CEagleLibraryManager::shell(void)
{
 if (m_ShellActive) return;
 m_ShellActive = true;
 showHelpMessage();
 while (m_ShellActive)
 {
  std::string command_str;
  std::cout<<std::endl<<"Console> ";
  std::getline(std::cin, command_str);
  while (command_str.empty())
  {
   std::cin.ignore();
   std::cin.clear();
   std::getline(std::cin, command_str);
  }
  executeCommand(QString::fromStdString(command_str));
 }
}

void CEagleLibraryManager::notImplemented(const QString& feature)
{
 std::cout<<"Not implemented ("<<feature.toStdString()<<")."<<std::endl;
}

void CEagleLibraryManager::show(const QString& source)
{
 m_Source.loadFromFile(source);
 m_Source.show(std::cout);
}

void CEagleLibraryManager::rewrite(const QString& source, const QString& target)
{
 m_Source.loadFromFile(source);
 m_Source.version().assign(m_TargetVersion);
 m_Source.setWriteDefaults(false);
 m_Source.saveToFile(target);
}

void CEagleLibraryManager::rewriteAll(const QString& source, const QString& target)
{
 m_Source.loadFromFile(source);
 m_Source.version().assign(m_TargetVersion);
 m_Source.setWriteDefaults(true);
 m_Source.saveToFile(target);
}

void CEagleLibraryManager::scale(const QString& source, const QString& target, const double value)
{
 m_Source.loadFromFile(source);
 // scale components by factor 'value'
 {
  m_Source.drawing().library().scalePackages(value);
  m_Source.drawing().board().scale(value);
  m_Source.drawing().schematic().scalePackages(value);
 }
 //
 Eagle::CNote *n = new Eagle::CNote();
 n->setNote(QString("Scaled %1 by %2 from %3")
 .arg((value>=1.0)?"up":"down").arg(value).arg(source));
 m_Source.preNotes().notes().append(n);
 //
 m_Source.version().assign(m_TargetVersion);
 m_Source.setWriteDefaults(false);
 m_Source.saveToFile(target);
}

void CEagleLibraryManager::changeLibrarySuffix(const QString& source, const QString& target,
 const QString& oldstr, const QString& newstr)
{
 QString oldstr_nq = unquote(oldstr);
 QString newstr_nq = unquote(newstr);
 m_Source.loadFromFile(source);
 //
 if (oldstr_nq.isEmpty())
 {
  {
   QList<Eagle::CLibrary*>& ll = m_Source.drawing().schematic().libraries();
   for (int i = 0; i < ll.size(); i++)
   {
    Eagle::CLibrary* l = ll.at(i);
    l->name().append(newstr_nq);
   }
   QList<Eagle::CPart*>& pl = m_Source.drawing().schematic().parts();
   for (int i = 0; i < pl.size(); i++)
   {
    Eagle::CPart* p = pl.at(i);
    p->setLibrary(p->library().append(newstr_nq));
   }
  }
  {
   QList<Eagle::CLibrary*>& ll = m_Source.drawing().board().libraries();
   for (int i = 0; i < ll.size(); i++)
   {
    Eagle::CLibrary* l = ll.at(i);
    l->name().append(newstr_nq);
   }
   QList<Eagle::CElement*>& el = m_Source.drawing().board().elements();
   for (int i = 0; i < el.size(); i++)
   {
    Eagle::CElement* e = el.at(i);
    e->setLibrary(e->library().append(newstr_nq));
   }
  }
 }
 else
 {
  {
   QList<Eagle::CLibrary*>& ll = m_Source.drawing().schematic().libraries();
   for (int i = 0; i < ll.size(); i++)
   {
    Eagle::CLibrary* l = ll.at(i);
    if (l->name().endsWith(oldstr_nq))
    {
     l->name().replace(oldstr_nq,newstr_nq);
    }
   }
   QList<Eagle::CPart*>& pl = m_Source.drawing().schematic().parts();
   for (int i = 0; i < pl.size(); i++)
   {
    Eagle::CPart* p = pl.at(i);
    p->setLibrary(p->library().replace(oldstr_nq,newstr_nq));
   }
  }
  {
   QList<Eagle::CLibrary*>& ll = m_Source.drawing().board().libraries();
   for (int i = 0; i < ll.size(); i++)
   {
    Eagle::CLibrary* l = ll.at(i);
    if (l->name().endsWith(oldstr_nq))
    {
     l->name().replace(oldstr_nq,newstr_nq);
    }
   }
   QList<Eagle::CElement*>& el = m_Source.drawing().board().elements();
   for (int i = 0; i < el.size(); i++)
   {
    Eagle::CElement* e = el.at(i);
    e->setLibrary(e->library().replace(oldstr_nq,newstr_nq));
   }
  }
 }
 //
 m_Source.version().assign(m_TargetVersion);
 m_Source.setWriteDefaults(false);
 m_Source.saveToFile(target);
}

//------------------------------------------------------------------------------
