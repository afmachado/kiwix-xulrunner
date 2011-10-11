/*
 * Copyright 2011 Emmanuel Engelhart <kelson@kiwix.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU  General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#ifndef KIWIX_MANAGER_H
#define KIWIX_MANAGER_H

#include <map>
#include <string>
#include <sstream>
#include <time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>

#include <pugixml.hpp>

#include "../base64.h"
#include <kiwix/library.h>
#include <kiwix/reader.h>

using namespace std;

namespace kiwix {

  enum supportedListMode { LASTOPEN, REMOTE, LOCAL };
  enum supportedListSortBy { TITLE, SIZE, DATE, PUBLISHER };

  class Manager {
    
  public:
    Manager();
    ~Manager();

    bool readFile(const string path, const bool readOnly = true);
    bool readXml(const string xml, const bool readOnly = true, const string libraryPath = "");
    bool writeFile(const string path);
    bool removeBookByIndex(const unsigned int bookIndex);
    bool removeBookById(const string id);
    bool setCurrentBookId(const string id);
    bool setBookIndex(const string id, const string path, const supportedIndexType type);
    bool setBookPath(const string id, const string path);
    string getCurrentBookId();
    bool addBookFromPath(const string pathToOpen, const string pathToSave = "", const string url = "", const bool checkMetaData = false);
    Library cloneLibrary();
    bool getBookById(const string id, Book &book);
    unsigned int getBookCount(const bool localBooks, const bool remoteBooks);
    bool updateBookLastOpenDateById(const string id);
    void removeBookPaths();
    bool listBooks(const supportedListMode mode, const supportedListSortBy sortBy, const unsigned int maxSize);
    vector<string> getBooksLanguages();
    vector<string> getBooksPublishers();

    string writableLibraryPath;

    vector<std::string> bookIdList;
    
  protected:
    kiwix::Library library;
    
    bool readBookFromPath(const string path, Book &book);
    bool parseXmlDom(const pugi::xml_document &doc, const bool readOnly, const string libraryPath);
    bool isRelativePath(const string &path);
    string computeAbsolutePath(const string libraryPath, const string relativePath);
    string removeLastPathElement(const string path, const bool removePreSeparator = false, 
				 const bool removePostSeparator = false);
  };

}

#endif
