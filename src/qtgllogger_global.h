#pragma execution_character_set("utf-8")

#ifndef QTGLLOGGER_GLOBAL_H
#define QTGLLOGGER_GLOBAL_H

#include <QtCore/qglobal.h>

#include <QDebug>
#include <QMetaEnum>


#if defined(QTGLLOGGER_LIBRARY)
#define QTGLLOGGER_EXPORT Q_DECL_EXPORT
#else
#define QTGLLOGGER_EXPORT Q_DECL_IMPORT
#endif


#define QGL_LOGGER_OBJECT_EXTERN extern QGL::QtGllogger &qgl; // 对于命名空间
#define QGL_LOGGER_OBJECT_STATIC static QGL::QtGllogger &qgl; // 对于类
#define QGL_LOGGER_OBJECT_DEFINE(moduleName) QGL::QtGllogger &moduleName::qgl = QGL::_QGL_GET_LOGGER(moduleName);

// #define QGL_LOGGER_OBJECT_DEFINE_FOR_FILE \
//     const QString &_qgl_file_full_path = QStringLiteral(__FILE__); \
//     qint32 _qgl_file_name_length = _qgl_file_full_path.length() - _qgl_file_full_path.lastIndexOf(QRegExp(QStringLiteral("[/\\\\]"))) - 1; \
//     if (_qgl_file_name_length <= 0) { \
//         _qgl_file_name_length = 0; \
//     } \
//     QGL::QtGllogger &qgl = QGL::QtGllogger::getLogger(_qgl_file_full_path.right(_qgl_file_name_length));
// #define QGL_LOGGER_OBJECT_DEFINE_FOR_OTHER_FILE_PAER1 QGL::QtGllogger &qgl = QGL::QtGllogger::getLogger("");
// // #ifndef QGL_LOGGER_OBJECT_DEFINE_FOR_OTHER_FILE_PAER2
// #define QGL_LOGGER_OBJECT_DEFINE_FOR_OTHER_FILE_PAER2 \
//     class _qgl_temp_class_for_creating { \
//       public: \
//         _qgl_temp_class_for_creating() { \
//             const QString &_qgl_file_full_path = QStringLiteral(__FILE__); \
//             qint32 _qgl_file_name_length = _qgl_file_full_path.length() - _qgl_file_full_path.lastIndexOf(QRegExp(QStringLiteral("[/\\\\]"))) - 1; \
//             if (_qgl_file_name_length <= 0) { \
//                 _qgl_file_name_length = 0; \
//             } \
//             qgl = QGL::QtGllogger::getLogger(_qgl_file_full_path.right(_qgl_file_name_length)); \
//         } \
//     }; \
//     static _qgl_temp_class_for_creating _qgl_temp_class;
// // #endif
#define QGL_LOGGER_OBJECT_DEFINE_FOR_FILE(fileTreeLevel) QGL::QtGllogger &qgl = QGL::QtGllogger::getLogger_for_file(__FILE__, fileTreeLevel);
#define QGL_LOGGER_OBJECT_DEFINE_STATIC_FOR_FILE(fileTreeLevel) \
    static QGL::QtGllogger &qgl = QGL::QtGllogger::getLogger_for_file(__FILE__, fileTreeLevel); // 只能加在cpp文件里,写在头文件里会导致重定义


#define _QGL_LOG_EXTINFO_FUNCTION_NAME QString::fromStdString(std::string(__func__))  // 附加函数名称
#define _QGL_LOG_EXTINFO_LINE_NUMBER QString::fromStdString(std::to_string(__LINE__)) // 行数
#define QGL_TRACE(...) qgl.trace_ext(QStringList{_QGL_LOG_EXTINFO_FUNCTION_NAME, _QGL_LOG_EXTINFO_LINE_NUMBER}, __VA_ARGS__)
#define QGL_DEBUG(...) qgl.debug_ext(QStringList{_QGL_LOG_EXTINFO_FUNCTION_NAME, _QGL_LOG_EXTINFO_LINE_NUMBER}, __VA_ARGS__)
#define QGL_INFO(...) qgl.info_ext(QStringList{_QGL_LOG_EXTINFO_FUNCTION_NAME, _QGL_LOG_EXTINFO_LINE_NUMBER}, __VA_ARGS__)
#define QGL_WARNING(...) qgl.warning_ext(QStringList{_QGL_LOG_EXTINFO_FUNCTION_NAME, _QGL_LOG_EXTINFO_LINE_NUMBER}, __VA_ARGS__)
#define QGL_ERROR(...) qgl.error_ext(QStringList{_QGL_LOG_EXTINFO_FUNCTION_NAME, _QGL_LOG_EXTINFO_LINE_NUMBER}, __VA_ARGS__)
// ;logger.blank(LOG::Level::Info)

#define _QGL_GET_LOGGER(moduleName) QtGllogger::getLogger(#moduleName)


// 注意宏是在预处理阶段展开, 写在命名空间里也限制不了作用域
#define QGL_LINE_LINE_BEGIN(level) \
    QGL::qgl.blank(level); \
    QGL::qgl.blank(level); \
    QGL::qgl.line(level);
#define QGL_LINE_LINE_END(level) QGL::qgl.line(level);


// 预特化一下,避免需要同时传类型和值
#define QGL_E2S(EnumValue) QGL::e2s(EnumValue, QStringLiteral(#EnumValue).mid(0, QStringLiteral(#EnumValue).lastIndexOf("::"))) // 用此方法就不需要特化了, 但只能处理字面值,对变量就不行了, 还是特化吧
#define QGL_E2S_LASTPART(EnumValue) QStringLiteral(#EnumValue).mid(QStringLiteral(#EnumValue).lastIndexOf("::") + 2)


// 使枚举可被qdebug打印
// 不要定义为inline,inline也会被展开,extern算是没生效,导致多个cpp文件都需要定义
#define QGL_E2D(EnumType, ...) \
    QDebug operator<<(QDebug debug, const EnumType &value) { \
        QDebugStateSaver saver(debug); \
        static const char *names[] = {__VA_ARGS__}; \
        int index = static_cast<int>(value); \
        const char *enumName = #EnumType; \
        if (index >= 0 && index < sizeof(names) / sizeof(names[0])) { \
            debug.nospace() << enumName << "::" << names[index]; \
        } else { \
            debug.nospace() << enumName << "::??"; \
        } \
        debug.space(); \
        return debug; \
    };

#define QGL_E2D_BIT(EnumType, ...) \
    QDebug operator<<(QDebug debug, const EnumType &value) { \
        QDebugStateSaver saver(debug); \
        static const QList<QGL::eban> &bit_names = {__VA_ARGS__}; \
        const char *enumName = #EnumType; \
        foreach (const QGL::eban &i, bit_names) { \
            if (i.first == static_cast<quint32>(value)) { \
                debug.nospace() << enumName << "::" << i.second.toUtf8().constData(); \
                debug.space(); \
                return debug; \
            } \
        } \
        debug.nospace() << enumName << "::??"; \
        debug.space(); \
        return debug; \
    };


// 使类可以被qdebug打印,也打印对象的属性
// << ", " << __VA_ARGS__
#define QGL_C2D(Class, ...) \
    QDebug operator<<(QDebug d, const Class &o) { \
        QDebugStateSaver saver(d); \
        d.nospace() << #Class "(" << static_cast<const void *>(&o); \
        __VA_ARGS__; \
        d.nospace() << ")"; \
        d.space(); \
        return d; \
    };
#define QGL_C2D_FAS(attrible) \
    d.nospace().noquote() << ", " << QStringLiteral(#attrible).mid(2) << "="; \
    d.quote() << attrible

#define QGL_FAS(var) qUtf8Printable(QString(#var "=%1").arg(var))


#define QGL_ENUM_TO_STRING(EnumType, i) QGL::e2s(i, #EnumType)
#define QGL_ENUM_TO_STRING_SPECIAL(EnumType) \
    template<> \
    QString QGL::e2s(const EnumType &i) { \
        return QGL_ENUM_TO_STRING(EnumType, i); \
    };

namespace QGL {

Q_NAMESPACE

typedef QPair<quint32, QString> eban; // enum bit and name

enum class QTGLLOGGER_EXPORT Level { Trace, Debug, Info, Warning, Error, Critical };
Q_ENUM_NS(Level)


// 使枚举可被用于qstring.arg()
template<typename EnumType>
QString e2s(const EnumType &i, const QString &namespaceName) { // 将枚举转为可读字符串
    const QMetaEnum metaEnum = QMetaEnum::fromType<EnumType>();
    // typeid(EnumType).name() 结果里的::被替换了, 所以下面使用模板特化实现
    return QString("%1::%2").arg(namespaceName, metaEnum.valueToKey(static_cast<qint32>(i)));
}
template<typename EnumType>
QString e2s(const EnumType &i) {
    return QGL_ENUM_TO_STRING(EnumType, i);
}
template<typename EnumType>
QString e2s_lastpart(const EnumType &i) {
    const QString &fullname = QGL::e2s(i);
    return fullname.mid(fullname.lastIndexOf("::") + 2);
}
template<> // 特化模板声明, 实现要写到cpp文件里
QTGLLOGGER_EXPORT QString e2s(const QGL::Level &i);


} // namespace QGL


#endif // QTGLLOGGER_GLOBAL_H
