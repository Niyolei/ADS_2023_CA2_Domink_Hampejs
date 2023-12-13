#include "pch.h"
#include "CppUnitTest.h"
#include "../CA2/Tree.h"
#include "../CA2/TreeIterator.h"
#include "../CA2/Dlist.h"
#include "../CA2/DListNode.h"
#include "../CA2/DListIterator.h"
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
				Tree<DTO*> tree = Tree<DTO*>(nullptr);
				readXMl("XML_NotValid.xml", tree);
			}
			catch (logic_error e) {
				Assert::AreEqual("File not found", e.what());
			}
		}

		TEST_METHOD(FileFoundTest)
		{
			try {
				Tree<DTO*> tree = Tree<DTO*>(nullptr);
				readXMl("vs_sample_simple.xml", tree);
				Assert::AreEqual("ADS_Single_LinkedList_Exercises", tree.data->name.c_str());
			}
			catch (logic_error e) {
				Logger::WriteMessage(e.what());
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
			Tree<DTO*> tree = Tree<DTO*>(nullptr);
			readXMl("XML_JustDir.xml", tree);
			Assert::IsNotNull(tree.data);
			Assert::AreEqual(tree.count(), 2);
		}

		TEST_METHOD(readXMLVSSimple) {
			Tree<DTO*> tree = Tree<DTO*>(nullptr);
			readXMl("vs_sample_simple.xml", tree);
			Assert::IsNotNull(tree.data);
			Assert::AreEqual(12, tree.count());
		}

		TEST_METHOD(readXMLVSSimpleNesting) {
			Tree<DTO*> tree = Tree<DTO*>(nullptr);
			readXMl("vs_sample_simple.xml", tree);
			Assert::IsNotNull(tree.data);
			Assert::AreEqual(tree.children->getIterator().item()->count(),4);
			
		}

	
		TEST_METHOD(readXMLInvalidName) {
			Tree<DTO*> tree = Tree<DTO*>(nullptr);
			try {
				readXMl("vs_sample_InvalidName.xml", tree);
			}
			catch (logic_error e) {
				Assert::AreEqual("Invalid Data", e.what());
			}
		}

		TEST_METHOD(readXMLInvalidNesting) {
			Tree<DTO*> tree = Tree<DTO*>(nullptr);
			try {
				readXMl("vs_sample_InvalidNesting.xml", tree);
			}
			catch (logic_error e) {
				Assert::AreEqual("Invalid Nesting", e.what());
			}
		}

		
		TEST_METHOD(readXMLInvalidTag) {
			Tree<DTO*> tree = Tree<DTO*>(nullptr);
			try {
				readXMl("vs_sample_InvalidSyntax.xml", tree);
			}
			catch (logic_error e) {
				Assert::AreEqual("Invalid tag", e.what());
			}
		}

		TEST_METHOD(VerifyTagValidTest) {
			stack<DataTag> tags;
			tags.push(directory);

			bool outcome = verifyTag(closeDir, tags);

			Assert::IsTrue(outcome);
		}

		TEST_METHOD(VerifyTagInvalidTest) {
			stack<DataTag> tags;
			tags.push(directory);

			bool outcome = verifyTag(closeFile, tags);

			Assert::IsFalse(outcome);
		}
	};

	TEST_CLASS(TreeFucntionsTests)
	{
		TEST_METHOD(getItemBasedOnPath_Root) {
			Tree<DTO*> tree = Tree<DTO*>(nullptr);
			readXMl("XML_JustDir.xml", tree);
			string path = "ADS_Single_LinkedList_Exercises";
			Tree<DTO*> foundItem = Tree<DTO*>(nullptr);
			Assert::IsTrue(getItemBasedOnPath(&tree, path, foundItem));
			Assert::AreEqual("ADS_Single_LinkedList_Exercises", foundItem.data->name.c_str());
		};

		TEST_METHOD(getItemBasedOnPath_ChildDir) {
			Tree<DTO*> tree = Tree<DTO*>(nullptr);
			readXMl("vs_sample_simple.xml", tree);
			string path = "ADS_Single_LinkedList_Exercises/.git";
			Tree<DTO*> foundItem = Tree<DTO*>(nullptr);
			Assert::IsTrue(getItemBasedOnPath(&tree, path, foundItem));
			Assert::AreEqual(".git", foundItem.data->name.c_str());
		};

		TEST_METHOD(getItemBasedOnPath_ChildFile) {
			Tree<DTO*> tree = Tree<DTO*>(nullptr);
			readXMl("vs_sample_simple.xml", tree);
			string path = "ADS_Single_LinkedList_Exercises/.git/config";
			Tree<DTO*> foundItem = Tree<DTO*>(nullptr);
			Assert::IsTrue(getItemBasedOnPath(&tree, path, foundItem));
			Assert::AreEqual("config", foundItem.data->name.c_str());
		};

		TEST_METHOD(getItemBasedOnPath_InvalidPath) {
			Tree<DTO*> tree = Tree<DTO*>(nullptr);
			readXMl("vs_sample_simple.xml", tree);
			string path = "ADS_Single_LinkedList_Exercises/.git/config/invalid";
			Tree<DTO*> foundItem = Tree<DTO*>(nullptr);
			Assert::IsFalse(getItemBasedOnPath(&tree, path, foundItem));
		};

		TEST_METHOD(getItemBasedOnPath_InvalidPathRoot) {
			Tree<DTO*> tree = Tree<DTO*>(nullptr);
			readXMl("vs_sample_simple.xml", tree);
			string path = "ADS_Single.git/config";
			Tree<DTO*> foundItem = Tree<DTO*>(nullptr);
			Assert::IsFalse(getItemBasedOnPath(&tree, path, foundItem));
		};

		TEST_METHOD(getFolderContentAmount_FolderWithJustFiles) {
			Tree<DTO*> tree = Tree<DTO*>(nullptr);
			readXMl("vs_sample_simple.xml", tree);
			string path = "ADS_Single_LinkedList_Exercises/.git";
			Tree<DTO*> foundItem = Tree<DTO*>(nullptr);
			getItemBasedOnPath(&tree, path, foundItem);
			Assert::AreEqual(3, getFolderContentAmount(&foundItem));
		};

		TEST_METHOD(getFolderContentAmount_FolderWithJustFolders) {
			Tree<DTO*> tree = Tree<DTO*>(nullptr);
			readXMl("vs_sample_simple.xml", tree);
			string path = "ADS_Single_LinkedList_Exercises/.vs";
			Tree<DTO*> foundItem = Tree<DTO*>(nullptr);
			getItemBasedOnPath(&tree, path, foundItem);
			Assert::AreEqual(2, getFolderContentAmount(&foundItem));
		};

		TEST_METHOD(getFolderContentAmount_FolderWithFoldersAndFiles) {
			Tree<DTO*> tree = Tree<DTO*>(nullptr);
			readXMl("vs_sample_simple.xml", tree);
			string path = "ADS_Single_LinkedList_Exercises";
			Tree<DTO*> foundItem = Tree<DTO*>(nullptr);
			getItemBasedOnPath(&tree, path, foundItem);
			Assert::AreEqual(11, getFolderContentAmount(&foundItem));
		};

		TEST_METHOD(getFolderContentAmount_File) {
			Tree<DTO*> tree = Tree<DTO*>(nullptr);
			readXMl("vs_sample_simple.xml", tree);
			string path = "ADS_Single_LinkedList_Exercises/.git/config";
			Tree<DTO*> foundItem = Tree<DTO*>(nullptr);
			getItemBasedOnPath(&tree, path, foundItem);
			Assert::AreEqual(-1, getFolderContentAmount(&foundItem));
		};

		TEST_METHOD(getFolderMemoryUsage_EmptyFolder) {
			Tree<DTO*> tree = Tree<DTO*>(nullptr);
			readXMl("vs_sample_simple.xml", tree);
			string path = "ADS_Single_LinkedList_Exercises/.vs";
			Tree<DTO*> foundItem = Tree<DTO*>(nullptr);
			getItemBasedOnPath(&tree, path, foundItem);
			Assert::AreEqual(0, getFolderMemoryUsage(&foundItem));
		};

		TEST_METHOD(getFolderMemoryUsage_FolderWithJustFiles) {
			Tree<DTO*> tree = Tree<DTO*>(nullptr);
			readXMl("vs_sample_simple.xml", tree);
			string path = "ADS_Single_LinkedList_Exercises/.git";
			Tree<DTO*> foundItem = Tree<DTO*>(nullptr);
			getItemBasedOnPath(&tree, path, foundItem);
			Assert::AreEqual(449, getFolderMemoryUsage(&foundItem));
		};

		TEST_METHOD(getFolderMemoryUsage_File) {
			Tree<DTO*> tree = Tree<DTO*>(nullptr);
			readXMl("vs_sample_simple.xml", tree);
			string path = "ADS_Single_LinkedList_Exercises/.git/config";
			Tree<DTO*> foundItem = Tree<DTO*>(nullptr);
			getItemBasedOnPath(&tree, path, foundItem);
			Assert::AreEqual(-1, getFolderMemoryUsage(&foundItem));
		};

		TEST_METHOD(getFilePath_InvalidSearch) {
			Tree<DTO*> tree = Tree<DTO*>(nullptr);
			readXMl("vs_sample_simple.xml", tree);
			string searched = "invalid";
			string path = "";
			TreeIterator<DTO*> iter(&tree);
			Assert::IsFalse(getFilePath(searched, iter, path, true));
		};

		TEST_METHOD(getFilePath_Root) {
			Tree<DTO*> tree = Tree<DTO*>(nullptr);
			readXMl("vs_sample_simple.xml", tree);
			string searched = "ADS_Single_LinkedList_Exercises";
			string path = "";
			TreeIterator<DTO*> iter(&tree);
			Assert::IsTrue(getFilePath(searched, iter, path, true));
			Assert::AreEqual("ADS_Single_LinkedList_Exercises", path.c_str());
		};

		TEST_METHOD(getFilePath_ChildDir) {
			Tree<DTO*> tree = Tree<DTO*>(nullptr);
			readXMl("vs_sample_simple.xml", tree);
			string searched = ".git";
			string path = "";
			TreeIterator<DTO*> iter(&tree);
			Assert::IsTrue(getFilePath(searched, iter, path, true));
			Assert::AreEqual("ADS_Single_LinkedList_Exercises/.git", path.c_str());
		};

		TEST_METHOD(getFilePath_ChildFile) {
			Tree<DTO*> tree = Tree<DTO*>(nullptr);
			readXMl("vs_sample_simple.xml", tree);
			string searched = "config";
			string path = "";
			TreeIterator<DTO*> iter(&tree);
			Assert::IsTrue(getFilePath(searched, iter, path, true));
			Assert::AreEqual("ADS_Single_LinkedList_Exercises/.git/config", path.c_str());
		};

		TEST_METHOD(displayFolderByPath_Valid) {
			Tree<DTO*> tree = Tree<DTO*>(nullptr);
			readXMl("vs_sample_simple.xml", tree);
			string path = "ADS_Single_LinkedList_Exercises/.git";
			Assert::IsTrue(displayFolderByPath(&tree, path));
		};

		TEST_METHOD(displayFolderByPath_Invalid) {
			Tree<DTO*> tree = Tree<DTO*>(nullptr);
			readXMl("vs_sample_simple.xml", tree);
			string path = "ADS_Single_LinkedList_Exercises/.git/invalid";
			Assert::IsFalse(displayFolderByPath(&tree, path));
		};

		TEST_METHOD(pruneTest) {
			Tree<DTO*> tree = Tree<DTO*>(nullptr);
			readXMl("vs_sample_simple.xml", tree);
			TreeIterator<DTO*> iter(&tree);
			prune(iter);
			Assert::AreEqual(9, tree.count());
		}


	
	};
}