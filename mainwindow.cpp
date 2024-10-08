#include <include/mainwindow.h>
#include <include/interactive_view.h>
#include <include/hs_triangulation.h>

#include <QDesktopServices>


MainWindow::MainWindow(QWidget *parent, QApplication *app) : QMainWindow(parent) , ui(new Ui::MainWindow)
            {
              qDebug() << "MainWindow::MainWindow" ;
              ui->setupUi(this);
              scene_.setSceneRect(0,0, this->geometry().width(), this->geometry().height());


              QCursor customCursor =  Qt::CrossCursor;//(QPixmap( "/Users/hs/Desktop/GEO_ALG/haat.png") ); //https://doc.qt.io/qt-6/qcursor.html
              this->setCursor(customCursor);


              // Create a menu bar
              this->menu_bar = menuBar();
              this->menuBar()->setContextMenuPolicy(Qt::PreventContextMenu);

             //file
             this->menu_file = menu_bar->addMenu(QStringLiteral("File"));
             this->openAction = menu_file->addAction(QStringLiteral("Open"));
             this->saveAction = menu_file->addAction(QStringLiteral("Save"));
             this-> exitAction = menu_file->addAction(QStringLiteral("Exit"));
             QObject::connect(openAction, &QAction::triggered ,app, [&]() {this->openFile(false);});
             QObject::connect(saveAction, &QAction::triggered, app, [&]() {this->save_files();});
             QObject::connect(exitAction, &QAction::triggered, app, &QApplication::quit);

              //mode
             this->menu_mode = menu_bar->addMenu(QStringLiteral("Mode"));
             this->dtAction_v = menu_mode->addAction("Delaunay Triangulation");
             this->ctAction_v = menu_mode->addAction("Constrained Delaunay Triangulation");
             QObject::connect(dtAction_v, &QAction::triggered, app, [&]() {this->turn_flag_view(this->ptr_iv->flag_triangulation);});
             QObject::connect(ctAction_v, &QAction::triggered, app, [&]() {this->turn_flag_view(this->ptr_iv->flag_ctriangulation);});


              //view
             this->menu_view = menu_bar->addMenu(QStringLiteral("View"));
             QAction* fitviewAction = menu_view->addAction("Fit View");
             QObject::connect(fitviewAction, &QAction::triggered, app, [&]() {this->fit_view();});
             this->view_voronoi_action = menu_view->addAction("Voronoi");
             QObject::connect(view_voronoi_action, &QAction::triggered, app, [&]() {this->turn_flag_view(this->ptr_iv->flag_voronoi);});



              //sync with constructor
              
              dtAction_v->setCheckable(true);
              ctAction_v->setCheckable(true);

              dtAction_v->setChecked(true);
              ctAction_v->setChecked(false);

              view_voronoi_action->setCheckable(true);
              view_voronoi_action->setChecked(false);


              this->helpAction = new QAction("Help", this);
              connect(helpAction, &QAction::triggered, this, &MainWindow::openEmailClient);

              // Add the "Help" action to the menu bar
              QMenu *helpMenu = menuBar()->addMenu("Help");
              helpMenu->addAction(helpAction);

            }

MainWindow::~MainWindow()
            {
              qDebug() << "MainWindow::~MainWindow" ;
              //ui->setupUi(this);
              delete ui;
            }


