#include "pch.h"
#include "CppUnitTest.h"
#include "../CA2/Tree.h"
#include "../CA2/Functions.cpp"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CA2Test
{
	TEST_CLASS(FileReading)
	{
	public:
		TEST_METHOD(FileNotFoundTest)
		{
			try {
				Tree<DTO*>* tree = readXMl("XML_NotFound.xml");
			}
			catch (logic_error e) {
				Assert::AreEqual("File not found", e.what());
			}
		}

		TEST_METHOD(FileFoundTest)
		{
			try {
				Tree<DTO*>* tree = readXMl("XML_JustDir.xml");
				Assert::IsNotNull(tree);
			}
			catch (logic_error e) {
				Assert::Fail();
			}
		}

		TEST_METHOD(ParseTagDirTest)
		{
			stringstream ss("<dir>");
			DataTag tag = parseTag(ss);
			Assert::AreEqual((int)directory, (int)tag);
		}

		TEST_METHOD(ParseTagFileTest)
		{
			stringstream ss("<file>");
			DataTag tag = parseTag(ss);
			Assert::AreEqual((int)file, (int)tag);
		}

		TEST_METHOD(ParseTagWithIndent)
		{
			stringstream ss("   <file>");
			DataTag tag = parseTag(ss);
			Assert::AreEqual((int)file, (int)tag);
		}

		TEST_METHOD(ParseTagInvalidTest)
		{
			stringstream ss("<invalid>");
			try {
				DataTag tag = parseTag(ss);
			}
			catch (logic_error e) {
				Assert::AreEqual("Invalid tag", e.what());
			}
		}

		TEST_METHOD(GetDataFromTag_ValidTest) {
			stringstream ss("ADS_Single_LinkedList_Exercises</name>");
			string data = getDataFromTag(closeName, ss);
			Assert::AreEqual("ADS_Single_LinkedList_Exercises", data.c_str());
		}

		TEST_METHOD(GetDataFromTag_InvalidTest) {
			stringstream ss("test</name>");
			string data = getDataFromTag(DataTag::directory, ss);
			Assert::AreEqual("", data.c_str());
		}

		TEST_METHOD(ParseTagAndGetDataValidName) {
			stringstream ss("	<name>ADS_Single_LinkedList_Exercises</name>");
			DataTag tag = parseTag(ss);
			string data = getDataFromTag(closeName, ss);
			Assert::AreEqual("ADS_Single_LinkedList_Exercises", data.c_str());
		}

		TEST_METHOD(readXMLJustDirs) {
			Tree<DTO*>* tree = readXMl("XML_JustDir.xml");
			Assert::IsNotNull(tree);
			Assert::IsNotNull(tree->data);
		}

	};
}