#include <windows.h>

#include <QQuickView>
#include <QQmlContext>
#include <QQuickItem>
#include <QQmlContext>

#include "QMainPanel.h"

#include <windowsx.h>

#include "src/zip/archive.hpp"
#include "src/zipimageprovider.hpp"
#include "src/zipitemmodel.hpp"


QMainPanel::QMainPanel( HWND hWnd ) {

    windowHandle = hWnd;
    setResizeMode(ResizeMode::SizeViewToRootObject);


    QString zipFile = "C:/Users/d10dd/Desktop/Test3.zip";
    auto archive = QSharedPointer<Zip::Archive>::create(zipFile);
    ZipImageProvider* provider = new ZipImageProvider(archive);
    ZipItemModel* model = new ZipItemModel(archive);

    rootContext()->setContextProperty("archive", archive.data());
    rootContext()->setContextProperty("myModel", model);
    engine()->addImageProvider(QLatin1String("zipimageprovider"), provider);
    rootContext()->setContextProperty("mainWindow", this);
    setSource(QUrl("qrc:/main.qml"));

    parent = QWindow::fromWinId(reinterpret_cast<WId>(hWnd));
    setParent(parent);
    show();
}

// Button events
void QMainPanel::pushButtonMinimizeClicked() {
  ShowWindow( parentHwnd(), SW_MINIMIZE );
}

void QMainPanel::pushButtonMaximizeClicked() {
  WINDOWPLACEMENT wp;
  wp.length = sizeof( WINDOWPLACEMENT );
  GetWindowPlacement( parentHwnd(), &wp );
  if ( wp.showCmd == SW_MAXIMIZE ) {
    ShowWindow( parentHwnd(), SW_RESTORE );
  } else {
    ShowWindow( parentHwnd(), SW_MAXIMIZE );
  }
}

void QMainPanel::pushButtonCloseClicked() {
    PostQuitMessage( 0 );
}

void QMainPanel::toolbarLeftMouseEvent()
{
    qDebug() << "Fired!";
    ReleaseCapture();
    SendMessage( windowHandle, WM_NCLBUTTONDOWN, HTCAPTION, 0 );
}

void QMainPanel::toolbarDoubleClicked()
{
    WINDOWPLACEMENT wp;
    wp.length = sizeof( WINDOWPLACEMENT );
    GetWindowPlacement( parentHwnd(), &wp );
    if ( wp.showCmd == SW_MAXIMIZE ) {
      ShowWindow( parentHwnd(), SW_RESTORE );
    } else {
      ShowWindow( parentHwnd(), SW_MAXIMIZE );
    }
}

void QMainPanel::minimize()
{
    ShowWindow(windowHandle, SW_MINIMIZE);
}

void QMainPanel::maximize()
{
    WINDOWPLACEMENT wp;
    wp.length = sizeof( WINDOWPLACEMENT );
    GetWindowPlacement( windowHandle, &wp );
    if ( wp.showCmd == SW_MAXIMIZE ) {
      ShowWindow( windowHandle, SW_RESTORE );
    } else {
      ShowWindow( windowHandle, SW_MAXIMIZE );
    }
}

void QMainPanel::close()
{
    PostQuitMessage( 0 );
}

#if QT_VERSION >= 0x050000
  bool QMainPanel::nativeEvent( const QByteArray &, void *msg, long * ) {
#else
  bool QMainPanel::winEvent( MSG *message, long * ) {
#endif
#if QT_VERSION >= 0x050000
  MSG *message = ( MSG * )msg;
#endif
  switch( message->message ) {
    case WM_SYSKEYDOWN: {
      if ( message->wParam == VK_SPACE ) {
        SendMessage( windowHandle, WM_SYSKEYDOWN, VK_SPACE, message->lParam );
        return true;
      }
      return DefWindowProc( message->hwnd, message->message, message->wParam, message->lParam );
    }
    case WM_KEYDOWN: {
      if ( message->wParam == VK_F5 ||
           message->wParam == VK_F6 ||
           message->wParam == VK_F7
         ) {
        SendMessage( windowHandle, WM_KEYDOWN, message->wParam, message->lParam );
        break;
      }
    }
  }

  return false;
}

  void QMainPanel::mousePressEvent( QMouseEvent *event ) {
      if (0) { // Not needed anymore - may come in handy later
        if ( event->button() == Qt::LeftButton ) {
           ReleaseCapture();
           SendMessage( windowHandle, WM_NCLBUTTONDOWN, HTCAPTION, 0 );
        }
        if ( event->type() == QEvent::MouseButtonDblClick ) {
          if (event -> button() == Qt::LeftButton) {
            WINDOWPLACEMENT wp;
            wp.length = sizeof( WINDOWPLACEMENT );
            GetWindowPlacement( parentHwnd(), &wp );
            if ( wp.showCmd == SW_MAXIMIZE ) {
              ShowWindow( parentHwnd(), SW_RESTORE );
            } else {
              ShowWindow( parentHwnd(), SW_MAXIMIZE );
            }
          }
        }
      } else { // Always executed
          QQuickView::mousePressEvent(event);
      }
  }

void QMainPanel::setSize(int x, int y)
{
    setWidth(x);
    setHeight(y);
}
