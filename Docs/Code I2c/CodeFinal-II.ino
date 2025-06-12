/*   (PET SEMETARY - RAMONES)
Código em desenvolvimento....
  ___________________________________________
 |             MAV II - DIP II               |
 |                 iNovaX                    |
 |André_Monção - Julio_Oscar - Rafael_Xavier |
 |___________________________________________|

Incluir Bibliotecas. Adafruit é a do OLED*/
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "AnimacaoOlhos.h"  // Incluindo a biblioteca de animação dos olhos

// Definindo o pino do display OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDR 0x3C
// Endereço I2C do display OLED SSD1306

/* Definindo os modos de planta. Escolhi em base de quantidade de umidade, 
sendo cacto baixo, suculentas medias, e sambaias altas. A escolha são apenas
para exemplos, não uso real sendo possivel alterações futuramente. */

#define SUCULENTA 0
#define CACTOS 1
#define SAMAMBAIA 2

// Definindo o pino para o sensor de umidade
const int PINO_POTENCIOMETRO = A0; // Simulador não tem o sensor HW-080
int umidade;
int umidadeMinima = 40; // Umidade mínima ideal (valor inicial para suculentas)
int umidadeMaxima = 60; // Umidade máxima ideal (valor inicial para suculentas)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
AnimacaoOlhos animacao(display);  // Instanciando o objeto de animação dos olhos

// Variáveis para botões
const int BOTAO1 = 2;
const int BOTAO2 = 3;
const int BOTAO3 = 4;
int modoAtual = SUCULENTA; // 0: suculenta, 1: cactos, 2: samambaia
bool selecionandoModo = false;
unsigned long tempoTela = 0;

// Variáveis para controle de tempo não bloqueante
unsigned long tempoAnterior = 0;
const unsigned long intervaloAnimacao = 500; // Intervalo para animação (500ms)

void setup() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true); // Loop infinito caso o display falhe
  }

  // Usei 3 botões pois tive dificuldade com 2 botões, na hora de trocar o modo ele bugava.
  pinMode(BOTAO1, INPUT_PULLUP); // Mostra a % do sensor
  pinMode(BOTAO2, INPUT_PULLUP); // Mostra o Modo Atual
  pinMode(BOTAO3, INPUT_PULLUP); // Troca o Modo da planta.

  // Inicializando os olhos
  display.clearDisplay(); // Limpa o display
  display.setTextSize(1); // Reduzindo o tamanho da fonte
  display.setTextColor(WHITE); // White aqui significa que o LED vai acender
  display.display(); // Exibe o display
  animacao.expressaoBase(); // Exibe a expressão base
  atualizarFaixaUmidade(); // Inicializa as faixas de umidade de acordo com modo atual
}

///////////////////////////////VOID LOOP///////////////////////////////////////////////

void loop() {
  // Escolhi usar millis para que os botões sejam instantâneos e não tenham que esperar o fim do delay
  static unsigned long tempoAnterior = 0; // Armazena o tempo da última execução
  static int etapa = 0; // Controla a etapa atual da animação
  static int angulos[3]; // Armazena os ângulos das pupilas
  static int indiceAngulo = 0; // Controla o índice do ângulo atual

  unsigned long tempoAtual = millis(); // Tempo atual

  // Ler a umidade do solo
  int leitura = analogRead(PINO_POTENCIOMETRO);
  umidade = map(leitura, 0, 1023, 0, 100);

  // Verificar interações com os botões
  if (digitalRead(BOTAO1) == LOW) {
    exibirFaixaUmidade();
    resetarTempoTela();
    aguardarEntrada();
  }

  if (digitalRead(BOTAO2) == LOW) {
    exibirModoAtual();
    resetarTempoTela();
    aguardarEntrada();
  }

  if (digitalRead(BOTAO3) == LOW) {
    selecionarModo();
  }

  // Lógica das animações e expressões
  // Muito MasterMind usar uma switch case para executar as animações, eu estava com muita dificuldade nessa parte, vlw Otavio...

  switch (etapa) {
    case 0: // Exibir a expressão base
      display.clearDisplay();
      animacao.expressaoBase();
      if (tempoAtual - tempoAnterior >= 2000) { // Espera 2 segundos
        tempoAnterior = tempoAtual;
        etapa = 1; // Passa para a próxima etapa
      }
      break;

    case 1: // Exibir a expressão com base na umidade
      definirExpressao(); // Chama a função que define a expressão com base na umidade
      if (tempoAtual - tempoAnterior >= 2000) { // Espera 2 segundos
        tempoAnterior = tempoAtual;
        etapa = 2; // Passa para a próxima etapa
      }
      break;

    case 2: // Gerar ângulos aleatórios para as pupilas
      for (int i = 0; i < 3; i++) {
        angulos[i] = random(0, 360);
      }
      etapa = 3; // Passa para a próxima etapa
      break;

    case 3: // Mover as pupilas para os ângulos gerados
      if (indiceAngulo < 5) {
        if (tempoAtual - tempoAnterior >= 1000) { // Espera 1000ms para cada movimento
          tempoAnterior = tempoAtual;
          animacao.MoverPupilasParaAngulo(angulos[indiceAngulo]);
          indiceAngulo++;
        }
      } else {
        indiceAngulo = 0; // Reinicia o índice dos ângulos
        etapa = 4; // Passa para a próxima etapa
      }
      break;

    case 4: // Piscar os olhos
      animacao.piscar();
      if (tempoAtual - tempoAnterior >= 1000) { // Espera 1000ms após piscar
        tempoAnterior = tempoAtual;
        etapa = 0; // Volta para a primeira etapa (expressão base)
      }
      break;
  }
}

