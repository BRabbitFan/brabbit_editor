#pragma once

#include <QtCore/QString>
#include <QtCore/QCoreApplication>

namespace brabbit {

  inline auto translate(const char* source) -> QString {
    return QCoreApplication::translate(nullptr, source);
  }

}  // namespace brabbit
