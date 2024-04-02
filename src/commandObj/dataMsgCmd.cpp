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
