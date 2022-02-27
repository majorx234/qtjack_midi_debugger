#ifndef MAIN_WINDOW_HPP 
#define MAIN_WINDOW_HPP

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
 

signals:
  void closed();  
protected:
  void closeEvent(QCloseEvent *event) override;
  void resizeEvent(QResizeEvent* event) override;
private slots:

  void close();

private:
  void initActionsConnections();
  Ui::MainWindow *mainwindow_ui = nullptr;
  //Console *mainwindow_console = nullptr;
};        

#endif // MAIN_WINDOW_HPP
