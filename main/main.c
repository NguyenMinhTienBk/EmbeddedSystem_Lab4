#include <stdio.h>

// RTOS services
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

// GPIO services
#include "esp_system.h"
#include "esp_log.h"
// configuration
#include "sdkconfig.h"

#include "queueConfig.h"

QueueHandle_t perform_set;

static uint16_t numAvalQueue;

static const char *TAG = "LAB4";
static char *chuoi_code[] = {
    "Xinchao123",
    "ngaydaptroi0196",
    "1234567890",
    "MTAK127898",
    "anhkhoa8765",
    "nhokdeptrai0987"};
int length_code=sizeof(chuoi_code) / sizeof(chuoi_code[0]);
code_t create_code(uint32_t input_index, char *input_content)
{
    code_t *self = malloc(sizeof(code_t));
    self->index = input_index;
    self->content = input_content;
    return *self;
}
// Just print some information
void set_up()
{
    printf("********************************\n");
    printf("* Queue length: %d\n", __MAXLEN__);
    printf("* Max ID number: %d\n", __NOISE_VOLUME__);
    printf("********************************\n");
}

// this function send them to a queue
void send_code(QueueHandle_t queue, code_t self)
{
    if (self.index < 100)
    {
        ESP_LOGI(TAG, "[Send] at %ldms, Send item to queue, ID: %ld, Topic: \"%s\"", pdTICKS_TO_MS(xTaskGetTickCount()), self.index, self.content);
        
    }
    else
    {
        ESP_LOGI(TAG, "[Send] at %ldms, [Noise item], ID: %ld, Topic: \"%s\"", pdTICKS_TO_MS(xTaskGetTickCount()), self.index, self.content);

    }
    if (xQueueSend(queue, (void *)&self, (TickType_t)0) != pdTRUE)
    {
        ESP_LOGI(TAG, "[Queue] Unsuccessful! No slot availiable in queue, queue is full");
        printf("* at %ldms, Code failed to send, they are \"OVERLOADED\"\n", pdTICKS_TO_MS(xTaskGetTickCount()));
    }
    else
    {
        numAvalQueue =  uxQueueSpacesAvailable(perform_set);
        ESP_LOGI(TAG, "[Queue] Successful! %d/%d slot left in queue", numAvalQueue, __MAXLEN__);
    }
    printf("\n");
}

// this function presents the reception task
// In this lab, this function is distributed codes to user
void distribute_codes(void *pvParameter)
{
    // Create queue
    perform_set = xQueueCreate(__MAXLEN__, sizeof(code_t));

    // Create valid items
    code_t code[length_code];
    for (uint32_t i = 0; i < length_code; i++)
    {
        code[i] = create_code(i + 1, chuoi_code[i]);
    }
    // Create some noise items
    code_t noise_sentence1 = create_code(999, "!T@#1$&*.V,#$D/");
    code_t noise_sentence2 = create_code(766, "!H@#I$&E*.,U#$/");
    code_t noise_sentence3 = create_code(382, "g!@#g$&w*.,p#$/");

    int idx=0;
    for(;idx<length_code/2;idx++){
        send_code(perform_set, code[idx]);
    }
    // send_code(perform_set, code[0]);
    // send_code(perform_set, code[1]);
    // send_code(perform_set, code[2]);
    // send noise
    send_code(perform_set, noise_sentence1);
    for(;idx<length_code-length_code/6;idx++){
        send_code(perform_set, code[idx]);
    }
    // send_code(perform_set, code[3]);
    // send_code(perform_set, code[4]);
     // send noise
    send_code(perform_set, noise_sentence2);
    for(;idx<length_code;idx++){
        send_code(perform_set, code[idx]);
    }
    // send_code(perform_set, code[5]);
     // send noise
    send_code(perform_set, noise_sentence3);

    while (1)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

// some functional methods

// found a noise item, handle it but not execute, also remove it from queue
// in this lab, User will not receive noise sentences
void removeNoise(void *pvParameter)
{
    while (1)
    {
        code_t self;
        if (xQueuePeek(perform_set, (void *)&self, (TickType_t)5) == pdTRUE)
        {
            if (self.index > __NOISE_VOLUME__)
            {
                ESP_LOGI(TAG, "[Noise] at %ld, Found a NOISE (ID > %d) ", pdTICKS_TO_MS(xTaskGetTickCount()), __NOISE_VOLUME__);
                ESP_LOGI(TAG, "[Noise] at %ld, Remove noise, ID: %ld, Topic: \"%s\"", pdTICKS_TO_MS(xTaskGetTickCount()), self.index, self.content);
                xQueueReceive(perform_set, (void *)&self, (TickType_t)5);
                printf("* Users will not receive this noise sentence: %s\n", self.content);
                printf("\n");
            }
            else
            {
                // printf("* This is not a noise\n");
            }
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

// hadle and excuted the item in queue
// send code to users
void take_code(void *pvParameter)
{
    while (1)
    {
        code_t self;
        if (xQueuePeek(perform_set, (void *)&self, (TickType_t)5) == pdTRUE)
        {
            if (self.index <= __NOISE_VOLUME__)
            {
                ESP_LOGI(TAG, "[Receiver] at %ldms, Handle valid item  ", pdTICKS_TO_MS(xTaskGetTickCount()));
                ESP_LOGI(TAG, "[Receiver] at %ldms, Excuted valid item, ID: %ld, Topic: \"%s\"", pdTICKS_TO_MS(xTaskGetTickCount()), self.index, self.content);
                xQueueReceive(perform_set, (void *)&self, (TickType_t)5);
                printf("* Users take the %ld code\n* %s\n", self.index, self.content);
                printf("\n");
            }
            else
            {
                // printf("* This is a noise\n");
            }
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    set_up();
    esp_log_level_set(TAG, ESP_LOG_INFO);
    xTaskCreatePinnedToCore(distribute_codes, "distribute_codes", 4096, NULL, 5, NULL, 0);
    xTaskCreatePinnedToCore(removeNoise, "removeNoise", 4096, NULL, 2, NULL, 0);
    xTaskCreatePinnedToCore(take_code, "take_code", 4096, NULL, 1, NULL, 0);
}
