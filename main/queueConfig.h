

#ifndef MAIN_QUEUECONFIG_H_
#define MAIN_QUEUECONFIG_H_

#include <string.h>
#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
// [Config available] Maximum length of queue
#define __MAXLEN__ 10
//[Config available]  If some item has id larger than __NOISE_VOLUME__, it will be labeled as noise
#define __NOISE_VOLUME__ 100
typedef struct
{
    uint32_t index;
    char *content;
} code_t;
// this function is create a object
// In this lab "object" is a code
code_t create_code(uint32_t input_index, char *input_content);


#endif /* MAIN_QUEUECONFIG_H_ */
