#include <include/hs_shortest_path.h>





typedef K::FT FT;


void hs_triangulation::insertpoint (QPointF pos)
{
    qDebug() << "hs_triangulation::insertpoint";
   
    // Convert the mouse click position to world coordinates
    double x = CGAL::to_double(pos.x());
    double y = CGAL::to_double(pos.y());
    
    this->dt_.insert(Point_2(x, y)); //activate if i want to edit the triangulation
}



void hs_triangulation::deletepoint (QPoint mousePoint, InteractiveView_hs_triangulation* that)
{
    qDebug() << "hs_triangulation::deletepoint";

    hs_update_log_all("deletepoint", s_deletepoint);

    QPointF scenePoint = that->mapToScene(mousePoint);

    // Find the closest vertex to the clicked point.
    Point_2 queryPoint(scenePoint.x(), scenePoint.y());

    Delaunay::Vertex_handle closestVertex = nullptr;
    if (this->dt_.finite_vertices_begin() != this->dt_.finite_vertices_end())
    {
        this->f1_2_find_nearest_point(closestVertex, queryPoint, this->dt_);
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
        for (auto vertex = this->dt_.finite_vertices_begin(); vertex != this->dt_.finite_vertices_end(); ++vertex)
        {
            if (vertex->point() != point)
            {
                dt_copy.insert(vertex->point());
            }
        }
        this->dt_ = dt_copy;
        qDebug() << "vertex x" << closestVertex->point().x() << " y " << closestVertex->point().y() << "deleted";
        qDebug() << "vertex x" << point.x() << " y " << point.y() << "deleted!!";
    }
    else  if (closestVertex == nullptr /*|| !closestVertex->is_valid()*/)
        QMessageBox::warning(nullptr, "Warning", "No Point was found.");

}

void  hs_triangulation::f1_define_source_target (QPoint mousePoint, InteractiveView_hs_triangulation* that)
{
    qDebug() << "hs_triangulation::f1_define_source_target";
    QPointF scenePoint = that->mapToScene(mousePoint);

    // Find the closest vertex to the clicked point.
    Point_2 queryPoint(scenePoint.x(), scenePoint.y());
    CDT::Vertex_handle  closestVertex = nullptr;
    if (this->cdt_.finite_vertices_begin() != this->cdt_.finite_vertices_end())
    {
        f1_2_find_nearest_point(closestVertex, queryPoint, this->cdt_);
    }
    else
    {
        QMessageBox::warning(nullptr, "Warning", "Network is empty. Please open file first.");
    }

    if (closestVertex != nullptr)
    {
        that->f1_1_source_target_shortestpath(closestVertex->point());
    }
}

void hs_triangulation::f1_2_find_nearest_point (CDT::Vertex_handle  & cdt_handle_nearest, Point_2 pt , CDT  & cdt_)
                        {
                          qDebug() << "hs_triangulation::f1_2_find_nearest_point constrained" ;

                         Point_2 nearest_point;
                         CDT cdt = cdt_;

                          Point_2 query_point(pt.x(), pt.y());

                          cdt_handle_nearest = nullptr;

                          FT min_distance = std::numeric_limits<FT>::max();

                          for (auto e = cdt.finite_edges_begin(); e != cdt.finite_edges_end(); ++e)
                             {
                              if (cdt.is_constrained(*e))
                                 {
                                   for (int i = 1; i <= 2; ++i)
                                       {
                                         auto v = e->first->vertex((e->second + i) % 3);
                                         Point_2 vertex_point = v->point();
                                         FT distance = CGAL::squared_distance(query_point, vertex_point);
                                         if (distance < min_distance)
                                            {
                                              //qDebug() << "4.3";
                                              min_distance = distance;
                                              nearest_point = vertex_point;
                                              cdt_handle_nearest = v;
                                            }
                                       }
                                 }
                             }

                          if (cdt_handle_nearest != nullptr)
                             {
                              nearest_point = cdt_handle_nearest->point();
                               qDebug() << "Nearest point inside the triangulation: (" << nearest_point.x() << ", " << nearest_point.y() << ")" ;
                               qDebug() << "Squared distance to nearest point: " << min_distance ;
                              // qDebug() << "point: " << nearest_point << " handle: " << (cdt_handle_nearest != nullptr? "valid": "null") ;
                               qDebug() << "InteractiveView_hs_triangulation::f1_2_find_nearest_point exit ";
                             }
                             else
                                QMessageBox::warning(nullptr, "Warning", "No point found.");
                          }

