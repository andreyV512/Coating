R"(
CREATE PROCEDURE [dbo].[OpenBase] @path varchar(1024), @file_name varchar(1024) AS BEGIN
DECLARE @session_id INT = 0
DECLARE @SQL nvarchar(10)
DECLARE @create_base_str nvarchar(2048)

	SELECT @session_id = session_id FROM sys.dm_exec_sessions WHERE DB_NAME(database_id) = 'StoreBaseTMP'
	
	if 0 <> @session_id BEGIN
		SET @SQL = 'KILL ' + CAST(@session_id as varchar(4))
		EXEC (@SQL)
	END
	
	IF DB_ID('StoreBaseTMP') IS NOT NULL
	BEGIN
		DROP DATABASE [StoreBaseTMP]
	END

	SET @create_base_str = 
	'RESTORE DATABASE StoreBaseTMP FROM DISK ='''
	+ @file_name
	+ ''' WITH MOVE ''StoreBase'' to '''
	+ @path
	+ '\x.mdf'', MOVE ''StoreBase_log'' to '''
	+ @path
	+ '\x.ldf'''

	EXEC (@create_base_str)
END
)"


