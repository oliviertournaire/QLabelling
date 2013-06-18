// Copyright (c) 2012  Tel-Aviv University (Israel).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
// You can redistribute it and/or modify it under the terms of the GNU
// General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL: $
// $Id: $
//
// Author(s)     : Alex Tsui <alextsui05@gmail.com>

#include <QApplication>
#include <QImageReader>
#include <QImageWriter>
#include <QObject>

#include "ArrangementDemoWindow.h"
#include "QLabellingLogWidget.hpp"

int main( int argc, char* argv[] )
{
    QApplication app( argc, argv );

#ifdef WIN32
    app.addLibraryPath(qgetenv ("QTDIR")+"\\plugins");
#endif

    ArrangementDemoWindow demoWindow;
    demoWindow.show( );

    QString readFileFormats = "(";
    for (int i = 0; i < QImageReader::supportedImageFormats().count(); i++)
    {
        readFileFormats += QString(QImageReader::supportedImageFormats().at(i)).toLower();
        readFileFormats += " ";
    }
    readFileFormats += ")";

    QLabellingLogWidget::instance()->logInfo( QObject::tr("Available image file formats (read): ") + readFileFormats );

    QString writeFileFormats = "(";
    for (int i = 0; i < QImageWriter::supportedImageFormats().count(); i++)
    {
        writeFileFormats += QString(QImageWriter::supportedImageFormats().at(i)).toLower();
        writeFileFormats += " ";
    }
    writeFileFormats += ")";

    QLabellingLogWidget::instance()->logInfo( QObject::tr("Available image file formats (write): ") + writeFileFormats );

    return app.exec( );
}
