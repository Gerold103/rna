#ifndef TIMEFORMATTER_H
#define TIMEFORMATTER_H

#include <memory>
#include <ctime>
#include <string>

//----------------T I M E   F O R M A T T E R----------------

namespace TimeSettings {
    enum TimeFormat {
        HourMinSec, DayMonYear_HourMinSec, NoTime
    };
    bool CheckTimeFormat(TimeFormat tf);
}

class TimeFormatter
{
public:
    typedef TimeSettings::TimeFormat TimeFormat;
    static std::shared_ptr<TimeFormatter> CreateFormatter(TimeFormat tf);
    virtual std::string ToString() = 0;
    virtual ~TimeFormatter();
};

    //--------T F   T I M E    O N L Y--------

        class TFTimeOnly : public TimeFormatter
        {
        public:
            virtual std::string ToString();
            virtual ~TFTimeOnly();
        };

    //--------T F   D A T E   A N D   T I M E--------

        class TFDateAndTime : public TimeFormatter
        {
        public:
            virtual std::string ToString();
            virtual ~TFDateAndTime();
        };

    //--------N O   T I M E--------

        class TFNoTime : public TimeFormatter
        {
        public:
            virtual std::string ToString();
            virtual ~TFNoTime();
        };

#endif // TIMEFORMATTER_H
