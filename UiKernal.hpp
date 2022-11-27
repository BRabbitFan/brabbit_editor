#pragma once

#include <memory>

#include <QObject>
#include <QPointer>
#include <QQmlApplicationEngine>

#include "LanguageManager.hpp"

namespace br {

namespace ui {

class Kernal : public QObject {
  Q_OBJECT

public:
  explicit Kernal(QPointer<QObject> parent = nullptr);

public:
  Q_PROPERTY(QObject* languageManager READ getLanguageManager CONSTANT);
  Q_INVOKABLE QPointer<QObject> getLanguageManager() const;

public:
  Q_PROPERTY(QList<QString> arguments READ getArguments CONSTANT);
  Q_INVOKABLE QList<QString> getArguments() const;

public:
  Q_PROPERTY(QObject* window READ getWindow WRITE setWindow NOTIFY sigWindowChanged);
  Q_INVOKABLE QPointer<QObject> getWindow() const;
  Q_SLOT void setWindow(QObject* window);
  Q_SIGNAL void sigWindowChanged(QObject* window);

private:
  std::unique_ptr<LanguageManager> language_manager_{ nullptr };
  std::unique_ptr<QQmlApplicationEngine> qml_engine_{ nullptr };

  QPointer<QObject> window_{ nullptr };
};

}

}
