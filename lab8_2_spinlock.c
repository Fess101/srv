#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/neutrino.h>
#include <sys/syspage.h>

// Объявляем спин-лок
static pthread_spinlock_t spinlock;

// Функция потока, которая использует спин-лок
void* thread_func(void* arg) {
    int thread_num = *(int*)arg;
    
    printf("Поток %d пытается захватить спин-лок...\n", thread_num);
    
    // Захватываем спин-лок
    pthread_spin_lock(&spinlock);
    
    printf("Поток %d захватил спин-лок. Работает...\n", thread_num);
    
    // Имитация работы
    delay(500); // Задержка 500 мс
    
    printf("Поток %d освобождает спин-лок.\n", thread_num);
    
    // Освобождаем спин-лок
    pthread_spin_unlock(&spinlock);
    
printf("Sergeev Vladislav Vladimirovich i914B\n");
    return 1;
}