#pragma once

#include <iostream>
#include <vector>
#include <qdebug.h>
#include <string>
#include <qstring.h>


// does hs_vector_log keep the name of the funciton only ??? do we need that??? 

//passed to mainwindow.h to keep functions without using this header
extern std::vector <std::string> hs_vector_log;
extern std::vector <std::pair <std::string, std::string>> hs_vps;
static void hs_update_log(std::string s)
{

    QString qs = QString::fromStdString(s);
    qDebug() << "hs_update_log  (" + qs + ")" ;
    qDebug() << "before" << hs_vector_log.size();
    hs_vector_log.push_back(s);
    qDebug() << "after" << hs_vector_log.size();


    std::vector <std::string> v_copy;
    if (hs_vector_log.size() >= 100)
    {
        for (size_t i = hs_vector_log.size() - 50; i < hs_vector_log.size(); ++i)
            v_copy.push_back(hs_vector_log.at(i));

        hs_vector_log = v_copy;
        qDebug() << "resized " << hs_vector_log.size();
    }

    qDebug() << "hs_vector_log size " << hs_vector_log.size();
}

static void hs_update_log_all(std::string s, std::string s_text)
{
    qDebug() << "hs_update_log_all size " << hs_vector_log.size();

    hs_update_log(s);
    hs_vps.push_back(std::pair <std::string, std::string>("", s_text));
}



