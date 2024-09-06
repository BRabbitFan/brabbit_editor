#include <brabbit/translator.hpp>

namespace brabbit {

  Translator::Translator(QObject* parent) : QTranslator{ parent } {}

  auto Translator::load(const QString& filename) -> bool {
    if (filename.isEmpty()) {
      return false;
    }

    return QTranslator::load(filename);
  }

  auto Translator::translate(const char* context,
                             const char* source,
                             const char* disambiguation,
                             int         n) const -> QString {
    return QTranslator::translate(context, source, disambiguation, n);
  }

}  // namespace brabbit
