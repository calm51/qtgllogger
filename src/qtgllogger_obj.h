#pragma execution_character_set("utf-8")

#ifndef QTGLLOGGER_OBJ_H
#define QTGLLOGGER_OBJ_H


#include "./qtgllogger.h"


// 在此声明各命名空间的logger, 类等的logger只能在类内部声明
namespace QGL {

QGL_LOGGER_OBJECT_EXTERN;

}

/*

    QGL_LOGGER_OBJECT_STATIC;

    ~Cls() override{
        qgl.debug("~析构", this);
    }

*/


#endif // QTGLLOGGER_OBJ_H
