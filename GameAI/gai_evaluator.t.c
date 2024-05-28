#include <gai_evaluator.h>
#include <stdio.h>
#include <stdlib.h>

static void eval(gai_evaluator_t* evaluator){
    int result = gai_evaluator_evaluate(evaluator, 0);
    printf("Evaluator %s: %d\n", evaluator->object.name, result);
}

int main(int argc, char** argv){
    eval(&GAI_TRUE_Evaluator);
    eval(&GAI_FALSE_Evaluator);
    return 0;
}