//doesnt create/delete items just add/removes from scene
void MainWindow::turn_flag_view(bool& flag)
{
    qDebug() << " MainWindow::turn_flag_view";
    flag = flag != false ? false : true;

    if (&flag == &this->ptr_iv->flag_ctriangulation && this->ptr_iv->flag_ctriangulation == true)
        this->ptr_iv->flag_triangulation = false; // because in sync_flag_sets flag_triangulation comes first dont need to set that

    if (&flag == &this->ptr_iv->flag_voronoi && this->ptr_iv->flag_voronoi == true && this->ptr_iv->flag_triangulation == false)
    {
        this->ptr_iv->flag_voronoi = false;
        QMessageBox::warning(nullptr, "Warning", "Voronoi diagram is only available for DT. Select DT mode first.");
    }


    //TRIANGULATION
    if (ptr_iv->hst->dt_->vertices_begin() != ptr_iv->hst->dt_->vertices_end())
    {
        if (this->ptr_iv->flag_triangulation)
        {
            this->scene_.addItem(this->ptr_iv->hst->triangulationItems.back().get());
            for (auto item : ptr_iv->hst->textItems)
                this->scene_.addItem(item);
        }
        else if (!this->ptr_iv->flag_triangulation)
        {
            for (auto item : ptr_iv->hst->triangulationItems)
                this->scene_.removeItem(item.get());

            for (auto item : ptr_iv->hst->textItems)
                this->scene_.removeItem(item);
        }

        //CONSTRAINED
        if (ptr_iv->hst->cdt_->vertices_begin() != ptr_iv->hst->cdt_->vertices_end())
        {
            if (this->ptr_iv->flag_ctriangulation)
            {
                this->scene_.addItem(ptr_iv->hst->ctriangulationItem.get());
                for (auto item : ptr_iv->hst->ctextItems)
                    this->scene_.addItem(item);
            }
            else if (!this->ptr_iv->flag_ctriangulation)
            {
                this->scene_.removeItem(ptr_iv->hst->ctriangulationItem.get());

                for (auto item : ptr_iv->hst->ctextItems)
                    this->scene_.removeItem(item);
            }
        }

        //SHORTEST PATH
        if (ptr_iv->hst->sp.sp_size())
        {
            if (this->ptr_iv->flag_spath)
            {
                for (auto& a : ptr_iv->hst->sp.v_cdt_gi)
                    this->scene_.addItem(a);

                if (ptr_iv->hst->sp.elipse_source != nullptr && ptr_iv->hst->sp.elipse_target != nullptr)
                {
                    this->scene_.addItem(ptr_iv->hst->sp.elipse_source);
                    this->scene_.addItem(ptr_iv->hst->sp.elipse_target);
                }
            }
            else if (!this->ptr_iv->flag_spath)
            {
                for (auto& a : ptr_iv->hst->sp.v_cdt_gi)
                    this->scene_.removeItem(a);

                this->scene_.removeItem(ptr_iv->hst->sp.elipse_source);
                this->scene_.removeItem(ptr_iv->hst->sp.elipse_target);
            }
        }

        //VORONOI
        if (this->ptr_iv->flag_voronoi && this->ptr_iv->hst->voronoiItem != nullptr)
            this->scene_.addItem(this->ptr_iv->hst->voronoiItem.get());
        else if (!this->ptr_iv->flag_voronoi && this->ptr_iv->hst->voronoiItem != nullptr)
            this->scene_.removeItem(this->ptr_iv->hst->voronoiItem.get());
    }

    if (&flag != &this->ptr_iv->flag_voronoi) // inside sync_flag_sets we toggle textview for dt
        this->ptr_iv->sync_flag_sets();
    this->ptr_iv->scene_refresh();
    this->ptr_iv->update();
}
void MainWindow::turn_flag_edit(bool& flag)
{
    qDebug() << " MainWindow::turn_flag_edit";
    flag = flag != false ? false : true;
}
void MainWindow::view_refresh()
{
    qDebug() << "MainWindow::view_refresh";
    if (ptr_iv != nullptr)
        ptr_iv->scene_refresh();

    qDebug() << "MainWindow::view_refresh exit";
}
void MainWindow::fit_view()
{
    qDebug() << "MainWindow::fit_view ";
    this->ptr_iv->fitInView(this->ptr_iv->hst->triangulationItems.back().get(), Qt::KeepAspectRatio); //the last scale to view is the on
}
void MainWindow::openEmailClient()
{
    QString emailTo = "solheiro@iis.u-tokyo.ac.jp";
    QString subject = "Urban Geometry Help Request";
    QString body = "Dear Hermenegildo,\n\nI need help with: [Describe your issue here] \n\n Thanks.";

    // Create the mailto URL
    QString mailtoUrl = "https://mail.google.com/mail/?view=cm&fs=1&to=" + emailTo + "&su=" + subject + "&body=" + body;


    // Open the default email client
    QDesktopServices::openUrl(QUrl(mailtoUrl));
}


