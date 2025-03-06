#pragma execution_character_set("utf-8")

#ifndef QTGLLOGGER_GLOBAL_H
#define QTGLLOGGER_GLOBAL_H

#include <QtCore/qglobal.h>

#include <QDebug>
#include <QMetaEnum>


namespace QGL {

#if defined(QTGLLOGGER_LIBRARY)
#define QTGLLOGGER_EXPORT Q_DECL_EXPORT
#else
#define QTGLLOGGER_EXPORT Q_DECL_IMPORT
#endif


Q_NAMESPACE


enum class QTGLLOGGER_EXPORT Level {
    Debug,
    Info,
    Warning,
    Error,
    Critical
};
Q_ENUM_NS(Level)


// 注意宏是在预处理阶段展开, 写在命名空间里也限制不了作用域
#define QGL_LINE_LINE_BEGIN QGL::qgl.blank(); QGL::qgl.blank(); QGL::qgl.debug(QStringLiteral("==============================================="));
#define QGL_LINE_LINE_END QGL::qgl.debug(QStringLiteral("==============================================="));


// 使枚举可被用于qstring.arg()
#define QGL_ENUM_TO_STRING(EnumType, i) QGL::ets(i, #EnumType)
#define QGL_ENUM_TO_STRING_SPECIAL(EnumType) \
template <> \
    QString QGL::ets(const EnumType &i) { \
        return QGL_ENUM_TO_STRING(EnumType, i); \
}
template <typename EnumType>
QString ets(const EnumType &i, const QString& namespaceName) { // 将枚举转为可读字符串
    const QMetaEnum metaEnum = QMetaEnum::fromType<EnumType>();
    // typeid(EnumType).name() 结果里的::被替换了, 所以下面使用模板特化实现
    return QString("%1::%2").arg(namespaceName, metaEnum.valueToKey(static_cast<qint32>(i)));
}
template <typename EnumType>
QString ets(const EnumType &i) {
    return QGL_ENUM_TO_STRING(EnumType, i);
}
template <typename EnumType>
QString ets_lastpart(const EnumType &i){
    const QString &fullname = QGL::ets(i);
    return fullname.mid(fullname.lastIndexOf("::")+2);
}
template <> // 特化模板声明, 实现要写到cpp文件里
QTGLLOGGER_EXPORT QString ets(const QGL::Level &i); // 预特化一下,避免需要同时传类型和值
#define QGL_E2S(EnumValue) QGL::ets(EnumValue, QStringLiteral(#EnumValue).mid(0, QStringLiteral(#EnumValue).lastIndexOf("::"))) // 用此方法就不需要特化了, 但只能处理字面值,对变量就不行了, 还是特化吧
#define QGL_E2S_LASTPART(EnumValue) QStringLiteral(#EnumValue).mid(QStringLiteral(#EnumValue).lastIndexOf("::")+2)



// 使枚举可被qdebug打印
// 不要定义为inline,inline也会被展开,extern算是没生效,导致多个cpp文件都需要定义
#define QGL_E2D(EnumType, ...) \
QDebug operator<<(QDebug debug, const EnumType &value) { \
        QDebugStateSaver saver(debug); \
        static const char* names[] = {__VA_ARGS__}; \
        int index = static_cast<int>(value); \
        const char* enumName = #EnumType; \
        if (index >= 0 && index < sizeof(names) / sizeof(names[0])) { \
            debug.nospace() << enumName << "::" << names[index]; \
    } else { \
            debug.nospace() << enumName << "::??"; \
    } \
        debug.space(); \
        return debug; \
}

typedef QPair<quint32, QString> eban;  // enum bit and name
#define QGL_E2D_BIT(EnumType, ...) \
QDebug operator<<(QDebug debug, const EnumType &value) { \
        QDebugStateSaver saver(debug); \
        static const QList<QGL::eban> &bit_names = {__VA_ARGS__}; \
        const char* enumName = #EnumType; \
        foreach (const QGL::eban &i, bit_names) { \
            if (i.first == static_cast<quint32>(value)){ \
                debug.nospace() << enumName << "::" << i.second.toUtf8().constData(); \
                debug.space(); \
                return debug; \
        } \
    } \
        debug.nospace() << enumName << "::??"; \
        debug.space(); \
        return debug; \
}


// 使类可以被qdebug打印,也打印对象的属性
// << ", " << __VA_ARGS__
#define QGL_C2D(Class, ...) QDebug operator<<(QDebug d, const Class& o) { \
QDebugStateSaver saver(d); \
    d.nospace() << #Class"(" << &o; \
    __VA_ARGS__; \
    d.nospace() << ")"; \
    d.space(); \
    return d;}
#define QGL_C2D_FAS(attrible) d.nospace().noquote() << ", " << QStringLiteral(#attrible).mid(2) << "="; d.quote() << attrible



}



#endif // QTGLLOGGER_GLOBAL_H
