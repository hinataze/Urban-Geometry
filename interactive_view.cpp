#include <include/interactive_view.h>
#include <include/hs_triangulation.h>

// trying to do a  general approach that will allow me to adapt for CDT also
// seems to work but have to insert constrained edge to source and target points otherwise they are not considered




InteractiveView_hs_triangulation::InteractiveView_hs_triangulation (CDT &cdt, Delaunay& dt, MainWindow &mw, QMainWindow* parent):
                              QGraphicsView(parent), ref_mainwindow(mw)
                             {
                                qDebug() <<"InteractiveView_hs_triangulation::InteractiveView_hs_triangulation ";
                                grabGesture(Qt::PinchGesture);

                                setMouseTracking(true);


                                hst = std::make_shared <hs_triangulation>(cdt, dt);

                                this->ref_mainwindow.ptr_iv = this;
                                this->setScene(&ref_mainwindow.scene_);
                                ref_mainwindow.scene_.setBackgroundBrush(Qt::white);

                                this->setRenderHint(QPainter::Antialiasing);

                                this->installEventFilter(&navigation); // for panning and zooming

                                QFont font2("Helvetica", 10 );
                                textEditL.setFont(font2);
                                textEditL.setHtml("Code will be shown here.");
                                textEditL.setMinimumHeight(150);
                                
                                textEditL.setReadOnly(false); // Allow editing
                                textEditL.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

                                QFont font3("Helvetica", 10 );
                                textEditR.setFont(font3);
                                textEditR.setHtml("Explanations will be shown here.");
                                textEditR.setFixedHeight(150);

                                ref_mainwindow.openFile(true);
                                this->update();

                                this->message_qs_DT = message_r_DT;
                                this->message_qs_CDT = message_r_CDT;
                                this->message_qs_open = QString::fromStdString(message_open);

                                this->message_qs_DT_start = message_r_DT_start;

                                this->scale(0.001, 0.001);

                                qDebug() << "InteractiveView_hs_triangulation::InteractiveView_hs_triangulation exit";
   
                            }

void InteractiveView_hs_triangulation::paintEvent(QPaintEvent* event)
{
    QGraphicsView::paintEvent(event);
    QPainter painter(viewport());
    painter.setPen(Qt::black);
    painter.drawText(cursorPosition.x(), cursorPosition.y(), QString::number(canvasPosition.x()) + ", " + QString::number(canvasPosition.y()));
}


