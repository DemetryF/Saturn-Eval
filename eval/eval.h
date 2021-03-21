#include <stdlib.h>

#include "lexer/utils.h"
#include "lexer/lexer.h"

#include "parser/utils.h"
#include "parser/find_divider_pos.h"
#include "parser/parser.h"

#include "operators.h"
#include "free.h"
#include "solve.h"

double eval(char *expression, size_t *status)
{
    size_t tokens_count;
    Operator_s **operators = init_operators();
    Token_s **tokens = lexer(expression, &tokens_count, status, operators);

    if (*status)
    {
        eval_free(operators, tokens, tokens_count, NULL);
        return 0.0;
    }

    Node_s *head = new_node(tokens, tokens_count, NULL, NULL, 0, 0);
    parser(head, status, operators);

    if (*status)
    {
        eval_free(operators, tokens, tokens_count, head);
        return 0.0;
    }

    double result = solve(head, operators);
    eval_free(operators, tokens, tokens_count, head);

    return result;
}