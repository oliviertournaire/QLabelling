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

#include "QArrangementLabellingOverlayDialog.h"
#include "QArrangementLabellingWindow.h"
#include "ArrangementTypes.h"

#include <QListWidgetItem>
#include <QString>
#include <QItemSelectionModel>

#include "ui_QArrangementLabellingOverlayDialog.h"

// TODO: Don't color the text, but set an icon for each arrangement type...
// TODO: Or maybe don't bother
QArrangementLabellingOverlayDialog::QArrangementLabellingOverlayDialog( QArrangementLabellingWindow* parent, Qt::WindowFlags f ) :
    QDialog( parent, f ),
    ui( new Ui::QArrangementLabellingOverlayDialog )
{
    // An extra parenthesis around QColor to avoid the
    // http://en.wikipedia.org/wiki/Most_vexing_parse
    // on clang
    QBrush segColor( ( QColor( ::Qt::red ) ) );
    QBrush polyColor( ( QColor( ::Qt::darkGreen ) ) );
    this->ui->setupUi( this );

    std::vector< QString > labels = parent->getTabLabels( );
    std::vector< CGAL::Object > arrangements = parent->getArrangements( );
    
    for ( unsigned int i = 0; i < labels.size( ); ++i )
    {
        QListWidgetItem* item =
                new QListWidgetItem( this->ui->arrangementsListWidget );
        item->setText( labels[ i ] );
        item->setData( ARRANGEMENT, QVariant::fromValue( arrangements[ i ] ) );
        QIcon icon;
        Pol_arr* pol;
        if ( CGAL::assign( pol, arrangements[ i ] ) )
        {
            icon.addFile(QString::fromUtf8(":/icons/yellow_icon.xpm"), QSize(),
                         QIcon::Normal, QIcon::Off);
        }
        item->setIcon( icon );
    }
}

std::vector< CGAL::Object >
QArrangementLabellingOverlayDialog::selectedArrangements( ) const
{
    std::vector< CGAL::Object > res;
    for ( int i = 0; i < this->ui->overlayListWidget->count( ); ++i )
    {
        QListWidgetItem* item = this->ui->overlayListWidget->item( i );
        QVariant data = item->data( ARRANGEMENT );
        CGAL::Object arr = data.value< CGAL::Object >( );
        res.push_back( arr );
    }
    return res;
}

void QArrangementLabellingOverlayDialog::on_pickPushButton_pressed( )
{
    int currentIndex = this->ui->arrangementsListWidget->currentRow( );
    if ( currentIndex == -1 )
        return;

    if ( !(this->ui->arrangementsListWidget->item( currentIndex )->flags( ) &
           Qt::ItemIsEnabled) )
    {
        return;
    }
    QListWidgetItem* takenItem = this->ui->arrangementsListWidget->takeItem( currentIndex );
    this->ui->overlayListWidget->addItem( takenItem );

    if ( this->ui->overlayListWidget->count( ) == 2 )
    {
        this->ui->pickPushButton->setEnabled( false );
    }
    else
    {
        this->ui->pickPushButton->setEnabled( true );
    }
    QItemSelectionModel* selectionModel = this->ui->arrangementsListWidget->selectionModel( );
    selectionModel->clearSelection( );
    this->restrictSelection( takenItem );
}

void QArrangementLabellingOverlayDialog::on_unpickPushButton_pressed( )
{
    int currentIndex = this->ui->overlayListWidget->currentRow( );
    if ( currentIndex == -1 )
        return;

    QListWidgetItem* takenItem = this->ui->overlayListWidget->takeItem( currentIndex );
    this->ui->arrangementsListWidget->addItem( takenItem );

    if ( this->ui->overlayListWidget->count( ) == 2 )
    {
        this->ui->pickPushButton->setEnabled( false );
    }
    else
    {
        this->ui->pickPushButton->setEnabled( true );
    }
    if ( this->ui->overlayListWidget->count( ) == 0 )
        this->unrestrictSelection( );
}

void QArrangementLabellingOverlayDialog::restrictSelection( QListWidgetItem* item )
{
    CGAL::Object o = item->data( ARRANGEMENT ).value< CGAL::Object >( );
    Pol_arr* pol;
    if ( CGAL::assign( pol, o ) )
    {
        for ( int i = 0; i < this->ui->arrangementsListWidget->count( ); ++i )
        {
            QListWidgetItem* otherItem = this->ui->arrangementsListWidget->item( i );
            CGAL::Object o2 = otherItem->data( ARRANGEMENT ).value< CGAL::Object >( );
            bool enabled = CGAL::assign( pol, o2 );
            Qt::ItemFlags flags = otherItem->flags( );
            if ( ! enabled )
            {
                flags &= ~( Qt::ItemIsEnabled );
            }
            else
            {
                flags |= Qt::ItemIsEnabled;
            }
            otherItem->setFlags( flags );
        }
    }
}

void QArrangementLabellingOverlayDialog::unrestrictSelection( )
{
    for ( int i = 0; i < this->ui->arrangementsListWidget->count( ); ++i )
    {
        QListWidgetItem* otherItem = this->ui->arrangementsListWidget->item( i );
        Qt::ItemFlags flags = otherItem->flags( );
        flags |= Qt::ItemIsEnabled;
        otherItem->setFlags( flags );
    }
}
