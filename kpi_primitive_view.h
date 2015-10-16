/*
 * kpi_primitive_view.h
 *
 *  Created on: 2015年10月14日
 *      Author: chenms
 */

#pragma once
#include<string>
#include <odb/nullable.hxx>
#pragma db view table("KPIPrimitiveInfo")
struct KPIPrimitiveView
{
#pragma db column("kpi_name") type("text")
std::string kpi_name;
#pragma db column("calc_name") type("text")
std::string calc_name;
#pragma db column("calc_param_name") type("text")
odb::nullable<std::string> calc_param_name;

#pragma db column("ie_event_id") type("int")
int ie_event_id;
#pragma db column("relation_name") type("text")
odb::nullable<std::string> relation_name;
#pragma db column("range_name") type("text")
odb::nullable<std::string> range_name;

#pragma db column("assist_ie_event_id1") type("INT")
odb::nullable<int> assist_ie_event_id1;
#pragma db column("assist_ie_relation1") type("text")
odb::nullable<std::string> assist_ie_relation1;
#pragma db column("assist_ie_range1") type("text")
odb::nullable<std::string> assist_ie_range1;

#pragma db column("assist_ie_event_id2") type("INT")
odb::nullable<int> assist_ie_event_id2;
#pragma db column("assist_ie_relation2") type("text")
odb::nullable<std::string> assist_ie_relation2;
#pragma db column("assist_ie_range2") type("text")
odb::nullable<std::string> assist_ie_range2;

#pragma db column("assist_ie_event_id3") type("INT")
odb::nullable<int> assist_ie_event_id3;
#pragma db column("assist_ie_relation3") type("text")
odb::nullable<std::string> assist_ie_relation3;
#pragma db column("assist_ie_range3") type("text")
odb::nullable<std::string> assist_ie_range3;
};


