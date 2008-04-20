/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#include <TestCommon.h>
#include <maxutils/all.h>

using namespace maxutils;

void maxutils::initLogger()
{
    Logger::init();
    Logger::insertLogFile            ("test.log");
    
    Logger::debugConfig.insertStream ("test.log");
    
//    Logger::infoConfig.insertStream  ("cout");
    Logger::infoConfig.insertStream  ("test.log");
    
//    Logger::warningConfig.insertStream("cout");
    Logger::warningConfig.insertStream("test.log");
    
//    Logger::errorConfig.insertStream("cerr");
    Logger::errorConfig.insertStream("test.log");
}
