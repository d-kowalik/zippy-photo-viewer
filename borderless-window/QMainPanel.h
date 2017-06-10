#ifndef QMAINPANEL_H
#define QMAINPANEL_H

#include <QMouseEvent>
#include <QQuickView>

class QMainPanel : public QQuickView {
  Q_OBJECT

    QWindow* parent;
public:
  QMainPanel();
#if QT_VERSION >= 0x050000
  bool nativeEvent(const QByteArray &, void *msg, long *result);
#else
  bool winEvent(MSG *message, long  *result);
#endif
  void mousePressEvent( QMouseEvent *event );
  HWND parentHwnd() const { return reinterpret_cast<HWND>(parent->winId()); }

  void setSize(int x, int y);

  void init(HWND hWnd);
public slots:
    void pushButtonMinimizeClicked();
    void pushButtonMaximizeClicked();
    void pushButtonCloseClicked();
    void toolbarLeftMouseEvent();
    void toolbarDoubleClicked();
    void minimize();

    void maximize();
    void close();
private:
    HWND windowHandle;

signals:
    mouseButton5Pressed();

};

#endif // QMAINPANEL_H
