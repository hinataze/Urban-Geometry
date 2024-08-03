#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <include/ui_mainwindow.h>

//#include "/Applications/GitClones/liboai/liboai/include/liboai.h"

//OPENAI
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

//CGAL

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include "HS_Delaunay_triangulation_2.h"
//#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Voronoi_diagram_2.h>
#include <CGAL/Delaunay_triangulation_adaptation_traits_2.h>
#include <CGAL/Delaunay_triangulation_adaptation_policies_2.h>
#include <CGAL/Aff_transformation_2.h>
#include <CGAL/Qt/GraphicsViewNavigation.h>
#include <CGAL/Qt/GraphicsItem.h>
//#include <CGAL/Qt/TriangulationGraphicsItem.h>
#include <CGAL/Qt/VoronoiGraphicsItem.h>
#include <CGAL/Bbox_2.h>

#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <include/HS_TriangulationGraphicsItem.h>
#include <include/HS_CTriangulationGraphicsItem.h>

#include <CGAL/Segment_2.h>
#include <CGAL/algorithm.h>


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

#include <fstream>

#include <QPinchGesture>

#include <QGraphicsLineItem>

#include <QLinearGradient>

#include <QPushButton>

#include <QCursor>

#include <QPixmap>

#include <QMessageBox>

#include <QIcon>


#include <QWheelEvent>




//SHORTEST PATH

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_id_2.h>
#include <CGAL/boost/graph/graph_traits_Delaunay_triangulation_2.h>
//#include <CGAL/boost/graph/dijkstra_shortest_paths.h>
#include <fstream>
typedef CGAL::Exact_predicates_inexact_constructions_kernel             K;
typedef K::Point_2                                                      Point;
typedef CGAL::Triangulation_vertex_base_with_id_2<K>                    Tvb;
typedef CGAL::Triangulation_face_base_2<K>                              Tfb;
typedef CGAL::Triangulation_data_structure_2<Tvb, Tfb>                  Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>                       Triangulation;
typedef boost::graph_traits<Triangulation>::vertex_descriptor           vertex_descriptor;
typedef boost::graph_traits<Triangulation>::vertex_iterator             vertex_iterator;
typedef boost::property_map<Triangulation, boost::vertex_index_t>::type VertexIdPropertyMap;


typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point_2;
//typedef CGAL::Delaunay_triangulation_2<K> Delaunay;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Delaunay;


typedef CGAL::Segment_2<K> Segment;
typedef CGAL::Constrained_Delaunay_triangulation_2<K> CDT;
typedef CDT::Vertex_handle Vertex_handle;
typedef CDT::Edge_iterator Edge_iterator;


typedef CGAL::Delaunay_triangulation_adaptation_traits_2<Delaunay>                 AT;
typedef CGAL::Delaunay_triangulation_caching_degeneracy_removal_policy_2 <Delaunay> AP;
////typedef CGAL::Delaunay_graph::Geom_traits                 GT;
typedef CGAL::Voronoi_diagram_2 <Delaunay, AT, AP> Voronoi;


typedef CGAL::Aff_transformation_2<K> Transformation;
typedef CGAL::Point_2<K> Point;
typedef CGAL::Vector_2<K> Vector;
typedef CGAL::Direction_2<K> Direction;






//dt to transform



class InteractiveViewTriangulation;

class MainWindow;




QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE




class MainWindow : public QMainWindow
                  {
                    Q_OBJECT

                    friend class InteractiveViewTriangulation;
                    friend std::vector <std::pair <double, double>> openfile  (QString path);

                    public:
                    MainWindow(QWidget *parent = nullptr, QApplication * app = nullptr);
                    ~MainWindow();

                    void turn_flag_view (bool &flag);
                    void turn_flag_edit (bool &flag);




                    bool isGeoJson (const QString);
                    std::vector <std::pair <double, double>>
                         openfile_txt  (QString path);
                    std::map <QString, std::pair <QString, QString>>
                         openfile_GeoJson  (QString path);
                    void openfile_insertpoints_txt(std::vector <std::pair <double, double>> &vpts);
                    void openfile_insertpoints_gjson(std::map <QString, std::pair <QString, QString>> &vpts);
                    void openfile_insertedges_txt(std::vector <std::pair <double, double>> &vegs, std::vector <std::pair <double, double>> &vpts);
                    void openfile_insertedges_gjson(std::map <QString, std::pair <QString, QString>> &vegs, std::map <QString, std::pair <QString, QString>> &vpts);
                    void view_refresh();

                    void fit_view();

                    void openEmailClient ();

                    InteractiveViewTriangulation * ptr_iv = nullptr ;

                    public slots:

                    void save_files();
                    void openFile (bool flag_start);

                     private:

                     Ui::MainWindow *ui; //allows to access the ui elements
                     QLabel * statusLabel;

                     QGraphicsScene scene_;

                    protected:
                    void paintEvent(QPaintEvent *event) override;

                    void mouseMoveEvent(QMouseEvent *event) override;

                    private:
                     QPoint cursorPosition;





                 private:
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

