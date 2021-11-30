#ifndef SVGPARSER_H
#define SVGPARSER_H

#include <libxml/tree.h>
#include <libxml/parser.h>
#include <stdio.h>
#include <string.h>

typedef struct xmlNodeShape_s xmlNodeShape;
typedef struct xmlNodeAttribute_s xmlNodeAttribute;


struct xmlNodeShape_s {
	xmlNode* data;
	struct xmlNodeShape_s* next;
};

struct xmlNodeAttribute_s {
	char* key;
	char* value;

	struct xmlNodeAttribute_s* next;
};

/**
* @brief Load the svg in memory as a tree
* \param[in] svg_path path to svg file
* \return 
*/
xmlDocPtr PARSER_LoadSVG(char* svg_path);
/**
* @brief Free the svg tree in memory
* \param[in] svg_tree svg tree to free
*/
void PARSER_FreeSVG(xmlDocPtr svg_tree);
/**
* @brief Get nodes containing shapes
* \param[in] svg_tree pointer to the svg tree
* \return shape_stack
*/
xmlNodeShape* PARSER_GetShapesFromSVG(xmlDocPtr svg_tree);
/**
* @brief Returning shapes node from the svg tree
* \param[in] svg_tree the svg tree
* \return  
*/
void PARSER_ReadShapesFromSVG(xmlNodeShape** shapes, xmlNode* svg_node);
/**
* @brief Adding shape node to a list of shapes
* \param[in] shapes_node the list of shapes
* \param[in] node_to_add the shape node to add
*/
void PARSER_AddShapesNode(xmlNodeShape** shapes_node, xmlNode* node_to_add);

/**
* @brief Get attributes from a shape node 
* \param[in] shapes_node 
* \return attribute_stack
* 
*/
void PARSER_FreeShapeStack(xmlNodeShape* shape_stack);

xmlNodeAttribute* PARSER_GetAttributesFromShape(xmlNodeShape* shapes_node);
/**
* @brief Add attribute to a stack of attributes
* \param[in] attribute_stack the stack of attributes
* \param[in] attribute_to_add the attribute to add to the stack
*/
void PARSER_AddAttributes(xmlNodeAttribute** attribute_stack, xmlNodeAttribute* attribute_to_add);
/**
* @brief Read attributes from a shape node 
* \param[in] attribute_stack the stack of attributes
* \param[in] shape_node the shape node to read attributes from
*/
void PARSER_ReadAttributesFromShape(xmlNodeAttribute** attribute_stack, xmlNodeShape* shape_node);
void PARSER_FreeAttributeStack(xmlNodeAttribute* attribute_stack);

#endif // !PARSER_H