/****************************************************************************
** Meta object code from reading C++ file 'ArrangementDemoWindow.h'
**
** Created: Mon May 27 19:05:47 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../demo/Arrangement_on_surface_2/ArrangementDemoWindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ArrangementDemoWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ArrangementDemoWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      23,   22,   22,   22, 0x05,

 // slots: signature, parameters, type, tag, flags
      40,   38,   22,   22, 0x0a,
      61,   38,   22,   22, 0x0a,
      86,   38,   22,   22, 0x0a,
     111,   38,   22,   22, 0x0a,
     137,   22,   22,   22, 0x0a,
     165,   22,   22,   22, 0x0a,
     193,   22,   22,   22, 0x0a,
     219,   22,   22,   22, 0x0a,
     245,   22,   22,   22, 0x0a,
     275,   22,   22,   22, 0x0a,
     304,   22,   22,   22, 0x0a,
     334,   22,   22,   22, 0x0a,
     372,   22,   22,   22, 0x0a,
     400,   22,   22,   22, 0x0a,
     429,   22,   22,   22, 0x0a,
     462,   22,   22,   22, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ArrangementDemoWindow[] = {
    "ArrangementDemoWindow\0\0modelChanged()\0"
    "a\0updateMode(QAction*)\0updateEnvelope(QAction*)\0"
    "updateSnapping(QAction*)\0"
    "updateConicType(QAction*)\0"
    "on_actionNewTab_triggered()\0"
    "on_actionSaveAs_triggered()\0"
    "on_actionOpen_triggered()\0"
    "on_actionQuit_triggered()\0"
    "on_tabWidget_currentChanged()\0"
    "on_actionOverlay_triggered()\0"
    "on_actionCloseTab_triggered()\0"
    "on_actionPrintConicCurves_triggered()\0"
    "on_actionZoomIn_triggered()\0"
    "on_actionZoomOut_triggered()\0"
    "on_actionPreferences_triggered()\0"
    "on_actionFillColor_triggered()\0"
};

void ArrangementDemoWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ArrangementDemoWindow *_t = static_cast<ArrangementDemoWindow *>(_o);
        switch (_id) {
        case 0: _t->modelChanged(); break;
        case 1: _t->updateMode((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 2: _t->updateEnvelope((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 3: _t->updateSnapping((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 4: _t->updateConicType((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 5: _t->on_actionNewTab_triggered(); break;
        case 6: _t->on_actionSaveAs_triggered(); break;
        case 7: _t->on_actionOpen_triggered(); break;
        case 8: _t->on_actionQuit_triggered(); break;
        case 9: _t->on_tabWidget_currentChanged(); break;
        case 10: _t->on_actionOverlay_triggered(); break;
        case 11: _t->on_actionCloseTab_triggered(); break;
        case 12: _t->on_actionPrintConicCurves_triggered(); break;
        case 13: _t->on_actionZoomIn_triggered(); break;
        case 14: _t->on_actionZoomOut_triggered(); break;
        case 15: _t->on_actionPreferences_triggered(); break;
        case 16: _t->on_actionFillColor_triggered(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ArrangementDemoWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ArrangementDemoWindow::staticMetaObject = {
    { &CGAL::Qt::DemosMainWindow::staticMetaObject, qt_meta_stringdata_ArrangementDemoWindow,
      qt_meta_data_ArrangementDemoWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ArrangementDemoWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ArrangementDemoWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ArrangementDemoWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ArrangementDemoWindow))
        return static_cast<void*>(const_cast< ArrangementDemoWindow*>(this));
    typedef CGAL::Qt::DemosMainWindow QMocSuperClass;
    return QMocSuperClass::qt_metacast(_clname);
}

int ArrangementDemoWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    typedef CGAL::Qt::DemosMainWindow QMocSuperClass;
    _id = QMocSuperClass::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    }
    return _id;
}

// SIGNAL 0
void ArrangementDemoWindow::modelChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
