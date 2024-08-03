# Created by the script cgal_create_cmake_script
# This is the CMake script for compiling a CGAL application.

cmake_minimum_required(VERSION 3.1...3.23)
project(UGLab)


add_executable (UGLab 
	            hs_shortest_path.cpp 
                interactive_view.cpp 
                main.cpp 
                mainwindow.cpp
				
	            include/HS_CTriangulationGraphicsItem.h
                include/HS_Delaunay_triangulation_2.h
                include/HS_TriangulationGraphicsItem.h
                include/hs_shortest_path.h
                include/interactive_view.h
                include/mainwindow.h
		 )

   


find_package(CGAL REQUIRED OPTIONAL_COMPONENTS Qt5)



if(CGAL_Qt5_FOUND)
  target_link_libraries(UGLab PUBLIC CGAL::CGAL_Basic_viewer)
else()
  message(STATUS "NOTICE: Several examples require Qt5 and will not be compiled.")
endif()
