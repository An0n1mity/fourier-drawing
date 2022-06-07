//
// Created by biist on 18/04/2022.
//

#include "svgparser.h"

xmlDocPtr PARSER_LoadSVG(char* svg_path)
{
    xmlDocPtr svg_tree;
    svg_tree = xmlReadFile(svg_path, NULL, 0);
    if (!svg_tree) {
        fprintf(stderr, "Error : File %s could not be load\n", svg_path);
        return NULL;
    }

    return svg_tree;
}

void PARSER_FreeSVG(xmlDocPtr xml_tree)
{
    xmlFreeDoc(xml_tree);
}

svgShapeStack* PARSER_GetShapesFromSVG(xmlDocPtr xml_tree)
{
    if (!xml_tree) {
        fprintf(stderr, "Error : No svg was loaded in memory\n");
        return NULL;
    }
    svgShapeStack* shapes = NULL;
    xmlNode* svg_node = xmlDocGetRootElement(xml_tree);
    PARSER_ReadShapesFromXMLTree(&shapes, svg_node);

    return shapes;
}

void PARSER_ReadShapesFromXMLTree(svgShapeStack** svg_shape_stack, xmlNode* xml_root_node)
{
    if (!svg_shape_stack)
    {
        fprintf(stderr, "Error : Argument shape_stack is void\n");
        return;
    }
    for (xml_root_node = xml_root_node->children; xml_root_node != NULL; xml_root_node = xml_root_node->next) {
        if (xml_root_node->type == XML_ELEMENT_NODE && (strcmp(xml_root_node->name, "rect") == 0) ||
            (strcmp((const char*)xml_root_node->name, "circle") == 0) ||
            (strcmp((const char*)xml_root_node->name, "point") == 0) ||
            (strcmp((const char*)xml_root_node->name, "ellipse") == 0) ||
            (strcmp((const char*)xml_root_node->name, "line") == 0) ||
            (strcmp((const char*)xml_root_node->name, "polyline") == 0) ||
            (strcmp((const char*)xml_root_node->name, "polygon") == 0) ||
            (strcmp((const char*)xml_root_node->name, "path") == 0)
                ) {
            svgShapeStack* svg_shape = (svgShapeStack*)calloc(sizeof(svgShapeStack), 1);
#ifdef _WIN32
            svg_shape->name = _strdup(xml_root_node->name);
#endif
#ifdef __linux__
            svg_shape->name = strdup((const char*)xml_root_node->name);
#endif
            svg_shape->attributes = PARSER_GetAttributesFromXMLNode(xml_root_node);
            PARSER_AddShapeToStack(svg_shape_stack, svg_shape);
        }
        PARSER_ReadShapesFromXMLTree(svg_shape_stack, xml_root_node);
    }
}

void PARSER_AddShapeToStack(svgShapeStack** svg_shape_stack, svgShapeStack* svg_shape)
{
    if (!svg_shape_stack)
    {
        fprintf(stderr, "Error : Argument shape_stack is void\n");
        return;
    }

    svg_shape->ns = (*svg_shape_stack);
    (*svg_shape_stack) = svg_shape;

}

void PARSER_FreeShapeStack(svgShapeStack* svg_shape_stack)
{
    if (!svg_shape_stack)
        return;

    PARSER_FreeAttributeStack(svg_shape_stack->attributes);
    PARSER_FreeShapeStack(svg_shape_stack->ns);
    free(svg_shape_stack->name);
    free(svg_shape_stack);
}


// Retrieve from a shape a list of attributes
svgAttributeStack* PARSER_GetAttributesFromXMLNode(xmlNode* xml_node)
{
    if (!xml_node)
    {
        fprintf(stderr, "Error : Argument shape_stack is void\n");
        return NULL;
    }

    svgAttributeStack* attributes = NULL;
    PARSER_ReadAttributesFromXMLNode(&attributes, xml_node);

    return attributes;
}

// Adding an attribute to the attributes stack
void PARSER_AddAttributesToStack(svgAttributeStack** svg_attribute_stack, svgAttributeStack* attribute_to_add)
{
    if (!svg_attribute_stack)
    {
        fprintf(stderr, "Error : Argument attribute_stack is void\n");
        return;
    }

    if (!(*svg_attribute_stack))
    {
        *svg_attribute_stack = attribute_to_add;
        return;
    }

    attribute_to_add->na = *svg_attribute_stack;
    *svg_attribute_stack = attribute_to_add;
}

// Reading attributes to a attributes stack
void PARSER_ReadAttributesFromXMLNode(svgAttributeStack** svg_attribute_stack, xmlNode* xml_node)
{
    if (!svg_attribute_stack)
    {
        fprintf(stderr, "Error : Argument attribute_stack is void\n");
        return;
    }

    xmlChar* attribute_value, *attribute_key;

    while (xml_node->properties)
    {
        svgAttributeStack* attribute = (svgAttributeStack*)calloc(sizeof(svgAttributeStack), 1);
        if (!attribute)
            return;

        attribute_key = xml_node->properties->name;
        attribute_value = xmlNodeListGetString(xml_node->doc, xml_node->properties->children, 1);

        if(attribute_key)
#ifdef _WIN32
            attribute->key = _strdup(attribute_key);
#endif
#ifdef __linux__
            attribute->key = strdup(attribute_key);
#endif
        if(attribute_value)
#ifdef _WIN32
        attribute->value = _strdup(attribute_value);
#endif
#ifdef __linux__
        attribute->value = strdup(attribute_value);
#endif
        PARSER_AddAttributesToStack(svg_attribute_stack, attribute);

        xml_node->properties = xml_node->properties->next;
    }
}

void PARSER_FreeAttributeStack(svgAttributeStack* svg_attribute_stack)
{
    if (!svg_attribute_stack) {
        return;
    }

    PARSER_FreeAttributeStack(svg_attribute_stack->na);
    free(svg_attribute_stack->key);
    free(svg_attribute_stack->value);
    free(svg_attribute_stack);
}



