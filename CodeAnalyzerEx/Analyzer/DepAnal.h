#pragma once
/////////////////////////////////////////////////////////////////////////
// DepAnal.h - Demonstrate how to start developing dependency analyzer //
//                                                                     //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2017           //
/////////////////////////////////////////////////////////////////////////
/*
 * You need to provide all the manual and maintenance informnation
 */

//#include "../Parser/ActionsAndRules.h"
#include <iostream>
#include <functional>
#include "TypeTable.h"

#pragma warning (disable : 4101)  // disable warning re unused variable x, below

namespace CodeAnalysis
{
  class TypeAnal
  {
  
  public:
	 
    using SPtr = std::shared_ptr<ASTNode*>;
    TypeAnal();
	unordered_map<std::string, std::pair<std::string, std::string>> doTypeAnal();
	TypeTable tt;

  private:
	  void DFS(ASTNode* pNode);
	  AbstrSynTree& ASTref_;
	  ScopeStack<ASTNode*> scopeStack_;
	  Scanner::Toker& toker_;
	  
  };
  
  inline TypeAnal::TypeAnal() : 
    ASTref_(Repository::getInstance()->AST()),
    scopeStack_(Repository::getInstance()->scopeStack()),
    toker_(*(Repository::getInstance()->Toker()))        //the only one which returns a pointer
  {
    std::function<void()> test = [] { int x; };  // This is here to test detection of lambdas.
  }                                              // It doesn't do anything useful for dep anal.

  inline bool doDisplay(ASTNode* pNode)
  {
    static std::string toDisplay[] = {
      "function", "lambda", "class", "struct", "enum", "alias", "typedef"
    };
    for (std::string type : toDisplay)
    {
      if (pNode->type_ == type)
        return true;
    }
    return false;
  }

  //DFS - Traversing AST using DFS 
  //inserting into type table
  inline void TypeAnal::DFS(ASTNode* pNode)
  {
    static std::string path = "";
    
	if (pNode->path_ != path)
    {
      std::cout << "\n    -- " << pNode->path_ << "\\" << pNode->package_;
      path = pNode->path_;
    }

    if (doDisplay(pNode))
    {
		tt.insertintable(pNode);
    }
    for (auto pChild : pNode->children_)
      DFS(pChild);

  }
 //displaying the type table
  inline unordered_map<std::string, std::pair<std::string, std::string>> TypeAnal::doTypeAnal()
  {
    std::cout << "\n  starting type analysis:\n";
    //std::cout << "\n  scanning AST and displaying important things:";
    std::cout << "\n -----------------------------------------------";
    ASTNode* pRoot = ASTref_.root();
	unordered_map<std::string, std::pair<std::string, std::string>> temp;
	DFS(pRoot);
	tt.display();

	return tt.getTypeTable();
  }

  

  
}