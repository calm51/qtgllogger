#pragma execution_character_set("utf-8")

#include "qtgllogger.h"

#include <QMutex>

#include "qtgllogger_obj.h"



namespace QGL {


Level globalLogLevel = Level::Debug;
void setGlobalLogLevel(Level level) {
    globalLogLevel = level;
}

QMap<QString, QtGllogger*> loggers;


QtGllogger::~QtGllogger(){
    // qDebug()<<"析构"<<this->module;
    if (!this->module.isEmpty() &&this->module != QStringLiteral("QGL")){
        qgl.debug("~析构", this);
    }
}
QtGllogger::QtGllogger(){
    // qDebug()<<"默认构造"<<this->module;
}
QtGllogger::QtGllogger(const QString &moduleName)
    : module(moduleName){
    // qDebug()<<"构造"<<this->module;
}

QString QtGllogger::logLevelToString(Level level) const {
    switch (level) {
    case Level::Debug:
        return "D";
    case Level::Info:
        return "I";
    case Level::Warning:
        return "W";
    case Level::Error:
        return "E";
    case Level::Critical:
        return "C";
    default:
        return "?";
    }
}



}


QGL_E2D(QGL::Level, "Debug", "Info", "Warning", "Error", "Critical")



QGL_C2D(QGL::QtGllogger, QGL_C2D_FAS(o.module); QGL_C2D_FAS(o.logLevel))
// QDebug operator<<(QDebug debug, const QGL::QtGllogger& object) {
//     QDebugStateSaver saver(debug);
//     debug.nospace() << "QGL::QtGllogger(" << &object
//                     // << ", mode=" << object.mode
//                     << ")";
//     debug.space();
//     return debug;
// }

