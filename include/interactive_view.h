//the last scale applied to view is the one that shows
// ? does the scale apply to the contained items?
// ? why does the line width scale when adding a point or opening file but not from the beginning ? set pens first??

#ifndef INTERACTIVE_VIEW_H
#define INTERACTIVE_VIEW_H

#include <include/mainwindow.h>

#include <boost/graph/adjacency_list.hpp>

#include <QPlainTextEdit>




typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property, boost::property<boost::edge_weight_t, double>> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor Boost_Vertex;

class InteractiveViewTriangulation : public QGraphicsView
                                   {
                                   Q_OBJECT
                                   friend class MainWindow;

                                   friend void deletepoint (QPoint, InteractiveViewTriangulation *);
                                   friend void insertpoint (QPoint, InteractiveViewTriangulation *);
                                   friend void define_source_target (QPoint, InteractiveViewTriangulation *);


                                   public:
                                       InteractiveViewTriangulation (CDT &cdt, Delaunay& dt, MainWindow &mw, QMainWindow* parent = nullptr);

                                       QPlainTextEdit  textEditL;
                                       QPlainTextEdit  textEditR;

                                       QString  message_qs_DT_start;
                                       QString  message_qs_DT;
                                       QString  message_qs_CDT;
                                       QString  message_qs_open;
                                       QString  message_qs_save;

                                       bool flag_message_DT = false;
                                       bool flag_message_CDT = false;
                                       bool flag_message_open = false;
                                       bool flag_message_save = false;

                                       MainWindow  & ref_mainwindow;

                                       bool flag_scale_first = true;


                                       bool flag_triangulation = 1;
                                       bool flag_edit_triangulation = 1;
                                       bool flag_voronoi = 0;
                                       bool flag_edit_voronoi = 1;

                                       bool flag_ctriangulation = 0;
                                       bool flag_edit_ctriangulation = 0;
                                       bool flag_spath = 0;
                                       bool flag_edit_spath = 0;

                                       int  pathindex = 0;

                                       double scale_factor ();

                                       void source_target_shortestpath (Point_2);

                                       void sync_flag_sets();


                                   protected:

                                       void wheelEvent(QWheelEvent *event) override;

                                       void mousePressEvent (QMouseEvent* event) override;

                                       void keyPressEvent (QKeyEvent *event) override;


                                       void mouseReleaseEvent(QMouseEvent* event) override;

                                       void mouseMoveEvent(QMouseEvent* event) override;




                                       QGraphicsTextItem* createTextItem (QPointF position, QString text);

                                       bool event(QEvent* event) override;

                                       void scene_refresh();
                                       void clear_graphicitems();
                                       void create_graphicitems();
                                       void scene_add_graphicitems();
                                       void set_textitems();
                                       void set_textitems_dt();
                                       void set_textitems_ct();
                                       void scene_add_textitems();
                                       void setpens ();
                                       void update_log();

                                      void shortest_path();




                                      size_t flag_input_st = 0;


                                      void save_paths_coordinates (std::vector <CDT> & v_paths);
                                      void save_paths_indices (std::vector <std::vector<size_t>> & v_paths_ids);


                                      private:

                                       Delaunay& dt_;

                                       CDT &cdt_;

                                       CGAL::Qt::GraphicsViewNavigation navigation;

                                       std::vector<CGAL::Qt::TriangulationGraphicsItem<Delaunay>*> triangulationItems = { nullptr };
                                       std::vector <QGraphicsTextItem*> textItems;

                                       CGAL::Qt::VoronoiGraphicsItem<Delaunay> * voronoiItem = nullptr;

                                       CGAL::Qt::CTriangulationGraphicsItem<CDT> * ctriangulationItem = nullptr;
                                       std::vector <QGraphicsTextItem*> ctextItems;

                                       double fontSize = 6;

                                       double scalefactor = 1;

                                       public:
                                       std::pair <Point_2, Point_2> vppt_source_target;
                                       struct shortestpath {
                                                          std::vector<Point_2> v_source;
                                                          std::vector<Point_2> v_target;
                                                          std::vector<Boost_Vertex> bv_source;
                                                          std::vector<Boost_Vertex> bv_target;
                                                          std::vector <CDT> v_cdt;
                                                          std::vector<CGAL::Qt::CTriangulationGraphicsItem<CDT>*> v_cdt_gi;
                                                          std::vector <std::vector <CDT>> vv_paths;
                                                          std::vector <std::vector <std::vector<size_t>>> vv_paths_ids;

                                                          std::vector <std::vector<Boost_Vertex>> v_predecessors;
                                                          std::vector <std::vector<double>> v_distances;

                                                          QGraphicsEllipseItem* elipse_source = nullptr;
                                                          QGraphicsEllipseItem* elipse_target = nullptr;

                                                          size_t sp_size() {
                                                                             qDebug() << "shortestpath::sp_size" ;
                                                                             qDebug() << v_source.size() ;
                                                                             qDebug() << v_target.size () ;
                                                                             qDebug() << bv_source.size() ;
                                                                             qDebug() << bv_target.size () ;
                                                                             qDebug() << v_cdt.size() ;
                                                                             qDebug() << v_cdt_gi.size() ;
                                                                             qDebug() << "shortestpath::sp_size exit" ;
                                                                             return v_cdt.size();
                                                                           }
                                                            } sp;



                                       bool panning;
                                       QPointF lastPanPos;

                                   };
static const char* message_r_DT_start = R"(
You are in Delaunay Triangulation Mode.

In this mode you can add / remove points by left / right clicking on the canvas.
You can also see the code that runs everytime you insert or remove a point (limited).

You can save the final triangulation to a file and later open it again and start from there!

HINT: Make sure to toggle View > Voronoi to check the corresponding Voronoi diagram!

(You can switch modes in the Menu Bar) 
)";





static const char* message_r_DT = R"(
Delaunay Triangulation:

In this mode you can add / remove points by left / right clicking on the canvas.
You can also see the code that runs everytime you insert or remove a point (limited).

You can save the final triangulation to a file and later open it again and start from there!

HINT: Make sure to toggle View > Voronoi to check the corresponding Voronoi diagram!
)";

static const char* message_r_CDT = R"(
Constrained Delaunay Triangulation:

In this mode you can select two points and calculate the Shortest Path between them. Just click on the image and try.

HINT: You can save the shortest paths you calculated to files!
)";

static const char* message_r_open = R"(
Open your own files! This app accepts files:

In geojson format, you need one file with the nodes and one file with the links.
Check sample files in https://www.geospatial.jp/ckan/dataset/0401

In txt format, you also need both nodes and links' files.
Check the "Sample Files" in this App's folder!
)";




#endif // INTERACTIVE_VIEW_H
