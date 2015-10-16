/*
 * kpi_stat_analysis.h
 *
 *  Created on: 2015年10月13日
 *      Author: chenms
 */
#pragma once
#include <iostream>
#include <odb/database.hxx>
#include <odb/transaction.hxx>
#include <odb/mysql/database.hxx>
#include <odb/mysql/connection-factory.hxx>
#include"validation_check.h"
#include"kpi_primitive_view.h"
//#include"ie_event_name_id.h"
#include "kpi_primitive_view-odb.hxx"
#include"analysis_base.h"
#include<map>
#include<set>
#include<memory>
#include"calc_method.h"
using std::string;
using std::multimap;
using std::map;
using std::set;
using std::shared_ptr;
using std::make_pair;

struct KPIPrimitiveInfo{
	string kpi_name;
	string calc_name;
	string	calc_param_name;
	shared_ptr<ValidationCheck> validation_check_ptr;
};

struct KPIActiveInfo{
	string kpi_name;
	shared_ptr<BaseCalcMethod> calc_ptr;
	shared_ptr<ValidationCheck> validation_check_ptr;
};
class KPIStatAnalysis:public AnalysisBase{
public:
	static bool  GetKPIInfoFromDB(const std::string& username,const std::string& password,const std::string& database);//从数据库中获得数据
	static void  PrintKPIInfo();//TODO 测试用
	//static int ConvertIENameToID(const std::string& ie_name);
	void AnalysisData(IEEventData& iee);
	void DoConclusion();
private:
	void AnalysisIEEvent(IEEventUnitData& iee_unit,std::map<int,double>* ie_map);
	bool AddKPIActiveInfo(int ie_id);
    static bool ConvertKPIViewToKPIInfo(KPIPrimitiveView& kv,KPIPrimitiveInfo& ki);
private:
	static multimap<int,KPIPrimitiveInfo>  mulmap_kpi_info_;
	multimap<int,KPIActiveInfo> mulmap_kpi_active_info_;
//	static  set<IEEventNameID> ie_event_name_id_set_;
};



