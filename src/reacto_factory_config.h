#ifndef EXAMPLES_HOST_CONTEXT_REACTO_FACTORY_CONFIG_H_
#define EXAMPLES_HOST_CONTEXT_REACTO_FACTORY_CONFIG_H_

enum reacto_queue_id
{
    reacto_queue_id_temperature = 0,
    reacto_queue_id_ui = 1,
    reacto_queue_id_log = 2,
    reacto_queue_id_max,
};

#define REACTO_N_OF_QUEUES reacto_queue_id_max
#define REACTO_MAX_N_OF_HANDLERS_PER_QUEUE 1

#endif /* EXAMPLES_HOST_CONTEXT_REACTO_FACTORY_CONFIG_H_ */
