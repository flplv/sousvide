#ifndef ERROR_H
#define ERROR_H

enum errors
{
    error_owts_no_presence,
};

/* Platform dependent error function. */
void error (enum errors);

#endif //ERRO_H
