#include "timeformatter.h"

//----------------T I M E   F O R M A T T E R----------------

//----TimeSettings----

bool TimeSettings::CheckTimeFormat(TimeFormat tf)
{
    return (tf == TimeFormat::HourMinSec) || (tf == TimeFormat::DayMonYear_HourMinSec) || (tf == TimeFormat::NoTime);
}

std::shared_ptr<TimeFormatter> TimeFormatter::CreateFormatter(TimeFormat tf)
{
    std::shared_ptr<TimeFormatter> result;
    switch (tf) {
    case TimeFormat::HourMinSec:
        try {
            result.reset(new TFTimeOnly());
        }
        catch (std::bad_alloc)
        {
            throw;
        }
        catch (...)
        {
            throw std::runtime_error("TimeFormatter::CreateFormatter: unknown error.");
        }
        return result;
    case TimeFormat::DayMonYear_HourMinSec:
        try {
            result.reset(new TFDateAndTime());
        }
        catch (std::bad_alloc)
        {
            throw;
        }
        catch (...)
        {
            throw std::runtime_error("TimeFormatter::CreateFormatter: unknown error.");
        }
        return result;
    case TimeFormat::NoTime:
        try {
            result.reset(new TFNoTime());
        }
        catch (std::bad_alloc)
        {
            throw;
        }
        catch (...)
        {
            throw std::runtime_error("TimeFormatter::CreateFormatter: unknown error.");
        }
        return result;
    default:
        return std::shared_ptr<TimeFormatter>(nullptr);
    }
}

TimeFormatter::~TimeFormatter() { }

//--------T F   T I M E   O N L Y--------

    std::string TFTimeOnly::ToString()
    {
        time_t rawTime;
        struct tm *timeInfo;
        time(&rawTime);
        timeInfo = localtime(&rawTime);

        std::string result("[");
        if (timeInfo->tm_hour < 10) {
            result += '0';
        }
        result += std::to_string(timeInfo->tm_hour) + ':';
        if (timeInfo->tm_min < 10) {
            result += '0';
        }
        result += std::to_string(timeInfo->tm_min) + ':';
        if (timeInfo->tm_sec < 10) {
            result += '0';
        }
        result += std::to_string(timeInfo->tm_sec) + ']';

        return result;
    }

    TFTimeOnly::~TFTimeOnly() { }

//--------T F   D A T E   A N D   T I M E--------

    std::string TFDateAndTime::ToString()
    {
        time_t rawTime;
        struct tm *timeInfo;
        time(&rawTime);
        timeInfo = localtime(&rawTime);

        std::string result("[");
        if (timeInfo->tm_mday < 10) {
            result += '0';
        }
        result += std::to_string(timeInfo->tm_mday) + '.';
        if (timeInfo->tm_mon + 1 < 10) {
            result += '0';
        }
        result += std::to_string(timeInfo->tm_mon + 1) + '.';
        result += std::to_string(timeInfo->tm_year + 1900) + ']';
        result += TFTimeOnly().ToString();
        return result;
    }

    TFDateAndTime::~TFDateAndTime() { }

//--------N O   T I M E--------

    std::string TFNoTime::ToString() { return ""; }

    TFNoTime::~TFNoTime() { }