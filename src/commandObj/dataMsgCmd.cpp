#include "./include/dataMsgCmd.h"

CDataMsgCmd::CDataMsgCmd()
{
    _childCmdType=DTAT_MSG_CMD;
}
CDataMsgCmd::CDataMsgCmd(CUser &recvUser) 
{
    _recvUser=recvUser;
    _childCmdType=DTAT_MSG_CMD;
}


int CDataMsgCmd::do_command(COtlUse &cmdOtlUse)
{
    _childDoCommandReturn=false; //开始时，执行成功标记设置为false

    int existReturn,executeReturn;
    
    // 检查用户账号密码，
    existReturn=cmdOtlUse.select_user_exist(_recvUser);
    if(existReturn!=0) {std::cout<<cmdOtlUse.get_errmsg()<<std::endl;return -1;}

    if(MSG_CONFIRM==_requestType)
    {
        //如果发送确认消息，其实vector中只有一条包含发送者和接收者的空的消息
        for(std::vector<CMsg>::iterator it=_msgDataLists.begin();it!=_msgDataLists.end();it++)
        {
            executeReturn=cmdOtlUse.set_msg_read_over((*it).get_recv_id(),(*it).get_send_id());
            if(executeReturn==-1) {std::cout<<cmdOtlUse.get_errmsg()<<std::endl;return -1;}
        }
    }
    else if(MSG_SEND==_requestType)
    {
        
    }


    _childDoCommandReturn=true;
    return 0;
}

std::string CDataMsgCmd::get_command_obj_json()
{
    std::ostringstream ostrStream;
    cereal::JSONOutputArchive jsonOA(ostrStream);
    super_json_add_make_nvp(jsonOA,this->_childCmdType);
    
    jsonOA(cereal::make_nvp("dataMsg", *this));

    return ostrStream.str();
}
void CDataMsgCmd::reload_recv_obj_by_json(cereal::JSONInputArchive &jsonIA) 
{
    jsonIA(cereal::make_nvp("dataMsg", *this));
}

void CDataMsgCmd::show_do_command_info()
{
    std::cout<<"this user:"<<std::endl;
    _recvUser.print();
}


void CDataMsgCmd::set_msg_request_type(MSG_REQUEST_TYPE requestType)
{
    _requestType=requestType;
}
void CDataMsgCmd::set_recv_user(CUser &recvUser)
{
    _recvUser=recvUser;
}
CUser CDataMsgCmd::get_recv_user()
{
    return _recvUser;
}
void CDataMsgCmd::set_msg_data_lists(std::vector<CMsg> &msgDataLists)
{
    _msgDataLists=msgDataLists;
}
std::vector<CMsg>& CDataMsgCmd::get_msg_data_lists()
{
    return _msgDataLists;
}

CDataMsgCmd::~CDataMsgCmd() {
    _msgDataLists.clear();
};