void hs_triangulation::f1_2_find_nearest_point (Triangulation::Vertex_handle  & dt_handle_nearest, Point_2 pt , Triangulation  & dt_)
                        {
                             qDebug() << "hs_triangulation::f1_2_find_nearest_point delaunay" ;

                             Point_2 nearest_point;
                             Delaunay dt = dt_;

                             Point_2 query_point(pt.x(), pt.y());

                             dt_handle_nearest = nullptr;

                             FT min_distance = std::numeric_limits<FT>::max();

                             for (auto e = dt.finite_edges_begin(); e != dt.finite_edges_end(); ++e)
                             {
                                       for (int i = 1; i <= 2; ++i)
                                       {
                                            auto v = e->first->vertex((e->second + i) % 3);
                                            Point_2 vertex_point = v->point();
                                            FT distance = CGAL::squared_distance(query_point, vertex_point);
                                            if (distance < min_distance)
                                            {
                                              //qDebug() << "4.3";
                                              min_distance = distance;
                                              nearest_point = vertex_point;
                                              dt_handle_nearest = v;
                                            }
                                       }
                             }

                             if (dt_handle_nearest != nullptr)
                             {
                                       nearest_point = dt_handle_nearest->point();
                                qDebug() << "Nearest point inside the triangulation: (" << nearest_point.x() << ", " << nearest_point.y() << ")" ;
                                qDebug() << "Squared distance to nearest point: " << min_distance ;
                                //qDebug() << "point: " << nearest_point << " handle: " << ( dt_handle_nearest->is_valid()? "valid": "null") ;
                                qDebug() << "InteractiveView_hs_triangulation::f1_2_find_nearest_point exit ";
                             }
                             else
                                QMessageBox::warning(nullptr, "Warning", "No point found.");
                        }

void  hs_triangulation::f1_shortest_path (CDT & cdt_, InteractiveView_hs_triangulation *ptr_view)
                      {
                       qDebug() << "hs_triangulation::f1_shortest_path" ;
                       CDT triangulation = cdt_;  // Initialize your CGAL Delaunay triangulation

                       //source and target points
                       qDebug() << "1 is valid?"<< cdt_.is_valid();
                       CDT::Vertex_handle source;//auto is Triangulation::Vertex_handle source
                       CDT::Vertex_handle target;
                       f1_1_identify_target(source, target, this->vppt_source_target, triangulation);

                       //nearest point for constrained edges
                       qDebug() << "2";
                       CDT::Vertex_handle  closest_source = nullptr;
                       CDT::Vertex_handle  closest_target = nullptr;
                       if (source != nullptr && target != nullptr )
                          {
                         
                          f1_2_find_nearest_point (closest_source, source->point(), triangulation);
                       
                          f1_2_find_nearest_point (closest_target, target->point(), triangulation);

                          Graph graph;
                          std::map<CDT::Vertex_handle, Boost_Vertex> vertex_map;
                       
                          f1_3_add_vertices (graph, vertex_map, triangulation);


                           f1_4_add_edges (graph, vertex_map, triangulation);
                           // Specify the source vertex for Dijkstra's algorithm
                           // Vertex source_vertex = vertex_map[triangulation.finite_vertices_begin()];
                           Boost_Vertex source_vertex = vertex_map[source];
                           Boost_Vertex target_vertex = vertex_map[target];
                           std::vector<Boost_Vertex> predecessors(boost::num_vertices(graph));
                           std::vector<double> distances(boost::num_vertices(graph));

                           auto simplifycall = boost::predecessor_map(boost::make_iterator_property_map(predecessors.begin(), boost::get(boost::vertex_index, graph))).distance_map(boost::make_iterator_property_map(distances.begin(),boost::get(boost::vertex_index, graph)));

                           boost::dijkstra_shortest_paths(
                                                          graph,
                                                          source_vertex,
                                                          boost::predecessor_map(boost::make_iterator_property_map(predecessors.begin(), boost::get(boost::vertex_index, graph))).distance_map(boost::make_iterator_property_map(distances.begin(),boost::get(boost::vertex_index, graph))));


                           bool flag_invalid_path = false;
                           f1_5_print_shortest_path_only (flag_invalid_path, source, target_vertex, source_vertex, distances, predecessors, vertex_map, triangulation);
                           f1_6_make_triangulation(flag_invalid_path, target_vertex, source_vertex, distances, predecessors, vertex_map, triangulation);
                          }
                      }

