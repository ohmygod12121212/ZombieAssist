int Client_ID[MAXPLAYERS + 1];
char Client_Name[MAXPLAYERS + 1][32];
char Client_Steam[MAXPLAYERS + 1][32];
void Client_Init(int Client)
{
	GetClientName(Client, Client_Name[Client], sizeof(Client_Name[]));
	GetClientAuthId(Client, AuthId_Steam2, Client_Steam[Client], sizeof(Client_Steam[]));
}
void Client_Load(int Client)
{
	char Query[192];
	Format(Query, sizeof(Query), "SELECT * FROM user WHERE steam='%s'", Client_Steam[Client]);
	SQL_TQuery(MySQL, Client_Load_Query, Query, Client);
}
void Client_Clean(int Client)
{
	Client_ID[Client] = 0;
	Format(Client_Name[Client], sizeof(Client_Name[]), "");
	Format(Client_Steam[Client], sizeof(Client_Steam[]), "");
}
void Client_Load_Query(Handle Owner, Handle Child, const char[] Error, any Client)
{
	if (Owner == INVALID_HANDLE || Child == INVALID_HANDLE)
	{
		LogError("Client Handle Failure");
		PrintToChat(Client, "[ZA]玩家信息读取失败,请联系管理员");
		return;
	}
	if (SQL_GetRowCount(Child) == 0)
	{
		Client_Create(Client);
		return;
	}
	if (!SQL_FetchRow(Child))
	{
		LogError("Client FetchRow Failure");
		return;
	}
	Client_ID[Client] = SQL_FetchInt(Child, 0);
	PrintToChat(Client, "[ZA]玩家信息读取完成,玩家UID为: %d", Client_ID[Client]);
}
void Client_Create(int Client)
{
	char Query[192];
	Format(Query, sizeof(Query), "INSERT INTO user(name,steam) VALUES ('%s','%s')", Client_Name[Client], Client_Steam[Client]);
	SQL_TQuery(MySQL, MySQL_Query, Query);
	PrintToChat(Client, "[ZA]玩家信息创建成功,欢迎您的到来");
}