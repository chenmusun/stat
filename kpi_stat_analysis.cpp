/*
 * kpi_stat_analysis.cpp
 *
 *  Created on: 2015年10月14日
 *      Author: chenms
 */
#include"kpi_stat_analysis.h"

BasePredicate * CreatePredicate(const std::string& predicate_name,const std::string& parm)
{
	BasePredicate * predicate_ptr=NULL;

	if(predicate_name=="range")
		predicate_ptr=new Range(parm);
	else if(predicate_name=="erange")
		predicate_ptr=new Erange(parm);
	else if(predicate_name=="equal")
		predicate_ptr=new Equal(parm);
	else if(predicate_name=="section")
		predicate_ptr= new Section(parm);
	else{
	}
	return predicate_ptr;
}

BaseCalcMethod * CreateCalcMethod(const std::string& calc_name,const std::string& parm)
{
	//TODO 列出所有算法
	BaseCalcMethod * calc_ptr=NULL;

	if(calc_name=="avg")
		calc_ptr=new Avg(parm);
	else if(calc_name=="count")
		calc_ptr=new Count(parm);
	else if(calc_name=="distance")
		calc_ptr=new Distance(parm);
	else if(calc_name=="cdf")
		calc_ptr= new Cdf(parm);
	else if(calc_name=="max")
		calc_ptr= new Max(parm);
	else{
	}
	return calc_ptr;
}


//std::set<IEEventNameID> KPIStatAnalysis::ie_event_name_id_set_(IE_EVENT_NAME_ID,IE_EVENT_NAME_ID+sizeof(IEEventNameID)/sizeof(IE_EVENT_NAME_ID));
std::multimap<int,KPIPrimitiveInfo> KPIStatAnalysis::mulmap_kpi_info_;//(GetKPIInfoFromDB("root","root","odb_test"));

 bool  KPIStatAnalysis::GetKPIInfoFromDB(const std::string& username,const std::string& password,const std::string& database)
 {
	 bool ret=true;
	// multimap<int,KPIActiveInfo> temp_mulmap;
	  try{
	    std::auto_ptr<odb::database> db (new odb::mysql::database (username,password,database));
		typedef odb::query<KPIPrimitiveView> query;
		typedef odb::result<KPIPrimitiveView> result;

		odb::transaction t (db->begin ());
		// t.tracer (stderr_tracer);
		result r1(db->query<KPIPrimitiveView>());
		for (result::iterator i (r1.begin ()); i != r1.end (); ++i){
			KPIPrimitiveInfo kp;
			ConvertKPIViewToKPIInfo(*i,kp);
			mulmap_kpi_info_.insert(make_pair(i->ie_event_id,kp));
		}
		t.commit();
	  }
	  catch (const odb::exception& e)
	  {
	  std::cout << e.what () << std::endl;
	  ret=false;
	  }
	  return ret;
 }

 void KPIStatAnalysis::PrintKPIInfo()
 {
	 for(auto pos=mulmap_kpi_info_.begin();pos!=mulmap_kpi_info_.end();++pos)
	 {
		 std::cout<<pos->first<<" "<<pos->second.kpi_name<<" "<<pos->second.calc_name<<" ";
		 pos->second.validation_check_ptr->PrintIEPredicate();
	 }
 }


 bool  KPIStatAnalysis::ConvertKPIViewToKPIInfo(KPIPrimitiveView& kv,KPIPrimitiveInfo& ki)
 {
	 bool ret=true;
	 try{
		 do{
			 ki.kpi_name=kv.kpi_name;
			 ki.calc_name=kv.calc_name;
			 ki.calc_param_name=kv.calc_param_name.null()?"":kv.calc_param_name.get();
			 //此处隐含条件，辅助判断存在的前提是主IE判断一定存在
			 if(kv.relation_name.null())
				 break;

			 {
				 ki.validation_check_ptr.reset(new ValidationCheck);
				 IEIDPredicate ie_pre;
				 ie_pre.ie_id=kv.ie_event_id;
				 ie_pre.ie_predicate_ptr.reset(CreatePredicate(kv.relation_name.get(),kv.range_name.get()));
				 ki.validation_check_ptr->AddIEPredicate(ie_pre);
			 }

			 if(!kv.assist_ie_event_id1.null()){
				 IEIDPredicate ie_pre;
				 ie_pre.ie_id=kv.assist_ie_event_id1.get();
				 ie_pre.ie_predicate_ptr.reset(CreatePredicate(kv.assist_ie_relation1.get(),kv.assist_ie_range1.get()));
				 ki.validation_check_ptr->AddIEPredicate(ie_pre);
			 }

			 if(!kv.assist_ie_event_id2.null()){
				 IEIDPredicate ie_pre;
				 ie_pre.ie_id=kv.assist_ie_event_id2.get();
				 ie_pre.ie_predicate_ptr.reset(CreatePredicate(kv.assist_ie_relation2.get(),kv.assist_ie_range2.get()));
				 ki.validation_check_ptr->AddIEPredicate(ie_pre);
			 }

			 if(!kv.assist_ie_event_id3.null()){
				 IEIDPredicate ie_pre;
				 ie_pre.ie_id=kv.assist_ie_event_id3.get();
				 ie_pre.ie_predicate_ptr.reset(CreatePredicate(kv.assist_ie_relation3.get(),kv.assist_ie_range3.get()));
				 ki.validation_check_ptr->AddIEPredicate(ie_pre);
			 }
		 }while(0);
	 }
	 catch(...)
	 {
		 ret=false;
	 }
	 return ret;
 }

