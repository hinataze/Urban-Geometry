#include <include/interactive_view.h>
#include <include/hs_shortest_path.h>



static std::string s_deletepoint = R"(

// this function doesnt actually delete the point, it creates a new triangulation object with all the point except for the one you clicked on!
void deletepoint (QPoint mousePoint , InteractiveTriangulationView * that) 
{
    // Convert the clicked mouse point to scene coordinates
    QPointF scenePoint = that->mapToScene(mousePoint); 
    
    // Create a query point from the scene coordinates
    Point queryPoint(scenePoint.x(), scenePoint.y()); 

    // Variable to hold the closest vertex to the clicked point
    Delaunay::Vertex_handle closestVertex = nullptr;
    // Check if there are any finite vertices in the triangulation
    if (that->dt_.finite_vertices_begin() != that->dt_.finite_vertices_end())
    {
        // Find the nearest point in the triangulation to the query point
        find_nearest_point(closestVertex, queryPoint, that->dt_);
    }
    else
    {
        // Show a warning message if the network is empty
        QMessageBox::warning(nullptr, "Warning", "Network is empty. Please open file first.");
    }

    // Check if the closest vertex is valid
    if (closestVertex != nullptr && closestVertex->is_valid())
    {
        // Debugging output: print the coordinates of the vertex to be deleted
        qDebug() << "delete vertex x" << closestVertex->point().x() << " y " << closestVertex->point().y();

        // Store the point of the vertex to be deleted
        auto point = closestVertex->point();
        // Create a new triangulation object to hold the remaining points
        Triangulation dt_copy;
        // Iterate through all the finite vertices in the current triangulation
        for (auto vertex = that->dt_.finite_vertices_begin(); vertex != that->dt_.finite_vertices_end(); ++vertex)
        {
            // Add all points except for the one to be deleted to the new triangulation
            if (vertex->point() != point)
            {
                dt_copy.insert(vertex->point());
            }
        }
        // Replace the current triangulation with the new one
        that->dt_ = dt_copy;
        // Debugging output: confirm the vertex has been deleted
        qDebug() << "vertex x" << closestVertex->point().x() << " y " << closestVertex->point().y() << "deleted";
        qDebug() << "vertex x" << point.x() << " y " << point.y() << "deleted!!";
    }
    else if (closestVertex == nullptr)
    {
        // Show a warning message if no point was found
        QMessageBox::warning(nullptr, "Warning", "No Point was found.");
    }
})";


InteractiveTriangulationView::InteractiveTriangulationView (CDT &cdt, Delaunay& dt, MainWindow &mw, QMainWindow* parent):
                              QGraphicsView(parent), cdt_(cdt), dt_(dt), ref_mainwindow (mw)
                             {
                                qDebug() <<"InteractiveTriangulationView::InteractiveTriangulationView ";
                                grabGesture(Qt::PinchGesture);

                                this->ref_mainwindow.view_dt = this;
                                this->setScene(&ref_mainwindow.scene_);
                                ref_mainwindow.scene_.setBackgroundBrush(Qt::white);

                                this->setRenderHint(QPainter::Antialiasing);

                                // Create a GraphicsViewNavigation for panning and zooming
                                this->installEventFilter(&navigation);

                                //triangulationItems.back()->setScale(100);
                                QFont font2("Helvetica", 10 );
                                textEditL.setFont(font2);
                                textEditL.setPlainText("Code will be shown here.");
                                textEditL.setMinimumHeight(150);
                                
                                //textEditL.setBaseSize (100);
                                textEditL.setReadOnly(false); // Allow editing
                                textEditL.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

                                QFont font3("Helvetica", 10 );
                                textEditR.setFont(font3);
                                textEditR.setPlainText("Explanations will be shown here.");
                                textEditR.setFixedHeight(150);

                                ref_mainwindow.openFile(true);
                                this->update();

                                this->message_qs_DT = message_r_DT;
                                this->message_qs_CDT = message_r_CDT;
                                this->message_qs_open = message_r_open;

                                this->message_qs_DT_start = message_r_DT_start;

                                qDebug() << "InteractiveTriangulationView::InteractiveTriangulationView exit";

                                
                            }