void MainWindow::openFile (bool flag_start)
                {
                   qDebug() << "MainWindow::openFile() ";
                   QString filePath_vertices= "";
                   QString filePath_edges= "";

                 
                  if (flag_start)
                     {
                      // remove // to open files on start
                     //filePath_vertices  = QCoreApplication::applicationDirPath() +"/Sample Files/渋谷/node.geojson";
                     //filePath_edges= QCoreApplication::applicationDirPath() + "/Sample Files/渋谷/link.geojson";
                     //
                     //qDebug() << ">>>>>>>>>>>>>>>>>>>"  << filePath_vertices;
                     //qDebug() << ">>>>>>>>>>>>>>>>>>>"  << filePath_edges;

                     }
                      else
                         {
                           QMessageBox::information(nullptr, "Open File", this->ptr_iv->message_qs_open);
                           QMessageBox::information(nullptr, "Information", "Open the file containing the nodes.");
                           filePath_vertices = QFileDialog::getOpenFileName(this, "Open File", QDir::homePath());

                           if (!this->ptr_iv->flag_triangulation)
                           {
                               QMessageBox::information(nullptr, "Information", "Now open the file containing the links.");
                               filePath_edges = QFileDialog::getOpenFileName(this, "Open File", QDir::homePath());
                           }
                         }

                  if(filePath_vertices!= "")
                  {
                      if (!isGeoJson(filePath_vertices))
                      {
                          qDebug() << "txt files";
                          std::vector <std::pair <double, double>> vpts;
                          std::vector <std::pair <double, double>> vegs;

                          vpts = openfile_txt(filePath_vertices);
                          qDebug() << "vpts size" << vpts.size();


                          if (filePath_edges!= "")
                          if (!filePath_edges.isEmpty())
                              vegs = openfile_txt(filePath_edges);

                          qDebug() << "vegs size" << vegs.size();
                          if (!vpts.empty())
                          this->openfile_insertpoints_txt(vpts);


                          if (!vegs.empty())
                              this->openfile_insertedges_txt(vegs, vpts);
                      }
                      else if (isGeoJson(filePath_vertices))
                      {
                          qDebug() << "geojson files";
                          std::map <QString, std::pair <QString, QString>> map_id_nodes;
                          std::map <QString, std::pair <QString, QString>> map_id_links;

                          map_id_nodes = openfile_GeoJson(filePath_vertices);
                          
                          if (filePath_edges != "")
                          map_id_links = openfile_GeoJson(filePath_edges);
                          this->openfile_insertpoints_gjson(map_id_nodes);
                          this->openfile_insertedges_gjson(map_id_links, map_id_nodes);
                      }
                  }

                  qDebug() << " now 3 " << ptr_iv;
                  if ( ptr_iv != nullptr)
                  {
                      qDebug() << " ptr_iv != nullptr "  << ptr_iv;
                      this->view_refresh();
                      this->ptr_iv->fitInView(this->ptr_iv->hst->triangulationItems.back().get(), Qt::KeepAspectRatio); //the last scale to view is the on
                  }

                  qDebug() << "MainWindow::openFile exit " ;
                }
