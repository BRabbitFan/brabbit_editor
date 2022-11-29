#include <QGuiApplication>

#include "UiKernal.hpp"

int main(int argc, char* argv[]) {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

  QGuiApplication application(argc, argv);

  br::ui::Kernal ui_kernal;

  return QGuiApplication::exec();
}