void insertpoint(QPoint posi, InteractiveTriangulationView* that)
{
    qDebug() << "insertpoint" ;
    QPointF pos = that->mapToScene(posi);

    qDebug() << "insert point";
    // Convert the mouse click position to world coordinates
    
    double x = CGAL::to_double(pos.x());
    double y = CGAL::to_double(pos.y());

    //CHANGE DT
    if (that->flag_triangulation)
        that->dt_.insert(Point_2(x, y)); //activate if i want to edit the triangulation
}

void deletepoint(QPoint mousePoint , InteractiveTriangulationView * that)
{
    hs_update_log_all("deletepoint", s_deletepoint);

    QPointF scenePoint = that->mapToScene(mousePoint);

    // Find the closest vertex to the clicked point.
    Point queryPoint(scenePoint.x(), scenePoint.y());

    Delaunay::Vertex_handle closestVertex = nullptr;
    if (that->dt_.finite_vertices_begin() != that->dt_.finite_vertices_end())
    {
        find_nearest_point(closestVertex, queryPoint, that->dt_);
    }
    else
    {
        QMessageBox::warning(nullptr, "Warning", "Network is empty. Please open file first.");
    }

    if (closestVertex != nullptr && closestVertex->is_valid())
    {
        qDebug() << "delete vertex x" << closestVertex->point().x() << " y " << closestVertex->point().y();
        auto point = closestVertex->point();
        Triangulation dt_copy;
        for (auto vertex = that->dt_.finite_vertices_begin(); vertex != that->dt_.finite_vertices_end(); ++vertex)
        {
            if (vertex->point() != point)
            {
                dt_copy.insert(vertex->point());
            }
        }
        that->dt_ = dt_copy;
        qDebug() << "vertex x" << closestVertex->point().x() << " y " << closestVertex->point().y() << "deleted";
        qDebug() << "vertex x" << point.x() << " y " << point.y() << "deleted!!";
    }
    else  if (closestVertex == nullptr /*|| !closestVertex->is_valid()*/)
        QMessageBox::warning(nullptr, "Warning", "No Point was found.");

}

void define_source_target (QPoint mousePoint, InteractiveTriangulationView * that)
{
    QPointF scenePoint = that->mapToScene(mousePoint);

    // Find the closest vertex to the clicked point.
    Point queryPoint(scenePoint.x(), scenePoint.y());

    CDT::Vertex_handle  closestVertex = nullptr;
    if (that->cdt_.finite_vertices_begin() != that->cdt_.finite_vertices_end())
    {
        find_nearest_point(closestVertex, queryPoint, that->cdt_);
    }
    else
    {
        QMessageBox::warning(nullptr, "Warning", "Network is empty. Please open file first.");
    }

    if (closestVertex != nullptr)
    {
        that->source_target_shortestpath(closestVertex->point());
    }

}

void InteractiveTriangulationView::mousePressEvent(QMouseEvent* event)
                                  {
                                    qDebug() << " InteractiveTriangulationView::mousePressEvent " ;

                                    if (flag_edit_ctriangulation)
                                        {
                                          if (event->button() == Qt::LeftButton)
                                             {
                                               define_source_target(event->pos(), this);
                                             }                                   
                                        }
                                        else if (flag_edit_triangulation)
                                                {  
                                                   if (event->button() == Qt::LeftButton)
                                                       {
                                                        insertpoint(event->pos() , this); 
                                                       }
                                                       else if (event->button() == Qt::RightButton)
                                                               {  
                                                               deletepoint( event->pos(), this); 
                                                               }
                                                               else if (event->button() == Qt::MiddleButton)
                                                                       {
                                                                         panning = true;
                                                                         lastPanPos = event->pos();
                                                                         setCursor(Qt::ClosedHandCursor);
                                                                       } else
                                                                             {
                                                                              QGraphicsView::mousePressEvent(event);
                                                                             }
                                                }              

                                    scene_refresh();
                                    this->update();
                                    qDebug() << " InteractiveTriangulationView::mousePressEvent exit" ;
                                  }

void InteractiveTriangulationView::source_target_shortestpath (Point_2 pt)
                                  {
                                    qDebug() << "InteractiveTriangulationView::source_target_shortestpath" ;
                                    if(!flag_input_st)
                                      {
                                       flag_input_st = 1;
                                       vppt_source_target.first = pt; // Replace with your source point
                                       //qDebug() << "source point registered " << pt ;
                                      }
                                       else if (flag_input_st == 1)
                                               {
                                                 flag_input_st = 0;
                                                 vppt_source_target.second = pt; // Replace with your source point
                                                 //this->shortest_path();
                                                 shortest_path_2  (this->cdt_, this);

                                                 if (this->sp.sp_size())
                                                    this->pathindex = this->sp.v_cdt.size()-1;//cannot refer to v_cdt_gi because latest gi was not created yet

                                                 scene_refresh();
                                                 this->update();

                                                 //qDebug() << "target point registered " << pt ;
                                               }
                                  }

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation2;