std::vector <std::pair <double, double>> MainWindow::openfile_txt  (QString path)
                                                   {
                                                        qDebug() << "MainWindow::openfile_txt " ;
                                                        qDebug() << "string is " << QString::fromStdString(path.toStdString());
                                                        std::vector<std::pair<double, double>> vec;

                                                        QFile file(path);
                                                        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
                                                        {
                                                            QTextStream in(&file);
                                                            while (!in.atEnd())
                                                                  {
                                                                   QString line = in.readLine();
                                                                   QStringList tokens = line.split(' '); // Assuming points are space-separated
                                                                       if (tokens.size() == 2)
                                                                          {
                                                                           double x = tokens[0].toDouble();
                                                                           double y = tokens[1].toDouble();
                                                                           vec.push_back({x, y});

                                                                           qDebug() << x;
                                                                           qDebug() << y;
                                                                         }
                                                                  }
                                                                  file.close();
                                                        } else {
                                                                 qDebug() << "file open error" ;
                                                               }

                                                        qDebug() << "openfile exit" ;
                                                        return vec;
                                                    }
 std::map <QString, std::pair <QString, QString>> MainWindow::openfile_GeoJson  (QString filePath)
                                                   {
                                                    qDebug() << "MainWindow::openfile_GeoJson "  ;
                                                    std::map <QString, std::pair <QString, QString>> map_id_data;

                                                    QFile file(filePath);
                                                       if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
                                                          {
                                                            qDebug() << "Failed to open the file.";
                                                            return map_id_data;
                                                          }

                                                       QByteArray jsonData = file.readAll();
                                                       file.close();

                                                       QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
                                                       if (jsonDoc.isNull() || !jsonDoc.isObject())
                                                          {
                                                            qDebug() << "Invalid JSON format.";
                                                            return map_id_data;
                                                          }

                                                       QJsonObject jsonObj = jsonDoc.object();
                                                       if (jsonObj.contains("features") && jsonObj["features"].isArray())
                                                       {
                                                           size_t count_nodes = 0;
                                                           size_t count_links = 0;

                                                           QJsonArray features = jsonObj["features"].toArray();


                                                           for (const QJsonValue &featureValue : features) {
                                                               if (featureValue.isObject())
                                                                  {
                                                                   QJsonObject feature = featureValue.toObject();
                                                                   if (feature.contains("properties") && feature["properties"].isObject())
                                                                      {
                                                                          QJsonObject properties = feature["properties"].toObject();

                                                                          if (properties.contains ("node_id") && properties.contains("lat") && properties.contains("lon"))
                                                                             {
                                                                               QString node_id = properties["node_id"].toString();
                                                                               QString lat = QString::fromStdString(std::to_string(properties["lat"].toDouble()));
                                                                               QString lon = QString::fromStdString(std::to_string(properties["lon"].toDouble()));

                                                                               //qDebug()<< "Node ID: " << node_id << "Latitude: " << lat << ", Longitude: " << lon;
                                                                               std::pair <QString, QString> coord (lat, lon);
                                                                               map_id_data[node_id] = coord;
                                                                               count_nodes++;
                                                                             }
                                                                               else if (properties.contains ("link_id") && properties.contains("start_id") && properties.contains("end_id"))
                                                                                       {
                                                                                         QString link_id = properties["link_id"].toString();
                                                                                         QString start_id = properties["start_id"].toString();
                                                                                         QString end_id = properties["end_id"].toString();

                                                                                         //qDebug()<< "Link ID: " << link_id << "Start ID: " << start_id << ", End ID: " << end_id;
                                                                                         std::pair <QString, QString> beids (start_id, end_id);
                                                                                         map_id_data[link_id] = beids;
                                                                                         count_links++;
                                                                                       }
                                                                      }
                                                               }
                                                           }

                                                          //qDebug() <<"IMPORTED NODES "<< count_nodes ;
                                                          //qDebug() <<"IMPORTED LINKS "<<  count_links ;
                                                       }
                                                        return map_id_data;
                                                   }
void MainWindow::openfile_insertpoints_txt (std::vector <std::pair <double, double>> &vpts)
                {
                  qDebug()<< "MainWindow::openfile_insertpoints_txt";
                   ptr_iv->hst->dt_->clear();
                   for (auto & a : vpts)
                       {
                           ptr_iv->hst->dt_->insert (Point_2 (a.first, a.second));
                           ptr_iv->hst->cdt_->insert (Point_2 (a.first, a.second));
                       }
                }
void MainWindow::openfile_insertpoints_gjson (std::map <QString, std::pair <QString, QString>>  &vpts)
                {
                  qDebug()<< "MainWindow::openfile_insertpoints_gjson";
                   ptr_iv->hst->dt_->clear();
                   for (auto & a : vpts)
                       {
                          ptr_iv->hst->dt_->insert (Point_2 (a.second.first.toDouble(), a.second.second.toDouble()));
                          ptr_iv->hst->cdt_->insert (Point_2(a.second.first.toDouble(), a.second.second.toDouble()));
                         // qDebug() << Point_2 (a.second.first.toDouble(), a.second.second.toDouble()) ;
                          //qDebug() << Point_2 (a.second.first.toDouble(), a.second.second.toDouble()) ;
                       }
                   int numberOfPoints = 0;
                      for (auto v =  ptr_iv->hst->dt_->finite_vertices_begin(); v !=  ptr_iv->hst->dt_->finite_vertices_end(); ++v)
                      {
                          numberOfPoints++;
                      }

                      int numberOfPointscdt = 0;
                         for (auto v =  ptr_iv->hst->cdt_->finite_vertices_begin(); v !=  ptr_iv->hst->cdt_->finite_vertices_end(); ++v)
                         {
                             numberOfPointscdt++;
                         }

                      qDebug() << "Number of points: " << numberOfPoints ;
                      qDebug() << "Number of points in the CDT: " << numberOfPointscdt ;

                }
