/**
 * The primary application code.
 */

#include <QtGui>
#include <QtCore>
#include <QtWidgets>

class App : public QMainWindow
{
  Q_OBJECT

public:
  App(QString path, QWidget* parent=0);
  virtual ~App();

protected:
  QString renderableFilePath;
  
signals:

public slots:

private:
  void buildGui();
};