// typedef CGAL::Constrained_Delaunay_triangulation_2<K,Tds> Triangulation2;
// trying to do a more general approach that will allow me to adapt for CDT also
// seems to work but have to insert constrained edge to source and target points otherwise they are not considered

bool InteractiveTriangulationView::event(QEvent* event)
     {
        //qDebug() << "InteractiveTriangulationView::event" ;
         if (event->type() == QEvent::Gesture)
            {
                QGestureEvent* gestureEvent = static_cast<QGestureEvent*>(event);
                if (QPinchGesture* pinch = qobject_cast<QPinchGesture*>(gestureEvent->gesture(Qt::PinchGesture))) {
                    qreal scaleFactor = pinch->scaleFactor();
                    scale (scaleFactor, scaleFactor);
                }
                return true;
            }
            //qDebug() << "InteractiveTriangulationView::event exit" ;
            return QGraphicsView::event(event);
     }


void InteractiveTriangulationView::wheelEvent(QWheelEvent *event)
    {
       qDebug() << "InteractiveTriangulationView::wheelEvent" ;
           if (event != nullptr && event->modifiers() & Qt::ControlModifier) {

           int delta = event->angleDelta().y();
           qreal scaleFactor = (delta > 0) ? 1.1 : 0.9;

           qDebug() << "scaleFactor"<< scaleFactor;
           //zoom(scaleFactor);
           scale (scaleFactor, scaleFactor);

               event->accept();
           } else {
               QGraphicsView::wheelEvent(event);
           }
    }


void InteractiveTriangulationView::keyPressEvent(QKeyEvent *event)
    {
      if (event->key() == Qt::Key_P || event->key() == Qt::Key_Equal)
         {
          if(pathindex < this->sp.v_cdt_gi.size())
           pathindex++;
          event->accept();
         }
         else if (event->key() == Qt::Key_O)
                 {
                   if(pathindex > 0)
                   // Decrement the count variable when the '-' key is pressed.
                   pathindex--;
                   event->accept();
                 }


      scene_refresh();
      this->update();
       qDebug() << "pathindex: " << pathindex;
    }



void InteractiveTriangulationView::mouseReleaseEvent(QMouseEvent* event)  {
       if (event->button() == Qt::MiddleButton) {
                   // Middle mouse button release for panning
                   panning = false;
                   setCursor(Qt::ArrowCursor);
       } else {
                   QGraphicsView::mouseReleaseEvent(event);
       }
    }

void InteractiveTriangulationView::mouseMoveEvent(QMouseEvent* event)  {
       if (panning) {
                   // Pan the view
                   QPointF delta = mapToScene(lastPanPos.toPoint()) - mapToScene(event->pos());
                   translate(delta.x(), delta.y());
                   lastPanPos = event->pos();
       } else {
                   QGraphicsView::mouseMoveEvent(event);
       }
    }

void InteractiveTriangulationView::scene_refresh()
                                   {
                                    qDebug() << "scene_refresh" ;

                                   this->scale_factor();
                                   qDebug() << "1" ;

                                   this->clear_graphicitems();
                                   qDebug() << "2" ;

                                   this->create_graphicitems();
                                   qDebug() << "3" ;

                                   this->scene_add_graphicitems();
                                   qDebug() << "4" ;

                                   //this->set_textitems();
                                   //this->scene_add_textitems();
                                   this->setpens();
                                   qDebug() << "5" ;

                                   if (flag_triangulation)
                                      this->update_log();
                                   }

