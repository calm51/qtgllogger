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


namespace QGL {

extern QTGLLOGGER_EXPORT Level globalLogLevel;
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
    static QtGllogger &getLogger_for_file(const QString &filepath, const qint32 &file_tree_level) {
        QStringList sl = filepath.split(QRegExp(QStringLiteral("[/\\\\]+")));
        while (sl.length() > file_tree_level) {
            sl.removeFirst();
        }
        return getLogger(sl.join("/"));
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

    void line(Level level = Level::Debug) {
        if (level < logLevel || level < globalLogLevel) {
            return;
        }
        if (module.isEmpty()) {
            return;
        }
        qDebug() << QStringLiteral("===============================================");
    }

    template<typename... Args>
    void trace(Args &&...args) {
        if (Level::Trace < logLevel || Level::Trace < globalLogLevel) {
            return;
        }
        if (module.isEmpty()) {
            return;
        }
        log(header(Level::Trace).quote().space(), std::forward<Args>(args)...);
    }

    template<typename... Args>
    void trace_ext(const QStringList &extinfos, Args &&...args) {
        if (Level::Trace < logLevel || Level::Trace < globalLogLevel) {
            return;
        }
        if (module.isEmpty()) {
            return;
        }
        log(header(Level::Trace, extinfos).quote().space(), std::forward<Args>(args)...);
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
    Level logLevel = Level::Trace;

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
