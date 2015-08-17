#include "logger.h"
#include <stdexcept>

std::mutex LogStreamer::StreamMutex;

//------------L O G   S T R E A M E R------------

//Constructors

LogStreamer::LogStreamer(std::ostream &strm, const std::string &strng, std::shared_ptr<TimeFormatter> tf)
    : stream(strm), message(strng + tf->ToString()), lg(LogStreamer::StreamMutex)
{
    stream << message;
}

//Destructor

LogStreamer::~LogStreamer()
{
    stream.flush();
}

//------------S T R E A M   S A V E R------------

StreamSaver::StreamSaver(std::ostream &strm) : stream(strm) { }

std::ostream &StreamSaver::getStream() { return stream; }

OFStreamOwner::OFStreamOwner(const std::string &way) : stream(way) { }

std::ofstream &OFStreamOwner::getStream() { return stream; }

OFStreamOwner::~OFStreamOwner() { stream.close(); }

//------------L O G G E R------------

//Static Members

std::shared_ptr<TimeFormatter> Logger::tformatter(
        TimeFormatter::CreateFormatter(TimeSettings::TimeFormat::NoTime));

std::map<LogTypes, std::shared_ptr<StreamSaver> > Logger::StreamsMap;

std::vector<std::shared_ptr<OFStreamOwner> > Logger::DefStreams;

//Set Methods

bool Logger::SetTimeFormat(TimeSettings::TimeFormat tf)
{
    std::lock_guard<std::mutex> lc(LogStreamer::StreamMutex);
    if (!TimeSettings::CheckTimeFormat(tf)) {
        return false;
    }
    tformatter = TimeFormatter::CreateFormatter(tf);
    return true;
}

bool Logger::SetStreamForMessages(LogTypes tp, std::ostream &stream)
{
    if (!Logger::CheckLogType(tp)) {
        return false;
    }
    if (!stream.good()) {
        return false;
    }
    if (StreamsMap.find(tp) == StreamsMap.end()) {
        try {
            StreamsMap.insert(std::make_pair(tp, std::shared_ptr<StreamSaver>(new StreamSaver(stream))));
        }
        catch (std::bad_alloc)
        {
            throw;
        }
        catch (...)
        {
            throw std::runtime_error("Logger::SetStreamForMessages: unknown error.");
        }
        return true;
    }
    return false;
}

//Get Methods

LogStreamer Logger::Log(LogTypes tp)
{
    if (!Logger::CheckLogType(tp)) {
        throw std::invalid_argument("Logger::Log: log type is invalid");
    }
    if (StreamsMap.find(tp) == StreamsMap.end()) {
        std::shared_ptr<OFStreamOwner> DefStream;
        try {
            DefStream.reset(new OFStreamOwner("Default" + Logger::LogTypeToString(tp)));
        } catch (std::bad_alloc) {
            throw;
        } catch (...) {
            throw std::runtime_error("Logger::Log: unknown error.");
        }
        Logger::DefStreams.push_back(DefStream);
        Logger::SetStreamForMessages(tp, DefStream->getStream());
    }
    std::string type = Logger::LogTypeToString(tp);
    return LogStreamer(StreamsMap[tp]->getStream(), type, tformatter);
}

bool Logger::CheckLogType(LogTypes lt)
{
    return (lt == LogTypes::DEBUG) || (lt == LogTypes::INFO) || (lt == LogTypes::WARNING) ||
            (lt == LogTypes::ERROR) || (lt == LogTypes::NOTE) || (lt == LogTypes::RESULT) || (lt == LogTypes::TEST);
}

std::string Logger::LogTypeToString(LogTypes tp)
{
    switch (tp) {
    case LogTypes::DEBUG:
        return std::string("[ DEBUG ]");
    case LogTypes::INFO:
        return std::string("[ INFO  ]");
    case LogTypes::ERROR:
        return std::string("[ ERROR ]");
    case LogTypes::WARNING:
        return std::string("[WARNING]");
    case LogTypes::NOTE:
        return std::string("[ NOTE  ]");
    case LogTypes::RESULT:
        return std::string("[RESULT ]");
    case LogTypes::TEST:
        return std::string("[ TEST  ]");
    default:
        return std::string();
    }
}
