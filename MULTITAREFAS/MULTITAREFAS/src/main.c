/**
 * \file
 *
 * \brief Exemplos diversos de tarefas e funcionalidades de um sistema operacional multitarefas.
 *
 */

/**
 * \mainpage Sistema operacional multitarefas
 *
 * \par Exemplso de tarefas
 *
 * Este arquivo contem exemplos diversos de tarefas e 
 * funcionalidades de um sistema operacional multitarefas.
 *
 *
 * \par Conteudo
 *
 * -# Inclui funcoes do sistema multitarefas (atraves de multitarefas.h)
 * -# Inicialização do processador e do sistema multitarefas
 * -# Criação de tarefas de demonstração
 *
 */

/*
 * Inclusao de arquivos de cabecalhos
 */
#include <asf.h>
#include "stdint.h"
#include "multitarefas.h"

/*
 * Prototipos das tarefas
 */

void tarefa_1(void);
void tarefa_2(void);
void tarefa_3(void);
void tarefa_4(void);
void tarefa_5(void);
void tarefa_6(void);
void configure_extint_channel(void);void configure_extint_callbacks(void);
void extint_detection_callback(void);

/*
 * Configuracao dos tamanhos das pilhas
 */
#define TAM_PILHA_1			(TAM_MINIMO_PILHA + 24)
#define TAM_PILHA_2			(TAM_MINIMO_PILHA + 24)
#define TAM_PILHA_3			(TAM_MINIMO_PILHA + 24)
#define TAM_PILHA_4			(TAM_MINIMO_PILHA + 24)
#define TAM_PILHA_5			(TAM_MINIMO_PILHA + 24)
#define TAM_PILHA_6			(TAM_MINIMO_PILHA + 24)

#define TAM_PILHA_OCIOSA	(TAM_MINIMO_PILHA + 24)

/*
 * Declaracao das pilhas das tarefas
 */

uint32_t PILHA_TAREFA_1[TAM_PILHA_1];
uint32_t PILHA_TAREFA_2[TAM_PILHA_2];
uint32_t PILHA_TAREFA_3[TAM_PILHA_3];
uint32_t PILHA_TAREFA_4[TAM_PILHA_4];
uint32_t PILHA_TAREFA_5[TAM_PILHA_5];
uint32_t PILHA_TAREFA_6[TAM_PILHA_6];

uint32_t PILHA_TAREFA_OCIOSA[TAM_PILHA_OCIOSA];

uint32_t soma_contadores = 0, a=0, b=0, c=0, d=0, e=0;

uint32_t timer_irq=0;

/*
 * Funcao principal de entrada do sistema
 */
int main(void)
{
	system_init();
	
	configure_extint_channel();
	
	configure_extint_callbacks();
	
	system_interrupt_enable_global();
	
	/* Criacao das tarefas */
	/* Parametros: ponteiro, nome, ponteiro da pilha, tamanho da pilha, prioridade da tarefa */
		
	CriaTarefa(tarefa_1, "Tarefa 1", PILHA_TAREFA_1, TAM_PILHA_1, 6);
	
	CriaTarefa(tarefa_2, "Tarefa 2", PILHA_TAREFA_2, TAM_PILHA_2, 5);
	
	CriaTarefa(tarefa_3, "Tarefa 3", PILHA_TAREFA_3, TAM_PILHA_3, 4);

	CriaTarefa(tarefa_4, "Tarefa 4", PILHA_TAREFA_4, TAM_PILHA_4, 3);
	
	CriaTarefa(tarefa_5, "Tarefa 5", PILHA_TAREFA_5, TAM_PILHA_5, 2);
	
	CriaTarefa(tarefa_6, "Tarefa 6", PILHA_TAREFA_6, TAM_PILHA_6, 1);
	
	
	/* Cria tarefa ociosa do sistema */
	CriaTarefa(tarefa_ociosa,"Tarefa ociosa", PILHA_TAREFA_OCIOSA, TAM_PILHA_OCIOSA, 0);

	/* Configura marca de tempo */
	ConfiguraMarcaTempo();   
	
	/* Inicia sistema multitarefas */
	IniciaMultitarefas();
	
	/* Nunca chega aqui */
	while (1)
	{
	}
}

/* Tarefas de exemplo que usam funcoes para suspender/continuar as tarefas */

void tarefa_1(void)
{
	for(;;)
	{
		/*
		static char buffer_msg[50]; 
		sprintf(buffer_msg, "Número de trocas de contexto: %ld \n", a+b+c+d+e);
		*/
		soma_contadores=a+b+c+d+e;
		TarefaEspera(1000);
	}
}

void tarefa_2(void)
{
	for(;;)
	{
		a++;
		TarefaSuspende(2);
	}
}

void tarefa_3(void)
{
	for(;;)
	{
		b++;
		TarefaContinua(2);	
		b++;
		TarefaSuspende(3);
	}
}

void tarefa_4(void)
{
	for(;;)
	{
		c++;
		TarefaContinua(3);
		c++;
		TarefaSuspende(4);
	}
}

void tarefa_5(void)
{
	for(;;)
	{
		d++;
		TarefaContinua(4);
		d++;
		TarefaSuspende(5);
	}
}

void tarefa_6(void)
{
	for(;;)
	{	
		e++; 
		TarefaContinua(5);
	}
}


void configure_extint_channel(void)
{
	struct extint_chan_conf config_extint_chan;
	extint_chan_get_config_defaults(&config_extint_chan);
	config_extint_chan.gpio_pin = BUTTON_0_EIC_PIN;
	config_extint_chan.filter_input_signal = true;
	config_extint_chan.gpio_pin_mux = BUTTON_0_EIC_MUX;
	config_extint_chan.gpio_pin_pull = EXTINT_PULL_UP;
	config_extint_chan.detection_criteria = EXTINT_DETECT_FALLING;
	extint_chan_set_config(BUTTON_0_EIC_LINE, &config_extint_chan);
}void configure_extint_callbacks(void)
{
	extint_register_callback(extint_detection_callback,
	BUTTON_0_EIC_LINE,
	EXTINT_CALLBACK_TYPE_DETECT);
	extint_chan_enable_callback(BUTTON_0_EIC_LINE,
	EXTINT_CALLBACK_TYPE_DETECT);
}

void extint_detection_callback(void)
{
	static bool pin_state = 0; // port_pin_get_input_level(LED_0_PIN);
	pin_state = !pin_state;
	port_pin_set_output_level(LED_0_PIN, pin_state);
 }