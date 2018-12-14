#include"libs.h"
#include"general_config.h"
#include"msg.h"

int createMsg(key_t key)
{
	int msgid = msgget(key, PERMISSION | IPC_CREAT | IPC_EXCL);
	if(msgid < 0)
	{
		if(errno == EEXIST)
		{
			msgid = msgget(key, PERMISSION);
			CHECK("msgget", msgid);

			CHECK("msgctl", msgctl(msgid, IPC_RMID, (struct msqid_ds *)NULL));

			msgid = msgget(key, PERMISSION | IPC_CREAT | IPC_EXCL);
			CHECK("msgid", msgid);
		}
		else
		{
			perror("msgget");
			exit(1);
		}
	}

	return msgid;
}

int connectToMsg(key_t key)
{
	int msgid;
	do msgid = msgget(key, PERMISSION);
	while((msgid == -1) && (errno ==ENOENT));

	if(msgid == -1)
	{
		perror("msgget");
		exit(1);
	}

	return msgid;

}
