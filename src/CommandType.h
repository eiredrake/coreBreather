#include "WString.h"

namespace CommandType
{
    enum Enum
    {
        unknown = -1,
        mode = 1,
    };


    Enum ToEnum(String command)
    {
        Enum result = Enum::unknown;

        if(command.compareTo("mode") == 0)
        {
            result = CommandType::mode;
        }

        return result;
    };
};