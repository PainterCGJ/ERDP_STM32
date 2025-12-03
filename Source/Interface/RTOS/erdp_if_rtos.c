#include "erdp_if_rtos.h"
// #include "systick.h"

#define List_Index_Loop(item) for (OS_ListItem *__end = (OS_ListItem *)listGET_END_MARKER(item->pxContainer); item != __end; item = item->pxNext)

OS_TaskHandle erdp_if_rtos_task_create(void (*task_function)(void *),
                                       const char *name, uint32_t stack_size,
                                       void *arg, uint32_t priority)
{
    TaskHandle_t xHandle = NULL;
    xTaskCreate(task_function, name, stack_size, arg, priority, &xHandle);
    return (OS_TaskHandle)xHandle;
}

void erdp_if_rtos_task_delete(OS_TaskHandle task_handle)
{
    if (task_handle != NULL)
    {
        vTaskDelete(task_handle);
    }
    else
    {
        vTaskDelete(NULL);
    }
}

void erdp_if_rtos_task_suspend(OS_TaskHandle task_handle)
{
    if (task_handle != NULL)
    {
        vTaskSuspend(task_handle);
    }
}

void erdp_if_rtos_task_resume(OS_TaskHandle task_handle)
{
    if (task_handle != NULL)
    {
        vTaskResume(task_handle);
    }
}

void erdp_if_rtos_start_scheduler()
{
    vTaskStartScheduler();
}

uint32_t erdp_if_rtos_ms_to_ticks(uint32_t nms)
{
    return pdMS_TO_TICKS(nms);
}

void erdp_if_rtos_delay_ms(uint32_t nms)
{
    vTaskDelay(pdMS_TO_TICKS(nms));
}

uint32_t erdp_if_rtos_get_1ms_timestamp(void)
{
    return xTaskGetTickCount()*portTICK_PERIOD_MS;
}

uint32_t erdp_if_rtos_get_system_heap_size(void)
{
    return xPortGetFreeHeapSize();
}

void *erdp_if_rtos_malloc(size_t size)
{
    return pvPortMalloc(size);
}

void erdp_if_rtos_free(void *pv)
{
    vPortFree(pv);
}

OS_Queue erdp_if_rtos_queue_create(uint32_t queue_length, uint32_t item_size)
{
    return xQueueCreate(queue_length, item_size);
}