void InteractiveTriangulationView::clear_graphicitems()
                                   {
                                    qDebug() << "InteractiveTriangulationView::clear_graphicitems "<< scalefactor ;

                                    //qDebug() << "1 ";
                                    for (auto &item : triangulationItems)
                                        {
                                          this->ref_mainwindow.scene_.removeItem(item);
                                          delete item;
                                        }
                                    //qDebug() << "1.1 ";
                                    if (!triangulationItems.empty())
                                       triangulationItems.clear();

                                    //qDebug() << "2 ";
                                    for (auto item : textItems)
                                        {
                                          this->ref_mainwindow.scene_.removeItem(item);
                                          delete item;
                                        }
                                    //qDebug() << "2.1 ";
                                    if (!textItems.empty())
                                       textItems.clear();

                                    //qDebug() << "3 ";
                                    if(ctriangulationItem!=nullptr)
                                    this->ref_mainwindow.scene_.removeItem(ctriangulationItem);
                                    //qDebug() << "3.1 ";
                                    delete ctriangulationItem;


                                     if(sp.elipse_source!=nullptr &&sp.elipse_target!=nullptr )
                                     {
                                     this->ref_mainwindow.scene_.removeItem(sp.elipse_source);
                                     this->ref_mainwindow.scene_.removeItem(sp.elipse_target);
                                     }

                                    qDebug() << "3.2 ";
                                    if(sp.sp_size())
                                      {
                                       //this->ref_mainwindow.scene_.removeItem(sp.v_cdt_gi.back());
                                         for (auto & a:  sp.v_cdt_gi)
                                             {
                                                 this->ref_mainwindow.scene_.removeItem(a);
                                                // delete a;
                                             }
                                         qDebug() << "3.3 ";
                                         sp.v_cdt_gi.clear();
                                      }

                                    //qDebug() << "4 ";
                                    for (auto item : ctextItems)
                                        {
                                          this->ref_mainwindow.scene_.removeItem(item);
                                          delete item;
                                        }

                                    //qDebug() << "4.1 ";
                                    if (!ctextItems.empty())
                                       ctextItems.clear();

                                    //qDebug() << "5 ";
                                    this->ref_mainwindow.scene_.removeItem(voronoiItem);

                                    //qDebug() << "5.1 ";
                                    delete voronoiItem;
                                   }

void InteractiveTriangulationView::create_graphicitems()
                                   {
                                    qDebug() << "InteractiveTriangulationView::create_graphicitems"<< scalefactor ;
                                    triangulationItems.push_back(new CGAL::Qt::TriangulationGraphicsItem<Delaunay> (&this->dt_, false));
                                    triangulationItems.back()->setZValue(0.1);

                                    ctriangulationItem = new CGAL::Qt::CTriangulationGraphicsItem<CDT> (&this->cdt_, true);
                                    ctriangulationItem->setZValue(0.5);

                                    if (this->sp.sp_size())
                                        for (auto & a : sp.v_cdt)
                                        {
                                            sp.v_cdt_gi.push_back(new CGAL::Qt::CTriangulationGraphicsItem<CDT> (&a, true));
                                            sp.v_cdt_gi.back()->setZValue(1);
                                        }

                                    voronoiItem = new CGAL::Qt::VoronoiGraphicsItem<Delaunay> (&this->dt_);
                                   }

void InteractiveTriangulationView::scene_add_graphicitems()
                                   {
                                    qDebug() << "InteractiveTriangulationView::scene_add_graphicitems" << scalefactor ;

                                    if (this->flag_triangulation)
                                       {
                                        if(!triangulationItems.empty())
                                          ref_mainwindow.scene_.addItem(triangulationItems.back());
                                          qDebug() << "added delaunay ";
                                       }

                                    if (this->flag_ctriangulation)
                                       {
                                         ref_mainwindow.scene_.addItem(ctriangulationItem);
                                         qDebug() << "added cdelaunay ";
                                       }

                                    if (this->flag_spath)
                                       {
                                        //for (auto & a: sp.v_cdt_gi)
                                         sp.elipse_source = new QGraphicsEllipseItem (vppt_source_target.first.x() - 5/scale_factor(), vppt_source_target.first.y() - 5/scale_factor(), 10/scale_factor(), 10/scale_factor());
                                         sp.elipse_target = new QGraphicsEllipseItem (vppt_source_target.second.x() - 5/scale_factor(), vppt_source_target.second.y() - 5/scale_factor(), 10/scale_factor(), 10/scale_factor());

                                        ref_mainwindow.scene_.addItem(sp.elipse_source);
                                        ref_mainwindow.scene_.addItem(sp.elipse_target);
                                       }

                                    if (this->flag_spath)
                                       {
                                        if (sp.sp_size() && pathindex < sp.v_cdt_gi.size())
                                           {
                                           // for (auto & a: sp.v_cdt_gi)
                                               {
                                                ref_mainwindow.scene_.addItem(sp.v_cdt_gi.at(pathindex));
                                               }
                                           }
                                       }

                                    if (this->flag_voronoi)
                                       {
                                         ref_mainwindow.scene_.addItem(voronoiItem);
                                         qDebug() << "added voronoi ";
                                       }

                                   }
