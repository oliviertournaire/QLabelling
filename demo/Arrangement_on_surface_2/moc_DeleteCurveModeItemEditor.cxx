/****************************************************************************
** Meta object code from reading C++ file 'DeleteCurveModeItemEditor.h'
**
** Created: Mon May 27 19:05:56 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../demo/Arrangement_on_surface_2/DeleteCurveModeItemEditor.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DeleteCurveModeItemEditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DeleteCurveModeItemEditor[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       1,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
      42,   26, 0x0019510b,

       0        // eod
};

static const char qt_meta_stringdata_DeleteCurveModeItemEditor[] = {
    "DeleteCurveModeItemEditor\0DeleteCurveMode\0"
    "mode\0"
};

void DeleteCurveModeItemEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData DeleteCurveModeItemEditor::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DeleteCurveModeItemEditor::staticMetaObject = {
    { &QComboBox::staticMetaObject, qt_meta_stringdata_DeleteCurveModeItemEditor,
      qt_meta_data_DeleteCurveModeItemEditor, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DeleteCurveModeItemEditor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DeleteCurveModeItemEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DeleteCurveModeItemEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DeleteCurveModeItemEditor))
        return static_cast<void*>(const_cast< DeleteCurveModeItemEditor*>(this));
    return QComboBox::qt_metacast(_clname);
}

int DeleteCurveModeItemEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QComboBox::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< DeleteCurveMode*>(_v) = mode(); break;
        }
        _id -= 1;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setMode(*reinterpret_cast< DeleteCurveMode*>(_v)); break;
        }
        _id -= 1;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 1;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
