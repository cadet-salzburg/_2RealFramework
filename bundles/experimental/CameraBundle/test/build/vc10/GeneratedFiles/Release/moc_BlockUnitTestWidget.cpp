/****************************************************************************
** Meta object code from reading C++ file 'BlockUnitTestWidget.h'
**
** Created: Tue 10. Jul 20:32:26 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/BlockUnitTestWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'BlockUnitTestWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_BlockUnitTestWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      21,   20,   20,   20, 0x08,
      31,   20,   20,   20, 0x08,
      40,   20,   20,   20, 0x08,
      55,   20,   20,   20, 0x08,
      72,   20,   20,   20, 0x08,
      90,   84,   20,   20, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_BlockUnitTestWidget[] = {
    "BlockUnitTestWidget\0\0onStart()\0onStop()\0"
    "onSingleStep()\0onStopFinished()\0"
    "stopBlock()\0value\0setFpsValue(double)\0"
};

void BlockUnitTestWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        BlockUnitTestWidget *_t = static_cast<BlockUnitTestWidget *>(_o);
        switch (_id) {
        case 0: _t->onStart(); break;
        case 1: _t->onStop(); break;
        case 2: _t->onSingleStep(); break;
        case 3: _t->onStopFinished(); break;
        case 4: _t->stopBlock(); break;
        case 5: _t->setFpsValue((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData BlockUnitTestWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject BlockUnitTestWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_BlockUnitTestWidget,
      qt_meta_data_BlockUnitTestWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &BlockUnitTestWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *BlockUnitTestWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *BlockUnitTestWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_BlockUnitTestWidget))
        return static_cast<void*>(const_cast< BlockUnitTestWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int BlockUnitTestWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE