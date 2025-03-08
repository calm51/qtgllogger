#pragma execution_character_set("utf-8")

#ifndef QTGLLOGGER_H
#define QTGLLOGGER_H


#include <QDateTime>
#include <QDebug>
#include <QMutex>
#include <QStringList>

#include "qtgllogger_global.h"


namespace QGL {
class QtGllogger;
}
extern QDebug operator<<(QDebug debug, const QGL::QtGllogger &);


#define QGL_LOGGER_OBJECT_EXTERN extern QGL::QtGllogger &qgl; // 对于命名空间
#define QGL_LOGGER_OBJECT_STATIC static QGL::QtGllogger &qgl; // 对于类
#define QGL_LOGGER_OBJECT_DEFINE(moduleName) QGL::QtGllogger &moduleName::qgl = QGL::_QGL_GET_LOGGER(moduleName);

#define QGL_LOGGER_OBJECT_DEFINE_FOR_FILE \
    const QString &_qgl_file_full_path = QStringLiteral(__FILE__); \
    qint32 _qgl_file_name_length = _qgl_file_full_path.length() - _qgl_file_full_path.lastIndexOf(QRegExp(QStringLiteral("[/\\\\]"))) - 1; \
    if (_qgl_file_name_length <= 0) { \
        _qgl_file_name_length = 0; \
    } \
    QGL::QtGllogger &qgl = QGL::QtGllogger::getLogger(_qgl_file_full_path.right(_qgl_file_name_length));


#define _QGL_LOG_EXTINFO_FUNCTION_NAME QString::fromStdString(std::string(__func__))  // 附加函数名称
#define _QGL_LOG_EXTINFO_LINE_NUMBER QString::fromStdString(std::to_string(__LINE__)) // 行数
#define QGL_DEBUG(...) qgl.debug_ext(QStringList{_QGL_LOG_EXTINFO_FUNCTION_NAME, _QGL_LOG_EXTINFO_LINE_NUMBER}, __VA_ARGS__)
#define QGL_INFO(...) qgl.info_ext(QStringList{_QGL_LOG_EXTINFO_FUNCTION_NAME, _QGL_LOG_EXTINFO_LINE_NUMBER}, __VA_ARGS__)
#define QGL_WARNING(...) qgl.warning_ext(QStringList{_QGL_LOG_EXTINFO_FUNCTION_NAME, _QGL_LOG_EXTINFO_LINE_NUMBER}, __VA_ARGS__)
#define QGL_ERROR(...) qgl.error_ext(QStringList{_QGL_LOG_EXTINFO_FUNCTION_NAME, _QGL_LOG_EXTINFO_LINE_NUMBER}, __VA_ARGS__)
// ;logger.blank(LOG::Level::Info)


#define _QGL_GET_LOGGER(moduleName) QtGllogger::getLogger(#moduleName)


namespace QGL {

extern Level globalLogLevel;
extern QTGLLOGGER_EXPORT void setGlobalLogLevel(Level level);

extern QMap<QString, QtGllogger *> loggers;

class QTGLLOGGER_EXPORT QtGllogger {
  public:
    static QtGllogger &getLogger(const QString &module) {
        static QMutex mutex;
        QMutexLocker locker(&mutex);
        if (!loggers.contains(module)) {
            loggers[module] = new QtGllogger(module);
        }
        return *loggers[module];
    }
    explicit QtGllogger(); // qmap的[]操作需要声明个默认构造

    // QGL_LOGGER_OBJECT_STATIC

    ~QtGllogger();

    void setLogLevel(Level level) { this->logLevel = level; }

    void blank(Level level = Level::Debug) {
        if (level < logLevel || level < globalLogLevel) {
            return;
        }
        if (module.isEmpty()) {
            return;
        }
        qDebug() << "";
    }

    template<typename... Args>
    void debug(Args &&...args) {
        if (Level::Debug < logLevel || Level::Debug < globalLogLevel) {
            return;
        }
        if (module.isEmpty()) {
            return;
        }
        log(header(Level::Debug).quote().space(), std::forward<Args>(args)...);
    }

    template<typename... Args>
    void debug_ext(const QStringList &extinfos, Args &&...args) {
        if (Level::Debug < logLevel || Level::Debug < globalLogLevel) {
            return;
        }
        if (module.isEmpty()) {
            return;
        }
        log(header(Level::Debug, extinfos).quote().space(), std::forward<Args>(args)...);
    }

    template<typename... Args>
    void info(Args &&...args) {
        if (Level::Info < logLevel || Level::Info < globalLogLevel) {
            return;
        }
        if (module.isEmpty()) {
            return;
        }
        log(header(Level::Info).quote().space(), std::forward<Args>(args)...);
    }

    template<typename... Args>
    void info_ext(const QStringList &extinfos, Args &&...args) {
        if (Level::Info < logLevel || Level::Info < globalLogLevel) {
            return;
        }
        if (module.isEmpty()) {
            return;
        }
        log(header(Level::Info, extinfos).quote().space(), std::forward<Args>(args)...);
    }

    template<typename... Args>
    void warning(Args &&...args) {
        if (Level::Warning < logLevel || Level::Warning < globalLogLevel) {
            return;
        }
        if (module.isEmpty()) {
            return;
        }
        log(header(Level::Warning).quote().space(), std::forward<Args>(args)...);
    }

    template<typename... Args>
    void warning_ext(const QStringList &extinfos, Args &&...args) {
        if (Level::Warning < logLevel || Level::Warning < globalLogLevel) {
            return;
        }
        if (module.isEmpty()) {
            return;
        }
        log(header(Level::Warning, extinfos).quote().space(), std::forward<Args>(args)...);
    }

    template<typename... Args>
    void error(Args &&...args) {
        if (Level::Error < logLevel || Level::Error < globalLogLevel) {
            return;
        }
        if (module.isEmpty()) {
            return;
        }
        log(header(Level::Error).quote().space(), std::forward<Args>(args)...);
    }

    template<typename... Args>
    void error_ext(const QStringList &extinfos, Args &&...args) {
        if (Level::Error < logLevel || Level::Error < globalLogLevel) {
            return;
        }
        if (module.isEmpty()) {
            return;
        }
        log(header(Level::Error, extinfos).quote().space(), std::forward<Args>(args)...);
    }


    QString module;
    Level logLevel = Level::Debug;

  private:
    explicit QtGllogger(const QString &moduleName);

    QDebug header(Level level, const QStringList &extinfos = {}) {
        QDebug qdebug = qDebug().noquote().nospace() << "[" << logLevelToString(level) << ", " << QDateTime::currentDateTime().toString("hh:mm:ss") << ", " << module
                                                     << (!extinfos.isEmpty() ? ", " : "") << extinfos.join(", ") << "]";
        return qdebug;
    }
    template<typename T>
    void log(QDebug qdebug, T &&arg) {
        qdebug << std::forward<T>(arg);
    }
    template<typename T, typename... Args>
    void log(QDebug qdebug, T &&first, Args &&...args) {
        qdebug << std::forward<T>(first);
        log(qdebug, std::forward<Args>(args)...);
    }

    QString logLevelToString(Level level) const;

  signals:
};

} // namespace QGL


#include "qtgllogger_obj.h"


extern QDebug operator<<(QDebug debug, const QGL::Level &);

extern QDebug operator<<(QDebug debug, const QGL::QtGllogger &);


#endif // QTGLLOGGER_H
