#pragma once

#include <QtCore/QTranslator>

namespace brabbit {

  class Translator : public QTranslator {
    Q_OBJECT;

   public:
    explicit Translator(QObject* parent = nullptr);
    virtual ~Translator() override = default;

   public:
    auto load(const QString& filename) -> bool;

    auto translate(const char* context,
                   const char* source,
                   const char* disambiguation = nullptr,
                   int         n              = -1) const -> QString override;

   private:
    QString filename_{};
  };

}  // namespace brabbit
