#ifndef DHT_H_
#define DHT_H_

#include "main.h"

/* Настройки */
#define DHT_TIMEOUT 10000               // Количество итераций, после которых функция вернёт пустые значения 翻译：函数返回空值后的迭代次数
#define DHT_POLLING_CONTROL 0           // Включение проверки частоты опроса датчика 翻译：启用检查传感器的查询频率
#define DHT_POLLING_INTERVAL_DHT11 2000 // Интервал опроса DHT11 (0.5 Гц по даташиту). Можно поставить 1500, будет работать 翻译：DHT11的查询间隔（根据数据表0.5Hz）。可以设置为1500，它将工作
#define DHT_POLLING_INTERVAL_DHT22 1000 // Интервал опроса DHT22 (1 Гц по даташиту)
#define DHT_IRQ_CONTROL                 // Выключать прерывания во время обмена данных с датчиком
/* Структура возвращаемых датчиком данных */
typedef struct
{
    float hum;
    float temp;
} DHT_data;

/* Тип используемого датчика */
typedef enum
{
    DHT11,
    DHT22
} DHT_type;

/* Структура объекта датчика */
typedef struct
{
    GPIO_TypeDef *DHT_Port; // Порт датчика (GPIOA, GPIOB, etc)
    uint16_t DHT_Pin;       // Номер пина датчика (GPIO_PIN_0, GPIO_PIN_1, etc)
    DHT_type type;          // Тип датчика (DHT11 или DHT22)
    uint8_t pullUp;         // Нужна ли подтяжка линии данных к питанию (GPIO_NOPULL - нет, GPIO_PULLUP - да)

    // Контроль частоты опроса датчика. Значения не заполнять!
    uint32_t lastPollingTime; // Время последнего опроса датчика
    float lastTemp;           // Последнее значение температуры
    float lastHum;            // Последнее значение влажности
} DHT_sensor;

/* Прототипы функций */
DHT_data DHT_getData(DHT_sensor *sensor); // Получить данные с датчика
extern DHT_sensor THbCONFIG;
extern DHT_sensor THcCONFIG;
extern DHT_data THb;
extern DHT_data THc;
#endif