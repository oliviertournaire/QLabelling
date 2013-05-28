/****************************************************************************
** Meta object code from reading C++ file 'Callback.h'
**
** Created: Mon May 27 19:05:54 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../demo/Arrangement_on_surface_2/Callback.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Callback.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CGAL__Qt__Callback[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      20,   19,   19,   19, 0x05,

 // slots: signature, parameters, type, tag, flags
      35,   19,   19,   19, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CGAL__Qt__Callback[] = {
    "CGAL::Qt::Callback\0\0modelChanged()\0"
    "slotModelChanged()\0"
};

void CGAL::Qt::Callback::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Callback *_t = static_cast<Callback *>(_o);
        switch (_id) {
        case 0: _t->modelChanged(); break;
        case 1: _t->slotModelChanged(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData CGAL::Qt::Callback::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CGAL::Qt::Callback::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CGAL__Qt__Callback,
      qt_meta_data_CGAL__Qt__Callback, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CGAL::Qt::Callback::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CGAL::Qt::Callback::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CGAL::Qt::Callback::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CGAL__Qt__Callback))
        return static_cast<void*>(const_cast< Callback*>(this));
    if (!strcmp(_clname, "QGraphicsSceneMixin"))
        return static_cast< QGraphicsSceneMixin*>(const_cast< Callback*>(this));
    return QObject::qt_metacast(_clname);
}

int CGAL::Qt::Callback::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void CGAL::Qt::Callback::modelChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
