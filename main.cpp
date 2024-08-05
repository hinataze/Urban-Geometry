#include <include/mainwindow.h>
#include <include/interactive_view.h>
#include <include/hs_shortest_path.h>


#include <QMenuBar>
#include <QMenu>
#include <QAction>

#include <QTextCodec>
#include <Windows.h>

#include <QToolButton>

#include <QSplitter>



#include <cstdlib>

#include <string>



int main(int argc, char *argv[])
        {
            qDebug() << "Main" ;
            QApplication app(argc, argv);
           
            QFont font("Segoe UI", 10); // You can choose a font that supports Japanese
            app.setFont(font);

            QLocale::setDefault(QLocale(QLocale::Japanese, QLocale::Japan));
            QTextCodec::setCodecForLocale(QTextCodec::codecForName("Shift-JIS"));

            MainWindow mainWindow(nullptr, &app);
            mainWindow.setGeometry(100, 100, 1600, 800);
            mainWindow.setWindowTitle("Urban Geometry");
            mainWindow.setWindowIcon(QIcon("D:/hsdev/project/GEO_ALG/icon_windows.ico"));

            UINT codePage = GetACP();
            // Output the code page
            qDebug() << "System Encoding Code Page: " << codePage;

            QTextCodec* codec = QTextCodec::codecForLocale();
            qDebug() << "Current locale encoding: " << codec->name();

            qDebug() << "2 ";

             // Create a Delaunay triangulation
             Delaunay dt;
             dt.clear();

             CDT cdt;



             InteractiveView_hs_triangulation view (cdt, dt, mainWindow,  &mainWindow);

             QWidget centralWidget (&mainWindow);

             QVBoxLayout * layout = new QVBoxLayout(&centralWidget);
             layout->addWidget(&view);
             layout->addWidget(&view.textEditL);
             //view.textEditL.setHidden(!view.isHidden());
             view.textEditL.setHidden(false);

 
             centralWidget.setLayout(layout);
             mainWindow.setCentralWidget(&centralWidget);



             mainWindow.show();
             mainWindow.fit_view();


             QString currentFilePath = __FILE__;
             QFileInfo fileInfo(currentFilePath);
             QString sourceDir = fileInfo.absolutePath();
             qDebug() << "Source Directory:" << sourceDir;




const char* rs = R"(Welcome to the urban geometry app!

Here you get to test and understand some fundamental algorithms used in urban analysis and how the underlying C++ computer code works!

We're starting with Delaunay Triangulations and Shortest Paths.

HINT: Open your own files! This app accepts files in .geojson and .txt.)";

 QString  s = rs;
  QMessageBox::information(nullptr, "Welcome!", rs);

  QMessageBox::information(nullptr, "Colaborate!", "This is an open project, so any problems, comments or sugestions, please send them through the direct help email above!");

  QMessageBox::information(nullptr, "Delaunay Triangulation", mainWindow.ptr_iv->message_qs_DT_start);

            return app.exec();
        }
