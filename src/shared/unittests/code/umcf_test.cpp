
#include "Common.h"
#include <gtest/gtest.h>
#include "umcf/UMcf.h"


#define TEST_DIR "copyFile"
#include "util_fs/testFunctions.cpp"

#define SRC (getTestDirectory()/"mcffiles.xml")

void PrintfMsg(const char* format, ...)
{
}

namespace UnitTest
{
	TEST_F(FSTestFixture, UMCF_LoadXmlFile)
	{
		{
			std::string strFile(SRC.string());
			UTIL::FS::FileHandle fh(strFile.c_str(), UTIL::FS::FILE_WRITE);

			gcString strData("\
				<?xml version=\"1.0\" encoding=\"utf-8\"?>\
				<appupdate version=\"1\">\
				<status code=\"0\"/>\
				<mcf id=\"1528\" build=\"53\" appid=\"100\">\
				<url>http://app.desura.com/100/2011/1528.mcf</url>\
			<date>20111116031808</date>\
				<files>\
				<file>\
				<name>desura.exe</name>\
				<path>\\</path>\
				<flags>10</flags>\
				<tstamp>20111113160342</tstamp>\
				<offset>27853760</offset>\
				<size>2529096</size>\
				<csize>1050209</csize>\
				<nom_csum>662dbac36ceb124666e21db92feb9140</nom_csum>\
				<com_csum>d73918dfc59b6b7cd7a7836aff14e2ab</com_csum>\
				<crc blocksize=\"524288\">Gg+8nGYE5yDZ2vok</crc>\
				</file>\
				</files>\
				</mcf>\
				</appupdate>\
				");

			fh.write(strData.c_str(), strData.size());
		}
		
		gcWString strFile(SRC.string());

		UMcf umcf;
		ASSERT_EQ(MCF_OK, umcf.loadFromFile(strFile.c_str()));
	}
}