void MainWindow::openfile_insertedges_txt (std::vector <std::pair <double, double>> &vegs, std::vector <std::pair <double, double>> &vpts)
                 {
                  qDebug()<< "MainWindow::openfile_insertedges_txt";
                  ptr_iv->hst->cdt_->clear();

                  for (auto & a : vegs)
                      {
                        size_t index_source = a.first;
                        size_t index_target = a.second;
                        qDebug()<< "index_source " << index_source << " index_target " << index_target ;

                        Point_2 point_source = Point_2(vpts.at(index_source).first, vpts.at(index_source).second);
                        Point_2 point_target = Point_2(vpts.at(index_target).first, vpts.at(index_target).second);
                        //qDebug() << "point_source " <<  point_source << " point_target " << point_target ;

                       try
                         {
                           ptr_iv->hst->cdt_->insert_constraint (point_source, point_target);
                         }
                         catch (const std::exception& e)
                               {
                                qDebug() << "constrained edges dont follow the rules. Please correct and try again." ;
                               // break;
                               }
                       }
                  qDebug() << "is valid? " << ptr_iv->hst->cdt_->is_valid() ;
                 }
void MainWindow::openfile_insertedges_gjson (std::map <QString, std::pair <QString, QString>> &vegs, std::map <QString, std::pair <QString, QString>>  &vpts)
                 {
                  qDebug()<< "MainWindow::openfile_insertedges_geojson";
                  ptr_iv->hst->cdt_->clear();

                  qDebug() <<"nodes to insert: "<< vegs.size() << ", links to insert: " <<vpts.size() ;

                  for (auto & a : vegs)
                      {

                        QString id_source = a.second.first;
                        QString id_target = a.second.second;
                        //qDebug()<< "id_source " << id_source << " id_target " << id_target ;

                        Point_2 point_source = Point_2(vpts[id_source].first.toDouble(), vpts[id_source].second.toDouble());
                        Point_2 point_target = Point_2(vpts[id_target].first.toDouble(), vpts[id_target].second.toDouble());
                       // qDebug() << "point_source " <<  point_source << " point_target " << point_target ;

                       try
                         {
                           ptr_iv->hst->cdt_->insert_constraint (point_source, point_target);
                         }
                         catch (const std::exception& e)
                               {
                                qDebug() << "constrained edges dont follow the rules. Please correct and try again." ;
                                //break;
                               }
                       }
                  qDebug() << "is valid? " << ptr_iv->hst->cdt_->is_valid() ;
                 }
bool MainWindow::isGeoJson(const QString filePath)
{
    qDebug() << "MainWindow::isGeoJson ";
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open the file.";
        return false;
    }

    QTextStream stream(&file);
    QString line = stream.readLine();
    file.close();

    // Check if the file has the GeoJSON extension (.geojson)
    if (filePath.toLower().endsWith(".geojson"))
    {
        qDebug() << "is geojson";
        return true;
    }

    //// Check if the first line contains a GeoJSON marker (e.g., {"type": "FeatureCollection")
    //if (line.contains("\"type\": \"FeatureCollection\"")) {
    //    return true;
    //}

    qDebug() << "not geojson";
    return false;
}

