#ifndef ERROR_H
#define ERROR_H

enum errors
{
    error_owts_event_buffer_too_small
};

/* Platform dependent error function. */
void error (enum errors);

#endif //ERRO_H
