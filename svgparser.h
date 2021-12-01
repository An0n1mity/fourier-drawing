#ifndef SVGPARSER_H
#define SVGPARSER_H

#include <libxml/tree.h>
#include <libxml/parser.h>
#include <stdio.h>
#include <string.h>

typedef struct svgShapeStack_s svgShapeStack;
typedef struct svgAttributeStack_s svgAttributeStack;


struct svgAttributeStack_s {
	char* key;
	char* value;

	struct svgAttributeStack_s* next;
};

struct svgShapeStack_s {
	char* name;
	struct svgAttributeStack_s* attributes;
	
	struct svgShapeStack_s* next;
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
void PARSER_FreeSVG(xmlDocPtr xml_tree);
/**
* @brief Get nodes containing shapes
* \param[in] svg_tree pointer to the svg tree
* \return shape_stack
*/
svgShapeStack* PARSER_GetShapesFromSVG(xmlDocPtr xml_tree);
/**
* @brief Returning shapes node from the svg tree
* \param[in] svg_tree the svg tree
* \return  
*/
void PARSER_ReadShapesFromXMLTree(svgShapeStack** svg_shape_stack, xmlNode* xml_root_node);
/**
* @brief Adding shape node to a list of shapes
* \param[in] shapes_node the list of shapes
* \param[in] node_to_add the shape node to add
*/
void PARSER_AddShapeToStack(svgShapeStack** svg_shape_stack, svgShapeStack* svg_shape);

/**
* @brief Get attributes from a shape node 
* \param[in] shapes_node 
* \return attribute_stack
* 
*/
void PARSER_FreeShapeStack(svgShapeStack* svg_shape_stack);

svgAttributeStack* PARSER_GetAttributesFromXMLNode(xmlNode* xml_node);
/**
* @brief Add attribute to a stack of attributes
* \param[in] attribute_stack the stack of attributes
* \param[in] attribute_to_add the attribute to add to the stack
*/
void PARSER_AddAttributesToStack(svgAttributeStack** svg_attribute_stack, svgAttributeStack* attribute_to_add);
/**
* @brief Read attributes from a shape node 
* \param[in] attribute_stack the stack of attributes
* \param[in] shape_node the shape node to read attributes from
*/
void PARSER_ReadAttributesFromXMLNode(svgAttributeStack** svg_attribute_stack, xmlNode* xml_node);
void PARSER_FreeAttributeStack(svgAttributeStack* svg_attribute_stack);

void PARSER_ReadAtrributesFromPath(svgAttributeStack* path_attribute);

#endif // !PARSER_H