// int KPIStatAnalysis::ConvertIENameToID(const std::string& ie_name);
// {
//	 int ret=-1;
//	 IEEventNameID iee;
//	 iee.ie_event_name=ie_name;
//	 auto pos=ie_event_name_id_set_.find(iee);
//	 if(pos!=ie_event_name_id_set_.end())
//		 ret=pos->ie_event_id;
//	 return ret;
// }


 bool KPIStatAnalysis::AddKPIActiveInfo(int ie_id)
 {
	 bool ret=true;
	 try{
		 auto range=mulmap_kpi_info_.equal_range(ie_id);
		 if(range.first!=range.second){
			 for(auto pos=range.first;pos!=range.second;++pos){
			 	 KPIActiveInfo kai;
			 	kai.kpi_name=pos->second.kpi_name;
			 	kai.calc_ptr.reset(CreateCalcMethod(pos->second.calc_name,pos->second.calc_param_name));
			 	kai.validation_check_ptr=pos->second.validation_check_ptr;
			 	mulmap_kpi_active_info_.insert(make_pair(ie_id,kai));
			 }
		 }
		 else{
			 ret=false;//原始库中未找到对应ie_id
		 }
	 }
	 catch(...){
		 ret=false;//出现异常
	 }

	 return ret;
 }


void KPIStatAnalysis::AnalysisData(IEEventData &iee)
{
	IEEventUnitData iee_unit;
	iee_unit.latitude=iee.latitude;
	iee_unit.longitude=iee.longitude;
	iee_unit.mos=iee.mos;
	iee_unit.timestamp=iee.timestamp;

	if(!iee.ie_map.empty()){
		//IE分析
		for(auto pos=iee.ie_map.begin();pos!=iee.ie_map.end();++pos){
			iee_unit.ie_id=pos->first;
			iee_unit.ie_value=pos->second;
			AnalysisIEEvent(iee_unit,&iee.ie_map);
		}
	}

	if(!iee.event_vec.empty()){
		//EVENT分析
		for(auto pos=iee.event_vec.begin();pos!=iee.event_vec.end();++pos){
			iee_unit.ie_id=*pos;
			AnalysisIEEvent(iee_unit,NULL);
		}
	}
}

void KPIStatAnalysis::DoConclusion()
{

}


void KPIStatAnalysis::AnalysisIEEvent(IEEventUnitData& iee_unit,map<int,double>* ie_map)
{
	auto pos=mulmap_kpi_active_info_.find(iee_unit.ie_id);
	if(pos==mulmap_kpi_active_info_.end()){//对象中暂无原始数据信息
		if(AddKPIActiveInfo(iee_unit.ie_id))
			pos=mulmap_kpi_active_info_.find(iee_unit.ie_id);
	}

	while(pos!=mulmap_kpi_active_info_.end()){
		if(pos->first==iee_unit.ie_id){
			if(pos->second.validation_check_ptr)
				if(!pos->second.validation_check_ptr->CheckValidation(iee_unit.ie_value,*ie_map))
					continue;
			pos->second.calc_ptr->ProcessIEEvent(iee_unit);//此处ProcessIE应该提供统一的接口TODO
		}
		++pos;
	}
}
