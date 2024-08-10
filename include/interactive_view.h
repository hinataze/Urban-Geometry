//the last scale applied to view is the one that shows
// ? does the scale apply to the contained items?
// ? why does the line width scale when adding a point or opening file but not from the beginning ? set pens first??

#ifndef INTERACTIVE_VIEW_H
#define INTERACTIVE_VIEW_H

#include <include/mainwindow.h>
#include <hs_execution_log.h>

#include <boost/graph/adjacency_list.hpp>

#include <QPlainTextEdit>

#include <include/HS_Delaunay_triangulation_2.h>
#include <include/HS_TriangulationGraphicsItem.h>
#include <include/HS_CTriangulationGraphicsItem.h>

//SHORTEST PATH

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_vertex_base_with_id_2.h>
#include <CGAL/boost/graph/graph_traits_Delaunay_triangulation_2.h>

#include <CGAL/Qt/GraphicsViewNavigation.h>
#include <CGAL/Qt/VoronoiGraphicsItem.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>


typedef CGAL::Exact_predicates_inexact_constructions_kernel             K;
typedef K::Point_2                                                      Point_2;
typedef CGAL::Delaunay_triangulation_2<K, CGAL::Triangulation_data_structure_2 <CGAL::Triangulation_vertex_base_with_id_2<K>, CGAL::Triangulation_face_base_2<K>>> Delaunay;
typedef CGAL::Constrained_Delaunay_triangulation_2<K>                   CDT;


class hs_triangulation;

class InteractiveView_hs_triangulation : public QGraphicsView
                                   {
                                   Q_OBJECT
                                   friend class MainWindow;

                                   public:
                                       MainWindow& ref_mainwindow;

                                       InteractiveView_hs_triangulation (CDT &cdt, Delaunay& dt, MainWindow &mw, QMainWindow* parent = nullptr);

                                       size_t log_size = 0;

                                       QTextEdit   textEditL;
                                       QTextEdit   textEditR;

                                       QString  message_qs_DT_start;
                                       QString  message_qs_DT;
                                       QString  message_qs_CDT;
                                       QString  message_qs_open;
                                       QString  message_qs_save;

                                       bool flag_message_DT = false;
                                       bool flag_message_CDT = false;
                                       bool flag_message_open = false;
                                       bool flag_message_save = false;

                                       bool flag_scale_first = true;


                                       bool flag_click_lr = 0;
                                       bool flag_start = true;

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
                                       void f1_1_source_target_shortestpath (Point_2);
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
                                       void update_text();

                                       CGAL::Qt::GraphicsViewNavigation navigation;       
                                    
                                  

                                      void save_to_file_paths (QString filePath_ids, QString filePath_coord);

                                      private:

                                      std::shared_ptr <hs_triangulation> hst;
                                      double fontSize = 6;

                                      double scalefactor = 1;

                                      bool panning;
                                      QPointF lastPanPos;

                                   };



static const char* message_r_DT_start = R"(
You are in Delaunay Triangulation (DT) Mode.

In this mode you can add / remove points by left / right clicking on the canvas.
You can also see the code that runs everytime you insert or remove a point!

You can save the final triangulation to a file and later open it again and start from there!

HINT 1: Make sure to toggle View > Voronoi to check the corresponding Voronoi diagram!

HINT 2: Switch to Constrained Delaunay Triangulation in the Menu Bar!
)";


static const char* message_r_DT = R"(
Delaunay Triangulation (DT):

In this mode you can add / remove points by left / right clicking on the canvas.
You can also see the code that runs everytime you insert or remove a point (limited).

You can save the final triangulation to a file and later open it again and start from there!

HINT: Make sure to toggle View > Voronoi to check the corresponding Voronoi diagram!
)";

static const char* message_r_CDT = R"(
Constrained Delaunay Triangulation (CDT):

In this mode you can select two points and calculate the Shortest Path between them. Just click on the image and try.

HINT 1: Open the nodes' and the links' files first!

HINT 2: You can save the you shortest paths to files!
)";

static const char* message_r_open = R"(
Open your own files! This app accepts files:

In geojson format, you need one file with the nodes and one file with the links.
Check sample files in "<a href=\"https://www.geospatial.jp/ckan/dataset/0401\">www.geospatial.jp</a>.</p>"

In txt format, you also need both nodes and links' files.
Check the "Sample Files" in this App's folder!
)";


static std::string message_open = R"(<p>Open your own files! This app accepts files : 

<br>In geojson format, check sample files in <a href=\"https://www.geospatial.jp/ckan/dataset/0401\">www.geospatial.jp</a>.

<br>In txt format, check the 'Sample Files' in this App's folder.

<br><br>HINT: For CDT you need one file with the nodes and one file with the links!)";


static std::string s_deletepoint = R"( <p style = "color: rgb(255, 51, 221);"> (This function doesn't actually delete the point, it creates a new triangulation object with all the points except for the one you clicked on!) </p>
<br>    void deletepoint (QPoint mousePoint , InteractiveView_hs_triangulation * that) 
<br>                 {
<br>                     QPointF scenePoint = that->mapToScene(mousePoint); 
<br>                     Point queryPoint(scenePoint.x(), scenePoint.y()); 
<br>                     Delaunay::Vertex_handle closestVertex = nullptr;
<br>                     if (that->dt_.finite_vertices_begin() != that->dt_.finite_vertices_end())
<br>                         find_nearest_point(closestVertex, queryPoint, that->dt_);
<br>                         else
<br>                             QMessageBox::warning(nullptr, "Warning", "Network is empty. Please open file first.");
<br>    
<br>             
<br>                     if (closestVertex != nullptr && closestVertex->is_valid())
<br>                        {
<br>                            auto point = closestVertex->point();
<br>                            Triangulation dt_copy;
<br>                            for (auto vertex = that->dt_.finite_vertices_begin(); vertex != that->dt_.finite_vertices_end(); ++vertex)
<br>                                {
<br>                                    if (vertex->point() != point)
<br>                                       {
<br>                                           dt_copy.insert(vertex->point());
<br>                                       }
<br>                                }
<br>                            that->dt_ = dt_copy;
<br>                        }
<br>                        else if (closestVertex == nullptr)
<br>                                {
<br>                                 QMessageBox::warning(nullptr, "Warning", "No Point was found.");
<br>                                }
<br>                 }
)";


static std::string s_insertpoint = R"(
<br>void hs_triangulation::insertpoint (QPointF pos)
<br>     {
<br>         double x = CGAL::to_double(pos.x());
<br>         double y = CGAL::to_double(pos.y());
<br>         this->dt_->insert(Point_2(x, y));
<br>     }
)";



#endif // INTERACTIVE_VIEW_H
