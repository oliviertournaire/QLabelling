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

#ifndef __QARRANGEMENT_LABELLING_OVERLAY_DIALOG_H__
#define __QARRANGEMENT_LABELLING_OVERLAY_DIALOG_H__

#include <QDialog>
#include <vector>
#include <CGAL/Object.h>

class QArrangementLabellingWindow;
class QListWidgetItem;

namespace Ui { class QArrangementLabellingOverlayDialog; }

class QArrangementLabellingOverlayDialog : public QDialog
{
    Q_OBJECT

public:
    typedef enum OverlayDialogRole {
        ARRANGEMENT = 32
    } OverlayDialogRole;

    QArrangementLabellingOverlayDialog( QArrangementLabellingWindow* parent, Qt::WindowFlags f = 0 );

    std::vector< CGAL::Object > selectedArrangements( ) const;

public slots:
    void on_pickPushButton_pressed( );
    void on_unpickPushButton_pressed( );

protected:
    void restrictSelection( QListWidgetItem* item );
    void unrestrictSelection( );

    Ui::QArrangementLabellingOverlayDialog* ui;
};

#endif // __QARRANGEMENT_LABELLING_OVERLAY_DIALOG_H__
