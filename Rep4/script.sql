USE [master]
GO
/****** Object:  Database [StoreBase]    Script Date: 08.02.2021 13:14:27 ******/
CREATE DATABASE [StoreBase]
 CONTAINMENT = NONE
 ON  PRIMARY 
( NAME = N'StoreBase', FILENAME = N'C:\Program Files (x86)\Microsoft SQL Server\MSSQL11.MSSQLSERVER\MSSQL\DATA\StoreBase.mdf' , SIZE = 4160KB , MAXSIZE = UNLIMITED, FILEGROWTH = 1024KB )
 LOG ON 
( NAME = N'StoreBase_log', FILENAME = N'C:\Program Files (x86)\Microsoft SQL Server\MSSQL11.MSSQLSERVER\MSSQL\DATA\StoreBase_log.ldf' , SIZE = 1040KB , MAXSIZE = 5242880KB , FILEGROWTH = 10240KB )
GO
ALTER DATABASE [StoreBase] SET COMPATIBILITY_LEVEL = 110
GO
IF (1 = FULLTEXTSERVICEPROPERTY('IsFullTextInstalled'))
begin
EXEC [StoreBase].[dbo].[sp_fulltext_database] @action = 'enable'
end
GO
ALTER DATABASE [StoreBase] SET ANSI_NULL_DEFAULT OFF 
GO
ALTER DATABASE [StoreBase] SET ANSI_NULLS OFF 
GO
ALTER DATABASE [StoreBase] SET ANSI_PADDING OFF 
GO
ALTER DATABASE [StoreBase] SET ANSI_WARNINGS OFF 
GO
ALTER DATABASE [StoreBase] SET ARITHABORT OFF 
GO
ALTER DATABASE [StoreBase] SET AUTO_CLOSE ON 
GO
ALTER DATABASE [StoreBase] SET AUTO_CREATE_STATISTICS ON 
GO
ALTER DATABASE [StoreBase] SET AUTO_SHRINK OFF 
GO
ALTER DATABASE [StoreBase] SET AUTO_UPDATE_STATISTICS ON 
GO
ALTER DATABASE [StoreBase] SET CURSOR_CLOSE_ON_COMMIT OFF 
GO
ALTER DATABASE [StoreBase] SET CURSOR_DEFAULT  GLOBAL 
GO
ALTER DATABASE [StoreBase] SET CONCAT_NULL_YIELDS_NULL OFF 
GO
ALTER DATABASE [StoreBase] SET NUMERIC_ROUNDABORT OFF 
GO
ALTER DATABASE [StoreBase] SET QUOTED_IDENTIFIER OFF 
GO
ALTER DATABASE [StoreBase] SET RECURSIVE_TRIGGERS OFF 
GO
ALTER DATABASE [StoreBase] SET  ENABLE_BROKER 
GO
ALTER DATABASE [StoreBase] SET AUTO_UPDATE_STATISTICS_ASYNC OFF 
GO
ALTER DATABASE [StoreBase] SET DATE_CORRELATION_OPTIMIZATION OFF 
GO
ALTER DATABASE [StoreBase] SET TRUSTWORTHY OFF 
GO
ALTER DATABASE [StoreBase] SET ALLOW_SNAPSHOT_ISOLATION OFF 
GO
ALTER DATABASE [StoreBase] SET PARAMETERIZATION SIMPLE 
GO
ALTER DATABASE [StoreBase] SET READ_COMMITTED_SNAPSHOT OFF 
GO
ALTER DATABASE [StoreBase] SET HONOR_BROKER_PRIORITY OFF 
GO
ALTER DATABASE [StoreBase] SET RECOVERY SIMPLE 
GO
ALTER DATABASE [StoreBase] SET  MULTI_USER 
GO
ALTER DATABASE [StoreBase] SET PAGE_VERIFY CHECKSUM  
GO
ALTER DATABASE [StoreBase] SET DB_CHAINING OFF 
GO
ALTER DATABASE [StoreBase] SET FILESTREAM( NON_TRANSACTED_ACCESS = OFF ) 
GO
ALTER DATABASE [StoreBase] SET TARGET_RECOVERY_TIME = 0 SECONDS 
GO
USE [StoreBase]
GO
/****** Object:  User [user]    Script Date: 08.02.2021 13:14:27 ******/
CREATE USER [user] FOR LOGIN [user] WITH DEFAULT_SCHEMA=[dbo]
GO
ALTER ROLE [db_accessadmin] ADD MEMBER [user]
GO
ALTER ROLE [db_datareader] ADD MEMBER [user]
GO
ALTER ROLE [db_datawriter] ADD MEMBER [user]
GO
/****** Object:  StoredProcedure [dbo].[OpenBase]    Script Date: 08.02.2021 13:14:27 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO



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
	--print(@create_base_str)
END



GO
/****** Object:  Table [dbo].[DataTable]    Script Date: 08.02.2021 13:14:27 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[DataTable](
	[ID] [int] IDENTITY(2,1) NOT NULL,
	[StoreTime] [datetime] NULL,
	[TresholdsTableID] [int] NULL,
	[DeadZonesTableID] [int] NULL,
	[UserTableID] [int] NULL,
	[ZonesData] [varbinary](max) NULL,
	[ZonesStatus] [varbinary](max) NULL,
PRIMARY KEY CLUSTERED 
(
	[ID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[DeadZonesTable]    Script Date: 08.02.2021 13:14:27 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[DeadZonesTable](
	[ID] [int] IDENTITY(2,1) NOT NULL,
	[DeadZoneStart] [int] NULL,
	[DeadZoneStop] [int] NULL,
PRIMARY KEY CLUSTERED 
(
	[ID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
/****** Object:  Table [dbo].[TresholdsTable]    Script Date: 08.02.2021 13:14:27 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[TresholdsTable](
	[ID] [int] IDENTITY(2,1) NOT NULL,
	[AlarmThresh] [float] NULL,
	[AlarmThreshStart] [int] NULL,
	[AlarmThreshStop] [int] NULL,
	[AlarmGainStart] [float] NULL,
	[AlarmGainStop] [float] NULL,
	[BottomReflectionThresh] [float] NULL,
	[BottomReflectionThreshStart] [int] NULL,
	[BottomReflectionThreshStop] [int] NULL,
	[BottomReflectionGainStart] [float] NULL,
	[BottomReflectionGainStop] [float] NULL,
	[BottomReflectionOn] [bit] NULL,
PRIMARY KEY CLUSTERED 
(
	[ID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
/****** Object:  Table [dbo].[UserTable]    Script Date: 08.02.2021 13:14:27 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[UserTable](
	[ID] [int] IDENTITY(2,1) NOT NULL,
	[UserName] [varchar](64) NOT NULL,
	[UserPersonnelNumber] [int] NOT NULL,
PRIMARY KEY CLUSTERED 
(
	[ID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY],
UNIQUE NONCLUSTERED 
(
	[UserPersonnelNumber] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY],
UNIQUE NONCLUSTERED 
(
	[UserName] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
USE [master]
GO
ALTER DATABASE [StoreBase] SET  READ_WRITE 
GO