void MainWindow::save_files()
                 {
                  qDebug() << "MainWindow::save_files" ;

                  if (this->ptr_iv->flag_triangulation)
                  {
                      QMessageBox::information(nullptr, "Information", "Please chose the path to save the present delaunay triangulation. (Paths with Japanese characters are currently not supported!)");
                      QString filePath = QFileDialog::getSaveFileName(this, "Save Delaunay Triangulation", "", "Text Files (*.txt)");

                      if (filePath.isEmpty())
                      {
                          std::cerr << "path is empty" ;
                          return; // User canceled the dialog
                      }
                      else 
                         {
                           qDebug() << "Selected file path:" << filePath;
                         }

                  std::ofstream outputFile(filePath.toStdString());

                  outputFile << std::fixed << std::setprecision(20);

                  if (!outputFile.is_open())
                     {
                      qWarning() << "!!Error opening the output file.";
                      return;
                     }

                  for (Delaunay::Finite_vertices_iterator it = ptr_iv->hst->dt_->finite_vertices_begin(); it != ptr_iv->hst->dt_->finite_vertices_end(); ++it)
                     {
                      Point_2 p = it->point();
                      outputFile << p.x() << " " << p.y() << "\n";
                     }
                  outputFile.close();
                      QMessageBox::information(nullptr, "Information", "Delaunay triangulation has been saved.");
                  }

                  else if (!this->ptr_iv->flag_triangulation)
                  {
                      QMessageBox::information(nullptr, "Information", "Please chose the path to save the present Constrained delaunay triangulation (nodes).");
                      QString filePath_v = QFileDialog::getSaveFileName(this, "Save Delaunay Triangulation", "", "Text Files (*.txt)");

                      if (filePath_v.isEmpty())
                      {
                      return; // User canceled the dialog
                      }

                      std::ofstream outputFile_v(filePath_v.toStdString());

                      outputFile_v << std::fixed << std::setprecision(20);
                      if (!outputFile_v.is_open())
                         {
                         qWarning() << "Error opening the output file.";
                         return;
                         }


                      for (auto it_v = ptr_iv->hst->cdt_->finite_vertices_begin(); it_v != ptr_iv->hst->cdt_->finite_vertices_end(); ++it_v)
                          {
                          outputFile_v << it_v->point().x() << " " << it_v->point().y() <<"\n" ;
                          }
                      outputFile_v.close();
                      QMessageBox::information(nullptr, "Information", "CDT vertices have been saved.");

                      //cdt constrains (vertex indices)
                      QMessageBox::information(nullptr, "Information", "Please chose the path to save the present Constrained delaunay triangulation (links).");
                      QString filePath = QFileDialog::getSaveFileName(this, "Save Delaunay Triangulation", "", "Text Files (*.txt)");

                      if (filePath.isEmpty())
                      {
                      return; // User canceled the dialog
                      }

                      std::ofstream outputFile(filePath.toStdString());

                      outputFile << std::fixed << std::setprecision(20);
                      if (!outputFile.is_open())
                          {
                            qWarning() << "Error opening the output file.";
                            return;
                          }


                      for (auto e = ptr_iv->hst->cdt_->finite_edges_begin(); e != ptr_iv->hst->cdt_->finite_edges_end(); ++e)
                          {
                            if (ptr_iv->hst->cdt_->is_constrained(*e))
                               {
                                 for (int i = 1; i <= 2; ++i)
                                     {
                                       auto v = e->first->vertex((e->second + i) % 3);
                                       //Point vertex_point = v->point();
                                       //outputFile << vertex_point << " " ;

                                       size_t count_vertex_index = 0;
                                       for (auto vert = ptr_iv->hst->cdt_->finite_vertices_begin(); vert != ptr_iv->hst->cdt_->finite_vertices_end(); ++vert)
                                           {
                                            if (vert->point() == v->point())
                                               {
                                                outputFile << count_vertex_index;
                                                if (i == 1)
                                                    outputFile << " ";
                                               }
                                            count_vertex_index++;
                                           }
                                     }
                                 outputFile<<"\n";
                               }
                          }
                          outputFile.close();
                      QMessageBox::information(nullptr, "Information", "CDT constraints have been saved.");

                  //COORDINATES
                  QMessageBox::information(nullptr, "Information", "Please chose the file to save the shortest paths' nodes (coordinates).");
                  QString filePath_coord = QFileDialog::getSaveFileName (ptr_iv, "Save Shortest paths", "", "Text Files (*.txt)");

                  // INDICES
                  QMessageBox::information(nullptr, "Information", "Now chose the file to save the shortest paths' nodes (indices).");
                  QString filePath_ids = QFileDialog::getSaveFileName(ptr_iv, "Save Shortest paths", "", "Text Files (*.txt)");

                  this->save_to_file_paths(filePath_ids, filePath_coord);
                  QMessageBox::information(nullptr, "Information", "Files have been saved.");
                      }

                 }

void MainWindow::save_to_file_paths (QString filePath_ids, QString filePath_coord)
{ 
    qDebug() << "MainWindow::save_to_file_paths";

    this->ptr_iv->save_to_file_paths (filePath_ids, filePath_coord);
}









