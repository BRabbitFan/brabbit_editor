#include "UiKernal.hpp"

#include <QGuiApplication>
#include <QQmlContext>

#include "DockController.hpp"

namespace br {

namespace ui {

Kernal::Kernal(QPointer<QObject>parent)
    : QObject{parent}
    , language_manager_(std::make_unique<LanguageManager>(this))
    , qml_engine_(std::make_unique<QQmlApplicationEngine>(this)) {

  qmlRegisterType<DockController>("DockController", 1, 0, "DockController");

  const QUrl url(QStringLiteral("qrc:/main.qml"));
  QObject::connect(qml_engine_.get(), &QQmlApplicationEngine::objectCreated,
    qml_engine_.get(), [url](QObject* object, const QUrl& object_url) {
      if (!object && url == object_url) {
        QCoreApplication::exit(-1);
      }
    }, Qt::QueuedConnection);
  qml_engine_->rootContext()->setContextProperty("kernal", this);
  qml_engine_->load(url);

  QObject::connect(language_manager_.get(), &LanguageManager::sigCurrentLanguageChanged,
                   qml_engine_.get(), &QQmlApplicationEngine::retranslate, Qt::QueuedConnection);
}

QPointer<QObject> Kernal::getLanguageManager() const {
  return language_manager_.get();
}

QList<QString> Kernal::getArguments() const {
  return QCoreApplication::arguments();
}

QPointer<QObject> Kernal::getWindow() const {
  return window_;
}

void Kernal::setWindow(QObject* window) {
  window_ = window;
  Q_EMIT sigWindowChanged(window_);
}

}

}