void InteractiveTriangulationView::set_textitems ()
                                    {
                                      qDebug() << "InteractiveTriangulationView::set_textitems" << scalefactor ;
                                      set_textitems_dt();
                                      set_textitems_ct();
                                    }
void InteractiveTriangulationView::set_textitems_dt ()
                                    {
                                      qDebug() << "InteractiveTriangulationView::set_textitems_dt"<< scalefactor  ;

                                      QFont font("Helvetica", fontSize);
                                      font.setPointSizeF(font.pointSizeF() /* /scalefactor */);

                                      for (auto it = dt_.finite_vertices_begin(); it != dt_.finite_vertices_end(); ++it)
                                          {
                                            // qDebug() << "Point: " << it->point() ;

                                             std::ostringstream ssx;
                                             ssx << std::setprecision(4) << CGAL::to_double(it->point().x());
                                             std::ostringstream ssy;
                                             ssy << std::setprecision(4) << CGAL::to_double(it->point().y());

                                             QString text = "(" + QString::fromStdString(ssx.str()) + "," + QString::fromStdString(ssy.str())   + ")"  ;

                                             textItems.push_back(createTextItem(QPointF(it->point().x(), it->point().y()), text));
                                             textItems.back()->setFont(font);
                                          }

                                    }
void InteractiveTriangulationView::set_textitems_ct ()
                                    {
                                      qDebug() << "InteractiveTriangulationView::set_textitems_ct"<< scalefactor  ;

                                      QFont font("Helvetica", fontSize);

                                      font.setPointSizeF(font.pointSizeF() /* / scalefactor*/);

                                      size_t count = 0;
                                      for (auto it = cdt_.finite_vertices_begin(); it != cdt_.finite_vertices_end(); ++it, ++count)
                                          {
                                            //qDebug() << "Point: " << it->point() ;
                                            QString text = QString::fromStdString(std::to_string(count));
                                            ctextItems.push_back(createTextItem(QPointF(it->point().x(), it->point().y()), text));
                                            ctextItems.back()->setFont(font);
                                          }
                                    }

void InteractiveTriangulationView::scene_add_textitems()
                                   {
                                    qDebug() << "InteractiveTriangulationView::scene_add_textitems" << scalefactor ;
                                    if (this->flag_triangulation)
                                       {
                                        for (auto a : textItems)
                                            {
                                              ref_mainwindow.scene_.addItem(a);
                                              //qDebug() << "added point ";
                                            }
                                       }

                                    if (this->flag_ctriangulation)
                                       {
                                        for (auto a : ctextItems)
                                            {
                                              ref_mainwindow.scene_.addItem(a);
                                              //qDebug() << "added point ";
                                            }
                                       }
                                   }