void InteractiveView_hs_triangulation::mousePressEvent(QMouseEvent* event)
                                  {
                                    qDebug() << " InteractiveView_hs_triangulation::mousePressEvent " ;

                                    if (flag_edit_ctriangulation)
                                        {
                                          if (event->button() == Qt::LeftButton)
                                             {
                                               hst->f1_define_source_target(event->pos(), this);
                                             }                                   
                                        }
                                        else if (flag_edit_triangulation)
                                                {  
                                                   if (event->button() == Qt::LeftButton)
                                                       {
                                                       flag_click_lr = 0;
                                                       QPointF pos = this->mapToScene(event->pos());

                                                       //CHANGE DT
                                                       if (this->flag_triangulation)
                                                        hst->insertpoint(pos); 
                                                       }
                                                       else if (event->button() == Qt::RightButton)
                                                               {  
                                                               flag_click_lr = 1;
                                                               hst->deletepoint( event->pos(), this); 
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
                                    qDebug() << " InteractiveView_hs_triangulation::mousePressEvent exit" ;
                                  }

bool InteractiveView_hs_triangulation::event (QEvent* event)
     {
        //qDebug() << "InteractiveView_hs_triangulation::event" ;
         if (event->type() == QEvent::Gesture)
            {
                QGestureEvent* gestureEvent = static_cast<QGestureEvent*>(event);
                if (QPinchGesture* pinch = qobject_cast<QPinchGesture*>(gestureEvent->gesture(Qt::PinchGesture))) {
                    qreal scaleFactor = pinch->scaleFactor();
                    scale (scaleFactor, scaleFactor);
                }
                return true;
            }
            //qDebug() << "InteractiveView_hs_triangulation::event exit" ;
            return QGraphicsView::event(event);
     }
void InteractiveView_hs_triangulation::wheelEvent (QWheelEvent *event)
    {
       qDebug() << "InteractiveView_hs_triangulation::wheelEvent" ;
           if (event != nullptr && event->modifiers() & Qt::ControlModifier) {

           int delta = event->angleDelta().y();
           qreal scaleFactor = (delta > 0) ? 1.1 : 0.9;

           qDebug() << "scaleFactor"<< scaleFactor;
           //zoom(scaleFactor);

           this->scale (scaleFactor, scaleFactor);

               event->accept();
           } else {
               QGraphicsView::wheelEvent(event);
           }

           qDebug() << "wheelEvent exit";
    }
void InteractiveView_hs_triangulation::keyPressEvent (QKeyEvent *event)
    {
      if (event->key() == Qt::Key_P || event->key() == Qt::Key_Equal)
         {
          if(pathindex < this->hst->sp.v_cdt_gi.size())
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
void InteractiveView_hs_triangulation::mouseReleaseEvent (QMouseEvent* event)  {
       if (event->button() == Qt::MiddleButton) {
                   // Middle mouse button release for panning
                   panning = false;
                   setCursor(Qt::ArrowCursor);
       } else {
                   QGraphicsView::mouseReleaseEvent(event);
       }
    }
void InteractiveView_hs_triangulation::mouseMoveEvent (QMouseEvent* event)  {

       cursorPosition = event->pos();
       canvasPosition = mapToScene(cursorPosition);

       update();

       if (panning) {
                   // Pan the view
                   QPointF delta = mapToScene(lastPanPos.toPoint()) - mapToScene(event->pos());
                   translate(delta.x(), delta.y());
                   lastPanPos = event->pos();
       } else {
                   QGraphicsView::mouseMoveEvent(event);
       }
    }
void InteractiveView_hs_triangulation::scene_refresh ()
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
                                      this->update_text();
                                   }
void InteractiveView_hs_triangulation::clear_graphicitems ()
                                   {
                                    qDebug() << "InteractiveView_hs_triangulation::clear_graphicitems "<< scalefactor ;

                                    //qDebug() << "1 ";
                                    for (auto &item : hst->triangulationItems)
                                        {
                                          this->ref_mainwindow.scene_.removeItem(item.get());
                                          //delete item;
                                          item = nullptr;
                                        }
                                    //qDebug() << "1.1 ";
                                    if (!hst->triangulationItems.empty())
                                        hst->triangulationItems.clear();

                                    //qDebug() << "2 ";
                                    for (auto item : hst->textItems)
                                        {
                                          this->ref_mainwindow.scene_.removeItem(item);
                                          delete item;
                                        }
                                    //qDebug() << "2.1 ";
                                    if (!hst->textItems.empty())
                                        hst->textItems.clear();

                                    //qDebug() << "3 ";
                                    if(hst->ctriangulationItem!=nullptr)
                                    this->ref_mainwindow.scene_.removeItem(hst->ctriangulationItem.get());
                                    //qDebug() << "3.1 ";
                                    //delete hst->ctriangulationItem; // std::shared_ptr <CGAL::Qt::CTriangulationGraphicsItem<CDT>>
                                    hst->ctriangulationItem = nullptr;


                                     if(hst->sp.elipse_source!=nullptr && hst->sp.elipse_target!=nullptr )
                                     {
                                     this->ref_mainwindow.scene_.removeItem(hst->sp.elipse_source);
                                     this->ref_mainwindow.scene_.removeItem(hst->sp.elipse_target);
                                     }

                                    qDebug() << "3.2 ";
                                    if(hst->sp.sp_size())
                                      {
                                       //this->ref_mainwindow.scene_.removeItem(sp.v_cdt_gi.back());
                                         for (auto & a: hst->sp.v_cdt_gi)
                                             {
                                                 this->ref_mainwindow.scene_.removeItem(a);
                                                // delete a;
                                             }
                                         qDebug() << "3.3 ";
                                         hst->sp.v_cdt_gi.clear();
                                      }

                                    //qDebug() << "4 ";
                                    for (auto item : hst->ctextItems)
                                        {
                                          this->ref_mainwindow.scene_.removeItem(item);
                                          delete item;
                                        }

                                    //qDebug() << "4.1 ";
                                    if (!hst->ctextItems.empty())
                                        hst->ctextItems.clear();

                                    qDebug() << "debug voronoi";
                                    if (hst->voronoiItem != nullptr)
                                    this->ref_mainwindow.scene_.removeItem(hst->voronoiItem.get());

                                    //qDebug() << "5.1 ";
                                   // delete
                                    hst->voronoiItem = nullptr;
                                   }
void InteractiveView_hs_triangulation::create_graphicitems ()
                                   {
                                    qDebug() << "InteractiveView_hs_triangulation::create_graphicitems"<< scalefactor ;
                                    hst->triangulationItems.push_back(std::make_shared <CGAL::Qt::TriangulationGraphicsItem<Delaunay>> (this->hst->dt_.get(), false));
                                    hst->triangulationItems.back()->setZValue(0.1);

                                    hst->ctriangulationItem = std::make_shared < CGAL::Qt::CTriangulationGraphicsItem<CDT>> (this->hst->cdt_.get(), true);
                                    hst->ctriangulationItem->setZValue(0.5);

                                    if (this->hst->sp.sp_size())
                                        for (auto & a : hst->sp.v_cdt)
                                        {
                                            hst->sp.v_cdt_gi.push_back(new CGAL::Qt::CTriangulationGraphicsItem<CDT> (&a, true));
                                            hst->sp.v_cdt_gi.back()->setZValue(1);
                                        }

                                    if(this->hst->dt_ != nullptr)
                                    hst->voronoiItem = std::make_shared <CGAL::Qt::VoronoiGraphicsItem<Delaunay>> (this->hst->dt_.get());
                                   }
void InteractiveView_hs_triangulation::scene_add_graphicitems ()
                                   {
                                    qDebug() << "InteractiveView_hs_triangulation::scene_add_graphicitems" << scalefactor ;

                                    if (this->flag_triangulation)
                                       {
                                        if(!hst->triangulationItems.empty())
                                          ref_mainwindow.scene_.addItem(hst->triangulationItems.back().get());
                                          qDebug() << "added delaunay ";
                                       }

                                    if (this->flag_ctriangulation)
                                       {
                                         ref_mainwindow.scene_.addItem(hst->ctriangulationItem.get());
                                         qDebug() << "added cdelaunay ";
                                       }

                                    if (this->flag_spath)
                                       {
                                        //for (auto & a: sp.v_cdt_gi)
                                        hst->sp.elipse_source = new QGraphicsEllipseItem (hst->vppt_source_target.first.x() - 5/scale_factor(), hst->vppt_source_target.first.y() - 5/scale_factor(), 10/scale_factor(), 10/scale_factor());
                                        hst->sp.elipse_target = new QGraphicsEllipseItem (hst->vppt_source_target.second.x() - 5/scale_factor(), hst->vppt_source_target.second.y() - 5/scale_factor(), 10/scale_factor(), 10/scale_factor());

                                        ref_mainwindow.scene_.addItem(hst->sp.elipse_source);
                                        ref_mainwindow.scene_.addItem(hst->sp.elipse_target);
                                       }

                                    if (this->flag_spath)
                                       {
                                        if (hst->sp.sp_size() && pathindex < hst->sp.v_cdt_gi.size())
                                           {
                                           // for (auto & a: sp.v_cdt_gi)
                                               {
                                                ref_mainwindow.scene_.addItem(hst->sp.v_cdt_gi.at(pathindex));
                                               }
                                           }
                                       }

                                    if (this->flag_voronoi)
                                       {
                                        qDebug() << "debug voronoi";

                                        if(hst->voronoiItem != nullptr)
                                         ref_mainwindow.scene_.addItem(hst->voronoiItem.get());
                                         qDebug() << "added voronoi ";
                                       }

                                   }
void InteractiveView_hs_triangulation::set_textitems ()
                                    {
                                      qDebug() << "InteractiveView_hs_triangulation::set_textitems" << scalefactor ;
                                      set_textitems_dt();
                                      set_textitems_ct();
                                    }
void InteractiveView_hs_triangulation::set_textitems_dt ()
                                    {
                                      qDebug() << "InteractiveView_hs_triangulation::set_textitems_dt"<< scalefactor  ;

                                      QFont font("Helvetica", fontSize);
                                      font.setPointSizeF(font.pointSizeF() /* /scalefactor */);

                                      for (auto it = this->hst->dt_->finite_vertices_begin(); it != hst->dt_->finite_vertices_end(); ++it)
                                          {
                                            // qDebug() << "Point: " << it->point() ;

                                             std::ostringstream ssx;
                                             ssx << std::setprecision(4) << CGAL::to_double(it->point().x());
                                             std::ostringstream ssy;
                                             ssy << std::setprecision(4) << CGAL::to_double(it->point().y());

                                             QString text = "(" + QString::fromStdString(ssx.str()) + "," + QString::fromStdString(ssy.str())   + ")"  ;

                                             hst->textItems.push_back(createTextItem(QPointF(it->point().x(), it->point().y()), text));
                                             hst->textItems.back()->setFont(font);
                                          }

                                    }
void InteractiveView_hs_triangulation::set_textitems_ct ()
                                    {
                                      qDebug() << "InteractiveView_hs_triangulation::set_textitems_ct"<< scalefactor  ;

                                      QFont font("Helvetica", fontSize);

                                      font.setPointSizeF(font.pointSizeF() /* / scalefactor*/);

                                      size_t count = 0;
                                      for (auto it = hst->cdt_->finite_vertices_begin(); it != hst->cdt_->finite_vertices_end(); ++it, ++count)
                                          {
                                            //qDebug() << "Point: " << it->point() ;
                                            QString text = QString::fromStdString(std::to_string(count));
                                            hst->ctextItems.push_back(createTextItem(QPointF(it->point().x(), it->point().y()), text));
                                            hst->ctextItems.back()->setFont(font);
                                          }
                                    }
void InteractiveView_hs_triangulation::scene_add_textitems()
                                   {
                                    qDebug() << "InteractiveView_hs_triangulation::scene_add_textitems" << scalefactor ;
                                    if (this->flag_triangulation)
                                       {
                                        for (auto a : hst->textItems)
                                            {
                                              ref_mainwindow.scene_.addItem(a);
                                              //qDebug() << "added point ";
                                            }
                                       }

                                    if (this->flag_ctriangulation)
                                       {
                                        for (auto a : hst->ctextItems)
                                            {
                                              ref_mainwindow.scene_.addItem(a);
                                              //qDebug() << "added point ";
                                            }
                                       }
                                   }
void InteractiveView_hs_triangulation::setpens ()
                                  {
                                    qDebug() << "InteractiveView_hs_triangulation::setpens" << scalefactor ;

                                    //SHORTEST PATH
                                    QPen verticepen_gray(Qt::gray);
                                    verticepen_gray.setWidthF(3);
                                    verticepen_gray.setCosmetic(true);

                                    QPen edgespen_gray(Qt::gray);
                                    edgespen_gray.setStyle(Qt::DashLine);  // Line style: Solid
                                    edgespen_gray.setWidthF(1);
                                    edgespen_gray.setCosmetic(true);


                                    //TRIANGULATION
                                    QPen verticepen_dt(Qt::magenta);
                                    verticepen_dt.setWidthF(3);
                                    verticepen_dt.setCosmetic(true); //essential to be scale independent

                                    QPen edgespen_dt (Qt::black);
                                    edgespen_dt.setStyle(Qt::DashLine);  // Line style: Solid
                                    edgespen_dt.setWidthF(0.2);
                                    edgespen_dt.setCosmetic(true);

                                    if(!hst->triangulationItems.empty())
                                      for (auto & a : hst->triangulationItems)
                                          {
                                           if (a->visibleVertices())
                                               a->setVerticesPen(verticepen_dt);
                                           if (a->visibleEdges())
                                               a->setEdgesPen(edgespen_dt);
                                          }

                                    //CONSTRAINED
                                    QPen verticepen_cdt(Qt::cyan);
                                    verticepen_cdt.setWidthF(3);
                                    verticepen_cdt.setCosmetic(true);

                                    QPen edgespen_cdt(Qt::black);
                                    edgespen_cdt.setStyle(Qt::SolidLine);  // Line style: Solid
                                    edgespen_cdt.setWidthF(0.1);
                                    edgespen_cdt.setCosmetic(true);

                                    hst->ctriangulationItem->setVerticesPen(verticepen_cdt);
                                    hst->ctriangulationItem->setEdgesPen(edgespen_cdt);

                                    //SHORTEST PATH
                                    QPen verticepen_sp(Qt::red);
                                    verticepen_sp.setWidthF(6);
                                    verticepen_sp.setCosmetic(true);

                                    QPen edgespen_sp(Qt::red);
                                    edgespen_sp.setStyle(Qt::DashLine);  // Line style: Solid
                                    edgespen_sp.setWidthF(1);
                                    edgespen_sp.setCosmetic(true);

                                    if (this->hst->sp.sp_size())
                                       {
                                        for (auto & a : hst->sp.v_cdt_gi)
                                           {
                                            a->setVerticesPen(verticepen_gray);
                                            a->setEdgesPen(edgespen_gray);
                                           }

                                        hst->sp.v_cdt_gi.back()->setVerticesPen(verticepen_sp);
                                        hst->sp.v_cdt_gi.back()->setEdgesPen(edgespen_sp);


                                       hst->sp.elipse_source->setPen(verticepen_sp);
                                       hst->sp.elipse_target->setPen(verticepen_sp);
                                       }
                                  
                         
                                    //VORONOI
                                    QPen vedgespen(Qt::cyan);
                                    vedgespen.setStyle(Qt::SolidLine);  // Line style: Solid
                                    vedgespen.setWidthF(1);
                                    vedgespen.setCosmetic(true);

                                    if(hst->voronoiItem != nullptr)
                                      {
                                        hst->voronoiItem->setEdgesPen(vedgespen);
                                      }

                                    qDebug() << "InteractiveView_hs_triangulation::setpens exit" ;
                                  }


void InteractiveView_hs_triangulation::f1_1_source_target_shortestpath(Point_2 pt)
{
    qDebug() << "InteractiveView_hs_triangulation::f1_1_source_target_shortestpath";
    if (!hst->flag_input_st) //source
    {
        hst->flag_input_st = 1;
        hst->vppt_source_target.first = pt; // Replace with your source point
    }
    else if (hst->flag_input_st == 1) //target
    {
        hst->flag_input_st = 0;
        hst->vppt_source_target.second = pt; // Replace with your source point
        hst->f1_shortest_path(*this->hst->cdt_, this);

        if (this->hst->sp.sp_size())
            this->pathindex = this->hst->sp.v_cdt.size() - 1;//cannot refer to v_cdt_gi because latest gi was not created yet

        scene_refresh();
        this->update();
    }
}

void InteractiveView_hs_triangulation::save_to_file_paths(QString filePath_ids, QString filePath_coord)
    {
        qDebug() << "InteractiveView_hs_triangulation::save_to_file_paths";
        this->hst->f1_7_save_to_file_paths(filePath_ids, filePath_coord);
    }


/// LOG
//size_t position_insert = literal.find("ese: ");
size_t countlines = 0;
size_t count_updates = 0;
void InteractiveView_hs_triangulation::update_text()
                                   {
                                    qDebug() << "InteractiveView_hs_triangulation::update_text: " << hs_vector_log.size()  ;
                             
                                    int dif = hs_vector_log.size() - log_size;

                                    std::string s_click_type;
                                    
                                    if (log_size)
                                        s_click_type = flag_click_lr ? "Right" : "Left";

                                   
                                    if (!flag_start && flag_triangulation)
                                     if (dif > 0)
                                        {
                                        
                                         if(count_updates)
                                         textEditL.setHtml( textEditL.toHtml() + QString::fromStdString("<br><br><b>" + s_click_type + " Click "+ std::to_string(count_updates) +"<\b>"));
                                         log_size = hs_vector_log.size();

                                         size_t count_function = 0;
                                         for (size_t i_function = hs_vector_log.size() - dif-1; i_function < hs_vector_log.size(); ++i_function)
                                             {
                                              if (i_function < hs_vps.size())
                                                 {
                                                     int j_ref = -1;
                                                     for (size_t j = 0; j < i_function; ++j)
                                                         {
                                                            if (hs_vps.at(j).second == hs_vps.at(i_function).second)
                                                                j_ref = j;
                                                         }

                                                     if (j_ref>=0)
                                                       {
                                                         textEditL.setHtml("    " + textEditL.toHtml() + "<br><br><i>   " + QString::number(count_function) + " Function Applied:</i><br>" + QString::fromStdString(hs_vps.at(i_function).second));

                                                     } else if (j_ref < 0)
                                                                 {
                                                                  qDebug()<< "using existing explanation " ;
                                                                  textEditL.setHtml("    " + textEditL.toHtml() +
                                                                      "<br><br><i>  " + QString::number(count_function) + " Function:</i><br>" +
                                                                      QString::fromStdString(hs_vps.at(i_function).second) + "<br>" +
                                                                      QString::fromStdString(hs_vps.at(i_function).first));
                                                     }
                                                 }
                                              count_function++;
                                             }
                                         count_updates++;
                                        }

                                   textEditL.moveCursor(QTextCursor::End); // s

                                   flag_start = false;
                                   }

QGraphicsTextItem*  
       InteractiveView_hs_triangulation::createTextItem(QPointF position, QString text)
                                                 {
                                                   qDebug() << "InteractiveView_hs_triangulation::createTextItem" ;
                                                   QGraphicsTextItem* textItem = new QGraphicsTextItem(text);
                                                   textItem->setPos(position);
                                                   return textItem;
                                                 }

double InteractiveView_hs_triangulation::scale_factor ()
                                                  {
                                                    qDebug() << "InteractiveView_hs_triangulation::scale_factor" ;
                                                    CGAL::Bbox_2 bb = CGAL::bbox_2(hst->dt_->points_begin(), hst->dt_->points_end());

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

void InteractiveView_hs_triangulation::sync_flag_sets(){
                                                    qDebug() << "InteractiveView_hs_triangulation::sync_flag_sets" ;

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


static std::string replaceLFWithEscapeSequence(const std::string& input)
{
    std::string result;
    for (char ch : input) {
        if (ch == '\n') {
            result += "\\u000A";
        }
        else {
            result += ch;
        }
    }
    return result;
}

std::string literal = R"({
                           "model": "text-davinci-003",
                           "prompt": "explain under 100 tokens, in english(under 50 tokens),  then write in japanese: ",
                           "max_tokens": 100,
                           "temperature": 0
                       })";

//std::string literal = "{\n\"model\": \"text-davinci-003\",\n\"prompt\": \"explain: \",\n\"max_tokens\": 20,\n\"temperature\": 0\n}";