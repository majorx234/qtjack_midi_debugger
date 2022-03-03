#ifndef MAIN_WINDOW_HPP 
#define MAIN_WINDOW_HPP

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"
#include "messagehistory.hpp"
#include "jackmidi.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  void msg_history_cb(std::string str);
 
 signals:
  void closed();  

 protected:
  void closeEvent(QCloseEvent *event) override;
  void resizeEvent(QResizeEvent* event) override;

 private slots:
  void close();
  void test();

 private:
  void initActionsConnections();
  Ui::MainWindow *mainwindow_ui_ = nullptr;
  MessageHistory *message_history_ = nullptr;
  JackMidi *jack_midi_interface_;
};        

#endif // MAIN_WINDOW_HPP
