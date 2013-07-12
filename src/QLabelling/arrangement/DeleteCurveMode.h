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

#ifndef DELETE_CURVE_MODE_H
#define DELETE_CURVE_MODE_H

#include <QMetaType>

class QString;

/**
An attribute describing the policy for deleting curves from the arrangement.
*/
class DeleteCurveMode
{
public:
    enum Mode
    {
        DELETE_CURVE,
        DELETE_EDGE
    };

    DeleteCurveMode() : m_mode( DELETE_EDGE ) {}
    DeleteCurveMode( const DeleteCurveMode& dcm ) : m_mode( dcm.mode( ) ) {}
    DeleteCurveMode( Mode mode ) : m_mode( mode ) {}
    ~DeleteCurveMode() {}

    inline Mode mode() const { return m_mode; }
    inline void setMode( Mode mode ) { m_mode = mode; }

    static QString ToString( const DeleteCurveMode& mode ) { return ( mode.mode( ) == DELETE_CURVE ) ? QObject::tr("Delete Curve") : QObject::tr("Delete Edge"); }

protected:
    Mode m_mode;
};

Q_DECLARE_METATYPE( DeleteCurveMode )

#endif // DELETE_CURVE_MODE_H
