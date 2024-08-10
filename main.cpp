#include <include/mainwindow.h>
#include <include/interactive_view.h>
#include <include/hs_triangulation.h>


#include <QMenuBar>
#include <QMenu>
#include <QAction>

#include <QTextCodec>
#include <Windows.h>

#include <QToolButton>

#include <QSplitter>



#include <cstdlib>

#include <string>


static std::string rs = R"(Welcome to the urban geometry app!

Here you get to test and understand some fundamental algorithms used in urban analysis and how the underlying C++ computer code works!

We're starting with Delaunay Triangulations and Constrained Delaunay Triangulations, that allow us to find the Shortest Paths.
)";


int main(int argc, char *argv[])
        {
            qDebug() << "Main" ;
            QApplication app(argc, argv);
           
            QFont font("Segoe UI", 10); // You can choose a font that supports Japanese
            app.setFont(font);

            QLocale::setDefault(QLocale(QLocale::Japanese, QLocale::Japan));
            QTextCodec::setCodecForLocale(QTextCodec::codecForName("Shift-JIS"));

            QDir currentDir(QCoreApplication::applicationDirPath());
            QString iconPath = currentDir.filePath("icon_windows.ico");
            
            qDebug() << "icon path" << iconPath;


            MainWindow mainWindow(nullptr, &app);
            mainWindow.setGeometry(100, 100, 1600, 800);
            mainWindow.setWindowTitle("Urban Geometry");
            mainWindow.setWindowIcon(QIcon(iconPath));

            UINT codePage = GetACP();
            // Output the code page
            qDebug() << "System Encoding Code Page: " << codePage;

            QTextCodec* codec = QTextCodec::codecForLocale();
            qDebug() << "Current locale encoding: " << codec->name();

             // Create a Delaunay triangulation
             Delaunay dt;
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






            QString  s = QString::fromStdString(rs);
            QMessageBox::information(nullptr, "Welcome!", s);

            QMessageBox::information(nullptr, "Colaborate!", "This is an open project, so any problems, comments or sugestions, please send them through the direct help email above!");

            QMessageBox::information(nullptr, "Delaunay Triangulation", mainWindow.ptr_iv->message_qs_DT_start);

            return app.exec();
        }
