/****************************************************************************
** Meta object code from reading C++ file 'backend.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../backend.h"
#include <QtNetwork/QSslError>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'backend.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.11.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN7backEndE_t {};
} // unnamed namespace

template <> constexpr inline auto backEnd::qt_create_metaobjectdata<qt_meta_tag_ZN7backEndE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "backEnd",
        "sourceAddedSuccess",
        "",
        "sourceAddedError",
        "message",
        "searchResultsChanged",
        "init",
        "addSource",
        "url",
        "getSources",
        "QList<QMap<QString,QVariant>>",
        "deleteSource",
        "id",
        "searchMusic",
        "search",
        "setInfosTo",
        "name",
        "artist",
        "cover",
        "getName",
        "getArtist",
        "getCover",
        "searchResults",
        "QVariantList"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'sourceAddedSuccess'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'sourceAddedError'
        QtMocHelpers::SignalData<void(QString)>(3, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 4 },
        }}),
        // Signal 'searchResultsChanged'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'init'
        QtMocHelpers::MethodData<int()>(6, 2, QMC::AccessPublic, QMetaType::Int),
        // Method 'addSource'
        QtMocHelpers::MethodData<bool(QString)>(7, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 8 },
        }}),
        // Method 'getSources'
        QtMocHelpers::MethodData<QList<QMap<QString,QVariant>>()>(9, 2, QMC::AccessPublic, 0x80000000 | 10),
        // Method 'deleteSource'
        QtMocHelpers::MethodData<bool(const int)>(11, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::Int, 12 },
        }}),
        // Method 'searchMusic'
        QtMocHelpers::MethodData<void(const QString)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 14 },
        }}),
        // Method 'setInfosTo'
        QtMocHelpers::MethodData<void(const QString, const QString, const QString)>(15, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 16 }, { QMetaType::QString, 17 }, { QMetaType::QString, 18 },
        }}),
        // Method 'getName'
        QtMocHelpers::MethodData<QString()>(19, 2, QMC::AccessPublic, QMetaType::QString),
        // Method 'getArtist'
        QtMocHelpers::MethodData<QString()>(20, 2, QMC::AccessPublic, QMetaType::QString),
        // Method 'getCover'
        QtMocHelpers::MethodData<QString()>(21, 2, QMC::AccessPublic, QMetaType::QString),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'searchResults'
        QtMocHelpers::PropertyData<QVariantList>(22, 0x80000000 | 23, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 2),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<backEnd, qt_meta_tag_ZN7backEndE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject backEnd::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7backEndE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7backEndE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN7backEndE_t>.metaTypes,
    nullptr
} };

void backEnd::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<backEnd *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->sourceAddedSuccess(); break;
        case 1: _t->sourceAddedError((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->searchResultsChanged(); break;
        case 3: { int _r = _t->init();
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 4: { bool _r = _t->addSource((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 5: { QList<QMap<QString,QVariant>> _r = _t->getSources();
            if (_a[0]) *reinterpret_cast<QList<QMap<QString,QVariant>>*>(_a[0]) = std::move(_r); }  break;
        case 6: { bool _r = _t->deleteSource((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 7: _t->searchMusic((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 8: _t->setInfosTo((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3]))); break;
        case 9: { QString _r = _t->getName();
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 10: { QString _r = _t->getArtist();
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 11: { QString _r = _t->getCover();
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (backEnd::*)()>(_a, &backEnd::sourceAddedSuccess, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (backEnd::*)(QString )>(_a, &backEnd::sourceAddedError, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (backEnd::*)()>(_a, &backEnd::searchResultsChanged, 2))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QVariantList*>(_v) = _t->getSearchResults(); break;
        default: break;
        }
    }
}

const QMetaObject *backEnd::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *backEnd::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7backEndE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int backEnd::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 12;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void backEnd::sourceAddedSuccess()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void backEnd::sourceAddedError(QString _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void backEnd::searchResultsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
