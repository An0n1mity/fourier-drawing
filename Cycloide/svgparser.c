#include "svgparser.h"

xmlDocPtr PARSER_LoadSVG(char* svg_path)
{
	xmlDocPtr svg_tree;
	svg_tree = xmlReadFile(svg_path, NULL, 0);
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
    xmlNodeShape* shapes = (xmlNodeShape*)calloc(sizeof(xmlNodeShape*), 1);
    xmlNode* svg_node = xmlDocGetRootElement(svg_tree);
    PARSER_ReadShapesFromSVG(&shapes, svg_node);

    return shapes;
}

void PARSER_ReadShapesFromSVG(xmlNodeShape** shapes, xmlNode* svg_node)
{
    for (svg_node = svg_node->children; svg_node != NULL; svg_node = svg_node->next) {
        if (svg_node->type == XML_ELEMENT_NODE && (strcmp(svg_node->name, "rect") == 0) ||
            (strcmp(svg_node->name, "circle") == 0)
            ) {
            PARSER_AddShapesNode(shapes, svg_node);
        }
        PARSER_ReadShapesFromSVG(shapes, svg_node);
    }
}

void PARSER_AddShapesNode(xmlNodeShape** shapes, xmlNode* node_to_add)
{
    xmlNodeShape* shape_node = (xmlNodeShape*)calloc(sizeof(xmlNodeShape), 1);
    if (!shape_node)
        return;

    shape_node->shape = (xmlNode*)malloc(sizeof(xmlNode));
    memcpy(shape_node->shape, node_to_add, sizeof(xmlNode));

    shape_node->next = (*shapes);
    (*shapes) = shape_node;
}
