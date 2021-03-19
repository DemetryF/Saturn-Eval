#include <stdio.h>
#include <stdlib.h>

#include <complex.h>

#include "utils.h"
#include "find_divider_pos.h"
#include "remove_brackets.h"

void parser(Node_s *node, size_t *status, Operator_s **operators)
{
    node->divider_pos = find_divider_pos(node->tokens, node->length, operators, status);
    if (node->divider_pos == -1)
        remove_brackets(node);
    
    node->divider_pos = find_divider_pos(node->tokens, node->length, operators, status);

    if (node->divider_pos == -1) return;

    node->left = new_node(node->tokens, node->divider_pos, NULL, NULL, '\0', 0);
    node->right = new_node(node->tokens + node->divider_pos, node->length - node->divider_pos - 1, NULL, NULL, '\0', 0);

    parser(node->left, status, operators);
    parser(node->right, status, operators);
}