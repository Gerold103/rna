#ifndef __ASTNODE_H__
#define __ASTNODE_H__

class ASTNode {
public:
	enum NodeType {
		STATEMENT,
		EXPRESSION,
		CATALOG_OBJECT,
	};

	NodeType ASTType() { return ast_type; }

	ASTNode(NodeType tp) : ast_type(tp) { }

	virtual ~ASTNode() { }
	
private:
	NodeType ast_type;
};

#endif