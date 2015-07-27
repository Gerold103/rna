#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <streambuf>
#include <fstream>
#include "timeformatter.h"
#include <map>
#include <mutex>
#include <vector>

#define LogFL(log_type) Logger::Log(log_type) << "File: " << __FILE__ << ", line: " << __LINE__ << ": "

class Logger;

//--------L O G   S T R E A M-------

enum LogTypes {
    DEBUG, INFO, WARNING, ERROR, NOTE, RESULT
};

/*--------LogStreamer--------
 * Class-wrapper for stream. It can't be copied or assigned. Single thing,
 * that with him can be done - operator <<. It seems like ostream object.
 * In constructor it catch mutex and hold it until destroying. Objects of this class
 * can be created only by Logger class.
*/
class LogStreamer {
private:
    std::ostream &stream;
    std::string message;
    std::lock_guard<std::mutex> lg;

    static std::mutex StreamMutex;

    LogStreamer(const LogStreamer &);             //copying    is forbidden
    LogStreamer &operator =(const LogStreamer &); //assignment is forbidden

    //constructors
    LogStreamer(std::ostream &strm, const std::string &strng, std::shared_ptr<TimeFormatter> timef);
public:
    //operators
    template<class T>
    LogStreamer &operator <<(T n)
    {
        stream << n;
        return *this;
    }

    //destructor
    ~LogStreamer();

    //over
    friend class Logger;
};

/*----StreamSaver----
 * Class-wrapper for ostream. It survice to store reference on ostream.
*/
class StreamSaver {
    std::ostream &stream;
public:
    StreamSaver(std::ostream &strm);
    std::ostream &getStream();
    static std::shared_ptr<StreamSaver> CreateStreamSaver(std::ostream &strm);
};

class OFStreamOwner {
    std::ofstream stream;
public:
    OFStreamOwner(const std::string &way);
    std::ofstream &getStream();
    ~OFStreamOwner();
    static std::shared_ptr<OFStreamOwner> OFCreateStreamOwner(const std::string &way);
};

//----------------L O G G E R----------------

/*----Logger----
 * Thread-safety class for write logs. They can be one of the following types: debug, info.
 * Before record it write message type and time. Time format can be changed
 * during executing thread-safety. Before start to write messages of some type you must to
 * bind certain ostream with corresponding type of message (by method SetStreamForMessage).
*/
class Logger {
private:
    static std::shared_ptr<TimeFormatter> tformatter;
    static std::map<LogTypes, std::shared_ptr<StreamSaver> > StreamsMap;
    static std::vector<std::shared_ptr<OFStreamOwner> > DefStreams;

    Logger();               //creating is forbidden
    Logger(const Logger &); //copying  is forbidden

public:
    //set-methods
    static bool SetTimeFormat(TimeSettings::TimeFormat tf);
    static bool SetStreamForMessages(LogTypes tp, std::ostream &stream);

    //get-methods
    static LogStreamer Log(LogTypes tp);
    static bool CheckLogType(LogTypes lt);
    static std::string LogTypeToString(LogTypes tp);

    //print-methods
    //logs const objects
    template<typename T>
    static void LogObject(LogTypes tp, const T &ob)
    {
        Logger::Log(tp).stream << ob;
    }

    //logs changable objects
    template<typename T>
    static void LogObject(LogTypes tp, T &ob)
    {
        Logger::Log(tp).stream << ob;
    }

};

#endif // LOGGER_H


/*
 * LogStreamer illustration
 *
 *               _________                                                                        ________
 *            /           \                                                                     _/        \_
 *           /     /\      \      Here LogStreamer starts                                      / |\     /|  \
 *          |      \/     _/      to "eat" operators << and                                    { |_\   /_|  }  Here LogStreamer
 *          |           _/        prints them in ostream                                       {            }  will dead and release
 *          |         _/          _________          __________         __________             { _  _  _  _ }  mutex
Printing <- | <-  <-  \_    <<  /"some text"\  <<  /"some text2"\ <<  /"some text3"\  <<.....  |/ \/ \/ \/ \|
 *          \           \_      \___________/      \____________/     \____________/             Destructor
 *           \            \
 *             \___________\
 *               LogStreamer
*/
