#ifndef SVGPARSER_H
#define SVGPARSER_H

#include <libxml/tree.h>
#include <libxml/parser.h>
#include <stdio.h>

typedef struct xmlDoc* svgTree;
typedef struct xmlNode svgNode;

/**
* @brief Load the svg in memory as a tree
* \param[in] svg_path path to svg file
* \return svgTree
*/
svgTree PARSER_LoadSVG(char* svg_path);
/**
* @brief Free the svg tree in memory
* \param[in] svg_tree svg tree to free
*/
void PARSER_FreeSVG(svgTree svg_tree);

void PARSER_PrintNames(xmlNode* svg_node);
#endif // !PARSER_H






