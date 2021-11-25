#include "svgparser.h"

svgTree PARSER_LoadSVG(char* svg_path)
{
	svgTree svg_tree;
	svg_tree = xmlReadFile(svg_path, NULL, 0);
	if (!svg_tree)
		return NULL;

	return svg_tree;
}

void PARSER_FreeSVG(svgTree svg_tree)
{
	xmlFreeDoc(svg_tree);
}

void PARSER_PrintNames(xmlNode* svg_node)
{
	if (!svg_node)
		return;
	printf("%s\n", svg_node->name);
	PARSER_PrintNames(svg_node->children);
}
