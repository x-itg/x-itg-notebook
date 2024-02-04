#include "DHT.h"
DHT_sensor THbCONFIG = {GPIOA, GPIO_PIN_10, DHT11, GPIO_NOPULL};
DHT_sensor THcCONFIG = {GPIOA, GPIO_PIN_11, DHT11, GPIO_NOPULL};
DHT_data THb;
DHT_data THc;

#define lineDown() HAL_GPIO_WritePin(sensor->DHT_Port, sensor->DHT_Pin, GPIO_PIN_RESET)
#define lineUp() HAL_GPIO_WritePin(sensor->DHT_Port, sensor->DHT_Pin, GPIO_PIN_SET)
#define getLine() (HAL_GPIO_ReadPin(sensor->DHT_Port, sensor->DHT_Pin) == GPIO_PIN_SET)
#define Delay(d) HAL_Delay(d)

static void goToOutput(DHT_sensor *sensor)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // По умолчанию на линии высокий уровень
    lineUp();

    // Настройка порта на выход
    GPIO_InitStruct.Pin = sensor->DHT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD; // Открытый сток
    GPIO_InitStruct.Pull = sensor->pullUp;      // Подтяжка к питанию

    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; // Высокая скорость работы порта
    HAL_GPIO_Init(sensor->DHT_Port, &GPIO_InitStruct);
}

static void goToInput(DHT_sensor *sensor)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Настройка порта на вход
    GPIO_InitStruct.Pin = sensor->DHT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = sensor->pullUp; // Подтяжка к питанию
    HAL_GPIO_Init(sensor->DHT_Port, &GPIO_InitStruct);
}

DHT_data DHT_getData(DHT_sensor *sensor)
{
    DHT_data data;

    data.hum = 40.0f;
    data.temp = 20.0f;

#if DHT_POLLING_CONTROL == 1
    /* Ограничение по частоте опроса датчика */
    // Определение интервала опроса в зависимости от датчика
    uint16_t pollingInterval;
    if (sensor->type == DHT11)
    {
        pollingInterval = DHT_POLLING_INTERVAL_DHT11;
    }
    else
    {
        pollingInterval = DHT_POLLING_INTERVAL_DHT22;
    }

    // Если интервал маленький, то возврат последнего удачного значения
    if ((HAL_GetTick() - sensor->lastPollingTime < pollingInterval) && sensor->lastPollingTime != 0)
    {
        data.hum = sensor->lastHum;
        data.temp = sensor->lastTemp;
        return data;
    }
    sensor->lastPollingTime = HAL_GetTick() + 1;
#endif

    /* Запрос данных у датчика */
    // Перевод пина "на выход"
    goToOutput(sensor);
    // Опускание линии данных на 18 мс
    lineDown();
    Delay(18);
    // Подъём линии, перевод порта "на вход"
    lineUp();
    goToInput(sensor);

#ifdef DHT_IRQ_CONTROL
    // Выключение прерываний, чтобы ничто не мешало обработке данных
    __disable_irq();
#endif
    /* Ожидание ответа от датчика */
    uint16_t timeout = 0;
    // Ожидание спада
    while (getLine())
    {
        timeout++;
        if (timeout > DHT_TIMEOUT)
        {
#ifdef DHT_IRQ_CONTROL
            __enable_irq();
#endif
            data.hum = sensor->lastHum;
            data.temp = sensor->lastTemp;
            return data;
        }
    }
    timeout = 0;
    // Ожидание подъёма// 翻译：等待上升
    while (!getLine())
    {
        timeout++;
        if (timeout > DHT_TIMEOUT)
        {
#ifdef DHT_IRQ_CONTROL
            __enable_irq();
#endif
            data.hum = sensor->lastHum;
            data.temp = sensor->lastTemp;
            return data;
        }
    }
    timeout = 0;
    // Ожидание спада
    while (getLine())
    {
        timeout++;
        if (timeout > DHT_TIMEOUT)
        {
#ifdef DHT_IRQ_CONTROL
            __enable_irq();
#endif
            data.hum = sensor->lastHum;
            data.temp = sensor->lastTemp;
            return data;
        }
    }

    /* Чтение ответа от датчика */ // 翻译：读取传感器的响应
    uint8_t rawData[5] = {0, 0, 0, 0, 0};
    for (uint8_t a = 0; a < 5; a++)
    {
        for (uint8_t b = 7; b != 255; b--)
        {
            uint16_t hT = 0, lT = 0;
            // Пока линия в низком уровне, инкремент переменной lT 翻译：当线路在低电平时，增加lT变量
            while (!getLine() && lT != 65535)
                lT++;
            // Пока линия в высоком уровне, инкремент переменной hT 翻译：当线路在高电平时，增加hT变量
            timeout = 0;
            while (getLine() && hT != 65535)
                hT++;
            // Если hT больше lT, то пришла единица
            if (hT > lT)
                rawData[a] |= (1 << b);
        }
    }

#ifdef DHT_IRQ_CONTROL
    // Включение прерываний после приёма данных
    __enable_irq();
#endif

    /* Проверка целостности данных */
    if ((uint8_t)(rawData[0] + rawData[1] + rawData[2] + rawData[3]) == rawData[4]) // 翻译：检查数据完整性
    {
        // Если контрольная сумма совпадает, то конвертация и возврат полученных значений 翻译：如果校验和匹配，则转换并返回获得的值
        if (sensor->type == DHT22)
        {
            data.hum = (float)(((uint16_t)rawData[0] << 8) | rawData[1]) * 0.1f;
            // Проверка на отрицательность температуры
            if (!(rawData[2] & (1 << 7)))
            {
                data.temp = (float)(((uint16_t)rawData[2] << 8) | rawData[3]) * 0.1f;
            }
            else
            {
                rawData[2] &= ~(1 << 7);
                data.temp = (float)(((uint16_t)rawData[2] << 8) | rawData[3]) * -0.1f;
            }
        }
        if (sensor->type == DHT11)
        {
            data.hum = (float)rawData[0];
            data.temp = (float)rawData[2];
        }
    }

    sensor->lastHum = data.hum;
    sensor->lastTemp = data.temp;

    return data;
}