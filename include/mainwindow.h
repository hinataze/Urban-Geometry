#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <include/ui_mainwindow.h>

//OPENAI
//#include "/Applications/GitClones/liboai/liboai/include/liboai.h"
//#include "openai.hpp"

#include <QGLWidget>
#include <QSvgGenerator>


//JSON INPUT
//#include <rapidjson/document.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QApplication>
#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <vector>


#include <QPointF>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QVBoxLayout>
#include <QToolBar>
#include <QLabel>
#include <QGraphicsTextItem>
#include <QTextEdit>
#include <QTextStream>
#include <QFileDialog>
#include <QDebug>
//#include <fstream>
#include <QPinchGesture>
#include <QGraphicsLineItem>
#include <QLinearGradient>
#include <QPushButton>
#include <QCursor>
#include <QPixmap>
#include <QMessageBox>
#include <QIcon>
#include <QWheelEvent>
#include <qtextbrowser.h>


#include <QGraphicsBlurEffect>





class InteractiveView_hs_triangulation;

class MainWindow;




QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
                  {
                    Q_OBJECT

                    friend class InteractiveView_hs_triangulation;
                    friend std::vector <std::pair <double, double>> openfile  (QString path);

                    public:
                    MainWindow(QWidget *parent = nullptr, QApplication * app = nullptr);
                    ~MainWindow();

                    void turn_flag_view (bool &flag);
                    void turn_flag_edit (bool &flag);

                    InteractiveView_hs_triangulation* ptr_iv = nullptr;

                    void view_refresh();
                    void fit_view();

                    void openEmailClient ();

                    public slots:

                    void openFile (bool flag_start);
                    std::vector <std::pair <double, double>> openfile_txt(QString path);
                    std::map <QString, std::pair <QString, QString>> openfile_GeoJson(QString path);
                    void openfile_insertpoints_txt(std::vector <std::pair <double, double>>& vpts);
                    void openfile_insertpoints_gjson(std::map <QString, std::pair <QString, QString>>& vpts);
                    void openfile_insertedges_txt(std::vector <std::pair <double, double>>& vegs, std::vector <std::pair <double, double>>& vpts);
                    void openfile_insertedges_gjson(std::map <QString, std::pair <QString, QString>>& vegs, std::map <QString, std::pair <QString, QString>>& vpts);
                    bool isGeoJson(const QString);

                    void save_to_file_paths (QString, QString);

                    void save_files();

                    protected:
         
                    private:

                     Ui::MainWindow *ui; //allows to access the ui elements
                     QLabel * statusLabel;
                     QGraphicsScene scene_;
                     

                     //menu bar
                     QMenuBar* menu_bar;

                     //file
                     QMenu* menu_file;
                     QAction* openAction;
                     QAction* saveAction;
                     QAction* exitAction;

                     //mode
                     QMenu* menu_mode ;
                     QAction* dtAction_v ;
                     QAction* ctAction_v;

                     //view
                     QMenu* menu_view ;
                     QAction* fitviewAction ;
                     QAction* view_voronoi_action;

                     QToolBar* toolbar_v;

                     QAction* helpAction;
                  };


#endif // MAINWINDOW_H