void hs_triangulation::f1_1_identify_target (CDT::Vertex_handle &source, CDT::Vertex_handle &target, std::pair <Point_2, Point_2> ppt, CDT & triangulation)
                       {
                         qDebug() << "hs_triangulation::f1_1_identify_target" ;
                         for (auto v = triangulation.finite_vertices_begin(); v != triangulation.finite_vertices_end(); ++v)
                             {
                               if (v->point() == ppt.first)
                                   source = v;
                               else if (v->point() == ppt.second)
                                    target = v;
                              }
                       }

void hs_triangulation::f1_3_add_vertices (Graph & graph, std::map<CDT::Vertex_handle, Boost_Vertex> & vertex_map, CDT & triangulation)
                    {
                        qDebug() << " hs_triangulation::f1_3_add_vertices" ;

                      std::set<CDT::Vertex_handle> added_vertices;
                      for (CDT::Finite_edges_iterator e = triangulation.finite_edges_begin(); e != triangulation.finite_edges_end(); ++e) // also can use constrained_edges_begin
                          {
                              if (triangulation.is_constrained(*e))
                                  {
                                    CDT::Vertex_handle v1 = e->first->vertex((e->second + 1) % 3);
                                    CDT::Vertex_handle v2 = e->first->vertex((e->second + 2) % 3);

                                     if (added_vertices.find(v1) == added_vertices.end()) //not in set
                                        {
                                          added_vertices.insert(v1);
                                          vertex_map[v1] = boost::add_vertex(graph);
                                         // qDebug() << "Added point: " << v1->point() ;
                                        }
                                         else
                                             {
                                             //  qDebug() << "Already added point v1: " << v1->point() ;
                                             }

                                     if (added_vertices.find(v2) == added_vertices.end())
                                        {
                                         added_vertices.insert(v2);
                                       
                                         vertex_map[v2] = boost::add_vertex(graph);
                                        // qDebug() << "Added point: " << v2->point() ;
                                        }
                                        else
                                            {
                                              //qDebug() << "Already added point v2: " << v2->point() ;
                                            }
                                  }
                          }
                    }

void hs_triangulation::f1_4_add_edges (Graph & graph, std::map<CDT::Vertex_handle, Boost_Vertex> & vertex_map, CDT & triangulation)
                  {
                   qDebug() << "hs_triangulation::f1_4_add_edges" ;
                   std::set<CDT::Edge> added_edges;


                   for (auto e = triangulation.constrained_edges_begin(); e != triangulation.constrained_edges_end(); ++e)
                      {
                        CDT::Vertex_handle v1 = e->first->vertex((e->second + 1) % 3);
                        CDT::Vertex_handle v2 = e->first->vertex((e->second + 2) % 3);

                        // Check if this edge has already been added to the se
                        if (added_edges.find(*e) == added_edges.end())
                           {
                              // Add the edge to the set to mark it as added
                              added_edges.insert(*e);

                              double edge_weight = std::sqrt(CGAL::to_double(CGAL::squared_distance(v1->point(), v2->point()))); //euclidean distance not squared distance!
                              boost::add_edge(vertex_map[v1], vertex_map[v2], edge_weight, graph);

                              //qDebug() << "Added edge between vertices: " << v1->point() << " and " << v2->point() ;
                           } else {
                                   //qDebug() << "Already added edge between vertices: " << v1->point() << " and " << v2->point() << " already added." ;
                                  }
                      }

                   qDebug() << "added eges count "<< added_edges.size() ;
                   }

