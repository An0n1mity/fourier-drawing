#ifndef SVGPARSER_H
#define SVGPARSER_H

#include <libxml/tree.h>
#include <libxml/parser.h>
#include <stdio.h>
#include <string.h>

typedef struct xmlNodeShape_s xmlNodeShape;

struct xmlNodeShape_s {
	xmlNode* shape;
	struct xmlNodeShape_s* next;
};

/**
* @brief Load the svg in memory as a tree
* \param[in] svg_path path to svg file
* \return svgTree
*/
xmlDocPtr PARSER_LoadSVG(char* svg_path);
/**
* @brief Free the svg tree in memory
* \param[in] svg_tree svg tree to free
*/
void PARSER_FreeSVG(xmlDocPtr svg_tree);
/**
* @brief Get nodes containing shapes
*/
xmlNodeShape* PARSER_GetShapesFromSVG(xmlDocPtr svg_tree);
void PARSER_ReadShapesFromSVG(xmlNodeShape** shapes, xmlNode* svg_node);
void PARSER_AddShapesNode(xmlNodeShape** shapes_node, xmlNode* node_to_add);
#endif // !PARSER_H