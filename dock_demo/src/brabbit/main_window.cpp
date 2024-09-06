#include <brabbit/main_window.hpp>
#include <brabbit/main_window.ui.hpp>

namespace brabbit {

  MainWindow::MainWindow(QWidget* parent)
      : QMainWindow{ parent }, ui_{ std::make_unique<Ui::MainWindow>() } {
    ui_->setupUi(this);
  }

  MainWindow::~MainWindow() {}

}  // namespace brabbit