void hs_triangulation::f1_5_print_shortest_paths_all (Boost_Vertex & source_vertex, std::vector<double> & distances, std::vector<Boost_Vertex> & predecessors, std::map<CDT::Vertex_handle, Boost_Vertex> & vertex_map)
                                {
                                   qDebug() << "hs_triangulation::f1_5_print_shortest_paths_all" ;
                                   for (auto it = vertex_map.begin(); it != vertex_map.end(); ++it)
                                       {
                                         if (it->first->is_valid())
                                            {
                                              qDebug() << "print this!?";
                                              Boost_Vertex target_vertex = it->second;
                                              qDebug() << "Shortest paths from source "  << source_vertex << "to target " << target_vertex << ": ";
                                              std::vector<Boost_Vertex> path;
                                              Boost_Vertex current_vertex = target_vertex;
                                              while (current_vertex != source_vertex)
                                                    {
                                                      path.push_back(current_vertex);
                                                      current_vertex = predecessors[current_vertex];
                                                    }
                                              path.push_back(source_vertex);
                                              for (int i = path.size() - 1; i >= 0; --i)
                                                  {
                                                      qDebug() << path[i];
                                                      if (i > 0)
                                                         {
                                                           qDebug() << " -> ";
                                                         }
                                                  }
                                              qDebug() << " (Distance: " << distances[target_vertex] << ")" ;
                                          }
                                       }
                                }

//!!!test if there is any shortest path otherwise endless loop when not in continuous constraints!!
void hs_triangulation::f1_5_print_shortest_path_only (bool & flag_invalid_path, CDT::Vertex_handle & vh_source, Boost_Vertex & bv_target, Boost_Vertex & bv_source, std::vector<double> & distances, std::vector<Boost_Vertex> & predecessors, std::map<CDT::Vertex_handle, Boost_Vertex> & vertex_map, CDT & triangulation)
                                 {
                                   qDebug() << "hs_triangulation::f1_5_print_shortest_path_only " ;
                                   qDebug() << " ("<< vh_source->point().x() << ", " << vh_source->point().y() << ")"<< " to target: (" << this->vppt_source_target.second.x() << ", " << this->vppt_source_target.second.y() << ")\n";
                                   Boost_Vertex bv_current = bv_target;
                                   while (bv_current != bv_source)
                                         {
                                           CDT::Vertex_handle cdt_handle_vertex = triangulation.finite_vertices_begin();
                                           for (auto v = triangulation.finite_vertices_begin(); v != triangulation.finite_vertices_end(); ++v)
                                               {
                                                   if (vertex_map[v] == bv_current)
                                                      {
                                                        cdt_handle_vertex = v;
                                                        qDebug() << "[" << bv_current << "]" <<" -> (" << cdt_handle_vertex->point().x() << ", " << cdt_handle_vertex->point().y() << ")";
                                                      }
                                               }
                                           if (bv_current != predecessors[bv_current])
                                              {
                                                  bv_current = predecessors[bv_current];
                                              }
                                              else
                                                  {
                                                    QMessageBox::warning(nullptr, "Warning", "No valid path found.");
                                                    flag_invalid_path = true;
                                                    break;
                                                  }
                                         }
                                         qDebug() << " (Distance: " << distances[bv_target] << ")" ;

                                   qDebug() << "Make result triangulation: " ;
                                 }

