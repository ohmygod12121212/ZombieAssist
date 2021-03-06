int Client_ID[MAXPLAYERS + 1];
char Client_Name[MAXPLAYERS + 1][32];
char Client_Steam[MAXPLAYERS + 1][32];
void Client_Init(int Client)
{
	char Name[32];
	GetClientName(Client, Name, sizeof(Name));
	EscapeStringMySQL(Name, Client_Name[Client], sizeof(Client_Name[]));
	GetClientAuthId(Client, AuthId_Steam2, Client_Steam[Client], sizeof(Client_Steam[]));
}
void Client_Load(int Client)
{
	char Query[192];
	Format(Query, sizeof(Query), "SELECT * FROM user WHERE steam='%s'", Client_Steam[Client]);
	SQL_TQuery(MySQL, Client_Load_Query, Query, Client);
}
void Client_OnClientDisconnect(int Client)
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
		CPrintToChat(Client, "{green}[ZA][用户]{red} 玩家信息读取失败,请联系管理员");
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
	char Query[192];
	Format(Query, sizeof(Query), "UPDATE user SET name='%s' WHERE id=%d", Client_Name[Client], Client_ID[Client]);
	SQL_TQuery(MySQL, MySQL_Query, Query);
	LogMessage(Query);
	CPrintToChat(Client, "{green}[ZA][用户]{blue} 玩家信息读取完成,玩家UID为: %d", Client_ID[Client]);
	LogMessage("Client %d Load Success", Client_ID[Client]);
}
void Client_Create(int Client)
{
	char Query[192];
	Format(Query, sizeof(Query), "INSERT INTO user(steam,name) VALUES ('%s','%s')", Client_Steam[Client], Client_Name[Client]);
	SQL_TQuery(MySQL, Client_Create_Query, Query, Client);
	LogMessage(Query);
}
void Client_Create_Query(Handle Owner, Handle Child, const char[] Error, any Client)
{
	if (Owner == INVALID_HANDLE || Child == INVALID_HANDLE)
	{
		CPrintToChat(Client, "{green}[ZA][用户]{red} 用户信息创建失败,请联系管理员");
		return;
	}
	CPrintToChat(Client, "{green}[ZA][用户]{blue} 玩家信息创建成功,欢迎您的到来");
	LogMessage("Client %d Create Success", SQL_GetInsertId(Child));
}