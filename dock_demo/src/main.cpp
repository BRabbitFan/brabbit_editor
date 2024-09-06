#include <QtCore/QLocale>
#include <QtWidgets/QApplication>

#include <brabbit/main_window.hpp>
#include <brabbit/translator.hpp>

auto main(int argc, char* argv[]) -> int {
  auto application = QApplication{ argc, argv };

  auto translator = brabbit::Translator{ QApplication::instance() };
  // translator.load(QStringLiteral(":/i18n/%1").arg(QLocale::system().uiLanguages()));
  QApplication::installTranslator(&translator);

  auto main_window = brabbit::MainWindow{};
  main_window.show();

  return QApplication::exec();
}