void hs_triangulation::f1_6_make_triangulation (bool & flag_invalid_path, Boost_Vertex & bv_target, Boost_Vertex & bv_source, std::vector<double> & distances, std::vector<Boost_Vertex> & predecessors, std::map<CDT::Vertex_handle, Boost_Vertex> & vertex_map, CDT & triangulation)
      {
         qDebug() << "hs_triangulation::f1_6_make_triangulation" ;

          if (!flag_invalid_path)
          {
         bool flag_first = true;
             {
               size_t count_path = 0;
               CDT pathTriangulation;
               Boost_Vertex bv_current = bv_target;
               CDT::Point previouspoint;
               while (bv_current != bv_source)
                     { qDebug() << "count_path " << count_path++ ;
                       CDT::Vertex_handle cdt_handle_vertex = nullptr;
                       for (auto v = triangulation.finite_vertices_begin(); v != triangulation.finite_vertices_end(); ++v)
                           {
                             if (vertex_map[v] == bv_current)
                                {
                                  cdt_handle_vertex = v;
                                  pathTriangulation.insert(cdt_handle_vertex->point());

                                  //qDebug() << "inserted point in sp triangulation." << cdt_handle_vertex->point() ;
                                   if (!flag_first)
                                      {
                                        try
                                           {
                                             pathTriangulation.insert_constraint (cdt_handle_vertex->point(), previouspoint);
                                           }
                                           catch (const std::exception& e)
                                                 {
                                                  qDebug() << "constrained edges dont follow the rules. skipping." ;
                                                 }
                                      }
                                      previouspoint = cdt_handle_vertex->point();
                                      flag_first = false;
                                }
                           }

                       if (cdt_handle_vertex->is_valid())
                          qDebug() << "[" << bv_current << "]" <<" -> (" << cdt_handle_vertex->point().x() << ", " << cdt_handle_vertex->point().y() << ")" ;

                       bv_current = predecessors[bv_current];
                     }

               //source vertex
               qDebug() << "insert source vertex " ;
               CDT::Vertex_handle cgal_vertex_source = nullptr;
               for (auto v = triangulation.finite_vertices_begin(); v != triangulation.finite_vertices_end(); ++v)
                   {
                     if (vertex_map[v] == bv_source)
                        {
                          cgal_vertex_source = v;
                          pathTriangulation.insert(cgal_vertex_source->point());
                          //qDebug() << "inserted source point in sp triangulation." << cgal_vertex_source->point() ;

                          try
                            {
                              pathTriangulation.insert_constraint (cgal_vertex_source->point(), previouspoint);
                            }
                            catch (const std::exception& e)
                                  {
                                   qDebug() << "constrained edges dont follow the rules. skipping." ;
                                   //break;
                                  }
                          break;
                        }
                   }


               if (pathTriangulation.finite_vertices_begin()!= pathTriangulation.finite_vertices_end() /*&& bv_target1 == bv_target*/)
                  {
                   qDebug() << " Inserted triangulation for the shortest path." ;
                   this->sp.v_source.push_back(this->vppt_source_target.first);
                   this->sp.v_target.push_back(this->vppt_source_target.second);
                   this->sp.bv_source.push_back(bv_source);
                   this->sp.bv_target.push_back(bv_target);
                   this->sp.v_cdt.push_back(pathTriangulation);
                   this->sp.v_predecessors.push_back(predecessors);
                   this->sp.v_distances.push_back(distances);
                  }      
                this->sp.sp_size();

               qDebug() << " (Distance: " << distances[bv_current] << ")" ;
               qDebug() << "  paths sp.sp_size() "   << this->sp.sp_size() ;
             }

          }

          else if (flag_invalid_path)
                 {
                  qDebug() << " Recording empty path." ;
                  this->sp.v_source.push_back({});
                  this->sp.v_target.push_back({});
                  this->sp.bv_source.push_back({});
                  this->sp.bv_target.push_back({});
                  this->sp.v_cdt.push_back({});
                  this->sp.v_predecessors.push_back({});
                  this->sp.v_distances.push_back({});
                 }
              flag_invalid_path = false; // jic it doenst need to be reassigned cause its created each time
      }