/* Em resumo o Switch e Case são usados para dividir a animação em etapas. Cada case representa uma etapa da animação
(expressão base, expressão com base da umidade, piscar, etc...) 
 O tempo de Cada etapa é controlado pelo "millis" o que garante que o programa não fique bloqueado como seria com "delay".
Após a última etapa o ciclo reinicia, criando uma animação contínua :D
 Eu tentei usar Interrupt e Flags para os botões mas travava as animações.*/

///////////////////////////////////FIM VOID LOOP//////////////////////////////////////

// Função para atualizar a faixa de umidade conforme modoAtual
void atualizarFaixaUmidade() {
  if (modoAtual == SUCULENTA) {
    umidadeMinima = 40;
    umidadeMaxima = 60;
  } else if (modoAtual == CACTOS) {
    umidadeMinima = 10;
    umidadeMaxima = 20;
  } else if (modoAtual == SAMAMBAIA) {
    umidadeMinima = 40;
    umidadeMaxima = 70;
  }
}

// Função para definir a expressão com base na umidade
void definirExpressao() {
  if (umidade < umidadeMinima) {
    animacao.expressaoTriste(); // Exibe expressão triste se a umidade estiver baixa
  } else if (umidade >= umidadeMinima && umidade <= umidadeMaxima) {
    animacao.expressaoFeliz(); // Exibe expressão feliz se a umidade estiver ideal
  } else {
    animacao.expressaoCansado(); // Exibe expressão cansada se a umidade estiver alta
  }
  display.display(); // Atualiza o display com a expressão
}

//////////////////////////VOIDS BOTOES///////////////////////////////////////////////////

void exibirFaixaUmidade() {
  int leitura = analogRead(PINO_POTENCIOMETRO);
  int porcentagem = map(leitura, 1023, 0, 0, 100);

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(10, 5);
  display.print("Umidade Atual: ");
  display.setCursor(10, 20);
  display.setTextSize(2);
  display.print(porcentagem);
  display.print("%");

  display.setTextSize(1);
  display.setCursor(10, 40);
  display.print("Recomendado: ");
  display.setCursor(10, 55);

  if (modoAtual == SUCULENTA) {
    display.print("40 a 60%"); // Faixa recomendada para suculentas
  } else if (modoAtual == CACTOS) {
    display.print("10 a 20%"); // Faixa recomendada para cactos
  } else {
    display.print("40 a 70%"); // Faixa recomendada para samambaias
  }
  display.display();
}

void exibirModoAtual() {
  display.clearDisplay();
  display.setCursor(30, 25);
  if (modoAtual == SUCULENTA) {
    display.print("Suculenta");
  } else if (modoAtual == CACTOS) {
    display.print("Cactos");
  } else {
    display.print("Samambaia");
  }
  display.display();
}

void selecionarModo() {
  selecionandoModo = true;
  resetarTempoTela(); // Para não ficar travado na tela esperando uma ação
  while (millis() - tempoTela < 3000) {
    display.clearDisplay();
    display.setCursor(30, 25);
    if (modoAtual == SUCULENTA) {
      display.print("Suculenta");
    } else if (modoAtual == CACTOS) {
      display.print("Cactos");
    } else {
      display.print("Samambaia");
    }
    display.display();

    if (digitalRead(BOTAO3) == LOW) {
      alternarModo();
      resetarTempoTela(); // Antes dessa opção ele estava somando o tempo ao apertar os botões.
      delay(50);
      display.clearDisplay();
      delay(50);
    }
    if (digitalRead(BOTAO2) == LOW) {
      confirmarModo();
      return;
    }
  }
  confirmarModo();
}

void alternarModo() {
  if (modoAtual == SUCULENTA) {
    modoAtual = CACTOS;
  } else if (modoAtual == CACTOS) {
    modoAtual = SAMAMBAIA;
  } else {
    modoAtual = SUCULENTA;
  }
  atualizarFaixaUmidade();
  exibirModoAtual();
}

void confirmarModo() {
  display.clearDisplay();
  display.setCursor(40, 25);
  display.print("OK");
  display.display();
  delay(1000);
  animacao.expressaoBase();
}

void resetarTempoTela() {
  tempoTela = millis();
}

/* A função abaixo aguarda 5 segundos por uma interação do usuário. Se o BOTAO2 for pressionado, exibe o 
modo atual e aguarda outra interação por 5 segundos. Se o BOTAO1 ou BOTAO3 for pressionado durante esse tempo, 
executa ações específicas e encerra a função. E se nenhum botão for pressionado, volta para a tela inicial após 5 segundos.
Ela é necessária para voltar a tela inicial */
void aguardarEntrada() {
  while (millis() - tempoTela < 3000) {
    if (digitalRead(BOTAO2) == LOW) {
      exibirModoAtual();
      resetarTempoTela();
      while (millis() - tempoTela < 3000) {
        if (digitalRead(BOTAO1) == LOW) {
          animacao.expressaoBase();
          return;
        }
        if (digitalRead(BOTAO3) == LOW) {
          selecionarModo();
          return;
        }
      }
      animacao.expressaoBase();
      return;
    }
  }
  animacao.expressaoBase();
}

// Este código ainda está sujeito a alterações!!! Irei deixar os artigos de referências em outra pasta para consultas.

