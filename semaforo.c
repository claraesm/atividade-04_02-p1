// Inclusão das bibliotecas necessárias
#include <stdio.h>
#include "pico/stdlib.h" 
#include "hardware/timer.h" 
#include "hardware/pio.h"

// Definição dos pinos para os LEDs
#define LED_VERMELHO 11
#define LED_AMARELO 12
#define LED_VERDE 13

// Enumeração para representar os estados do semáforo
typedef enum {
    VERMELHO,
    AMARELO, 
    VERDE
} Estado;

// Variável global para armazenar o estado atual do semáforo
Estado estadoAtual = VERMELHO;

// Estrutura para o temporizador
struct repeating_timer temporizador;

// Função para configurar os LEDs
void configurar_leds() {
    gpio_init(LED_VERMELHO);
    gpio_set_dir(LED_VERMELHO, GPIO_OUT);
    gpio_init(LED_AMARELO);
    gpio_set_dir(LED_AMARELO, GPIO_OUT);
    gpio_init(LED_VERDE);
    gpio_set_dir(LED_VERDE, GPIO_OUT);
    
    // Iniciar com o LED vermelho aceso
    gpio_put(LED_VERMELHO, 1);
}

// Função para exibir uma mensagem de atenção
void exibir_mensagem() {
    printf("Fique atento ao semáforo!\n");
    sleep_ms(1000);
}

// Função de callback para alterar os estados do semáforo
bool alterar_estado(struct repeating_timer *t) {
    // Desligar todos os LEDs antes de mudar para o próximo estado
    gpio_put(LED_VERMELHO, 0);
    gpio_put(LED_AMARELO, 0);
    gpio_put(LED_VERDE, 0);

    // Alternar entre os estados do semáforo
    switch (estadoAtual) {
        case VERMELHO:
            estadoAtual = AMARELO;
            gpio_put(LED_AMARELO, 1);
            break;
        case AMARELO:
            estadoAtual = VERDE;
            gpio_put(LED_VERDE, 1);
            break;
        case VERDE:
            estadoAtual = VERMELHO;
            gpio_put(LED_VERMELHO, 1);
            break;
    }
    
    return true; // Mantém o temporizador em execução
}

int main() {
    // Inicializar a comunicação serial e os LEDs
    stdio_init_all();
    configurar_leds();

    // Configurar temporizador para trocar o estado a cada 3 segundos
    add_repeating_timer_ms(3000, alterar_estado, NULL, &temporizador);

    // Loop infinito exibindo a mensagem de atenção
    while (true) {
        exibir_mensagem();
    }
    return 0;
}