void hs_triangulation::f1_7_save_to_file_paths (QString path_id, QString path_coord)
                         {
                           qDebug() << "hs_triangulation::f1_7_save_to_file_paths" ;
                           std::vector <CDT> v_paths;
                           std::vector <std::vector<size_t>> v_paths_ids;
                           //Print the vertices of the shortest paths
                           qDebug() << "save all shortest paths: ";

                           std::size_t count_path = 0;
                         for (size_t i = 0 ; i < this->sp.v_cdt.size(); ++i)
                          {
                             qDebug() << "save path number: " << count_path++ ;
                          //for (auto it = vertex_map.begin(); it != vertex_map.end(); ++it)
                          //    {
                                 v_paths.push_back(CDT());
                                 v_paths_ids.push_back(std::vector <size_t>());
                          //     Boost_Vertex target = it->second;
                                  qDebug() << "2";
                                 //qDebug() << "Xhortest paths from source " <<  ptr_view->sp.v_source.at(i) << " to target " <<  ptr_view->sp.v_target.at(i) << ": " ;

                                 qDebug() << "1";
                                 std::vector<Boost_Vertex> path;
                                 //Boost_Vertex current_vertex = target_vertex;
                                 Boost_Vertex current_vertex = this->sp.bv_target.at(i);
                                 size_t count_vertices = 0;

                                 while (current_vertex != this->sp.bv_source.at(i))
                                       {

                                          qDebug() << "current_vertex " <<  current_vertex << " ptr_view->sp.bv_source.at(i)" << this->sp.bv_source.at(i) ;
                                         qDebug() << "2";
                                         path.push_back(current_vertex);

                                         qDebug() << "2.1";
                                         for (auto v = this->sp.v_cdt.at(i).finite_vertices_begin(); v != this->sp.v_cdt.at(i).finite_vertices_end(); ++v)
                                         //for (auto v = ptr_view->sp.v_cdt.back().finite_vertices_begin(); v != ptr_view->sp.v_cdt.back().finite_vertices_end(); ++v)
                                         //for (auto v = triangulation.finite_vertices_begin(); v != triangulation.finite_vertices_end(); ++v)
                                            {
                                              qDebug() << "3";
                                              // if (vertex_map[v] == current_vertex)
                                                {
                                                  qDebug() << count_vertices++;
                                                  CDT::Vertex_handle cdt_handle_vertex = v;
                                                  if (cdt_handle_vertex->is_valid())
                                                     {
                                                     v_paths.back().insert(cdt_handle_vertex->point());
                                                     }
                                                }
                                            }
                                          v_paths_ids.back().push_back(current_vertex);
                                          qDebug() << "inserted point" << current_vertex;
                                          current_vertex = this->sp.v_predecessors.at(i)[current_vertex];
                                       }
                               }

                           qDebug() << "v_paths extracted from path size "<< v_paths.size() ;


                           qDebug() << "v_paths_ids extracted from path size "<< v_paths_ids.size();
                             for (auto & a:v_paths_ids )
                                  qDebug() << "a size "<< a.size();

                
                           f1_7_1_save_paths_coordinates(path_coord, v_paths);
                           f1_7_2_save_paths_indices (path_id, v_paths_ids);
                         }

void hs_triangulation::f1_7_1_save_paths_coordinates (QString path_coord, std::vector <CDT> & v_paths)
                           {
                             qDebug() << "hs_triangulation::f1_7_1_save_paths_coordinates" ;

                             if (path_coord.isEmpty())
                                {
                                 return; // User canceled the dialog
                                }

                             std::ofstream outputFile(path_coord.toStdString());

                             outputFile << std::fixed << std::setprecision(20);
                             if (!outputFile.is_open())
                                {
                                 qWarning() << "Error opening the output file.";
                                 return;
                                }

                            // outputFile << "PATHS FROM SOURCE [" <<  (std::prev((v_paths.begin())->finite_vertices_end()))->point() << "] TO ALL POINTS : COORDINATES ";
                             size_t count = 0;
                             for (auto & a : v_paths)
                                 {
                                    outputFile << "SP"<< count++ << " ";
                                    for (CDT::Finite_vertices_iterator it = a.finite_vertices_begin(); it != a.finite_vertices_end(); ++it)
                                        {
                                         Point_2 p = it->point();
                                         outputFile << p.x() << " " << p.y()<< " | ";
                                        }
                                    outputFile ;
                                 }
                             outputFile.close();
                           }

void hs_triangulation::f1_7_2_save_paths_indices (QString path_id, std::vector <std::vector<size_t>>& v_paths_ids)
                        {
                          qDebug() << "hs_triangulation::f1_7_2_save_paths_indices";

                          if (path_id.isEmpty())
                             {
                              return; // User canceled the dialog
                             }

                          std::ofstream outputFileids(path_id.toStdString());
                          if (!outputFileids.is_open())
                             {
                              qWarning() << "Error opening the output file.";
                              return;
                             }

                          //outputFileids << "PATHS FROM SOURCE [" <<  *(std::prev((v_paths_ids.begin())->end())) << "] TO ALL POINTS : INDICES ";
                          size_t count = 0;
                          for (auto & a : v_paths_ids)
                              {
                                 outputFileids << "SP" << count++ << " ";
                                 for (auto it = a.begin(); it != a.end(); ++it)
                                     {
                                      outputFileids << *it << " ";
                                     }
                                 outputFileids ;
                              }
                          outputFileids.close();
                        }
