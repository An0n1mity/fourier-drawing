#define _CRT_SECURE_NO_WARNINGS

#include "svgparser.h"

xmlDocPtr PARSER_LoadSVG(char* svg_path)
{
	xmlDocPtr svg_tree;
	svg_tree = xmlReadFile(svg_path, NULL, 0);//salut :)
	if (!svg_tree)
		return NULL;

	return svg_tree;
}

void PARSER_FreeSVG(xmlDocPtr svg_tree)
{
	xmlFreeDoc(svg_tree);
}

xmlNodeShape* PARSER_GetShapesFromSVG(xmlDocPtr svg_tree)
{
    xmlNodeShape* shapes = NULL;
    xmlNode* svg_node = xmlDocGetRootElement(svg_tree);
    PARSER_ReadShapesFromSVG(&shapes, svg_node);

    return shapes;
}

void PARSER_ReadShapesFromSVG(xmlNodeShape** shapes, xmlNode* svg_node)
{
    for (svg_node = svg_node->children; svg_node != NULL; svg_node = svg_node->next) {
        if (svg_node->type == XML_ELEMENT_NODE && (strcmp(svg_node->name, "rect") == 0) ||
            (strcmp(svg_node->name, "circle") == 0) || 
            (strcmp(svg_node->name, "point") == 0) || 
            (strcmp(svg_node->name, "ellipse") == 0) || 
            (strcmp(svg_node->name, "line") == 0) || 
            (strcmp(svg_node->name, "polyline") == 0) || 
            (strcmp(svg_node->name, "polygone") == 0) ||
            (strcmp(svg_node->name, "path") == 0)
            ) {
            PARSER_AddShapesNode(shapes, svg_node);
        }
        PARSER_ReadShapesFromSVG(shapes, svg_node);
    }
}

void PARSER_AddShapesNode(xmlNodeShape** shape_stack, xmlNode* node_to_add)
{
    xmlNodeShape* shape_node = (xmlNodeShape*)calloc(sizeof(xmlNodeShape), 1);
    if (!shape_node)
        return;

    shape_node->data = (xmlNode*)malloc(sizeof(xmlNode));

    if(shape_node->data)
        memcpy(shape_node->data, node_to_add, sizeof(xmlNode));

    shape_node->next = (*shape_stack);
    (*shape_stack) = shape_node;

}

void PARSER_FreeShapeStack(xmlNodeShape* shape_stack)
{
    if (!shape_stack)
        return;
    PARSER_FreeShapeStack(shape_stack->next);
    free(shape_stack->data);
    free(shape_stack);
}


// Retrieve from a shape a list of attributes
xmlNodeAttribute* PARSER_GetAttributesFromShape(xmlNodeShape* shapes_node)
{
    xmlNodeAttribute* attributes = NULL;
    while (shapes_node)
    {
        PARSER_ReadAttributesFromShape(&attributes, shapes_node);
        shapes_node = shapes_node->next;
    }

    return attributes;
}

// Adding an attribute to the attributes stack
void PARSER_AddAttributes(xmlNodeAttribute** attribute_stack, xmlNodeAttribute* attribute_to_add)
{
    if (!(*attribute_stack))
    {
        *attribute_stack = attribute_to_add;
        return;
    }

    attribute_to_add->next = *attribute_stack;
    *attribute_stack = attribute_to_add;
}

// Reading attributes to a attributes stack
void PARSER_ReadAttributesFromShape(xmlNodeAttribute** attribute_stack, xmlNodeShape* shape_node)
{
    xmlChar* attribute_value, *attribute_key;

    while (shape_node->data->properties)
    {
        xmlNodeAttribute* attribute = (xmlNodeAttribute*)calloc(sizeof(xmlNodeAttribute), 1);
        if (!attribute)
            return;

        attribute->key = (char*)malloc(sizeof(char) * 10);
        attribute->value = (char*)malloc(sizeof(char) * 10);
        
        attribute_key = shape_node->data->properties->name;
        attribute_value = xmlNodeListGetString(shape_node->data->doc, shape_node->data->properties->children, 1);
        
        if(attribute->key)
            strcpy(attribute->key, attribute_key);
        if(attribute->value)
            strcpy(attribute->value, attribute_value);

        PARSER_AddAttributes(attribute_stack, attribute);

        shape_node->data->properties = shape_node->data->properties->next;
    }
}

void PARSER_FreeAttributeStack(xmlNodeAttribute* attribute_stack)
{
    if (!attribute_stack)
        return;
    PARSER_FreeAttributeStack(attribute_stack->next);
    free(attribute_stack->key);
    free(attribute_stack->value);
    free(attribute_stack);
}



