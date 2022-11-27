#pragma once

#include <set>
#include <optional>

#include <QTranslator>
#include <QPointer>
#include <QVariant>
#include <QObject>

namespace br {

namespace ui {

using I18n = QString;
inline const std::set<I18n>& GetI18nSet();
inline I18n GetDefaultI18n();
inline bool CheckI18nVaild(const I18n& i18n);





class AbstractTranslator : public QTranslator {
  Q_OBJECT;

public:
  explicit AbstractTranslator(QPointer<QObject> parent = nullptr);
  explicit AbstractTranslator(const I18n& i18n, QPointer<QObject> parent = nullptr);

public:
  virtual QString translate(const char* context, const char* source,
                            const char* disambiguation = nullptr, int n = -1) const override;
  virtual bool isEmpty() const override;

public:
  virtual bool initialize() = 0;
  bool isInitialized() const;
  void release();

  I18n getI18n() const;
  void setI18n(const I18n&);

  QString getDescribe() const;

  QString translate(const QString& source) const;

protected:
  I18n i18n_;
  QString describe_;
  std::map<QString, QString> source_translation_map_;
};





class XmlTranslator : public AbstractTranslator {
  Q_OBJECT;

public:
  explicit XmlTranslator(QPointer<QObject> parent = nullptr);
  explicit XmlTranslator(const I18n& i18n, QPointer<QObject> parent = nullptr);

public:
  virtual bool initialize() override;
};





class JsonTranslator : public AbstractTranslator {
  Q_OBJECT;

public:
  explicit JsonTranslator(QPointer<QObject> parent = nullptr);
  explicit JsonTranslator(const I18n& i18n, QPointer<QObject> parent = nullptr);

public:
  virtual bool initialize() override;
};





class IniTranslator : public AbstractTranslator {
  Q_OBJECT;

public:
  explicit IniTranslator(QPointer<QObject> parent = nullptr);
  explicit IniTranslator(const I18n& i18n, QPointer<QObject> parent = nullptr);

public:
  virtual bool initialize() override;
};





class Language : public QObject {
  Q_OBJECT;

public:
  explicit Language(QPointer<QObject> parent = nullptr);
  explicit Language(const QString& i18n, QPointer<QObject> parent = nullptr);

public:
  Q_PROPERTY(QString i18n READ getI18n WRITE setI18n NOTIFY sigI18nChanged);
  Q_INVOKABLE QString getI18n() const;
  Q_SLOT void setI18n(const QString& i18n);
  Q_SIGNAL void sigI18nChanged(const QString& i18n);

public:
  Q_PROPERTY(QString source READ getSource NOTIFY sigSourceChanged);
  Q_INVOKABLE QString getSource() const;
  Q_SIGNAL void sigSourceChanged(const QString& source);

private:
  QString i18n_;
};





class LanguageManager : public QObject {
  Q_OBJECT;

public:
  explicit LanguageManager(QPointer<QObject> parent = nullptr);

public:
  Q_PROPERTY(QString   currentLanguage
             READ   getCurrentLanguage
             WRITE  setCurrentLanguage
             NOTIFY sigCurrentLanguageChanged);
  Q_INVOKABLE QString getCurrentLanguage() const;
  Q_SLOT void setCurrentLanguage(const QString& language);
  Q_SIGNAL void sigCurrentLanguageChanged(const QString& language);

public:
  Q_PROPERTY(QList<QObject*> languages READ getLanguages CONSTANT);
  Q_INVOKABLE QList<QObject*> getLanguages() const;

private:
  QPointer<QTranslator> translator_;
  QString current_language_;
};

}

}