bool erdp_if_rtos_queue_recv(OS_Queue os_queue, uint8_t *pxdata, uint32_t ticks_to_wait)
{
    if (xPortIsInsideInterrupt() == pdTRUE)
    {
        bool os_sta;
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        os_sta = (bool)xQueueReceiveFromISR(os_queue, pxdata, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        return os_sta;
    }
    return (bool)xQueueReceive(os_queue, pxdata, ticks_to_wait);
}

bool erdp_if_rtos_queue_send(OS_Queue os_queue, uint8_t *pxdata, uint32_t ticks_to_wait)
{
    if (xPortIsInsideInterrupt() == pdTRUE)
    {
        bool os_sta;
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        os_sta = (bool)xQueueSendFromISR(os_queue, pxdata, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        return os_sta;
    }
    return (bool)xQueueSend(os_queue, pxdata, ticks_to_wait);
}

bool erdp_if_rtos_queue_overwrite(OS_Queue os_queue, uint8_t *pxdata)
{
    if (xPortIsInsideInterrupt() == pdTRUE)
    {
        bool os_sta;
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        os_sta = (bool)xQueueOverwriteFromISR(os_queue, pxdata, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        return os_sta;
    }
    return (bool)xQueueOverwrite(os_queue, pxdata);
}

void erdp_if_rtos_queue_index(OS_Queue os_queue, uint32_t index_base_tail)
{
}

void erdp_if_rtos_queue_delet(OS_Queue os_queue)
{
    vQueueDelete(os_queue);
}

OS_List erdp_if_rtos_list_create(void)
{
    OS_List list = (OS_List)erdp_if_rtos_malloc(sizeof(List_t));
    if (list == NULL)
    {
        return NULL;
    }
    vListInitialise(list);
    return list;
}

void erdp_if_rtos_list_item_init(OS_ListItem *pxItem, void *pxOwner)
{
    vListInitialiseItem(pxItem);
    listSET_LIST_ITEM_OWNER(pxItem, pxOwner);
}

void os_list_insert_end(OS_List list, OS_ListItem *pxItem)
{
    vListInsertEnd(list, pxItem);
}

void erdp_if_rtos_list_printf(OS_List list)
{
    OS_ListItem *pxItem = (OS_ListItem *)(list->xListEnd.pxNext);
    printf("\r\n================================\r\n");
    printf("List:               0x%p\r\n", list);
    printf("Number of item:     %u\r\n", (uint32_t)list->uxNumberOfItems);
    printf("xListEnd:           0x%p\r\n", &list->xListEnd);
    printf("xListEnd->pxNext:   0x%p\r\n", list->xListEnd.pxNext);

    uint8_t i = 0;
    // while (pxItem != (OS_ListItem *)(&list->xListEnd))
    List_Index_Loop(pxItem)
    {
        printf("\r\n--------------------------------\r\n");
        printf("order:          [%d]\r\n", i++);
        printf("pxItem:         0x%p\r\n", pxItem);
        printf("xItemValue:     %d\r\n", pxItem->xItemValue);
        printf("pxPrevious:     0x%p\r\n", pxItem->pxPrevious);
        printf("pxContainer:    0x%p\r\n", pxItem->pxContainer);
        printf("pvOwner:        0x%p\r\n", pxItem->pvOwner);
        printf("pxNext:         0x%p\r\n", pxItem->pxNext);
        // pxItem = pxItem->pxNext;
    }
    printf("\r\n================================\r\n");
}

uint32_t erdp_if_rtos_list_remove_item(OS_ListItem *pxItem)
{
    return uxListRemove(pxItem);
}

void erdp_if_rtos_delet_list_item(OS_ListItem *pxItem, bool free_owner)
{
    if (pxItem->pxContainer != NULL)
    {
        /*this list item still in the list*/
        erdp_if_rtos_list_remove_item(pxItem);
    }

    if (free_owner)
    {
        erdp_if_rtos_free(pxItem->pvOwner);
    }

    erdp_if_rtos_free(pxItem);
}

void erdp_if_rtos_delet_list(OS_List list, bool free_item, bool free_owner)
{
    if (free_item)
    {
        OS_ListItem *pxItem = list->xListEnd.pxNext;
        OS_ListItem *pxNext;
        while ((void *)pxItem != (void *)(&list->xListEnd))
        {
            pxNext = pxItem->pxNext;
            erdp_if_rtos_delet_list_item(pxItem, free_owner);
            pxItem = pxNext;
        }
    }
    erdp_if_rtos_free(list);
}

OS_Event erdp_if_rtos_event_create(void)
{
    return xEventGroupCreate();
}

OS_EventBits erdp_if_rtos_set_event_bits(OS_Event event, OS_EventBits bits_to_set)
{
    if (xPortIsInsideInterrupt() == pdTRUE)
    {
        OS_EventBits event_bits;
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        event_bits = (bool)xEventGroupSetBitsFromISR(event, bits_to_set, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        return event_bits;
    }
    return (OS_EventBits)xEventGroupSetBits(event, bits_to_set);
}

OS_EventBits erdp_if_rtos_clear_event_bits(OS_Event event, OS_EventBits bits_to_clear)
{
    if (xPortIsInsideInterrupt() == pdTRUE)
    {
        return (OS_EventBits)xEventGroupClearBitsFromISR(event, bits_to_clear);
    }
    return (OS_EventBits)xEventGroupClearBits(event, bits_to_clear);
}

OS_EventBits erdp_if_rtos_get_event_bits(OS_Event event)
{
    if (xPortIsInsideInterrupt() == pdTRUE)
    {
        OS_EventBits event_bits;
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        event_bits = (OS_EventBits)xEventGroupGetBitsFromISR(event);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        return event_bits;
    }
    return (OS_EventBits)xEventGroupGetBits(event);
}

OS_EventBits erdp_if_rtos_event_sync(OS_Event event, const OS_EventBits bits_to_set, const OS_EventBits bits_wait_for, uint32_t ticks_to_wait)
{
    if (xPortIsInsideInterrupt() == pdFAIL)
    {
        return xEventGroupSync(event, bits_to_set, bits_wait_for, ticks_to_wait);
    }
    return 0;
}

OS_Semaphore erdp_if_rtos_semaphore_creat(Semaphore_tag tag)
{
    switch (tag)
    {
    case BINARY_TAG:
    {
        return xSemaphoreCreateBinary();
    }
    case MUTEX_TAG:
    {
        return xSemaphoreCreateMutex();
    }
    case RECURISIVE_TAG:
    {
        return xSemaphoreCreateRecursiveMutex();
    }
    default:break;
    }
    return NULL;
}

OS_Semaphore erdp_if_rtos_counting_semaphore_creat(uint32_t max_count, uint32_t initial_count)
{
    return xSemaphoreCreateCounting(max_count, initial_count);
}

bool erdp_if_rtos_semaphore_take(OS_Semaphore semaphore, uint32_t ticks_to_wait)
{
    if (xPortIsInsideInterrupt() == pdTRUE)
    {
        bool os_sta;
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        os_sta = (bool)xSemaphoreTakeFromISR(semaphore, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        return os_sta;
    }
    return (bool)xSemaphoreTake(semaphore, ticks_to_wait);
}

bool erdp_if_rtos_semaphore_give(OS_Semaphore semaphore)
{
    if (xPortIsInsideInterrupt() == pdTRUE)
    {
        bool os_sta;
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        os_sta = (bool)xSemaphoreGiveFromISR(semaphore, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        return os_sta;
    }
    return (bool)xSemaphoreGive(semaphore);
}

bool erdp_if_rtos_recursive_semaphore_take(OS_Semaphore semaphore, uint32_t ticks_to_wait)
{
    return (bool)xSemaphoreTakeRecursive(semaphore, ticks_to_wait);
}

bool erdp_if_rtos_recursive_semaphore_give(OS_Semaphore semaphore)
{
    return (bool)xSemaphoreGiveRecursive(semaphore);
}

void erdp_if_rtos_semaphore_delet(OS_Semaphore semaphore)
{
    vSemaphoreDelete(semaphore);
}

uint32_t erdp_if_rtos_cpu_lock(void)
{
    if (xPortIsInsideInterrupt() == pdTRUE)
    {
        return taskENTER_CRITICAL_FROM_ISR();
    }
    else
    {
        taskENTER_CRITICAL();
    }
    return 0;
}

void erdp_if_rtos_cpu_unlock(uint32_t key)
{
    if (xPortIsInsideInterrupt() == pdTRUE)
    {
        taskEXIT_CRITICAL_FROM_ISR(key);
    }
    else
    {
        taskEXIT_CRITICAL();
    }
}

void erdp_if_rtos_system_config(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	SysTick_Config(SystemCoreClock / 1000);  //≈‰÷√SysTick ±÷”Œ™1ms÷–∂œ
    // systick_config();
}
/* end of file */
