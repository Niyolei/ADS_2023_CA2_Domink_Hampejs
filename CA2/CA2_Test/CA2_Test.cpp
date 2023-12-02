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
			Tree<DTO*>* tree = readXMl("test.xml");
			Assert::IsNull(tree->getData());
		}

		TEST_METHOD(FileFoundTest)
		{
			Tree<DTO*>* tree = readXMl("test.xml");
			Assert::IsNotNull(tree->getData());
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
			stringstream ss("test</name>");
			string data = getDataFromTag(DataTag::name,ss);
			Assert::AreEqual("test", data.c_str());
		}

		TEST_METHOD(GetDataFromTag_InvalidTest) {
			stringstream ss("test</name>");
			string data = getDataFromTag(DataTag::directory, ss);
			Assert::AreEqual("", data.c_str());
		
		}

	};
}