void InteractiveTriangulationView::setpens ()
                                  {
                                    qDebug() << "InteractiveTriangulationView::setpens" << scalefactor ;

                                    qDebug() << "1" << scalefactor;
                                    //QColor(255,184,191, 255)
                                    //SHORTEST PATH
                                    QPen verticepen_gray(Qt::gray);
                                    verticepen_gray.setWidthF(3);
                                    verticepen_gray.setCosmetic(true);

                                    QPen edgespen_gray(Qt::gray);
                                    edgespen_gray.setStyle(Qt::DashLine);  // Line style: Solid
                                    edgespen_gray.setWidthF(1);
                                    edgespen_gray.setCosmetic(true);


                                    qDebug() << "2" << scalefactor;
                                    //TRIANGULATION
                                    QPen verticepen_dt(Qt::magenta);
                                    verticepen_dt.setWidthF(3);
                                    verticepen_dt.setCosmetic(true); //essential to be scale independent

                                    QPen edgespen_dt (Qt::black);
                                    edgespen_dt.setStyle(Qt::DashLine);  // Line style: Solid
                                    edgespen_dt.setWidthF(0.2);
                                    edgespen_dt.setCosmetic(true);


                                    qDebug() << "3" << scalefactor;

                                    if(!triangulationItems.empty())
                                      for (auto & a : triangulationItems)
                                          {
                                           if (a->visibleVertices())
                                               a->setVerticesPen(verticepen_dt);
                                           if (a->visibleEdges())
                                               a->setEdgesPen(edgespen_dt);
                                          }


                                    qDebug() << "4" << scalefactor;

                                    //CONSTRAINED
                                    QPen verticepen_cdt(Qt::cyan);
                                    verticepen_cdt.setWidthF(3);
                                    verticepen_cdt.setCosmetic(true);

                                    QPen edgespen_cdt(Qt::black);
                                    edgespen_cdt.setStyle(Qt::SolidLine);  // Line style: Solid
                                    edgespen_cdt.setWidthF(0.1);
                                    edgespen_cdt.setCosmetic(true);

                                    ctriangulationItem->setVerticesPen(verticepen_cdt);
                                    ctriangulationItem->setEdgesPen(edgespen_cdt);


                                    qDebug() << "5";

                                    //SHORTEST PATH
                                    QPen verticepen_sp(Qt::red);
                                    verticepen_sp.setWidthF(6);
                                    verticepen_sp.setCosmetic(true);
                                    qDebug() << "5.1";

                                    QPen edgespen_sp(Qt::red);
                                    edgespen_sp.setStyle(Qt::DashLine);  // Line style: Solid
                                    edgespen_sp.setWidthF(1);
                                    edgespen_sp.setCosmetic(true);
                                    qDebug() << "5.2";

                                    if (this->sp.sp_size())
                                       {
                                        for (auto & a : sp.v_cdt_gi)
                                           {
                                            a->setVerticesPen(verticepen_gray);
                                            a->setEdgesPen(edgespen_gray);
                                           }

                                        sp.v_cdt_gi.back()->setVerticesPen(verticepen_sp);
                                        sp.v_cdt_gi.back()->setEdgesPen(edgespen_sp);

                                        qDebug() << "5.3";

                                        sp.elipse_source->setPen(verticepen_sp);
                                        sp.elipse_target->setPen(verticepen_sp);
                                       }
                                   

                                 


                                    qDebug() << "6";

                                    //VORONOI
                                    QPen vedgespen(Qt::cyan);
                                    vedgespen.setStyle(Qt::SolidLine);  // Line style: Solid
                                    vedgespen.setWidthF(1);
                                    vedgespen.setCosmetic(true);

                                    if(voronoiItem != nullptr)
                                      {
                                       voronoiItem->setEdgesPen(vedgespen);
                                      }

                                    qDebug() << "InteractiveTriangulationView::setpens exit" ;
                                  }



/// EXECUTION LOG
///
size_t log_size = 0;

std::string literal = R"({
                           "model": "text-davinci-003",
                           "prompt": "explain under 100 tokens, in english(under 50 tokens),  then write in japanese: ",
                           "max_tokens": 100,
                           "temperature": 0
                       })";

static std::string replaceLFWithEscapeSequence(const std::string& input)
                                              {
                                                  std::string result;
                                                  for (char ch : input) {
                                                      if (ch == '\n') {
                                                          result += "\\u000A";
                                                      } else {
                                                          result += ch;
                                                      }
                                                  }
                                                  return result;
                                              }

//std::string literal = "{\n\"model\": \"text-davinci-003\",\n\"prompt\": \"explain: \",\n\"max_tokens\": 20,\n\"temperature\": 0\n}";

