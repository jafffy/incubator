#include <stdio.h>
#include <string.h>
#include <mysql.h>

#define DB_HOST "localhost"
#define DB_USER "root"
#define DB_PASS "1234"
#define DB_NAME "cndi"

int main(void)
{
	MYSQL		*connection=NULL;
	MYSQL_RES	*sql_result;
	MYSQL_ROW	sql_row;

	int nQueryStat;

	connection = mysql_init(NULL);
	if(connection == NULL)
	{
		printf("Mysql init fail\n");
		return -1;
	}

	if(mysql_real_connect(connection, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, (char *)NULL, 0) == NULL)
	{
		printf("Mysql connection error : %s\n", mysql_error(connection));
		return 1;
	}
	
	nQueryStat =  mysql_query(connection, "select * from testresults");
	if(nQueryStat != 0)
	{
		printf("Mysql query error %s\n", mysql_error(connection));
		return 1;
	}
	
	sql_result = mysql_store_result(connection);

	printf("name     , korean , english, math \n");	
	while((sql_row = mysql_fetch_row(sql_result)) != NULL)
	{
		printf("%10s, %6s, %6s, %6s\n",sql_row[0],sql_row[1],sql_row[2],sql_row[3]);
	}

	mysql_close(connection);

}


