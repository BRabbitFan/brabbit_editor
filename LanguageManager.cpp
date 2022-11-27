#include "LanguageManager.hpp"

#include <mutex>

#include <QCoreApplication>

#include <QJsonDocument>
#include <QJsonObject>

#include <QSettings>

#include <QFileInfo>
#include <QLocale>
#include <QDir>

#include <QDebug>

#include "translate.hpp"

namespace br {

namespace ui {

namespace {

const auto DEFAULT_LANGUAGE{ QStringLiteral("en_US") };
const auto LANGUAGE_FILT_TYPE{ QStringLiteral("ini")};
const auto LANGUAGE_DIR_FMT{ QStringLiteral("%1/language/") };
const auto LANGUAGE_FILE_FMT{ QStringLiteral("%1/language/%2.") + LANGUAGE_FILT_TYPE };

const auto KEY_HEAD{ QStringLiteral("head") };
const auto KEY_BODY{ QStringLiteral("body") };
const auto KEY_I18N{ QStringLiteral("i18n") };
const auto KEY_DESCRIBE{ QStringLiteral("describe") };

}





inline const std::set<I18n>& GetI18nSet() {
  static std::set<I18n> I18n_Set;

  static std::once_flag Init_Once;
  std::call_once(Init_Once, []() mutable {
    const QString dir_path{ LANGUAGE_DIR_FMT.arg(QCoreApplication::applicationDirPath()) };

    QFileInfo dir_info{ dir_path };
    if (!dir_info.isDir()) {
      return;
    }

    const QDir dir{ dir_info.dir() };

    const QList<QString> name_filter{ QStringLiteral("*.") + LANGUAGE_FILT_TYPE };
    const auto type_filter{ QDir::Files | QDir::Readable };
    const auto sort_flag{ QDir::Name };

    const QList<QString> file_name_list{ dir.entryList(name_filter, type_filter, sort_flag) };

    for (const auto& file_name : file_name_list) {
      I18n_Set.insert(file_name.split(QStringLiteral(".")).first());
    }
  });

  return I18n_Set;
}

inline I18n GetDefaultI18n() {
  return CheckI18nVaild(DEFAULT_LANGUAGE) ? DEFAULT_LANGUAGE : *GetI18nSet().cbegin();
}

inline bool CheckI18nVaild(const I18n& i18n) {
  return GetI18nSet().find(i18n) != GetI18nSet().cend();
}





AbstractTranslator::AbstractTranslator(QPointer<QObject> parent) : AbstractTranslator(GetDefaultI18n(), parent) {}

AbstractTranslator::AbstractTranslator(const I18n& i18n, QPointer<QObject> parent) : QTranslator(parent) {
  setI18n(i18n);
}

QString AbstractTranslator::translate(const char* context, const char* source,
                                      const char* disambiguation, int n) const {
  return translate(source);
}

bool AbstractTranslator::isEmpty() const {
  return isInitialized();
}

bool AbstractTranslator::isInitialized() const {
  return !i18n_.isEmpty() && !describe_.isEmpty() && !source_translation_map_.empty();
}

void AbstractTranslator::release() {
  i18n_.clear();
  describe_.clear();
  source_translation_map_.clear();
}

I18n AbstractTranslator::getI18n() const {
  return i18n_;
}

void AbstractTranslator::setI18n(const I18n& i18n) {
  i18n_ = i18n;
}

QString AbstractTranslator::getDescribe() const {
  return describe_;
}

QString AbstractTranslator::translate(const QString& source) const {
  if (!isInitialized()) {
    return source;
  }

  auto itor = source_translation_map_.find(source);
  if (itor == source_translation_map_.end()) {
    return source;
  }

  return itor->second;
}





XmlTranslator::XmlTranslator(QPointer<QObject> parent) : AbstractTranslator(parent) {}

XmlTranslator::XmlTranslator(const I18n& i18n, QPointer<QObject> parent)
    : AbstractTranslator(i18n, parent) {}

bool XmlTranslator::initialize() {
  return false;
}





JsonTranslator::JsonTranslator(QPointer<QObject> parent) : AbstractTranslator(parent) {}

JsonTranslator::JsonTranslator(const I18n& i18n, QPointer<QObject> parent)
    : AbstractTranslator(i18n, parent) {}

bool JsonTranslator::initialize() {
  const QString file_path = LANGUAGE_FILE_FMT.arg(QCoreApplication::applicationDirPath()).arg(getI18n());

  if (!QFileInfo{ file_path }.isFile()) {
    return false;
  }

  QFile file{ file_path };
  if (!file.open(QIODevice::OpenModeFlag::ReadOnly)) {
    return false;
  }

  QJsonParseError error;
  auto document = QJsonDocument::fromJson(file.readAll(), &error);
  if (QJsonParseError::NoError != error.error) {
    return false;
  }

  auto object = document.object();
  if (object.isEmpty()) {
    return false;
  }

  auto head_value = object[KEY_HEAD];
  auto body_value = object[KEY_BODY];
  if (!head_value.isObject() || !body_value.isObject()) {
    return false;
  }

  auto head_object = head_value.toObject();
  auto i18n_value = head_object[KEY_I18N];
  auto describe_value = head_object[KEY_DESCRIBE];
  if (!i18n_value.isString() || !describe_value.isString()) {
    return false;
  }

  if (i18n_value.toString() != getI18n()) {
    return false;
  }

  describe_ = describe_value.toString();

  source_translation_map_.clear();
  auto body_object = body_value.toObject();
  for (auto itor = body_object.constBegin(); itor != body_object.constEnd(); itor++) {
    auto key = itor.key();
    if (key.startsWith(QStringLiteral("//"))) {
      continue;
    }

    const auto value = itor.value();
    if (!value.isString()) {
      continue;
    }

    source_translation_map_.insert({ std::move(key), value.toString() });
  }

  return true;
}





IniTranslator::IniTranslator(QPointer<QObject> parent) : AbstractTranslator(parent) {}

IniTranslator::IniTranslator(const I18n& i18n, QPointer<QObject> parent)
    : AbstractTranslator(i18n, parent) {}

bool IniTranslator::initialize() {
  const QString file_path = LANGUAGE_FILE_FMT.arg(QCoreApplication::applicationDirPath()).arg(getI18n());

  if (!QFileInfo{ file_path }.isFile()) {
    return false;
  }

  QSettings file{ file_path, QSettings::Format::IniFormat };

  file.beginGroup(KEY_HEAD);
  i18n_.clear();
  describe_.clear();
  for (const auto& key : file.childKeys()) {
    if (KEY_I18N == key) {
      i18n_ = file.value(key).toString();
    } else if (KEY_DESCRIBE == key) {
      describe_ = file.value(key).toString();
    }
  }
  file.endGroup();

  if (i18n_.isEmpty() || describe_.isEmpty()) {
    return false;
  }

  file.beginGroup(KEY_BODY);
  source_translation_map_.clear();
  for (const auto& key : file.childKeys()) {
    auto value = file.value(key).toString();
    source_translation_map_.insert({key, std::move(value)});
  }
  file.endGroup();

  return true;
}






Language::Language(QPointer<QObject> parent) : Language(GetDefaultI18n(), parent) {}

Language::Language(const QString& i18n, QPointer<QObject> parent)
    : QObject(parent)
    , i18n_(CheckI18nVaild(i18n) ? i18n : GetDefaultI18n()) {
}

QString Language::getI18n() const {
  return i18n_;
}

void Language::setI18n(const QString& i18n) {
  if (i18n_ == i18n || !CheckI18nVaild(i18n)) {
    return;
  }

  i18n_ = i18n;

  Q_EMIT sigI18nChanged(i18n_);
  Q_EMIT sigSourceChanged(getSource());
}

QString Language::getSource() const {
  const auto SOURCE_QSTR = QStringLiteral("i18n_%1_source").arg(getI18n());
  const auto SOURCE_STDSTR = SOURCE_QSTR.toStdString();
  const auto SOURCE_CSTR = SOURCE_STDSTR.c_str();
  return tr::load(SOURCE_CSTR);
}





LanguageManager::LanguageManager(QPointer<QObject> parent) : QObject{parent} {
  setCurrentLanguage(QLocale().name());
}

QString LanguageManager::getCurrentLanguage() const {
  return current_language_;
}

void LanguageManager::setCurrentLanguage(const QString& i18n) {
  if (translator_) {
    QCoreApplication::removeTranslator(translator_);
    translator_->deleteLater();
  }

  auto* translator = new IniTranslator{ i18n };
  if (!translator->initialize()) {
    translator->deleteLater();
    return;
  }

  current_language_ = i18n;
  translator_ = translator;
  QCoreApplication::installTranslator(translator_);
  Q_EMIT sigCurrentLanguageChanged(current_language_);
  qDebug() << "new language:" << current_language_;
}

QList<QObject*> LanguageManager::getLanguages() const {
  QList<QObject*> language_list;
  for (const auto& i18n : GetI18nSet()) {
    auto* language = new Language{ i18n, const_cast<LanguageManager*>(this) };
    language_list.append(language);
  }
  return language_list;
}

}

}
