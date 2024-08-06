///GPT explain
///1 Finite vs. Infinite edges and vertices:
//Finite Vertices: These are the vertices that correspond to points in the input and are part of the final triangulation. You are generally interested in these vertices as they represent your input data.
//Infinite Vertices: These vertices are introduced by the convex hull of the input data and are not part of your actual input. They extend to infinity and are used to represent unbounded regions in the triangulation. For many applications, you may not need to operate on infinite vertices.
//Finite Edges vs. Infinite Edges:
//Finite Edges: These are edges between pairs of finite vertices. They represent Delaunay or constrained Delaunay edges in your triangulation. You may want to iterate over these edges for most of your operations.
//Infinite Edges: These edges connect finite vertices to infinite vertices and represent the edges of the convex hull. They are typically not part of the primary triangulation and might not be relevant to your specific task.
//The constrained_edges_begin() function in CGAL's constrained triangulation data structure is used to iterate over the constrained edges, and it returns only the finite constrained edges. This means that you will iterate over edges that connect pairs of finite vertices and represent constrained edges within your input data.


///2 Dijkstra algorithm and geometric distance
//You have a grid-like graph where all edges have the same weight, representing the geometric distance between vertices.
//The source vertex is at one corner of the grid, and the target vertex is at the diagonally opposite corner.
//In this case, Dijkstra's algorithm may not find the diagonal path (geometrically shortest) but might follow a zig-zag path along the edges, which accumulates the same weight as the diagonal path. This is because the algorithm explores vertices in a way that is driven by the cumulative weight, not necessarily the geometric distance.
//To find the geometrically shortest path, you would need an algorithm like A* (A-star) or a visibility graph algorithm, which specifically considers the geometric distances between vertices. These algorithms take into account the actual geometry of the space and can find paths that more closely match the geometric shortest path.
//In summary, Dijkstra's algorithm is primarily concerned with cumulative edge weights and does not guarantee finding the geometrically shortest path, especially in scenarios where there are multiple paths with the same weight.


#ifndef HS_SHORTEST_PATH_H
#define HS_SHORTEST_PATH_H

#include <include/interactive_view.h>

#include <boost/graph/adjacency_list.hpp>
#include <CGAL/boost/graph/dijkstra_shortest_paths.h>


//SHORTEST PATH

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <include/HS_Delaunay_triangulation_2.h>
#include <include/HS_TriangulationGraphicsItem.h>
#include <include/HS_CTriangulationGraphicsItem.h>


typedef CGAL::Exact_predicates_inexact_constructions_kernel K;                                                                                                      
typedef K::Point_2 Point_2;
typedef CGAL::Delaunay_triangulation_2<K, CGAL::Triangulation_data_structure_2 <CGAL::Triangulation_vertex_base_with_id_2<K>, CGAL::Triangulation_face_base_2<K>>>  Triangulation;
typedef CGAL::Constrained_Delaunay_triangulation_2<K> CDT;
typedef CDT::Vertex_handle Vertex_handle;


typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property, boost::property<boost::edge_weight_t, double>> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor Boost_Vertex;

struct shortestpath;

class hs_triangulation
{
   friend class InteractiveView_hs_triangulation;

   public:
   hs_triangulation ();
   hs_triangulation (CDT& cdt, Delaunay& dt) 
                    {
                      dt_  = std::make_shared <Delaunay> (dt); 
                      cdt_ = std::make_shared <CDT> (cdt);
                    }

   size_t flag_input_st = 0;
   
   std::pair <Point_2, Point_2> vppt_source_target;
   
   std::shared_ptr <Delaunay> dt_;
   std::shared_ptr <CDT> cdt_;
   
   std::vector <std::shared_ptr <CGAL::Qt::TriangulationGraphicsItem<Delaunay>>> triangulationItems = { nullptr }; // dt 
   std::shared_ptr <CGAL::Qt::VoronoiGraphicsItem<Delaunay>> voronoiItem = nullptr;// = std::make_shared<CGAL::Qt::VoronoiGraphicsItem<Delaunay>>(); // dt only

   std::shared_ptr <CGAL::Qt::CTriangulationGraphicsItem<CDT>> ctriangulationItem = nullptr;  // cdt

   std::vector <QGraphicsTextItem*> textItems; // dt 
   std::vector <QGraphicsTextItem*> ctextItems; // cdt
   
   
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
            qDebug() << "shortestpath::sp_size";
            qDebug() << v_source.size();
            qDebug() << v_target.size();
            qDebug() << bv_source.size();
            qDebug() << bv_target.size();
            qDebug() << v_cdt.size();
            qDebug() << v_cdt_gi.size();
            qDebug() << "shortestpath::sp_size exit";
            return v_cdt.size();
        }
    } sp;


  void insertpoint(QPointF);

  void deletepoint(QPoint, InteractiveView_hs_triangulation*);

  void f1_define_source_target(QPoint mousePoint, InteractiveView_hs_triangulation* that);
  void f1_shortest_path(CDT& cdt_, InteractiveView_hs_triangulation*);
  void f1_2_find_nearest_point(CDT::Vertex_handle& nearest_vertex, Point_2 pt, CDT& cdt_);
  void f1_2_find_nearest_point(Triangulation::Vertex_handle& nearest_vertex, Point_2 pt, Triangulation& dt_);
  void f1_1_identify_target(CDT::Vertex_handle& source, CDT::Vertex_handle& target, std::pair <Point_2, Point_2> ppt, CDT& triangulation);
  void f1_3_add_vertices(Graph& graph, std::map<CDT::Vertex_handle, Boost_Vertex>& vertex_map, CDT& triangulation);
  void f1_4_add_edges(Graph& graph, std::map <CDT::Vertex_handle, Boost_Vertex>& vertex_map, CDT& triangulation);
  void f1_5_print_shortest_paths_all(Boost_Vertex& source_vertex, std::vector<double>& distances, std::vector<Boost_Vertex>& predecessors, std::map<CDT::Vertex_handle, Boost_Vertex>& vertex_map);
  void f1_5_print_shortest_path_only(bool& flag_invalid_path, CDT::Vertex_handle& source, Boost_Vertex& target_vertex, Boost_Vertex& source_vertex, std::vector<double>& distances, std::vector<Boost_Vertex>& predecessors, std::map<CDT::Vertex_handle, Boost_Vertex>& vertex_map, CDT&);
  void f1_6_make_triangulation(bool& flag_invalid_path, Boost_Vertex& target_vertex, Boost_Vertex& source_vertex, std::vector<double>& distances, std::vector<Boost_Vertex>& predecessors, std::map<CDT::Vertex_handle, Boost_Vertex>& vertex_map, CDT& triangulation);
  void f1_7_save_to_file_paths(QString, QString);
  void f1_7_1_save_paths_coordinates(QString path_coord, std::vector <CDT>& v_paths);
  void f1_7_2_save_paths_indices(QString, std::vector <std::vector<size_t>>& v_paths_ids);
};






#endif // HS_SHORTEST_PATH_H