size_t position_insert = literal.find("ese: ");
size_t countlines = 0;
size_t count_updates = 0;
void InteractiveTriangulationView::update_log ()
                                   {
                                    qDebug() << "InteractiveTriangulationView::update_log: " << hs_vector_log.size()  ;
                                    if (flag_triangulation)
                                     if (int dif = hs_vector_log.size() - log_size > 0)
                                        {
                                         count_updates++;

                                         textEditL.setPlainText( textEditL.toPlainText() + QString::fromStdString("\n\n->PROCESS " + std::to_string(count_updates)));

                                         log_size = hs_vector_log.size();
                                         for (size_t i = hs_vector_log.size() - dif; i < hs_vector_log.size(); ++i)
                                         // for (size_t i = 0; i < hs_vector_log.size(); ++i)
                                             {
                                              if (i < hs_vps.size())
                                                 {
                                                     int j_ref = -1;
                                                     for (size_t j = 0; j < i; ++j)
                                                         {
                                                            if (hs_vps.at(j).second == hs_vps.at(i).second)
                                                                j_ref = j;
                                                         }

                                                     if (j_ref>=0)
                                                       {
                                                        //textEditL.setPlainText( "Function Applied: " + QString::fromStdString(hs_vector_log.at(i)) + "\n" + "This is the code of the function : \n" + QString::fromStdString(hs_vps.at(i).second) + "\n" + textEditL.toPlainText());
                                                        textEditL.setPlainText(textEditL.toPlainText() + "\n"+ QString::fromStdString(hs_vps.at(i).second) );
                                                        } else if (j_ref < 0)
                                                                 {
                                                                  qDebug()<< "using existing explanation " ;
                                                                  textEditL.setPlainText(textEditL.toPlainText() + "\n\n" + "Function Applied: " + QString::fromStdString(hs_vector_log.at(i)) + "\n" + "This is the code of the function being aplied: \n" + QString::fromStdString(hs_vps.at(i).second) + "\n" + QString::fromStdString(hs_vps.at(i).first));
                                                                 }
                                                 }
                                             }
                                        }
                                    textEditL.moveCursor(QTextCursor::End); // s
                                   }

QGraphicsTextItem*  InteractiveTriangulationView::createTextItem(QPointF position, QString text)
                                                 {
                                                   qDebug() << "InteractiveTriangulationView::createTextItem" ;
                                                   QGraphicsTextItem* textItem = new QGraphicsTextItem(text);
                                                   textItem->setPos(position);
                                                   return textItem;
                                                 }

double InteractiveTriangulationView::scale_factor ()
                                                  {
                                                    qDebug() << "InteractiveTriangulationView::scale_factor" ;
                                                    CGAL::Bbox_2 bb = CGAL::bbox_2(dt_.points_begin(), dt_.points_end());

                                                    double bounding_box_width = bb.xmax() - bb.xmin();
                                                    double bounding_box_height = bb.ymax() - bb.ymin();

                                                    double view_width = this->viewport()->width();
                                                    double view_height = this->viewport()->height();

                                                    double x_scale = view_width / bounding_box_width;
                                                    double y_scale = view_height / bounding_box_height;

                                                    // Choose the smaller of the two scales to ensure that the entire bounding box fits

                                                    scalefactor = std::min (x_scale, y_scale);
                                                    return scalefactor;
                                                  }


void InteractiveTriangulationView::sync_flag_sets(){
                                                    qDebug() << "InteractiveTriangulationView::sync_flag_sets" ;

                                                    //check inverse
                                                     if (flag_triangulation)
                                                        {
                                                           flag_ctriangulation = false;
                                                            this->ref_mainwindow.ctAction_v->setChecked(false);
                                                            //this->textEditL.setHidden(!textEditL.isHidden());
                                                            this->textEditL.setHidden(false);

                                                        }


                                                     if (flag_ctriangulation)
                                                        {
                                                           flag_triangulation = false;
                                                            this->ref_mainwindow.dtAction_v->setChecked(false);
                                                           // this->textEditL.setHidden(!textEditL.isHidden());
                                                            this->textEditL.setHidden(true);
                                                        }


                                                    if (flag_triangulation)
                                                       {
                                                         flag_edit_triangulation = true;
                                                         if (!flag_message_DT)
                                                             {
                                                               QMessageBox::information(nullptr, "Delaunay Triangulation", message_qs_DT);
                                                               flag_message_DT = true;
                                                             }
                                                        }
                                                         else if (!flag_triangulation)
                                                                 {
                                                                  flag_edit_triangulation = false;
                                                                  flag_voronoi = false;
                                                                  flag_edit_voronoi = false;
                                                                 }


                                                    if (flag_ctriangulation)
                                                       {
                                                         flag_edit_ctriangulation = true;
                                                         flag_spath = true;
                                                         flag_edit_spath =true;
                                                         if (!flag_message_CDT)
                                                             {
                                                              QMessageBox::information(nullptr, "Constrained Delaunay Triangulation", message_qs_CDT);
                                                              flag_message_CDT = true;
                                                             }
                                                       }
                                                       else if (!flag_ctriangulation)
                                                               {
                                                                   flag_edit_ctriangulation = false;
                                                                   flag_spath = false;
                                                                   flag_edit_spath = false;
                                                               }
                                                  }



