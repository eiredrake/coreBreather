#include "WString.h"

namespace ModeType
{
  enum Enum
  {
    off = -1,
    breathe = 1,
    shoot = 2,
  };

  Enum ToEnum(String mode)
  {
    Enum result = Enum::off;
    if(mode.compareTo("breathe") == 0)
    {
      result = ModeType::breathe;
    }

    return result;
  };
};

