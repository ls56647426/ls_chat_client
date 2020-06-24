#ifndef MSG_H
#define MSG_H

#include "../pojo/User.h"
#include "../pojo/Group.h"

/* 消息内容最大长度限制 4096 B */
const int MSGINFO_MAX_LEN = 4096;

/**
 * UNCOMPRESSED：		没压缩
 * COMPRESSED：			压缩了
 */
enum MsgHeaderType
{
	UNCOMPRESSED, COMPRESSED
};

/* 消息头 */
typedef struct
{
	/* 是否压缩过 */
	bool compressflag;
	/* 压缩前大小 */
	uint32_t originsize;
	/* 压缩后大小 */
	uint32_t compresssize;
	/* 不知道干嘛的，预留出来 */
	char reserved[16];
} MsgHeader;

/* 消息内容 */
class MsgInfo
{
public:
	MsgInfo();

	string getInfo() const;
	void setInfo(const string &value);

	string toString() const;

private:
	string info;
	//	FileInfo file_info;
	//	TranFile file_data;
};

/**
 * @brief [The MsgType enum]
 * MSG_UNKNOW：						未知类型
 * MSG_HEARTBEAT：					心跳包
 * MSG_REGISTER：					注册
 * MSG_LOGIN：						登录
 * MSG_ALTER_PWD：					修改密码
 * MSG_FIND_PWD：					找回密码
 * MSG_SEND_MSG：					发送消息
 * MSG_LOGOUT：						退出
 * MSG_TRAN_FILE：					发送文件
 * COMMAND_GET_FRIEND：				获取好友用户列表
 * COMMAND_GROUP_BANUSERPOST：		群内禁言
 * COMMAND_GROUP_DELETEUSER：		踢人
 * COMMAND_FILE_UPLOAD：				上传文件
 * COMMAND_FILE_DOWNLOAD：			下载文件
 * COMMAND_CREATE_GROUP：			创建群
 * COMMAND_APPLY_ADD_GROUP：			申请加群
 * COMMAND_ADMINIST_ADD_GROUP：		管理加群申请
 * COMMAND_GET_GROUP_FILE：			获取群文件
 * COMMAND_ADD_USER：				加好友
 * COMMAND_GET_GROUP_USER：			获取群成员列表
 * COMMAND_INTO_GROUP：				进群聊天
 * COMMAND_INTO_CHATUSER：			进入私聊
 * COMMAND_FILE_UPLOAD_ING：			正在上传群文件
 * COMMAND_EXIT_CHATGROUP：			离开了群聊
 * COMMAND_FIND_USER：				查找用户(nickname, username)
 * ERRNO_SUCCESS：					成功
 * ERRNO_ALREADYEXIST：				该用户已存在
 * ERRNO_INEXISTENCE：				该用户不存在
 * ERRNO_NOTFOUND：					未找到该用户
 * ERRNO_BEOFFLINE：					被挤掉了，下线
 * ERRNO_GROUP_ALREADYEXIST：		该群已存在
 * ERRNO_GROUP_INEXISTENCE：			该群不存在
 * ERRNO_ADDUSER_NOTFOUND：			加好友，没找到该用户
 */
enum MsgType
{
	MSG_UNKNOW, MSG_HEARTBEAT = 1000,

	MSG_REGISTER, MSG_LOGIN, MSG_ALTER_PWD, MSG_FIND_PWD, MSG_SEND_MSG,
	MSG_LOGOUT, MSG_TRAN_FILE,

	COMMAND_GET_FRIEND, COMMAND_GROUP_BANUSERPOST, COMMAND_GROUP_DELETEUSER,
	COMMAND_FILE_UPLOAD, COMMAND_FILE_DOWNLOAD, COMMAND_CREATE_GROUP,
	COMMAND_APPLY_ADD_GROUP, COMMAND_ADMINIST_ADD_GROUP, COMMAND_GET_GROUP_FILE,
	COMMAND_ADD_USER, COMMAND_GET_GROUP_USER, COMMAND_INTO_GROUP,
	COMMAND_INTO_CHATUSER, COMMAND_FILE_UPLOAD_ING, COMMAND_EXIT_CHATGROUP,
	COMMAND_FIND_USER,

	ERRNO_SUCCESS, ERRNO_ALREADYEXIST, ERRNO_INEXISTENCE, ERRNO_NOTFOUND,
	ERRNO_BEOFFLINE, ERRNO_GROUP_ALREADYEXIST, ERRNO_GROUP_INEXISTENCE,
	ERRNO_ADDUSER_NOTFOUND
};

/* 消息主体 */
class Msg
{
public:
	Msg();

	uint32_t getType() const;
	void setType(const uint32_t &value);

	Group getGroup() const;
	void setGroup(const Group &value);

	User getSrc() const;
	void setSrc(const User &value);

	User getDest() const;
	void setDest(const User &value);

	MsgInfo getInfo() const;
	void setInfo(const MsgInfo &value);

	string toString() const;

private:
	uint32_t type;
	Group group;
	User src;
	User dest;
	MsgInfo info;
};

#endif // MSG_H
