#include "App.h"

#include "BasicWidget.h"

App::App(QString path, QWidget* parent) : QMainWindow(parent)
{
  renderableFilePath = path;
  buildGui();
}

App::~App()
{}

void App::buildGui()
{
  qDebug() << renderableFilePath;
  // A simple menubar.
  QMenuBar* menu = menuBar();
  QMenu* file = menu->addMenu("File");
  QAction* exit = file->addAction("Quit", [this]() {close();});

  // Our basic widget.
  BasicWidget* widget = new BasicWidget(this);
  widget->setObjectPath(renderableFilePath);
  setCentralWidget(widget);
}
