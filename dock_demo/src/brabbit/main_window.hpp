#pragma once

#include <memory>

#include <QtWidgets/QMainWindow>

namespace Ui {
  class MainWindow;
}  // namespace Ui

namespace brabbit {

  class MainWindow : public QMainWindow {
    Q_OBJECT;

   public:
    explicit MainWindow(QWidget* parent = nullptr);
    virtual ~MainWindow() override;

   public:
    Q_PROPERTY(QString version READ getVersion CONSTANT);
    auto getVersion() const -> QString { return {}; };

   private:
    std::unique_ptr<Ui::MainWindow> ui_{ nullptr };
  };

}  // namespace brabbit
